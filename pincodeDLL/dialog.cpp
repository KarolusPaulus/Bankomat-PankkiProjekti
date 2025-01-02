#include "dialog.h"
#include "ui_dialog.h"
#include <QRegularExpression>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
    ui->line->setMaxLength(4);

    ui->line->setEchoMode(QLineEdit::Password); //hides password

    connect(ui->enterBtn,SIGNAL(clicked(bool)),
            this,SLOT(clickHandler()));
    connect(ui->undoBtn, SIGNAL(clicked()), this, SLOT(undoClickHandler()));
    connect(ui->N1, SIGNAL(clicked()), this, SLOT(onN1Clicked()));
    connect(ui->N2, SIGNAL(clicked()), this, SLOT(onN2Clicked()));
    connect(ui->N3, SIGNAL(clicked()), this, SLOT(onN3Clicked()));
    connect(ui->N4, SIGNAL(clicked()), this, SLOT(onN4Clicked()));
    connect(ui->N5, SIGNAL(clicked()), this, SLOT(onN5Clicked()));
    connect(ui->N6, SIGNAL(clicked()), this, SLOT(onN6Clicked()));
    connect(ui->N7, SIGNAL(clicked()), this, SLOT(onN7Clicked()));
    connect(ui->N8, SIGNAL(clicked()), this, SLOT(onN8Clicked()));
    connect(ui->N9, SIGNAL(clicked()), this, SLOT(onN9Clicked()));
    connect(ui->N0, SIGNAL(clicked()), this, SLOT(onN0Clicked()));
    connect(this, SIGNAL(finished(int)), this, SLOT(clearLineEdit()));
}

Dialog::~Dialog()
{
    delete ui;
}

QString Dialog::getPincode() const
{
    return pincode;
}

void Dialog::numberClickedHandler()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    QString pin = button->text();
    QString currentText = ui->line->text();
    ui->line->setText(currentText + pin);
}

void Dialog::undoClickHandler()
{
    QString currentText = ui->line->text();
    if (!currentText.isEmpty()) {
        currentText.chop(1);
        ui->line->setText(currentText);
    }
}

void Dialog::onUndoClicked()
{
    undoClickHandler();
}

void Dialog::clickHandler()
{
    QString s = ui->line->text();
    emit sendString(s);
    pincode = s;
    emit pincodeReady();
    ui->line->clear();
    this->close();
}

void Dialog::onN1Clicked()
{
    numberClickedHandler();
}

void Dialog::onN2Clicked()
{
    numberClickedHandler();
}

void Dialog::onN3Clicked()
{
    numberClickedHandler();
}

void Dialog::onN4Clicked()
{
    numberClickedHandler();
}

void Dialog::onN5Clicked()
{
    numberClickedHandler();
}

void Dialog::onN6Clicked()
{
    numberClickedHandler();
}

void Dialog::onN7Clicked()
{
    numberClickedHandler();
}

void Dialog::onN8Clicked()
{
    numberClickedHandler();
}

void Dialog::onN9Clicked()
{
    numberClickedHandler();
}

void Dialog::onN0Clicked()
{
    numberClickedHandler();
}

void Dialog::clearLineEdit()
{
    ui->line->clear();
}

