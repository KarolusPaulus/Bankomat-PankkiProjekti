const db=require('../database');
const bcrypt=require('bcryptjs');

const user={
    getUser(id_user, callback){
        return db.query("SELECT u.fname, u.lname, u.state, c.id_card FROM user u INNER JOIN card c ON u.id_user = c.owner WHERE c.owner = ?", [id_user], callback);
    }, // for admins

    listAllUsers(callback){
        return db.query("SELECT id_user, fname, lname FROM user", callback);
    },

    addUser(newUser, callback){
        bcrypt.hash(newUser.password, 10, function(err, hashedPassword){
            return db.query("INSERT INTO user(fname, lname, state, password) VALUES(?,?,null,?)", [newUser.fname, newUser.lname, hashedPassword], callback);
        });
    },

    login(id_user, callback) {
        return db.query("SELECT password FROM user WHERE id_user = ?", [id_user], callback);
    }, //verify correct pincode
    
    getUserByName(fname, lname, callback) {
        return db.query("SELECT id_user FROM user WHERE fname = ? AND lname = ?", [fname, lname], callback);
    },

    updateUser(id, updateData, callback){
        bcrypt.hash(updateData.password, 10, function(err,hashedPassword){
            return db.query("UPDATE user SET fname=?, lname=?, state=?, temp_restriction=?, password=? WHERE id_user=?",
            [updateData.fname, updateData.lname, updateData.state, updateData.temp_restriction, hashedPassword, id], callback);
        });
    }, //for admins


    deleteUser(id_user, callback){
        return db.query("DELETE FROM user WHERE id_user=?", [id_user], callback); //for admins
    },

    getAccountAndCardData(id, callback) {
        return db.query("SELECT id_account AS 'data', 'accounts' AS 'description' FROM account_ownership WHERE id_user = ? UNION SELECT id_card, 'cards' FROM card WHERE owner = ?", [id, id], callback);
    },

    
}
    
module.exports=user;

