#include "mainwindow.h"
#include "ui_mainwindow.h"
#define menu_path ":/pic/video_menu.jpeg"
#define exit_path ":/pic/exit.png"
#define next_path ":/pic/next.png"
#define openfile_path ":/pic/openfile.png"
#define pause_path ":/pic/pause.png"
#define play_path ":/pic/play.png"
#define prev_path ":/pic/prev.png"
#define vol_path ":/pic/vol.png"
#define video_path "video/"
#define lrc_path "lrc/"
#define photo_path "photo/"
#define file_path ":/pic/openfile.png"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(800,480);
    this->setWindowTitle("视频播放器V1.0");
    QString backg = QString("MainWindow{border-image: url(%1);}").arg(menu_path);
    this->setStyleSheet("MainWindow{border-image: url(:/pic/video_menu.jpeg);}");

    //设置触摸事件
    setAttribute(Qt::WA_AcceptTouchEvents);
    this->installEventFilter(this);

    timer_anim = new QTimer(this);
     mypro = new QProcess(this);
    timer_init();
    push_button_init();
    qSlider_init();
    QListWidget_init();
    label_init();

}

MainWindow::~MainWindow()
{
    delete ui;
    delete prev_button;
        delete pause_button;
        delete next_button;
        delete exit_button;
        delete vol_button;
        delete file_button;

        delete timer_anim;
        delete timer_vol;
        delete timer_percent;
        delete timer_time;
        delete timer_TLength;
        delete timer_lab;

        delete mypro;
    delete LabelMainWindowBackground;
       delete labelDuration;
       delete labelSongsTitle;
}



void MainWindow::push_button_init()
{
    //上一曲
    prev_button = new myPushButton(prev_path,"",40,40);
    prev_button->setParent(this);
    prev_button->move(20,400);
    connect(prev_button,&QPushButton::clicked,[=](){
        prev_button->setEnabled(false);
        prev_button->zoom1();
        prev_button->zoom2();
        timer_anim->disconnect(timer_anim,&QTimer::timeout,nullptr,nullptr);
        timer_anim->start(500);
        connect(timer_anim,&QTimer::timeout,[=](){
        timer_anim->stop();
        if(curr_num>0)curr_num--;
        else {
            curr_num = video_info_list.size()-1;
        }
        mypro->close();
        QString cmd = QString("mplayer   -geometry 0:0 -zoom -x 600 -y 380 -slave -quiet %1").arg(video_info_list[curr_num].filePath());
        labelSongsTitle->setText(video_info_list[curr_num].fileName());
        mypro->start(cmd);
        prev_button->setEnabled(true);
        });
    });

    //暂停
    pause_button = new myPushButton(pause_path,"",40,40);
    pause_button->setParent(this);
    pause_button->move(80,400);
    connect(pause_button,&QPushButton::clicked,[=](){
        pause_button->setEnabled(false);
        pause_button->zoom1();
        pause_button->zoom2();
        timer_anim->disconnect(timer_anim,&QTimer::timeout,nullptr,nullptr);
        timer_anim->start(500);
        connect(timer_anim,&QTimer::timeout,[=](){
        timer_anim->stop();
        QPixmap pix;
        pause_flag =!pause_flag;
        if(pause_flag == 1)
        {
            system("killall -CONT mplayer");
            pix.load(pause_path);
        }else
        {
            system("killall -STOP mplayer");
            pix.load(play_path);
        }
        pause_button->setIcon(pix);
        pause_button->setEnabled(true);
        });
    });


    //下一曲
    next_button = new myPushButton(next_path,"",40,40);
    next_button->setParent(this);
    next_button->move(140,400);
    connect(next_button,&QPushButton::clicked,[=](){
        next_button->setEnabled(false);
        next_button->zoom1();
        next_button->zoom2();
        timer_anim->disconnect(timer_anim,&QTimer::timeout,nullptr,nullptr);
        timer_anim->start(500);
        connect(timer_anim,&QTimer::timeout,[=](){
        timer_anim->stop();
        mypro->close();
        if(curr_num>video_info_list.size()-1)curr_num = 0;
        else
        {
            curr_num++;
        }
        QString cmd = QString("mplayer  -geometry 0:0 -zoom -x 600 -y 380 -slave -quiet %1").arg(video_info_list[curr_num].filePath());
        labelSongsTitle->setText(video_info_list[curr_num].fileName());
        mypro->start(cmd);
        next_button->setEnabled(true);
        });
    });

    //返回
    exit_button = new myPushButton(exit_path,"",40,40);
    exit_button->setParent(this);
    exit_button->move(760,440);
    connect(exit_button,&QPushButton::clicked,[=](){
        exit_button->zoom1();
        exit_button->zoom2();
        exit_button->setEnabled(false);
        timer_anim->disconnect(timer_anim,&QTimer::timeout,nullptr,nullptr);
        timer_anim->start(500);
        connect(timer_anim,&QTimer::timeout,[=](){
        timer_anim->stop();
        mypro->close();
        exit_button->setEnabled(true);
        exit(1);
        });
    });

    //音量
    vol_button = new myPushButton(vol_path,"",40,40);
    vol_button->setParent(this);
    vol_button->move(720,400);
    connect(vol_button,&QPushButton::clicked,[=](){
        vol_button->setEnabled(false);
        vol_button->zoom1();
        vol_button->zoom2();
        timer_anim->disconnect(timer_anim,&QTimer::timeout,nullptr,nullptr);
        timer_anim->start(500);
        connect(timer_anim,&QTimer::timeout,[=](){
        timer_anim->stop();
        vol_button_flag = !vol_button_flag;
        volumeslider->setVisible(vol_button_flag);
        volumeslider->raise();
        vol_button->setEnabled(true);
        });
    });

    //打开文件
    file_button = new myPushButton(file_path,"",40,40);
    file_button->setParent(this);
    file_button->move(600,0);
    connect(file_button,&QPushButton::clicked,[=](){
        file_button->setEnabled(false);
        file_button->zoom1();
        file_button->zoom2();
        timer_anim->disconnect(timer_anim,&QTimer::timeout,nullptr,nullptr);
        timer_anim->start(500);
        connect(timer_anim,&QTimer::timeout,[=](){
        timer_anim->stop();
        QStringList msg = QFileDialog::getOpenFileNames(this,"Select one or more files to open","./video","video(*.mp4 *.avi)");
        for(int i = 0;i<msg.size() ;i++)
        {
            video_info_list.append(QFileInfo(msg[i]));
            videoList->addItem(QFileInfo(msg[i]).fileName());
        }
        file_button->setEnabled(true);
        });
    });
}
//设置列表
void MainWindow::QListWidget_init()
{

    videoList = new QListWidget(this);
    videoList->clear();
    videoList->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    videoList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    videoList->setFrameShape(QListWidget::NoFrame);
    videoList->setGeometry(600,20,200,385);

    QFont font;
    font.setPointSize(12);
    videoList->setFont(font);
    videoList->setStyleSheet(SONGSLIST_STYLE);

    QListWidgetItem *item = nullptr;

    scanVideo();

    for(int i = 0;i<video_info_list.size();i++)
    {
        videoList->addItem(video_info_list[i].fileName());
        item = videoList->item(i);
        item->setSizeHint(QSize((float)250/800*this->geometry().width(),(float)30/480*this->geometry().height()));
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    }
    videoList->setVisible(true);
    connect(videoList,&QListWidget::itemClicked,[=](QListWidgetItem *item){
        mypro->close();
        for(int i = 0;i< video_info_list.size();i++)
        {
            if(video_info_list[i].fileName().indexOf(item->text(),0) != -1)
            {
                curr_num = i;
                labelSongsTitle->setText(video_info_list[i].fileName());
            }
        }
        QString cmd = QString("mplayer  -geometry 0:0 -zoom -x 600 -y 380 -slave -quiet %1").arg(video_info_list[curr_num].filePath());
        mypro->start(cmd);
    });

}

void MainWindow::scanVideo()
{
    QDir dir1(video_path);
    QDir dir2(dir1.absolutePath());
    if(dir2.exists())
    {
        QStringList filter;
        filter << "*.avi";
        video_info_list = dir2.entryInfoList(filter,QDir::Files);
    }
    else qDebug() << "没有找到视频";
}


//进度条音量条
void MainWindow::qSlider_init()
{
    video_Slider = new QSlider(Qt::Horizontal,this);
    video_Slider->setGeometry(200,430,500,40);
    video_Slider->setFocusPolicy(Qt::NoFocus);
    video_Slider->setStyleSheet(MEDIASLIDER_STYLE);
    video_Slider->installEventFilter(this);
    video_Slider->setValue(0);
    connect(video_Slider,&QSlider::sliderReleased,[=](){
        int pos = video_Slider->value() - now_progress;
        pos = pos*(now_playtime*100/now_progress)/100;;
        QString msg = QString("seek %1\n").arg(pos);
        mypro->write(msg.toUtf8().data());
    });

    volumeslider = new QSlider(Qt::Vertical,this);
    volumeslider->setGeometry(730,250,40,150);
    volumeslider->setFocusPolicy(Qt::NoFocus);
    volumeslider->setVisible(false);
    volumeslider->setValue(100);
    volumeslider->setStyleSheet(VOLUMESLIDER_STYLE);
    volumeslider->installEventFilter(this);
    connect(volumeslider,&QSlider::valueChanged,[=](int value){

        QString msg = QString("volume %1 1\n").arg(value);
        mypro->write(msg.toUtf8().data());
    });

}

void MainWindow::label_init()
{
    QFont font;
    font.setPointSize(10);

    LabelMainWindowBackground = new QLabel(this);           //用于覆盖桌面写真背景，防止背景过亮


    labelDuration = new QLabel(this);
    labelDuration->setFont(font);
    labelDuration->setGeometry((float)570/800*this->geometry().width(),
                               (float)430/480*this->geometry().height(),
                               (float)120/800*this->geometry().width(),
                               (float)20/480*this->geometry().height());
    labelDuration->setText("00:00/00:00");
    labelDuration->setStyleSheet("QLabel{color:white;}QLabel{background:transparent}");
    labelDuration->setAlignment(Qt::AlignVCenter);



    labelSongsTitle = new QLabel(this);
    labelSongsTitle->setGeometry((float)212/800*this->geometry().width(),
                                 (float)416/480*this->geometry().height(),
                                 (float)350/800*this->geometry().width(),
                                 (float)25/480*this->geometry().height());
    labelSongsTitle->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    labelSongsTitle->setStyleSheet("QLabel{color:white;}QLabel{background:transparent}");
    font.setPointSize(10);
    labelSongsTitle->setFont(font);
}

void MainWindow::timer_init()
{


    timer_percent = new QTimer(this);
    timer_time = new QTimer(this);
    timer_TLength = new QTimer(this);

    timer_percent->start(400);
    connect(timer_percent,&QTimer::timeout,[=](){
        if(mypro->state() == QProcess::Running)
        {
            timer_percent->stop();
            timer_time->start(100);
            mypro->write("get_percent_pos\n");
        }

    });


    connect(timer_time,&QTimer::timeout,[=](){
        if(mypro->state() == QProcess::Running)
        {
        timer_time->stop();
        timer_TLength->start(100);
        mypro->write("get_time_pos\n");

        }
    });


    connect(timer_TLength,&QTimer::timeout,[=](){
        if(mypro->state() == QProcess::Running)
        {
        timer_TLength->stop();
        timer_percent->start(100);
        mypro->write("get_time_length\n");
        }
    });


    connect(mypro,&QProcess::readyReadStandardOutput,[=](){
        QString msg = mypro->readAll();
        //qDebug() << "mplayer:" <<msg;
        if(msg.indexOf("ANS_TIME_POSITION",0)!=-1)
        {
            msg = msg.section('=',1,1).trimmed();
            now_playtime = msg.toDouble();
            //qDebug() << "now_playtime:" <<now_playtime;
            QString text = QString("%1%2:%3%4/%5%6:%7%8")
                            .arg(now_playtime/60/10)
                            .arg(now_playtime/60%10)
                            .arg(now_playtime%60/10)
                            .arg(now_playtime%60%10)
                            .arg(time_length/60/10)
                            .arg(time_length/60%10)
                            .arg(time_length%60/10)
                            .arg(time_length%60%10);
            if(now_playtime != 0)
            {
                labelDuration->setText(text);
            }
        }
        else if(msg.indexOf("ANS_LENGTH",0)!=-1)//获取时间
            {
                msg = msg.section('=', 1, 1).trimmed();
                time_length = msg.toDouble();
                //qDebug() << "time_length:" <<time_length;
            }
        else if(msg.indexOf("ANS_PERCENT_POSITION=",0)!=-1)
        {
            last_progress = now_progress;
            msg = msg.section('=',1,1).trimmed();
            now_progress = msg.toInt();
            //qDebug() << "now_progress:" <<now_progress;
            if(now_progress != 0)
            {
                video_Slider->setValue(now_progress);

            }
            if(last_progress != now_progress)
            {
                QPixmap pix;
                pause_flag = 1;
                pix.load(pause_path);
                pause_button->setIcon(pix);
            }
        }
    });




}

bool MainWindow::eventFilter(QObject *watched,QEvent *event)
{
    QPoint point;
    if(watched == this)
    {
        QTouchEvent* touch = static_cast<QTouchEvent*>(event);

        switch (event->type())
        {

           case QEvent::TouchBegin:
            point.setY(touch->touchPoints().at(0).pos().y());
            point.setX(touch->touchPoints().at(0).pos().x());
            if(point.x() < 600 && point.y() < 380)
            {
                video_fill_flag = !video_fill_flag;
                if(video_fill_flag)
                {
                    mypro->close();
                    QString cmd = QString("mplayer  -geometry 0:0 -zoom -x 800 -y 480 -slave -quiet %1").arg(video_info_list[curr_num].filePath());
                    mypro->start(cmd);
                    video_Slider->hide();
                    labelDuration->hide();
                    pause_button->hide();
                }else
                {
                    mypro->close();
                    QString cmd = QString("mplayer  -geometry 0:0 -zoom -x 600 -y 380 -slave -quiet %1").arg(video_info_list[curr_num].filePath());
                    this->show();
                    video_Slider->show();
                    labelDuration->show();
                    pause_button->show();
                    videoList->show();
                    mypro->start(cmd);
                }
            }
            event->accept();
            break;
        default:break;
        }
    }
    return QWidget::eventFilter(watched,event);//将事件传递给父类
}









