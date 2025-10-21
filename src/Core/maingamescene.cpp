#include "maingamescene.h"
#include "mypushbutton.h"
#include "beginscene.h"
#include <QGraphicsView>

// *** 包含你的 "桥梁" 类 ***
#include "Entities/gameworlditem.h"

// (你不再需要在这里包含 Player/Enemy/EntityManager 等)

mainGameScene::mainGameScene(QWidget *parent)
    : QWidget{parent}
{
    // 定义你的游戏窗口大小
    const qreal VIEW_WIDTH = 350.0;
    const qreal VIEW_HEIGHT = 600.0;

    game_scene = new QGraphicsScene(this);
    setWindowTitle("游戏界面");

    // *** 重要：设置场景的边界，必须和视图/地图匹配 ***
    game_scene->setSceneRect(0, 0, VIEW_WIDTH, VIEW_HEIGHT);

    // 1. 添加背景
    bgc = new QGraphicsPixmapItem;
    bgc->setZValue(0); // 背景在 Z=0 (最底层)
    bgc->setPixmap(QPixmap(":/assets/images/mainGameScene_bgc.png"));
    game_scene->addItem(bgc);

    // 2. 添加你的游戏世界 "桥梁"
    //    构造函数会自动创建 Map, EntityManager, Player 等
    GameWorldItem *world = new GameWorldItem(VIEW_WIDTH, VIEW_HEIGHT);
    world->setZValue(1); // 游戏世界在 Z=1 (背景之上)

    // ***********************************************
    // *** 这就是“打印到主窗口上”的答案 ***
    // ***********************************************
    game_scene->addItem(world);

    // 3. 设置视图
    QGraphicsView *view = new QGraphicsView(game_scene, this);
    view->setFixedSize(VIEW_WIDTH, VIEW_HEIGHT);
    // (你之前的 view->setSceneRect(...) 可以删掉了，我们已经在 game_scene 上设置了)
    view->show();

    // 4. *** 关键：让游戏世界获得键盘焦点 ***
    world->setFocus();

    // 5. 返回按钮 (你的代码保持不变)
    MyPushButton *backBtn = new MyPushButton(":/assets/images/button.png");
    backBtn->setParent(this);
    backBtn->setGeometry(240,10,100,100);
    connect(backBtn,&QPushButton::clicked,[=](){
        this->hide();
        beginScene *begin_scene = new beginScene;
        begin_scene->show();
    });
}
