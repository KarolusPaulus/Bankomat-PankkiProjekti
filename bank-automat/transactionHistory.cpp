#include "transactionHistory.h"
#include "env.h"

transactionHistory::transactionHistory(QWidget *parent): QDialog(parent)
{
   // tableView = new QTableView(this);
    //tableView->setObjectName("tableView");
}

transactionHistory::~transactionHistory()
{

}

void transactionHistory::attachWindow(QWidget *window)
{
    tableView = window->findChild<QTableView*>("tableView");
    if (!tableView) {
        qDebug() << "Error";
    }
}

int transactionHistory::addEvents(int pageNum)
{
    //qDebug() << response_data;

    QJsonDocument json_doc = QJsonDocument::fromJson(response_data);
    QJsonArray json_array = json_doc.array();
    int eventsOnPage = 10;
    int maxNum = ((json_array.size() - 1) / eventsOnPage) + 1;

    QStandardItemModel *table_model = new QStandardItemModel(eventsOnPage, 3);
    table_model->setHeaderData(0, Qt::Horizontal, QObject::tr("Amount"));
    table_model->setHeaderData(1, Qt::Horizontal, QObject::tr("Description"));
    table_model->setHeaderData(2, Qt::Horizontal, QObject::tr("Date"));

    //qDebug()<<"arrayn koko:" << json_array.size();
    //qDebug()<< "Viimeinen sivu: "<<maxNum;
    for (int row = (pageNum - 1) * eventsOnPage; row < (pageNum * eventsOnPage); ++row) {
        if (row >= json_array.size()) {
            break;
        }

        //qDebug()<<row;
        QJsonObject events = json_array[row].toObject();
        QStandardItem *amount = new QStandardItem(events["amount"].toString());
        table_model->setItem(row-((pageNum-1)*eventsOnPage), 0, amount);
        QStandardItem *description = new QStandardItem(events["description"].toString());
        table_model->setItem(row-((pageNum-1)*eventsOnPage), 1, description );
        QStandardItem *date = new QStandardItem(events["time"].toString());
        table_model->setItem(row-((pageNum-1)*eventsOnPage), 2, date);
    }

    QString styleSheet = "QObject {"
                         "background-color: #426ca4;"
                         "color: #ffffff;"
                         "border-width: 1px;"
                         "border-radius: 0px;"
                         "border-color: #051a39;"
                         "padding: 2px;"
                         "}";

    tableView->setStyleSheet(styleSheet);
    tableView->setModel(table_model);

    return maxNum;
}


void transactionHistory::getEventSlot(QByteArray data)
{
    response_data = data;
}


