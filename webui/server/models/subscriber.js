const mongoose = require('mongoose');
const Schema = mongoose.Schema;
require('mongoose-long')(mongoose);

const Subscriber = new Schema({

  schema_version: {
    $type: Number,
    default: 1  // Current Schema Version
  },

  imsi: { $type: String, unique: true, required: true },

  msisdn: [ String ],
  imeisv: [ String ],

  security: {
    k: String,
    op: String,
    opc: String,
    amf: String,
    rand: String,
    sqn: Schema.Types.Long
  },

  ambr: {
    downlink: { value: Number, unit: Number },
    uplink: { value: Number, unit: Number }
  },

  slice: [{
    sst: { $type: Number, required: true },
    sd: String,
    default_indicator: Boolean,
    session: [{
      name: { $type: String, required: true }, // DNN or APN
      type: Number,
      qos: {
        index: Number, // 5QI or QCI
        arp: {
          priority_level: Number,
          pre_emption_capability: Number,
          pre_emption_vulnerability: Number,
        }
      },
      ambr: {
        downlink: { value: Number, unit: Number },
        uplink: { value: Number, unit: Number }
      },
      ue: {
        addr: String,
        addr6: String
      },
      smf: {
        addr: String,
        addr6: String
      },
      pcc_rule: [{
        flow: [{
          direction: Number,
          description: String
        }],
        qos: {
          index: Number, // 5QI or QCI
          arp: {
            priority_level: Number,
            pre_emption_capability: Number,
            pre_emption_vulnerability: Number,
          },
          mbr: {
            downlink: { value: Number, unit: Number },
            uplink: { value: Number, unit: Number }
          },
          gbr: {
            downlink: { value: Number, unit: Number },
            uplink: { value: Number, unit: Number }
          },
        },
      }]
    }]
  }],

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
    default: 0 // Packet and Circuit
  },
  subscribed_rau_tau_timer: {
    $type: Number,
    default: 12 // minites
  }

}, { typeKey: '$type' });

module.exports = mongoose.model('Subscriber', Subscriber);
