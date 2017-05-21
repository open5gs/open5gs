const passport = require('passport');
const LocalStrategy = require('passport-local').Strategy;

const models = require('../models');

passport.use(new LocalStrategy((username, password, done) => {
    models.Account.findOne({ where: {username: username} }).then(account => {
      if (!account) { 
        return done(null, false, { message: 'Incorrect username' }); 
      }
      if (account.password != password) {
        return done(null, false, { message: 'Incorrect password' });
      }
      return done(null, account);
    });
  }));

passport.serializeUser((account, done) => {
  done(null, account.id);
})

passport.deserializeUser((id, done) => {
  models.Account.findOne({
    where: { id: id },
    include: [{ model: models.AccountRole }]
  }).then(account => {
    done(null, {
      username: account.username,
      role: account.AccountRole.role
    });
  })
});
