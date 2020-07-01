#include "configurewindow.h"
#include "types.h"
#include "ui_configurewindow.h"
#include <QtSerialPort/QSerialPort>

ConfigureWindow::ConfigureWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConfigureWindow)
{
    ui->setupUi(this);

    //wpisywanie domyślnych watości konfiguracji portu
    QStringList slDataBits = (QStringList() << "5" << "6" << "7" << "8");
    ui->cbDataBits->addItems(slDataBits);
    ui->cbDataBits->setCurrentIndex(3);

    QStringList slBaudRate = (QStringList() << "1200" << "2400" << "4800" << "9600" << "19200" << "38400" << "57600" << "115200");
    ui->cbBaudRate->addItems(slBaudRate);
    ui->cbBaudRate->setCurrentIndex(7);

    QStringList slParity = (QStringList() << "NoParity" << "EvenParity" << "OddParity");
    ui->cbParity->addItems(slParity );
    ui->cbParity->setCurrentIndex(0);

    QStringList slStopBits= (QStringList() << "OneStop" << "OneAndHalfStop" << "TwoStop");
    ui->cbStopBits->addItems(slStopBits);
    ui->cbStopBits->setCurrentIndex(0);

    this->setWindowModality(Qt::ApplicationModal);

}

void ConfigureWindow::showEvent(QShowEvent *ev)
{
    //dodanie dostępnych portów com do listy
    ui->cbSerialPort->clear();
    portList.clear();
    serialList.clear();
    bool found = false;

    foreach (const QSerialPortInfo &infoport, QSerialPortInfo::availablePorts())
    {
        if(infoport.serialNumber().startsWith("V1"))
        {
            ui->cbSerialPort->addItem(infoport.portName()+": "+infoport.serialNumber());
            portList.append(infoport.portName());
            serialList.append(infoport.serialNumber());
            found = true;
        }
    }

    ui->bApply->setEnabled(found);
    UNUSED(*ev);
}

ConfigureWindow::~ConfigureWindow()
{
    delete ui;
}

void ConfigureWindow::on_bRefreshSerialPort_clicked()
{
    ui->cbSerialPort->clear();
    portList.clear();
    serialList.clear();
    bool found = false;
    //dodanie dostępnych portów com do listy
    foreach (const QSerialPortInfo &infoport, QSerialPortInfo::availablePorts())
    {
        if(infoport.serialNumber().startsWith("V1"))
        {
            ui->cbSerialPort->addItem(infoport.portName()+": "+infoport.serialNumber());
            portList.append(infoport.portName());
            serialList.append(infoport.serialNumber());
            found = true;
        }
    }

    ui->bApply->setEnabled(found);
}

void ConfigureWindow::on_bClose_clicked()
{
    this->close();
}

void ConfigureWindow::on_bApply_clicked()
{
    qint32 qiBaudRate;
    QSerialPort::StopBits spStopBits;
    QSerialPort::DataBits spDataBits;
    QSerialPort::Parity spParity;

    //konfiguracja baudrate
    qiBaudRate = static_cast<qint32>(ui->cbBaudRate->currentText().toInt());
    serial.qiBaudRate = qiBaudRate;

    //konfiguracja stopbits
    switch(ui->cbStopBits->currentIndex()) {
        case 0:
            spStopBits = QSerialPort::OneStop;
            break;
        case 1:
            spStopBits = QSerialPort::OneAndHalfStop;
            break;
        case 2:
            spStopBits = QSerialPort::TwoStop;
            break;
    default:
            spStopBits = QSerialPort::OneStop;
    }
    serial.spStopBits = spStopBits;

    //konfiguracja DataBits
    spDataBits = static_cast<QSerialPort::DataBits>(ui->cbDataBits->currentText().toInt());
    serial.spDataBits = spDataBits;

    //Konfiguracja Parity
    switch(ui->cbParity->currentIndex()) {
        case 0:
            spParity = QSerialPort::NoParity;
            break;
        case 1:
            spParity = QSerialPort::EvenParity;
            break;
        case 2:
            spParity = QSerialPort::OddParity;
            break;
    default:
        spParity = QSerialPort::NoParity;
    }
    serial.spParity = spParity;

        /*
    if(ui->cbParity->currentText().contains("NoParity",Qt::CaseInsensitive))
        spParity = QSerialPort::NoParity;
    else if(ui->cbParity->currentText().contains("EvenParity",Qt::CaseInsensitive))
        spParity = QSerialPort::EvenParity;
    else if(ui->cbParity->currentText().contains("OddParity",Qt::CaseInsensitive))
        spParity = QSerialPort::EvenParity;

        */

    if(portList.size()>0)
    {
        serial.sPortName = portList.at(ui->cbSerialPort->currentIndex());
        serial.serialNumber = serialList.at(ui->cbSerialPort->currentIndex());
    }

    //przesłanie struktury do aplikacji
    emit ConfigureResponse(serial);
    this->close();
}

void ConfigureWindow::on_checkBox_toggled(bool checked)
{
    if(checked)
    {
        ui->cbDataBits->setEnabled(false);
        ui->cbBaudRate->setEnabled(false);
        ui->cbParity->setEnabled(false);
        ui->cbStopBits->setEnabled(false);
    } else
    {
        ui->cbDataBits->setEnabled(true);
        ui->cbBaudRate->setEnabled(true);
        ui->cbParity->setEnabled(true);
        ui->cbStopBits->setEnabled(true);
    }
}
