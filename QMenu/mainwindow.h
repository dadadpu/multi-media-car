#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mypushbutton.h"
#include <QMainWindow>
#include <QProcess>
#include <QTimer>
#include <QLabel>
#include <QTime>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    bool eventFilter(QObject *watched,QEvent *event);
    ~MainWindow();

private:

    Ui::MainWindow *ui;
    QProcess *mypro;
    QTimer *timer_anim,*mouse_timer,*update_timer;
    QLabel *time_label;
    QLabel *date_label;
    myPushButton *music_button,*video_button,*map_button,*weather_button;



public slots:
    void updateTime();
};

#endif // MAINWINDOW_H
