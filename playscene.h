#ifndef PLAYSCENE_H
#define PLAYSCENE_H

#include <QMainWindow>
#include "mycoin.h"

class PlayScene : public QMainWindow
{
    Q_OBJECT
public:
    //explicit PlayScene(QWidget *parent = nullptr);

    PlayScene(int index);

    //内部成员属性  记录所选关卡
    int levalIndex;

    //重写paintEvent绘图事件  背景设置
    void paintEvent(QPaintEvent *);

    //二维数组数据  维护每个关卡的具体数据
    int gameArray[4][4];

    //金币按钮数组
    MyCoin * coinBtn[4][4];

    //是否胜利的标志
    bool isWin = false;


signals:
    //自定义信号 告诉选择关卡场景 点击了返回
    void chooseSceneBack();
};

#endif // PLAYSCENE_H
