const bodyParser = require('body-parser');
const session = require('express-session');
const SequelizeStore = require('connect-session-sequelize')(session.Store);
const passport = require('passport');
const LocalStrategy = require('passport-local').Strategy;
const models = require('../models');

exports.configure = ({
  app = null,
  server = null,
  secret = 'change-me',
  store = new SequelizeStore({ db: models.sequelize, table: 'Session' }),
  maxAge = 60000 * 60 * 24 * 7 * 4 // 4 weeks 
} = {}) =>  {
  if (!app) throw new Error('Null param')
  if (!server) throw new Error('Null param')

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
    secret: secret,
    store: store,
    resave: false,
    rolling: true,
    saveUninitialized: true,
    httpOnly: true,
    cookie: {
      maxAge: maxAge
    }
  }));

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
      done(null, user);
    })
  });

  server.use(passport.initialize());
  server.use(passport.session());

  server.post('/login', 
    passport.authenticate('local', { 
      failureRedirect: '/login', 
    }),
    (req, res) => {
      res.redirect('/')
    }
  );

  server.get('/logout', (req, res) => {
    req.logout();
    res.redirect('/');
  });
}
