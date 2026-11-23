/*
 * Active CHF charging session — read-only view for the WebUI.
 *
 * This schema mirrors the documents written by the CHF NF into
 * the open5gs.chf.sessions MongoDB collection.  The WebUI only
 * READS these documents; the CHF NF is the sole writer.
 *
 * Collection: open5gs.chf.sessions
 *
 * 3GPP references:
 *   TS 23.255 §6   — active session monitoring
 *   TS 32.291 §6.3 — Nchf_ConvergedCharging session state
 *
 * Copyright (C) 2026 by Rami Mohamed <ramrode@gmail.com>
 */

'use strict';

const mongoose = require('mongoose');
const Schema = mongoose.Schema;

const ChfSession = new Schema(
  {
    /*
     * ChargingDataRef — the resource URI suffix returned by CHF in the
     * 201 Created Location header (TS 32.291 §6.3.1.2).
     * Used as the document key.
     */
    chargingDataRef: { $type: String, unique: true },

    /* SUPI of the subscriber (TS 23.003 §2.2B) */
    supi: { $type: String },

    /* Data Network Name (APN/DNN, TS 23.003 §9.1) */
    dnn: { $type: String },

    /* PDU Session ID (TS 24.501 §9.11.3.41) */
    pduSessionId: { $type: Number },

    /* Timestamp when the CHF Create was received */
    startTime: { $type: Date },

    /* Accumulated uplink octets reported by PFCP URRs */
    ulOctets: { $type: Number, default: 0 },

    /* Accumulated downlink octets reported by PFCP URRs */
    dlOctets: { $type: Number, default: 0 },

    /* UL + DL combined octets (denormalised sum written by db-handler.c) */
    totalOctets: { $type: Number, default: 0 },

    /* Granted total quota in bytes — UL+DL combined (db-handler.c: grantedTotalBytes) */
    grantedTotalBytes: { $type: Number },

    /* Session duration in seconds at last update */
    duration: { $type: Number, default: 0 },

    /* Whether the quota auto-renews at period expiry */
    autoRenew: { $type: Boolean },

    /* Validity period length in seconds */
    periodLengthSec: { $type: Number },

    /* GPSI (Generic Public Subscription Identifier, TS 23.003 §2.2C) */
    gpsi: { $type: String },

    /* Timestamp of last update written by db-handler.c */
    updatedAt: { $type: Date }
  },
  {
    typeKey: '$type',

    /* Map to the CHF sessions collection; do not auto-create it */
    collection: 'chf.sessions',
    autoCreate: false
  }
);

module.exports = mongoose.model('ChfSession', ChfSession);
