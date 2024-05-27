#ifndef PANNABLEWIDGET_H
#define PANNABLEWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QPixmap>
#include <QVector>

class PannableWidget : public QWidget {
    Q_OBJECT

public:
    explicit PannableWidget(const QString &imagePath, QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private slots:
    void onButtonClicked();

private:
    QPixmap background;
    QPoint offset;
    QPoint lastMousePos;

    struct ButtonInfo {
        QPushButton *button;
        QPoint position; // Position of the button relative to the background
    };

    QVector<ButtonInfo> buttons; // List of buttons and their positions
    void addButton(const QString &iconPath, const QPoint &position);
};

#endif // PANNABLEWIDGET_H
