#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QThread>
#include <QDesktopServices>
#include<QUrl>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Lamp1Status = 0; //灯初始为关闭状态
    Lamp2Status = 0;
    Lamp3Status = 0;
    Curtain2Status = 0;  //窗帘默认为关闭
    Curtain3Status = 0;
    serialPort = NULL;
    server = NULL;

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    tcpDialog = new TcpDialog;
    serialDialog = new SerialPortDialog;
    aboutDialog = new AboutDialog;
    QObject::connect(ui->SwitchPushButton1,SIGNAL(clicked(bool)),this,SLOT(SwitchLamp()));
    QObject::connect(ui->SwitchPushButton2,SIGNAL(clicked(bool)),this,SLOT(SwitchLamp()));
    QObject::connect(ui->SwitchPushButton3,SIGNAL(clicked(bool)),this,SLOT(SwitchLamp()));
    QObject::connect(ui->CurtainPushButton1,SIGNAL(clicked(bool)),this,SLOT(SwitchCurtain()));
    QObject::connect(ui->CurtainPushButton2,SIGNAL(clicked(bool)),this,SLOT(SwitchCurtain()));
    QObject::connect(ui->action_C,SIGNAL(triggered(bool)),this,SLOT(setSerialPort()));
    QObject::connect(ui->action_D,SIGNAL(triggered(bool)),this,SLOT(disAllConnect()));
    QObject::connect(serialDialog,SIGNAL(sendSerialObject(QSerialPort*)),this,SLOT(getSerialPort(QSerialPort*)));
    QObject::connect(ui->action_N,SIGNAL(triggered(bool)),this,SLOT(setNet()));
    QObject::connect(tcpDialog,SIGNAL(sendTcpObject(QTcpServer*)),this,SLOT(getTcpServer(QTcpServer*)));
    QObject::connect(ui->action_A,SIGNAL(triggered(bool)),this,SLOT(aboutSoftware()));
    QObject::connect(ui->action_H,SIGNAL(triggered(bool)),this,SLOT(help()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

unsigned char MainWindow::XOR(QByteArray array, unsigned char len)
{
    unsigned char result = array[0];
    for(int i = 1; i<len; i++)
    {
        result = result^array[i];
    }
    return result;
}

void MainWindow::sendSocketData(QByteArray array)
{
    if(!socketList.isEmpty())
    {
        for(int i = 0; i<socketList.size(); i++)
        {
            socketList.at(i)->write(array);
        }
    }
}

void MainWindow::SwitchLamp()
{
    QByteArray array,temp;

    array[0] = 0xFE; //消息头，zigbee会自动去除

    //判断是哪个按钮按下
    QPushButton* pt=qobject_cast <QPushButton*>(sender());  //获取信号发送者
    if(!pt) return;
    if(pt == ui->SwitchPushButton1)  //厨房
    {
        array[1] = 0x01; //数据长度
        array[2] = 0x01;
        if(Lamp1Status == 0)
        {
            ui->SwitchPushButton1->setStyleSheet("background-image: url(:/new/image/image/light.png);");
            array[3] = 0x01;  //开
            Lamp1Status = 1;
        }
        else
        {
            ui->SwitchPushButton1->setStyleSheet("background-image: url(:/new/image/image/dark.png);");
            array[3] = 0x00;  //关
            Lamp1Status = 0;
        }
    }
    else if(pt == ui->SwitchPushButton2) //卧室
    {
        array[1] = 0x02; //数据长度
        array[2] = 0x02;
        if(Lamp2Status == 0)
        {
            ui->SwitchPushButton2->setStyleSheet("background-image: url(:/new/image/image/light.png);");
            array[3] = 0x01;  //开
            Lamp2Status = 1;
        }
        else
        {
            ui->SwitchPushButton2->setStyleSheet("background-image: url(:/new/image/image/dark.png);");
            array[3] = 0x00;  //关
            Lamp2Status = 0;
        }
        if(Curtain2Status == 0)
            array[4] = 0x00;
        else
            array[4] = 0x01;
    }
    else if(pt == ui->SwitchPushButton3) //客厅
    {
        array[1] = 0x02; //数据长度
        array[2] = 0x03;
        if(Lamp3Status == 0)
        {
            ui->SwitchPushButton3->setStyleSheet("background-image: url(:/new/image/image/light.png);");
            array[3] = 0x01;  //开
            Lamp3Status = 1;
        }
        else
        {
            ui->SwitchPushButton3->setStyleSheet("background-image: url(:/new/image/image/dark.png);");
            array[3] = 0x00;  //关
            Lamp3Status = 0;
        }
        if(Curtain3Status == 0)
            array[4] = 0x00;
        else
            array[4] = 0x01;
    }
    if(pt == ui->SwitchPushButton1)
    {
        temp[0] = array[2];
        temp[1] = array[3];
        array[4] = XOR(temp,2);
        temp[0] = array[1];
        temp[1] = array[2];
        temp[2] = array[3];
        temp[3] = array[4];
        array[5] = XOR(temp,4);
    }
    else
    {
        temp[0] = array[2];
        temp[1] = array[3];
        temp[2] = array[4];
        array[5] = XOR(temp,3);
        temp[0] = array[1];
        temp[1] = array[2];
        temp[2] = array[3];
        temp[3] = array[4];
        temp[4] = array[5];
        array[6] = XOR(temp,5);
    }
    if(serialPort != NULL)
        serialPort->write(array);

}

void MainWindow::SwitchCurtain()
{
    QByteArray array,temp;

    array[0] = 0xFE; //消息头，zigbee会自动去除
    array[1] = 0x02; //数据长度
    QPushButton* pt=qobject_cast <QPushButton*>(sender());  //获取信号发送者
    if(!pt) return;
    if(pt == ui->CurtainPushButton1)
    {
        array[2] = 0x02;
        if(Lamp2Status == 0)
            array[3] = 0x00;
        else
            array[3] = 0x01;
        if(Curtain2Status == 0)
        {
            Curtain2Status = 1;
            ui->CurtainPushButton1->setStyleSheet("background-image: url(:/new/image/image/open.png);");
            array[4] = 0x01;
        }
        else
        {
            Curtain2Status = 0;
            ui->CurtainPushButton1->setStyleSheet("background-image: url(:/new/image/image/close.png);");
            array[4] = 0x00;
        }

    }
    else if(pt == ui->CurtainPushButton2)
    {
        array[2] = 0x03;
        if(Lamp3Status == 0)
            array[3] = 0x00;
        else
            array[3] = 0x01;
        if(Curtain3Status == 0)
        {
            Curtain3Status = 1;
            ui->CurtainPushButton2->setStyleSheet("background-image: url(:/new/image/image/open.png);");
            array[4] = 0x01;
        }
        else
        {
            Curtain3Status = 0;
            ui->CurtainPushButton2->setStyleSheet("background-image: url(:/new/image/image/close.png);");
            array[4] = 0x00;
        }
    }
    temp[0] = array[2];
    temp[1] = array[3];
    temp[2] = array[4];
    array[5] = XOR(temp,3);
    temp[0] = array[1];
    temp[1] = array[2];
    temp[2] = array[3];
    temp[3] = array[4];
    temp[4] = array[5];
    array[6] = XOR(temp,5);
    if(serialPort != NULL)
        serialPort->write(array);
}

void MainWindow::setSerialPort()
{
    serialDialog->show();
}

void MainWindow::getSerialPort(QSerialPort *port)
{
    this->serialPort = port;
    QMessageBox::information(this,"连接...","连接成功！");
    QObject::connect(serialPort,SIGNAL(readyRead()),this,SLOT(my_readData()));
    //qDebug()<< port->portName();
}

void MainWindow::my_readData()
{
    QByteArray temp;
    temp = serialPort->readAll();
    if(NULL != temp )
    {
        receData.append(temp);
        if(XOR(receData,receData.size()-1)== receData[receData.size()-1])  //异或校验
        {
            if(0x01 == receData[0])  //厨房
            {
                ui->temp1->setText(QString::number(receData[2]));
                ui->progressBar->setValue(receData[2]);
                ui->humi1->setText(QString::number(receData[1]));
                ui->progressBar_2->setValue(receData[1]);
               // qDebug()<<(receData);
                ui->smog->setText(QString::number((receData[3]*256 +receData[4])));
                receData[5] = Lamp1Status;  //加入灯的状态，保持服务器和客户端状态一致
                receData[6] = XOR(receData,6);
                sendSocketData(receData);

            }
            else if(0x02 == receData[0]) //卧室
            {
                ui->temp2->setText(QString::number(receData[2]));
                ui->progressBar_3->setValue(receData[2]);
                ui->humi2->setText(QString::number(receData[1]));
                ui->progressBar_4->setValue(receData[1]);
                receData[3] = Lamp2Status;  //加入灯的状态，保持服务器和客户端状态一致
                receData[4] = Curtain2Status;
                receData[5] = XOR(receData,5);
                sendSocketData(receData);

            }
            else if(0x03 == receData[0]) //客厅
            {
                ui->temp3->setText(QString::number(receData[2]));
                ui->progressBar_5->setValue(receData[2]);
                ui->humi3->setText(QString::number(receData[1]));
                ui->progressBar_6->setValue(receData[1]);
                receData[3] = Lamp3Status;  //加入灯的状态，保持服务器和客户端状态一致
                receData[4] = Curtain3Status;
                receData[5] = XOR(receData,5);
                sendSocketData(receData);

            }
            receData.clear();
        }
        if(receData.size()>10)
            receData.clear();
    }

}

void MainWindow::disAllConnect()
{
    if(serialPort != NULL)
        serialPort->close();
    if(server != NULL)
        server->close();
    QMessageBox::information(this,"断开...","所有连接已断开！");
}

void MainWindow::setNet()
{
    tcpDialog->show();
}

void MainWindow::getTcpServer(QTcpServer *Tcpserver)
{
    this->server = Tcpserver;
    QObject::connect(server,SIGNAL(newConnection()),this,SLOT(getSocket()));
    //qDebug()<<server->serverAddress()<<server->serverPort();
}

void MainWindow::getSocket()
{
    QTcpSocket* socket = server->nextPendingConnection();
    QObject::connect(socket,SIGNAL(disconnected()),socket,SLOT(deleteLater()));  //某个Socket断开连接后释放资源
    QObject::connect(socket,SIGNAL(disconnected()),this,SLOT(disConnectSlot())); //断开连接移除List中的Socket
    QObject::connect(socket,SIGNAL(readyRead()),this,SLOT(readSocketData()));
    socketList.append(socket);
    //qDebug()<<socketList;
}

//当客户端断开连接时移除Socket
void MainWindow::disConnectSlot()
{
    socketList.removeAt(socketList.indexOf(qobject_cast <QTcpSocket*>(sender()))); //移除指定的socket
    //qDebug()<<socketList;
}

void MainWindow::readSocketData()
{
    QByteArray socketData;
    QTcpSocket* socket = qobject_cast <QTcpSocket*>(sender());
    socketData = socket->readAll();
    if(serialPort != NULL)
        serialPort->write(socketData);
    if( 0x01 == socketData[2])
    {
        if(Lamp1Status != socketData[3])  //状态改变
        {
           if(0x01 == socketData[3])
           {
               ui->SwitchPushButton1->setStyleSheet("background-image: url(:/new/image/image/light.png);");
               Lamp1Status = 1;
           }
           else
           {
               ui->SwitchPushButton1->setStyleSheet("background-image: url(:/new/image/image/dark.png);");
               Lamp1Status = 0;
           }
        }
    }
    else if( 0x02 == socketData[2])
    {
        if(Lamp2Status != socketData[3])  //状态改变
        {
           if(0x01 == socketData[3])
           {
               ui->SwitchPushButton2->setStyleSheet("background-image: url(:/new/image/image/light.png);");
               Lamp2Status = 1;
           }
           else
           {
               ui->SwitchPushButton2->setStyleSheet("background-image: url(:/new/image/image/dark.png);");
               Lamp2Status = 0;
           }
        }
        if(Curtain2Status != socketData[4])
        {
            if(0x01 == socketData[4])
            {
                Curtain2Status = 1;
                ui->CurtainPushButton1->setStyleSheet("background-image: url(:/new/image/image/open.png);");
            }
            else
            {
                Curtain2Status = 0;
                ui->CurtainPushButton1->setStyleSheet("background-image: url(:/new/image/image/close.png);");
            }
        }
    }
    else if( 0x03 == socketData[2])
    {
        if(Lamp3Status != socketData[3])  //状态改变
        {
           if(0x01 == socketData[3])
           {
               ui->SwitchPushButton3->setStyleSheet("background-image: url(:/new/image/image/light.png);");
               Lamp3Status = 1;
           }
           else
           {
               ui->SwitchPushButton3->setStyleSheet("background-image: url(:/new/image/image/dark.png);");
               Lamp3Status = 0;
           }
        }
        if(Curtain3Status != socketData[4])
        {
            if(0x01 == socketData[4])
            {
                Curtain3Status = 1;
                ui->CurtainPushButton2->setStyleSheet("background-image: url(:/new/image/image/open.png);");
            }
            else
            {
                Curtain3Status = 0;
                ui->CurtainPushButton2->setStyleSheet("background-image: url(:/new/image/image/close.png);");
            }
        }
    }
}

void MainWindow::aboutSoftware()
{
    aboutDialog->exec();
}

void MainWindow::help()
{
    QDesktopServices::openUrl(QUrl("http://www.baidu.com"));
}

