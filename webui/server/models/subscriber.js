const mongoose = require('mongoose');
const Schema = mongoose.Schema;

const Subscriber = new Schema({
  imsi: { type: String, unique: true, required: true },

  access_restriction_data: Number,
  subscriber_status: Number,
  network_access_mode: Number,

  ue_ambr: {
    max_bandwidth_ul: Number,
    max_bandwidth_dl: Number
  },

  pdn: [{
    apn: String,
    type: Number,
    pdn_ambr: {
      max_bandwidth_ul: Number,
      max_bandwidth_dl: Number
    },
    qci: Number,
    priority_level: Number,
    pre_emption_capability: Number,
    pre_emption_vulnerability: Number
  }],

  security: {
    k: String,
    sqn: Number,
    rand: String,
    op: String,
    amf: String
  }
});

module.exports = mongoose.model('Subscriber', Subscriber);