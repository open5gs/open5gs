const express = require('express');
const router = express.Router();

const passport = require('passport');

router.get('/csrf', (req, res) => {
  return res.json({csrfToken: res.locals._csrf});
})

router.get('/session', (req, res) => {
  let session = {
    clientMaxAge: 60000, // 60 seconds
    csrfToken: res.locals._csrf
  }
  if (req.user) {
    session.user = req.user
  }

  return res.json(session)
})

router.post('/login', 
  passport.authenticate('local', { successRedirect: '/' }));

router.post('/logout', (req, res) => {
  req.logout();
  res.redirect('/');
});

module.exports = router;