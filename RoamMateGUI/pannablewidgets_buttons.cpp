// pannablewidget_buttons.cpp
#include "pannablewidget.h"

void PannableWidget::addButton(const QString &iconPath, const QPoint &position, const QString &category, int scale) {
    QPushButton *button = new QPushButton(this);
    QPixmap buttonImage(iconPath);
    button->setIcon(QIcon(buttonImage));
    button->setIconSize(buttonImage.size() / scale);
    button->setFixedSize(buttonImage.size() / scale);
    button->setFlat(true); // Make button background transparent

    // Connect the button's clicked signal to a lambda function that calls onButtonClicked with the category
    connect(button, &QPushButton::clicked, this, [this, category]() { onButtonClicked(category); });

    ButtonInfo buttonInfo = { button, position, category };
    buttons.append(buttonInfo);
}

void PannableWidget::setButtonsVisible(bool visible) {
    for (const ButtonInfo &buttonInfo : buttons) {
        buttonInfo.button->setVisible(visible);
    }
}
