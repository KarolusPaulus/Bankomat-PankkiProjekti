const user = require('../../models/user_model');
const token = require('jsonwebtoken');

// Adds given user data to the database, 
function addUser(request, response) {
    // Trims the request down for the addUser() request
    var userObject = {
        'fname' : request.body['fname'],
        'lname' : request.body['lname'],
        'state' : null,
        'password' : request.body['password']
    };
    let url = request.headers['origin'];

    user.addUser(userObject, function(err, result) {
        if (err) {
            response.render('signup', {error: "Database error"});
        }
        else {
            console.log("New user added to database, id " + result['insertId']);
            response.cookie('simulbankuserid', result['insertId'], { expires: new Date(Date.now() + 300000), httpOnly : true, secure : true});
            response.cookie('simulbankusername', userObject['fname'] + " " + userObject['lname'], { expires: new Date(Date.now() + 300000), httpOnly : true, secure : true});
            let token = generateToken({userid: result['insertId']});
            response.cookie('simulbanktoken', token, { expires: new Date(Date.now() + 300000), httpOnly : true, secure : true});
            response.redirect(url + "/netbank/home");
        }
    });
}

function generateToken(id) {
    return token.sign(id, process.env.Web_Token);
}

module.exports = {
    addUser
};