const express = require('express');
const router = express.Router();

const tokenCheck = require('./verifytoken');
const procedure = require('../models/procedure_model');
const account = require('../models/account_model');

router.post('/withdraw', function (request, response) {
    console.log("Nostoyritys, summa " + request.body['amount'] + ", kortti " + request.body['card'] + ", tili " + request.body['account']);
    tokenCheck.verify(request, request.body['card'], function(err) {
        if(err) {
            console.log(err);
            response.send(false);
        }
        else {
            console.log("Token check passed succesfully");
            
            account.getAccount(request.body['account'], function (err, result) {
                if (err) {
                    console.log(err);
                    response.send(false);
                }
                else {
                    let data = {
                        "id_account": request.body['account'],
                        "credit_limit": result[0]['credit_limit'],
                        "amount": request.body['amount'] * -1.00,
                        "description": "Bankomat withdrawal"
                    }
                    //console.log(data);
                    procedure.bankomatTransaction(data, function(err, result) {
                        if (err) {
                            console.log(err);
                            response.send(false);
                        }
                        else {
                            //console.log(result);
                            if (result['affectedRows'] == 0) {
                                console.log("Withdrawal attempted from account " + request.body['account'] + ", balance too low.")
                                response.send(false);
                            }
                            else {
                                console.log("Withdrawal from account " + request.body['account'] + ", amount" + request.body['amount'])
                                response.send(true);
                            }
                        }
                    });
                }
            });
        }
    });
});

module.exports = router;