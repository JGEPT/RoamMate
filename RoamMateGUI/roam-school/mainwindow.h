#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QScrollArea>
#include <QComboBox>
#include <QPlainTextEdit>
#include <QLineEdit>

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
    void toggleButton2Clicked();


private:
    Ui::MainWindow *ui;
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

};
#endif // MAINWINDOW_H
