const express = require('express');
const next = require('next');

const dev = process.env.NODE_ENV != 'production';
const app = next({ dev });
const handle = app.getRequestHandler();

const bodyParser = require('body-parser');
const session = require('express-session');
const SequelizeStore = require('connect-session-sequelize')(session.Store);
const passport = require('passport');
const LocalStrategy = require('passport-local').Strategy;
const csrf = require('lusca').csrf();

const models = require('./models');
const api = require('./routes');

app.prepare()
.then(() => {
  return models.sequelize.sync();
})
.then(() => {
  const server = express();

  models.UserRole.count().then(c => {
    if (c == 0) {
      models.UserRole.create({
        role: 'admin',
        Users: [{
          username: 'admin',
          password: '1423'
        }]
      }, {
        include: [ models.UserRole.User ]
      });
    }
  });

  server.use(bodyParser.json());
  server.use(bodyParser.urlencoded({ extended: true }));

  server.use(session({
    secret: 'change-me',
    store: new SequelizeStore({ db: models.sequelize, table: 'Session' }),
    resave: false,
    rolling: true,
    saveUninitialized: true,
    httpOnly: true,
    cookie: {
      maxAge: 60000 * 60 * 24 * 7 * 4  // 4 weeks
    }
  }));

  server.use((req, res, next) => {
    csrf(req, res, next);
  })

  passport.use(new LocalStrategy((username, password, done) => {
    models.User.findOne({ where: {username: username} }).then(user => {
      if (!user) { 
        return done(null, false, { message: 'Incorrect username' }); 
      }
      if (user.password != password) {
        return done(null, false, { message: 'Incorrect password' });
      }
      return done(null, user);
    });
  }));

  passport.serializeUser((user, done) => {
    done(null, user.id);
  })

  passport.deserializeUser((id, done) => {
    models.User.findOne({
      where: { id: id },
      include: [{ model: models.UserRole }]
    }).then(user => {
      done(null, {
        username: user.username,
        role: user.UserRole.role
      });
    })
  });

  server.use(passport.initialize());
  server.use(passport.session());

  server.use('/api', api);

  server.get('*', (req, res) => {
    return handle(req, res);
  });

  // Set vary header (good practice)
  // Note: This overrides any existing 'Vary' header but is okay in this app
  server.use(function (req, res, next) {
    res.setHeader('Vary', 'Accept-Encoding')
    next()
  });

  server.listen(3000, err => {
    if (err) throw err;
    console.log('> Ready on http://localhost:3000');
  });
})
.catch(err => console.log(err));
