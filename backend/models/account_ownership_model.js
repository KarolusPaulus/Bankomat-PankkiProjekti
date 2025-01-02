const db=require('../database');
const bcrypt=require('bcryptjs');

const accountOwnership={
    associateUserWithAccount(id_user, rights_level, id_account, callback) {
        return db.query("INSERT INTO account_ownership (id_user, rights_level, id_account) VALUES (?, ?, ?)", [id_user, rights_level, id_account], callback);
    }, //associate user with an account

    getUserAssociatedAccount(id_user, callback) {
        return db.query("SELECT id_account FROM account_ownership WHERE id_user = ?", [id_user], callback);
    }, //get the associated account of user

    removeAssociationUser(id_user, id_account, callback) {
        //console.log(id_user + " " + id_account);
        db.query("UPDATE card INNER JOIN card_attached_account ON card.id_card = card_attached_account.id_card SET state = 2 WHERE id_account = ? AND owner = ?", [id_account, id_user], function(err, result) {
            if (err) console.log(err);
        });
        return db.query("DELETE FROM account_ownership WHERE id_user = ? AND id_account = ?", [id_user, id_account], callback);
    }, //remove association between user and an account

    getAuthorizedAccountsByUser(id_user, callback) {
        return db.query("SELECT account.id_account, type, state, balance, owner, credit_limit, account_ownership.id_user AS attached_user, rights_level FROM account INNER JOIN account_ownership ON account_ownership.id_account = account.id_account WHERE owner = ? AND account_ownership.id_user != ?", [id_user, id_user], callback);
    }
}

module.exports=accountOwnership;