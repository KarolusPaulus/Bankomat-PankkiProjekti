const db=require('../database');
const bcrypt=require('bcryptjs');

const admin={
    adminLogin(id_admin, callback){
        return db.query("SELECT admin_password FROM admin WHERE id_admin = ?", [id_admin], callback);
    }, //verify correct password for admin

    adminVerify(id_admin,  callback){
        return db.query("SELECT temp_restriction FROM admin WHERE id_admin = ?", [id_admin], callback);
    }, //admin login verify

    checkRights(id_admin, callback){
        return db.query("SELECT admin_rights FROM admin WHERE id_admin = ?", [id_admin], callback);
    }, // check admins rights
}

module.exports=admin;