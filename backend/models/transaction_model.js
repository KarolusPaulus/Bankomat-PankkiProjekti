const db=require('../database');
const bcrypt=require('bcryptjs');

const transaction={
    getTransactionHistory(id_account, callback) {
        return db.query("SELECT id_transaction, amount, time, description FROM transaction WHERE id_account = ? ORDER BY time DESC", [id_account], callback);
    }, //transaction history

    addTransaction(acc, am, des, callback) {
        return db.query("INSERT INTO transaction (id_account, amount, time, description) VALUES (?, ?, NOW(), ?)", [acc, am, des], callback);
    }, //add transaction to history

    depositToAccount(acc, am, callback) {
        return db.query("UPDATE account SET balance = balance + ? WHERE id_account = ?", [am, acc], callback);
    }, //update the account balance by adding the deposited amount

    withdrawFromAccount(acc, am, callback) {  
        return db.query("UPDATE account SET balance = balance - ? WHERE id_account = ?", [am, acc], callback);
    }, //update the account balance by subtracting the withdrawn amount
}

module.exports=transaction;