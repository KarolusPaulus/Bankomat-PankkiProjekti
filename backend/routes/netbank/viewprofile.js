const userdata = require('./userdata');

function getData(request, response) {
    userdata.getUserData(request, response, function(err, cards, debitAccounts, creditAccounts, authorizedAccounts) {
        if (err) {
            response.render('profile', {error: "Something went wrong with getting your data"});
        }
        else {
            let accounts = debitAccounts.concat(creditAccounts);
            //console.log(accounts);
            response.render('profile', {name: request.cookies['simulbankusername'], id: request.cookies['simulbankuserid'], accounts: accounts, authorizedAccounts: authorizedAccounts, cards: cards});
        }
    });
}

module.exports = {
    getData
};