#ifndef PANNABLEWIDGET_H
#define PANNABLEWIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QPushButton>
#include <QVector>
#include <QPoint>
#include <QColor>
#include <QPropertyAnimation>
#include <QLineEdit>
#include <QString> // Include QString
#include <QLabel>  // Include QLabel for displaying route and fare

#include "RoamMateReq.cpp" // Include the RouteRequest class

class PannableWidget : public QWidget {
    Q_OBJECT
    Q_PROPERTY(QPointF menuPosition READ menuPosition WRITE setMenuPosition NOTIFY PosChanged);
    Q_PROPERTY(qreal menuRotation READ menuRotation WRITE setMenuRotation NOTIFY RotChanged);

public:
    explicit PannableWidget(const QString &imagePath, QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

signals:
    void PosChanged();
    void RotChanged();

private slots:
    void onButtonClicked();
    void onCloseButtonClicked();
    void onExitButtonClicked();
    void onSearchButtonClicked();
    void onSearchConfirmButtonClicked();

private:
    struct ButtonInfo {
        QPushButton *button;
        QPoint position;
    };

    void addButton(const QString &iconPath, const QPoint &position);
    void setButtonsVisible(bool visible);
    void drawMenu(QPainter &painter);
    void drawSearchMenu(QPainter &painter);
    QPointF menuPosition() const;
    void setMenuPosition(const QPointF &position);
    qreal menuRotation() const;
    void setMenuRotation(qreal rotation);

    QPixmap background;
    QPoint offset;
    QPoint lastMousePos;
    bool showMenu;
    bool showSearchMenu;  // New flag for search menu visibility
    QRect menuRect;
    QRect searchMenuRect; // Rectangle for search menu
    QVector<ButtonInfo> buttons;
    QPropertyAnimation *positionAnimation;
    QPropertyAnimation *rotationAnimation;
    QPropertyAnimation *searchPositionAnimation;
    QPropertyAnimation *searchRotationAnimation;
    QPropertyAnimation *searchButtonPositionAnimation;
    QPointF m_menuPosition;
    qreal m_menuRotation;
    QPushButton *closeButton;
    QPushButton *ExitButton;
    QPushButton *SearchButton;
    QPushButton *searchConfirmButton;
    bool ButtonClicked;
    bool SearchClicked;
    QLineEdit *originLineEdit;
    QLineEdit *destinationLineEdit;
    QString originInput;
    QString destinationInput;

    // New members for route request and display
    RouteRequest *routeRequest;
    QLabel *routeLabel;
    QLabel *fareLabel;
};

#endif // PANNABLEWIDGET_H








