#include "mainscene.h"
#include "ui_mainscene.h"
#include <QPainter>
#include <mypushbutton.h>
#include <QTimer>
#include <QSoundEffect>

MainScene::MainScene(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainScene)
{
    ui->setupUi(this);

    //配置主场景/////////////////////////////////////////////////

    //设置固定大小
    this->setFixedSize(320,588);
    //设置应用图标
    this->setWindowIcon(QPixmap(":/res/Coin0001.png"));
    //设置窗口标题
    this->setWindowTitle("翻金币游戏");

    //退出按钮实现
    //点击退出，退出程序
    connect(ui->actionquit,&QAction::triggered,[=](){this->close();});

    //准备开始按钮的音效////////////////////////////////////////////////////////////////////
    //QSound *startSound = new QSound(":/res/TapButtonSound.wav",this); 新版已弃用

    // 创建 QSoundEffect 对象
    QSoundEffect *startSound = new QSoundEffect(this);

    // 设置音频文件路径
    startSound->setSource(QUrl::fromLocalFile(":/res/TapButtonSound.wav"));

    // startSound->play();
    // startSound->setLoopCount(10); //-1 无限循环


    //开始按钮实现
    MyPushButton * startBtn = new MyPushButton(":/res/MenuSceneStartButton.png");

    startBtn->setParent(this);

    startBtn->move(this->width()*0.5-startBtn->width()*0.5,this->height()*0.7);

    //实例化选择关卡场景
    chooseScene = new ChooseLevelScene;

    //监听选择场景的返回按钮
    connect(chooseScene,&ChooseLevelScene::chooseSceneBack,[=](){

        this->setGeometry(chooseScene->geometry());

        //将选择关卡场景隐藏掉
        chooseScene->hide();

        //重新显示主场景
        this->show();
    });

    //监听点击事件，执行特效
    connect(startBtn,&MyPushButton::clicked,[=](){

        // 播放音频
        startSound->play();

        //startBtn->zoom1(); //向下跳跃
        startBtn->zoom2(); //向上跳跃

        //进入到选择关卡场景中
        //延时0.5秒后 进入选择场景
        QTimer::singleShot(500, this,[=](){

            //设置choosescence场景的位置
            chooseScene->setGeometry(this->geometry());

            //自身隐藏
            this->hide();

            //显示选择关卡场景
            chooseScene->show();
        });
    });



}

//重写paintEvent事件 画背景图
void MainScene::paintEvent(QPaintEvent *)
{
    //创建画家，指定绘图设备
    QPainter painter(this);
    //创建QPixmap对象
    QPixmap pix;
    //加载图片
    pix.load(":/res/PlayLevelSceneBg.png");
    //绘制背景图
    painter.drawPixmap(0,0,this->width(),this->height(),pix);


    //加载背景上的图标
    pix.load(":/res/Title.png");
    //缩放图片
    pix = pix.scaled(pix.width()*0.8,pix.height()*0.8);
    //绘制标题
    painter.drawPixmap( 10,30,pix.width(),pix.height(),pix);
}


MainScene::~MainScene()
{
    delete ui;
}
