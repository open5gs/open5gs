const mongoose = require('mongoose');
const Schema = mongoose.Schema;
const Eir = new Schema(
  {
    pei: {
      $type: String,
      required: true,
      match: /^(imei-[0-9]{15}|imeisv-[0-9]{16})$/
    },
    supi: { $type: String, default: undefined, match: /^imsi-[0-9]{6,15}$/ },
    status: {
      $type: String,
      required: true,
      enum: ['WHITELISTED', 'BLACKLISTED', 'GREYLISTED']
    }
  },
  { typeKey: '$type', collection: 'eir' }
);
Eir.index({ pei: 1 }, { unique: true, name: 'pei_unique' });
Eir.index({ supi: 1 }, { unique: true, sparse: true, name: 'supi_unique' });
module.exports = mongoose.model('Eir', Eir);
