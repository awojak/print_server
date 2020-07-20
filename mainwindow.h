#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "configurewindow.h"
#include "communication.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    Communication *communication;

private slots:
    void on_actionCommunication_triggered();
    void communicationError();
    void ConfigureResponse(SerialStruct serial);

    void on_connectButton_clicked();

private:
    void connectPrinter();
    void disconnectPrinter();
    void appViewDisconnected();
    void appViewConnected();
    Ui::MainWindow *ui;
    ConfigureWindow *configure_window;
};

#endif // MAINWINDOW_H
