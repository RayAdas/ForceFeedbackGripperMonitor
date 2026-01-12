#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "motor.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(COM* comGrip, Motor* motorFeed, QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void on_pos_modifided(float pos);
    void on_grip_com_readed(QString data);
private slots:
    void on_scanButton_clicked();

    void on_linkButton_grip_clicked();

    void on_linkButton_feed_clicked();

    void on_pushButton_release_clicked();

private:
    Ui::MainWindow *ui;
    COM* comGrip;
    Motor* motorFeed;
};
#endif // MAINWINDOW_H
