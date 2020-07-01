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

private slots:
    void on_actionCommunication_triggered();
    void communicationError();
    void ConfigureResponse(SerialStruct serial);

private:
    Ui::MainWindow *ui;
    ConfigureWindow *configure_window;
    Communication *communication;
};

#endif // MAINWINDOW_H
