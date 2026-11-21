/*
 * The ChfSubscriber REST resource against a REAL MongoDB.
 *
 * Opt-in: runs only when CHF_TMF_MONGO_URI is set, e.g.
 *   CHF_TMF_MONGO_URI=mongodb://127.0.0.1:27999 npx jest chf-subscriber.mongo
 * It creates, and drops, its own uniquely named database.
 *
 * chf-subscriber.test.js proves the model assigns a bucketId at validation.
 * This one proves the symptom that motivated it is gone, end to end, through
 * the router the WebUI actually mounts: a subscriber created with
 * POST /db/ChfSubscriber and no bucketId used to be stored without one, and
 * since the resource is addressed by bucketId, DELETE /db/ChfSubscriber/<supi>
 * then answered 404.  The WebUI form, which sends no bucketId either, could
 * create a subscriber it could not edit or delete until the next restart.
 *
 * Auth is the server's, applied where server/index.js mounts /api; this mounts
 * the router directly, as tmf-api.mongo.test.js mounts the TMF one.
 */
'use strict';

const URI = process.env.CHF_TMF_MONGO_URI;
const maybe = URI ? describe : describe.skip;

maybe('ChfSubscriber REST on real MongoDB', () => {
  const express = require('express');
  const mongoose = require('mongoose');

  let server, base, dbName;
  const Sub = () => mongoose.model('ChfSubscriber');

  beforeAll(async () => {
    dbName = `chf_sub_it_${process.pid}_${Date.now()}`;
    await mongoose.connect(`${URI}/${dbName}`, {
      useNewUrlParser: true, useUnifiedTopology: true });

    const router = require('../server/routes/db');
    /* The unique index on bucketId is what a second catch-all would hit. */
    await Sub().init();

    const app = express();
    app.use(express.json());
    /* server/routes/index.js mounts this router at /db, behind JWT auth. */
    app.use('/api/db', router);
    await new Promise(r => { server = app.listen(0, r); });
    base = `http://127.0.0.1:${server.address().port}/api/db/ChfSubscriber`;
  }, 30000);

  afterAll(async () => {
    if (server) await new Promise(r => server.close(r));
    if (mongoose.connection.db) await mongoose.connection.db.dropDatabase();
    await mongoose.disconnect();
  });

  async function call(method, path, body) {
    const res = await fetch(base + path, { method,
      headers: body === undefined ? {} : { 'Content-Type': 'application/json' },
      body: body === undefined ? undefined : JSON.stringify(body) });
    const text = await res.text();
    return { status: res.status, body: text ? JSON.parse(text) : null };
  }

  const QUOTA = { totalQuotaBytes: 0, validityPeriod: 'custom',
                  validitySeconds: 3600, autoRenew: false,
                  finalUnitAction: 'TERMINATE' };

  test('S1. created without a bucketId, it can be read and deleted by its SUPI', async () => {
    const supi = 'imsi-999702000000001';

    /* The exact body tests/chf/test-chf-api.sh TC-P6-4 sends. */
    const created = await call('POST', '', { supi, quota: QUOTA });
    expect(created.status).toBe(201);
    expect(created.body.bucketId).toBe(supi);

    const stored = await Sub().findOne({ supi }).lean();
    expect(stored.bucketId).toBe(supi);

    expect((await call('GET', `/${supi}`)).status).toBe(200);

    const deleted = await call('DELETE', `/${supi}`);
    expect([200, 204]).toContain(deleted.status);
    expect(await Sub().countDocuments({ supi })).toBe(0);
  });

  test('S2. a second catch-all for the same SUPI is refused, not silently duplicated', async () => {
    const supi = 'imsi-999702000000002';
    expect((await call('POST', '', { supi, quota: QUOTA })).status).toBe(201);

    /* Both would take bucketId = SUPI, which the unique index rejects. */
    const again = await call('POST', '', { supi, quota: QUOTA });
    expect(again.status).toBeGreaterThanOrEqual(400);
    expect(await Sub().countDocuments({ supi })).toBe(1);
  });

  test('S3. a scoped bucket created the same way gets its own id and coexists', async () => {
    const supi = 'imsi-999702000000003';
    expect((await call('POST', '', { supi, quota: QUOTA })).status).toBe(201);

    const scoped = await call('POST', '', {
      supi, quota: QUOTA, scope: { snssai: { sst: 1, sd: '000001' } } });
    expect(scoped.status).toBe(201);
    expect(scoped.body.bucketId).toMatch(new RegExp(`^${supi}-[0-9a-f]{12}$`));
    expect(await Sub().countDocuments({ supi })).toBe(2);

    /* Each is addressable by its own id. */
    const del = await call('DELETE', `/${scoped.body.bucketId}`);
    expect([200, 204]).toContain(del.status);
    expect(await Sub().countDocuments({ supi })).toBe(1);
  });
});
