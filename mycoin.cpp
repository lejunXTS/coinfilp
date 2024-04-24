#include "mycoin.h"
#include <QTimer>

// MyCoin::MyCoin(QWidget *parent)
//     : QPushButton(parent)
// {}


//参数代表传入金币路径还是银币路径
MyCoin::MyCoin(QString btnImg)
{
    QPixmap pixmap;

    bool ret = pixmap.load(btnImg);

    if(!ret)
    {
        QString str = QString("图片 %1 加载失败").arg(btnImg);
        qDebug() << str << "加载图片失败!";
    }

    this->setFixedSize(pixmap.width(), pixmap.height() );

    this->setStyleSheet("QPushButton{border:0px;}");

    this->setIcon(pixmap);

    this->setIconSize(QSize(pixmap.width(),pixmap.height()));

    //初始化定时器
    timer1 = new QTimer(this);

    timer2 = new QTimer(this);


    //监听正面翻反面的信号  并且翻转金币//////////////////////////////////////////////////////////////
    connect(timer1,&QTimer::timeout,[=](){

        QPixmap pixmap;

        QString str = QString(":/res/Coin000%1.png").arg(this->min++);

        pixmap.load(str);

        this->setFixedSize(pixmap.width(),pixmap.height() );

        this->setStyleSheet("QPushButton{border:0px;}");

        this->setIcon(pixmap);

        this->setIconSize(QSize(pixmap.width(),pixmap.height()));

        //如果大于最大值，重置最小值，并停止定时器（翻完了）
        if(this->min > this->max)
        {
            this->min = 1;

            //停止做动画
            this->isAnimation  = false;

            timer1->stop();
        }
    });

    //监听反面翻正面的信号  并且翻转金币
    connect(timer2,&QTimer::timeout,[=](){

        QPixmap pixmap;

        QString str = QString(":/res/Coin000%1.png").arg((this->max)-- );

        pixmap.load(str);

        this->setFixedSize(pixmap.width(),pixmap.height() );

        this->setStyleSheet("QPushButton{border:0px;}");

        this->setIcon(pixmap);

        this->setIconSize(QSize(pixmap.width(),pixmap.height()));

        //如果小于最小值，重置最大值，并停止定时器（翻完了）
        if(this->max < this->min)
        {
            this->max = 8;

            //停止做动画
            this->isAnimation  = false;

            timer2->stop();
        }
    });



}


void MyCoin::mousePressEvent(QMouseEvent *e)
{
    if(this->isAnimation)
    {
        return;
    }
    else if(isWin == true)
    {
        return;
    }
    else
    {
        return QPushButton::mousePressEvent(e);
    }
}




//改变正反面标志的方法  执行翻转效果
void MyCoin::changeFlag()
{
    if(this->flag) //如果是正面，执行下列代码
    {
        //开启正面翻反面的定时器
        timer1->start(30);

        //开始做动画
        this->isAnimation  = true;

        this->flag = false;
    }
    else //反面执行下列代码
    {
        timer2->start(30);

        //开始做动画
        this->isAnimation  = true;

        this->flag = true;
    }
}

