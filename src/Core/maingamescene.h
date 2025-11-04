#ifndef MAINGEMESCENE_H
#define MAINGEMESCENE_H

#include <QWidget>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "Entities/gameworlditem.h"

class mainGameScene : public QWidget
{
    Q_OBJECT
public:
    mainGameScene(QWidget *parent = nullptr);
    void closeEvent(QCloseEvent *event);
private:
    QGraphicsPixmapItem *bgc;
    QGraphicsScene *game_scene;
    GameWorldItem *world;

};

#endif // MAINGEMESCENE_H
