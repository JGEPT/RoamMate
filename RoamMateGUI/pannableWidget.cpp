#include "pannablewidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QScreen>
#include <QGuiApplication>
#include <QDebug>
#include <QPainterPath>
#include <QPropertyAnimation>
#include <QIcon>
#include <QLabel>

PannableWidget::PannableWidget(const QString &imagePath, QWidget *parent)
    : QWidget(parent), background(imagePath), showMenu(false), showSearchMenu(false),
    m_menuPosition(2200, 0), m_menuRotation(-15), ButtonClicked(false), SearchClicked(false) {
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    offset = QPoint(((background.width() - screenGeometry.width()) / 2) * -1, (screenGeometry.height() / 4) * -1);
    setMinimumSize(background.size());
    setAttribute(Qt::WA_TranslucentBackground); // Make widget background transparent

    // Add multiple buttons
    addButton(":res/assets/UPCEBU.PNG", QPoint(847, 420), 14);
    addButton(":res/assets/PRODUCTIVITY.PNG", QPoint(762, 102), 17);
    addButton(":res/assets/SCHOOL.PNG", QPoint(482, 462), 17);
    addButton(":res/assets/POLICESTATION.PNG", QPoint(256, 682), 17);
    addButton(":res/assets/LEISURE.PNG", QPoint(1363, 227), 17);
    addButton(":res/assets/CAFE.PNG", QPoint(656, 705), 17);
    addButton(":res/assets/BANK.PNG", QPoint(1081, 724), 17);
    addButton(":res/assets/GYM.PNG", QPoint(1142, 406), 17);
    addButton(":res/assets/MALLS.PNG", QPoint(1056, 128), 17);
    addButton(":res/assets/LAUNDRY.PNG", QPoint(1500, 705), 17);
    addButton(":res/assets/CLINIC.PNG", QPoint(1632, 430), 17);
    addButton(":res/assets/TRAVEL.PNG", QPoint(1631, 129), 17);
    addButton(":res/assets/GROCERIES.PNG", QPoint(411, 194), 17);
    addButton(":res/assets/OFFICES.PNG", QPoint(86, 441), 17);
    addButton(":res/assets/FOOD.PNG", QPoint(127, 129), 17);
    // Initialize the animations
    positionAnimation = new QPropertyAnimation(this, "menuPosition");
    positionAnimation->setDuration(150); // Duration in milliseconds
    positionAnimation->setStartValue(QPointF(750, 0));
    positionAnimation->setEndValue(QPointF(-100, 125));

    rotationAnimation = new QPropertyAnimation(this, "menuRotation");
    rotationAnimation->setDuration(150); // Duration in milliseconds
    rotationAnimation->setStartValue(45);
    rotationAnimation->setEndValue(-5);

    // Initialize the animations for the search menu
    searchPositionAnimation = new QPropertyAnimation(this, "menuPosition");
    searchPositionAnimation->setDuration(150); // Duration in milliseconds
    searchPositionAnimation->setStartValue(QPointF(-750, 100));
    searchPositionAnimation->setEndValue(QPointF(-20, 100));

    searchRotationAnimation = new QPropertyAnimation(this, "menuRotation");
    searchRotationAnimation->setDuration(150);
    searchRotationAnimation->setStartValue(0);
    searchRotationAnimation->setEndValue(0);

    // Initialize the exit button
    ExitButton = new QPushButton(this);
    QPixmap ExitPixmap(":res/assets/Exit.png");
    ExitButton->setIcon(QIcon(ExitPixmap));
    ExitButton->setIconSize(ExitPixmap.size()/1.5);
    ExitButton->setFixedSize(ExitPixmap.size()/1.5);
    ExitButton->setFlat(true); // Make button background transparent
    ExitButton->setVisible(true);
    connect(ExitButton, &QPushButton::clicked, this, &PannableWidget::onExitButtonClicked);

    // Initialize the search button
    SearchButton = new QPushButton(this);
    QPixmap SearchPixmap(":res/assets/Search.png");
    SearchButton->setIcon(QIcon(SearchPixmap));
    SearchButton->setIconSize(SearchPixmap.size()*1.25);
    SearchButton->setFixedSize(SearchPixmap.size()*1.25);
    SearchButton->setFlat(true); // Make button background transparent
    SearchButton->setVisible(true);
    connect(SearchButton, &QPushButton::clicked, this, &PannableWidget::onSearchButtonClicked);

    // Initialize the close button
    closeButton = new QPushButton(this);
    QPixmap closePixmap(":res/assets/Close.png");
    closeButton->setIcon(QIcon(closePixmap));
    closeButton->setIconSize(closePixmap.size());
    closeButton->setFixedSize(closePixmap.size());
    closeButton->setFlat(true); // Make button background transparent
    closeButton->setVisible(false);
    connect(closeButton, &QPushButton::clicked, this, &PannableWidget::onCloseButtonClicked);

    // Add a search confirmation button
    searchConfirmButton = new QPushButton("Submit", this);
    searchConfirmButton->setVisible(false); // Initially hidden
    searchConfirmButton->setStyleSheet("QPushButton {"
                                  "border: 3px solid white;"  // Change the border color
                                  "background-color: black;" // Change the background color
                                  "color: white;"             // Change the text color
                                  "font-size: 16px;"          // Change the font size
                                  "padding: 5px;"             // Add padding
                                  "border-radius: 10px;"      // Make the corners rounded
                                  "}");
    connect(searchConfirmButton, &QPushButton::clicked, this, &PannableWidget::onSearchConfirmButtonClicked);

    // Initialize the origin line edit
    originLineEdit = new QLineEdit(this);
    originLineEdit->setPlaceholderText("Enter your origin");
    originLineEdit->setVisible(false); // Initially hidden
    originLineEdit->setStyleSheet("QLineEdit {"
                                  "border: 3px solid white;"  // Change the border color
                                  "background-color: black;" // Change the background color
                                  "color: white;"             // Change the text color
                                  "font-size: 16px;"          // Change the font size
                                  "padding: 5px;"             // Add padding
                                  "border-radius: 10px;"      // Make the corners rounded
                                  "}");
    originLineEdit->setFixedSize(550, 40); // Set the size of the origin line edit

    // Initialize the destination line edit
    destinationLineEdit = new QLineEdit(this);
    destinationLineEdit->setPlaceholderText("Enter your destination");
    destinationLineEdit->setVisible(false); // Initially hidden
    destinationLineEdit->setStyleSheet("QLineEdit {"
                                       "border: 3px solid white;"  // Change the border color
                                       "background-color: black;" // Change the background color
                                       "color: white;"             // Change the text color
                                       "font-size: 16px;"          // Change the font size
                                       "padding: 5px;"             // Add padding
                                       "border-radius: 10px;"      // Make the corners rounded
                                       "}");
    destinationLineEdit->setFixedSize(550, 40); // Set the size of the destination line edit
}

void PannableWidget::addButton(const QString &iconPath, const QPoint &position, int scale) {
    QPushButton *button = new QPushButton(this);
    QPixmap buttonImage(iconPath);
    button->setIcon(QIcon(buttonImage));
    button->setIconSize(buttonImage.size() / scale);
    button->setFixedSize(buttonImage.size() / scale);
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

    // Adjust exit button position
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    ExitButton->move(screenGeometry.width() - 75, 10);

    // Adjust search button position
    SearchButton->move(-15, 40);

    if (ButtonClicked) {
        // Draw the new menu
        painter.save();
        painter.translate(m_menuPosition.x(), m_menuPosition.y());
        painter.rotate(m_menuRotation);
        drawMenu(painter);
        painter.restore();
    }
    if (showMenu) {
        // Adjust close button position
        closeButton->move(m_menuPosition.x() + menuRect.right() - closeButton->width() - 36, m_menuPosition.y() - 125);
        closeButton->setVisible(true);
    }
    if (SearchClicked) {
        // Draw the search menu
        painter.save();
        painter.translate(m_menuPosition.x(), m_menuPosition.y());
        painter.rotate(m_menuRotation);
        drawSearchMenu(painter);
        painter.restore();
    }
    if (showSearchMenu) {
        // Adjust search button position
        SearchButton->move(m_menuPosition.x() + searchMenuRect.right() - 13, 40);
        SearchButton->setVisible(true);

        // Ensure the text input bars are positioned and visible
        originLineEdit->move(m_menuPosition.x() + 50, m_menuPosition.y() + 25);
        originLineEdit->setVisible(true);
        destinationLineEdit->move(m_menuPosition.x() + 50, m_menuPosition.y() + 90);
        destinationLineEdit->setVisible(true);
        searchConfirmButton->move(m_menuPosition.x() + 535, m_menuPosition.y() + 150);
        searchConfirmButton->setVisible(true);

        // Draw the request result text
        painter.save();
        painter.setFont(QFont("p5Hatty", 12));
        painter.setPen(Qt::white);
        QRect textRect(m_menuPosition.x() + 50, m_menuPosition.y() + 200, searchMenuRect.width() - 40, searchMenuRect.height() - 300);
        painter.drawText(textRect, Qt::AlignLeft | Qt::TextWordWrap, requestResultText);
        painter.restore();
    } else {
        // Hide the text input bars and search confirmation button
        originLineEdit->setVisible(false);
        destinationLineEdit->setVisible(false);
        searchConfirmButton->setVisible(false);
    }
}

void PannableWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        if ((!showMenu && !showSearchMenu) || (showMenu && !menuRect.contains(event->pos())) || (showSearchMenu && !searchMenuRect.contains(event->pos()))){
            lastMousePos = event->pos();
        }
    }
}

void PannableWidget::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        if ((!showMenu && !showSearchMenu) || (showMenu && !menuRect.contains(event->pos())) || (showSearchMenu && !searchMenuRect.contains(event->pos()))){
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
    ButtonClicked = true;
    SearchClicked = false;
    showSearchMenu = false;

    setButtonsVisible(!showMenu); // Hide buttons when showing the menu
    if (showMenu) {
        positionAnimation->setDirection(QPropertyAnimation::Forward);
        rotationAnimation->setDirection(QPropertyAnimation::Forward);
    } else {
        positionAnimation->setDirection(QPropertyAnimation::Backward);
        rotationAnimation->setDirection(QPropertyAnimation::Backward);
    }
    positionAnimation->start();
    rotationAnimation->start();
    ExitButton->setVisible(false);
    update(); // Trigger a repaint to show the new menu
}

void PannableWidget::onExitButtonClicked() {
    QCoreApplication::quit();
}

void PannableWidget::onCloseButtonClicked() {
    showMenu = false;
    showSearchMenu = false;
    positionAnimation->setDirection(QPropertyAnimation::Backward);
    rotationAnimation->setDirection(QPropertyAnimation::Backward);
    positionAnimation->start();
    rotationAnimation->start();
    update();
    setButtonsVisible(true);
    ExitButton->setVisible(true);
    closeButton->setVisible(false);
}

void PannableWidget::onSearchButtonClicked() {
    if (!showMenu) {
        qDebug() << "Search button clicked!";
        showSearchMenu = !showSearchMenu;
        SearchClicked = true;
        ButtonClicked = false;
        showMenu = false;
        setButtonsVisible(!showSearchMenu); // Hide buttons when showing the search menu
        if (showSearchMenu) {
            searchPositionAnimation->setDirection(QPropertyAnimation::Forward);
            searchRotationAnimation->setDirection(QPropertyAnimation::Forward);
        } else {
            searchPositionAnimation->setDirection(QPropertyAnimation::Backward);
            searchRotationAnimation->setDirection(QPropertyAnimation::Backward);

            // Hide the text input bars
            originLineEdit->setVisible(false);
            destinationLineEdit->setVisible(false);
        }
        searchPositionAnimation->start();
        searchRotationAnimation->start();
        closeButton->setVisible(false);
        searchConfirmButton->move(m_menuPosition.x() + 50, m_menuPosition.y() + 350);
        searchConfirmButton->setVisible(showSearchMenu);
        update(); // Trigger a repaint to show the new search menu
    }
}


void PannableWidget::drawMenu(QPainter &painter) {
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    menuRect = QRect(screenGeometry.width() - 505, 0, screenGeometry.width() / 2.25, screenGeometry.height() * 1.2); // Example menu dimensions

    // Rotate the painter -15 degrees around the center of the menu rectangle
    painter.save();
    QPoint center = menuRect.center();
    painter.translate(center);
    painter.rotate(m_menuRotation);
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
    painter.drawText(menuRect.adjusted(0, 0, 0, 0), Qt::AlignTop, "FOOD");

    painter.restore();
}

void PannableWidget::drawSearchMenu(QPainter &painter) {
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    searchMenuRect = QRect(25, -100, screenGeometry.width() / 2.25, screenGeometry.height()); // Example search menu dimensions


    // Draw the search menu with a border
    painter.save();
    QPainterPath path;
    path.addRect(searchMenuRect);

    QPen pen(Qt::white, 5); // Adjust the border color and width here
    painter.setPen(pen);
    painter.setBrush(QBrush(Qt::black));
    painter.drawPath(path);

    // Draw the text
    painter.setFont(QFont("Persona 5 Menu Font Prototype", 64));
    painter.setPen(Qt::white);
    painter.drawText(searchMenuRect.adjusted(15, 15, 0, 0), Qt::AlignTop, "SEARCH");

    painter.restore();
}

QPointF PannableWidget::menuPosition() const {
    return m_menuPosition;
}

void PannableWidget::setMenuPosition(const QPointF &position) {
    m_menuPosition = position;
    update(); // Trigger a repaint to apply the new position
}

qreal PannableWidget::menuRotation() const {
    return m_menuRotation;
}

void PannableWidget::setMenuRotation(qreal rotation) {
    m_menuRotation = rotation;
    update(); // Trigger a repaint to apply the new rotation
}


void PannableWidget::onSearchConfirmButtonClicked() {
    QString origin = originLineEdit->text();
    QString destination = destinationLineEdit->text();
    qDebug() << "Search confirmed with origin:" << origin << "and destination:" << destination;

    RouteRequest RoamMate;
    requestResultText = QString::fromStdString(RoamMate.makeRequest(origin.toStdString(), destination.toStdString()));

    update();
}

