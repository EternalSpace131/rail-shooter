#include "picturebook.h"
#include "beginscene.h"
#include "brochureintro.h"
#include <QLabel>

brochureIntro::brochureIntro(QWidget *parent)
    : QWidget{parent}
{
    pbIntro_scene = new QGraphicsScene(this);
    setWindowTitle("图鉴");

    bgc = new QGraphicsPixmapItem;
    bgc->setZValue(0);
    bgc->setPixmap(QPixmap(":/assets/images/brochureImg/brochure_bg.jpg").scaled(350, 600));
    pbIntro_scene->addItem(bgc);

    QGraphicsView *view = new QGraphicsView(pbIntro_scene, this);
    view->setFixedSize(350,600);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->show();

    //返回按钮
    MyPushButton *backBtn = new MyPushButton(":/assets/images/buttons/return.png");
    backBtn->setParent(this);
    backBtn->setGeometry(180,10,100,100);

    connect(backBtn,&QPushButton::clicked,[=](){
        this->hide();
        picturebook *_pb_scene = new picturebook;
        _pb_scene->show();
    });

}

void brochureIntro::getPicture(QString _s){
    s = _s;

    QPixmap pix(s);
    if (!pix.isNull()) {
        QPixmap scaledPix = pix.scaled(qMin(pix.width() * 1.5, 800.0),
                                       qMin(pix.height() * 1.5, 600.0),
                                       Qt::KeepAspectRatio,
                                       Qt::SmoothTransformation);
        picIntro = new QGraphicsPixmapItem(scaledPix);
        picIntro->setZValue(1);
        picIntro->setPos(-125, 0);
        pbIntro_scene->addItem(picIntro);
    } else {
        qDebug() << "图片加载失败";
    }
}
