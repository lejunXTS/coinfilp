#ifndef MYCOIN_H
#define MYCOIN_H

#include <QPushButton>

class MyCoin : public QPushButton
{
    Q_OBJECT
public:
    //explicit MyCoin(QWidget *parent = nullptr);

    //参数代表传入金币路径还是银币路径
    MyCoin(QString btnImg);

    //金币的属性////////

    //x坐标
    int posX;

    //y坐标
    int posY;

    //正反标志
    bool flag;

    //改变标志的方法  执行翻转效果
    void changeFlag();

    //正面翻反面 定时器
    QTimer *timer1;

    //反面翻正面 定时器
    QTimer *timer2;

    //最小图片
    int min = 1;

    //最大图片
    int max = 8;

    //是否在做翻转动画的标志
    bool isAnimation  = false;

    //重写按下
    void mousePressEvent(QMouseEvent *e);

    //是否胜利的标志
    bool isWin = false;


signals:
};

#endif // MYCOIN_H
