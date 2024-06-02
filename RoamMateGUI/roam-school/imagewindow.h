#ifndef IMAGEWINDOW_H
#define IMAGEWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QVBoxLayout>

class ImageWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ImageWindow(const QString &imageFilePath, QWidget *parent = nullptr);

private:
    QLabel *imageLabel;
    QVBoxLayout *mainLayout;
};

#endif // IMAGEWINDOW_H
