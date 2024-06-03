#ifndef ROAMSCHOOL_H
#define ROAMSCHOOL_H

#include <QMainWindow>
#include <QLabel>
#include <QScrollArea>
#include <QComboBox>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QPushButton>

class RoamSchool : public QWidget
{
    Q_OBJECT

public:
    RoamSchool(QWidget *parent = nullptr);

private slots:
    void submitButtonClicked();
    void on_sourceBox_currentIndexChanged();
    void on_destinationBox_currentIndexChanged();
    void toggleButton2Clicked();
    void onExitButtonClicked();


private:
    void readToComboBoxFromFile(const QString &filePath);
    QLabel *backgroundLabel;
    QScrollArea *scrollArea;
    QComboBox *sourceBox;
    QComboBox *destinationBox;
    QPlainTextEdit *outputTextEdit;
    QLineEdit *outputLineEdit;
    QWidget *overlayWidget1;
    QWidget *overlayWidget2;
    QLabel *imageLabel;
    QPixmap originalPixmap;
    QPushButton *ExitButton;

};
#endif // ROAMSCHOOL_H
