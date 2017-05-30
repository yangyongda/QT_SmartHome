#ifndef TCPDIALOG_H
#define TCPDIALOG_H

#include <QDialog>
#include <QTcpServer>
#include <QtNetwork>
#include <QHostInfo>
#include <QHostAddress>
namespace Ui {
class TcpDialog;
}

class TcpDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TcpDialog(QWidget *parent = 0);
    ~TcpDialog();

private:
    Ui::TcpDialog *ui;
    QTcpServer* tcpServer;
private slots:
    void connectNet();
    void cancelSlot();
signals:
    void sendTcpObject(QTcpServer * server);
};

#endif // TCPDIALOG_H
