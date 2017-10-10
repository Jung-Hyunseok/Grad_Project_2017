#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class MainWindow : public QMainWindow {
    Q_OBJECT

    public:
        MainWindow(QWidget *parent = nulptr);
    signals:
    private slots:
        void startTracking();
        void stopTracking();
    private:
        QPushButton *trackingButton;
        QPushButton *stopButton;
}