var express = require('express');
var router = express.Router();
var transaction = require('../models/transaction_model');
const tokenCheck = require('./verifytoken');
const { token } = require('morgan');

router.post('/', function(request, response) {
    if (!request.body.idaccount || !request.body.card) {
        response.send(false);
        return;
    }

    tokenCheck.verify(request, request.body.card, function(err) {
        if (err) {
            console.log(err);
            response.send(false);
        }
        else {
    transaction.getTransactionHistory(request.body.idaccount, function(err, result) {
        if (err) {
            response.send(false);
            console.log(err);
        } else {
            //console.log(result);
            response.send(result);
        }
    });
}
});
});

module.exports = router;