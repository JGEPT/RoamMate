#include "RoamSchool.h"
#include "imagewindow.h"
#include "dijkstra_algorithm.h"
#include <QFile>
#include <cstring>
#include <QFileDialog>
#include <QPixmap>
#include <QLabel>
#include <QComboBox>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QToolButton>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QIcon>
#include <QDebug>
#include <QtSvg>
#include <QSvgRenderer>

RoamSchool::RoamSchool(QWidget *parent)
    : QWidget(parent)
{

    QPixmap backgroundImage(":/res/files/school-map.jpg");

    // Scale the background image
    backgroundImage = backgroundImage.scaled(backgroundImage.size() * 0.5);

    // Create a label to display the background image
    QLabel *backgroundLabel = new QLabel;
    backgroundLabel->setPixmap(backgroundImage);

    // Create a scroll area to display the background image
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(backgroundLabel);

    // Get the screen resolution
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int screenWidth = screenGeometry.width();
    int screenHeight = screenGeometry.height();

    // Set the window size to the screen size
    scrollArea->setFixedSize(screenWidth, screenHeight);

    // Create a widget to overlay on top of the background image
    overlayWidget1 = new QWidget(this);
    overlayWidget1->setGeometry(10, 10, 400, 350);

    // Define the custom stylesheet
    QString stylesheet = R"(
        QWidget#overlayWidget1 {
            background-color: rgba(102, 25, 19, 200);
            border: 1px solid #000000;
            border-radius: 10px;
        }

        QLabel {
            font-size: 14px;
            font-weight: bold;
            color: #ffffff;
        }

        QLabel#outputLineEdit{
            background-color: rgb(184, 82, 73);
            border: none;
            padding: 5px;
            color: #ffffff;
            font-size: 14px;
            font-weight: 600;
        }

        QComboBox {
            background-color: rgb(184, 82, 73);
            border: none;
            padding: 5px;
            color: #ffffff;
            font-size: 14px;
            font-weight: 600;
        }
        QComboBox QAbstractItemView {
            background-color: #ffffff;
            border: none;
            selection-background-color: #f0f0f0;
            color: black;
            font-size: 10px;
            font-weight: normal;
        }
        QLineEdit, QPushButton {
            background-color: rgb(184, 82, 73);
            border: none;
            padding: 5px;
            font-size: 14px;
            color: #ffffff;
            font-weight: 600;
        }
        QToolButton {
            height: 500;
            width: auto;
            border: none;
        }
        QToolButton#toggleButton2 {
            background-color: white;
        }
    )";

    // Apply the stylesheet to the overlay widgets and their children
    overlayWidget1->setObjectName("overlayWidget1");
    overlayWidget1->setStyleSheet(stylesheet);

    // Create labels and comboboxes for the first overlay widget
    QLabel *sourceLabel = new QLabel("Source:", overlayWidget1);
    QLabel *destinationLabel = new QLabel("Destination:", overlayWidget1);

    // Set alignment for the labels
    sourceLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    destinationLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    sourceBox = new QComboBox(overlayWidget1);
    destinationBox = new QComboBox(overlayWidget1);

    filepath = ":/res/files/mapping.csv";
    readToComboBoxFromFile(filepath);

    sourceBox->setCurrentIndex(0);
    destinationBox->setCurrentIndex(36);

    connect(sourceBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &RoamSchool::on_sourceBox_currentIndexChanged);
    connect(destinationBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &RoamSchool::on_destinationBox_currentIndexChanged);

    // Create the additional widgets for the first overlay widget
    outputTextEdit = new QLabel(overlayWidget1);
    outputLineEdit = new QLineEdit(overlayWidget1);
    outputTextEdit->setStyleSheet(stylesheet);
    outputLineEdit->setReadOnly(true);

    QPushButton *submitButton = new QPushButton("Submit", overlayWidget1);
    // Connect the clicked signal of the QPushButton to the submitButtonClicked slot function
    connect(submitButton, &QPushButton::clicked, this, &RoamSchool::submitButtonClicked);

    // Create labels for the first overlay widget
    QLabel *outputTextLabel = new QLabel("Shortest Path:", overlayWidget1);
    QLabel *outputLineEditLabel = new QLabel("Distance Covered:", overlayWidget1);

    // Create a layout for the first overlay widget
    QVBoxLayout *overlayLayout1 = new QVBoxLayout(overlayWidget1);
    overlayLayout1->addWidget(sourceLabel);
    overlayLayout1->addWidget(sourceBox);
    overlayLayout1->addWidget(destinationLabel);
    overlayLayout1->addWidget(destinationBox);
    overlayLayout1->addWidget(outputTextLabel);
    overlayLayout1->addWidget(outputTextEdit);
    overlayLayout1->addWidget(outputLineEditLabel);
    overlayLayout1->addWidget(outputLineEdit);
    overlayLayout1->addWidget(submitButton);

    // Create the toggle button
    QToolButton *toggleButton = new QToolButton(this);
    toggleButton->setIcon(QIcon(":/res/files/magnifying.png")); // Add your icon here
    toggleButton->setIconSize(QSize(100, 100));
    toggleButton->setGeometry(10, 9, 35, 35);  // Adjust the size and position as needed
    toggleButton->setStyleSheet(stylesheet);

    // Connect the toggle button to a slot to toggle the overlay visibility
    connect(toggleButton, &QToolButton::clicked, this, [this]() {
        overlayWidget1->setVisible(!overlayWidget1->isVisible());
    });

    //Create the toggle button for the second overlay widget
    QToolButton *toggleButton2 = new QToolButton(this);
    toggleButton2->setIcon(QIcon(":/res/files/point.png")); // Add your icon here
    toggleButton2->setIconSize(QSize(100, 100));
    toggleButton2->setGeometry(50, 9, 35, 35);  // Adjust the size and position as needed
    toggleButton2->setStyleSheet(stylesheet);

    // Connect the toggle button to open ImageWindow
    connect(toggleButton2, &QToolButton::clicked, this, &RoamSchool::toggleButton2Clicked);

    // Initialize the exit button
    ExitButton = new QPushButton(this);
    QPixmap ExitPixmap(":res/assets/Exit.png");
    ExitButton->setIcon(QIcon(ExitPixmap));
    ExitButton->setIconSize(ExitPixmap.size() / 6);
    ExitButton->setFixedSize(ExitPixmap.size() / 6);
    ExitButton->setFlat(true); // Make button background transparent
    ExitButton->setVisible(true);
    ExitButton->move(screenGeometry.width() - 85, 10);
    connect(ExitButton, &QPushButton::clicked, this, &RoamSchool::onExitButtonClicked);

    // Ensure the overlay widgets and toggle button are always on top
    overlayWidget1->raise();
    toggleButton->raise();
}

void RoamSchool::readToComboBoxFromFile(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        // Handle error if the file cannot be opened
        qWarning() << "Could not open file for reading.";
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(',');

        if (!fields.isEmpty()) {
            sourceBox->addItem(fields[1]);
            destinationBox->addItem(fields[1]);
        }
    }

    file.close();
}

void RoamSchool::toggleButton2Clicked()
{
    // Specify the file path to the image
    QString imagePath = ":/res/files/Legend.png"; // Update with your image file path

    // Open the ImageWindow with the specified image file path
    ImageWindow *imageWindow = new ImageWindow(imagePath, this);
    imageWindow->show();
}

void RoamSchool::submitButtonClicked() {
    // Add your functionality here
    qDebug() << "Submit button clicked!";

    QString source = sourceBox->currentText();
    QString dest = destinationBox->currentText();
    int sourceNode, destNode;

    QString filePath = ":/res/files/mapping.csv";
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        // Handle error if the file cannot be opened
        qWarning() << "Could not open file for reading.";
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(',');

        if (!fields.isEmpty()) {
            if (strcmp(source.toStdString().c_str(), fields[1].toStdString().c_str()) == 0) {
                sourceNode = fields[0].toInt();
            }

            if (strcmp(dest.toStdString().c_str(), fields[1].toStdString().c_str()) == 0) {
                destNode = fields[0].toInt();
            }
        }
    }

    file.close();

    DijkstraAlgorithm dijkstra;

    outputTextEdit->clear();
    QString Output;
    QVector<QString> path = dijkstra.findShortestPath(sourceNode, destNode);
    for (int i = 0;  i < path.size(); i++) {
        Output += path[i];
        if ( i < path.size() - 1)
            Output += " >> ";
    }
    outputTextEdit->setText(Output);
    outputTextEdit->setWordWrap(true); // Enable word wrap for the label
    outputTextEdit->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    outputTextEdit->adjustSize();

    QString combinedText = dijkstra.getDistance(destNode) + " meters";
    outputLineEdit->setText(combinedText);
}

void RoamSchool::on_sourceBox_currentIndexChanged()
{
    QString selectedSource = sourceBox->currentText();
    QString selectedDestination = destinationBox->currentText();

    // If the selected item in sourceBox matches the item selected in destinationBox,
    // reset the selection in destinationBox
    if (selectedSource == selectedDestination) {
        // Disconnect the signal temporarily to avoid recursion
        disconnect(destinationBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &RoamSchool::on_destinationBox_currentIndexChanged);

        // Reset the selection in destinationBox
        destinationBox->setCurrentIndex(-1);

        // Reconnect the signal
        connect(destinationBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &RoamSchool::on_destinationBox_currentIndexChanged);
    }
}

void RoamSchool::on_destinationBox_currentIndexChanged()
{
    // Retrieve the selected item text from destinationBox
    QString selectedItem = destinationBox->currentText();
}

void RoamSchool::onExitButtonClicked() {
    QCoreApplication::quit();
}
