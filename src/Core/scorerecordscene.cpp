#include "scoreRecordScene.h"
#include "beginscene.h"
#include <QGraphicsTextItem>
#include <QColor>
#include <QFont>

scoreRecordScene::scoreRecordScene(QWidget *parent)
    : QWidget{parent}
{
    score_scene = new QGraphicsScene(this);
    setWindowTitle("排行榜");
    score_scene->setSceneRect(0, 0, 350, 600); // 与视图大小一致

    // 背景设置
    bgc = new QGraphicsPixmapItem;
    bgc->setZValue(0);
    bgc->setPixmap(QPixmap(":/assets/images/mainGameScene_bgc.png").scaled(350, 600));
    score_scene->addItem(bgc);

    // 加载排行榜数据
    m_rankingList = RankingManager::getInstance()->getRankingList();

    // 绘制排行榜（核心）
    drawRankingList();

    // 视图设置
    QGraphicsView *view = new QGraphicsView(score_scene, this);
    view->setFixedSize(350, 600);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->show();

    // 返回按钮
    MyPushButton *backBtn = new MyPushButton(":/assets/images/buttons/return.png");
    backBtn->setParent(this);
    backBtn->setGeometry(180, 10, 100, 100);
    connect(backBtn,&QPushButton::clicked,[=](){
        this->hide();
        beginScene *begin_scene = new beginScene;
        begin_scene->show();
    });
}

// 绘制排行榜主逻辑
void scoreRecordScene::drawRankingList()
{
    // 1. 绘制标题
    QGraphicsTextItem *titleItem = new QGraphicsTextItem("分数排行榜 TOP 10");
    titleItem->setFont(getTitleFont());
    titleItem->setDefaultTextColor(QColor(255, 215, 0)); // 金色标题
    // 标题居中（x = 场景宽度/2 - 标题宽度/2，y = 50）
    QRectF titleRect = titleItem->boundingRect();
    titleItem->setPos((350 - titleRect.width())/2, 50);
    titleItem->setZValue(1); // 标题在背景之上
    score_scene->addItem(titleItem);

    // 2. 绘制榜单内容
    if (m_rankingList.isEmpty()) {
        // 无记录时显示提示
        QGraphicsTextItem *emptyItem = new QGraphicsTextItem("暂无游戏记录");
        emptyItem->setFont(getRecordFont());
        emptyItem->setDefaultTextColor(QColor(255, 255, 255));
        QRectF emptyRect = emptyItem->boundingRect();
        emptyItem->setPos((350 - emptyRect.width())/2, 250);
        emptyItem->setZValue(1);
        score_scene->addItem(emptyItem);
        return;
    }

    // 有记录时绘制每条数据（从y=120开始，每条间隔40像素）
    for (int i = 0; i < m_rankingList.size(); ++i) {
        const auto& record = m_rankingList[i];
        // 拼接文本：排名 + 分数 + 时间
        QString text = QString("%1. 分数：%2  |  时间：%3")
                           .arg(i+1, 2, 10, QChar('0')) // 排名补0（01, 02...）
                           .arg(record.score)
                           .arg(record.time.mid(5)); // 时间简化（只显示月-日 时:分）

        QGraphicsTextItem *recordItem = new QGraphicsTextItem(text);
        recordItem->setFont(getRecordFont());
        // 前三名高亮（金色），其余白色
        if (i < 3) {
            recordItem->setDefaultTextColor(QColor(255, 215, 0));
        } else {
            recordItem->setDefaultTextColor(QColor(255, 255, 255));
        }
        // 文本左对齐，y坐标递增
        recordItem->setPos(30, 120 + i*40);
        recordItem->setZValue(1);
        score_scene->addItem(recordItem);
    }
}

// 标题字体样式
QFont scoreRecordScene::getTitleFont()
{
    QFont font;
    font.setFamily("Microsoft YaHei");
    font.setPointSize(18);
    font.setBold(true);
    return font;
}

// 记录字体样式
QFont scoreRecordScene::getRecordFont()
{
    QFont font;
    font.setFamily("Microsoft YaHei");
    font.setPointSize(12);
    return font;
}
