#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>  //Tuomas
#include <QSerialPortInfo> //Tuomas
#include <QDebug>   //Tuomas
#include <QMessageBox>
#include <QMovie>
#include <QDialog>
#include <QFile>
#include <QDir>
#include <QJsonObject>
#include <QJsonDocument>
#include <QtNetwork>
#include <QNetworkRequest>
#include "profilewindow.h"
#include "dialog.h"
#include "env.h"
#include <QList>
#include "transactionHistory.h"
#include "transaction.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void cardVerificationSlot(QNetworkReply *reply);
    void profileDataSlot(QNetworkReply *reply);
    void attachedAccountCheckSlot(QNetworkReply *reply);
    void transactionEventsData(QNetworkReply *reply);
    void loginSlot(QNetworkReply *reply);

    void onBtnEnterPinClicked();
    void onBtnValitseCreditClicked();
    void onBtnValitseDebitClicked();
    void onBtnKirjauduUlosClicked();

    void onBtnNostaRahaaClicked();

    void onBtnPaneRahaaClicked();//tama uusi

    void onBtnTilitapahtumatClicked();
    void onBtnTakaisinClicked();
    void onBtnTakaisin2Clicked();
    void onBtnTakaisin3Clicked();
    void onBtnKatsoTiedotClicked();
    void muuSummaClicked();
    void muuSumma_3Clicked();
    void onN20Clicked();
    void onN40Clicked();
    void onN50Clicked();
    void onN100Clicked();

    void clearSum();
    void hideLessAndMoreButtons();
    void updateLabelWithdrawSum(int);
    void onMoreButtonClicked();
    void onLessButtonClicked();
    void withdrawClickHandler();
    void withdrawReplySlot(QNetworkReply *reply);

    void onN20_3Clicked();
    void onN40_3Clicked();
    void onN50_3Clicked();
    void onN100_3Clicked();
    void onLessButtonClicked_3();
    void onMoreButtonClicked_3();
    void clearSum_3();
    void onBtnTakaisinClicked_3();
    void updateLabelDepositSum(int);
    void depositClickHandler();//tama uusi
    void depositReplySlot(QNetworkReply *reply);//tama uusi

    void clearGifs();
    void restartGifs();
    void displayGifsOnStartMenu();
    void displayMoneyGif();
    void handleClick();
    void onnextButtonclicked();
    void onpreviousButtonclicked();
    void checkPage();
    void onBtnOpenPortclicked();
    void readData();

private:
    Ui::MainWindow *ui;
    Dialog *ptr_dll;

    QString cardNo;
    QString accountNo;
    QString creditAccount, debitAccount;
    QString token;
    QString accountBalance;

    ProfileWindow *accountInfo;
    
    QSerialPort *_serialPort;

    QNetworkAccessManager *cardVerificationManager;
    QNetworkAccessManager *transferManager;
    QNetworkAccessManager *transferManagerEvents;
    QNetworkAccessManager *loginManager;
    QNetworkAccessManager *accountCheckManager;
    
    QNetworkReply *cardVerificationReply;
    QNetworkReply *reply;
    QNetworkReply *accountCheckReply;
    QNetworkReply *replyEvents;
    
    QByteArray data;

    int currentPage = 0;
    int maxPage;

    QLabel *arro = nullptr;
    QLabel *arro2 = nullptr;
    QLabel *money = nullptr;
    QLabel *label = nullptr;
    
    transactionHistory * eventData;
    transactiontwo *transaction;
    
    void verifyCard();
    void checkAttachedAccounts();
    void loadPorts();
};
#endif // MAINWINDOW_H
