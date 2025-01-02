const express = require('express');
const router = express.Router();
const card = require('../models/card_model');
const failedLogin = require('./failed_login');

/* {{url}}/bankomat/verifycard, kutsutaan kun halutaan tarkistaa onko maattiin syötetty kortti
pankin listoilla & käytettävissä, mukaan laitetaan kortin numero
*/
router.post('/', function(request, response) {
    // Before proceeding, check that a card no was sent
    if (!request.body.card) {
        response.send(false);
        return;
    } 

    verifyCard(request, response);
});

// Takes a card no in the request (name 'cardno'), performs a check in the database to verify whether the 
// card is usable
function verifyCard(request, response) {
    // Tarkistetaan, onko kortti olemassa tietokannassa
    card.getCard(request.body.card, function(err, result) {
        if (err) {
            console.log(err);
            response.send(false);
            return;
        }

        let res = JSON.parse(JSON.stringify(result));

        failedLogin.isCardBlocked(request.body['card'], function(err, blockStatus) {
            // cardStatus = "is the card accepted", blockStatus = "is the card not accepted",
            // thus the flip
            var cardStatus = !blockStatus;
            var cause = ": ";
            
            // If database check leads to an error
            if (err) {
                console.log(err);
                response.send(false);
                return;
            }
            // If nothing is returned from the database
            else if ((res[0] == null) || (res[0] === undefined)) {
                cause = cause + "not in database";
                cardStatus = false;
            }
            // If the card's state or temp restriction brings up something (null = nothing of note)
            else if (res[0]['state'] != null) {
                cause = cause + "card restricted";
                cardStatus = false;
            }
            else if (res[0]['temp_restriction'] != null && res[0]['curr_time'] < res[0]['temp_restriction']) {
                cause = cause + "card temporarily restricted";
                cardStatus = false;
            }
            else {
                if (res[0]['temp_restriction'] != null) {
                    card.removeTempRestriction(request.body['card'], function(err, result) {
                        if (err) {
                            console.log(err);
                        }
                    });
                }
                // Compare the current date to the card's expiration date
                // Code below transforms card & current date to 'YYYY-MM-DD' formats
                let cardDate = new Date(res[0]['expiration']).toISOString().split('T')[0];
                let thisDate = new Date(Date.now()).toISOString().split('T')[0];
                if (cardDate < thisDate) {
                    card.cardExpired(res[0]['id_card'], function (err, result) {
                        if (err) {
                            console.log(err);
                        }
                    });
                    cause = cause + "card expired";
                    cardStatus = false;
                }
            }
            // True-false-response sent back, console gets message with id_card, success/failure & reason for failure if applicable
            response.send(cardStatus);
            console.log("Card " + request.body['card'] + " verified, result " + cardStatus + (cardStatus ? "" : cause));
        })
    });
}

module.exports = router;