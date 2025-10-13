#ifndef BEGINSCENE_H
#define BEGINSCENE_H

#include <QWidget>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "mypushbutton.h"

class beginScene : public QWidget
{
    Q_OBJECT
public:
    beginScene(QWidget *parent = nullptr);

private:
    QGraphicsPixmapItem *bgc;
    QGraphicsScene *begin_scene;
signals:
};

#endif // BEGINSCENE_H
