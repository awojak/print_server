#ifndef CONFIGUREWINDOW_H
#define CONFIGUREWINDOW_H

#include <QWidget>
#include <QtSerialPort/QtSerialPort>
#include <QVector>
#include "communication.h"

namespace Ui {
class ConfigureWindow;
}

class ConfigureWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ConfigureWindow(QWidget *parent = 0);
    ~ConfigureWindow();

private slots:
    void on_bRefreshSerialPort_clicked();

    void on_bClose_clicked();

    void on_bApply_clicked();

    void on_checkBox_toggled(bool checked);

signals:
    void ConfigureResponse(SerialStruct serial);

private:
    void showEvent(QShowEvent *ev);
    Ui::ConfigureWindow *ui;
    SerialStruct serial;
    QVector<QString> portList;
    QVector<QString> serialList;
};

#endif // CONFIGUREWINDOW_H
