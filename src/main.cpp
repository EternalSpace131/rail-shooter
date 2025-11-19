#include <QApplication>
#include <QWidget>
#include "Core/beginscene.h"
#include <QSoundEffect>
#include <Manager/musicmanager.h>
#include <QResource>
#include <QFile>
int main(int argc, char *argv[]) {

    QApplication app(argc, argv);

    MusicManager::getInstance()->playMusic(":/assets/sounds/Music/bgm.wav");

    beginScene w;

    w.setFixedSize(350,600);
    w.setWindowTitle("开始界面");
    w.show();
    return app.exec();
}
