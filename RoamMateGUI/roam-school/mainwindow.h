#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qplaintextedit.h"
#include <QMainWindow>
#include <QLabel>
#include <QScrollArea>
#include <QComboBox>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void submitButtonClicked();
    void on_sourceBox_currentIndexChanged();
    void on_destinationBox_currentIndexChanged();

private:
    Ui::MainWindow *ui;
    void readToComboBoxFromFile(const QString &filePath);
    QLabel *backgroundLabel;
    QScrollArea *scrollArea;
    QComboBox *sourceBox;
    QComboBox *destinationBox;
    QPlainTextEdit *outputTextEdit;
    QLineEdit *outputLineEdit;
};
#endif // MAINWINDOW_H
