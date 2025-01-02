#include "profilewindow.h"

ProfileWindow::ProfileWindow() {}

void ProfileWindow::attachWindow(QWidget *window)
{
    descriptionLabels[0] = window->findChild<QLabel*>("displayAccountOwner");
    descriptionLabels[1] = window->findChild<QLabel*>("displayAccountID");
    descriptionLabels[2] = window->findChild<QLabel*>("displayAccountOwnerID");
    descriptionLabels[3] = window->findChild<QLabel*>("displayAccountBalance");
    descriptionLabels[4] = window->findChild<QLabel*>("displayAccountCreditLimit");
    descriptionLabels[5] = window->findChild<QLabel*>("displayCardID");
    descriptionLabels[6] = window->findChild<QLabel*>("displayCardOwner");
    descriptionLabels[7] = window->findChild<QLabel*>("displayCardOwnerID");

    table = window->findChild<QTableWidget*>("transactionTable");

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 5; j++) {
            transactions[i][j] = new QTableWidgetItem;
            table->setItem(j, i, transactions[i][j]);
        }
    }
}

void ProfileWindow::updateUserData(QByteArray data)
{
    QJsonDocument dataUnpacked = QJsonDocument::fromJson(data);
    QJsonArray array = dataUnpacked.array();
    QJsonObject info = array[0].toObject();
    descriptionLabels[0]->setText(info["account_owner"].toString());
    descriptionLabels[1]->setText(info["id_account"].toString());
    descriptionLabels[2]->setText(QString::number(info["id_account_owner"].toInt()));
    descriptionLabels[3]->setText(info["balance"].toString());
    descriptionLabels[4]->setText(info["credit_limit"].toString());
    descriptionLabels[5]->setText(info["id_card"].toString());
    descriptionLabels[6]->setText(info["card_owner"].toString());
    descriptionLabels[7]->setText(QString::number(info["id_card_owner"].toInt()));

    for (int i = 0; i < 5; i++) {
        QJsonObject transaction = array[i + 1].toObject();
        transactions[0][i]->setText(transaction["amount"].toString());
        QStringList beautifiedTime = transaction["time"].toString().split(QRegularExpression("T|:\\d{1,2}\\.\\d{1,3}Z"), Qt::SkipEmptyParts);//, Qt::SkipEmptyParts);
        if (beautifiedTime.length() > 0) {
            transactions[1][i]->setText(beautifiedTime.at(0) + " klo " + beautifiedTime.at(1)); // + " " + beautifiedTime.at(1));
        }
        else {
            transactions[1][i]->setText(" "); // + " " + beautifiedTime.at(1));
        }
        transactions[2][i]->setText(transaction["description"].toString());
    }
}
