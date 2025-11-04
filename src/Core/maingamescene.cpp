#include "maingamescene.h"
#include "mypushbutton.h"
#include "beginscene.h"
#include <QGraphicsView>

#include "Manager/rankingmanager.h"
#include "Entities/gameworlditem.h"


mainGameScene::mainGameScene(QWidget *parent)
    : QWidget{parent}
{
    // 定义游戏窗口大小
    const qreal VIEW_WIDTH = 340.0;
    const qreal VIEW_HEIGHT = 600.0;

    game_scene = new QGraphicsScene(this);
    setWindowTitle("游戏界面");

    game_scene->setSceneRect(0, 0, VIEW_WIDTH, VIEW_HEIGHT);

    // 1. 添加背景
    bgc = new QGraphicsPixmapItem;
    bgc->setZValue(0); // 背景在 Z=0 (最底层)
    bgc->setPixmap(QPixmap(":/assets/images/mainGameScene_bgc.png"));
    game_scene->addItem(bgc);

    // 2. 添加游戏世界
    world = new GameWorldItem(VIEW_WIDTH, VIEW_HEIGHT);
    world->setZValue(1); // 游戏世界在 Z=1 (背景之上)

    game_scene->addItem(world);

    // 3. 设置视图
    QGraphicsView *view = new QGraphicsView(game_scene, this);
    view->setFixedSize(VIEW_WIDTH, VIEW_HEIGHT);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->show();

    // 4.让游戏世界获得键盘焦点
    world->setFocus();

    // 5. 返回按钮     点击时存储分数
    MyPushButton *backBtn = new MyPushButton(":/assets/images/buttons/return.png");
    backBtn->setParent(this);
    backBtn->setGeometry(180,10,100,100);
    connect(backBtn,&QPushButton::clicked,[=](){
        // 存储当前分数到排行榜
        int currentScore = world->getEntityManager()->getTotalScore();
        // qDebug() << "当前分数：" << currentScore;
        bool isAdded = RankingManager::getInstance()->updateRanking(currentScore);
        // qDebug() << "是否进入排行榜：" << isAdded;

        this->hide();
        beginScene *begin_scene = new beginScene;
        begin_scene->show();
    });
}

// 窗口关闭事件：点击右上角X时存储分数
void mainGameScene::closeEvent(QCloseEvent *event) {
    int currentScore = world->getEntityManager()->getTotalScore();
    RankingManager::getInstance()->updateRanking(currentScore);
    event->accept();
}
