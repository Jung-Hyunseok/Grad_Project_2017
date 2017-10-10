#include <QApplication>
#include "mainwindow.h"

int main(int agrc, char *agrv[]) {
    QApplication app(agrc, agrv);

    MainWindow *mainWin = new MainWindow;
    mainwin->show();

    return app.exec();
}