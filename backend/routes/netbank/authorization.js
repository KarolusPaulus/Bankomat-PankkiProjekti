const accountOwnership = require('../../models/account_ownership_model');
const account = require('../../models/account_model');
const user = require('../../models/user_model');

function authorizationWindow(request, response) {
    accountOwnership.getAuthorizedAccountsByUser(request.cookies['simulbankuserid'], function (err, result) {
        let authorizations = result;
        if (err) {
            console.log(err);
            response.render('authorization', {error: "Something went wrong with getting your data"});
        }
        else {
            account.allAccountsOwnedByUser(request.cookies['simulbankuserid'], function (err, result) {
                if (err) {
                    console.log(err);
                    response.render('authorization', {error: "Something went wrong with getting your data"});
                }
                else {
                    //console.log(result);
                    response.render('authorization', {authorizations: authorizations, accounts: result});
                }
            })
        }
    })
    
}

function authorizeUser(request, response) {
    //console.log(request.body);
    user.getUser(request.body['user'], function (err, result) {
        if (err) {
            console.log(err);
            response.render('authorization', {error: "Something went wrong with the database"});
        }
        else if (result.length == 0) {
            console.log("Tried to authorize nonexistent user");
            response.render('authorization', {error: "No user with ID " + request.body['user'] + " found."});
        }
        else {
            accountOwnership.getUserAssociatedAccount(request.body['user'], function(err, result) {
                if (err) {
                    console.log(err);
                    response.render('authorization', {error: "Something went wrong with the database"});
                }
                else {
                    for (let i = 0; i < result.length; i++) {
                        if (result[i].id_account == request.body['account']) {
                            console.log("Tried to authorize the same account twice for the same user");
                            response.render('authorization', {error: "You've already authorized this user"});
                        }
                    }
                    accountOwnership.associateUserWithAccount(request.body['user'] / 1, 2, request.body['account'], function(err, result) {
                        if (err) {
                            console.log(err);
                            response.render('authorization', {error: "Something went wrong with the database"});
                        }
                        else {
                            response.render('authorization', {success: "Authorization completed succesfully!"});
                        }
                    });
                }
            });
        }
    });
}

function removeAuthorization(request, response) {
    let data = request.body['accountuserpair'].split('/');
    accountOwnership.removeAssociationUser(data[0], data[1], function(err, result) {
        if (err) {
            console.log(err);
            response.render('authorization', {error: "Something went wrong with the database"});
        }
        else {
            response.render('authorization', {success: "Authorization removed successfully!"});
        }
    })
}

module.exports = {
    authorizationWindow,
    authorizeUser,
    removeAuthorization
}