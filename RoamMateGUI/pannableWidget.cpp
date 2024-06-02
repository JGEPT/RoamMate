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
#include <QScrollArea>
#include <QVBoxLayout>
#include <QLabel>
#include <QtConcurrent/QtConcurrent>
#include <QPushButton>
#include <QTransform>
#include <QVBoxLayout>
#include <QGraphicsTransform>

PannableWidget::PannableWidget(const QString &imagePath, QWidget *parent)
    : QWidget(parent), background(imagePath), showMenu(false), showSearchMenu(false),
    m_menuPosition(2200, 0), m_menuRotation(-15), ButtonClicked(false), SearchClicked(false) {
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    offset = QPoint(((background.width() - screenGeometry.width()) / 2) * -1, (screenGeometry.height() / 4) * -1);
    setMinimumSize(background.size());
    setAttribute(Qt::WA_TranslucentBackground); // Make widget background transparent

    // Add multiple buttons
    addButton(":res/assets/UPCEBU.PNG", QPoint(847, 420), "UP Cebu", 14);
    addButton(":res/assets/PRODUCTIVITY.PNG", QPoint(762, 102),"Workspace", 17);
    addButton(":res/assets/SCHOOL.PNG", QPoint(482, 462), "Supplies", 17);
    addButton(":res/assets/POLICESTATION.PNG", QPoint(256, 682), "Police", 17);
    addButton(":res/assets/LEISURE.PNG", QPoint(1363, 227),"Leisure",17);
    addButton(":res/assets/CAFE.PNG", QPoint(656, 705), "Cafe",17);
    addButton(":res/assets/BANK.PNG", QPoint(1081, 724), "Bank",17);
    addButton(":res/assets/GYM.PNG", QPoint(1142, 406), "Fitness",17);
    addButton(":res/assets/MALLS.PNG", QPoint(1056, 128), "Mall",17);
    addButton(":res/assets/LAUNDRY.PNG", QPoint(1500, 705),"Laundry", 17);
    addButton(":res/assets/CLINIC.PNG", QPoint(1632, 430), "Clinic",17);
    addButton(":res/assets/TRAVEL.PNG", QPoint(1631, 129), "Travel",17);
    addButton(":res/assets/GROCERIES.PNG", QPoint(411, 194), "Groceries",17);
    addButton(":res/assets/OFFICES.PNG", QPoint(86, 441),"GovOffice", 17);
    addButton(":res/assets/FOOD.PNG", QPoint(127, 129), "Food", 17);

    addLocation("Mall", "Ayala Center Cebu");
    addLocation("Mall", "Ayala Central Block");
    addLocation("Mall", "SM City");
    addLocation("Mall", "SM seaside");
    addLocation("Food", "McDonald's Cebu Escario");
    addLocation("Food", "Kaipaz Sizzlers");
    addLocation("Food", "Silogan ni Gian 2 Gorordo Ave");
    addLocation("Food", "Jollibee Gorordo Ave");
    addLocation("Groceries", "7-Eleven Azalea Place");
    addLocation("Groceries", "Marty Mart Gonsales Compo");
    addLocation("Groceries", "Metro Supermarket Cebu Business Park");
    addLocation("Supplies", "National Book Store Mango Plaza");
    addLocation("Supplies", "Nics School supplies and printing shop");
    addLocation("Supplies", "Joneal School Supply");
    addLocation("Supplies", "Expressions Robinsons Fuente");
    addLocation("Laundry", "WANASH & FOLD HUB");
    addLocation("Laundry", "Coleen Laundry House");
    addLocation("Laundry", "Tumbled Laundromat");
    addLocation("Clinic", "Cebu Doctors’ Hospital");
    addLocation("Clinic", "Chong Hua Hospital");
    addLocation("Clinic", "ThreeSixty Pharmacy 2 Gorordo Ave");
    addLocation("Clinic", "Rose Pharmacy Capitol Site Escario");
    addLocation("Workspace", "Cebu City Public Library");
    addLocation("Workspace", "Sync Work + Study Hub");
    addLocation("Workspace", "Witska Cafe and Study Hub");
    addLocation("Workspace", "ROC Study Cafe");
    addLocation("Cafe", "Cafe Talk Library");
    addLocation("Cafe", "Nicole's Cafe");
    addLocation("Cafe", "Te'Amo Cafe");
    addLocation("Cafe", "Stamp Coffee house");
    addLocation("Bank", "BPI Cebu Lahug Branch ATM");
    addLocation("Bank", "Metrobank Cebu - Gorordo");
    addLocation("Bank", "BPI ATM USPF");
    addLocation("Bank", "BDO Cebu - Osmeña Branch");
    addLocation("Travel", "Mactan-Cebu International Airport");
    addLocation("Travel", "Cebu IT Park Transport Terminal");
    addLocation("Travel", "South Bus Terminal");
    addLocation("Travel", "Cebu Pier 1");
    addLocation("Police", "Camp General Arcadio Maxilom");
    addLocation("GovOffice", "Land Transportation Office - DLRO");
    addLocation("GovOffice", "Department of Foreign Affairs – Cebu");
    addLocation("GovOffice", "Department of Science and Technology Central Visayas");
    addLocation("GovOffice", "Commission on Higher Education Region 7");
    addLocation("Fitness", "Anytime Fitness Escario");
    addLocation("Fitness", "GymTeam Fitness");
    addLocation("Fitness", "Sacred heart center sports club");
    addLocation("Fitness", "Abellana Sport Complex");

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
    ExitButton->setIconSize(ExitPixmap.size() / 1.5);
    ExitButton->setFixedSize(ExitPixmap.size() / 1.5);
    ExitButton->setFlat(true); // Make button background transparent
    ExitButton->setVisible(true);
    connect(ExitButton, &QPushButton::clicked, this, &PannableWidget::onExitButtonClicked);

    // Initialize the search button
    SearchButton = new QPushButton(this);
    QPixmap SearchPixmap(":res/assets/Search.png");
    SearchButton->setIcon(QIcon(SearchPixmap));
    SearchButton->setIconSize(SearchPixmap.size() * 1.25);
    SearchButton->setFixedSize(SearchPixmap.size() * 1.25);
    SearchButton->setFlat(true); // Make button background transparent
    SearchButton->setVisible(true);
    connect(SearchButton, &QPushButton::clicked, this, &PannableWidget::onSearchButtonClicked);

    // Initialize the search button
    HistoryButton = new QPushButton(this);
    QPixmap HistoryPixmap(":res/assets/History.png");
    HistoryButton->setIcon(QIcon(HistoryPixmap));
    HistoryButton->setIconSize(HistoryPixmap.size() * 1.25);
    HistoryButton->setFixedSize(HistoryPixmap.size() * 1.25);
    HistoryButton->setFlat(true); // Make button background transparent
    HistoryButton->setVisible(true);
    connect(HistoryButton, &QPushButton::clicked, this, &PannableWidget::onHistoryButtonClicked);

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

    // Initialize the scroll area for routes
    scrollArea = new QScrollArea(this);
    scrollArea->setVisible(false); // Initially hidden
    scrollArea->setFixedSize(550, 450); // Set the size of the scroll area
    scrollArea->setStyleSheet("QScrollArea {"
                              "border: 1px solid white;"  // Change the border color
                              "background-color: black;" // Change the background color
                              "}");

    // Create a content widget and layout for the scroll area
    scrollAreaContent = new QWidget();
    scrollAreaLayout = new QVBoxLayout(scrollAreaContent);
    scrollAreaContent->setLayout(scrollAreaLayout);
    scrollArea->setWidget(scrollAreaContent);

    // Initialize the processing label
    processingLabel = new QLabel("Route request is being processed...", this);
    processingLabel->setVisible(false); // Initially hidden
    processingLabel->setStyleSheet("QLabel {"
                                   "color: white;" // Change the text color
                                   "font-size: 16px;" // Change the font size
                                   "}");
    processingLabel->setFixedSize(550, 40); // Set the size of the label
    processingLabel->setAlignment(Qt::AlignCenter); // Center the text
}

void PannableWidget::addButton(const QString &iconPath, const QPoint &position, const QString &category, int scale) {
    QPushButton *button = new QPushButton(this);
    QPixmap buttonImage(iconPath);
    button->setIcon(QIcon(buttonImage));
    button->setIconSize(buttonImage.size() / scale);
    button->setFixedSize(buttonImage.size() / scale);
    button->setFlat(true); // Make button background transparent

    // Connect the button's clicked signal to a lambda function that calls onButtonClicked with the category
    connect(button, &QPushButton::clicked, this, [this, category]() { onButtonClicked(category); });

    ButtonInfo buttonInfo = { button, position, category };
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
    HistoryButton->move(-15, 140);

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
        HistoryButton->move(m_menuPosition.x() + searchMenuRect.right() - 13, 140);
        HistoryButton->setVisible(true);

        // Ensure the text input bars are positioned and visible
        originLineEdit->move(m_menuPosition.x() + 50, m_menuPosition.y() + 25);
        originLineEdit->setVisible(true);
        destinationLineEdit->move(m_menuPosition.x() + 50, m_menuPosition.y() + 90);
        destinationLineEdit->setVisible(true);
        searchConfirmButton->move(m_menuPosition.x() + 535, m_menuPosition.y() + 150);
        searchConfirmButton->setVisible(true);

        // Adjust scroll area and processing label positions
        scrollArea->move(m_menuPosition.x() + 50, m_menuPosition.y() + 200);
        processingLabel->move(m_menuPosition.x() - 100, m_menuPosition.y() + 145);
        scrollAreaContent->setLayout(scrollAreaLayout);
        scrollAreaContent->adjustSize();
        scrollArea->setWidget(scrollAreaContent);
        scrollArea->setVisible(true);
    } else {
        // Hide the text input bars, search confirmation button, scroll area, and processing label
        originLineEdit->setVisible(false);
        destinationLineEdit->setVisible(false);
        searchConfirmButton->setVisible(false);
        scrollArea->setVisible(false);
        processingLabel->setVisible(false);
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

void PannableWidget::onButtonClicked(const QString &category) {
    qDebug() << "Button clicked!";
    showMenu = !showMenu;
    ButtonClicked = true;
    SearchClicked = false;
    showSearchMenu = false;

    currentCategory = category;

    setButtonsVisible(!showMenu);
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
    update();
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

    // Hide origin and destination buttons
    for (auto button : menuButtons) {
        button->setVisible(false);
    }
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

void PannableWidget::onHistoryButtonClicked() {
    qDebug() << "History button clicked";
}

void PannableWidget::drawMenu(QPainter &painter) {
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    menuRect = QRect(screenGeometry.width() - 505, 0, screenGeometry.width() / 2.25, screenGeometry.height() * 1.2);

    painter.save();
    QPoint center = menuRect.center();
    painter.translate(center);
    painter.rotate(m_menuRotation);
    painter.translate(-center);

    QPainterPath path;
    path.addRect(menuRect);

    QPen pen(Qt::white, 5);
    painter.setPen(pen);
    painter.setBrush(QBrush(Qt::black));
    painter.drawPath(path);

    painter.setFont(QFont("Persona 5 Menu Font Prototype", 64));
    painter.setPen(Qt::white);
    painter.drawText(menuRect.adjusted(10, 10, 0, 0), Qt::AlignTop, currentCategory);

    painter.setFont(QFont("p5hatty", 32));
    int yOffset = 100;
    int xOffset = -50;
    for (const auto &location : locations) {
        if (location.category == currentCategory) {
            // Draw location name
            painter.drawText(menuRect.adjusted(30, yOffset, 0, 0), Qt::AlignLeft, location.name);

            // Create buttons for setting origin and destination
            QPushButton *originButton = new QPushButton(this);
            QPixmap OriginImage(":/res/assets/Origin.png");
            originButton->setIcon(QIcon(OriginImage));
            originButton->setIconSize(OriginImage.size());
            originButton->setFixedSize(OriginImage.size());
            originButton->setFlat(true); // Make button background transparent
            originButton->setVisible(showMenu); // Set initial visibility based on menu state
            originButton->move(menuRect.left() + xOffset, yOffset + 120);
            connect(originButton, &QPushButton::clicked, this, [this, location]() { setOrigin(location.name); });

            QPushButton *destinationButton = new QPushButton(this);
            QPixmap DestinationImage(":/res/assets/Destination.png");
            destinationButton->setIcon(QIcon(DestinationImage));
            destinationButton->setIconSize(DestinationImage.size());
            destinationButton->setFixedSize(DestinationImage.size());
            destinationButton->setFlat(true); // Make button background transparent
            destinationButton->setVisible(showMenu); // Set initial visibility based on menu state
            destinationButton->move(menuRect.left() + xOffset + 150, yOffset + 100);
            connect(destinationButton, &QPushButton::clicked, this, [this, location]() { setDestination(location.name); });

            xOffset += 40;
            yOffset += 150;

            // Add buttons to a list to manage visibility
            menuButtons.append(originButton);
            menuButtons.append(destinationButton);
        }
    }

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

    // Show the processing label
    processingLabel->setVisible(true);

    // Simulate a route request asynchronously
    auto future = QtConcurrent::run([this, origin, destination]() {
        RouteRequest RoamMate;
        requestResultText = QString::fromStdString(RoamMate.makeRequest(origin.toStdString(), destination.toStdString()));

        // Clear previous results (run on the GUI thread)
        QMetaObject::invokeMethod(this, [this]() {
                // Clear the layout
                QLayoutItem *child;
                while ((child = scrollAreaLayout->takeAt(0)) != nullptr) {
                    if (child->widget()) {
                        child->widget()->setParent(nullptr);
                        delete child->widget();
                    }
                    delete child;
                }

                // Simulate route parsing (assuming requestResultText contains multiple routes)
                QStringList routes = requestResultText.split("\n\n"); // Example parsing
                for (const QString &route : routes) {
                    if (!route.isEmpty()) {
                        QPushButton *RouteButton = new QPushButton(scrollAreaContent);
                        RouteButton->setStyleSheet("QPushButton {"
                                                   "border: 0px solid white;"  // Change the border color
                                                   "background-color: black;" // Change the background color
                                                   "color: white;"             // Change the text color
                                                   "font-size: 12px;"          // Change the font size
                                                   "padding: 1px;"             // Add padding
                                                   "border-radius: 10px;"      // Make the corners rounded
                                                   "text-align: left;"         // Align text to the left
                                                   "}");
                        RouteButton->setFixedSize(520, 150); // Adjust the size of the route buttons

                        // Add QLabel for the route text
                        routeLabel = new QLabel(route, RouteButton);
                        routeLabel->setWordWrap(true); // Enable word wrap for the label
                        routeLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
                        routeLabel->setStyleSheet("QLabel { color: white; border: 0px solid white;}"); // Set text color
                        routeLabel->setGeometry(5, 0, 530, 130); // Adjust the position and size of the label

                        scrollAreaLayout->addWidget(RouteButton);
                    }
                }

                // Ensure the layout and scroll area are updated
                scrollAreaContent->setLayout(scrollAreaLayout);
                scrollAreaContent->adjustSize();
                scrollArea->setWidget(scrollAreaContent);
                scrollArea->setVisible(true);

                // Hide the processing label after completion
                processingLabel->setVisible(false);

                // Force update and repaint to reflect changes
                scrollArea->update();
                scrollAreaContent->update();
                update();
                repaint();
            }, Qt::QueuedConnection);
    });
}

void PannableWidget::setOrigin(const QString &origin) {
    originLineEdit->setText(origin);
}

void PannableWidget::setDestination(const QString &destination) {
    destinationLineEdit->setText(destination);
}
