#include "mainwindow.h"
#include "ui_mainwindow.h"
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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QPixmap backgroundImage(":/resources/files/school-map.jpg");


    // Scale the background image
    backgroundImage = backgroundImage.scaled(backgroundImage.size() * 0.5); // Increase size by a factor of 1.25

    // Create a label to display the background image
    QLabel *backgroundLabel = new QLabel;
    backgroundLabel->setPixmap(backgroundImage);

    // Create a scroll area to display the background image
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(backgroundLabel);
    setCentralWidget(scrollArea);

    // Create a widget to overlay on top of the background image
    QWidget *overlayWidget = new QWidget(this);
    overlayWidget->setGeometry(10, 10, 400, 350);  // Adjust the size and position as needed

    // Define the custom stylesheet
    QString stylesheet = R"(
        QWidget#overlayWidget {
            background-color: rgba(102, 25, 19, 200);
            border: 1px solid #000000;
            border-radius: 10px;
        }
        QLabel {
            font-size: 14px;
            font-weight: bold;
            color: #ffffff;
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
        QPlainTextEdit, QLineEdit, QPushButton {
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
    )";

    // Apply the stylesheet to the overlay widget and its children
    overlayWidget->setObjectName("overlayWidget");
    overlayWidget->setStyleSheet(stylesheet);

    // Create labels and comboboxes
    QLabel *sourceLabel = new QLabel("Source:", overlayWidget);
    QLabel *destinationLabel = new QLabel("Destination:", overlayWidget);

    // Set alignment for the labels
    sourceLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    destinationLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    sourceBox = new QComboBox(overlayWidget);
    destinationBox = new QComboBox(overlayWidget);

    QString filepath = ":/resources/files/mapping.csv";
    readToComboBoxFromFile(filepath);

    sourceBox->setCurrentIndex(0);
    destinationBox->setCurrentIndex(36);

    connect(sourceBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::on_sourceBox_currentIndexChanged);
    connect(destinationBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::on_destinationBox_currentIndexChanged);


    // Create the additional widgets
    outputTextEdit = new QPlainTextEdit(overlayWidget);
    outputLineEdit = new QLineEdit(overlayWidget);
    outputTextEdit->setReadOnly(true);
    outputLineEdit->setReadOnly(true);

    QPushButton *submitButton = new QPushButton("Submit", overlayWidget);
    // Connect the clicked signal of the QPushButton to the submitButtonClicked slot function
    connect(submitButton, &QPushButton::clicked, this, &MainWindow::submitButtonClicked);

    // Create labels
    QLabel *outputTextLabel = new QLabel("Shortest Path:", overlayWidget);
    QLabel *outputLineEditLabel = new QLabel("Distance Covered:", overlayWidget);

    // Create a layout for the overlay widget
    QVBoxLayout *overlayLayout = new QVBoxLayout(overlayWidget);
    overlayLayout->addWidget(sourceLabel);
    overlayLayout->addWidget(sourceBox);
    overlayLayout->addWidget(destinationLabel);
    overlayLayout->addWidget(destinationBox);
    overlayLayout->addWidget(outputTextLabel);
    overlayLayout->addWidget(outputTextEdit);
    overlayLayout->addWidget(outputLineEditLabel);
    overlayLayout->addWidget(outputLineEdit);
    overlayLayout->addWidget(submitButton);

    // Create the toggle button

    QToolButton *toggleButton = new QToolButton(this);
    toggleButton->setIcon(QIcon(":/resources/files/magnifying.png")); // Add your icon here
    toggleButton->setIconSize(QSize(100, 100));
    toggleButton->setGeometry(10, 9, 35, 35);  // Adjust the size and position as needed
    toggleButton->setStyleSheet(stylesheet);

    // Connect the toggle button to a slot to toggle the overlay visibility
    connect(toggleButton, &QToolButton::clicked, [overlayWidget]() {
        overlayWidget->setVisible(!overlayWidget->isVisible());
    });

    // Ensure the overlay widget and toggle button are always on top
    overlayWidget->raise();
    toggleButton->raise();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::readToComboBoxFromFile(const QString &filePath) {
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

void MainWindow::submitButtonClicked() {
    // Add your functionality here
    qDebug() << "Submit button clicked!";

    QString source = sourceBox->currentText();
    QString dest = destinationBox->currentText();
    int sourceNode, destNode;

    QString filePath = ":/resources/files/mapping.csv";
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
    QVector<QString> path = dijkstra.findShortestPath(sourceNode, destNode);
    for (int i = 0;  i < path.size(); i++) {
        outputTextEdit->appendPlainText(path[i]);
        if ( i < path.size() - 1)
            outputTextEdit->appendPlainText(" >> ");
    }

    QString combinedText = dijkstra.getDistance(destNode) + " meters";
    outputLineEdit->setText(combinedText);
}

void MainWindow::on_sourceBox_currentIndexChanged()
{
    QString selectedSource = sourceBox->currentText();
    QString selectedDestination = destinationBox->currentText();

    // If the selected item in sourceBox matches the item selected in destinationBox,
    // reset the selection in destinationBox
    if (selectedSource == selectedDestination) {
        // Disconnect the signal temporarily to avoid recursion
        disconnect(destinationBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::on_destinationBox_currentIndexChanged);

        // Reset the selection in destinationBox
        destinationBox->setCurrentIndex(-1);

        // Reconnect the signal
        connect(destinationBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::on_destinationBox_currentIndexChanged);
    }
}

void MainWindow::on_destinationBox_currentIndexChanged()
{
    // Retrieve the selected item text from destinationBox
    QString selectedItem = destinationBox->currentText();

    // Remove the selected item from sourceBox
    disconnect(sourceBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::on_sourceBox_currentIndexChanged);
    int indexToRemove = sourceBox->findText(selectedItem);
    if (indexToRemove != -1) {
        sourceBox->removeItem(indexToRemove);
    }
    connect(sourceBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::on_sourceBox_currentIndexChanged);
}


