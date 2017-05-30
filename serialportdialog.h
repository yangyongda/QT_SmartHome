#ifndef SERIALPORTDIALOG_H
#define SERIALPORTDIALOG_H

#include <QDialog>
#include <QSerialPort>
#include <QSerialPortInfo>

namespace Ui {
class SerialPortDialog;
}

class SerialPortDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SerialPortDialog(QWidget *parent = 0);
    ~SerialPortDialog();

private:
    Ui::SerialPortDialog *ui;
    QSerialPort *my_serialport;

private slots:
    void connectSerialPort();
    void concelSlot();
signals:
    void sendSerialObject(QSerialPort *serial);
};

#endif // SERIALPORTDIALOG_H
