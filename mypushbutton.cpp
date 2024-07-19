#include "mypushbutton.h"
#include <QDebug>
#include <QPropertyAnimation>

// MyPushButton::MyPushButton(QWidget *parent)
//     : QWidget{parent}
// {}


//构造函数 参数1 正常显示的图片路径  参数2 按下后显示的图片路径
MyPushButton::MyPushButton(QString normalImg, QString pressImg)
{
    this->normalImgPath = normalImg;

    this->pressImgPath = pressImg;

    QPixmap pix;

    bool ret = pix.load(normalImg);

    if (!ret)
    {
        qDebug()<<"图片加载失败";
        return;
    }

    //设置图片的固定尺寸
    this->setFixedSize(pix.width(), pix.height() );

    //设置不规则图片的样式表
    this->setStyleSheet("QPushButton{border:0px;}");

    //设置图标
    this->setIcon(pix);

    //设置图标大小
    this->setIconSize(QSize(pix.width(),pix.height()));
}

//弹跳特效  向下跳
void MyPushButton::zoom1()
{
    //创建动画对象
    QPropertyAnimation * animation1 = new QPropertyAnimation(this,"geometry");

    //设置动画时间间隔，单位毫秒
    animation1->setDuration(200);

    //起始位置
    animation1->setStartValue(QRect(this->x(),this->y(),this->width(),this->height()));

    //结束位置
    animation1->setEndValue(QRect(this->x(),this->y()+10,this->width(),this->height()));

    //设置弹跳缓和曲线，QEasingCurve::OutBounce 为弹跳效果
    animation1->setEasingCurve(QEasingCurve::OutBounce);

    //开始执行动画
    animation1->start();
}

//弹跳特效  向上跳
void MyPushButton::zoom2()
{
    QPropertyAnimation * animation1 =  new QPropertyAnimation(this,"geometry");

    animation1->setDuration(200);

    animation1->setStartValue(QRect(this->x(),this->y()+10,this->width(),this->height()));

    animation1->setEndValue(QRect(this->x(),this->y(),this->width(),this->height()));

    animation1->setEasingCurve(QEasingCurve::OutBounce);

    animation1->start();
}

//重写鼠标按下事件
void MyPushButton::mousePressEvent(QMouseEvent *e)
{
    if (pressImgPath != "") //传入按下图片路径不为空，说明需要显示按下状态，切换图片，
    {
        QPixmap pixmap;

        bool ret = pixmap.load(pressImgPath);

        if (!ret)
        {
            qDebug() << pressImgPath << "加载图片失败!";
        }

        this->setFixedSize(pixmap.width(), pixmap.height() );

        this->setStyleSheet("QPushButton{border:0px;}");

        this->setIcon(pixmap);

        this->setIconSize(QSize(pixmap.width(),pixmap.height()));
    }
    //交给父类执行其他内容
    return QPushButton::mousePressEvent(e);
}

//重写鼠标释放事件
void MyPushButton::mouseReleaseEvent(QMouseEvent *e)
{
    if(normalImgPath != "") //选中路径不为空，显示选中图片
    {
        QPixmap pixmap;
        bool ret = pixmap.load(normalImgPath);
        if(!ret)
        {
            qDebug() << normalImgPath << "加载图片失败!";
        }
        this->setFixedSize( pixmap.width(), pixmap.height() );
        this->setStyleSheet("QPushButton{border:0px;}");
        this->setIcon(pixmap);
        this->setIconSize(QSize(pixmap.width(),pixmap.height()));
    }
    //交给父类执行 释放事件
    return QPushButton::mouseReleaseEvent(e);
}


