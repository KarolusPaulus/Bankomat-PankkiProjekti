const db=require('../database');
const bcrypt=require('bcryptjs');

/* Most of these are procedures because they require multiple steps and it's easier to catch errors
with SQL TRANSACTION.
*/

/* Creates:
    - a new account
    - a new card
    - a card-account connection
    - a user-account connection
*/
function newCardAndAccount(data, callback) {
    bcrypt.hash(data.pincode, 10, function(err, hashedPincode) {
        return db.query("CALL newCardAndAccount(?, ?, ?, ?, ?, ?)", [data.id_card, data.account_type, data.id_user, hashedPincode, data.id_account, data.credit_limit], callback);
    });
}

/* Creates:
    - a new account
    - a user-account connection
*/
function newAccount(data, callback) {
    return db.query("CALL newAccount(?, ?, ?, ?)", [data.id_account, data.account_type, data.id_user, data.credit_limit], callback);
}

/* Creates:
    - a new card
    - a card-account connection
*/
function newCard(data, callback) {
    bcrypt.hash(data.pincode, 10, function(err, hashedPincode) {
        return db.query("CALL newCard(?, ?, ?, ?)", [data.id_card, data.id_account, data.id_user, hashedPincode], callback);
    });
}

/* Creates:
    - a new card
    - a card-account connection
    - a user-account connection
*/
function addAuthorizedCard(data, callback) {
    bcrypt.hash(data.pincode, 10, function(err, hashedPincode) {
        return db.query("CALL addAuthorizedCard(?, ?, ?, ?, ?)", [data.id_card, data.id_user, hashedPincode, data.id_account, data.credit_limit], callback);
    });
}

/* Does:
    - deduct/add the amount to the account
    - create a transaction record 
*/
function bankomatTransaction(data, callback) {
    return db.query("CALL bankomatTransaction(?, ?, ?, ?)", [data.id_account, data.credit_limit, data.amount, data.description], callback);
}

/*  Does:
    - deduct the amount from account one
    - add the amount to account two
    - create a transaction record with account one, amount, current time and a description
*/
function accountToAccountTransaction(data, callback) {
    var success = 0;
    return db.query("CALL accountToAccountTransaction(?, ?, ?, ?, ?)", [data.account_one, data.account_two, data.amount, data.description_one, data.description_two], callback);
}

module.exports = {
    newCardAndAccount,
    newAccount,
    newCard,
    addAuthorizedCard,
    bankomatTransaction,
    accountToAccountTransaction
}