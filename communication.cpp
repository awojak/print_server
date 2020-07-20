#include "communication.h"
#include <QErrorMessage>

Communication::Communication()
{
    /* Utowrzenie interfejsu serial port */
    serial = new QSerialPort();

    /* Wywołać funkcję w momencie nadejścia sygnału readyRead() */
    connect(serial, SIGNAL(readyRead()), this, SLOT(ReadData()));
    /* Gdy wystąpi błąz połączeniem */
    connect(serial, SIGNAL(error(QSerialPort::SerialPortError)),this, SLOT(serialError(QSerialPort::SerialPortError)));

    timeout = new QTimer(this);
    connect(timeout, SIGNAL(timeout()), this, SLOT(Timeout()));
    timeout->setSingleShot(true);

    /* Jeszcze nie skonfigurowano serial port */
    configured = false;

    /* Jeszcze nie połączono z urządzeniem */
    connected = false;

    /* Przygotowanie kolejki */
    TransmitQueue.clear();
    transmitIsGoing = false;
}

Communication::~Communication()
{
    serial->close();
    delete timeout;
    delete serial;
}

/**
 * @brief Communication::serialError
 *
 * Obsługa błędów komunikacji
 * @param error
 */
void Communication::serialError(QSerialPort::SerialPortError error)
{
    if(error==QSerialPort::ResourceError)
    {
        /* An I/O error occurred when a resource becomes unavailable,
         *  e.g. when the device is unexpectedly removed from the system.
         */
        serial->close();
        serial->clearError();
        connected = false;
        TransmitQueue.clear();
        transmitIsGoing = false;
        qWarning("The device is unexpectedly removed from the system!");
        emit communicationError();
    }
}

QString Communication::getSerialID()
{
    return (serial_struct.sPortName + ": " + serial_struct.serialNumber);
}

bool Communication::isConfigured()
{
    return configured;
}

void Communication::SetSerialPort(SerialStruct s)
{
    /* Sprawdzamy czy struktura zawiera nazwę portu */
    if(!s.sPortName.isEmpty())
    {
        serial_struct = s;
        configured = true;
    } else
    {
        qDebug() << "Skturktura serial port zawiera blad, nie skonfigurowano\n";

    }
}

int Communication::OpenSerialPort()
{
    if(!connected)
    {
        if(configured)
        {

            /* Konfiguracja RS232 przy wykorzystaniu struktury */
            serial->setPortName(serial_struct.sPortName);
            serial->setBaudRate(serial_struct.qiBaudRate);
            serial->setDataBits(serial_struct.spDataBits);
            serial->setParity(serial_struct.spParity);
            serial->setStopBits(serial_struct.spStopBits);

            serial->clearError();

            if (serial->open(QIODevice::ReadWrite))
            {
                connected = true;
                serial->flush();
                qDebug() << "Info: Connected to the device\n";
                return 0;

            } else
            {
                qDebug() << "Error: Nie można otworzyć urządzenia UART: " << serial->error() << "\n";
                return -1;
            }

        } else
        {
            qDebug() << "Error: Not configured serial port\n";
            return -1;
        }

    } else
    {
         qDebug() << "Info: Already connected\n";
         return 0;
    }
}

int Communication::CloseSerialPort()
{
    if(connected)
    {
        serial->close();
        connected = false;
        qDebug() << "Info: Device disconnected\n";
        return 0;
    } else
    {
        return -1;
    }
}

bool Communication::isConnected()
{
    return connected;
}

void Communication::Timeout()
{
    /** @todo Ponowne wysyłanie danych w razie niepowodzenia, w tym momencie dane są tracone */
    qWarning("Timeout during transmit data to device");
    //Wywołaj funkcję Transmit, która sprawdza czy są jeszcze dane do wysłania
    Transmit();
}

void Communication::SendData(TData tx)
{
    //Dodanie danych do kolejki
    if(!tx.data.isEmpty())
        TransmitQueue.enqueue(tx);
    //Jeżeli nie są aktualnie transmitowane żadne dane to aktywuj transmisję
    if(!transmitIsGoing)
    {
        Transmit();
    }
}

void Communication::Transmit()
{
    TData tx;
    if(connected)
    {
        if(!TransmitQueue.isEmpty())
        {
            tx = TransmitQueue.dequeue();
            serial->write(tx.data);
            timeout->start(100);
            //Aktywyj flagę, transmisja jest w trakcie
            transmitIsGoing = true;
        } else {
            //Jeżeli nie ma danych do wysłania transmisja jest zakończona
            transmitIsGoing = false;
        }
    }
}

void Communication::ReadData()
{
    //zmienne stałe do kofiguracji komunikacji
    const char header = static_cast<const char>(0xBC);
    const int frame_size_max = 28;

    QByteArray RxData;
    serial->waitForReadyRead(1);
    RxData = serial->readAll();
    static int state = 1;
    static int frameindex = 0;
    static int framelength = 0;

    /* Recived package */
    //qDebug() << "Recived package: " << QString(RxData.toHex()) << "\n";

    /*  Przeszukujemy dane w poszukiwaniu nagłówka, w celu gdyby
        na początku znalazły się niewiadome dane */
    for(int i=0;i<RxData.length();i++)
    {
        if(state == 3)
        {
            //Odbiór pozostałych danych
            frame.append(RxData.at(i));
            frameindex++;

            if(frameindex >= framelength)
            {
                //odebrano całą ramkę, usunięcie przekopiowanych dannych z bufora
                //TODO co w momencie gdy odbierzemy całą ramkę a w buforze zostaną jeszcze dane z kolejne ramki
                //RxData.remove(0,i+1);
                //Przygotuj funkcję na odczyt nowej ramki
                state = 1;
                //włącz analize danych

                /** @todo poważny problem ! w momencie kiedy w jednym buforze znajdzie się kilka pakietów to
                 * tylko pierwszy pakiet jest odbierany ! Trzeba to naprawić */

                /* Rozwiązanie jest następujące, wysyłamy pakiet który musi zostać dodany do kolejki
                 * i obsłużony przez odpowiednią funkcję */
                emit frameReady(frame);

                //Wymagane jest przerwanie aktualnej pętli bo gdy ostatnia wartość jest równa 0xBC wejdzie to kroku 1
                //Tak to już się zdażyło :)
                continue;
                /* Gdy ramka zostanie zapisana już w kolejce możemy przejść do kroku 1, jeżeli są jeszcze dane do odbioru */
            }
        }

        if(state == 2)
        {
            //pobieramy wielkość ramki
            framelength = RxData.at(i);
            //jeżeli wielkość ramki przekroczy maksymalny lub minimalny rozmiar to odrzucamy dane
            if(framelength>frame_size_max||framelength<=4)
            {
                state = 1;
                 qDebug() << "F001E001: Odczytano niepoprawną wielkość ramki\n";
                //TODO można pomyśleć o przerwaniu funkcji w tym miejscu
            } else {
                //Dodanie rozmiaru ramki do danych
                frame.append(RxData.at(i));
                //ramka zawiera też rozmiar
                frameindex=2;
                state = 3;

            }
        }

        if((RxData.at(i) == header) && (state == 1))
        {
            //dodanie nagłówka do ramki
            frame.clear();
            frame.append(RxData.at(i));
            state = 2;

            if(transmitIsGoing)
            {
                //Wyłącz timeout
                timeout->stop();
                //Odebrano ACK, wyślij kolejną porcję danych jeśli dostępna
                Transmit();
            }
        }
    }
}
