const express = require('express');
const auth = require('./auth');
const db = require('./db')

const router = express.Router();

var crypto = require("crypto");
const secret = process.env.JWT_SECRET_KEY || crypto.randomBytes(32).toString('hex');;
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

module.exports = router;