#include "RoamSchool.h"

#include <QApplication>
#include <QDebug>
#include <QMainWindow>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.showMaximized();
    return a.exec();
}
