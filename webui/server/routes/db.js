const express = require('express');
const router = express.Router();

const restify = require('express-restify-mongoose')
const options = {
  prefix: '',
  version: ''
}

const Subscriber = require('../models/subscriber');

restify.serve(router, Subscriber, options);

module.exports = router;