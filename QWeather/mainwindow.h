#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "weather.h"
#include <QMainWindow>
#include <QHBoxLayout>
#include <QLabel>
#include <QDateTime>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include <QTextCodec>
#include <QFileInfo>
#include <QDesktopWidget>
#include <QMessageBox>
#include "mypushbutton.h"
#include <QProcess>
#include <QGraphicsDropShadowEffect>
#include <QPropertyAnimation>
#include <QTimer>
#include <QPushButton>
#include <QDebug>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void mainWindowInit();                          // 主窗体布局初始化
    void weatherDisplayInit();                      // 天气显示相关初始化
    void currentTimeInit();                         // 日期显示相关初始化
    void sendResquest();
    void weatherinit();
private:
    Ui::MainWindow *ui;
    //网络
    QNetworkRequest resquest;
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    //进程
    QProcess * mypro;
    //定时器
    QTimer *timer,*timer1,*timer2;
    //按钮
    myPushButton * closeBt,*searchBt;
    //城市
    QString city;
    QString citycode;
    //地址
    double east,north;//东经北纬
    //当前温度
    QString temp;

    QWidget         *mainWidget;                    // 主Widge
    QWidget         *weatherWidgetArr[5];           // 天气显示面板背景arr，设置为透明
    QVBoxLayout     *vlayoutArr[5];                 // 水平布局arr
    QLabel          *weekLabelArr[5];               // 显示星期几arr
    QLabel          *minMaxTempLabelArr[5];         // 最高温度与最低温度arr
    QLabel          *weatherIconLabelArr[5];        // 天气图标标签arr
    QLabel          *weatherTypeLabelArr[5];        // 天气类型arr

    QGraphicsDropShadowEffect *loopGraphicsDropShadowEffect[5];
    QPropertyAnimation *propertyAnimation[5];
    QWidget  *topWidget;
    QLabel   *appNameLabel;
    QPushButton *exitPushButton;

    QDateTime       *currentTime;
    QLabel          *currentTimeLael;               // 当前时间
    QTimer          *updateTimer;                   // 更新时间定时器

    QNetworkAccessManager *NAManager;               // 网络请求
    QNetworkReply *newReply;

    QPushButton     *cityPushButton;                // 显示当前所在的地里位置（城市）
    QLabel          *currentTempLabel;              // 当前温度
    QLabel          *currentWeatherIcon;            // 当前天气类型图片
    QLabel          *currentWeather;                // 当前天气类型
    bool eventFilter(QObject *watched, QEvent *event);

private slots:
    void slotFinishedWeatherGetReply();
    void updateTimerTimeOut();
    void ready_Read_slots();
    void handleProcessFinished();

};

#endif // MAINWINDOW_H
