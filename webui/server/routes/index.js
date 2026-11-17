const express = require('express');
const auth = require('./auth');
const db = require('./db')

const router = express.Router();

const secret = process.env.JWT_SECRET_KEY;

const passport = require('passport');
const JWTstrategy = require('passport-jwt').Strategy;
const ExtractJWT = require('passport-jwt').ExtractJwt;

const Account = require('../models/account');

passport.use(
  new JWTstrategy(
    {
      secretOrKey: secret,
      jwtFromRequest: ExtractJWT.fromAuthHeaderWithScheme('bearer')
    },
    async (token, done) => {
      try {
        // The token only names the account; take the roles from the
        // database so that a deleted or demoted account loses access.
        const account = token.user && token.user._id &&
          await Account.findById(token.user._id).lean();
        if (!account) {
          return done(null, false);
        }
        return done(null, {
          '_id': account._id,
          'username': account.username,
          'roles': account.roles
        });
      } catch (error) {
        done(error);
      }
    }
  )
);

router.use('/auth', auth);
router.use('/db', passport.authenticate('jwt', { session: false }), db);

module.exports = router;
