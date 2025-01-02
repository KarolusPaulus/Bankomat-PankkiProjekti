var express = require('express');
var router = express.Router();
var user=require('../models/user_model');
var accountAttached=require('../models/card_attached_account_model');
var card=require('../models/card_model');
var account=require('../models/account_model');
var transaction=require('../models/transaction_model');
const tokenCheck = require('./verifytoken');
const { token } = require('morgan');

router.post('/', function(request, response) {
    let answer = [];
    console.log("Bankomat profile request, card " + request.body['card'])

    tokenCheck.verify(request, request.body['card'], function(err) {
        if (err) {
            console.log(err);
            response.send(false);
        }
        else {
            card.getCard(request.body['card'], function(err,result){
                if(err){
                    response.send(err);
                    return;
                }
                else if ((result[0] == null) || (result[0] === undefined)) {
                    response.send(false);
                    return;
                }
                let cardData = result[0];

                user.getUser(cardData['owner'], function(err,result){
                    if(err){
                        response.send(err);
                        return;
                    }
                    else if ((result[0] == null) || (result[0] === undefined)) {
                        response.send(false);
                        return;
                    }
                    let cardOwnerData = result[0];

                    account.getAccount(request.body['account'], function(err,result){
                        if(err){
                            response.send(err);
                            return;
                        }
                        else if ((result[0] == null) || (result[0] === undefined)) {
                            response.send(false);
                            return;
                        }
                        let accountData = result[0];

                        user.getUser(accountData['owner'], function(err,result){
                            if(err){
                                response.send(err);
                                return;
                            }
                            else if ((result[0] == null) || (result[0] === undefined)) {
                                response.send(false);
                                return;
                            }
                            let userData = result[0];

                            transaction.getTransactionHistory(accountData['id_account'], function(err,result){
                                if(err){
                                    response.send(false);
                                    return;
                                }

                                let answer =[ {
                                    "id_card": cardData['id_card'],
                                    "id_card_owner": cardData['owner'],
                                    "card_owner": cardOwnerData['fname'] + " " + cardOwnerData['lname'],
                                    "id_account": accountData['id_account'],
                                    "id_account_owner": accountData['owner'],
                                    "account_owner": userData['fname'] + " " + userData['lname'],
                                    "balance": accountData['balance'],
                                    "credit_limit": accountData['credit_limit']
                                },
                                    result[0], 
                                    result[1], 
                                    result[2], 
                                    result[3], 
                                    result[4]
                                ];
                                //console.log(answer);
                                response.send(answer);
                            });
                        });
                    });
                });
            });
        }
    });
});

module.exports=router;
