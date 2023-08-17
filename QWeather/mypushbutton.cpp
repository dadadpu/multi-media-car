
#include "mypushbutton.h"
#include <QDebug>

myPushButton:: myPushButton(QString normal_path,QString press_path,int pixwidth,int pixheight)
{
   this->normal_path = normal_path;
   this->press_path = press_path;

   QPixmap pix;
   bool ret = pix.load(this->normal_path);
   if(!ret)
   {
       qDebug() << "图片加载失败";
       return ;
   }

   this->setFixedSize(pixwidth,pixheight);
   this->setStyleSheet("QPushButton{border:0px;}");
   this->setIcon(pix);

   this->setIconSize(QSize(pixwidth,pixheight));
   this->setFocusPolicy(Qt::NoFocus);

   this->animation = new QPropertyAnimation(this,"geometry");

}

void myPushButton::zoom1()
{
    // 设置动画的持续时间为200毫秒
    this->animation->setDuration(200);

    // 设置动画的起始值为当前按钮的位置和尺寸
    this->animation->setStartValue(QRect(this->x(), this->y()+10, this->width(), this->height()));

    // 设置动画的结束值为当前按钮的位置和尺寸
    this->animation->setEndValue(QRect(this->x(), this->y(), this->width(), this->height()));

    // 设置动画的缓动曲线为OutBounce，使得动画效果更加逼真
    animation->setEasingCurve(QEasingCurve::OutBounce);

    // 启动动画
    animation->start();
}


void myPushButton::zoom2()
{
    // 设置动画的持续时间为200毫秒
    this->animation->setDuration(200);

    // 设置动画的起始值为当前按钮的位置和尺寸
    this->animation->setStartValue(QRect(this->x(), this->y(), this->width(), this->height()));

    // 设置动画的结束值为当前按钮的位置向上偏移10个像素后的位置和尺寸
    this->animation->setEndValue(QRect(this->x(), this->y()-10, this->width(), this->height()));



    // 设置动画的缓动曲线为InElastic，使得动画有一定的弹性效果
    animation->setEasingCurve(QEasingCurve::InElastic);

    // 启动动画
    animation->start();
}


void myPushButton::mousePressEvent(QMouseEvent *e)
{
    // 检查是否设置了按下状态的图片路径
    if(this->press_path != nullptr)
    {
        QPixmap pix;
        // 加载按下状态的图片
        bool ret = pix.load(this->press_path);
        if(!ret)
        {
            qDebug() << "图片加载失败";
            return;
        }
        // 设置按钮的固定大小为图片的宽度和高度
        this->setFixedSize(pix.width(), pix.height());
        // 设置按钮样式为无边框
        this->setStyleSheet("QPushButton{border:0px;}");
        // 设置按钮的图标为按下状态的图片
        this->setIcon(pix);
        // 设置图标的大小为图片的宽度和高度
        this->setIconSize(QSize(pix.width(), pix.height()));
    }
    // 调用父类的鼠标按下事件处理函数
    return QPushButton::mousePressEvent(e);
}

void myPushButton::mouseReleaseEvent(QMouseEvent *e)
{
    // 检查是否设置了按下状态的图片路径
    if(this->press_path != nullptr)
    {
        QPixmap pix;
        // 加载正常状态的图片
        bool ret = pix.load(this->normal_path);
        if(!ret)
        {
            qDebug() << "图片加载失败";
            return;
        }
        // 设置按钮的固定大小为图片的宽度和高度
        this->setFixedSize(pix.width(), pix.height());
        // 设置按钮样式为无边框
        this->setStyleSheet("QPushButton{border:0px;}");
        // 设置按钮的图标为正常状态的图片
        this->setIcon(pix);
        // 设置图标的大小为图片的宽度和高度
        this->setIconSize(QSize(pix.width(), pix.height()));
    }
    // 调用父类的鼠标释放事件处理函数
    return QPushButton::mouseReleaseEvent(e);
}




















