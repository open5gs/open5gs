const express = require('express');
const router = express.Router();

const restify = require('express-restify-mongoose')

const Subscriber = require('../models/subscriber');
restify.serve(router, Subscriber, {
  prefix: '',
  version: '',
  idProperty: 'imsi'
});

const Eir = require('../models/eir');
restify.serve(router, Eir, { prefix: '', version: '' });

const Profile = require('../models/profile');
restify.serve(router, Profile, {
  prefix: '',
  version: ''
});

/*
 * The UI only offers account management to admins, but these routes accept
 * any valid token, so enforce the same rules here: only an admin may create,
 * delete or edit other accounts, and a non-admin may only change the
 * password (hash/salt) of their own account. The edit form posts the whole
 * document back, so a non-admin's other fields (roles, username) are dropped
 * rather than rejected.
 */
const isAdmin = req =>
  Array.isArray(req.user.roles) && req.user.roles.indexOf('admin') !== -1;

const requireAdmin = (req, res, next) =>
  isAdmin(req) ? next() : res.sendStatus(403);

const Account = require('../models/account');
restify.serve(router, Account, {
  prefix: '',
  version: '',
  idProperty: 'username',
  preRead: (req, res, next) => {
    /*
     * hash and salt are select:false in the schema, but the select,
     * distinct, query and sort options can still reach them. The UI
     * does not send any of these for accounts.
     */
    req.erm.query = {};
    next();
  },
  preCreate: requireAdmin,
  preUpdate: (req, res, next) => {
    if (isAdmin(req))
      return next();

    if (req.params.id !== req.user.username)
      return res.sendStatus(403);

    const body = {};
    ['hash', 'salt'].forEach(key => {
      if (req.body && req.body[key] !== undefined)
        body[key] = req.body[key];
    });
    req.body = body;
    next();
  },
  preDelete: requireAdmin
});

module.exports = router;