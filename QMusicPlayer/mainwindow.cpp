#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "config.h"
#define menu_path ":/pic/music_menu.jpeg"
#define ljj_path ":/pic/ljj.jpg"
#define exit_path ":/pic/exit.png"
#define next_path ":/pic/next.png"
#define openfile_path ":/pic/openfile.png"
#define pause_path ":/pic/pause.png"
#define play_path ":/pic/play.png"
#define prev_path ":/pic/prev.png"
#define vol_path ":/pic/vol.png"
#define zjl_path ":/pic/zjl.jpeg"
#define music_path "music/"
#define lrc_path "lrc/"
#define photo_path "photo/"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(800,480);
    this->setWindowTitle("音乐播放器V1.0");
    QString musicPath = menu_path;
    this->setStyleSheet("MainWindow{border-image: url("+musicPath+");}");

    timerInit();//计时器初始化
    PushButton_init();
    QSlider_init();
    QListWidget_init();
    Label_init();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete mypro;
    delete timer_animt;
    delete timer_percent;
    delete timer_time;
    delete timer_TLength;
    delete timer_lab;
    delete timer_openfile;
    delete music_Slider,volumeslider;
    delete prev_button;
    delete pause_button;
    delete next_button;
    delete exit_button;
    delete vol_button;
    delete  file_button;
    delete  songList;
    for (int i = 0; i < labelArr.size(); i++) {
        delete labelArr[i];
    }
    labelArr.clear();
    delete  LabelMainWindowBackground;
    delete  labelDuration;
    delete  labelSongsTitle;

}


    //初始化歌曲列表
void MainWindow::QListWidget_init()
{
    //初始化列表
    songList = new QListWidget(this);
    songList->clear();
    songList->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//设置垂直滚动条关闭
    songList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//横向滚动条关闭
    songList->setFrameShape(QListWidget::NoFrame);//没有边框
    songList->setGeometry(0,20,250,385);

    QFont font;
    font.setPointSize(12);
    songList->setFont(font);
    songList->setStyleSheet(SONGSLIST_STYLE);


    QListWidgetItem *item = nullptr;
    //扫描默认目录下的音乐
    scanMusic();
    //将扫描到的音乐加入列表
    for(int i = 0; i < music_info_list.size() ; i++)
    {
        songList->addItem(music_info_list[i].fileName());
        item = songList->item(i);
        item->setSizeHint(QSize((float)250/800*this->geometry().width(),(float)30/480*this->geometry().height()));
    }
    songList->setVisible(true);
    //绑定列表音乐播放事件
    connect(songList,&QListWidget::itemClicked,[=](QListWidgetItem *item){
        mypro->close();
        for(int i = 0;i < music_info_list.size(); i++)
        {
            if(music_info_list[i].fileName().indexOf(item->text(),0)!=-1)
            {
                curr_num = i;
                labelSongsTitle->setText(music_info_list[curr_num].fileName());
            }
        }
        QString cmd = QString("mplayer -quiet -slave %1").arg(music_info_list[curr_num].filePath());
        mypro->start(cmd);
        pause_flag = true;
        //加载歌词
        loading_lrc = 1;
        lrcArr_init();

    });


}

//扫描本地路径音乐
void MainWindow::scanMusic()
{
    //创建目录
    QDir dir1(music_path);
    QDir dir2(dir1.absolutePath());//转化为绝对路径
    if(dir2.exists())//如果目录存在
    {
        QStringList filter;//过滤条件
        filter << "*.mp3";
        music_info_list = dir2.entryInfoList(filter,QDir::Files);
        if(music_info_list.count() == 0)
        {
             qDebug() << "当前目录未找到音乐";
        }
    }


}

//歌词容器初始化
void MainWindow::lrcArr_init()
{
    if(loading_lrc == 0)
    {
        return;
    }
    //清空歌词容器
    lrcArr.clear();
    for(int i = 0;i < 7;i++)
    {
        labelArr[i]->clear();
    }

    //获取歌词文件，设置写真
    QString lrcFile = labelSongsTitle->text();
    qDebug() << "lrcArr_init_labelSongsTitle:"  << lrcFile;
    lrcFile.remove(QString("mp3"),Qt::CaseInsensitive);
    lrcFile ="lrc/" + lrcFile + "lrc";
    qDebug() << "lrcArr_init_lrcFile:"  << lrcFile;
    if(lrcFile.indexOf("zjl",0)!=-1)//显示写真
    {

        QString zjl = "zjl.jpeg" ;
        zjl = photo_path + zjl;
        qDebug() << "lrcArr_init_photo:" << zjl;
        this->setStyleSheet("MainWindow{border-image: url("+zjl+");}");
    }else if(lrcFile.indexOf("ljj",0)!=-1)
    {
        qDebug() << "lrcArr_init_设置林俊杰写真";
        QString ljj = "ljj.jpg" ;
        ljj = photo_path + ljj;
        qDebug() << "lrcArr_init_photo:" << ljj;
        this->setStyleSheet("MainWindow{border-image: url("+ljj+");}");
    }else
    {
        qDebug() << "lrcArr_init_未找到写真_设置为默认背景";
        QString music_menu = menu_path;
         this->setStyleSheet("MainWindow{border-image: url("+music_menu+");}");
    }
    this->show();


    //读取歌词
    QFile file(lrcFile);
    if(!file.exists())
    {
        qDebug() << "找不到歌词";
        return;
    }
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "无法打开歌词文件";
        return;
    }

    //丢弃前两行
    QTextCodec *code = QTextCodec::codecForName("UTF-8");
    QTextStream in(&file);
    in.setCodec(code);
    in.readLine();
    in.readLine();
    QString line;
    LrcFormat info;//歌词结构体

    while (!in.atEnd())
    {
        line = in.readLine();
        info.time = line.mid(1,5);
        info.content = line.mid(10);
        lrcArr.append(info);

        //qDebug() << info.time <<info.content;
    }

    file.close();
    //设置其
    int count = lrcArr.count();
    if(count >0)
        labelArr[4]->setText(lrcArr[0].content);
    if(count >1)
        labelArr[5]->setText(lrcArr[1].content);
    if(count >2)
        labelArr[6]->setText(lrcArr[2].content);
    qDebug() << "歌词初始化成功";


}

void MainWindow::timerInit()
{
    timer_percent = new QTimer(this);
    timer_time = new QTimer(this);
    timer_TLength = new QTimer(this);

    timer_percent->start(400);

    //获取播放进度百分比
    connect(timer_percent,&QTimer::timeout,[=](){
        if(mypro->state() == QProcess::Running)
        {
            timer_percent->stop();
            mypro->write("get_percent_pos\n");
            timer_time->start(100);
        }
    });
    //当前播放时间
    connect(timer_time,&QTimer::timeout,[=](){
        if(mypro->state() == QProcess::Running)
        {
            timer_time->stop();
            mypro->write("get_time_pos\n");
            timer_TLength->start(100);
        }
    });
    //音频总时长
    connect(timer_TLength,&QTimer::timeout,[=](){
        if(mypro->state() == QProcess::Running)
        {
            timer_TLength->stop();
            mypro->write("get_time_length\n");
            timer_percent->start(100);
        }
    });

    QLabel *lab = new QLabel(this);
    lab->setGeometry(170,440,50,20);
    QFont f("宋体",10,50);
    lab->setFont(f);
    lab->setStyleSheet("color:white;");

    mypro = new QProcess(this);
    connect(mypro,&QProcess::readyReadStandardOutput,[=](){

       QString msg = mypro->readAll();

       if(msg.indexOf("ANS_TIME_POSITION",0)!=-1)
       {
           last_playTime = now_playTime;
           msg = msg.section('=',1,1).trimmed();//从字符串msg中提取出等号后面的部分，并移除首尾的空白字符。
           if(msg.toDouble() == 0 &&last_playTime !=0)
           {

           }else
           {
               now_playTime = msg.toDouble();
           }

           QString text = QString("%1%2:%3%4/%5%6:%7%8")
                   .arg(now_playTime/60/10)
                   .arg(now_playTime/60%10)
                   .arg(now_playTime%60/10)
                   .arg(now_playTime%60%10)
                   .arg(time_length/60/10)
                   .arg(time_length/60%10)
                   .arg(time_length%60/10)
                   .arg(time_length%60%10);
           if(now_playTime != 0)
           {
               labelDuration->setText(text);
           }
               qDebug() << now_playTime;

       }else if(msg.indexOf("ANS_LENGTH",0)!=-1)//获取播放的总时长
       {
           msg = msg.section('=',1,1).trimmed();
           time_length = msg.toDouble();
       }else if (msg.indexOf("ANS_PERCENT_POSITION=",0)!=-1) //获取进度
       {
           last_progress = now_progress;
           msg = msg.section('=',1,1).trimmed();
           now_progress = msg.toInt();
           if(now_progress!= 0)
           {
               music_Slider->setValue(now_progress);
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

void MainWindow::PushButton_init()
{


    timer_animt = new QTimer(this);
    prev_button = new myPushButton(prev_path,"",40,40);
    prev_button->setParent(this);
    prev_button->move(20,400);

    //播放上一首
    connect(prev_button,&myPushButton::clicked,[=](){
        prev_button->zoom1();
        prev_button->zoom2();
        disconnect(timer_animt, &QTimer::timeout, nullptr, nullptr);
        timer_animt->start(500);
        prev_button->setEnabled(false);
        connect(timer_animt,&QTimer::timeout,[=](){
            timer_animt->stop();
            if(curr_num>0)curr_num--;
            else curr_num=music_info_list.size()-1;
            mypro->close();
            QString cmd = QString("mplayer -quiet -slave %1").arg(music_info_list[curr_num].filePath());
            labelSongsTitle->setText(music_info_list[curr_num].fileName());
            mypro->start(cmd);
            lrcArr_init();
            qDebug() << "上一首";
            prev_button->setEnabled(true);

        });
    });

    //暂停播放按钮
    pause_button = new myPushButton(pause_path,"",40,40);
    pause_button->setParent(this);
    pause_button->move(80,400);

    connect(pause_button,&myPushButton::clicked,[=](){
        pause_button->zoom1();
        pause_button->zoom2();
        disconnect(timer_animt, &QTimer::timeout, nullptr, nullptr);
        // 将按钮设置为不可点击状态
        pause_button->setEnabled(false);
        timer_animt->start(500);
        connect(timer_animt,&QTimer::timeout,[=](){
            timer_animt->stop();
            pause_flag = !pause_flag;
            qDebug() << pause_flag;
            QPixmap pix;
            int playtime;
            if(pause_flag == true)
            {
                system("killall -CONT mplayer");
                pix.load(pause_path);
                qDebug() << "继续播放";
            }
            else if(pause_flag == false)
            {
                playtime = now_playTime;
                system("killall -STOP mplayer");
                pix.load(play_path);
                qDebug() << "暂停播放";
            }
            pause_button->setIcon(pix);
            pause_button->setEnabled(true);


        });
    });

    //下一曲按钮
        next_button=new myPushButton(":/pic/next.png","",40,40);
        next_button->setParent(this);
        next_button->move(140,400);

        //下一曲按钮下处理
        connect(next_button,&myPushButton::clicked,[=](){
            next_button->zoom1();//弹跳
            next_button->zoom2();
            disconnect(timer_animt, &QTimer::timeout, nullptr, nullptr);
            timer_animt->start(500);
            next_button->setEnabled(false);
            connect(timer_animt,&QTimer::timeout,[=](){
                    timer_animt->stop();
                    if(curr_num<music_info_list.size()-1)curr_num++;
                    else curr_num = 0;
                    mypro->close();
                    QString cmd = QString("mplayer -quiet -slave %1").arg(music_info_list[curr_num].filePath());
                    labelSongsTitle->setText(music_info_list[curr_num].fileName());
                    mypro->start(cmd);
                    lrcArr_init();
                    qDebug() << "下一首";
                    next_button->setEnabled(true);
                });

            });


        //退出按钮
        exit_button = new myPushButton(exit_path,"",40,40);
        exit_button->setParent(this);
        exit_button->move(760,440);
        connect(exit_button,&myPushButton::clicked,[=](){
            exit_button->zoom1();
            exit_button->zoom2();
            disconnect(timer_animt, &QTimer::timeout, nullptr, nullptr);
            timer_animt->start(500);
             exit_button->setEnabled(false);
            connect(timer_animt,&QTimer::timeout,[=](){
                timer_animt->stop();
                mypro->close();
                qDebug() << "退出音乐播放器";
                 exit_button->setEnabled(true);
                exit(1);


            });
        });


        //音量调节按钮
        vol_button = new myPushButton(vol_path,"",40,40);
        vol_button->setParent(this);
        vol_button->move(720,400);
        connect(vol_button,&myPushButton::clicked,[=](){
            vol_button->zoom1();
            vol_button->zoom2();
            disconnect(timer_animt, &QTimer::timeout, nullptr, nullptr);
            timer_animt->start(500);
            vol_button_flag = !vol_button_flag;
            connect(timer_animt,&QTimer::timeout,[=](){
                timer_animt->stop();
                if(vol_button_flag == 1)
                {
                    volumeslider->raise();
                    volumeslider->setVisible(vol_button_flag);
                    qDebug() << "打开音量条";
                }else if(vol_button_flag == 0)
                {
                    volumeslider->setVisible(vol_button_flag);
                    qDebug() << "隐藏音量条";
                }

            });
        });

        //选择文件按钮
           timer_openfile = new QTimer(this);
           file_button=new myPushButton(openfile_path,"",30,25);
           file_button->setParent(this);
           file_button->move(200,0);
           connect(file_button,&myPushButton::clicked,[=](){
               file_button->zoom1();
               file_button->zoom2();
               disconnect(timer_animt, &QTimer::timeout, nullptr, nullptr);
               timer_openfile->start(500);
               connect(timer_openfile,&QTimer::timeout,[=](){
                   timer_openfile->stop();
                   QStringList msg = QFileDialog::getOpenFileNames(this,"Select one or more files to open",
                                                                   "./","Images (*.mp3 *.wav)");

                   for(int i = 0;i<msg.size();i++)
                   {//c查找是否有相同歌曲
                     QList<QListWidgetItem*>  matchingItems =  songList->findItems(QFileInfo(msg[i]).fileName(),Qt::MatchExactly);
                       if(matchingItems.isEmpty())
                       {
                           music_info_list.append(QFileInfo(msg[i]));
                           songList->addItem(QFileInfo(msg[i]).fileName());
                           qDebug() << "导入歌曲："<<QFileInfo(msg[i]).fileName();
                       }else
                       {
                            qDebug() << "歌曲已存在"<<QFileInfo(msg[i]).fileName();
                       }

                   }
               });
           });

}

//进度条和音量条初始化
void MainWindow::QSlider_init()
{
    //进度条
    music_Slider = new QSlider(Qt::Horizontal,this);
    music_Slider->setGeometry(200,430,500,40);
    music_Slider->setFocusPolicy(Qt::NoFocus);
    music_Slider->setStyleSheet(MEDIASLIDER_STYLE);

    //music_Slider->installEventFilter(this);//安装事件过滤其
    music_Slider->setValue(0);
    connect(music_Slider,&QSlider::sliderReleased,[=](){
        int pos = music_Slider->value()-now_progress;
        QString msg = QString("seek %1\n").arg(pos);
        mypro->write(msg.toUtf8().data());
    });
    //音量
    volumeslider = new QSlider(Qt::Vertical,this);
    volumeslider->setGeometry(730,250,40,150);
    volumeslider->setFocusPolicy(Qt::NoFocus);
    //volumeslider->setStyleSheet(VOLUMESLIDER_STYLE);
    volumeslider->setVisible(false);
    //volumeslider->installEventFilter(this);
    volumeslider->setValue(100);
    volumeslider->raise();
    connect(volumeslider,&QSlider::valueChanged,[=](int value){
        QString msg = QString("volume %1 1\n").arg(value);
        mypro->write(msg.toUtf8().data());
    });
}

void MainWindow::Label_init()
{
    //歌词标签初始化
    QFont font;
    font.setPointSize(12);
    LabelMainWindowBackground = new QLabel(this);//用于覆盖桌面写真背景，防止背景过亮
    labelDuration = new QLabel(this);
    labelDuration->setFont(font);
    //相当于相对位置，在其他尺寸中不会乱
    labelDuration->setGeometry((float)570/800*this->geometry().width(),
                                (float)430/480*this->geometry().height(),
                               (float)120/800*this->geometry().width(),
                               (float)20/480*this->geometry().height());
    labelDuration->setText("00:00/00:00");
    labelDuration->setStyleSheet("QLabel{color:white;}QLabel{background:transparent}");
    labelDuration->setAlignment(Qt::AlignVCenter);//确保 label 中文本的垂直位置居中，而不是默认的上对齐或下对齐

    //正在播放歌曲名.mp3
    labelSongsTitle = new QLabel(this);
    labelSongsTitle->setGeometry(212,416,350,25);
    labelSongsTitle->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);//左对齐并垂直居中
    labelSongsTitle->setText("生命随音乐律动!");
    font.setPointSize(10);
    labelSongsTitle->setFont(font);

    //歌词label
    labelArr.clear();
    font.setPointSize(14);
    for(int i = 0; i < 7;i++)
    {
        QLabel *label = new QLabel(this);
        label->setFont(font);
        label->setStyleSheet("QLabel{color:white;}QLabel{background:transparent}");
        label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        labelArr.append(label);
    }

    //正在播放的歌词
    labelArr[3]->setStyleSheet("QLabel{color:#faff02;}QLabel{background:transparent}");
    labelArr[3]->setText("暂未播放歌曲");

    for(int i = 0;i< 7 ;i++)
    {
        labelArr[i]->setGeometry((float)250/800*this->geometry().width(),
                                         (float)(50 + i*40)/480*this->geometry().height(),
                                         (float)550/800*this->geometry().width(),
                                         (float)30/480*this->geometry().height());//250, (50 + i*40), 550, 30
    }

    timer_lab = new QTimer(this);
    timer_lab->start(200);
    connect(timer_lab,&QTimer::timeout,this,&MainWindow::lrcArrDisplay);
}


//歌词显示
void MainWindow::lrcArrDisplay()
{

       //播放结束
       if(last_playTime == now_playTime)
       {
           return;
       }
       //提取分秒
       int second = now_playTime%60;
       int minute = now_playTime/60;

       QString strLabel;
       strLabel.clear();

       if(minute >= 10)
       {
           strLabel = QString::number(minute,10);
       }else
       {
           strLabel = "0"+QString::number(minute,10);
       }


       if(second >= 10)
       {
           strLabel = strLabel +":" +QString::number(second,10);
       }else
       {
           strLabel = strLabel +":0" +QString::number(second,10);
       }


       //设置歌词
       int count = lrcArr.count();
       for(int i = 0;i < count;i++)
       {
           //如果时间重合
           if(lrcArr[i].time == strLabel)
           {
               labelArr[3]->setText(lrcArr[i].content);//正在唱的歌词
               if(i-1 >= 0)//有上一句
               {
                   labelArr[2]->setText(lrcArr[i-1].content);
               }else
               {
                   labelArr[2]->clear();
               }

               if(i-2 >= 0)//有上两句
               {
                   labelArr[1]->setText(lrcArr[i-2].content);
               }else
               {
                   labelArr[1]->clear();
               }

               if (i-3 >= 0)//有前三句
               {
                   labelArr[0]->setText(lrcArr[i-3].content);
               }
               else
               {
                   labelArr[0]->clear();
               }

               if (i+1 < count)//有后一句
                   labelArr[4]->setText(lrcArr[i+1].content);
               else
                   labelArr[4]->clear();

               if (i+2 < count)//有后两句
                   labelArr[5]->setText(lrcArr[i+2].content);
               else
                   labelArr[5]->clear();

               if (i+3 < count)//有后四句
                   labelArr[6]->setText(lrcArr[i+3].content);
               else
                   labelArr[6]->clear();
           }
       }
}





















