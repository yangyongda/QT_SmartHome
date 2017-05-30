#include "serialportdialog.h"
#include "ui_serialportdialog.h"

SerialPortDialog::SerialPortDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SerialPortDialog)
{
    ui->setupUi(this);
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
        {
            QSerialPort serial;
            serial.setPort(info);
            if (serial.open(QIODevice::ReadWrite))
            {
                ui->PortComboBox->addItem(info.portName());
                serial.close();
            }
        }
      my_serialport= new QSerialPort;//初始化串口
      QObject::connect(ui->OpenButton,SIGNAL(clicked(bool)),this,SLOT(connectSerialPort()));
      QObject::connect(ui->CancelPushButton,SIGNAL(clicked(bool)),this,SLOT(concelSlot()));
}

SerialPortDialog::~SerialPortDialog()
{
    delete ui;
}

void SerialPortDialog::connectSerialPort()
{
    my_serialport->setPortName(ui->PortComboBox->currentText()); //设置端口号
    my_serialport->open(QIODevice::ReadWrite);      //要先打开才能设置，否则出错
    my_serialport->setBaudRate(ui->BaudComboBox->currentText().toInt()); //设置波特率
    switch(ui->DataBitComboBox->currentIndex())  //设置数据位
    {
    case 0: my_serialport->setDataBits(QSerialPort::Data8);break;
    case 1: my_serialport->setDataBits(QSerialPort::Data7);break;
    case 2: my_serialport->setDataBits(QSerialPort::Data6);break;
    }
    switch(ui->StopBitComboBox->currentIndex())  //设置停止位
    {
    case 0: my_serialport->setStopBits(QSerialPort::OneStop);break;
    case 1: my_serialport->setStopBits(QSerialPort::TwoStop);break;
    }
    my_serialport->setParity(QSerialPort::NoParity);  //校验位
    my_serialport->setFlowControl(QSerialPort::NoFlowControl); //流控

    emit sendSerialObject(my_serialport);
    this->close();
}

void SerialPortDialog::concelSlot()
{
    this->close();
}
