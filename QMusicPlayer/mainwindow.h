#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mypushbutton.h"
#include <QMainWindow>
#include <QFileDialog>
#include <QTimer>
#include <QProcess>
#include <QSlider>
#include <QListWidget>
#include <QVector>
#include <QLabel>
#include <QString>
#include <QDebug>
#include <QTextCodec>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void QListWidget_init();
    void scanMusic();
    void lrcArr_init();
    void timerInit();
    void PushButton_init();
    void QSlider_init();
    void Label_init();

private:
    Ui::MainWindow *ui;
    //播放列表
    QFileInfoList music_info_list;
    //定时器
    QTimer *timer_animt,*timer_percent,*timer_time,*timer_TLength,*timer_lab,*timer_openfile;

    //当前播放
    int curr_num;
    //音量显示开关
    bool vol_button_flag = 0;
    //当前播放暂停状态 1，0
    bool pause_flag = 0 ;
    //播放时间，进度
    int now_playTime,last_playTime,time_length,now_progress,last_progress,loading_lrc = 1;
    //进程
    QProcess *mypro;
    //进度条
    QSlider *music_Slider,*volumeslider;
    //按钮
    myPushButton *prev_button,*pause_button,*next_button,*exit_button,*vol_button,*file_button;
    //播放列表
    QListWidget *songList;
    //歌词显示标签
    QVector<QLabel *> labelArr;
    QLabel *LabelMainWindowBackground;
    QLabel *labelDuration;
    QLabel *labelSongsTitle;

    typedef struct LrcParseFormat {
        QString time;
        QString content;
    }LrcFormat;

    QVector<LrcFormat> lrcArr;//歌词


public slots:
   void lrcArrDisplay();
};

#endif // MAINWINDOW_H
