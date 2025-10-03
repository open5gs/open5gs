process.env.DB_URI = process.env.DB_URI || 'mongodb://127.0.0.1/open5gs';

const _hostname = process.env.HOSTNAME || 'localhost';
const port = process.env.PORT || 9999;

const co = require('co');
const next = require('next');

const dev = process.env.NODE_ENV !== 'production';
const app = next({ dev });
const handle = app.getRequestHandler();

const express = require('express');
const bodyParser = require('body-parser');
const methodOverride = require('method-override');
const morgan = require('morgan');
const session = require('express-session');

const mongoose = require('mongoose');
const MongoStore = require('connect-mongo');

const passport = require('passport');
const LocalStrategy = require('passport-local').Strategy;

const csrf = require('lusca').csrf();
const secret = process.env.SECRET_KEY || 'change-me';

const api = require('./routes');

const Account = require('./models/account.js');

co(function* () {
  yield app.prepare();

  mongoose.Promise = global.Promise;
  if (dev) {
    mongoose.set('debug', true);
  }
  const db = yield mongoose.connect(process.env.DB_URI, {
    useNewUrlParser: true,
    useUnifiedTopology: true,
    serverSelectionTimeoutMS: 1000
    /* other options */
  })

  if (dev) {
    Account.count((err, count) => {
      if (err) {
        console.error(err);
        throw err;
      }

      if (!count) {
        const newAccount = new Account();
        newAccount.username = 'admin';
        newAccount.roles = [ 'admin' ];
        Account.register(newAccount, '1423', err => {
          if (err) {
            console.error(err);
            throw err;
          }
        })
      }
    })
  }

  const server = express();
  
  server.use(bodyParser.json());
  server.use(bodyParser.urlencoded({ extended: true }));
  server.use(methodOverride());

  server.use(session({
    secret: secret,
    store: MongoStore.create({
      mongoUrl: process.env.DB_URI,
      ttl: 60 * 60 * 24 * 7 * 2
    }),
    resave: false,
    rolling: true,
    saveUninitialized: true,
    httpOnly: true,
    cookie: {
      maxAge: 1000 * 60 * 60 * 24 * 7 * 2  // 2 weeks
    }
  }));

  server.use((req, res, next) => {
    req.db = db;
    next();
  })

  server.use(csrf);

  server.use(passport.initialize());
  server.use(passport.session());

  passport.use(new LocalStrategy(Account.authenticate()));
  passport.serializeUser(Account.serializeUser());
  passport.deserializeUser(Account.deserializeUser());

  server.use('/api', api);

  server.get('*', (req, res) => {
    return handle(req, res);
  });

  if (dev) {
    server.use(morgan('tiny'));
  }

  server.listen(port, _hostname, err => {
    if (err) throw err;
    console.log('> Ready on http://' + _hostname + ':' + port);
  });
})
.catch(error => console.error(error.stack));
