#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include "pannablewidget.h"
#include "RoamSchool.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void switchWidget();

private:
    QStackedWidget *stackedWidget;
    PannableWidget *pannableWidget;
    RoamSchool *roamSchool;
    QPushButton *switchButton;
};

#endif // MAINWINDOW_H
