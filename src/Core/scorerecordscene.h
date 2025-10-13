#ifndef SCORERECORDSCENE_H
#define SCORERECORDSCENE_H

#include <QWidget>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "mypushbutton.h"

class scoreRecordScene : public QWidget
{
    Q_OBJECT
public:
    scoreRecordScene(QWidget *parent = nullptr);

private:
    QGraphicsPixmapItem *bgc;
    QGraphicsScene *score_scene;
signals:
};

#endif // SCORERECORDSCENE_H
