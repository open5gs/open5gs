const express = require('express');
const next = require('next');

const dev = process.env.NODE_ENV != 'production';
const app = next({ dev });
const handle = app.getRequestHandler();

const bodyParser = require('body-parser');
const session = require('express-session');
const SequelizeStore = require('connect-session-sequelize')(session.Store);
const passport = require('passport');
require('./passport');
const csrf = require('lusca').csrf();
const secret = process.env.SECRET_KEY || 'change-me';

const models = require('./models');
const api = require('./routes');

app.prepare()
.then(() => {
  return models.sequelize.sync();
})
.then(() => {
  const server = express();

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

  server.use(bodyParser.json());
  server.use(bodyParser.urlencoded({ extended: true }));

  server.use(session({
    secret: secret,
    store: new SequelizeStore({ db: models.sequelize, table: 'Session' }),
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

  server.listen(3000, err => {
    if (err) throw err;
    console.log('> Ready on http://localhost:3000');
  });
})
.catch(err => console.log(err));
