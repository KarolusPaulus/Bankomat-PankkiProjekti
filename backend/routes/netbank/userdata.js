const user = require('../../models/user_model');
const account = require('../../models/account_model');
const card = require('../../models/card_model');

function getUserData(request, response, next) {
    let userid = request.cookies['simulbankuserid'];

    if (!userid) {
        next(true, [], [], [], []);
        return;
    }

    account.allAccountsAccessibleByUser(userid, function(err,result){
        if(err){
            next(true, [], [], [], []);
            return;
        }

        let creditAccounts = [];
        let debitAccounts = [];
        let authorizedAccounts = [];
        let i = 0;
        while (result[i] != null && result[i] != undefined) {
            if (result[i]['state'] == null) {
                if (result[i]['rights_level'] != 1) {
                    authorizedAccounts.push(result[i])
                }
                else if (result[i]['type'] == 0) {
                    creditAccounts.push(result[i]);
                }
                else {
                    debitAccounts.push(result[i]);
                }
            }
            i++;
        }
        //console.log(creditAccounts);
        //console.log(debitAccounts);

        card.getCardsByOwner(userid, function(err, result) {
            if (err) {
                next(true, [], [], [], []);
                return;
            }
            //console.log(result);
            let cards = [];
            let i = 0;
            while (result[i] != null && result[i] != undefined) {
                // Converts the expiration to a yyyy-mm-dd from YYYY-MM-DDTHH:mm:ss.sssZ (or something of the like)
                result[i]['expiration'] = new Date(result[i]['expiration']).toISOString().split('T')[0];
                cards.push(result[i]);
                i++;
            }

            next(false, cards, debitAccounts, creditAccounts, authorizedAccounts);
        });
    });
}

function getCardApprovedAccounts(request, response, next) {
    let userid = request.cookies['simulbankuserid'];

    if (!userid) {
        console.log("No user id for fetching user data given");
        next(true, [], [], [], []);
        return;
    }

    account.allAccountsAccessibleByUser(userid, function(err, accounts){
        if(err){
            console.log(err);
            next(true, [], [], [], []);
            return;
        }

        //console.log(creditAccounts);
        //console.log(debitAccounts);

        card.getCardsByOwner(userid, function(err, result) {
            if (err) {
                console.log(err);
                next(true, [], [], [], []);
                return;
            }

            let creditAccounts = [];
            let debitAccounts = [];
            let authorizedAccounts = [];

            // Prunes out all accounts that already have a card attached (by the user)
            for (let j = 0; j < accounts.length; j++) {
                let accountCleared = true;
                for (let k = 0; k < result.length; k++) {
                    if (accounts[j]['id_account'] == result[k]['id_account']) {
                        //console.log(accounts[j]['id_account']);
                        //console.log(cards[k]['id_account']);
                        accountCleared = false;
                    }
                }
                // 'State' for things like frozen accounts
                if (accounts[j]['state'] == null && accountCleared == true) {
                    // rights_level 1 for personally owned accounts
                    if (accounts[j]['rights_level'] != 1) {
                        //console.log(accounts[j]);
                        authorizedAccounts.push(accounts[j])
                    }
                    // Type 0 for credit
                    else if (accounts[j]['type'] == 0) {
                        creditAccounts.push(accounts[j]);
                    }
                    else {
                        debitAccounts.push(accounts[j]);
                    }
                }
            }

            //console.log(result);
            let debitAvailableCards = [];
            let creditAvailableCards = [];
            let i = 0;
            while (result[i] != null && result[i] != undefined) {
                // Converts the expiration to a yyyy-mm-dd from YYYY-MM-DDTHH:mm:ss.sssZ (or something of the like)
                result[i]['expiration'] = new Date(result[i]['expiration']).toISOString().split('T')[0];
                if (result[i].type == 0) {
                    debitAvailableCards.push(result[i]);
                }
                else {
                    creditAvailableCards.push(result[i]);
                }
                i++;
            }

            /* Removes cards with two types of accounts attached from the lists "It's in list A and list B, dump it from both"
            */
            let cleanPass = false;
            loop: while (cleanPass == false) {
                //console.log(1);
                for (let l = 0; l < creditAvailableCards.length; l++) {
                    //console.log(2);
                    for (let m = 0; m < debitAvailableCards.length; m++) {
                        //console.log(creditAvailableCards[l]['id_card']);
                        //console.log(debitAvailableCards[m]['id_card']);
                        if (creditAvailableCards[l]['id_card'] == debitAvailableCards[m]['id_card']) {
                            //console.log("Splice");
                            creditAvailableCards.splice(l, 1);
                            debitAvailableCards.splice(m, 1);
                            cleanPass = false;
                            continue loop;
                        }
                    }
                    
                }
                cleanPass = true;
            }
            //console.log(creditAvailableCards);
            //console.log(debitAvailableCards);
            
            next(false, debitAvailableCards, creditAvailableCards, debitAccounts, creditAccounts, authorizedAccounts);
        });
    });
}

module.exports = {
    getUserData,
    getCardApprovedAccounts
}