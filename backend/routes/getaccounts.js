const express = require('express');
const router = express.Router();
const tokenCheck = require('./verifytoken');
const cardAccount = require('../models/card_attached_account_model');
const { token } = require('morgan');

router.post('/', function(request, response) {
    if (!request.body['card']) {
        response.send(false);
        return;
    }

    tokenCheck.verify(request, request.body['card'], function(err) {
        if (err) {
            console.log(err);
            response.send(false);
        }
        else {
            cardAccount.accessibleAttachedAccountsWithType(request.body['card'], function(err, result) {
                //console.log(result);
                if (err) {
                    console.log(err);
                    response.send(false);
                }
                else {
                    let answers = [];
                    for (let i = 0; i < result.length; i++) {
                        if (result[i]['state'] == null) {
                            answers.push(result[i]);
                        }   
                    }
                    //console.log(answers);
                    response.send(answers);
                }
            });
        }
    })
});

module.exports = router;