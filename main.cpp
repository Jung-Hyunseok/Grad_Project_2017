#include <QApplication>
#include <QWidget>

int main(int agrc, char *agrv[]) {
    QApplication app(agrc, agrv);
    QWidget window;

    window.resize(800, 600);
    window.setWindowTitle("Program");
    window.show();

    return app.exec();
}