#include <QApplication>
#include <QWidget>
int main(int argc, char *argv[]) {

    QApplication app(argc, argv);


    QWidget window;


    window.setWindowTitle("Qt窗口创建成功!");
    window.resize(400, 300);


    window.show();

    return QApplication::exec();
}