#ifndef CUSTOMDIALOG_H
#define CUSTOMDIALOG_H

#include <QDialog>

class CustomDialog : public QDialog {
    Q_OBJECT

public:
    explicit CustomDialog(QWidget *parent = nullptr);

private slots:
    void onOkButtonClicked();
};

#endif // CUSTOMDIALOG_H
