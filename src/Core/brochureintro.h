#ifndef BROCHURETNTRO_H
#define BROCHUREINTRO_H

#include <QWidget>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QString>


class brochureIntro : public QWidget
{
    Q_OBJECT
public:
    brochureIntro(QWidget *parent = nullptr);
    void getPicture(QString _s);

private:
    QGraphicsPixmapItem *bgc;
    QGraphicsPixmapItem *picIntro;
    QGraphicsScene *pbIntro_scene;
    QString s;
signals:
};

#endif // BRICHUREINTRO_H
