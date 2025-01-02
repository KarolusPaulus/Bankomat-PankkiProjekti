const express = require('express');
const path = require('path');
const cookieParser = require('cookie-parser');
const logger = require('morgan');
const pug = require('pug');

// netbank
const netbankRouter = require('./routes/netbank/router');

//admin
const adminRouter = require('./routes/admin/router');
const swagger = require('swagger-ui-express');
const swaggerDoc = require('./swagger.json');

// unverified bankomat
const loginRouter = require('./routes/login');
const cardVerificationRouter = require('./routes/verifycard');

//verified bankomat
const accountRouter = require('./routes/getaccounts');
const viewCardRouter = require('./routes/viewcard');
const transactionRouter = require('./routes/transactiontwo');
const viewTransactionRouter = require('./routes/viewtransactions');

const port = process.env.PORT || 3000;
const app = express();

app.use(logger('dev'));
app.use(express.json());
app.use(express.urlencoded({ extended: false }));
app.use(cookieParser());
app.use(express.static(path.join(__dirname, 'public')));
app.use('/swagger', swagger.serve, swagger.setup(swaggerDoc));
app.set('view engine', 'pug');

app.listen(port, function(request, response) {
    console.log("Server online");
});

app.use('/netbank', netbankRouter);
app.use('/admin', adminRouter);
app.use('/bankomat/verifycard', cardVerificationRouter);
app.use('/bankomat/login', loginRouter);

// Token-requiring routes
app.use('/bankomat/getaccounts', accountRouter);
app.use('/bankomat/viewprofile', viewCardRouter);
app.use('/bankomat/transaction', transactionRouter);
app.use('/bankomat/viewtransactions', viewTransactionRouter);

module.exports = app;