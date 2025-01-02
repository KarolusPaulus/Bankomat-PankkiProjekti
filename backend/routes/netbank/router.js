/* The mecca of all netbank traffic, calls upon functions from different branches of action
*/

const express = require('express');
const router = express.Router();

const token = require('jsonwebtoken');
const path = require('path');

const login = require('./login');
const signup = require('./signup');
const profilelookup  = require('./viewprofile');
const newservices = require('./newservices');
const transaction = require('./transaction');
const userdata = require('./userdata');
const authorization = require('./authorization');

const user = require('../../models/user_model'); // this is here for the /updateuser bandaid, can be removed later
const card = require('../../models/card_model');

const base_path = __dirname; // location of this file, used for path.join() commands

router.use(express.json());
router.use(express.urlencoded({ extended: false }));


        // Purely here for turning all plaintext passwords in the database to encrypted ones so that you can
        // actually login into the netbank with more users ('000' in the database won't match with '000' fed into bcrypt.compare())
        router.post('/updateuser', function(request, response) {
            let data = {
                'fname': request.body['fname'],
                'lname': request.body['lname'],
                'state': null,
                'temp_restriction': null,
                'password': request.body['password']
            };
            user.updateUser(request.body['userid'], data, function(err, result) {
                if (err) {
                    response.send(err);
                }
                else {
                    response.send(result);
                } 
            });
        });
        // remove later
        // Purely here for turning all plaintext passwords in the database to encrypted ones so that you can
        // actually login into the netbank with more users ('000' in the database won't match with '000' fed into bcrypt.compare())
        router.get('/addcard', function(request, response) {
            let data = {
                'id_card': "0600064988",
                'state': null,
                'id_owner': 4,
                'pincode': '000'
            };
            card.addCard(data, function(err, result) {
                if (err) {
                    response.send(err);
                }
                else {
                    response.send(result);
                } 
            });
        });
        // remove later


// Unauthenticated routes
router.get('/', function(request, response) {
    response.render('netbank');
});

router.get('/login', function(request, response) {
    response.render('login');
});

router.post('/login', login.checkUser);

router.get('/logout', login.logOut);

router.get('/signup', function(request, response) {
    response.render('signup');
});

router.post('/signup', signup.addUser);


//Authenticated routes
router.get('/home', function(request, response) {
    authenticateToken(request, response, function(request, response) {
        //console.log("Accessing home page");
        response.render('home', {name: request.cookies['simulbankusername']});
    });
});

router.get('/profile', function(request, response) {
    authenticateToken(request, response, profilelookup.getData);
});

router.get('/newservices', function(request, response) {
    authenticateToken(request, response, newservices.newServicesWindow);
});

router.post('/newservices/openaccount', function(request, response) {
    authenticateToken(request, response, function(request, response) {
        newservices.openAccount(request, response);
    });
});

router.get('/newservices/getcard', function(request, response) {
    authenticateToken(request, response, function(request, response) {
        userdata.getUserData(request, response, function(err, user, cards, accounts) {
            if (err == true) {
                response.render('getcard', {error: "Something went wrong with getting your data"});
            }
            else {
                response.render('getcard', {accounts: accounts});
            }
        });
    });
});

router.post('/newservices/getcard', function(request, response) {
    authenticateToken(request, response, function(request, response) {
        newservices.openCard(request, response);
    });    
});

router.post('/newservices/secondaccount', function(request, response) {
    authenticateToken(request, response, function(request, response) {
        //console.log(request.body['openAccount']);
        let data = request.body['openAccount'].split('/');
        //console.log(data);
        if (data[1] == '0' || data[1] == '1') {
            console.log("Looking to attach new second account to card");
            newservices.attachNewSecondAccount(request, response);
        }
        else {
            console.log("Looking to attach existing second account to card");
            newservices.attachExistingSecondAccount(request, response);
        }
    });
});

router.get('/authorization', function(request, response) {
    authenticateToken(request, response, function(request, response) {
        authorization.authorizationWindow(request, response);
    });
});

router.post('/authorization/authorizeuser', function(request, response) {
    authenticateToken(request, response, function(request, response) {
        authorization.authorizeUser(request, response);
    });
});

router.post('/authorization/removeauthorization', function(request, response) {
    authenticateToken(request, response, function(request, response) {
        authorization.removeAuthorization(request, response);
    });
});

router.get('/transaction', function(request, response) {
    authenticateToken(request, response, function(request, response) {
        transaction.findTransactionCapableAccounts(request, response);
    });
});

router.post('/transaction', transaction.accountToAccountTransaction);


function authenticateToken(request, response, next) {
    token.verify(request.cookies['simulbanktoken'], process.env.Web_Token, function(err, user) {
        //console.log("Verifying token");
        if (!err && user['userid'] == request.cookies['simulbankuserid']) {
            // renews cookies
            response.cookie('simulbankuserid', request.cookies['simulbankuserid'], { expires: new Date(Date.now() + 300000), httpOnly : true, secure : true});
            response.cookie('simulbankusername', request.cookies['simulbankusername'], { expires: new Date(Date.now() + 300000), httpOnly : true, secure : true});
            response.cookie('simulbanktoken', request.cookies['simulbanktoken'], { expires: new Date(Date.now() + 300000), httpOnly : true, secure : true});
            next(request, response);
        } 
        else {
            //console.log("failed verification, user id " + request.cookies['simulbankuserid']);
            response.render('unauthorized');
        }
    });
}

module.exports = router;