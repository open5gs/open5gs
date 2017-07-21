const mongoose = require('mongoose');
const Schema = mongoose.Schema;
const passportLocalMongoose = require('passport-local-mongoose');

const Account = new Schema({
  roles: [String]
});

Account.plugin(passportLocalMongoose);

module.exports = mongoose.model('Account', Account);