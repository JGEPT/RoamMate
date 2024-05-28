#include "pannablewidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QScreen>
#include <QGuiApplication>
#include <QDebug>
#include <QPainterPath>
#include <QPropertyAnimation>

PannableWidget::PannableWidget(const QString &imagePath, QWidget *parent)
    : QWidget(parent), background(imagePath), showMenu(false), m_menuOpacity(0.0) {
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    offset = QPoint(((background.width() - screenGeometry.width()) / 2) * -1, (screenGeometry.height() / 4) * -1);
    setMinimumSize(background.size());
    setAttribute(Qt::WA_TranslucentBackground); // Make widget background transparent

    // Add multiple buttons
    addButton(":res/assets/UpIcon.PNG", QPoint(893, 473));
    addButton(":res/assets/UpIcon.PNG", QPoint(527, 269));
    addButton(":res/assets/UpIcon.PNG", QPoint(320, 507));
    addButton(":res/assets/UpIcon.PNG", QPoint(1442, 474));
    addButton(":res/assets/UpIcon.PNG", QPoint(717, 667));
    addButton(":res/assets/UpIcon.PNG", QPoint(1089, 776));
    addButton(":res/assets/UpIcon.PNG", QPoint(1060, 233));

    // Initialize the animation
    animation = new QPropertyAnimation(this, "menuOpacity");
    animation->setDuration(500); // Duration in milliseconds
    animation->setStartValue(0.0);
    animation->setEndValue(1.0);
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

void PannableWidget::setButtonsVisible(bool visible) {
    for (const ButtonInfo &buttonInfo : buttons) {
        buttonInfo.button->setVisible(visible);
    }
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

    // Draw the new menu if the flag is set
    if (showMenu) {
        painter.setOpacity(m_menuOpacity);
        drawMenu(painter); // Example usage with position (50, 50) and red color
    }
}

void PannableWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        if (!showMenu || !menuRect.contains(event->pos())) {
            lastMousePos = event->pos();
        }
    }
}

void PannableWidget::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        if (!showMenu || !menuRect.contains(event->pos())) {
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
}

void PannableWidget::onButtonClicked() {
    qDebug() << "Button clicked!";
    showMenu = !showMenu;
    setButtonsVisible(!showMenu); // Hide buttons when showing the menu
    if (showMenu) {
        animation->setDirection(QPropertyAnimation::Forward);
    } else {
        animation->setDirection(QPropertyAnimation::Backward);
    }
    animation->start();
    update(); // Trigger a repaint to show the new menu
}

void PannableWidget::drawMenu(QPainter &painter) {
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    menuRect = QRect(screenGeometry.width() - 505, 0, screenGeometry.width() / 2.25, screenGeometry.height() * 1.2); // Example menu dimensions

    // Rotate the painter -15 degrees around the center of the menu rectangle
    painter.save();
    QPoint center = menuRect.center();
    painter.translate(center);
    painter.rotate(-15);
    painter.translate(-center);

    // Draw the menu with a border
    QPainterPath path;
    path.addRect(menuRect);

    QPen pen(Qt::white, 5); // Adjust the border color and width here
    painter.setPen(pen);
    painter.setBrush(QBrush(Qt::black));
    painter.drawPath(path);

    // Draw the text
    painter.setFont(QFont("Persona 5 Menu Font Prototype", 64));
    painter.setPen(Qt::white);
    painter.drawText(menuRect.adjusted(20,20,0,0), "FOOD");

    painter.restore();
}

qreal PannableWidget::menuOpacity() const {
    return m_menuOpacity;
}

void PannableWidget::setMenuOpacity(qreal opacity) {
    m_menuOpacity = opacity;
    update(); // Trigger a repaint to apply the new opacity
}
