#include "mainwindow.h"
#include <QScreen>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {

    // Create the stacked widget
    stackedWidget = new QStackedWidget(this);

    // Create the widgets
    pannableWidget = new PannableWidget(":/res/assets/Background.png", this);
    roamSchool = new RoamSchool(this);

    // Add widgets to the stacked widget
    stackedWidget->addWidget(pannableWidget);
    stackedWidget->addWidget(roamSchool);

    // Set the stacked widget as the central widget
    setCentralWidget(stackedWidget);

    switchButton = new QPushButton(this);
    QPixmap SwitchPixmap(":res/assets/Switch.png");
    switchButton->setIcon(QIcon(SwitchPixmap));
    switchButton->setIconSize(SwitchPixmap.size() / 1.5);
    switchButton->setFixedSize(SwitchPixmap.size() / 1.5);
    switchButton->setFlat(true); // Make button background transparent
    switchButton->setVisible(true);
    connect(switchButton, &QPushButton::clicked, this, &MainWindow::switchWidget);

    // Get the screen resolution
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int screenWidth = screenGeometry.width();
    int screenHeight = screenGeometry.height();

    switchButton->move(screenGeometry.width() - 150, 10);

    // Set the window size to the screen size
    resize(screenWidth, screenHeight);
}

MainWindow::~MainWindow() {
}

void MainWindow::switchWidget() {
    int currentIndex = stackedWidget->currentIndex();
    int nextIndex = (currentIndex + 1) % stackedWidget->count();
    stackedWidget->setCurrentIndex(nextIndex);
}
