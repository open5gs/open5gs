/*
 * SimSwap — the audit trail of SIM swaps.
 *
 * Collection: tmf.simSwaps.  Every attempt is recorded before a bucket is
 * touched, with the id each bucket had and the id it was given, so a failed
 * or interrupted swap leaves a trace of exactly what it meant to move.
 *
 * idempotencyKey is unique (sparse, since a request need not carry one).  It
 * is what makes a retried POST safe: a repeat collides on insert and returns
 * the original record.  Without it a repeat of a completed swap finds the
 * old SIM with nothing left to move and is refused.
 *
 * locks holds both SUPIs while a swap is in progress, and is removed when it
 * ends.  The index on it is unique across records, so no two swaps that share
 * a SIM can run at once.  It must be removed, not emptied: an empty array is
 * still indexed, and two finished records would collide on it.
 *
 * lockedAt is when the lock was last taken: at the start, or by a request
 * that is undoing an interrupted swap.  A lock older than a minute is taken
 * to be abandoned, and writing lockedAt is how one request claims the undo.
 *
 * Copyright (C) 2026 by Rami Mohamed <ramrode@gmail.com>
 */

'use strict';

const mongoose = require('mongoose');

const SimSwap = new mongoose.Schema(
  {
    id: { type: String, required: true, unique: true },
    status: { type: String, index: true },
    requestedDate: String,
    confirmationDate: String,
    /* The SIM being replaced, and the one replacing it. */
    logicalResource: { id: { type: String, index: true } },
    receiverLogicalResource: { id: { type: String, index: true } },
    /* One entry per bucket: its id now, and the id it had.  A pool keeps
     * its id, and is marked shared: its member changed, not its id. */
    bucket: [{ _id: false, id: String, formerId: String, shared: Boolean }],
    reason: String,
    statusReason: String,
    idempotencyKey: { type: String, unique: true, sparse: true },
    locks: { type: [String], default: undefined, unique: true, sparse: true },
    lockedAt: String,
  },
  {
    collection: 'tmf.simSwaps',
    id: false,
    versionKey: false,
  }
);

module.exports = mongoose.model('TmfSimSwap', SimSwap);
