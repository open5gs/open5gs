const mongoose = require('mongoose');
const Schema = mongoose.Schema;
require('mongoose-long')(mongoose);

const Subscriber = new Schema({
  imsi: { $type: String, unique: true, required: true },

  security: {
    k: String,
    op: String,
    opc: String,
    amf: String,
    rand: String,
    sqn: Schema.Types.Long
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

  ambr: {
    downlink: Schema.Types.Long,
    uplink: Schema.Types.Long
  },

  pdn: [{
    apn: { $type: String, required: true },
    type: {
      $type: Number, default: 2 // IPv4, IPv6 and dualstack IPv4v6
    },
    qos: {
      qci: Number,
      arp: {
        priority_level: Number,
        pre_emption_capability: {
          $type: Number, default: 1 // Capability Disabled
        },
        pre_emption_vulnerability: {
          $type : Number, default: 0 // Vulnerability Disabled
        }
      }
    },
    ambr: {
      downlink: Schema.Types.Long,
      uplink: Schema.Types.Long
    },
    ue: {
      addr: String,
      addr6: String
    },
    pgw: {
      addr: String,
      addr6: String
    },
    pcc_rule: [{
      flow: [{
        direction: Number,
        description: String
      }],
      qos: {
        qci: Number,
        arp: {
          priority_level: Number,
          pre_emption_capability: {
            $type: Number, default: 1 // Capability Disabled
          },
          pre_emption_vulnerability: {
            $type : Number, default: 0 // Vulnerability Disabled
          }
        },
        mbr: {
          downlink: Schema.Types.Long,
          uplink: Schema.Types.Long
        },
        gbr: {
          downlink: Schema.Types.Long,
          uplink: Schema.Types.Long
        },
      },
    }]
  }]
}, { typeKey: '$type' });

module.exports = mongoose.model('Subscriber', Subscriber);
