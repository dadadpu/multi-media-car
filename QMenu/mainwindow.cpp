#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mypushbutton.h"

#define music_path ":/pic/music.jpeg"
#define video_path ":/pic/video.jpeg"
#define map_path ":/pic/map.jpg"
#define weather_path ":/pic/weather.jpg"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // 设置窗口属性
    this->setCursor(Qt::BlankCursor); //隐藏鼠从标
    this->setFixedSize(800,480);
    this->setWindowTitle("车载系统V1.0");
    this->setStyleSheet("background-color: rgb(72, 72, 72);");
    //创建线程
    mypro = new QProcess(this);
    //安装事件过滤器
    this->installEventFilter(this);
    //创建时间和日期的标签
    this->time_label = new QLabel(this);
    this->time_label->setGeometry(10,20,240,100);
    this->time_label->setStyleSheet("font-size:55px;color:white");

    this->date_label = new QLabel(this);
    this->date_label->setGeometry(100,120,240,100);
     //创建更新时间的定时器
    this->update_timer = new QTimer(this);
    this->update_timer->start(200);

    connect(this->update_timer,&QTimer::timeout,this,&MainWindow::updateTime);
    //创建隐藏鼠标的定时器
    this->mouse_timer = new QTimer(this);

        connect(this->mouse_timer,&QTimer::timeout,[=](){
            this->setCursor(Qt::BlankCursor); //隐藏鼠标
        });
      //创建音乐按钮
      this->music_button = new myPushButton(music_path,"",250,430);
      this->timer_anim = new QTimer(this);
      this->music_button->setParent(this);
      this->music_button->move(275,25);

        connect(this->music_button,&myPushButton::clicked,[=](){
            this->music_button->zoom1();
            this->music_button->zoom2();
            disconnect(timer_anim, &QTimer::timeout, nullptr, nullptr);
            // 将按钮设置为不可点击状态
            music_button->setEnabled(false);
            this->timer_anim->start(500);
            connect(this->timer_anim,&QTimer::timeout,[=](){
                this->timer_anim->stop();
                this->mypro->close();
                this->mypro->start("./QMusicPlayer");
                music_button->setEnabled(true);
            });
        });
         //创建视频按钮
        this->video_button = new myPushButton(video_path,"",250,200);
        this->video_button->setParent(this);
        this->video_button->move(550,25);

        connect(this->video_button,&myPushButton::clicked,[=](){
            this->video_button->zoom1();
            this->video_button->zoom2();
            disconnect(timer_anim, &QTimer::timeout, nullptr, nullptr);
            // 将按钮设置为不可点击状态
            video_button->setEnabled(false);
            this->timer_anim->start(500);
            connect(this->timer_anim,&QTimer::timeout,[=](){
                this->timer_anim->stop();
                this->mypro->close();
                this->mypro->start("./QvideoPlayer");
                video_button->setEnabled(true);
            });
        });
        //创建地图按钮
        this->map_button = new myPushButton(map_path,"",250,200);
        this->map_button->setParent(this);
        this->map_button->move(550,255);

        connect(this->map_button,&myPushButton::clicked,[=](){
            this->map_button->zoom1();
            this->map_button->zoom2();
            disconnect(timer_anim, &QTimer::timeout, nullptr, nullptr);
            // 将按钮设置为不可点击状态
            map_button->setEnabled(false);
            this->timer_anim->start(500);
            connect(this->timer_anim,&QTimer::timeout,[=](){
                this->timer_anim->stop();
                this->mypro->close();
                this->mypro->start("./QMap");
                map_button->setEnabled(true);
            });
        });
        //创建天气按钮
        this->weather_button = new myPushButton(weather_path,"",250,200);
        this->weather_button->setParent(this);
        this->weather_button->move(10,255);

        connect(this->weather_button,&myPushButton::clicked,[=](){
            this->weather_button->zoom1();
            this->weather_button->zoom2();
            disconnect(timer_anim, &QTimer::timeout, nullptr, nullptr);
            // 将按钮设置为不可点击状态
            weather_button->setEnabled(false);
            this->timer_anim->start(500);
            connect(this->timer_anim,&QTimer::timeout,[=](){
                this->timer_anim->stop();
                this->mypro->close();
                this->mypro->start("./QWeather");
                weather_button->setEnabled(true);
            });
        });

        //进程结束处理
            connect(mypro,&QProcess::stateChanged,[=](){
            });



}

MainWindow::~MainWindow()
{
        delete ui;
        delete mypro;
            delete timer_anim;
            delete mouse_timer;
            delete update_timer;
            delete time_label;
            delete date_label;
            delete music_button;
            delete video_button;
            delete map_button;
            delete weather_button;
}

void MainWindow::updateTime()
{
        QTime tim = QTime::currentTime();//获取当前时间
                   time_label->setText(tim.toString());

               QDate date = QDate::currentDate();//获取当前日期
               QString date_msg = QString("%1-%2-%3").arg(date.year()).arg(date.month()).arg(date.day());
                   date_label->setText(date_msg);

                   if(mypro->state() == QProcess::NotRunning)
                   {
                       this->show();//重新显示窗口
                   }
                   else this->hide();
}

//事件过滤器
bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{

    if(watched == this ){

        switch (event->type()) {
        case QEvent::MouseButtonPress:
                 mouse_timer->start(10000);
                 this->setCursor(Qt::ArrowCursor);  //显示正常鼠标
            break;
        default:
            break;
        }
    }

    return QWidget::eventFilter(watched,event);//将事件传递给父类
}
