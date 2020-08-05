#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "configurewindow.h"
#include "communication.h"
#include "callback.h"
#include "min.h"
#include "commandinterpreter.h"
#include "system.h"

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
    MinProtocol *protocol;
    System system;
    CommandInterpreter cmd;

    void connectPrinter();
    void disconnectPrinter();
    void appViewDisconnected();
    void appViewConnected();
    Ui::MainWindow *ui;
    ConfigureWindow *configure_window;
    /*
     * Callback Declarations
     */
    //Callback<MainViewBase, const touchgfx::AbstractButton&> serialReady;

    /*
     * Callback Handler Declarations
     */
    //void buttonCallbackHandler(const Communication& src);

};

#endif // MAINWINDOW_H
