#include "beginscene.h"
#include <QDebug>
#include "maingamescene.h"
#include "scorerecordscene.h"
#include "picturebook.h"

beginScene::beginScene(QWidget *parent)
    : QWidget{parent}
{
    begin_scene = new QGraphicsScene(this);
    setWindowTitle("开始界面");

    bgc = new QGraphicsPixmapItem;
    bgc->setZValue(0);
    QPixmap originalPixmap(":/assets/images/StartWindow.png");
    QPixmap scaledPixmap = originalPixmap.scaled(350, 595, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    bgc->setPixmap(scaledPixmap);
    begin_scene->addItem(bgc);

    QGraphicsView *view = new QGraphicsView(begin_scene, this);
    view->setFixedSize(350,600);
    view->show();

    //开始按钮
    MyPushButton *startBtn = new MyPushButton(":/assets/images/buttons/start.png");
    startBtn->setParent(this);
    startBtn->setGeometry(105,280,100,100);

    //得分记录
    MyPushButton *score_record_btn = new MyPushButton(":/assets/images/buttons/ranking.png");
    score_record_btn->setParent(this);
    score_record_btn->setGeometry(105,350,100,100);

    //图鉴
    MyPushButton *picture_book_btn = new MyPushButton(":/assets/images/buttons/brochure.png");
    picture_book_btn->setParent(this);
    picture_book_btn->setGeometry(105,420,100,100);

    //退出按钮
    MyPushButton *exitBtn = new MyPushButton(":/assets/images/buttons/exit.png");
    exitBtn->setParent(this);
    exitBtn->setGeometry(105,490,100,100);

    //点击开始按钮进入主游戏界面
    connect(startBtn,&QPushButton::clicked,[=](){
        this->hide();
        //打开主游戏界面
        mainGameScene *game_scene = new mainGameScene;
        game_scene->show();
    });


    //点击得分记录按钮进入得分记录页面
    connect(score_record_btn,&QPushButton::clicked,[=](){
        this->hide();
        //打开得分记录界面
        scoreRecordScene *scoreScene = new scoreRecordScene;
        // qDebug() << "打开了排行榜界面";
        scoreScene->show();

    });

    //点击图鉴按钮进入图鉴页面
    connect(picture_book_btn,&QPushButton::clicked,[=](){
        this->hide();
        //打开图鉴界面
        picturebook *pb_scene = new picturebook;
        pb_scene->show();

    });

    //退出游戏
    connect(exitBtn,&QPushButton::clicked,[=](){
        this->close();
    });

}
