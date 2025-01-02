const db=require('../database');
const bcrypt=require('bcryptjs');

const account={
    getAccount(id_account, callback) {
    return db.query("SELECT * FROM account WHERE id_account = ?", [id_account], callback);
    }, //for admins

    selectAccountType(id_account, callback) {
        return db.query("SELECT type FROM account WHERE id_account = ?", [id_account], callback);
    }, //select account at ATM

    getBalance(id_account, callback) {
        return db.query("SELECT balance FROM account WHERE id_account = ?", [id_account], callback);
    }, //shows the balance of selected account

    getLimit(id_account, callback) {
        return db.query("SELECT credit_limit FROM account WHERE id_account = ?", [id_account], callback);
    }, //shows the credit limit of selected account

    deleteAccount(id_account, callback) {
        return db.query("DELETE FROM account WHERE id_account = ?", [id_account], callback);
    }, //delete account

    addAccount(acc, type, state, balance, owner, credit, callback) {
        return db.query("INSERT INTO account (id_account, type, state, balance, owner, credit_limit) VALUES (?, ?, ?, ?, ?, ?)", [acc, type, state, balance, owner, credit], callback);
    }, //add new account(

    allAccountsByUser(id_account, callback){
        return db.query("SELECT id_account, balance, type, state, credit_limit FROM account WHERE owner=?", [id_account], callback);
    },

    allAccountsOwnedByUser(id_user, callback) {
        return db.query("SELECT account.id_account, type, state, balance, owner, credit_limit, account_ownership.id_user AS attached_user, rights_level FROM account INNER JOIN account_ownership ON account_ownership.id_account = account.id_account WHERE owner = ? AND account_ownership.id_user = ?", [id_user, id_user], callback);
    },

    allAccountsAccessibleByUser(id_user, callback) {
        return db.query("SELECT account.id_account, type, state, balance, credit_limit, owner, rights_level FROM account INNER JOIN account_ownership ON account.id_account = account_ownership.id_account WHERE account_ownership.id_user = ?", [id_user], callback);
    }
}

module.exports=account;