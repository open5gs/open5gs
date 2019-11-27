const mongoose = require('mongoose');
const Schema = mongoose.Schema;
require('mongoose-long')(mongoose);

const Profile = new Schema({
  title: { $type: String, required: true },

  security: {
    k: String,
    op: String,
    opc: String,
    amf: String,
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
          $type : Number, default: 1 // Vulnerability Disabled
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
            $type : Number, default: 1 // Vulnerability Disabled
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

module.exports = mongoose.model('Profile', Profile);
