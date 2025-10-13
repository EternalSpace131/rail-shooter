#include <QApplication>
#include <QWidget>
#include "Core/beginscene.h"
int main(int argc, char *argv[]) {

    QApplication app(argc, argv);


    // QWidget window;


    // window.setWindowTitle("Qt窗口创建成功!");
    // window.resize(400, 300);


    // window.show();

    // return QApplication::exec();

    beginScene w;

    w.setFixedSize(350,600);
    w.setWindowTitle("开始界面");
    w.show();
    return app.exec();
}
