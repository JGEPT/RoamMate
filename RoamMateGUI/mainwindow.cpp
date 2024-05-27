#include "mainwindow.h"
#include <QScreen>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {

    // Create the pannable widget
    pannableWidget = new PannableWidget(":/res/assets/Background.png", this);

    // Set the pannable widget as the central widget of the main window
    setCentralWidget(pannableWidget);

    // Get the screen resolution
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int screenWidth = screenGeometry.width();
    int screenHeight = screenGeometry.height();

    // Set the window size to the screen size
    resize(screenWidth, screenHeight);
}

MainWindow::~MainWindow() {
}
