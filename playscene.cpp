#include "playscene.h"
#include <QMenuBar>
#include <QPainter>
#include "mypushbutton.h"
#include "chooselevelscene.h"
#include <QTimer>
#include <QLabel>
#include "mycoin.h"
#include "dataconfig.h"
#include <QDebug>
#include <QPropertyAnimation>
#include <QSoundEffect>
// PlayScene::PlayScene(QWidget *parent)
//     : QMainWindow{parent}
// {}


PlayScene::PlayScene(int index)
{
    //场景基本设置/////////////////////////////////////////////////////////////////
    //qDebug() << "当前关卡为"<< index;
    this->levalIndex = index;

    //设置窗口固定大小
    this->setFixedSize(320,588);

    //设置图标
    this->setWindowIcon(QPixmap(":/res/Coin0001.png"));

    //设置标题
    this->setWindowTitle("翻金币");

    //创建菜单栏
    QMenuBar * bar = this->menuBar();

    this->setMenuBar(bar);

    bar->setAutoFillBackground(true);


    //创建开始菜单///////////////////////////////////////////////////////////////
    QMenu * startMenu = bar->addMenu("开始");

    //创建按钮菜单项
    QAction * quitAction = startMenu->addAction("退出");

    //点击退出 退出游戏
    connect(quitAction,&QAction::triggered,[=](){this->close();});

    //添加音效//////////////////////////////////////////////////////////////////

    //翻金币音效
    //QSound *flipSound = new QSound(":/res/ConFlipSound.wav",this);

    // 创建 QSoundEffect 对象
    QSoundEffect *flipSound = new QSoundEffect(this);

    // 设置音频文件路径
    flipSound->setSource(QUrl::fromLocalFile(":/res/ConFlipSound.wav"));

    //胜利按钮音效
    //QSound *winSound = new QSound(":/res/LevelWinSound.wav",this);

    // 创建 QSoundEffect 对象
    QSoundEffect *winSound = new QSoundEffect(this);

    // 设置音频文件路径
    winSound->setSource(QUrl::fromLocalFile(":/res/LevelWinSound.wav"));

    //返回按钮音效
    //QSound *backSound = new QSound(":/res/BackButtonSound.wav",this);

    // 创建 QSoundEffect 对象
    QSoundEffect *backSound = new QSoundEffect(this);

    // 设置音频文件路径
    backSound->setSource(QUrl::fromLocalFile(":/res/BackButtonSound.wav"));


    //返回按钮//////////////////////////////////////////////////////////////////
    MyPushButton * closeBtn = new MyPushButton(":/res/BackButton.png",":/res/BackButtonSelected.png");

    closeBtn->setParent(this);

    closeBtn->move(this->width() - closeBtn->width(),this->height() - closeBtn->height());

    //返回按钮功能实现
    connect(closeBtn,&MyPushButton::clicked,[=](){
        //返回按钮音效
        backSound->play();

        //qDebug()<<"翻金币场景中点击了返回按钮";
        QTimer::singleShot(500, this,[=](){
            this->hide();
            //触发自定义信号，关闭自身，该信号写到 signals下做声明
            emit this->chooseSceneBack();
        });
    });


    //显示当前关卡数/////////////////////////////////////////////////////////////
    QLabel * label = new QLabel;

    label->setParent(this);

    QFont font;

    font.setFamily("华文新魏");

    font.setPointSize(20);

    //将字体设置到标签控件中
    label->setFont(font);

    QString str = QString("Level: %1").arg(this->levalIndex);

    label->setText(str);

    label->setGeometry(QRect(30, this->height() - 50,120, 50)); //设置大小和位置


    //初始化每个关卡的二维数组  用于判断是显示金币还是银币//////////////////////////////////////
    dataConfig config;

    for(int i = 0 ; i < 4;i++)
    {
        for(int j = 0 ; j < 4; j++)
        {
            gameArray[i][j] = config.mData[this->levalIndex][i][j];
        }
    }

    //胜利图片显示////////////////////////////////////////////////////////////////////////
    QLabel* winLabel = new QLabel;

    QPixmap tmpPix;

    tmpPix.load(":/res/LevelCompletedDialogBg.png");

    winLabel->setGeometry(0,0,tmpPix.width(),tmpPix.height());

    winLabel->setPixmap(tmpPix);

    winLabel->setParent(this);

    winLabel->move( (this->width() - tmpPix.width())*0.5 , -tmpPix.height());


    //创建金币的背景图片/////////////////////////////////////////////////////////////////
    for(int i = 0 ; i < 4;i++)
    {
        for(int j = 0 ; j < 4; j++)
        {
            //绘制背景图片
            QLabel* label = new QLabel;

            QPixmap pix(":/res/BoardNode.png");

            label->setGeometry(0,0,pix.width(),pix.height());

            label->setPixmap(pix);

            label->setParent(this);

            label->move(57 + i*50,200+j*50);



            //创建金币/////////////////////////////////////////////////////////////////////
            //金币对象
            QString img;

            if(gameArray[i][j] == 1)//显示金币
            {
                img = ":/res/Coin0001.png";
            }
            else//显示银币
            {
                img = ":/res/Coin0008.png";
            }

            MyCoin * coin = new MyCoin(img);

            coin->setParent(this);

            coin->move(59 + i*50,204+j*50);

            //给金币属性赋值
            //记录x坐标
            coin->posX = i;

            //记录y坐标
            coin->posY = j;

            coin->isWin = false;

            //记录正反标志 1正面  0反面
            coin->flag = this->gameArray[i][j];

            //将金币放入到金币的二维数组里 以便后期的维护
            coinBtn[i][j] = coin;

            //点击金币  进行翻转
            connect(coin,&MyCoin::clicked,[=](){

                //播放翻金币音效
                flipSound->play();

                if (this->isWin == false)
                {
                    //qDebug() << "点击的位置： x = " <<  coin->posX << " y = " << coin->posY ;
                    coin->changeFlag();

                    gameArray[i][j] = gameArray[i][j] == 0 ? 1 : 0; //数组内部记录的标志同步修改

                    //翻转周围硬币
                    QTimer::singleShot(300, this,[=](){
                        //右侧金币翻转的条件
                        if(coin->posX + 1 <= 3)
                        {
                            coinBtn[coin->posX+1][coin->posY]->changeFlag();
                            gameArray[coin->posX+1][coin->posY] = gameArray[coin->posX+1][coin->posY]== 0 ? 1 : 0;
                        }
                        //左侧金币翻转的条件
                        if(coin->posX - 1 >= 0)
                        {
                            coinBtn[coin->posX-1][coin->posY]->changeFlag();
                            gameArray[coin->posX-1][coin->posY] = gameArray[coin->posX-1][coin->posY]== 0 ? 1 : 0;
                        }
                        //上侧金币翻转的条件
                        if(coin->posY + 1 <= 3)
                        {
                            coinBtn[coin->posX][coin->posY+1]->changeFlag();
                            gameArray[coin->posX][coin->posY+1] = gameArray[coin->posX+1][coin->posY]== 0 ? 1 : 0;
                        }
                        //下侧金币翻转的条件
                        if(coin->posY - 1 >= 0)
                        {
                            coinBtn[coin->posX][coin->posY-1]->changeFlag();
                            gameArray[coin->posX][coin->posY-1] = gameArray[coin->posX+1][coin->posY]== 0 ? 1 : 0;
                        }

                        //判断是否胜利
                        this->isWin = true;

                        for(int i = 0; i < 4;i++)
                        {
                            for(int j = 0; j < 4; j++)
                            {
                                //qDebug() << coinBtn[i][j]->flag ;
                                //只要有一个是反面 就算是失败
                                if(!coinBtn[i][j]->flag)
                                {
                                    this->isWin = false;
                                    break;
                                }
                            }
                        }

                        if(this->isWin == true)
                        {
                            //添加胜利音效
                            winSound->play();

                            qDebug()<< "胜利";

                            //胜利之后  将所有按钮的胜利标志改为true
                            //禁用所有按钮点击事件
                            for(int i = 0; i < 4; i++)
                            {
                                for(int j = 0; j < 4; j++)
                                {
                                    coinBtn[i][j]->isWin = true;
                                }
                            }

                            //将胜利的图片移动下来
                            if(this->isWin)
                            {
                                qDebug() << "胜利";

                                //将胜利图片移动下来
                                QPropertyAnimation * animation1 =  new QPropertyAnimation(winLabel,"geometry");

                                //设置时间间隔
                                animation1->setDuration(1000);

                                //设置开始位置
                                animation1->setStartValue(QRect(winLabel->x(),winLabel->y(),winLabel->width(),winLabel->height()));

                                //设置结束位置
                                animation1->setEndValue(QRect(winLabel->x(),winLabel->y()+166,winLabel->width(),winLabel->height()));

                                //设置缓和曲线
                                animation1->setEasingCurve(QEasingCurve::OutBounce);

                                //执行动画
                                animation1->start();
                            }

                        }


                    });


                    // //判断是否胜利
                    // this->isWin = true;

                    // for(int i = 0; i < 4;i++)
                    // {
                    //     for(int j = 0; j < 4; j++)
                    //     {
                    //         //qDebug() << coinBtn[i][j]->flag ;
                    //         //只要有一个是反面 就算是失败
                    //         if(!coinBtn[i][j]->flag)
                    //         {
                    //             this->isWin = false;
                    //             break;
                    //         }
                    //     }
                    // }

                    // if(this->isWin == true)
                    // {
                    //     qDebug()<< "胜利";

                    //     //胜利之后  将所有按钮的胜利标志改为true
                    //     //禁用所有按钮点击事件
                    //     for(int i = 0; i < 4; i++)
                    //     {
                    //         for(int j = 0; j < 4; j++)
                    //         {
                    //             coinBtn[i][j]->isWin = true;
                    //         }
                    //     }
                    // }
                }
            });
        }
    }

}


void PlayScene::paintEvent(QPaintEvent *)
{
    //加载背景
    QPainter painter(this);

    QPixmap pix;

    pix.load(":/res/PlayLevelSceneBg.png");

    painter.drawPixmap(0,0,this->width(),this->height(),pix);

    //加载标题
    pix.load(":/res/Title.png");

    pix = pix.scaled(pix.width()*0.5,pix.height()*0.5);

    painter.drawPixmap( 10,30,pix.width(),pix.height(),pix);
}

