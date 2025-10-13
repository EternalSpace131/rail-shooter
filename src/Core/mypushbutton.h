#ifndef MYPUSHBUTTON_H
#define MYPUSHBUTTON_H

#include <QPushButton>
#include <QMouseEvent>
#include <QPainter>

class MyPushButton : public QPushButton
{
public:
    MyPushButton(QString img);
    QString imgPath;
};

#endif // MYPUSHBUTTON_H
