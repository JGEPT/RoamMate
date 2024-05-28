#ifndef PANNABLEWIDGET_H
#define PANNABLEWIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QPushButton>
#include <QVector>
#include <QPoint>
#include <QColor>
#include <QPropertyAnimation>

class PannableWidget : public QWidget {
    Q_OBJECT
    Q_PROPERTY(qreal menuOpacity READ menuOpacity WRITE setMenuOpacity)

public:
    explicit PannableWidget(const QString &imagePath, QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private slots:
    void onButtonClicked();

private:
    struct ButtonInfo {
        QPushButton *button;
        QPoint position;
    };

    void addButton(const QString &iconPath, const QPoint &position);
    void setButtonsVisible(bool visible);
    void drawMenu(QPainter &painter);
    qreal menuOpacity() const;
    void setMenuOpacity(qreal opacity);

    QPixmap background;
    QPoint offset;
    QPoint lastMousePos;
    bool showMenu;
    QRect menuRect;
    QVector<ButtonInfo> buttons;
    QPropertyAnimation *animation;
    qreal m_menuOpacity;
};

#endif // PANNABLEWIDGET_H


