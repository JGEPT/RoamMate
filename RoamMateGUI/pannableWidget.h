#ifndef PANNABLEWIDGET_H
#define PANNABLEWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QPixmap>
#include <QPoint>
#include <QLineEdit>
#include <QScrollArea>
#include <QLabel>
#include <QPropertyAnimation>
#include <QVBoxLayout>
#include "RoamMateReq.cpp"

struct ButtonInfo {
    QPushButton *button;
    QPoint position;
    QString category;
};

struct Location {
    QString category;
    QString name;
};

class PannableWidget : public QWidget {
    Q_OBJECT
    Q_PROPERTY(QPointF menuPosition READ menuPosition WRITE setMenuPosition NOTIFY PosChanged)
    Q_PROPERTY(qreal menuRotation READ menuRotation WRITE setMenuRotation NOTIFY RotChanged)
public:
    PannableWidget(const QString &imagePath, QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

signals:
    void buttonPressed(); // Add this signal
    void closePressed(); // Add this signal
    void PosChanged();
    void RotChanged();

private slots:
    void onButtonClicked(const QString &category);
    void onExitButtonClicked();
    void onCloseButtonClicked();
    void onSearchButtonClicked();
    void onHistoryButtonClicked();
    void onSearchConfirmButtonClicked();

private:
    void addButton(const QString &iconPath, const QPoint &position, const QString &category, int scale);
    void setButtonsVisible(bool visible);
    void drawMenu(QPainter &painter);
    void drawSearchMenu(QPainter &painter);
    void setOrigin(const QString &origin);
    void setDestination(const QString &destination);
    void addLocation(const QString &category, const QString &name) {
        locations.append({category, name});
    }

    QPointF menuPosition() const;
    void setMenuPosition(const QPointF &position);
    qreal menuRotation() const;
    void setMenuRotation(qreal rotation);

    QPixmap background;
    QPoint offset;
    QPoint lastMousePos;
    QVector<ButtonInfo> buttons;

    bool showMenu;
    bool showSearchMenu;
    QPointF m_menuPosition;
    qreal m_menuRotation;
    QRect menuRect;
    QRect searchMenuRect;

    QPushButton *ExitButton;
    QPushButton *SearchButton;
    QPushButton *closeButton;
    QPushButton *searchConfirmButton;
    QPushButton *HistoryButton;
    QLineEdit *originLineEdit;
    QLineEdit *destinationLineEdit;
    QScrollArea *scrollArea;
    QWidget *scrollAreaContent;
    QVBoxLayout *scrollAreaLayout;
    QLabel *processingLabel;
    QLabel *routeLabel;

    QPropertyAnimation *positionAnimation;
    QPropertyAnimation *rotationAnimation;
    QPropertyAnimation *searchPositionAnimation;
    QPropertyAnimation *searchRotationAnimation;

    bool ButtonClicked;
    bool SearchClicked;

    QString currentCategory;
    QVector<Location> locations; // Add this line
    QString requestResultText;
    QVector<QWidget *> menuButtons;
};

#endif // PANNABLEWIDGET_H






