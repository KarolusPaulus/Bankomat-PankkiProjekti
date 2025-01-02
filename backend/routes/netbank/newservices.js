const account = require('../../models/account_model');
const card = require('../../models/card_model');
const userdata = require('./userdata');
const procedure = require('../../models/procedure_model');
const cardAssociatedAccount = require('../../models/card_attached_account_model');

function newServicesWindow(request, response) {
    userdata.getCardApprovedAccounts(request, response, function(err, debitAvailableCards, creditAvailableCards, debitAccounts, creditAccounts, authorizedAccounts) {
        if (err) {
            response.render('newservices', {error: "Something went wrong with getting your data"});
        }
        else {
            response.render('newservices', {debitAccounts: debitAccounts, creditAccounts: creditAccounts, authorizedAccounts: authorizedAccounts, creditAvailableCards: creditAvailableCards, debitAvailableCards: debitAvailableCards});
        }
    });
}

function openCard(request, response) {
    findFreeCardID(request, response, 0x0, function(cardID) {
        let pin = randomizePin();
        let data = {
            'id_user': request.cookies['simulbankuserid'],
            'id_card': cardID,
            'id_account': request.body['account'],
            'pincode': pin
        };

        procedure.newCard(data, function(err, result) {
            if (err) {
                response.render('getcard', {error: "Something went wrong"});
                console.log(err);
            }
            else {
                response.render('getcard', {success: "Your card was succesfully created: ID " + cardID + ", PIN " + pin + "." });
            }
        });
    });
}

/* 
*/
function openAccount(request, response) {
    /* Possible requests:
        0 - just credit account, 2 - credit card with account
        1 - just debit account, 3 - debit card with account
    */
    let requestType = request.body['openAccount'];
    console.log(requestType);
    findFreeAccountID(request, response, 0, function(accountID) {
        if (requestType > 1) {
            findFreeCardID(request, response, 0x0, function(cardID) {
                let pin = randomizePin();
                let data = {
                    'id_card': cardID,
                    'account_type': requestType - 2,
                    'id_user': request.cookies['simulbankuserid'],
                    'pincode': pin,
                    'id_account': accountID,
                    'credit_limit': (requestType - 2 == 0) ? 3000.00 : 0.00
                };
                procedure.newCardAndAccount(data, function(err, result) {
                    if (err) {
                        response.render('newservices', {error: "Something went wrong with the database, try again later."});
                        console.log(err);
                    }
                    else {
                        //console.log(data);
                        response.render('newservices', {success: "Account opened succesfully.", success2: "Your account ID is " + accountID + ", card ID is " + cardID + ", and PIN number is " + pin + "."});
                    }
                });
            });
        }
        else {
            let data = {
                'id_account': accountID,
                'account_type': requestType,
                'id_user': request.cookies['simulbankuserid'],
                'credit_limit': (requestType == 0) ? 3000.00 : 0.00
            };
            procedure.newAccount(data, function(err, result) {    
                if (err) {
                    response.render('newservices', {error: "Something went wrong with the database"});
                    console.log(err);
                }
                else {
                    //console.log(data);
                    response.render('newservices', {success: "Account opened succesfully, ID " + accountID + "!"});
                }
            });
        }
    });  
}

function attachExistingSecondAccount(request, response) {
    let data = request.body['openAccount'].split('/');
    cardAssociatedAccount.associateCardWithAccount(data[0], data[1], function (err, result) {
        if (err) {
            console.log(err);
            response.render('newservices', {error: "Something went wrong with the database"});
        }
        else {
            response.render('newservices', {success: "A second account was succesfully attached to your card!"});
        }
    });
}

function attachNewSecondAccount(request, response) {
    let data = request.body['openAccount'].split('/');
    findFreeAccountID(request, response, 0, function(accountID) {    
        let account = {
            'id_account': accountID,
            'account_type': data[1],
            'id_user': request.cookies['simulbankuserid'],
            'credit_limit': (data[1] == 0) ? 3000.00 : 0.00
        };
        procedure.newAccount(account, function(err, result) {    
            if (err) {
                response.render('newservices', {error: "Something went wrong with the database"});
                console.log(err);
            }
            else {
                cardAssociatedAccount.associateCardWithAccount(data[0], accountID, function (err, result) {
                    if (err) {
                        console.log(err);
                        response.render('newservices', {error: "Something went wrong with the database"});
                    }
                    else {
                        response.render('newservices', {success: "A second account was succesfully attached to your card!"});
                    }
                });
            }
        });
    });  
}


/* Creates a 4-digit pin
*/
function randomizePin() {
    let pin = "";
    for (let i = 0; i < 4; i++) {
        pin = pin + Math.floor((Math.random() * 10));
    }
    return pin;
}

/* Loops through
*/
function findFreeAccountID(request, response, number, next) {
    account.getAccount(number, function(err, result) {
        if (err) {
            response.render('openaccount', {error: "Something went wrong with the database"});
            return;
        }
        // 0 rows returned means that there's no existing account with that ID
        else if (result.length == 0){
            let idString;
            if (number < 10) {
                idString = "0000" + number;
            }
            else if (number < 100) {
                idString = "000" + number;
            }
            else if (number < 1000) {
                idString = "00" + number;
            }
            else if (number < 10000) {
                idString = "0" + number;
            }
            else idString = "" + number;
            next(idString);
        }
        else {
            accountIDLoop(request, response, number, next);
        }
    });
}

function accountIDLoop(request, response, number, next) {
    let id = number + 1;
    findFreeAccountID(request, response, id, next);
}

function findFreeCardID(request, response, number, next) {
    let idString;
    //console.log(number);
    if (number < Math.pow(16, 1)) {
        idString = "060000000" + number.toString(16);
    }
    else if (number < Math.pow(16, 2)) {
        idString = "06000000" + number.toString(16);
    }
    else if (number < Math.pow(16, 3)) {
        idString = "0600000" + number.toString(16);
    }
    else if (number < Math.pow(16, 4)) {
        idString = "060000" + number.toString(16);
    }
    else idString = "06000" + number.toString(16);

    card.getCard(idString, function(err, result) {
        if (err) {
            next(false);
            return;
        }
        // 0 rows returned means that there's no existing account with that ID
        else if (result.length == 0){
            next(idString.toUpperCase());
        }
        else {
            cardIDLoop(request, response, number, next);
        }
    });
}

function cardIDLoop(request, response, number, next) {
    let id = number + 0x1;
    findFreeCardID(request, response, id, next);
}

module.exports = {
    newServicesWindow,
    openCard,
    openAccount,
    attachExistingSecondAccount,
    attachNewSecondAccount
}