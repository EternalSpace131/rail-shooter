#ifndef MAINGEMESCENE_H
#define MAINGEMESCENE_H

#include <QWidget>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsView>

class mainGameScene : public QWidget
{
    Q_OBJECT
public:
    mainGameScene(QWidget *parent = nullptr);
private:
    QGraphicsPixmapItem *bgc;
    QGraphicsScene *game_scene;
};

#endif // MAINGEMESCENE_H
