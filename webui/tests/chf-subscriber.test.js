/*
 * Unit tests for the ChfSubscriber Mongoose model.
 *
 * Tests cover:
 *   1.  Valid subscriber passes validation
 *   2.  supi is required — missing supi fails validation
 *   3.  supi must match imsi-<15 digits> pattern
 *   4.  quota defaults: totalQuotaBytes = 10 GiB (10737418240)
 *   5.  quota defaults: validityPeriod = 'monthly'
 *   6.  quota defaults: validitySeconds = 2592000 (30-day month)
 *   7.  quota defaults: autoRenew = true
 *   8.  quota defaults: finalUnitAction = 'TERMINATE'
 *   9.  invalid finalUnitAction is rejected
 *  10.  invalid validityPeriod is rejected
 *  11.  explicit totalQuotaBytes is persisted correctly
 *  12.  autoRenew=false is persisted correctly
 *  13.  validityPeriod='daily' passes validation
 *  14.  validityPeriod='weekly' passes validation
 *  15.  validityPeriod='custom' with validitySeconds passes validation
 *  16.  finalUnitAction='REDIRECT' passes validation
 *  17.  finalUnitAction='RESTRICT_ACCESS' passes validation
 *  18.  a document created without bucketId gets the SUPI (catch-all)
 *  19.  a scoped document gets <supi>-<12 hex>, distinct per bucket
 *  20.  an explicit bucketId is never replaced
 *  21.  a scope present but empty is still the catch-all
 *  22.  a pool - members and no supi - passes, and gets a pool- id
 *  23.  a document with neither a supi nor members fails
 *  24.  a pool member must be a SUPI, and listed once
 *  25.  a document with both a supi and members is refused
 *  26.  a document that is not a pool has no members field at all
 *
 * These tests run entirely in-memory via Mongoose document
 * validation — no MongoDB connection is required.
 */

'use strict';

const mongoose = require('mongoose');

// Load the model under test.
// express-restify-mongoose registers it in the default connection;
// we just need the schema validation logic here.
const ChfSubscriber = require('../server/models/chf-subscriber');

describe('ChfSubscriber model', () => {
  // ------------------------------------------------------------------ helpers
  function makeDoc(overrides) {
    return new ChfSubscriber(
      Object.assign(
        {
          supi: 'imsi-001010000000001',
          quota: {
            totalQuotaBytes:  10737418240,   /* 10 GiB */
            validityPeriod:   'monthly',
            validitySeconds:  2592000,
            autoRenew:        true,
            finalUnitAction:  'TERMINATE',
          },
        },
        overrides
      )
    );
  }

  // ------------------------------------------------------------------ tests

  test('1. valid subscriber passes validation', async () => {
    const doc = makeDoc({});
    await expect(doc.validate()).resolves.toBeUndefined();
  });

  test('2. supi is required — missing supi fails validation', async () => {
    const doc = makeDoc({ supi: undefined });
    await expect(doc.validate()).rejects.toThrow(/supi/i);
  });

  test('3. supi must match imsi-<15 digits> pattern', async () => {
    const doc = makeDoc({ supi: 'imsi-12345' }); /* too short */
    await expect(doc.validate()).rejects.toThrow(/supi/i);
  });

  /* ── Default value tests (quota omitted entirely) ──────────────────────── */

  test('4. default totalQuotaBytes is 10 GiB (10737418240)', () => {
    const doc = new ChfSubscriber({ supi: 'imsi-001010000000002' });
    expect(doc.quota.totalQuotaBytes).toBe(10737418240);
  });

  test('5. default validityPeriod is monthly', () => {
    const doc = new ChfSubscriber({ supi: 'imsi-001010000000002' });
    expect(doc.quota.validityPeriod).toBe('monthly');
  });

  test('6. default validitySeconds is 2592000 (30-day month)', () => {
    const doc = new ChfSubscriber({ supi: 'imsi-001010000000002' });
    expect(doc.quota.validitySeconds).toBe(2592000);
  });

  test('7. default autoRenew is true', () => {
    const doc = new ChfSubscriber({ supi: 'imsi-001010000000002' });
    expect(doc.quota.autoRenew).toBe(true);
  });

  test('8. default finalUnitAction is TERMINATE', () => {
    const doc = new ChfSubscriber({ supi: 'imsi-001010000000002' });
    expect(doc.quota.finalUnitAction).toBe('TERMINATE');
  });

  /* ── Validation rejection tests ─────────────────────────────────────────── */

  test('9. invalid finalUnitAction is rejected', async () => {
    const doc = makeDoc({
      quota: {
        totalQuotaBytes: 10737418240, validityPeriod: 'monthly',
        validitySeconds: 2592000, autoRenew: true,
        finalUnitAction: 'BLOCK',   /* not in enum */
      },
    });
    await expect(doc.validate()).rejects.toThrow(/finalUnitAction/i);
  });

  test('10. invalid validityPeriod is rejected', async () => {
    const doc = makeDoc({
      quota: {
        totalQuotaBytes: 10737418240, validityPeriod: 'yearly', /* not in enum */
        validitySeconds: 31536000, autoRenew: true,
        finalUnitAction: 'TERMINATE',
      },
    });
    await expect(doc.validate()).rejects.toThrow(/validityPeriod/i);
  });

  /* ── Explicit field persistence tests ───────────────────────────────────── */

  test('11. explicit totalQuotaBytes is stored correctly', () => {
    const BYTES = 5 * 1073741824;   /* 5 GiB */
    const doc = makeDoc({
      quota: {
        totalQuotaBytes: BYTES, validityPeriod: 'monthly',
        validitySeconds: 2592000, autoRenew: true,
        finalUnitAction: 'TERMINATE',
      },
    });
    expect(doc.quota.totalQuotaBytes).toBe(BYTES);
  });

  test('12. autoRenew=false is stored correctly', () => {
    const doc = makeDoc({
      quota: {
        totalQuotaBytes: 10737418240, validityPeriod: 'monthly',
        validitySeconds: 2592000, autoRenew: false,
        finalUnitAction: 'TERMINATE',
      },
    });
    expect(doc.quota.autoRenew).toBe(false);
  });

  /* ── Valid enum value acceptance tests ──────────────────────────────────── */

  test('13. validityPeriod=daily passes validation', async () => {
    const doc = makeDoc({
      quota: {
        totalQuotaBytes: 10737418240, validityPeriod: 'daily',
        validitySeconds: 86400, autoRenew: true,
        finalUnitAction: 'TERMINATE',
      },
    });
    await expect(doc.validate()).resolves.toBeUndefined();
  });

  test('14. validityPeriod=weekly passes validation', async () => {
    const doc = makeDoc({
      quota: {
        totalQuotaBytes: 10737418240, validityPeriod: 'weekly',
        validitySeconds: 604800, autoRenew: true,
        finalUnitAction: 'TERMINATE',
      },
    });
    await expect(doc.validate()).resolves.toBeUndefined();
  });

  test('15. validityPeriod=custom with custom validitySeconds passes validation', async () => {
    const doc = makeDoc({
      quota: {
        totalQuotaBytes: 10737418240, validityPeriod: 'custom',
        validitySeconds: 7200, autoRenew: false,
        finalUnitAction: 'TERMINATE',
      },
    });
    await expect(doc.validate()).resolves.toBeUndefined();
  });

  test('16. finalUnitAction=REDIRECT passes validation', async () => {
    const doc = makeDoc({
      quota: {
        totalQuotaBytes: 10737418240, validityPeriod: 'monthly',
        validitySeconds: 2592000, autoRenew: true,
        finalUnitAction: 'REDIRECT',
      },
    });
    await expect(doc.validate()).resolves.toBeUndefined();
  });

  test('17. finalUnitAction=RESTRICT_ACCESS passes validation', async () => {
    const doc = makeDoc({
      quota: {
        totalQuotaBytes: 10737418240, validityPeriod: 'monthly',
        validitySeconds: 2592000, autoRenew: true,
        finalUnitAction: 'RESTRICT_ACCESS',
      },
    });
    await expect(doc.validate()).resolves.toBeUndefined();
  });

  // ---------------------------------------------------------- bucket ids
  //
  // The REST resource is addressed by bucketId.  A document created through
  // it without one used to get none, and could then be listed but neither
  // edited nor deleted until the WebUI restarted and the startup migration
  // backfilled it.  These pin the id the model now assigns at validation.

  test('18. a document created without bucketId gets the SUPI (catch-all)', async () => {
    const doc = makeDoc({});
    expect(doc.bucketId).toBeUndefined();
    await doc.validate();
    expect(doc.bucketId).toBe('imsi-001010000000001');
  });

  test('19. a scoped document gets <supi>-<12 hex>, distinct per bucket', async () => {
    const a = makeDoc({ scope: { snssai: { sst: 1, sd: '000001' }, dnn: 'internet' } });
    const b = makeDoc({ scope: { snssai: { sst: 1, sd: '000002' }, dnn: 'internet' } });
    await a.validate();
    await b.validate();
    expect(a.bucketId).toMatch(/^imsi-001010000000001-[0-9a-f]{12}$/);
    expect(b.bucketId).toMatch(/^imsi-001010000000001-[0-9a-f]{12}$/);
    /* Two scoped buckets of one subscriber must not collide on the unique
     * index, and neither may take the catch-all's id. */
    expect(a.bucketId).not.toBe(b.bucketId);
  });

  test('20. an explicit bucketId is never replaced', async () => {
    const doc = makeDoc({ bucketId: 'imsi-001010000000001-a1b2c3d4e5f6' });
    await doc.validate();
    expect(doc.bucketId).toBe('imsi-001010000000001-a1b2c3d4e5f6');
  });

  test('21. a scope present but empty is still the catch-all', async () => {
    /* A Mongoose nested path is always an object, so an empty scope must
     * not be mistaken for a scoped bucket - that would give a plain plan a
     * generated id where everything else expects the SUPI. */
    const doc = makeDoc({ scope: {} });
    await doc.validate();
    expect(doc.bucketId).toBe('imsi-001010000000001');
  });

  const pool = members => new ChfSubscriber({ members,
    quota: { totalQuotaBytes: 10737418240, validityPeriod: 'monthly',
             validitySeconds: 2592000, autoRenew: true,
             finalUnitAction: 'TERMINATE' } });

  test('22. a pool - members and no supi - passes, and gets a pool- id', async () => {
    const doc = pool(['imsi-001010000000001', 'imsi-001010000000002']);
    await expect(doc.validate()).resolves.toBeUndefined();
    expect(doc.bucketId).toMatch(/^pool-[0-9a-f]{12}$/);
    expect(doc.supi).toBeUndefined();
  });

  test('23. a document with neither a supi nor members fails', async () => {
    const doc = makeDoc({ supi: undefined });
    await expect(doc.validate()).rejects.toThrow(/supi/);
  });

  test('24. a pool member must be a SUPI, and listed once', async () => {
    await expect(pool(['imsi-001010000000001', 'not-a-supi']).validate())
      .rejects.toThrow();
    await expect(pool(['imsi-001010000000001', 'imsi-001010000000001'])
      .validate()).rejects.toThrow(/each once/);
  });

  test('25. a document with both a supi and members is refused', async () => {
    const doc = makeDoc({ members: ['imsi-001010000000002'] });
    await expect(doc.validate()).rejects.toThrow(/members, not a supi/);
  });

  test('26. a document that is not a pool has no members field at all', async () => {
    /* An empty array would be indexed, and two would collide on the unique
     * index that keeps a SIM in one pool. */
    const doc = makeDoc({});
    await doc.validate();
    expect(doc.toObject()).not.toHaveProperty('members');
  });
});
