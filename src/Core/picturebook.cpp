#include "picturebook.h"
#include "beginscene.h"
#include <QLabel>
#include "brochureintro.h"

picturebook::picturebook(QWidget *parent)
    : QWidget{parent}
{
    pb_scene = new QGraphicsScene(this);
    setWindowTitle("图鉴");

    bgc = new QGraphicsPixmapItem;
    bgc->setZValue(0);
    bgc->setPixmap(QPixmap(":/assets/images/brochureImg/brochure_bg.jpg").scaled(350, 600));
    pb_scene->addItem(bgc);

    QGraphicsView *view = new QGraphicsView(pb_scene, this);
    view->setFixedSize(350,600);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->show();

    //返回按钮
    MyPushButton *backBtn = new MyPushButton(":/assets/images/buttons/return.png");
    backBtn->setParent(this);
    backBtn->setGeometry(180,10,100,100);

    //点击返回按钮回到开始界面
    connect(backBtn,&QPushButton::clicked,[=](){
        this->hide();
        //打开开始界面
        // qDebug() << "点击了图鉴界面的返回按钮";
        beginScene *begin_scene = new beginScene;
        begin_scene->show();
    });

    //进入哪吒介绍
    MyPushButton *nezhaBtn = new MyPushButton(":/assets/images/brochureButton/Ne Zha.png");
    nezhaBtn->setParent(this);
    nezhaBtn->setGeometry(100,100,100,100);

    connect(nezhaBtn,&QPushButton::clicked,[=](){
        this->hide();
        QString s = ":/assets/images/brochureImg/nezha.png";
        brochureIntro *BI = new brochureIntro;
        BI->getPicture(s);
        BI->show();
    });

    //进入牢大介绍
    MyPushButton *kobeBtn = new MyPushButton(":/assets/images/brochureButton/kobe.png");
    kobeBtn->setParent(this);
    kobeBtn->setGeometry(100,150,100,100);

    connect(kobeBtn,&QPushButton::clicked,[=](){
        this->hide();
        // qDebug() << "进入老大介绍";
        QString s = ":/assets/images/brochureImg/kobe.png";
        brochureIntro *BI = new brochureIntro;
        BI->getPicture(s);
        BI->show();
    });

    //进入企鹅介绍
    MyPushButton *penguinBtn = new MyPushButton(":/assets/images/brochureButton/penguin.png");
    penguinBtn->setParent(this);
    penguinBtn->setGeometry(100,200,100,100);

    connect(penguinBtn,&QPushButton::clicked,[=](){
        this->hide();
        QString s = ":/assets/images/brochureImg/penguin.png";
        brochureIntro *BI = new brochureIntro;
        BI->getPicture(s);
        BI->show();
    });

    //进入wujing介绍
    MyPushButton *wujingBtn = new MyPushButton(":/assets/images/brochureButton/wujing.png");
    wujingBtn->setParent(this);
    wujingBtn->setGeometry(100,250,100,100);

    connect(wujingBtn,&QPushButton::clicked,[=](){
        this->hide();
        QString s = ":/assets/images/brochureImg/wujing.png";
        brochureIntro *BI = new brochureIntro;
        BI->getPicture(s);
        BI->show();
    });

    //进入xiaoyan介绍
    MyPushButton *xiaoyanBtn = new MyPushButton(":/assets/images/brochureButton/xiaoyan.png");
    xiaoyanBtn->setParent(this);
    xiaoyanBtn->setGeometry(100,300,100,100);

    connect(xiaoyanBtn,&QPushButton::clicked,[=](){
        this->hide();
        QString s = ":/assets/images/brochureImg/xiaoyan.png";
        brochureIntro *BI = new brochureIntro;
        BI->getPicture(s);
        BI->show();
    });

    //进入songhao介绍
    MyPushButton *songhaoBtn = new MyPushButton(":/assets/images/brochureButton/songhao.png");
    songhaoBtn->setParent(this);
    songhaoBtn->setGeometry(100,350,100,100);

    connect(songhaoBtn,&QPushButton::clicked,[=](){
        this->hide();
        QString s = ":/assets/images/brochureImg/songhao.png";
        brochureIntro *BI = new brochureIntro;
        BI->getPicture(s);
        BI->show();
    });

    //进入nju介绍
    MyPushButton *njuBtn = new MyPushButton(":/assets/images/brochureButton/nju.png");
    njuBtn->setParent(this);
    njuBtn->setGeometry(100,400,100,100);

    connect(njuBtn,&QPushButton::clicked,[=](){
        this->hide();
        QString s = ":/assets/images/brochureImg/nju.png";
        brochureIntro *BI = new brochureIntro;
        BI->getPicture(s);
        BI->show();
    });
}
