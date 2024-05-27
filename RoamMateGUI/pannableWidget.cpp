#include "pannablewidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QScreen>
#include <QGuiApplication>
#include <QDebug>
#include <QMessageBox>

PannableWidget::PannableWidget(const QString &imagePath, QWidget *parent)
    : QWidget(parent), background(imagePath) {
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    offset = QPoint((screenGeometry.width() / 4) * -1, (screenGeometry.height() / 4) * -1);
    setMinimumSize(background.size());
    setAttribute(Qt::WA_TranslucentBackground); // Make widget background transparent

    // Add multiple buttons
    addButton(":res/assets/UpIcon.PNG", QPoint(960, 540));
    addButton(":res/assets/UpIcon.PNG", QPoint(582, 421));
    addButton(":res/assets/UpIcon.PNG", QPoint(439, 659));
    addButton(":res/assets/UpIcon.PNG", QPoint(1442, 521));
    addButton(":res/assets/UpIcon.PNG", QPoint(723, 782));
    addButton(":res/assets/UpIcon.PNG", QPoint(1174, 721));
    addButton(":res/assets/UpIcon.PNG", QPoint(1332, 321));
}

void PannableWidget::addButton(const QString &iconPath, const QPoint &position) {
    QPushButton *button = new QPushButton(this);
    QPixmap buttonImage(iconPath);
    button->setIcon(QIcon(buttonImage));
    button->setIconSize(buttonImage.size() / 1.5);
    button->setFixedSize(buttonImage.size() / 1.5);
    button->setFlat(true); // Make button background transparent

    // Connect the button's clicked signal to the onButtonClicked slot
    connect(button, &QPushButton::clicked, this, &PannableWidget::onButtonClicked);

    ButtonInfo buttonInfo = { button, position };
    buttons.append(buttonInfo);
}

void PannableWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event); // Suppress unused parameter warning
    QPainter painter(this);
    painter.setCompositionMode(QPainter::CompositionMode_Clear);
    painter.fillRect(rect(), Qt::transparent); // Fill the widget with transparent background
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.drawPixmap(offset, background);

    // Adjust all buttons' positions based on the background's offset
    for (const ButtonInfo &buttonInfo : buttons) {
        buttonInfo.button->move(offset + buttonInfo.position);
    }
}

void PannableWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        lastMousePos = event->pos();
    }
}

void PannableWidget::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        QPoint delta = event->pos() - lastMousePos;
        QPoint newOffset = offset + delta;

        // Constrain newOffset within the valid range
        QScreen *screen = QGuiApplication::primaryScreen();
        QRect screenGeometry = screen->geometry();
        int maxX = std::min(0, screenGeometry.width() - background.width());
        int maxY = std::min(0, screenGeometry.height() - background.height());

        newOffset.setX(std::min(0, std::max(maxX, newOffset.x())));
        newOffset.setY(std::min(0, std::max(maxY, newOffset.y())));

        offset = newOffset;
        lastMousePos = event->pos();
        update();
    }
}

void PannableWidget::onButtonClicked() {
    qDebug() << "Button clicked!";
    QMessageBox::information(this, "Button Clicked", "I have been clicked!");
}
