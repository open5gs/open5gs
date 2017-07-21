const express = require('express');
const auth = require('./auth');
const db = require('./db')

const router = express.Router();

router.use('/auth', auth);
router.use('/db', db);

module.exports = router;