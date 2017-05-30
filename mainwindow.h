#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QTextCodec>
#include <QTcpServer>
#include <QList>
#include <QTcpSocket>
#include "tcpdialog.h"
#include "serialportdialog.h"
#include "aboutdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private:
    QByteArray receData;
    int Lamp1Status;
    int Lamp2Status;
    int Lamp3Status;
    int Curtain2Status;
    int Curtain3Status;
    Ui::MainWindow *ui;
    SerialPortDialog *serialDialog;
    TcpDialog* tcpDialog;
    AboutDialog* aboutDialog;
    QSerialPort *serialPort;
    QTcpServer* server;
    QList<QTcpSocket*> socketList;
    unsigned char XOR(QByteArray array, unsigned char len);
    void sendSocketData(QByteArray array);

private slots:
    void SwitchLamp();
    void SwitchCurtain();
    void setSerialPort();
    void getSerialPort(QSerialPort *port);
    void my_readData();
    void disAllConnect();
    void setNet();
    void getTcpServer(QTcpServer* server);
    void getSocket();
    void disConnectSlot();
    void readSocketData();
    void aboutSoftware();
    void help();
};

#endif // MAINWINDOW_H
