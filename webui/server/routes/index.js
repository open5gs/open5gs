const express = require('express');
const auth = require('./auth');
const db = require('./db')

const router = express.Router();

const secret = process.env.JWT_SECRET_KEY;

const passport = require('passport');
const JWTstrategy = require('passport-jwt').Strategy;
const ExtractJWT = require('passport-jwt').ExtractJwt;

passport.use(
  new JWTstrategy(
    {
      secretOrKey: secret,
      jwtFromRequest: ExtractJWT.fromAuthHeaderWithScheme('bearer')
    },
    async (token, done) => {
      try {
        return done(null, token.user);
      } catch (error) {
        done(error);
      }
    }
  )
);

router.use('/auth', auth);
router.use('/db', passport.authenticate('jwt', { session: false }), db);

/* TM Forum Open APIs (TMF632 / TMF666 / TMF654), same bearer auth as /db. */
const tmf = require('../tmf');
router.use('/tmf-api', passport.authenticate('jwt', { session: false }),
    tmf.createTmfRouter(tmf.mongooseRepos()));

module.exports = router;
