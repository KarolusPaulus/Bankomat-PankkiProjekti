const db=require('../database');
const bcrypt=require('bcryptjs');

const cardAttachedAccount={
    associateCardWithAccount(card, acc, callback) {
        return db.query("INSERT INTO card_attached_account (id_card, id_account) VALUES (?, ?)", [card, acc], callback);
    }, //associate a card with an account
    
    getCardAssociatedAccount(card, callback) {
        return db.query("SELECT id_account FROM card_attached_account WHERE id_card = ?", [card], callback);
    }, //get the associated account of a card

    removeAssociationCard(card, callback) {
        return db.query("DELETE FROM card_attached_account WHERE id_card = ?", [card], callback);
    }, //remove association between a card and an account

    accessibleAttachedAccountsWithType(card, callback) {
        return db.query("SELECT account.id_account, type, state FROM account INNER JOIN card_attached_account ON card_attached_account.id_account = account.id_account WHERE id_card = ?", [card], callback);
    } // "Here's my card, what accounts could I use for my financial needs?"
}

module.exports=cardAttachedAccount;