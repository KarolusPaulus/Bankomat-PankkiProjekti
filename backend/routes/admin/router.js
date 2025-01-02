/* Redirects admin requests to various branches of the admin folder

*/

const express = require('express');
const router = express.Router();
const usercontrols = require('./usercontrols');

// 
router.post('/adduser', function(request, response) {
    usercontrols.addUser(request, response);
});

router.delete('/deleteuser', function(request, response) {
    usercontrols.deleteUser(request, response);
});

router.get('/listallusers', function(request, response) {
    usercontrols.listAllUsers(request, response);
});

router.get('/getuserbyuserid', function(request, response) {
    usercontrols.getUser(request, response);
});

router.put('/updateuser', function(request, response) {
    usercontrols.updateUser(request, response);
});

router.put('/updatepincode', function(request, response) {
    usercontrols.updatePinCode(request, response);
});

router.put('/removeTempRestriction', function(request, response) {
    usercontrols.removeRestriction(request, response);
});
module.exports = router;