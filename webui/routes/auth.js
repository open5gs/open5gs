const bodyParser = require('body-parser');
const session = require('express-session');
const csrf = require('lusca').csrf();
const FileStore = require('session-file-store')(session);
const passport = require('passport');
const Strategy = require('passport-local').Strategy;

exports.configure = ({
  app = null,
  server = null,
  models = null,
  secret = 'change-me',
  store = new FileStore({ path: '/tmp/sessions', secret: secret }),
  maxAge = 60000 * 60 * 24 * 7 * 4, // 4 weeks 
  clientMaxAge = 60 * 1000 // 60 seconds
} = {}) =>  {
  if (!app) throw new Error('Null param')
  if (!server) throw new Error('Null param')
  if (!models) throw new Error('Null param')

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

  server.use((req, res, next) => {
    csrf(req, res, next);
  });

  passport.use(new Strategy((username, password, cb) => {
    models.User.findOne({ where: {username: username} }).then(user => {
      if (!user) return cb(null, false);
      if (user.password != password) return cb(null, false);
      return cb(null, user);
    });
  }));

  passport.serializeUser((user, cb) => {
    cb(null, user.id);
  })

  passport.deserializeUser((id, cb) => {
    models.User.findById(id).then(user => {
      cb(null, user);
    })
  });
  server.use(passport.initialize());
  server.use(passport.session());

  server.get('/csrf', (req, res) => {
    return res.json({ csrfToken: res.locals._csrf });
  });

  server.get('/session', (req, res) => {
    let session = {
      clientMaxAge: clientMaxAge,
      csrfToken: res.locals._csrf
    }

    if (req.user) session.user = req.user;

    return res.json(session);
  });

  server.post('/login', 
    passport.authenticate('local', { failureRedirect: '/login' }),
    (req, res) => {
      res.redirect('/')
    }
  );

  server.get('/logout', (req, res) => {
    req.logout();
    res.redirect('/');
  });
}
