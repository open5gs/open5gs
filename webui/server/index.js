process.env.DB_URI = process.env.DB_URI || 'mongodb://127.0.0.1/open5gs';

const _hostname = process.env.HOSTNAME || 'localhost';
const port = process.env.PORT || 9999;

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
const { migrateChfBuckets } = require('./chf-migrate');
const MongoStore = require('connect-mongo');

const passport = require('passport');
const LocalStrategy = require('passport-local').Strategy;

const csrf = require('lusca').csrf();

require('./ensure-secret')();

const secret = process.env.SECRET_KEY;


const api = require('./routes');
const metrics = require('./metrics');

const Account = require('./models/account.js');

(async () => {
  await app.prepare();

  mongoose.Promise = global.Promise;
  if (dev) {
    mongoose.set('debug', true);
  }
  const db = await mongoose.connect(process.env.DB_URI, {
    useNewUrlParser: true,
    useUnifiedTopology: true,
    serverSelectionTimeoutMS: 1000
    /* other options */
  })

  /* chf.subscribers holds one document per quota bucket now, not one per
   * subscriber; see server/chf-migrate.js. */
  await migrateChfBuckets(mongoose.connection);

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

  /* Prometheus scrape endpoint — unauthenticated, no CSRF (read-only, and a
   * scraper cannot present a session cookie or JWT). Mounted before the
   * session/CSRF/auth middleware. */
  server.get('/metrics', metrics.handler);

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

  /*
   * CSRF protection for session-cookie requests; skipped for requests that
   * authenticate with a JWT Bearer token, so non-browser API clients (curl
   * scripts, integration tests) do not need to fetch a CSRF token first.
   *
   * This is safe only because a cross-site page cannot set an Authorization
   * header on a request the browser sends automatically: doing so makes the
   * request non-simple, which requires a CORS preflight, and this server
   * mounts no CORS middleware, so the preflight fails. The React frontend
   * sends both headers and is unaffected either way.
   *
   * !! If CORS middleware is ever added to this server, revisit this: an
   *    allowed origin could then set Authorization and bypass CSRF on every
   *    route below, including the non-CHF ones.
   */
  const conditionalCsrf = (req, res, next) => {
    if (req.headers.authorization &&
        req.headers.authorization.toLowerCase().startsWith('bearer ')) {
      return next();
    }
    return csrf(req, res, next);
  };
  server.use(conditionalCsrf);

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
})()
.catch(error => console.error(error.stack));
