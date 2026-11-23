/*
 * TMF654 Prepay Balance Management API v4 — Bucket and TopupBalance.
 *
 *   GET    /prepayBalanceManagement/v4/bucket[?fields&offset&limit&id=&partyAccount.id=]
 *   GET    /prepayBalanceManagement/v4/bucket/{id}
 *   POST   /prepayBalanceManagement/v4/bucket            provision a SUPI, or a pool
 *   PATCH  /prepayBalanceManagement/v4/bucket/{id}       change the plan, or a pool's members
 *   DELETE /prepayBalanceManagement/v4/bucket/{id}       deprovision
 *
 *   GET    /prepayBalanceManagement/v4/topupBalance[?bucket.id=&status=]
 *   GET    /prepayBalanceManagement/v4/topupBalance/{id}
 *   POST   /prepayBalanceManagement/v4/topupBalance      add data to a bucket
 *
 * Buckets are stored as CHF subscriber documents (chf.subscribers) - the
 * same documents the CHF NF enforces against - so a change made here takes
 * effect on the subscriber's next usage report.  See bucket-map.js for the
 * rules that keep the published balance equal to what the CHF grants.
 *
 * A pool is a bucket with several SIMs in logicalResource: one plan and
 * one counter that all of them draw on.  A SIM is in one pool at most, a
 * pool has at most charging.pool.max_members SIMs (chf.yaml, published by
 * the CHF in chf.settings), and a SIM taken out of a pool has its sessions
 * on it released at once.
 *
 * Copyright (C) 2026 by Rami Mohamed <ramrode@gmail.com>
 */

'use strict';

const express = require('express');
const crypto = require('crypto');

/* A SUPI, a SUPI with the suffix a scoped bucket is given, or a pool. */
const BUCKET_ID_RE = /^(imsi-\d{15}(-[0-9a-f]{12})?|pool-[0-9a-f]{12})$/;
const C = require('./common');
const B = require('./bucket-map');

const BUCKET_FILTERABLE = {
  id: 'bucketId',
  /* A subscriber's buckets, and its pool: ?logicalResource.id=imsi-<15
   * digits>.  Rewritten below to match either. */
  'logicalResource.id': 'supi',
  'partyAccount.id': 'billingAccountId',
  'scope.dnn': 'scope.dnn',
  'scope.roaming': 'scope.roaming',
  'scope.sNSSAI.sst': 'scope.snssai.sst',
  'partyAccount.id': 'billingAccountId',
};

const TOPUP_FILTERABLE = {
  'bucket.id': 'bucket.id',
  status: 'status',
};

const TOPUP_READ_ONLY = ['id', 'href', '@type', '@baseType', 'status',
  'requestedDate', 'confirmationDate', 'reactivated'];

/* Idempotency-Key values are opaque to us, but bound them. */
const IDEMPOTENCY_KEY_RE = /^[\x21-\x7e]{1,128}$/;

async function assertAccountExists(repos, id) {
  if (!await repos.billingAccount.get(id))
    throw new C.TmfError(400, 'Bad Request',
        `partyAccount '${id}' does not exist`);
}

/* {'quota.x': v} -> {quota: {x: v}} for creating a document. */
function nest(setMap) {
  const out = {};
  Object.keys(setMap).forEach(path => {
    const [head, tail] = path.split('.');
    out[head] = out[head] || {};
    out[head][tail] = setMap[path];
  });
  return out;
}

function representTopup(req, t) {
  const out = {
    id: t.id,
    href: C.hrefFor(req, 'topupBalance', t.id),
    '@type': 'TopupBalance',
    status: t.status,
    requestedDate: t.requestedDate,
    amount: { amount: t.amount, units: 'octet' },
    bucket: { id: t.bucket.id, href: C.hrefFor(req, 'bucket', t.bucket.id),
              '@referredType': 'Bucket' },
  };
  if (t.confirmationDate) out.confirmationDate = t.confirmationDate;
  if (t.reason) out.reason = t.reason;
  if (t.statusReason) out.statusReason = t.statusReason;
  if (t.reactivated !== undefined) out.reactivated = t.reactivated;
  return out;
}

/*
 * @param repos.bucket          keyed by 'bucketId' (chf.subscribers)
 * @param repos.billingAccount  to validate partyAccount references
 * @param repos.topupBalance    keyed by 'id' (the top-up audit trail)
 * @param clock                 () => epoch ms; injectable for tests
 */
/*
 * A bucket by id.
 *
 * A document written before buckets existed has no bucketId: it is the
 * subscriber's catch-all and is addressed by SUPI.  The first time one is
 * touched its id is written, so every later read and write goes through
 * the same field and no other code needs the special case.
 */
async function getBucket(repos, id) {
  const direct = await repos.bucket.get(id);
  if (direct) return direct;

  const { items } = await repos.bucket.list({ supi: id }, 0, 2);
  const legacy = items.filter(d => !d.bucketId);
  if (!legacy.length) return null;

  await repos.bucket.update({ supi: id }, { $set: { bucketId: id } });
  return Object.assign({}, legacy[0], { bucketId: id });
}

/*
 * Tell the CHF that a live session should hear about this now.
 *
 * The CHF polls chf.notifications and turns each row into a TS 32.291
 * ChargingNotify to the sessions drawing from the bucket, so a top-up or a
 * bar reaches a session that is up instead of waiting for its next usage
 * report - up to five minutes with the shipped grant caps, and the full
 * five for a session that is idle.
 *
 * Best-effort on purpose.  The balance change is already committed and is
 * what actually counts; the notification only decides whether it lands
 * now or at the next report.  A failure here must not turn a successful
 * top-up into an error for the caller, so it is logged and swallowed.
 */
async function notifyChf(repos, supi, bucketId, type, reason) {
  if (!repos.notification) return;   /* not wired (tests, older deploys) */
  try {
    const row = { id: crypto.randomUUID(), supi, type,
                  createdAt: new Date() };
    if (bucketId && bucketId !== supi) row.bucketId = bucketId;
    if (reason) row.reason = reason;
    await repos.notification.create(row);
  } catch (err) {
    // eslint-disable-next-line no-console
    console.error('[tmf] could not queue a ChargingNotify:',
        err && err.message ? err.message : err);
  }
}

/*
 * Tell every SIM that draws on a bucket.  For a pool that is one row per
 * member, each naming the pool, so only their sessions on the pool hear.
 */
async function notifyBucket(repos, doc, type, reason) {
  for (const supi of B.membersOf(doc))
    await notifyChf(repos, supi, B.bucketId(doc), type, reason);
}

/*
 * How many SIMs a pool may have: what the CHF published from chf.yaml, or
 * the shipped default when no CHF has.
 */
async function poolMaxMembers(repos) {
  try {
    const s = repos.settings && await repos.settings.get('pool');
    const n = s && Number(s.maxMembers);
    if (Number.isInteger(n) && n > 0) return n;
  } catch (err) {
    // eslint-disable-next-line no-console
    console.error('[tmf] cannot read chf.settings:',
        err && err.message ? err.message : err);
  }
  return B.DEFAULT_POOL_MAX_MEMBERS;
}

async function assertPoolSize(repos, supis) {
  const max = await poolMaxMembers(repos);
  if (supis.length > max)
    throw new C.TmfError(400, 'Bad Request',
        `A pool can have at most ${max} SIMs (charging.pool.max_members)`);
}

/* After a duplicate key on members: say which SIM is already in a pool. */
async function poolConflict(repos, supis, ownId) {
  for (const supi of supis) {
    const { items } = await repos.bucket.list({ members: supi }, 0, 2);
    const other = items.find(d => B.bucketId(d) !== ownId);
    if (other)
      return new C.TmfError(409, 'Conflict',
          `'${supi}' is already in pool '${B.bucketId(other)}'; a SIM can be in one pool only`);
  }
  return new C.TmfError(409, 'Conflict', 'Resource already exists');
}

function randomSuffix() {
  return crypto.randomBytes(6).toString('hex');
}

function createBalanceRouter(repos, clock = Date.now) {
  const router = express.Router();

  /* ---------------- Bucket ---------------- */

  router.get('/bucket', C.handler(async (req, res) => {
    const { offset, limit } = C.paging(req.query);
    const filter = C.buildFilter(req.query, BUCKET_FILTERABLE);
    if (filter.supi !== undefined) {
      filter.$or = [{ supi: filter.supi }, { members: filter.supi }];
      delete filter.supi;
    }
    const now = clock();
    const { items, total } = await repos.bucket.list(filter, offset, limit);
    return C.sendList(res, items.map(d =>
        C.project(B.toBucket(req, d, now), req.query.fields)), total);
  }));

  router.get('/bucket/:id', C.handler(async (req, res) => {
    const doc = await getBucket(repos, req.params.id);
    if (!doc) throw new C.TmfError(404, 'Not Found', 'Bucket not found');
    return res.json(C.project(B.toBucket(req, doc, clock()), req.query.fields));
  }));

  router.post('/bucket', C.handler(async (req, res) => {
    const body = C.requireObjectBody(req);
    C.rejectReadOnly(body, B.READ_ONLY);

    const supis = B.supisFromLogicalResource(body.logicalResource);
    const scope = B.scopeToStored(body.scope);
    const doc = nest(B.planToQuotaSet(body, null));

    if (supis.length > 1) {
      /* A pool.  Its own id, since no one SUPI owns it. */
      await assertPoolSize(repos, supis);
      doc.members = supis;
      doc.bucketId = `pool-${randomSuffix()}`;
    } else {
      /*
       * The catch-all keeps the SUPI as its id, so a subscriber with one
       * plan is addressed exactly as before; a scoped bucket gets its own.
       * The unique index on bucketId is what stops a second catch-all.
       */
      doc.supi = supis[0];
      doc.bucketId = scope ? `${supis[0]}-${randomSuffix()}` : supis[0];
    }
    if (scope) doc.scope = scope;
    doc.priority = B.priorityOf(body.priority);

    if (body.partyAccount !== undefined && body.partyAccount !== null) {
      doc.billingAccountId = B.partyAccountId(body.partyAccount);
      await assertAccountExists(repos, doc.billingAccountId);
    }

    /* A duplicate SUPI surfaces as E11000 and is mapped to 409 by handler.
     * For a pool it is a SIM that is in another pool already. */
    let created;
    try {
      created = await repos.bucket.create(doc);
    } catch (err) {
      if (err && err.code === 11000 && doc.members)
        throw await poolConflict(repos, doc.members, doc.bucketId);
      throw err;
    }
    /* A SIM with a session up on the default quota moves onto a new pool
     * at its next report; ask for that report now. */
    if (doc.members)
      for (const supi of doc.members)
        await notifyChf(repos, supi, null, 'REAUTHORIZATION', 'pool-joined');
    return C.sendCreated(res, B.toBucket(req, created, clock()));
  }));

  router.patch('/bucket/:id', C.handler(async (req, res) => {
    const patch = C.requireObjectBody(req);
    C.rejectReadOnly(patch, B.READ_ONLY);
    const unknown = Object.keys(patch).filter(k => B.PATCHABLE.indexOf(k) === -1);
    if (unknown.length)
      throw new C.TmfError(400, 'Bad Request',
          `Attribute(s) cannot be patched: ${unknown.join(', ')}`);

    const current = await getBucket(repos, req.params.id);
    if (!current) throw new C.TmfError(404, 'Not Found', 'Bucket not found');

    /* A pool's members.  The list sent replaces the list held, as a JSON
     * Merge Patch replaces an array; a SIM's own bucket keeps its SUPI. */
    let members = null;
    if (patch.logicalResource !== undefined) {
      if (!B.isPool(current))
        throw new C.TmfError(400, 'Bad Request',
            "'logicalResource' of a SIM's own bucket cannot change; " +
            'create a pool to share an allowance');
      members = B.supisFromLogicalResource(patch.logicalResource);
      await assertPoolSize(repos, members);
    }

    /*
     * Only quota.* and billingAccountId are ever written here - never
     * usage.*, which is the CHF's live counter.  Each path is $set on its
     * own rather than replacing the document, so a usage report the CHF
     * records while this request runs is not lost.
     */
    const update = { $set: B.planToQuotaSet(patch, current.quota || {}) };

    /* Scope and priority decide which sessions this bucket serves; they
     * are written whole, so clearing a dimension means sending the scope
     * without it. */
    if (patch.scope !== undefined) {
      const scope = B.scopeToStored(patch.scope);
      if (scope) update.$set.scope = scope;
      else update.$unset = Object.assign(update.$unset || {}, { scope: '' });
    }
    if (patch.priority !== undefined)
      update.$set.priority = B.priorityOf(patch.priority);

    if (patch.partyAccount === null) {
      update.$unset = Object.assign(update.$unset || {},
          { billingAccountId: '' });
    } else if (patch.partyAccount !== undefined) {
      const id = B.partyAccountId(patch.partyAccount);
      await assertAccountExists(repos, id);
      update.$set.billingAccountId = id;
    }

    const planChanged = !!(update.$set && Object.keys(update.$set).length) ||
        !!update.$unset;
    const filter = { bucketId: B.bucketId(current) };
    if (members) {
      update.$set.members = members;
      /* Conditional on the members this request read, so two changes made
       * at once cannot each undo the other's. */
      filter.members = current.members;
    }

    if (!Object.keys(update.$set).length) delete update.$set;
    if (update.$set || update.$unset) {
      let matched;
      try {
        matched = await repos.bucket.update(filter, update);
      } catch (err) {
        if (err && err.code === 11000 && members)
          throw await poolConflict(repos, members, B.bucketId(current));
        throw err;
      }
      if (!matched) {
        if (members && await getBucket(repos, req.params.id))
          throw new C.TmfError(409, 'Conflict',
              "The pool's members changed while this request ran; retry");
        throw new C.TmfError(404, 'Not Found', 'Bucket not found');
      }
    }
    const saved = await getBucket(repos, req.params.id);

    if (members) {
      const before = current.members;
      const removed = before.filter(m => members.indexOf(m) === -1);
      const added = members.filter(m => before.indexOf(m) === -1);
      /* Taken out: their sessions on this pool are released at once. */
      for (const supi of removed)
        await notifyChf(repos, supi, B.bucketId(current),
            'ABORT_CHARGING', 'pool-member-removed');
      /* Put in: a session up on the default quota moves onto the pool at
       * its next report, so ask for that report now.  Every one of the
       * SIM's sessions, since none is on the pool yet. */
      for (const supi of added)
        await notifyChf(repos, supi, null,
            'REAUTHORIZATION', 'pool-member-added');
      /* The members who stayed only need to hear if the plan moved. */
      if (planChanged)
        for (const supi of members.filter(m => added.indexOf(m) === -1))
          await notifyChf(repos, supi, B.bucketId(current),
              'REAUTHORIZATION', 'plan-change');
    } else {
      /* The plan moved: more quota, less, or a different scope.  Either
       * way a session on this bucket should be re-authorised against it. */
      await notifyBucket(repos, current, 'REAUTHORIZATION', 'plan-change');
    }
    return res.json(B.toBucket(req, saved, clock()));
  }));

  router.delete('/bucket/:id', C.handler(async (req, res) => {
    /* Read it first: once it is gone there is nothing left to say whose
     * sessions this was, and the CHF matches on SUPI. */
    const doomed = await getBucket(repos, req.params.id);
    if (!await repos.bucket.remove(req.params.id))
      throw new C.TmfError(404, 'Not Found', 'Bucket not found');
    if (doomed)
      await notifyBucket(repos, doomed, 'ABORT_CHARGING', 'deprovisioned');
    return res.status(204).end();
  }));

  /* ---------------- TopupBalance ---------------- */

  router.get('/topupBalance', C.handler(async (req, res) => {
    const { offset, limit } = C.paging(req.query);
    const filter = C.buildFilter(req.query, TOPUP_FILTERABLE);
    const { items, total } =
      await repos.topupBalance.list(filter, offset, limit);
    return C.sendList(res, items.map(t =>
        C.project(representTopup(req, t), req.query.fields)), total);
  }));

  router.get('/topupBalance/:id', C.handler(async (req, res) => {
    const t = await repos.topupBalance.get(req.params.id);
    if (!t) throw new C.TmfError(404, 'Not Found', 'TopupBalance not found');
    return res.json(C.project(representTopup(req, t), req.query.fields));
  }));

  router.post('/topupBalance', C.handler(async (req, res) => {
    const body = C.requireObjectBody(req);
    C.rejectReadOnly(body, TOPUP_READ_ONLY);

    /* The target is a bucket id: a SUPI for a subscriber's catch-all, the
     * id of one of its scoped buckets, or a pool's. */
    const supi = C.isPlainObject(body.bucket) && body.bucket.id;
    if (typeof supi !== 'string' || !BUCKET_ID_RE.test(supi))
      throw new C.TmfError(400, 'Bad Request',
          "'bucket.id' must be a bucket id (a SUPI, a scoped bucket's id, or a pool's)");
    const bytes = B.quantityToOctets(body.amount, 'amount', { allowZero: false });
    if (body.reason !== undefined && typeof body.reason !== 'string')
      throw new C.TmfError(400, 'Bad Request', "'reason' must be a string");

    /* Resolve first, so a document written before buckets existed gets its
     * id written and the conditional update below has something unique to
     * match on. */
    await getBucket(repos, supi);

    const key = req.get('Idempotency-Key');
    if (key !== undefined && !IDEMPOTENCY_KEY_RE.test(key))
      throw new C.TmfError(400, 'Bad Request',
          'Idempotency-Key must be 1-128 printable ASCII characters');

    /* A repeated request returns the original outcome, not a second credit. */
    if (key) {
      const prior = await repos.topupBalance.list({ idempotencyKey: key }, 0, 1);
      if (prior.items.length)
        return res.status(200).json(representTopup(req, prior.items[0]));
    }

    const target = await repos.bucket.get(supi);
    if (!target)
      throw new C.TmfError(404, 'Not Found', 'Bucket not found');

    /*
     * Record the task BEFORE touching the balance, so every attempt leaves
     * an audit record even if a later step fails.  With an Idempotency-Key
     * this insert is also the lock: the key is unique, so a concurrent
     * duplicate collides here and returns the original task instead of
     * crediting the subscriber twice.
     */
    const now = clock();
    const task = {
      id: crypto.randomUUID(),
      status: 'inProgress',
      requestedDate: new Date(now).toISOString(),
      amount: bytes,
      bucket: { id: supi },
    };
    if (body.reason) task.reason = body.reason;
    if (key) task.idempotencyKey = key;

    try {
      await repos.topupBalance.create(task);
    } catch (err) {
      if (key && err && err.code === 11000) {
        const prior = await repos.topupBalance.list({ idempotencyKey: key }, 0, 1);
        if (prior.items.length)
          return res.status(200).json(representTopup(req, prior.items[0]));
      }
      throw err;
    }

    const result = await B.applyTopup(repos.bucket, supi, bytes, now);
    const finish = async fields => {
      await repos.topupBalance.update({ id: task.id }, { $set: fields });
      return Object.assign(task, fields);
    };

    if (result.outcome === 'applied') {
      await finish({ status: 'completed', reactivated: result.reactivated,
                     confirmationDate: new Date(clock()).toISOString() });
      /* The subscriber has data again - reach any session that is up,
       * including one already stopped on a FinalUnitIndication.  For a
       * pool, every member's. */
      await notifyBucket(repos, target, 'REAUTHORIZATION', 'topup');
      return C.sendCreated(res, representTopup(req, task));
    }

    if (result.outcome === 'notFound') {
      await finish({ status: 'failed', statusReason: 'Bucket not found' });
      throw new C.TmfError(404, 'Not Found', 'Bucket not found');
    }

    await finish({ status: 'failed',
                   statusReason: 'Billing cycle changed concurrently' });
    throw new C.TmfError(409, 'Conflict',
        'The billing cycle kept changing while applying the top-up; retry');
  }));

  /* A top-up is an immutable record of something that happened. */
  router.all('/topupBalance/:id', (req, res) => {
    res.set('Allow', 'GET');
    return C.sendError(res, 405, 'Method Not Allowed',
        'TopupBalance records cannot be modified or deleted');
  });

  return router;
}

module.exports = { createBalanceRouter, getBucket, notifyChf, poolMaxMembers };
