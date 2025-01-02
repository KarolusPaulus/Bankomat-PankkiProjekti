#include "transaction.h"

transactiontwo::transactiontwo(QMainWindow *mwindow) {
    window = mwindow;
}

void transactiontwo::withdrawFunds(int amount, QString card, QString account, QString token)
{
    qDebug() << "withdrawFunds()";

    QJsonObject sentData;
    sentData.insert("card", card);
    sentData.insert("account", account);
    sentData.insert("amount", amount);

    QString url = env::getUrl() + "/transaction/withdraw";
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QByteArray myToken="Bearer " + token.toUtf8();
    request.setRawHeader(QByteArray("Authorization"),(myToken));

    networkManager = new QNetworkAccessManager(window);
    QAbstractEventDispatcher::connect(networkManager, SIGNAL(finished(QNetworkReply*)), window, SLOT(withdrawReplySlot(QNetworkReply*)));

    reply = networkManager->post(request, QJsonDocument(sentData).toJson());
}

QString transactiontwo::withdrawReplySlot(QNetworkReply *reply)
{
    qDebug() << "withdrawReplySlot()";

    QByteArray data = reply->readAll();

    //qDebug() << data;

    QString message;

    if (data.length() == 0 || data == "-4078") {
        qDebug() << "Tietoliikenneyhteysvika";
        message = "Network error";
        reply->deleteLater();
        networkManager->deleteLater();
        return message;
    }

    if (data == "false") {
        qDebug() << "Virhe tietojen hankinnassa";
        message = "Transaction unsuccesful";
        reply->deleteLater();
        networkManager->deleteLater();
        return message;
    }

    qDebug() << "Money withdrawn successfully";

    reply->deleteLater();
    networkManager->deleteLater();

    message = "Money withdrawn successfully!";

    return message;
}

void transactiontwo::depositFunds(int amount, QString card, QString account, QString token)
{
    qDebug() << "depositFunds()";
    QJsonObject sentData;
    sentData.insert("card", card);
    sentData.insert("account", account);
    sentData.insert("amount", amount);

    QString url = env::getUrl() + "/transaction/deposit";
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QByteArray myToken="Bearer " + token.toUtf8();
    request.setRawHeader(QByteArray("Authorization"),(myToken));

    networkManager = new QNetworkAccessManager(window);
    QAbstractEventDispatcher::connect(networkManager, SIGNAL(finished(QNetworkReply*)), window, SLOT(depositReplySlot(QNetworkReply*)));

    reply = networkManager->post(request, QJsonDocument(sentData).toJson());
}

QString transactiontwo::depositReplySlot(QNetworkReply *reply)
{
    qDebug() << "depositReplySlot()";

    QByteArray data = reply->readAll();
    //qDebug() << data;

    QString message;

    if (data.length() == 0 || data == "-4078") {
        qDebug() << "Tietoliikenneyhteysvika";
        message = "Network error";
        reply->deleteLater();
        networkManager->deleteLater();
        return message;
    }

    if (data == "false") {
        qDebug() << "Virhe tietojen hankinnassa";
        message = "Transaction unsuccesful";
        reply->deleteLater();
        networkManager->deleteLater();
        return message;
    }

    qDebug() << "Money deposited successfully";

    reply->deleteLater();
    networkManager->deleteLater();

    message = "Money deposited successfully!";

    return message;
}

