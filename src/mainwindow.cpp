#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(COM* comGrip, Motor* motorFeed, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , comGrip(comGrip)
    , motorFeed(motorFeed)
{
    ui->setupUi(this);
    QObject::connect(comGrip, &COM::dataReaded, this, &MainWindow::on_grip_com_readed);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_scanButton_clicked()
{
    QList<QString> COMList = this->comGrip->getPortNameList();
    this->ui->comboBox_grip->clear();
    this->ui->comboBox_feed->clear();
    this->ui->comboBox_grip->addItems(COMList);
    this->ui->comboBox_feed->addItems(COMList);
}

void MainWindow::on_linkButton_grip_clicked()
{
    QString devName = this->ui->comboBox_grip->currentText();
    if(devName.isEmpty())
    {return;}

    if(!this->comGrip->getIsOpen())
    {
        //打开串口和CAN通道
        bool isSuccess = this->comGrip->open(devName);
        isSuccess &= (this->comGrip->write("S6\r") != -1);//设置CAN波特率为500 kbps
        isSuccess &= (this->comGrip->write("O\r") != -1);//打开CAN通道
    }
}


void MainWindow::on_linkButton_feed_clicked()
{
    QString devName = this->ui->comboBox_feed->currentText();
    if(devName.isEmpty())
    {return;}

    if(!this->motorFeed->COM0->getIsOpen())
    {
        //打开串口
        bool isSuccess = this->motorFeed->COM0->open(devName);
    }
}


void MainWindow::on_pushButton_release_clicked()
{
    this->motorFeed->mMode(0);
}

void MainWindow::on_pos_modifided(float pos){
    this->ui->label_pos->setText(QString::number(pos));
    float pos_limited = pos;
    pos_limited = pos_limited > 100 ? 100 : pos_limited;
    pos_limited = pos_limited < 0 ? 0 : pos_limited;
    this->comGrip->write("P:" + QString::number(pos_limited) + "\n");
}

void MainWindow::on_grip_com_readed(QString data){
    QStringList parts = data.split(',');

    if (parts.size() >= 3) {
        bool ok1, ok2, ok3;
        float curr = parts[0].toFloat(&ok1);
        float angleA = parts[1].toFloat(&ok2);
        float angleB = parts[2].toFloat(&ok3);

        if (ok1 && ok2 && ok3) {
            this->motorFeed->mMode(curr*-200);
        } else {
            qDebug() << "转换失败: 无法解析为浮点数";
        }
    } else {
        qDebug() << "数据格式错误: 需要至少3个逗号分隔的值";
    }
}
