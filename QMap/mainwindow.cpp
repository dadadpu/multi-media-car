#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

#define exit_path ":/pic/exit.png"
#define menu_path ":/pic/map_menu.jpeg"
#define search_path ":/pic/search.png"
#define down_path ":/pic/zoom_down.png"
#define up_path ":/pic/zoom_up.png"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qDebug() << "地图V1.0";
    this->setFixedSize(800,480);//设置界面固定大小
    this->setWindowTitle("百度地图beta");
    this->setStyleSheet("MainWindow{border-image: url(:/pic/map_menu.jpeg);}");

    //设置触摸事件
    setAttribute(Qt::WA_AcceptTouchEvents);

    //安装事件过滤器
    this->installEventFilter(this);
    timer_flash = new QTimer(this);

    //发送请求
    manager = new QNetworkAccessManager(this);

    city = "";
    east = 113.25;
    north = 23.13;
    markers = QString("%1,%2").arg(east).arg(north);
    zoom = "11";
    qDebug() << "发送网络请求";
    sendResquest(city,markers,zoom);

    //按钮初始化
    button_init();


}

MainWindow::~MainWindow()
{
    delete ui;
    delete manager;
    delete timer_anim;
    delete timer_flash;
    delete zoom_down;
    delete zoom_up;
    delete exit_button;

}


bool isconnect = false;
void MainWindow::sendResquest(QString city, QString markers, QString zoom)
{
    // 断开之前的连接
    if (isconnect) {
        manager->disconnect();
        disconnect(reply, &QIODevice::readyRead, this, &MainWindow::ready_Read_slots);
    } else {
        isconnect = true;
    }


    // 构建请求URL
    resq_msg = QString("http://api.map.baidu.com/staticimage?center=%1&markers=%2&zoom=%3&width=800&height=480")
            .arg(city).arg(markers).arg(zoom);
    resquest.setUrl(QUrl(resq_msg.toUtf8().data()));
    qDebug() << "正在发送网络请求";


    // 发送请求
    reply = manager->get(resquest);
    connect(reply, &QIODevice::readyRead, this, &MainWindow::ready_Read_slots);

    //这一段本来可以放在ready_Read_slots，但是由于这个接口会多次返回值，造成频繁绑定和断开，所以放在这里好
     disconnect(timer_flash, &QTimer::timeout, nullptr, nullptr);
    // 定时器连接
    connect(timer_flash, &QTimer::timeout, this, [=]() {
        timer_flash->stop();
        file_name = "1.png";
        file.setFileName(file_name);
        if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {//以只写和清空打开
            file.write(reply->readAll());
            file.close();

            QPixmap pix;
            if (pix.load(file_name)) {
                this->setStyleSheet("MainWindow{border-image: url(./1.png);}");
                qDebug() << "地图图片设置成功";
            }
        } else {
            qDebug() << "文件写入失败";
        }
        this->show();
    });



}



void MainWindow::button_init()
{
    //缩小按钮
    zoom_down = new myPushButton(down_path,"",80,80);
    zoom_down->setParent(this);
    zoom_down->move(580,350);

    connect(zoom_down,&myPushButton::clicked,[=](){
        zoom_down->zoom1();
        if(zoom.toInt()>1)
        {
            int num = zoom.toInt() -1;
            zoom.clear();
            zoom = QString("%1").arg(num);
            //qDebug() << resq_msg;
            sendResquest(city,markers,zoom);
        }
    });


    //放大按钮
    zoom_up = new myPushButton(up_path,"",80,80);
    zoom_up->setParent(this);
    zoom_up->move(700,350);

    connect(zoom_up,&myPushButton::clicked,[=](){
        zoom_up->zoom1();
        if(zoom.toInt()<18)
        {
            //qDebug() << resq_msg;
            int num = zoom.toInt()+1;
            zoom.clear();
            zoom = QString("%1").arg(num);
            sendResquest(city,markers,zoom);
        }
        zoom_up->zoom2();
    });

    //返回按钮
    exit_button = new myPushButton(exit_path,"",40,40);
    exit_button->setParent(this);
    exit_button->move(760,440);


    timer_anim = new QTimer(this);
    connect(exit_button,&myPushButton::clicked,[=](){
        exit_button->zoom1();
        exit_button->zoom2();
        disconnect(timer_anim,&QTimer::timeout,nullptr,nullptr);
        timer_anim->start(500);
        connect(timer_anim,&QTimer::timeout,[=](){
            timer_anim->stop();
            qDebug() << "更新点坐标文件";
            QString file_n = "p.info";
            QFile f_file;
            f_file.setFileName(file_n);
            f_file.open(QIODevice::WriteOnly | QIODevice::Truncate);
            QString point_msg = QString("%1\n%2").arg(east).arg(north);
            f_file.write(point_msg.toUtf8().data());
            f_file.close();
            exit(1);
        });

    });

}

QList<QHostAddress> MainWindow::getLocalIpAddresses()
{
    QList<QHostAddress> ipAddresses;

        QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
        for (const QNetworkInterface& interface : interfaces) {
            // 忽略回环接口和无效接口
            if (interface.flags().testFlag(QNetworkInterface::IsLoopBack) || !interface.isValid()) {
                continue;
            }

            QList<QNetworkAddressEntry> entries = interface.addressEntries();
            for (const QNetworkAddressEntry& entry : entries) {
                // 仅获取IPv4地址
                if (entry.ip().protocol() == QAbstractSocket::IPv4Protocol) {
                    ipAddresses.append(entry.ip());
                }
            }
        }

        return ipAddresses;
}



bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
   // qDebug() << "------eventFilter-------1";
    if(watched == this)
    {
         //qDebug() << "------eventFilter-------2";
        QTouchEvent *touch = static_cast<QTouchEvent*>(event);

        switch (event->type())
        {
        case QEvent::MouseButtonPress://鼠标按下事件
            pres_flag = 1;
            last_point.setY(cursor().pos().y());
            last_point.setX(cursor().pos().x());
            break;
        case QEvent::MouseButtonRelease://鼠标松开
            rele_flag = 1;
            point.setY(cursor().pos().y());
            point.setX(cursor().pos().x());
            break;

        case QEvent::TouchBegin://触摸
            point.setY(touch->touchPoints().at(0).pos().y());
            point.setX(touch->touchPoints().at(0).pos().x());
            if(point.x() < 580 || point.y()<350)
            {
                rele_flag = 1;
                pres_flag = 1;
            }
            event->accept();
            break;
           default:
             //qDebug() << "------eventFilter-------break";
            break;
        }

    }
    if(pres_flag == 1 && rele_flag == 1)
    {
        pres_flag = 0;
        rele_flag = 0;
        if(event->type() == QEvent::TouchBegin)
        {
            m_x = 400-point.x();
            m_y = 240-point.y();
        }
        else
        {
            m_x = point.x()-last_point.x();
            m_y = point.y()-last_point.y();
        }
        if(m_x>10 || m_x<-10 || m_y>10 || m_y<-10)
        {
            QString m_zoom = zoom;
            move_x = m_x*0.0000003*(19-m_zoom.toInt())*(19-m_zoom.toInt())*(19-m_zoom.toInt())*(19-m_zoom.toInt());
            move_y = m_y*0.0000003*(19-m_zoom.toInt())*(19-m_zoom.toInt())*(19-m_zoom.toInt())*(19-m_zoom.toInt());
            east -= move_x;
            north += move_y;
            markers = QString("%1,%2").arg(east).arg(north);
            // qDebug() << "------eventFilter-------3";
            sendResquest(city,markers,zoom);
             //qDebug() << "------eventFilter-------send";
        }

    }
     //qDebug() << "------eventFilter-------end";
    return QWidget::eventFilter(watched,event);
}

//读取reply
void MainWindow::ready_Read_slots()
{
         timer_flash->start(10);

}

