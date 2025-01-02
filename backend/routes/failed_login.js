const db = require('../database.js');
const card = require('../models/card_model');

function addFailedLoginAttempt(cardNumber, callback) {
    return db.query("INSERT INTO failed_login (id_card, login_time) VALUES (?, NOW())", [cardNumber], callback);
}

function getFailedLoginAttempts(cardNumber, callback) {
    return db.query("SELECT * FROM failed_login WHERE id_card = ?", [cardNumber], callback);
}

function deleteAttempt(callback){
    return db.query("DELETE FROM failed_login WHERE login_time < DATE_SUB(NOW(), INTERVAL 30 MINUTE)", callback);
}

function isCardBlocked(cardNumber, callback) {
    deleteAttempt(function(err, results){
        if (err) {
            console.error("Error deleting attempts: ", err);
            callback(err, false);
        }
        else {
            getFailedLoginAttempts(cardNumber, function(err, results) {
                if (err) {
                    console.error("Error checking if card is blocked: ", err);
                    callback(err, false);
                } else {
                    const failedAttempts = results.length;
                    if(failedAttempts>=3) {
                        card.addTempRestriction(cardNumber, function (err, result) {
                            if (err) {
                                console.log(err);
                            }
                        })
                    }
                    callback(false, (failedAttempts < 3) ? false : true);
                }
            })
        }
    })
}

module.exports = { addFailedLoginAttempt, isCardBlocked };