/*
 * TMF654 TopupBalance — the audit trail of balance top-ups.
 *
 * Collection: tmf.topupBalances.  Every top-up attempt is recorded before the
 * balance is touched, so a failed or interrupted attempt still leaves a trace.
 *
 * idempotencyKey is unique (sparse, since most requests carry none).  It is
 * what makes a retried POST safe: a repeat collides on insert and returns the
 * original record instead of crediting the subscriber a second time.
 *
 * Copyright (C) 2026 by Rami Mohamed <ramrode@gmail.com>
 */

'use strict';

const mongoose = require('mongoose');

const TopupBalance = new mongoose.Schema(
  {
    id: { type: String, required: true, unique: true },
    status: { type: String, index: true },
    requestedDate: String,
    confirmationDate: String,
    amount: Number,
    bucket: { id: { type: String, index: true } },
    reason: String,
    statusReason: String,
    reactivated: Boolean,
    idempotencyKey: { type: String, unique: true, sparse: true },
  },
  {
    collection: 'tmf.topupBalances',
    id: false,
    versionKey: false,
  }
);

module.exports = mongoose.model('TmfTopupBalance', TopupBalance);
