const passport = require('passport');
const LocalStrategy = require('passport-local').Strategy;

const models = require('../models');

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
