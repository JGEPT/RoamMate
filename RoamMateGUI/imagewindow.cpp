#include "imagewindow.h"
#include <QGuiApplication>
#include <QScreen>

ImageWindow::ImageWindow(const QString &imageFilePath, QWidget *parent)
    : QMainWindow(parent), imageLabel(new QLabel)
{
    setWindowTitle("Legend");
    setMinimumSize(342, 608);

    // Create layout
    mainLayout = new QVBoxLayout;
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // Load the image from the file path
    QPixmap image(imageFilePath);
    if (!image.isNull()) {
        // Set aspect ratio policy
        imageLabel->setPixmap(image);
        imageLabel->setScaledContents(true);
        imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
        mainLayout->addWidget(imageLabel);
    } else {
        imageLabel->setText("Failed to load image.");
        mainLayout->addWidget(imageLabel);
    }

    // Set central widget
    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    // Set the window's initial position
    QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();
    int x = (screenGeometry.width() - width());
    int y = 0;
    move(x, y);
}
