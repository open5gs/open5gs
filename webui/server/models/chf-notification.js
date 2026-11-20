/*
 * ChargingNotify queue — what the billing side wants a live session to hear
 * now rather than at its next usage report.
 *
 * Collection: chf.notifications.  Every CHF NF polls it (see
 * src/chf/notify-path.c) and turns each row into a TS 32.291
 * ChargingNotifyRequest to the sessions it holds that draw from the bucket.
 * Several CHFs can share the database, so none deletes a row on reading it:
 * each adds its NF instance id to seenBy and never reads that row again,
 * and rows are deleted five minutes after the first CHF read them.
 *
 * A queue rather than a call: the WebUI does not speak Nchf and the CHF has
 * no inbound admin endpoint, and MongoDB change streams need a replica set.
 *
 * Nothing here is read back by the WebUI, so the documents are write-only
 * from this side; the CHFs add seenBy and seenAt to them, and delete them.
 *
 * Copyright (C) 2026 by Rami Mohamed <ramrode@gmail.com>
 */

'use strict';

const mongoose = require('mongoose');

const Notification = new mongoose.Schema(
  {
    /* Unique per row, so a repeat is a new notification rather than an
     * overwrite.  The CHF never reads this; it is here so one row can be
     * told from another. */
    id: { type: String, required: true, unique: true },
    /* Whose sessions to reach. */
    supi: { type: String, required: true, index: true },
    /* Which of that subscriber's buckets; absent means all of them. */
    bucketId: String,
    /* REAUTHORIZATION: report now and take a fresh grant.
     * ABORT_CHARGING: this session may not continue. */
    type: {
      type: String,
      required: true,
      enum: ['REAUTHORIZATION', 'ABORT_CHARGING'],
    },
    /* Free text, logged by the CHF: 'topup', 'plan-change', ... */
    reason: String,
    createdAt: { type: Date, default: Date.now },
  },
  {
    collection: 'chf.notifications',
    id: false,
    versionKey: false,
  }
);

module.exports = mongoose.model('ChfNotification', Notification);
