#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"QMessageBox"
#include "min.h"
#include "types.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /* Utwórz okno konfiguracji RS232 */
    configure_window = new ConfigureWindow();
    connect(configure_window,SIGNAL(ConfigureResponse(SerialStruct)),this,SLOT(ConfigureResponse(SerialStruct)));

    /* Utórz interfejs komunikacyjny */
    communication = new Communication();
    connect(communication, SIGNAL(communicationError()), this, SLOT(communicationError()));

    protocol = new MinProtocol(communication, &system, &cmd);

    //buttonCallback(this, &MainViewBase::buttonCallbackHandler);
    //communication.
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::communicationError()
{
    //on_bStop_clicked();
    //ui->actionConnect->setText("Connect");
    //ui->label_conn_status->setText("The device is unexpectedly removed from the system!");
    //ui->bStart->setEnabled(false);
    //ui->actionCalibration->setEnabled(false);
    /* Wyczyść ID bo rozłączono z urządzeniem */
    //ui->label_device_id->setText("");

    QMessageBox::critical(this, tr("Error!"), tr("The device is unexpectedly removed from the system!"));
}

void MainWindow::ConfigureResponse(SerialStruct serial)
{

    if(!communication->isConnected())
    {
        //ui->label_conn_status->setText("Serial port is configured");
        communication->SetSerialPort(serial);
        ///ui->actionConnect->setEnabled(true);
        //Połącz
        //on_actionConnect_triggered();
    }
}

void MainWindow::on_actionCommunication_triggered()
{
    configure_window->show();
}

void MainWindow::connectPrinter()
{
    if(communication->isConnected())
        return;

    if(!communication->isConfigured())
        return;

    if(communication->OpenSerialPort()!=0)
    {
        return;
    }

    appViewConnected();
}
void MainWindow::disconnectPrinter()
{
    if(communication->CloseSerialPort()!=0)
    {
        return;
    }

    appViewDisconnected();
}

void MainWindow::appViewConnected()
{
    ui->connectButton->setText(tr("Disconnect"));
}

void MainWindow::appViewDisconnected()
{
    ui->connectButton->setText(tr("Connect"));
}

void MainWindow::on_connectButton_clicked()
{
    if(communication->isConnected())
    {
        disconnectPrinter();
    } else {
        connectPrinter();
    }
}
