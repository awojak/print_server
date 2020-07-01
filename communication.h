#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <QObject>
#include <QtSerialPort>

typedef struct s_data {
    QByteArray data;
} TData;

typedef struct {
    qint32 qiBaudRate;
    QSerialPort::StopBits spStopBits;
    QSerialPort::DataBits spDataBits;
    QSerialPort::Parity spParity;
    QString sPortName;
    QString serialNumber;

} SerialStruct;

class Communication : public QObject
{
    Q_OBJECT

public:
    Communication();
    ~Communication();
    void SetSerialPort(SerialStruct s);
    SerialStruct GetSerialPort();
    int OpenSerialPort();
    int CloseSerialPort();
    bool isConnected();
    void SendData(TData tx);
    QString getSerialID();

private:
    void Transmit();
    QSerialPort *serial;
    SerialStruct serial_struct;
    QByteArray frame;
    QQueue<TData> TransmitQueue;
    bool configured;
    bool connected;
    bool transmitIsGoing;
    QTimer *timeout;

private slots:
    void ReadData();
    void Timeout();
    void serialError(QSerialPort::SerialPortError error);

signals:
    void frameReady(QByteArray data);
    void communicationError();
};

#endif // COMMUNICATION_H
