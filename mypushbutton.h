#ifndef MYPUSHBUTTON_H
#define MYPUSHBUTTON_H

#include <QPushButton>

class MyPushButton : public QPushButton
{
    Q_OBJECT
public:
    //explicit MyPushButton(QWidget *parent = nullptr);

    //构造函数 参数1 正常显示的图片路径  参数2 按下后显示的图片路径
    MyPushButton(QString normalImg, QString pressImg = "");

    //成员属性  保存用户传入的默认显示路径 以及按下后显示的图片路径
    QString normalImgPath;

    QString pressImgPath;

    //弹跳特效  向下跳
    void zoom1();

    //弹跳特效  向上跳
    void zoom2();

    //重写鼠标按下事件
    void mousePressEvent(QMouseEvent *e);

    //重写鼠标释放事件
    void mouseReleaseEvent(QMouseEvent *e);

signals:
};

#endif // MYPUSHBUTTON_H
