const express = require('express');
const router = express.Router();

const restify = require('express-restify-mongoose')

const Subscriber = require('../models/subscriber');
restify.serve(router, Subscriber, {
  prefix: '',
  version: '',
  idProperty: 'imsi'
});

module.exports = router;