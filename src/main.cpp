#include "mainwindow.h"

#include <QApplication>
#include <QtSerialPort>

#include "motor.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    COM comFeed, comGrip;

    // 电机和串口目前运行在主线程中，通过 Qt 的事件循环和信号/槽实现异步 I/O。
    // 如果后续需要专门的工作线程，可以再按照建议的多线程方案进行重构。
    Motor motorFeed(&comFeed, 1);
    MainWindow w(&comGrip, &motorFeed);

    QObject::connect(&motorFeed, &Motor::pos_updated, &w, &MainWindow::on_pos_modifided);



    // 开启位置同步
    motorFeed.setPosPollingEnabled(true);

    w.show();
    return a.exec();
}
