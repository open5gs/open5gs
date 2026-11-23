/*
 * TMF API against a REAL MongoDB, through the production mongooseRepos().
 *
 * Opt-in: runs only when CHF_TMF_MONGO_URI is set, e.g.
 *   CHF_TMF_MONGO_URI=mongodb://127.0.0.1:27999 npx jest tmf-api.mongo
 * It creates, and drops, its own uniquely named database.
 *
 * The in-memory suite proves the HTTP layer and the rules.  This one proves
 * what only the real driver can: that the conditional top-up write actually
 * matches a stored BSON Date and a missing periodStart, that unique indexes
 * reject duplicates, that Mongoose 5 update results are read correctly, and
 * that two concurrent requests with one Idempotency-Key credit once.  For a
 * SIM swap it proves that a bucket's id can be rewritten under the unique
 * index, and that the lock in the swap record really does keep two swaps of
 * one SIM apart.  For a pool it proves the unique index that keeps a SIM in
 * one pool, the compare-and-swap on the whole member list, the positional
 * update a SIM swap makes, and the $or that finds a SIM's pool.
 */
'use strict';

const URI = process.env.CHF_TMF_MONGO_URI;
const maybe = URI ? describe : describe.skip;

const GB = 1073741824;
const MONTH = 2592000;

maybe('TMF API on real MongoDB', () => {
  const express = require('express');
  const mongoose = require('mongoose');
  const { createTmfRouter, mongooseRepos } = require('../server/tmf');

  let server, base, dbName;
  const Sub = () => mongoose.model('ChfSubscriber');

  beforeAll(async () => {
    dbName = `chf_tmf_it_${process.pid}_${Date.now()}`;
    await mongoose.connect(`${URI}/${dbName}`, {
      useNewUrlParser: true, useUnifiedTopology: true });

    const repos = mongooseRepos();
    /* Indexes build asynchronously; unique-key behaviour needs them first. */
    await Promise.all(['ChfSubscriber', 'TmfIndividual', 'TmfBillingAccount',
      'TmfTopupBalance', 'TmfSimSwap', 'ChfSettings'].map(n => mongoose.model(n).init()));

    /* The router checks roles on the stored account, so the stand-in token
     * names a real one. */
    const admin = await mongoose.model('Account').create({ username: 'tmf-it-admin', roles: ['admin'] });

    const app = express();
    app.use(express.json());
    app.use((req, res, next) => {
      req.user = { _id: String(admin._id), username: 'tmf-it-admin', roles: ['admin'] };
      next();
    });
    app.use('/api/tmf-api', createTmfRouter(repos));
    await new Promise(r => { server = app.listen(0, r); });
    base = `http://127.0.0.1:${server.address().port}/api/tmf-api`;
  }, 30000);

  afterAll(async () => {
    if (server) await new Promise(r => server.close(r));
    if (mongoose.connection.db) await mongoose.connection.db.dropDatabase();
    await mongoose.disconnect();
  });

  async function call(method, path, body, headers = {}) {
    const res = await fetch(base + path, { method,
      headers: Object.assign(body === undefined ? {}
        : { 'Content-Type': 'application/json' }, headers),
      body: body === undefined ? undefined : JSON.stringify(body) });
    const text = await res.text();
    return { status: res.status, body: text ? JSON.parse(text) : null };
  }

  const BK = '/prepayBalanceManagement/v4/bucket';
  const T = '/prepayBalanceManagement/v4/topupBalance';
  const provision = n => call('POST', BK, {
    logicalResource: [{ id: `imsi-99970000000${String(n).padStart(4, '0')}` }],
    allowance: { amount: 10, units: 'GB' }, validitySeconds: MONTH });
  const supi = n => `imsi-99970000000${String(n).padStart(4, '0')}`;

  test('M1. provisioning stores the shape the CHF reads; duplicate SUPI is 409', async () => {
    expect((await provision(1)).status).toBe(201);
    const doc = await Sub().findOne({ supi: supi(1) }).lean();
    expect(doc.quota).toMatchObject({ totalQuotaBytes: 10 * GB,
      validitySeconds: MONTH, autoRenew: true });
    expect((await provision(1)).status).toBe(409);
  });

  test('M2. top-up on a never-used bucket: null filter matches a MISSING periodStart', async () => {
    await provision(2);
    const res = await call('POST', T, { bucket: { id: supi(2) },
      amount: { amount: 1, units: 'GB' } });
    expect(res.status).toBe(201);
    expect(res.body.reactivated).toBe(true);
    const doc = await Sub().findOne({ supi: supi(2) }).lean();
    expect(doc.usage.topupBytes).toBe(GB);
    expect(doc.usage.periodStart instanceof Date).toBe(true);
  });

  test('M3. top-up on an active cycle: conditional write matches the stored BSON Date', async () => {
    await provision(3);
    /* Write the cycle as the CHF does: a BSON date and a counter. */
    await Sub().updateOne({ supi: supi(3) }, { $set: {
      'usage.periodStart': new Date(Date.now() - 3600 * 1000),
      'usage.consumedBytes': 4 * GB, 'usage.topupBytes': 0 } });

    const res = await call('POST', T, { bucket: { id: supi(3) },
      amount: { amount: 2, units: 'GB' } });
    /* 'conflict' here would mean the Date equality never matches in real Mongo. */
    expect(res.status).toBe(201);
    expect(res.body).toMatchObject({ status: 'completed', reactivated: false });

    const doc = await Sub().findOne({ supi: supi(3) }).lean();
    expect(doc.usage.consumedBytes).toBe(4 * GB);
    expect(doc.usage.topupBytes).toBe(2 * GB);
    const got = await call('GET', `${BK}/${supi(3)}`);
    expect(got.body.remainingValue.amount).toBe(8 * GB);
  });

  test('M4. PATCH leaves the CHF counter exactly as it was', async () => {
    await provision(4);
    const periodStart = new Date(Date.now() - 7200 * 1000);
    await Sub().updateOne({ supi: supi(4) }, { $set: {
      'usage.periodStart': periodStart, 'usage.consumedBytes': 3 * GB,
      'usage.topupBytes': GB } });
    const before = (await Sub().findOne({ supi: supi(4) }).lean()).usage;

    const res = await call('PATCH', `${BK}/${supi(4)}`, {
      allowance: { amount: 20, units: 'GB' }, finalUnitAction: 'REDIRECT' });
    expect(res.status).toBe(200);
    const after = (await Sub().findOne({ supi: supi(4) }).lean()).usage;
    expect(after).toEqual(before);
  });

  test('M5. concurrent requests with one Idempotency-Key credit ONCE', async () => {
    await provision(5);
    await Sub().updateOne({ supi: supi(5) }, { $set: {
      'usage.periodStart': new Date(Date.now() - 60 * 1000),
      'usage.consumedBytes': 0, 'usage.topupBytes': 0 } });

    const h = { 'Idempotency-Key': `it-${Date.now()}` };
    const body = { bucket: { id: supi(5) }, amount: { amount: 1, units: 'GB' } };
    const results = await Promise.all(
      Array.from({ length: 5 }, () => call('POST', T, body, h)));

    const ids = new Set(results.map(r => r.body.id));
    expect(ids.size).toBe(1);
    expect(results.filter(r => r.status === 201)).toHaveLength(1);
    const doc = await Sub().findOne({ supi: supi(5) }).lean();
    expect(doc.usage.topupBytes).toBe(GB);
  });

  test('M6. party/account PATCH (findOneAndReplace) and the array delete guard', async () => {
    const a = (await call('POST', '/partyManagement/v4/individual', { givenName: 'A' })).body;
    const b = (await call('POST', '/partyManagement/v4/individual', { givenName: 'B' })).body;
    const acct = await call('POST', '/accountManagement/v4/billingAccount',
      { name: 'Shared', relatedParty: [{ id: a.id }, { id: b.id }] });
    expect(acct.status).toBe(201);

    const renamed = await call('PATCH',
      `/accountManagement/v4/billingAccount/${acct.body.id}`, { name: 'Renamed' });
    expect(renamed.status).toBe(200);
    expect(renamed.body.name).toBe('Renamed');
    expect(renamed.body.relatedParty.map(p => p.id)).toEqual([a.id, b.id]);

    /* b is the SECOND element of relatedParty. */
    expect((await call('DELETE', `/partyManagement/v4/individual/${b.id}`)).status)
      .toBe(409);
  });

  const W = '/prepayBalanceManagement/v4/simSwap';
  const swap = (a, b, headers) => call('POST', W, {
    logicalResource: { id: supi(a) },
    receiverLogicalResource: { id: supi(b) } }, headers);
  const scoped = n => call('POST', BK, {
    logicalResource: [{ id: supi(n) }], allowance: { amount: 2, units: 'GB' },
    scope: { dnn: 'enterprise' }, priority: 50 });
  const owned = async n => (await Sub().find({ supi: supi(n) }).lean())
    .map(d => d.bucketId).sort();

  test('M7. SIM swap: ids are rewritten under the unique index, and the counter comes along', async () => {
    await provision(20);
    const extra = (await scoped(20)).body;
    const periodStart = new Date(Date.now() - 3600 * 1000);
    await Sub().updateOne({ bucketId: supi(20) }, { $set: {
      'usage.periodStart': periodStart, 'usage.consumedBytes': 4 * GB,
      'usage.topupBytes': GB } });

    const res = await swap(20, 21);
    expect(res.status).toBe(201);
    expect(res.body.status).toBe('completed');

    const suffix = extra.id.slice(supi(20).length);
    expect(await owned(20)).toEqual([]);
    expect(await owned(21)).toEqual([supi(21), supi(21) + suffix].sort());
    const doc = await Sub().findOne({ bucketId: supi(21) }).lean();
    expect(doc.usage.consumedBytes).toBe(4 * GB);
    expect(doc.usage.topupBytes).toBe(GB);
    /* Still the BSON Date the CHF wrote, to the millisecond. */
    expect(doc.usage.periodStart instanceof Date).toBe(true);
    expect(doc.usage.periodStart.getTime()).toBe(periodStart.getTime());
    const got = await call('GET', `${BK}/${supi(21)}`);
    expect(got.body.remainingValue.amount).toBe(7 * GB);

    /* The record keeps the plan, and gives up its lock. */
    const rec = await mongoose.model('TmfSimSwap')
      .findOne({ id: res.body.id }).lean();
    expect(rec.bucket).toHaveLength(2);
    expect(rec.locks).toBeUndefined();
  });

  test('M8. SIM swap: a new SIM that already has a bucket is refused, and nothing moves', async () => {
    await provision(22);
    await provision(23);
    const res = await swap(22, 23);
    expect(res.status).toBe(409);
    expect(await owned(22)).toEqual([supi(22)]);
    expect(await owned(23)).toEqual([supi(23)]);
  });

  test('M9. SIM swap: concurrent requests with one Idempotency-Key move ONCE', async () => {
    await provision(24);
    await scoped(24);
    const h = { 'Idempotency-Key': `swap-${Date.now()}` };
    const results = await Promise.all(
      Array.from({ length: 5 }, () => swap(24, 25, h)));

    expect(new Set(results.map(r => r.body.id)).size).toBe(1);
    expect(results.filter(r => r.status === 201)).toHaveLength(1);
    expect(results.filter(r => r.status === 200)).toHaveLength(4);
    expect(await owned(24)).toEqual([]);
    expect(await owned(25)).toHaveLength(2);
  });

  /*
   * This one checks the outcome, and it passes without the lock almost every
   * time: the interleaving that splits the buckets is a few milliseconds
   * wide.  M12 sets that interleaving up on purpose.
   */
  test('M10. SIM swap: two swaps of one SIM to different new ones - one takes everything', async () => {
    for (let round = 0; round < 10; round++) {
      const a = 30 + round * 3;
      await provision(a);
      await scoped(a);
      const results = await Promise.all([swap(a, a + 1), swap(a, a + 2)]);

      /* The loser met the lock (409), or ran after the winner (404). */
      expect(results.map(r => r.status).sort()[0]).toBe(201);
      expect(results.filter(r => r.status === 201)).toHaveLength(1);
      expect([404, 409]).toContain(results.find(r => r.status !== 201).status);

      const counts = [(await owned(a)).length, (await owned(a + 1)).length,
        (await owned(a + 2)).length];
      /* Never split: nothing left behind, and both buckets on one SIM. */
      expect(counts[0]).toBe(0);
      expect(counts.slice(1).sort()).toEqual([0, 2]);
    }
  });

  test('M11. SIM swap: a finished swap leaves both SIMs free for the next', async () => {
    await provision(70);
    expect((await swap(70, 71)).status).toBe(201);
    /* 71 was the new SIM of one swap and is the old SIM of the next. */
    expect((await swap(71, 72)).status).toBe(201);
    expect(await owned(72)).toEqual([supi(72)]);
    const held = await mongoose.model('TmfSimSwap')
      .countDocuments({ locks: { $exists: true } });
    expect(held).toBe(0);
  });

  test('M12. SIM swap: a swap that is still running blocks another that shares a SIM', async () => {
    await provision(80);
    const extra = (await scoped(80)).body;
    /*
     * The state between the two writes of a swap from 80 to 81: its record
     * is in progress and holds both SIMs, and the first bucket has moved.
     * A second swap of 80 would otherwise see one bucket left, move it to
     * 82 and report success, with the plan split across two SIMs.
     */
    await mongoose.model('TmfSimSwap').create({ id: `running-${Date.now()}`,
      status: 'inProgress', requestedDate: new Date().toISOString(),
      logicalResource: { id: supi(80) },
      receiverLogicalResource: { id: supi(81) },
      bucket: [{ id: supi(81), formerId: supi(80) }],
      locks: [supi(80), supi(81)] });
    await Sub().updateOne({ bucketId: supi(80) },
        { $set: { supi: supi(81), bucketId: supi(81) } });

    const res = await swap(80, 82);
    expect(res.status).toBe(409);
    expect(res.body.message).toMatch(/in progress/);
    expect(await owned(80)).toEqual([extra.id]);
    expect(await owned(82)).toEqual([]);
    /* And 81, the running swap's new SIM, cannot be swapped away either. */
    expect((await swap(81, 83)).status).toBe(409);
    expect(await owned(81)).toEqual([supi(81)]);
  });

  test('M13. SIM swap: a retry with the key of an interrupted swap undoes it', async () => {
    await provision(84);
    const extra = (await scoped(84)).body;
    /*
     * As M12, but ten minutes old: the server stopped mid-swap.  The record
     * has no lockedAt, as one written by hand would not, so the claim has
     * to match a field that is missing.
     */
    const id = `stopped-${Date.now()}`;
    await mongoose.model('TmfSimSwap').create({ id, status: 'inProgress',
      requestedDate: new Date(Date.now() - 600000).toISOString(),
      idempotencyKey: id,
      logicalResource: { id: supi(84) },
      receiverLogicalResource: { id: supi(85) },
      bucket: [{ id: supi(85), formerId: supi(84) }],
      locks: [supi(84), supi(85)] });
    await Sub().updateOne({ bucketId: supi(84) },
        { $set: { supi: supi(85), bucketId: supi(85) } });

    const retries = await Promise.all([1, 2, 3].map(() =>
        swap(84, 85, { 'Idempotency-Key': id })));
    expect(retries.map(r => r.status)).toEqual([200, 200, 200]);

    const record = await mongoose.model('TmfSimSwap').findOne({ id }).lean();
    expect(record.status).toBe('failed');
    expect(record.statusReason).toMatch(/put back/);
    expect(record).not.toHaveProperty('locks');
    expect(record).not.toHaveProperty('lockedAt');
    expect(await owned(84)).toEqual([supi(84), extra.id].sort());
    expect(await owned(85)).toEqual([]);

    /* Asked for again under a new key, it goes through. */
    const res = await swap(84, 85, { 'Idempotency-Key': `${id}-again` });
    expect(res.status).toBe(201);
    expect(await owned(84)).toEqual([]);
  });

  /* ---------------- Pools ---------------- */
  const pool = ns => call('POST', BK, {
    logicalResource: ns.map(n => ({ id: supi(n) })),
    allowance: { amount: 20, units: 'GB' } });
  const members = async id => (await Sub().findOne({ bucketId: id }).lean()).members;

  test('M14. pools: one per SIM, enforced by the real unique index', async () => {
    const first = await pool([9001, 9002]);
    expect(first.status).toBe(201);
    const doc = await Sub().findOne({ bucketId: first.body.id }).lean();
    expect(doc.members).toEqual([supi(9001), supi(9002)]);
    expect(doc).not.toHaveProperty('supi');

    const second = await pool([9003, 9002]);
    expect(second.status).toBe(409);
    expect(second.body.message).toMatch(new RegExp(`'${supi(9002)}' is already in pool`));
    expect(await Sub().countDocuments({ members: supi(9003) })).toBe(0);

    /* Buckets that are not pools have no members field, so the sparse
     * unique index leaves every one of them alone. */
    expect((await provision(9004)).status).toBe(201);
    expect((await provision(9005)).status).toBe(201);
    expect(await Sub().countDocuments({ members: { $exists: true } })).toBe(1);

    /* The pool, and the member's own bucket, both found by the member. */
    expect((await provision(9001)).status).toBe(201);
    const found = await call('GET', `${BK}?logicalResource.id=${supi(9001)}`);
    expect(found.body.map(b => b.id).sort())
      .toEqual([first.body.id, supi(9001)].sort());
  });

  test('M15. a member change is conditional on the list it read, on real MongoDB', async () => {
    const p = (await pool([9011, 9012])).body;
    /* The write the PATCH makes, against a stale list and a current one:
     * MongoDB matches an array value only if it is equal, in order. */
    const repo = mongooseRepos().bucket;
    const stale = [supi(9011)];
    const current = [supi(9011), supi(9012)];
    expect(await repo.update({ bucketId: p.id, members: stale },
        { $set: { members: [supi(9011), supi(9019)] } })).toBe(0);
    expect(await repo.update({ bucketId: p.id, members: current.slice().reverse() },
        { $set: { members: [supi(9011), supi(9019)] } })).toBe(0);
    expect(await members(p.id)).toEqual(current);

    /* Through the API, concurrently: whatever the interleaving, the list
     * held is one a successful request reported. */
    for (let round = 0; round < 5; round++) {
      const before = await members(p.id);
      const results = await Promise.all([9013 + 2 * round, 9014 + 2 * round]
        .map(n => call('PATCH', `${BK}/${p.id}`, {
          logicalResource: before.slice(0, 1).concat([supi(n)])
            .map(id => ({ id })) })));
      const codes = results.map(r => r.status).sort();
      /* Both may also succeed one after the other, if the first finished
       * before the second read; what may not happen is a lost update. */
      expect(codes.every(c => c === 200 || c === 409)).toBe(true);
      const after = await members(p.id);
      const winners = results.filter(r => r.status === 200)
        .map(r => r.body.logicalResource.map(x => x.id));
      expect(winners).toContainEqual(after);
    }
  });

  test('M16. a SIM swap puts the new SIM in the old one\'s place in the pool', async () => {
    const p = (await pool([9031, 9032, 9033])).body;
    const res = await swap(9032, 9039);
    expect(res.status).toBe(201);
    expect(await members(p.id)).toEqual([supi(9031), supi(9039), supi(9033)]);
    /* And one whose new SIM is in a pool already is refused. */
    await pool([9041, 9042]);
    expect((await swap(9031, 9042)).status).toBe(409);
    expect(await members(p.id)).toEqual([supi(9031), supi(9039), supi(9033)]);
  });

  test('M17. the limit the CHF publishes in chf.settings is the one enforced', async () => {
    await mongoose.model('ChfSettings').updateOne({ _id: 'pool' },
        { $set: { maxMembers: 2 } }, { upsert: true });
    try {
      const three = await pool([9051, 9052, 9053]);
      expect(three.status).toBe(400);
      expect(three.body.message).toMatch(/at most 2 SIMs/);
      expect((await pool([9051, 9052])).status).toBe(201);
    } finally {
      await mongoose.model('ChfSettings').deleteOne({ _id: 'pool' });
    }
  });
});
