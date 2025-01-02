#ifndef DIALOG_H
#define DIALOG_H
#include "pincodeDLL_global.h"
#include <QDialog>
#include <QPushButton>
#include <QGridLayout>
#include <QRegularExpression>

namespace Ui {
class Dialog;
}

class PINCODEDLL_EXPORT Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

    QString getPincode() const;

private slots:
    void clickHandler();
    void numberClickedHandler();
    void undoClickHandler();
    void onUndoClicked();
    void onN1Clicked();
    void onN2Clicked();
    void onN3Clicked();
    void onN4Clicked();
    void onN5Clicked();
    void onN6Clicked();
    void onN7Clicked();
    void onN8Clicked();
    void onN9Clicked();
    void onN0Clicked();
    void clearLineEdit();


signals:
    void sendString(QString);
    void pincodeReady();

private:
    Ui::Dialog *ui;
    QString pincode;
};

#endif // DIALOG_H
