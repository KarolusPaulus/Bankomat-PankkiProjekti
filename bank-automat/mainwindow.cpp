#include "mainwindow.h"
#include "ui_mainwindow.h"

/*Initial setup
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , _serialPort(nullptr)
{
    cardNo = "";
    accountNo = "";
    debitAccount = "";
    creditAccount = "";
    token = "";
    currentPage = 1;
    maxPage = 1;

    ui->setupUi(this);

    ptr_dll = new Dialog(this);
    transaction = new transactiontwo(this);

    ui->setupUi(this);
    loadPorts();

    connect(ptr_dll,SIGNAL(pincodeReady()),this,SLOT(onBtnEnterPinClicked()));
    connect(ui->btnValitseCredit, SIGNAL(clicked()), this, SLOT(onBtnValitseCreditClicked()));
    connect(ui->btnValitseDebit, SIGNAL(clicked()), this, SLOT(onBtnValitseDebitClicked()));
    connect(ui->btnKirjauduUlos, SIGNAL(clicked()), this, SLOT(onBtnKirjauduUlosClicked()));
    connect(ui->btnNostaRahaa, SIGNAL(clicked()), this, SLOT(onBtnNostaRahaaClicked()));
    connect(ui->btnPaneRahaa, SIGNAL(clicked()), this, SLOT(onBtnPaneRahaaClicked()));//uusi
    connect(ui->btnTilitapahtumat, SIGNAL(clicked()), this, SLOT(onBtnTilitapahtumatClicked()));
    connect(ui->btnTakaisin, SIGNAL(clicked()), this, SLOT(onBtnTakaisinClicked()));
    connect(ui->btnTakaisin2, SIGNAL(clicked()), this, SLOT(onBtnTakaisin2Clicked()));
    connect(ui->btnTakaisin3, SIGNAL(clicked()), this, SLOT(onBtnTakaisin3Clicked()));
    connect(ui->btnKatsoTiedot, SIGNAL(clicked()), this, SLOT(onBtnKatsoTiedotClicked()));
    connect(ui->previousButton, SIGNAL(clicked()), this, SLOT(onpreviousButtonclicked()));
    connect(ui->nextButton, SIGNAL(clicked()), this, SLOT(onnextButtonclicked()));
    connect(ui->btnOpenPort, SIGNAL(clicked()), this, SLOT(onBtnOpenPortclicked()));

    //transaction window buttons
    connect(ui->muuSumma, SIGNAL(clicked()), this, SLOT(muuSummaClicked()));
    connect(ui->N20, SIGNAL(clicked()), this, SLOT(onN20Clicked()));
    connect(ui->N40, SIGNAL(clicked()), this, SLOT(onN40Clicked()));
    connect(ui->N50, SIGNAL(clicked()), this, SLOT(onN50Clicked()));
    connect(ui->N100, SIGNAL(clicked()), this, SLOT(onN100Clicked()));
    connect(ui->clearBtn, SIGNAL(clicked()), this, SLOT(clearSum()));
    connect(ui->lessBtn, SIGNAL(clicked()), this, SLOT(onLessButtonClicked()));
    connect(ui->moreBtn, SIGNAL(clicked()), this, SLOT(onMoreButtonClicked()));
    connect(ui->withdrawBtn,SIGNAL(clicked(bool)), this,SLOT(withdrawClickHandler()));

//uutta deposittia
    connect(ui->btnTakaisin_3, SIGNAL(clicked()), this, SLOT(onBtnTakaisinClicked_3()));
    connect(ui->muuSumma, SIGNAL(clicked()), this, SLOT(muuSummaClicked()));
    connect(ui->N20_3, SIGNAL(clicked()), this, SLOT(onN20_3Clicked()));
    connect(ui->N40_3, SIGNAL(clicked()), this, SLOT(onN40_3Clicked()));
    connect(ui->N50_3, SIGNAL(clicked()), this, SLOT(onN50_3Clicked()));
    connect(ui->N100_3, SIGNAL(clicked()), this, SLOT(onN100_3Clicked()));
    connect(ui->clearBtn_3, SIGNAL(clicked()), this, SLOT(clearSum_3()));
    connect(ui->lessBtn_3, SIGNAL(clicked()), this, SLOT(onLessButtonClicked_3()));
    connect(ui->moreBtn_3, SIGNAL(clicked()), this, SLOT(onMoreButtonClicked_3()));
    connect(ui->depositBtn,SIGNAL(clicked(bool)), this,SLOT(depositClickHandler()));

    ui->stackedWidget->setCurrentIndex(0);

    displayGifsOnStartMenu();
    hideLessAndMoreButtons();

    accountInfo = new ProfileWindow;
    accountInfo->attachWindow(ui->stackedWidget);

    eventData = new transactionHistory(this);
    eventData->attachWindow(ui->stackedWidget);

    ui->labelKortinTila->setText(QString("Insert your card"));
}

void setMessageBoxStyles(QMessageBox& msgBox) {
    msgBox.setStyleSheet(
        "QDialog { background-color: #36548f; color: #ffffff; }"
        "QLabel { font-weight: bold; font-size: 15px; background-color: transparent; color: #ffffff; }"
        "QPushButton { font-weight: bold; font-size: 10px; background-color: #426ca4; color: #ffffff; border-style: solid; border-width: 1px; border-radius: 5px; border-color: transparent; padding: 10px; min-width: 20px; }"
        "QPushButton:hover { background-color: #54a6cc; color: #ffffff; border-style: solid; border-width: 3px; border-radius: 5px; border-color: #36548f; padding: 10px; min-width: 20px; }"
        "QPushButton:pressed { background-color: #284070; color: #ffffff; border-style: solid; border-width: 3px; border-radius: 5px; border-color: transparent; padding: 10px; min-width: 20px; }"
        );
}

void MainWindow::displayGifsOnStartMenu()
{
    QString gifs = env::gifFetchArrows();
    QMovie *movie = new QMovie(gifs);

    arro = new QLabel(this);
    arro->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    arro->setAlignment(Qt::AlignCenter);
    arro->setGeometry(145, 350, 250, 250);
    arro->setScaledContents(true);
    arro->setMovie(movie);

    arro2 = new QLabel(this);
    arro2->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    arro2->setGeometry(650, 355, 250, 250);
    arro2->setScaledContents(true);
    arro2->setMovie(movie);

    movie->start();

    qDebug() << "Arrows working";
}

void MainWindow::clearGifs()
{
    arro->movie()->setPaused(true);
    arro->setVisible(false);

    arro2->movie()->setPaused(true);
    arro2->setVisible(false);
}

void MainWindow::restartGifs()
{
    arro->setVisible(true);
    arro->movie()->setPaused(false);
    arro2->setVisible(true);
    arro2->movie()->setPaused(false);
}


/* Sets up a box that displays a gif (shown after you withdraw money)
 */
void MainWindow::displayMoneyGif()
{
    QString gifs = env::gifFetchMoney();
    QMovie *movie = new QMovie(gifs);

    money = new QLabel(this);
    money->setScaledContents(true);
    money->setMovie(movie);

    //label = new QLabel("Transaction Successful", this);

    movie->start();

    QMessageBox messageBox(this);
    messageBox.setWindowTitle("Transaction Successful!");
    //messageBox.setStandardButtons(QMessageBox::Ok);
    messageBox.setDefaultButton(QMessageBox::Ok);

    QGridLayout* layout = dynamic_cast<QGridLayout*>(messageBox.layout());
    if (layout)
        layout->addWidget(money, 0, 0, 1, layout->columnCount(), Qt::AlignCenter);
        //layout->addWidget(label, 1, 0, 1, layout->columnCount(), Qt::AlignCenter);

    setMessageBoxStyles(messageBox);
    messageBox.exec();

    movie->stop();
    delete movie;
    delete money;
    //delete label;

    qDebug() << "Moneyyy";
}


// Sets up data about the ports available for the card reader(?)
void MainWindow::loadPorts()
{
    foreach (auto &port, QSerialPortInfo::availablePorts()) {
        ui->cmbPorts->addItem(port.portName());
    }
}

/* Misceallaneous UI buttons
 */
void MainWindow::onBtnValitseCreditClicked()
{
    //qDebug() << "Credit valittu";

    accountNo = creditAccount;
    creditAccount = "";
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::onBtnValitseDebitClicked()
{
    //qDebug() << "Debit valittu";

    accountNo = debitAccount;
    debitAccount = "";
    ui->stackedWidget->setCurrentIndex(2);
}

// Specifically the "nosta rahaa" button which takes you to the withdrawal window
// (not the one that actually activates the transfer)
void MainWindow::onBtnNostaRahaaClicked()
{
    ui->stackedWidget->setCurrentIndex(3);
    clearSum();
}

void MainWindow::onBtnPaneRahaaClicked()
{
    ui->stackedWidget->setCurrentIndex(6);
    clearSum();
}

void MainWindow::onBtnTakaisinClicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::onBtnTakaisin2Clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::onBtnTakaisin3Clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::onBtnTakaisinClicked_3()
{
    ui->stackedWidget->setCurrentIndex(2);
}
void MainWindow::onBtnOpenPortclicked()
{
    if (_serialPort != nullptr) {
        _serialPort->close();
        delete _serialPort;
    }
    _serialPort = new QSerialPort(this);
    _serialPort->setPortName(ui->cmbPorts->currentText());
    _serialPort->setBaudRate(QSerialPort::Baud9600);
    _serialPort->setDataBits(QSerialPort::Data8);
    _serialPort->setParity(QSerialPort::NoParity);
    _serialPort->setStopBits(QSerialPort::OneStop);
    if (_serialPort->open(QIODevice::ReadOnly)) {
        /*QMessageBox msgBox(this);
        msgBox.setWindowTitle("Result");
        msgBox.setText("Port open");
        setMessageBoxStyles(msgBox);
        msgBox.exec();*/

        connect(_serialPort, &QSerialPort::readyRead, this, &MainWindow::readData);
    } else {
        //QMessageBox::critical(this, "Port Error", "Porttia ei voinut avata...");
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Port Error");
        msgBox.setText("Port closed");
        setMessageBoxStyles(msgBox);
        msgBox.exec();
    }

    // Debug
    //cardNo = "060006E2E7";
    //verifyCard();
}



void MainWindow::readData()
{
    if (!_serialPort->isOpen()) {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Port Error");
        msgBox.setText("Port closed");
        setMessageBoxStyles(msgBox);
        msgBox.exec();
        return;
    }

    if (ui->stackedWidget->currentIndex() == 0) {
        auto data = _serialPort->readAll();
        data.replace("\r\n-", "");
        data.replace("\r\n>", "");
        //ui->labelKortinNumero->setText(QString(data));
        ui->labelKortinTila->setText(QString("Card read: " + data));
        qDebug() << data;
        cardNo = data;

        verifyCard();
    }
}



/*  Sends the card number back to the server & asks whether the card is good for use
 *  at the bankomat (check includes making sure the card is in the database and unrestricted)
 */
void MainWindow::verifyCard()
{
    qDebug() << "verifyCard()";

    QJsonObject sentData;
    sentData.insert("card", cardNo);

    QString url = env::getUrl() + "/verifycard";
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    cardVerificationManager = new QNetworkAccessManager(this);
    connect(cardVerificationManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(cardVerificationSlot(QNetworkReply*)));

    accountCheckReply = cardVerificationManager->post(request, QJsonDocument(sentData).toJson());
}

void MainWindow::cardVerificationSlot(QNetworkReply *cardVerificationReply)
{
    qDebug() << "cardVerificationSlot()";
    QByteArray data = cardVerificationReply->readAll();
    QMessageBox msgBox;

    if(data.length()==0 || data == "-4078"){
        qDebug()<<"Tietoliikenneyhteysvika";
        msgBox.setText("Network error");
        setMessageBoxStyles(msgBox);
        msgBox.exec();
        cardVerificationReply->deleteLater();
        cardVerificationManager->deleteLater();
        return;
    }

    if (data == "false") {
        qDebug() << "Tietoa ei saatu";
        msgBox.setText("Invalid card");
        setMessageBoxStyles(msgBox);
        msgBox.exec();
        cardVerificationReply->deleteLater();
        cardVerificationManager->deleteLater();
        return;
    }

    handleClick();

    cardVerificationReply->deleteLater();
    cardVerificationManager->deleteLater();
}


/* Login
 */
// Brings up the pin code window
void MainWindow::handleClick()
{
    ptr_dll->show();
}

// Once you've entered a pin and clicked "enter" to login
void MainWindow::onBtnEnterPinClicked()
{
    qDebug()<<"enter clicked";

    //ui->stackedWidget->setCurrentIndex(1);

    QString pin = ptr_dll->getPincode();
    QJsonObject jsonObj;
    jsonObj.insert("card", cardNo);
    jsonObj.insert("pincode", pin);

    QString url = env::getUrl() + "/login";
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    //WEBTOKEN ALKU
    QByteArray myToken="Bearer "+token.toUtf8();
    request.setRawHeader(QByteArray("Authorization"),(myToken));
    //WEBTOKEN LOPPU

    loginManager = new QNetworkAccessManager(this);
    connect(loginManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(loginSlot(QNetworkReply*)));
    reply = loginManager->post(request, QJsonDocument(jsonObj).toJson());
}

void MainWindow::loginSlot(QNetworkReply *reply)
{
    data = reply->readAll();
    //qDebug()<<data;
    QMessageBox msgBox;
    if(data=="-4078" || data.length()==0){
        msgBox.setText("Network error");
        setMessageBoxStyles(msgBox);
        msgBox.exec();
    }
    else{
        if(data!="false"){
            qDebug() << data;
            token = data;
            qDebug() << "loginSlot(), data wasn't false";
            checkAttachedAccounts();

            if (arro && arro2) {
                clearGifs();
            }
        }
        else{
            msgBox.setText("Incorrect password");
            setMessageBoxStyles(msgBox);
            msgBox.exec();
        }
    }
    reply->deleteLater();
    loginManager->deleteLater();
}

/* Fetching data on the accounts attached to the card & if there are multiple,
 * bringing up the credit-debit choice window
 */
void MainWindow::checkAttachedAccounts()
{
    qDebug() << "checkAttachedAccounts()";

    QJsonObject sentData;
    sentData.insert("card", cardNo);

    QString url = env::getUrl() + "/getaccounts";
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QByteArray myToken="Bearer "+token.toUtf8();
    request.setRawHeader(QByteArray("Authorization"),(myToken));

    accountCheckManager = new QNetworkAccessManager(this);
    connect(accountCheckManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(attachedAccountCheckSlot(QNetworkReply*)));

    accountCheckReply = accountCheckManager->post(request, QJsonDocument(sentData).toJson());
}

void MainWindow::attachedAccountCheckSlot(QNetworkReply *reply)
{
    qDebug() << "attachedAccountCheckSlot()";

    QByteArray data = reply->readAll();
    QMessageBox msgBox;

    if (data=="-4078" || data.length()==0) {
        msgBox.setText("Network error");
        setMessageBoxStyles(msgBox);
        msgBox.exec();
    }
    else if(data == "false"){
        msgBox.setText("Data acquisition error");
        setMessageBoxStyles(msgBox);
        msgBox.exec();
    }
    else {
        QJsonDocument dataUnpacked = QJsonDocument::fromJson(data);
        //qDebug() << dataUnpacked;

        QJsonArray array = dataUnpacked.array();

        if (array.size() < 1) {
            msgBox.setText("No accounts attached to this card");
            setMessageBoxStyles(msgBox);
            msgBox.exec();
        }
        else if (array.size() > 1) {
            //qDebug() << "Tilin tyyppi: " << array[0].toObject()["type"].toInt();
            if (array[0].toObject()["type"].toInt() == 0) {
                creditAccount = array[0].toObject()["id_account"].toString();
                debitAccount = array[1].toObject()["id_account"].toString();
                //qDebug() << "Credit-tili:" << creditAccount << ", debit-tili:" << debitAccount;
            }
            else {
                creditAccount = array[1].toObject()["id_account"].toString();
                debitAccount = array[0].toObject()["id_account"].toString();

                //qDebug() << "Credit-tili:" << creditAccount << ", debit-tili:" << debitAccount;
            }
            ui->stackedWidget->setCurrentIndex(1);
        }
        else {
            accountNo = array[0].toObject()["id_account"].toString();

            ui->stackedWidget->setCurrentIndex(2);
        }
    }

    reply->deleteLater();
    accountCheckManager->deleteLater();
}


/* Viewing the user's profile (card & account data + some of the latest transactions)
 */
void MainWindow::onBtnKatsoTiedotClicked()
{
    QJsonObject sentData;
    sentData.insert("card", cardNo);
    sentData.insert("account", accountNo);

    QString url = env::getUrl() + "/viewprofile";
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QByteArray myToken="Bearer "+token.toUtf8();
    request.setRawHeader(QByteArray("Authorization"),(myToken));

    transferManager = new QNetworkAccessManager(this);
    connect(transferManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(profileDataSlot(QNetworkReply*)));

    reply = transferManager->post(request, QJsonDocument(sentData).toJson());
}

void MainWindow::profileDataSlot(QNetworkReply *reply)
{
    QByteArray data = reply->readAll();
    QMessageBox msgBox;

    if (data.length() == 0 || data == "-4078") {
        qDebug() << "Tietoliikenneyhteysvika";
        msgBox.setText("Network error");
        setMessageBoxStyles(msgBox);
        msgBox.exec();
        reply->deleteLater();
        transferManager->deleteLater();
        return;
    }

    if (data == "false") {
        qDebug() << "Virhe tietojen hankinnassa";
        msgBox.setText("Data acquisition error");
        setMessageBoxStyles(msgBox);
        msgBox.exec();
        reply->deleteLater();
        transferManager->deleteLater();
        return;
    }

    ui->stackedWidget->setCurrentIndex(5);

    // Extracts account balance data from the package and saves it
    QJsonDocument dataUnpacked = QJsonDocument::fromJson(data);
    QJsonArray array = dataUnpacked.array();
    QJsonObject info = array[0].toObject();
    //qDebug() << info["balance"];
    accountBalance = info["balance"].toString();

    accountInfo->updateUserData(data);

    reply->deleteLater();
    transferManager->deleteLater();
}


/* Viewing the entire transaction history of the current account
 */
void MainWindow::onBtnTilitapahtumatClicked()
{
    ui->previousButton->setVisible(false);
    maxPage = 1;
    currentPage = 1;
    QJsonObject sentData;
    sentData.insert("idaccount", accountNo);
    sentData.insert("card", cardNo);

    QString url = env::getUrl() + "/viewtransactions";
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    //WEBTOKEN ALKU
    QByteArray myToken="Bearer "+token.toUtf8();
    request.setRawHeader(QByteArray("Authorization"),(myToken));
    //WEBTOKEN LOPPU

    transferManagerEvents = new QNetworkAccessManager(this);
    connect(transferManagerEvents, SIGNAL(finished(QNetworkReply*)), this, SLOT(transactionEventsData(QNetworkReply*)));

    replyEvents = transferManagerEvents->post(request, QJsonDocument(sentData).toJson());
}

void MainWindow::transactionEventsData(QNetworkReply *reply)
{
    QByteArray data = reply->readAll();

    if(data.length()==0 || data == "-4078"){
        qDebug()<<"Tietoliikenneyhteysvika";
        reply->deleteLater();
        transferManagerEvents->deleteLater();
        return;
    }

    if (data == "false") {
        qDebug() << "Tietoa ei saatu";
        reply->deleteLater();
        transferManagerEvents->deleteLater();
        return;
    }

    ui->stackedWidget->setCurrentIndex(4);
    eventData->getEventSlot(data);

    maxPage = eventData->addEvents(currentPage);
    checkPage();


    replyEvents->deleteLater();
    transferManagerEvents->deleteLater();
}

void MainWindow::onpreviousButtonclicked()
{
    if (currentPage > 1) {
        currentPage--;
        maxPage = eventData->addEvents(currentPage);
    }

    checkPage();
}

void MainWindow::checkPage()
{
    if (currentPage == 1) {
        ui->previousButton->setVisible(false);
    }
    else {
        ui->previousButton->setVisible(true);
    }

    if (currentPage == maxPage) {
        ui->nextButton->setVisible(false);
    }
    else {
        ui->nextButton->setVisible(true);
    }
}

void MainWindow::onnextButtonclicked()
{
    if (currentPage <= maxPage) {
        currentPage++;
        maxPage = eventData->addEvents(currentPage);
    }

    qDebug()<<maxPage;

    checkPage();
}


/* Withdrawing funds
 */
//buttons
void MainWindow::muuSummaClicked()
{
    clearSum();
    ui->lessBtn->setVisible(true);
    ui->moreBtn->setVisible(true);
}

void MainWindow::onN20Clicked()
{
    hideLessAndMoreButtons();
    updateLabelWithdrawSum(20);
}

void MainWindow::onN40Clicked()
{
    hideLessAndMoreButtons();
    updateLabelWithdrawSum(40);
}

void MainWindow::onN50Clicked()
{
    hideLessAndMoreButtons();
    updateLabelWithdrawSum(50);
}

void MainWindow::onN100Clicked()
{
    hideLessAndMoreButtons();
    updateLabelWithdrawSum(100);
}

void MainWindow::muuSumma_3Clicked()
{
    clearSum();
    ui->lessBtn->setVisible(true);
    ui->moreBtn->setVisible(true);
}

void MainWindow::onN20_3Clicked()
{
    hideLessAndMoreButtons();
    updateLabelDepositSum(20);
}

void MainWindow::onN40_3Clicked()
{
    hideLessAndMoreButtons();
    updateLabelDepositSum(40);
}

void MainWindow::onN50_3Clicked()
{
    hideLessAndMoreButtons();
    updateLabelDepositSum(50);
}

void MainWindow::onN100_3Clicked()
{
    hideLessAndMoreButtons();
    updateLabelDepositSum(100);
}

void MainWindow::clearSum()
{
    ui->lessBtn->setVisible(false);
    ui->moreBtn->setVisible(false);
    ui->labelWithdrawSum->setText("0€");
}

void MainWindow::clearSum_3()
{
    ui->lessBtn->setVisible(false);
    ui->moreBtn->setVisible(false);
    ui->labelDepositSum->setText("0€");
}

void MainWindow::updateLabelWithdrawSum(int value)
{
    ui->labelWithdrawSum->setText(QString::number(value) + "€");
}

void MainWindow::updateLabelDepositSum(int value)
{
    ui->labelDepositSum->setText(QString::number(value) + "€");
}

void MainWindow::hideLessAndMoreButtons()
{
    ui->lessBtn->setVisible(false);
    ui->moreBtn->setVisible(false);
}

void MainWindow::onMoreButtonClicked()
{
    QString currentText = ui->labelWithdrawSum->text();

    int currentValue = currentText.replace("€", "").toInt();
    int newValue = currentValue + 10;

    ui->labelWithdrawSum->setText(QString::number(newValue) + "€");
}

void MainWindow::onLessButtonClicked()
{
    QString currentText = ui->labelWithdrawSum->text();

    int currentValue = currentText.replace("€", "").toInt();
    int newValue = currentValue - 10;

    if(newValue < 0)
        newValue = 0;

    ui->labelWithdrawSum->setText(QString::number(newValue) + "€");
}

void MainWindow::onMoreButtonClicked_3()
{
    QString currentText = ui->labelDepositSum->text();

    int currentValue = currentText.replace("€", "").toInt();
    int newValue = currentValue + 10;

    ui->labelDepositSum->setText(QString::number(newValue) + "€");
}

void MainWindow::onLessButtonClicked_3()
{
    QString currentText = ui->labelDepositSum->text();

    int currentValue = currentText.replace("€", "").toInt();
    int newValue = currentValue - 10;

    if(newValue < 0)
        newValue = 0;

    ui->labelDepositSum->setText(QString::number(newValue) + "€");
}

void MainWindow::depositClickHandler()
{
    int amount = ui->labelDepositSum->text().replace("€", "").toInt();
    qDebug() << amount;
    transaction->depositFunds(amount, cardNo, accountNo, token);
}

void MainWindow::depositReplySlot(QNetworkReply *reply) {
    QString message = transaction->depositReplySlot(reply);
    QMessageBox msgBox;
    msgBox.setText(message);
    setMessageBoxStyles(msgBox);
    msgBox.exec();
    if (message == "Money deposited successfully!") {
        displayMoneyGif();
    }
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::withdrawClickHandler()
{
    int amount = ui->labelWithdrawSum->text().replace("€", "").toInt();
    qDebug() << amount;
    transaction->withdrawFunds(amount, cardNo, accountNo, token);
}

void MainWindow::withdrawReplySlot(QNetworkReply *reply) {
    QString message = transaction->withdrawReplySlot(reply);
    QMessageBox msgBox;
    msgBox.setText(message);
    setMessageBoxStyles(msgBox);
    msgBox.exec();
    if (message == "Money withdrawn successfully!") {
        displayMoneyGif();
    }
    ui->stackedWidget->setCurrentIndex(2);
}


/* Logout
 */
void MainWindow::onBtnKirjauduUlosClicked()
{
    cardNo = "";
    accountNo = "";
    debitAccount = "";
    creditAccount = "";
    token = "";
    currentPage = 1;
    maxPage = 1;
    ui->stackedWidget->setCurrentIndex(0);
    restartGifs();
}
