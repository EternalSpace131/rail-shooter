#ifndef PICTUREBOOK_H
#define PICTUREBOOK_H

#include <QWidget>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsView>


class picturebook : public QWidget
{
    Q_OBJECT
public:
    picturebook(QWidget *parent = nullptr);

private:
    QGraphicsPixmapItem *bgc;
    QGraphicsScene *pb_scene;
signals:
};

#endif // PICTUREBOOK_H
