/*
 * SIM swap — move a subscriber's data balances to a new SIM.
 *
 *   POST /prepayBalanceManagement/v4/simSwap          swap
 *   GET  /prepayBalanceManagement/v4/simSwap[?logicalResource.id=&status=]
 *   GET  /prepayBalanceManagement/v4/simSwap/{id}
 *
 * TMF654 has no SIM swap.  This is an extension in its namespace, shaped as
 * a TMF630 task resource like topupBalance, and it borrows the names TMF654
 * TransferBalance uses for the two ends: `logicalResource` is the SIM being
 * replaced and `receiverLogicalResource` the one replacing it.
 *
 * What it does: every bucket of the old SUPI becomes a bucket of the new
 * one.  The document is moved, not copied, so the plan, the scope, the
 * priority, the billing account and this cycle's counter - what was used and
 * what was topped up - all arrive unchanged, and the CHF charges the new SIM
 * from its first session.  If the old SIM is in a pool, the new SIM takes
 * its place in the members, in the same position.  Nothing is left with the
 * old SUPI.
 *
 * Copyright (C) 2026 by Rami Mohamed <ramrode@gmail.com>
 */

'use strict';

const express = require('express');
const crypto = require('crypto');
const C = require('./common');
const B = require('./bucket-map');
const { getBucket, notifyChf } = require('./balance');

const FILTERABLE = {
  'logicalResource.id': 'logicalResource.id',
  'receiverLogicalResource.id': 'receiverLogicalResource.id',
  status: 'status',
};

const READ_ONLY = ['id', 'href', '@type', '@baseType', 'status',
  'requestedDate', 'confirmationDate', 'statusReason', 'bucket'];

/* Idempotency-Key values are opaque to us, but bound them. */
const IDEMPOTENCY_KEY_RE = /^[\x21-\x7e]{1,128}$/;

/* The CHF reads at most 16 buckets per subscriber; this is far above it. */
const MAX_BUCKETS = 1000;

/* A swap is a handful of single-document writes and takes milliseconds.  One
 * still marked in progress after this long was interrupted. */
const LOCK_STALE_MS = 60 * 1000;

function supiOf(body, name) {
  const ref = body[name];
  const supi = C.isPlainObject(ref) && ref.id;
  if (typeof supi !== 'string' || !B.SUPI_RE.test(supi))
    throw new C.TmfError(400, 'Bad Request',
        `'${name}.id' must be a SUPI of the form imsi-<15 digits>`);
  return supi;
}

/*
 * The id a bucket takes on the new SIM.  The catch-all's id is the SUPI
 * itself, and a scoped bucket's is the SUPI plus a suffix, which it keeps:
 * a caller holding the old id can work out the new one, and the swap record
 * lists both in any case.  An id that follows neither rule - one written by
 * hand through /api/db/ChfSubscriber - gets a fresh suffix.
 */
function movedId(oldId, from, to) {
  if (oldId === from) return to;
  if (oldId.startsWith(`${from}-`)) return to + oldId.slice(from.length);
  return `${to}-${crypto.randomBytes(6).toString('hex')}`;
}

function represent(req, t) {
  const out = {
    id: t.id,
    href: C.hrefFor(req, 'simSwap', t.id),
    '@type': 'SimSwap',
    status: t.status,
    requestedDate: t.requestedDate,
    logicalResource: { id: t.logicalResource.id,
                       '@referredType': 'LogicalResource' },
    receiverLogicalResource: { id: t.receiverLogicalResource.id,
                               '@referredType': 'LogicalResource' },
    bucket: (t.bucket || []).map(b => Object.assign({
      id: b.id, href: C.hrefFor(req, 'bucket', b.id),
      formerId: b.formerId, '@referredType': 'Bucket' },
      b.shared ? { isShared: true } : {})),
  };
  if (t.confirmationDate) out.confirmationDate = t.confirmationDate;
  if (t.reason) out.reason = t.reason;
  if (t.statusReason) out.statusReason = t.statusReason;
  return out;
}

/*
 * Put moved buckets back with the old SIM.  Each write is conditional on the
 * bucket still being where the swap put it, so one that was never moved, or
 * that someone has deleted since, is simply not matched.  Returns the ids
 * whose write failed outright.
 */
async function putBack(repos, moves, from, to) {
  const stuck = [];
  for (const m of moves.slice().reverse()) {
    try {
      if (m.shared)
        /* A pool: the old SIM back in the new SIM's place. */
        await repos.bucket.update({ bucketId: m.id, members: to },
            { $set: { 'members.$': from } });
      else
        await repos.bucket.update({ bucketId: m.id, supi: to },
            { $set: { supi: from, bucketId: m.formerId } });
    } catch (err) {
      stuck.push(m.id);
    }
  }
  return stuck;
}

/* When a swap's lock was last taken: at its start, or by whoever is undoing
 * it. */
const lockedAt = t => Date.parse(t.lockedAt || t.requestedDate);

/*
 * Undo a swap that was interrupted - the server stopped between two of its
 * writes.  Its record says what it meant to move, so whatever it did move
 * is put back, and the record is closed as failed.
 *
 * Two requests can find the same interrupted swap.  Only the one whose
 * write to lockedAt lands undoes it, and the record goes on holding both
 * SIMs until that is done.  Otherwise a put-back could run late, across a
 * newer swap of the same two SIMs, and take back what that one moved.
 *
 * Returns false if another request is already undoing it.
 */
async function undoInterrupted(repos, t, now) {
  const mine = await repos.simSwap.update(
      { id: t.id, status: 'inProgress', lockedAt: t.lockedAt || null },
      { $set: { lockedAt: new Date(now).toISOString() } });
  if (!mine) return false;

  const stuck = await putBack(repos, t.bucket || [],
      t.logicalResource.id, t.receiverLogicalResource.id);
  await repos.simSwap.update({ id: t.id, status: 'inProgress' },
      { $set: { status: 'failed', statusReason: stuck.length
            ? `Interrupted; could not put back: ${stuck.join(', ')}`
            : 'Interrupted; its buckets were put back' },
        $unset: { locks: '', lockedAt: '' } });
  if (stuck.length)
    throw new C.TmfError(500, 'Internal Server Error',
        `An interrupted swap could not be undone; see simSwap '${t.id}'`);
  return true;
}

/*
 * @param repos.bucket        keyed by 'bucketId' (chf.subscribers)
 * @param repos.simSwap       keyed by 'id' (the swap audit trail)
 * @param repos.notification  the ChargingNotify queue; optional
 * @param clock               () => epoch ms; injectable for tests
 */
function createSimSwapRouter(repos, clock = Date.now) {
  const router = express.Router();

  router.get('/simSwap', C.handler(async (req, res) => {
    const { offset, limit } = C.paging(req.query);
    const filter = C.buildFilter(req.query, FILTERABLE);
    const { items, total } = await repos.simSwap.list(filter, offset, limit);
    return C.sendList(res, items.map(t =>
        C.project(represent(req, t), req.query.fields)), total);
  }));

  router.get('/simSwap/:id', C.handler(async (req, res) => {
    const t = await repos.simSwap.get(req.params.id);
    if (!t) throw new C.TmfError(404, 'Not Found', 'SimSwap not found');
    return res.json(C.project(represent(req, t), req.query.fields));
  }));

  router.post('/simSwap', C.handler(async (req, res) => {
    const body = C.requireObjectBody(req);
    C.rejectReadOnly(body, READ_ONLY);

    const from = supiOf(body, 'logicalResource');
    const to = supiOf(body, 'receiverLogicalResource');
    if (from === to)
      throw new C.TmfError(400, 'Bad Request',
          "'receiverLogicalResource.id' must differ from 'logicalResource.id'");
    if (body.reason !== undefined && typeof body.reason !== 'string')
      throw new C.TmfError(400, 'Bad Request', "'reason' must be a string");

    const key = req.get('Idempotency-Key');
    if (key !== undefined && !IDEMPOTENCY_KEY_RE.test(key))
      throw new C.TmfError(400, 'Bad Request',
          'Idempotency-Key must be 1-128 printable ASCII characters');

    /*
     * A repeated request returns the original outcome.  It has to be looked
     * up before anything else: after a completed swap the old SIM has no
     * bucket left, and the checks below would refuse the repeat.
     *
     * If the original was interrupted, the repeat is what finds it.  It is
     * undone first, so the caller is told it failed and nothing was moved,
     * not left reading 'inProgress' for ever.
     */
    const prior = async () => {
      const found = await repos.simSwap.list({ idempotencyKey: key }, 0, 1);
      if (!found.items.length) return null;
      const original = found.items[0];
      if (original.status !== 'inProgress' ||
          clock() - lockedAt(original) <= LOCK_STALE_MS)
        return original;
      await undoInterrupted(repos, original, clock());
      return (await repos.simSwap.get(original.id)) || original;
    };
    if (key) {
      const original = await prior();
      if (original) return res.status(200).json(represent(req, original));
    }

    /*
     * Record the task BEFORE touching a bucket, as a top-up does, so every
     * attempt leaves an audit record.
     *
     * The record is also the lock.  `locks` names both SIMs and is unique
     * across records, so while this swap runs no other swap can involve
     * either of them.  Without it two swaps of one SIM to different new
     * ones could each move some of its buckets.  It is removed when the
     * swap ends, whichever way it ends.
     */
    const now = clock();
    const task = {
      id: crypto.randomUUID(),
      status: 'inProgress',
      requestedDate: new Date(now).toISOString(),
      logicalResource: { id: from },
      receiverLogicalResource: { id: to },
      bucket: [],
      locks: [from, to],
      lockedAt: new Date(now).toISOString(),
    };
    if (body.reason) task.reason = body.reason;
    if (key) task.idempotencyKey = key;

    const finish = async fields => {
      await repos.simSwap.update({ id: task.id },
          { $set: fields, $unset: { locks: '', lockedAt: '' } });
      delete task.locks;
      delete task.lockedAt;
      return Object.assign(task, fields);
    };
    const fail = async (status, reason, message) => {
      await finish({ status: 'failed', statusReason: message });
      return new C.TmfError(status, reason, message);
    };

    for (let attempt = 0; ; attempt++) {
      try {
        await repos.simSwap.create(task);
        break;
      } catch (err) {
        if (!err || err.code !== 11000) throw err;
        if (key) {
          const original = await prior();
          if (original) return res.status(200).json(represent(req, original));
        }

        let holder = null;
        for (const supi of [from, to]) {
          const found = await repos.simSwap.list({ locks: supi }, 0, 1);
          if (found.items.length) { holder = found.items[0]; break; }
        }
        const stale = holder && now - lockedAt(holder) > LOCK_STALE_MS;
        if (attempt || (holder && !stale))
          throw new C.TmfError(409, 'Conflict',
              'Another swap involving one of these SIMs is in progress');

        /* The holder was interrupted.  Undo it, and start this swap from a
         * clean state - unless another request is undoing it already. */
        if (holder && !(await undoInterrupted(repos, holder, now)))
          throw new C.TmfError(409, 'Conflict',
              'Another swap involving one of these SIMs is in progress');
      }
    }

    /* A document written before buckets existed has no bucketId.  Resolving
     * it first writes one, so every move below matches on a unique id. */
    await getBucket(repos, from);

    const { items: mine } =
      await repos.bucket.list({ supi: from }, 0, MAX_BUCKETS);
    /* A SIM is in one pool at most. */
    const { items: pools } = await repos.bucket.list({ members: from }, 0, 1);
    if (!mine.length && !pools.length)
      throw await fail(404, 'Not Found',
          `No bucket or pool is provisioned for '${from}'`);

    /* Never merge.  Two plans cannot be added up without deciding whose
     * cycle and whose usage survive, and that is not this request's call.
     * Nor can a SIM be in two pools. */
    if (await repos.bucket.exists({ supi: to }))
      throw await fail(409, 'Conflict',
          `'${to}' already has a bucket; delete it first, or pick another SIM`);
    if (await repos.bucket.exists({ members: to }))
      throw await fail(409, 'Conflict',
          `'${to}' is already in a pool; take it out first, or pick another SIM`);

    const moves = mine.map(d => {
      const formerId = B.bucketId(d);
      return { id: movedId(formerId, from, to), formerId };
    });
    /* The pool keeps its id; only the member changes. */
    pools.forEach(d => {
      const id = B.bucketId(d);
      moves.push({ id, formerId: id, shared: true });
    });
    /* The plan goes into the record before the first move, so an
     * interrupted swap can be undone from its record alone. */
    await repos.simSwap.update({ id: task.id }, { $set: { bucket: moves } });
    task.bucket = moves;

    /*
     * Move the buckets one at a time.  Only supi and bucketId are written,
     * each with its own $set, so a usage report the CHF adds to the same
     * document while this runs is kept.
     *
     * There is no multi-document transaction on a standalone MongoDB, so
     * all-or-nothing is done by hand: each move is conditional on the
     * bucket still being the old SIM's, and if one fails - it was deleted,
     * or the new SIM was given a bucket in the meantime - the ones already
     * moved are put back.
     */
    const moved = [];
    let failure = null;
    try {
      for (const m of moves) {
        const matched = m.shared
          ? await repos.bucket.update({ bucketId: m.id, members: from },
              { $set: { 'members.$': to } })
          : await repos.bucket.update(
              { bucketId: m.formerId, supi: from },
              { $set: { supi: to, bucketId: m.id } });
        if (!matched) {
          failure = `Bucket '${m.formerId}' changed while the swap ran`;
          break;
        }
        moved.push(m);
      }
    } catch (err) {
      if (!err || err.code !== 11000) {
        // eslint-disable-next-line no-console
        console.error('[tmf] SIM swap could not move a bucket:',
            err && err.message ? err.message : err);
      }
      failure = err && err.code === 11000
          ? `'${to}' was given a bucket, or put in a pool, while the swap ran`
          : 'A bucket could not be moved';
    }

    if (failure) {
      const stuck = await putBack(repos, moved, from, to);
      if (stuck.length) {
        // eslint-disable-next-line no-console
        console.error(`[tmf] SIM swap ${task.id} left moved: ${stuck.join(', ')}`);
        throw await fail(500, 'Internal Server Error',
            `${failure}; could not put back: ${stuck.join(', ')}`);
      }
      throw await fail(409, 'Conflict', `${failure}; nothing was moved. Retry`);
    }

    await finish({ status: 'completed',
                   confirmationDate: new Date(clock()).toISOString() });

    /*
     * The old SIM's sessions are charged to buckets that are no longer
     * its own, so they may not continue: the same ABORT_CHARGING a deleted
     * bucket sends.  The new SIM is re-authorised in case it is already up
     * on the default quota, as a plan change would do.
     */
    await notifyChf(repos, from, null, 'ABORT_CHARGING', 'sim-swap');
    await notifyChf(repos, to, null, 'REAUTHORIZATION', 'sim-swap');

    return C.sendCreated(res, represent(req, task));
  }));

  /* A swap is an immutable record of something that happened. */
  router.all('/simSwap/:id', (req, res) => {
    res.set('Allow', 'GET');
    return C.sendError(res, 405, 'Method Not Allowed',
        'SimSwap records cannot be modified or deleted');
  });

  return router;
}

module.exports = { createSimSwapRouter, movedId, LOCK_STALE_MS };
