/*
 * TMF666 BillingAccount — persistence for the TM Forum Account Management API.
 *
 * Collection: tmf.billingAccounts.  Groups the data buckets (chf.subscribers
 * documents, via their billingAccountId) charged together.  A BSS record
 * only; the CHF NF never reads it.  Validation lives in server/tmf/account.js.
 *
 * Copyright (C) 2026 by Rami Mohamed <ramrode@gmail.com>
 */

'use strict';

const mongoose = require('mongoose');

const BillingAccount = new mongoose.Schema(
  {
    id: { type: String, required: true, unique: true },
    name: { type: String, required: true },
  },
  {
    collection: 'tmf.billingAccounts',
    strict: false,
    id: false,
    versionKey: false,
  }
);

/*
 * Individual deletion refuses while an account still references it, which
 * queries relatedParty.id.  Declared as a schema-level index rather than a
 * 'relatedParty.id' path: a path would make Mongoose treat relatedParty as a
 * single object and mangle the TMF array on save.
 */
BillingAccount.index({ 'relatedParty.id': 1 });

module.exports = mongoose.model('TmfBillingAccount', BillingAccount);
