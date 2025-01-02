const token = require('jsonwebtoken');

function verify(request, id_card, next) {
    let header = request.headers['authorization']
    let tkn = header.split(' ')[1];
  
    if (tkn == null) {
        console.log("No token sent");
        next("No token sent");
        return;
    }

    token.verify(JSON.parse(tkn), process.env.Web_Token, function(err, card) {
        if (err) {
            console.log(err);
            next("Invalid token");
        }
        else {
            //console.log(id_card);
            //console.log(id_card + " " + card.card);
            if (id_card != card.card) {
                console.log("Card no doesn't match token");
                next("Invalid token");
            }
            else {
                next(null);
            }
        }
    });
}

module.exports = {
    verify
}