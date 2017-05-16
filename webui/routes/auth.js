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
  store = new FileStore({path: '/tmp/sessions', secret: secret}),
  maxAge = 60000 * 60 * 24 * 7 * 4, // 4 weeks 
  clientMaxAge = 60 * 1000 // 60 seconds
} = {}) =>  {
  if (app === null) throw new Error('Null param')
  if (server === null) throw new Error('Null param')
  if (models === null) throw new Error('Null param')

  models.UserRole.count().then(c => {
    if (c == 0) {
      models.UserRole.create({
        role: 'admin',
        Users: [{
          username: 'admin',
          password: '1423'
        }]
      }, {
        include: [models.UserRole.User]
      });
    }
  });

  server.use(bodyParser.json());
  server.use(bodyParser.urlencoded({extended: true}));

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
    csrf()
  })

  server.use(passport.initialize());
  server.use(passport.session());
}
