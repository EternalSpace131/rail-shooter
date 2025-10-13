#include "mypushbutton.h"

//构造函数
MyPushButton::MyPushButton(QString img){
    this->imgPath = img;

    QPixmap pix;
    pix.load(img);

    //设置图片固定大小
    this->setFixedSize(150,30);
    //设置不规则图片样式
    this->setStyleSheet("QPushButton{border:0px;}");
    //设置图标
    this->setIcon(pix);
    //设置图标的大小
    this->setIconSize(QSize(pix.width(),pix.height()));
};
