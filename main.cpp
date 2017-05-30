#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    //设置最大宽高和最小宽高相同就能固定窗口不变
    w.setMinimumSize(728,458);
    w.setMaximumSize(728,458);
    w.show();

    return a.exec();
}
