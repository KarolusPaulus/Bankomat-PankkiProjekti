#ifndef PROFILEWINDOW_H
#define PROFILEWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QGridLayout>
#include <QTableWidgetItem>
#include <QSize>
#include <QJsonObject>
#include <QJsonDocument>
#include <QtNetwork>
#include <QNetworkRequest>

class ProfileWindow : public QWidget
{
public:
    ProfileWindow();
    void attachWindow(QWidget *window);
    void updateUserData(QByteArray data);

private:
    QLabel *descriptionLabels[8];
    QTableWidgetItem *transactions[3][5];
    QTableWidget *table;
    //QByteArray data;
};

#endif // PROFILEWINDOW_H
