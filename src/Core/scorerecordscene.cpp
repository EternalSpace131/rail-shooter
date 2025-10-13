#include "scorerecordscene.h"
#include "beginscene.h"

scoreRecordScene::scoreRecordScene(QWidget *parent)
    : QWidget{parent}
{
    score_scene = new QGraphicsScene(this);
    setWindowTitle("排行榜");

    bgc = new QGraphicsPixmapItem;
    bgc->setZValue(0);
    bgc->setPixmap(QPixmap(":/assets/images/beginScene_bgc.png"));
    score_scene->addItem(bgc);

    QGraphicsView *view = new QGraphicsView(score_scene, this);
    view->setFixedSize(350,600);
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
