#include "chooselevelscene.h"
#include <QMenuBar>
#include <QPainter>
#include "mypushbutton.h"
#include <QTimer>
#include <QLabel>
#include "playscene.h"
#include <QSoundEffect>

ChooseLevelScene::ChooseLevelScene(QWidget *parent)
    : QMainWindow{parent}
{
    //配置选择关卡场景/////////////////////////////////////////////////////////////////////////////

    //设置窗口固定大小
    this->setFixedSize(320,588);

    //设置图标
    this->setWindowIcon(QPixmap(":/res/Coin0001.png"));

    //设置标题
    this->setWindowTitle("请选择关卡");

    //创建菜单栏
    QMenuBar * bar = menuBar();

    setMenuBar(bar);

    bar->setAutoFillBackground(true);

    //创建开始菜单
    QMenu * startMenu = bar->addMenu("开始");

    //创建退出菜单项
    QAction * quitAction = startMenu->addAction("退出");

    //点击退出 退出游戏
    connect(quitAction,&QAction::triggered,[=](){this->close();});


    //选择关卡按钮音效///////////////////////////////////////////////////////////////////
    //QSound *chooseSound = new QSound(":/res/TapButtonSound.wav",this);

    // 创建 QSoundEffect 对象
    QSoundEffect *chooseSound = new QSoundEffect(this);

    // 设置音频文件路径
    chooseSound->setSource(QUrl::fromLocalFile(":/res/TapButtonSound.wav"));

    //返回按钮音效
    //QSound *backSound = new QSound(":/res/BackButtonSound.wav",this);

    // 创建 QSoundEffect 对象
    QSoundEffect *backSound = new QSoundEffect(this);

    // 设置音频文件路径
    backSound->setSource(QUrl::fromLocalFile(":/res/BackButtonSound.wav"));



    //返回按钮////////////////////////////////////////////////////////////////////////////////////////
    MyPushButton * blackBtn = new MyPushButton(":/res/BackButton.png",":/res/BackButtonSelected.png");

    blackBtn->setParent(this);

    blackBtn->move(this->width()-blackBtn->width(),this->height()-blackBtn->height());

    //返回按钮功能实现
    connect(blackBtn,&MyPushButton::clicked,[=](){
        //播放返回按钮音效
        backSound->play();

        QTimer::singleShot(500, this,[=](){
            this->hide();
            //告诉主场景我返回了，主场景监听ChooseLevelScene的返回按钮
            //触发自定义信号，关闭自身，该信号写到 signals下做声明
            emit this->chooseSceneBack();
        });
    });

    //创建选择关卡按钮////////////////////////////////////////////////////////////////////////
    for(int i = 0 ; i < 20;i++)
    {
        MyPushButton * menuBtn = new MyPushButton(":/res/LevelIcon.png");

        menuBtn->setParent(this);

        menuBtn->move(25 + (i%4)*70 , 130+ (i/4)*70);

        //监听选择关卡按钮的信号槽
        connect(menuBtn,&MyPushButton::clicked,[=](){
            //播放选择关卡音效
            chooseSound->play();
            //QString str = QString("您选择的是第 %1 关").arg(i + 1);
            //qDebug() << "select: " << i;
            //进入到游戏场景
            if(playScene == NULL)  //游戏场景最好不用复用，直接移除掉创建新的场景
            {
                //将选择关卡场景隐藏掉
                this->hide();
                //创建游戏场景  将选择的关卡号 传入给PlayerScene
                playScene = new PlayScene(i+1);

                //设置游戏场景的初始位置
                playScene->setGeometry(this->geometry());

                //显示游戏场景
                playScene->show();

                //PlayScene的返回按钮监听  删除该scene并且将指针指向空
                connect(playScene,&PlayScene::chooseSceneBack,[=](){
                    this->setGeometry(playScene->geometry());

                    this->show();

                    delete playScene;

                    playScene = NULL;
                });
            }
        });


        //按钮上显示的文字
        QLabel * label = new QLabel;

        label->setParent(this);

        label->setFixedSize(menuBtn->width(),menuBtn->height());

        label->setText(QString::number(i+1));

        //设置居中  水平居中 垂直居中
        label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

        label->move(25 + (i%4)*70 , 130+ (i/4)*70);

        //设置让鼠标进行穿透  51号
        label->setAttribute(Qt::WA_TransparentForMouseEvents,true);
    }

}

//重写绘图事件
void ChooseLevelScene::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    QPixmap pix;

    pix.load(":/res/OtherSceneBg.png");

    painter.drawPixmap(0,0,this->width(),this->height(),pix);

    //加载标题
    pix.load(":/res/Title.png");

    painter.drawPixmap( (this->width() - pix.width())*0.5,30,pix.width(),pix.height(),pix);
}
