#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <QMainWindow>
#include <QtNetwork>
#include <QNetworkRequest>
#include <QMessageBox>
#include "env.h"


class transactiontwo
{
public:
    transactiontwo(QMainWindow *mwindow);

    void withdrawFunds(int amount, QString card, QString account, QString token);
    QString withdrawReplySlot(QNetworkReply *reply);
    void depositFunds(int amount, QString card, QString account, QString token);
    QString depositReplySlot(QNetworkReply *reply);

private:
    QNetworkAccessManager *networkManager;
    QNetworkReply *reply;

    QMainWindow *window;
};

#endif // TRANSACTION_H
