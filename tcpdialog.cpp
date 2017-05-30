#include "tcpdialog.h"
#include "ui_tcpdialog.h"

TcpDialog::TcpDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TcpDialog)
{
    ui->setupUi(this);
    tcpServer = new QTcpServer;
    QString localHostName = QHostInfo::localHostName();
    QHostInfo info = QHostInfo::fromName(localHostName);
    foreach(QHostAddress address,info.addresses())
    {
        if(address.protocol() == QAbstractSocket::IPv4Protocol)
           ui->IPComboBox->addItem(address.toString());
    }
    QObject::connect(ui->ListenPushButton,SIGNAL(clicked(bool)),this,SLOT(connectNet()));
    QObject::connect(ui->CancelPushButton,SIGNAL(clicked(bool)),this,SLOT(cancelSlot()));

}

TcpDialog::~TcpDialog()
{
    delete ui;
}

void TcpDialog::connectNet()
{
    tcpServer->listen(QHostAddress(ui->IPComboBox->currentText()),ui->Port->text().toLong());
    emit sendTcpObject(tcpServer);
    this->close();
}

void TcpDialog::cancelSlot()
{
    this->close();
}
