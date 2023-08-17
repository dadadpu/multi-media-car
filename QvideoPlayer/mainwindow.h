#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "mypushbutton.h"
#include <QTimer>
#include <QMainWindow>
#include <QProcess>
#include <QFile>
#include <QFileInfoList>
#include <QSlider>
#include <QListWidget>
#include <QLabel>
#include <QFileDialog>
#include <QDebug>
#include "config.h"
#include <QTouchEvent>
#include <QVBoxLayout>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void push_button_init();
    void timer_init();
    void QListWidget_init();
    void scanVideo();
    void qSlider_init();
    void label_init();



private:
    Ui::MainWindow *ui;
    myPushButton * prev_button,* pause_button,* next_button,*exit_button,* vol_button,*file_button;
    QTimer *timer_anim,*timer_vol,*timer_percent,*timer_time,*timer_TLength,*timer_lab;
    QProcess *mypro;
    bool eventFilter(QObject *watched,QEvent *event);

    unsigned long long windowId;

    //播放列表
    QFileInfoList video_info_list;
    //当前播放
    int curr_num;
    //播放状态
    bool pause_flag;
    //播放进度
    int now_playtime,time_length,now_progress,last_progress;
    //音量显示开关
    bool vol_button_flag = false;
    bool video_fill_flag = false;
    //进度条
    QSlider* video_Slider,*volumeslider;
    //歌单播放列表
    QListWidget * videoList;
    //显示标签
    QVector<QLabel *> labelArr;
    QLabel* LabelMainWindowBackground;
    QLabel* labelDuration;
    QLabel* labelSongsTitle;

};

#endif // MAINWINDOW_H
