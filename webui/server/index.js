process.env.DB_URI = process.env.DB_URI || 'mongodb://localhost/nextepc';

const next = require('next');

const dev = process.env.NODE_ENV !== 'production';
const app = next({ dev });
const handle = app.getRequestHandler();

const express = require('express');
const bodyParser = require('body-parser');
const morgan = require('morgan');
const session = require('express-session');

const mongoose = require('mongoose');
const MongoStore = require('connect-mongo')(session);

const passport = require('passport');
require('./passport');
const csrf = require('lusca').csrf();
const secret = process.env.SECRET_KEY || 'change-me';

const models = require('./models');
const api = require('./routes');

Promise.all([ 
  app.prepare(), 
  models.sequelize.sync()
])
.then(() => {
  mongoose.Promise = global.Promise;
  const db = mongoose.connection;
  db.on('error', err => { throw err });
  db.once('open', () => {
    console.log('Connected to mongod server');
  });

  if (dev) {
    mongoose.set('debug', true);
  }

  mongoose.connect(process.env.DB_URI);

  // FIXME : we need to implement landing page for adding admin account
  models.AccountRole.count().then(c => {
    if (c == 0) {
      models.AccountRole.create({
        role: 'admin',
        Accounts: [{
          username: 'admin',
          password: '1423'
        }]
      }, {
        include: [ models.AccountRole.Account ]
      });
    }
  });

  const server = express();
  
  server.use(bodyParser.json());
  server.use(bodyParser.urlencoded({ extended: true }));

  server.use(session({
    secret: secret,
    store: new MongoStore({ 
      mongooseConnection: mongoose.connection,
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
    csrf(req, res, next);
  })

  server.use(passport.initialize());
  server.use(passport.session());

  server.use('/api', api);

  server.get('*', (req, res) => {
    return handle(req, res);
  });

  if (dev) {
    server.use(morgan('tiny'));
  }

  server.listen(3000, err => {
    if (err) throw err;
    console.log('> Ready on http://localhost:3000');
  });
})
.catch(err => console.log(err));
