#ifndef TRANSACTIONHISTORY_H
#define TRANSACTIONHISTORY_H
#include <QString>
#include <QDateTime>
#include <QList>
#include <QMainWindow>
#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QStandardItem>
#include <QDialog>
#include <QLabel>
#include <QSize>
#include <QJsonObject>
#include <QNetworkRequest>
#include <QTableView>
#include <QColor>
#include <QColorDialog>
#include <QPalette>


QT_BEGIN_NAMESPACE
namespace Ui {
class transactionHistory;
}
QT_END_NAMESPACE

class transactionHistory : public QDialog
{
    Q_OBJECT

public:
    explicit transactionHistory(QWidget *parent = nullptr);
    ~transactionHistory();

    void attachWindow(QWidget *window);
    int addEvents(int pageNum);
    void getEventSlot(QByteArray data);

private:

    QNetworkAccessManager *getManager;
    QByteArray response_data;
    QList<transactionHistory> eventList;
    QTableView *tableView;

};

#endif // TRANSACTIONHISTORY_H
