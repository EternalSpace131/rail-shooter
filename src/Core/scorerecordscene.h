#ifndef SCORERECORDSCENE_H
#define SCORERECORDSCENE_H

#include <QWidget>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QVector>
#include "mypushbutton.h"
#include "Manager/rankingmanager.h"

class scoreRecordScene : public QWidget
{
    Q_OBJECT
public:
    scoreRecordScene(QWidget *parent = nullptr);

private:
    // 绘制排行榜
    void drawRankingList();
    // 初始化排行榜文本样式
    QFont getTitleFont();
    QFont getRecordFont();

    QGraphicsPixmapItem *bgc;
    QGraphicsScene *score_scene;

    // 排行榜数据
    QVector<RankingRecord> m_rankingList;
signals:
};

#endif // SCORERECORDSCENE_H
