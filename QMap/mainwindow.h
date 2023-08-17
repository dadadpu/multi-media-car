#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include "mypushbutton.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTimer>
#include <QFileDialog>
#include <QMessageBox>
#include <QTouchEvent>
#include <QDebug>
#include <QPixmap>
#include <QNetworkInterface>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QDebug>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void sendResquest(QString city,QString markers,QString zoom = "11");
    void button_init();
    QList<QHostAddress> getLocalIpAddresses();


private:
    //网络
    QNetworkRequest resquest;
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    //字符串参数
    QString city;
    QString markers;
    QString zoom;
    QString resq_msg;

    //图片文件
    QFile file;
    QString file_name = nullptr;
    //事件过滤器
    QPoint point,last_point;
    bool pres_flag,rele_flag;

    //定时器
    QTimer *timer_anim,*timer_flash;

    //按钮
    myPushButton  *zoom_down,*zoom_up,*exit_button;

    //移动
    int m_x,m_y;
    double move_x,move_y;
    double east,north;//东经北纬

    Ui::MainWindow *ui;
    bool eventFilter(QObject *watched,QEvent *event);




public slots:
    void ready_Read_slots();

};

#endif // MAINWINDOW_H
