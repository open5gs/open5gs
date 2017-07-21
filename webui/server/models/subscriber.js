const mongoose = require('mongoose');
const Schema = mongoose.Schema;

const Subscriber = new Schema({
  imsi: { $type: String, unique: true, required: true },

  security: {
    k: String,
    op: String,
    amf: String,
    rand: String,
    sqn: Number
  },

  access_restriction_data: { 
    $type: Number, 
    default: 32 // Handover to Non-3GPP Access Not Allowed
  },
  subscriber_status: { 
    $type: Number,
    default: 0  // Service Granted
  },
  network_access_mode: {
    $type: Number,
    default: 2 // Only Packet
  },

  subscribed_rau_tau_timer: {
    $type: Number,
    default: 12 // minites
  },

  ue_ambr: {
    max_bandwidth_ul: Number,
    max_bandwidth_dl: Number
  },

  pdn: [{
    apn: { $type: String, required: true },
    type: {
      $type: Number, default: 0 // IPv4
    },
    qos: {
      qci: Number,
      arp: {
        priority_level: Number,
        pre_emption_capability: {
          $type: Number, default: 1 // Capability Disabled 
        },
        pre_emption_vulnerability: {
          $type : Number, default: 1 // Vulnerability Disabled 
        }
      }
    },
    pdn_ambr: {
      max_bandwidth_ul: Number,
      max_bandwidth_dl: Number
    }
  }]
}, { typeKey: '$type' });

module.exports = mongoose.model('Subscriber', Subscriber);
