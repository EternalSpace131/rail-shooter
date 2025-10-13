#include "maingamescene.h"
#include "mypushbutton.h"
#include "beginscene.h"

mainGameScene::mainGameScene(QWidget *parent)
    : QWidget{parent}
{
    game_scene = new QGraphicsScene(this);
    setWindowTitle("游戏界面");

    bgc = new QGraphicsPixmapItem;
    bgc->setZValue(0);
    bgc->setPixmap(QPixmap(":/assets/images/mainGameScene_bgc.png"));
    game_scene->addItem(bgc);

    QGraphicsView *view = new QGraphicsView(game_scene, this);
    view->setFixedSize(350,600);
    // view->setSceneRect(0, 0, 400, 620);
    view->show();

    //返回按钮
    MyPushButton *backBtn = new MyPushButton(":/assets/images/button.png");
    backBtn->setParent(this);
    backBtn->setGeometry(240,10,100,100);

    //点击返回按钮回到开始界面
    connect(backBtn,&QPushButton::clicked,[=](){
        this->hide();
        //打开开始界面
        beginScene *begin_scene = new beginScene;
        begin_scene->show();
    });
}

