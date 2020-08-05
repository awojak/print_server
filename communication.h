#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <QObject>
#include <QtSerialPort>
#include "callback.h"
#include "iserialcommunication.h"

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

class Communication : public ISerialCommunication, public QObject
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
    bool isConfigured();
    void SendData(TData tx);

    virtual void sendByte(char c);
    virtual int transmitSpace();

    QString getSerialID();

    /**
     * Associates an data ready action with the Communiation class. The action is performed when the data is recived
     *
     * @param  callback The callback to be executed. The callback will be executed with a
     *                  reference to the Communication.
     *
     * @see GenericCallback, handleClickEvent, ClickEvent
     */
    void setDataReadyAction(GenericCallback<const Communication&>& callback)
    {
        dataReady = &callback;
    }

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
protected:
    GenericCallback<const Communication&>* dataReady; ///< The callback to be executed when this AbstractButton is clicked

private slots:
    void ReadData();
    void Timeout();
    void serialError(QSerialPort::SerialPortError error);

signals:
    void frameReady(QByteArray data);
    void communicationError();
};

#endif // COMMUNICATION_H
