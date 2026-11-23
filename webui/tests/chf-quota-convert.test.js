/*
 * Unit tests for webui/src/helpers/chf-quota-convert.js
 *
 * Covers the unified total-quota schema (totalQuotaBytes, validityPeriod,
 * validitySeconds, autoRenew, finalUnitAction) introduced in Session 13.
 *
 * dbToForm():
 *   1.  10 GiB totalQuotaBytes → totalVolume=10, volumeUnit='GB'
 *   2.  512 MiB totalQuotaBytes → totalVolume=512, volumeUnit='MB'
 *   3.  2 TiB totalQuotaBytes → totalVolume=2, volumeUnit='TB'
 *   4.  null document → safe defaults
 *   5.  Missing quota sub-object → safe defaults
 *   6.  validityPeriod='daily' → validitySeconds=86400
 *   7.  validityPeriod='custom', validitySeconds=7200 → preserved
 *   8.  autoRenew=false → preserved
 *   9.  finalUnitAction='RESTRICT_ACCESS' → preserved
 *  10.  Legacy volumeUlBytes+volumeDlBytes → totalVolume = UL+DL sum
 *
 * formToDb():
 *  11.  GB unit: 10 GB → totalQuotaBytes=10737418240
 *  12.  MB unit: 512 MB → totalQuotaBytes=536870912
 *  13.  TB unit: 1 TB → totalQuotaBytes=1099511627776
 *  14.  validityPeriod='daily' → validitySeconds=86400
 *  15.  validityPeriod='weekly' → validitySeconds=604800
 *  16.  validityPeriod='monthly' → validitySeconds=2592000
 *  17.  validityPeriod='custom', validitySeconds=7200 → validitySeconds=7200
 *  18.  Missing volumeUnit defaults to GB
 *  19.  supi field preserved through formToDb
 *  20.  Display fields (totalVolume, volumeUnit) absent from DB output
 *
 * bytesToDisplay():
 *  21.  >= 1 TiB → unit='TB'
 *  22.  >= 1 GiB, < 1 TiB → unit='GB'
 *  23.  < 1 GiB → unit='MB'
 *
 * fmtVolume():
 *  24.  >= 1 TiB → "X.XX TB"
 *  25.  >= 1 GiB → "X.XX GB"
 *  26.  < 1 GiB → "X.XX MB"
 *  27.  null → "—"
 *
 * fmtPeriod():
 *  28.  'daily' → "Daily (24 h)"
 *  29.  'weekly' → "Weekly (7 d)"
 *  30.  'monthly' → "Monthly (30 d)"
 *  31.  'custom' with seconds → "Custom (7200 s)"
 *  32.  'custom' without seconds → "Custom"
 *  33.  null/undefined → "—"
 *
 * Round-trip:
 *  34.  formToDb(dbToForm(newDoc)) preserves totalQuotaBytes exactly
 *  35.  formToDb(dbToForm(legacyDoc)) sums volumeUlBytes+volumeDlBytes
 */

'use strict';

const {
  dbToForm,
  formToDb,
  fmtVolume,
  fmtPeriod,
  bytesToDisplay,
  BYTES_PER_MB,
  BYTES_PER_GB,
  BYTES_PER_TB,
  PERIOD_SECONDS,
  cycleUsage,
  nextGrant,
} = require('../src/helpers/chf-quota-convert');

const GB = BYTES_PER_GB;   /* 1 073 741 824 */
const MB = BYTES_PER_MB;   /* 1 048 576     */
const TB = BYTES_PER_TB;   /* 1 099 511 627 776 */

function makeDoc(quota) {
  return { supi: 'imsi-001010000000001', quota };
}

// ============================================================ dbToForm tests

test('1. 10 GiB totalQuotaBytes → totalVolume=10, volumeUnit=GB', () => {
  const form = dbToForm(makeDoc({
    totalQuotaBytes: 10 * GB,
    validityPeriod:  'monthly',
    validitySeconds: 2592000,
    autoRenew:       true,
    finalUnitAction: 'TERMINATE',
  }));
  expect(form.quota.volumeUnit).toBe('GB');
  expect(form.quota.totalVolume).toBeCloseTo(10, 5);
  expect(form.quota.validityPeriod).toBe('monthly');
  expect(form.quota.autoRenew).toBe(true);
});

test('2. 512 MiB totalQuotaBytes → totalVolume=512, volumeUnit=MB', () => {
  const form = dbToForm(makeDoc({
    totalQuotaBytes: 512 * MB,
    validityPeriod:  'daily',
    validitySeconds: 86400,
    autoRenew:       false,
    finalUnitAction: 'REDIRECT',
  }));
  expect(form.quota.volumeUnit).toBe('MB');
  expect(form.quota.totalVolume).toBeCloseTo(512, 5);
  expect(form.quota.autoRenew).toBe(false);
});

test('3. 2 TiB totalQuotaBytes → totalVolume=2, volumeUnit=TB', () => {
  const form = dbToForm(makeDoc({
    totalQuotaBytes: 2 * TB,
    validityPeriod:  'weekly',
    validitySeconds: 604800,
    autoRenew:       true,
    finalUnitAction: 'TERMINATE',
  }));
  expect(form.quota.volumeUnit).toBe('TB');
  expect(form.quota.totalVolume).toBeCloseTo(2, 5);
});

test('4. null document → safe defaults (10 GB, monthly, autoRenew, TERMINATE)', () => {
  const form = dbToForm(null);
  expect(form.quota.volumeUnit).toBe('GB');
  expect(form.quota.totalVolume).toBe(10);
  expect(form.quota.validityPeriod).toBe('monthly');
  expect(form.quota.validitySeconds).toBe(2592000);
  expect(form.quota.autoRenew).toBe(true);
  expect(form.quota.finalUnitAction).toBe('TERMINATE');
});

test('5. Missing quota sub-object → safe defaults', () => {
  const form = dbToForm({ supi: 'imsi-001010000000002' });
  expect(form.quota.totalVolume).toBe(10);
  expect(form.quota.volumeUnit).toBe('GB');
  expect(form.quota.autoRenew).toBe(true);
});

test('6. validityPeriod=daily → validitySeconds resolved to 86400', () => {
  const form = dbToForm(makeDoc({
    totalQuotaBytes: 1 * GB,
    validityPeriod:  'daily',
    autoRenew:       true,
    finalUnitAction: 'TERMINATE',
  }));
  expect(form.quota.validityPeriod).toBe('daily');
  expect(form.quota.validitySeconds).toBe(PERIOD_SECONDS.daily);
});

test('7. validityPeriod=custom with validitySeconds=7200 → both preserved', () => {
  const form = dbToForm(makeDoc({
    totalQuotaBytes: 5 * GB,
    validityPeriod:  'custom',
    validitySeconds: 7200,
    autoRenew:       false,
    finalUnitAction: 'TERMINATE',
  }));
  expect(form.quota.validityPeriod).toBe('custom');
  expect(form.quota.validitySeconds).toBe(7200);
});

test('8. autoRenew=false is preserved', () => {
  const form = dbToForm(makeDoc({
    totalQuotaBytes: 1 * GB,
    validityPeriod:  'monthly',
    validitySeconds: 2592000,
    autoRenew:       false,
    finalUnitAction: 'TERMINATE',
  }));
  expect(form.quota.autoRenew).toBe(false);
});

test('9. finalUnitAction=RESTRICT_ACCESS is preserved', () => {
  const form = dbToForm(makeDoc({
    totalQuotaBytes: 5 * GB,
    validityPeriod:  'monthly',
    validitySeconds: 2592000,
    autoRenew:       true,
    finalUnitAction: 'RESTRICT_ACCESS',
  }));
  expect(form.quota.finalUnitAction).toBe('RESTRICT_ACCESS');
});

test('10. Legacy volumeUlBytes+volumeDlBytes → totalVolume = sum', () => {
  /* Old schema: 2 GB UL + 2 GB DL → 4 GB total */
  const form = dbToForm(makeDoc({
    volumeUlBytes: 2 * GB,
    volumeDlBytes: 2 * GB,
    validitySeconds: 3600,
    finalUnitAction: 'TERMINATE',
  }));
  expect(form.quota.volumeUnit).toBe('GB');
  expect(form.quota.totalVolume).toBeCloseTo(4, 5);
});

// ============================================================ formToDb tests

test('11. GB unit: 10 GB → totalQuotaBytes=10737418240', () => {
  const db = formToDb({
    supi:  'imsi-001010000000002',
    quota: {
      totalVolume:     10,
      volumeUnit:      'GB',
      validityPeriod:  'monthly',
      validitySeconds: 2592000,
      autoRenew:       true,
      finalUnitAction: 'TERMINATE',
    },
  });
  expect(db.quota.totalQuotaBytes).toBe(10 * GB);
});

test('12. MB unit: 512 MB → totalQuotaBytes=536870912', () => {
  const db = formToDb({
    quota: {
      totalVolume:     512,
      volumeUnit:      'MB',
      validityPeriod:  'custom',
      validitySeconds: 3600,
      autoRenew:       false,
      finalUnitAction: 'REDIRECT',
    },
  });
  expect(db.quota.totalQuotaBytes).toBe(512 * MB);
});

test('13. TB unit: 1 TB → totalQuotaBytes=1099511627776', () => {
  const db = formToDb({
    quota: {
      totalVolume:     1,
      volumeUnit:      'TB',
      validityPeriod:  'monthly',
      validitySeconds: 2592000,
      autoRenew:       true,
      finalUnitAction: 'TERMINATE',
    },
  });
  expect(db.quota.totalQuotaBytes).toBe(1 * TB);
});

test('14. validityPeriod=daily → validitySeconds=86400 (preset overrides field)', () => {
  const db = formToDb({
    quota: {
      totalVolume: 1, volumeUnit: 'GB',
      validityPeriod: 'daily', validitySeconds: 99999, /* ignored */
      autoRenew: true, finalUnitAction: 'TERMINATE',
    },
  });
  expect(db.quota.validitySeconds).toBe(86400);
  expect(db.quota.validityPeriod).toBe('daily');
});

test('15. validityPeriod=weekly → validitySeconds=604800', () => {
  const db = formToDb({
    quota: {
      totalVolume: 1, volumeUnit: 'GB',
      validityPeriod: 'weekly',
      autoRenew: true, finalUnitAction: 'TERMINATE',
    },
  });
  expect(db.quota.validitySeconds).toBe(604800);
});

test('16. validityPeriod=monthly → validitySeconds=2592000', () => {
  const db = formToDb({
    quota: {
      totalVolume: 1, volumeUnit: 'GB',
      validityPeriod: 'monthly',
      autoRenew: true, finalUnitAction: 'TERMINATE',
    },
  });
  expect(db.quota.validitySeconds).toBe(2592000);
});

test('17. validityPeriod=custom, validitySeconds=7200 → validitySeconds=7200', () => {
  const db = formToDb({
    quota: {
      totalVolume: 1, volumeUnit: 'GB',
      validityPeriod: 'custom', validitySeconds: 7200,
      autoRenew: false, finalUnitAction: 'TERMINATE',
    },
  });
  expect(db.quota.validitySeconds).toBe(7200);
});

test('18. Missing volumeUnit defaults to GB', () => {
  const db = formToDb({
    quota: { totalVolume: 5, validityPeriod: 'monthly', autoRenew: true, finalUnitAction: 'TERMINATE' },
  });
  expect(db.quota.totalQuotaBytes).toBe(5 * GB);
});

test('19. supi is preserved through formToDb', () => {
  const db = formToDb({
    supi:  'imsi-001010000000004',
    quota: { totalVolume: 1, volumeUnit: 'GB', validityPeriod: 'monthly',
             autoRenew: true, finalUnitAction: 'TERMINATE' },
  });
  expect(db.supi).toBe('imsi-001010000000004');
});

test('20. Display fields absent from DB document', () => {
  const db = formToDb({
    quota: { totalVolume: 10, volumeUnit: 'GB', validityPeriod: 'monthly',
             autoRenew: true, finalUnitAction: 'TERMINATE' },
  });
  expect(db.quota.totalVolume).toBeUndefined();
  expect(db.quota.volumeUnit).toBeUndefined();
});

// ========================================================= bytesToDisplay tests

test('21. >= 1 TiB → unit=TB', () => {
  const { unit } = bytesToDisplay(2 * TB);
  expect(unit).toBe('TB');
});

test('22. >= 1 GiB, < 1 TiB → unit=GB', () => {
  const { unit, value } = bytesToDisplay(10 * GB);
  expect(unit).toBe('GB');
  expect(value).toBeCloseTo(10, 5);
});

test('23. < 1 GiB → unit=MB', () => {
  const { unit, value } = bytesToDisplay(512 * MB);
  expect(unit).toBe('MB');
  expect(value).toBeCloseTo(512, 5);
});

// ============================================================ fmtVolume tests

test('24. >= 1 TiB → "X.XX TB"', () => {
  expect(fmtVolume(2 * TB)).toBe('2.00 TB');
  expect(fmtVolume(1.5 * TB)).toBe('1.50 TB');
});

test('25. >= 1 GiB → "X.XX GB"', () => {
  expect(fmtVolume(10 * GB)).toBe('10.00 GB');
  expect(fmtVolume(1.5 * GB)).toBe('1.50 GB');
});

test('26. < 1 GiB → "X.XX MB"', () => {
  expect(fmtVolume(512 * MB)).toBe('512.00 MB');
  expect(fmtVolume(1 * MB)).toBe('1.00 MB');
});

test('27. null/undefined → "—"', () => {
  expect(fmtVolume(null)).toBe('—');
  expect(fmtVolume(undefined)).toBe('—');
});

// ============================================================ fmtPeriod tests

test('28. daily → "Daily (24 h)"', () => {
  expect(fmtPeriod('daily')).toBe('Daily (24 h)');
});

test('29. weekly → "Weekly (7 d)"', () => {
  expect(fmtPeriod('weekly')).toBe('Weekly (7 d)');
});

test('30. monthly → "Monthly (30 d)"', () => {
  expect(fmtPeriod('monthly')).toBe('Monthly (30 d)');
});

test('31. custom with seconds → "Custom (7200 s)"', () => {
  expect(fmtPeriod('custom', 7200)).toBe('Custom (7200 s)');
});

test('32. custom without seconds → "Custom"', () => {
  expect(fmtPeriod('custom')).toBe('Custom');
  expect(fmtPeriod('custom', null)).toBe('Custom');
});

test('33. null/undefined → "—"', () => {
  expect(fmtPeriod(null)).toBe('—');
  expect(fmtPeriod(undefined)).toBe('—');
  expect(fmtPeriod('')).toBe('—');
});

// ========================================================== round-trip tests

test('34. formToDb(dbToForm(newDoc)) preserves totalQuotaBytes exactly', () => {
  const original = makeDoc({
    totalQuotaBytes: 7 * GB,
    validityPeriod:  'custom',
    validitySeconds: 7200,
    autoRenew:       false,
    finalUnitAction: 'REDIRECT',
  });
  const roundTripped = formToDb(dbToForm(original));
  expect(roundTripped.quota.totalQuotaBytes).toBe(7 * GB);
  expect(roundTripped.quota.validitySeconds).toBe(7200);
  expect(roundTripped.quota.autoRenew).toBe(false);
  expect(roundTripped.quota.finalUnitAction).toBe('REDIRECT');
});

test('35. formToDb(dbToForm(legacyDoc)) converts UL+DL sum to totalQuotaBytes', () => {
  /* Legacy document with separate UL/DL fields */
  const legacyDoc = makeDoc({
    volumeUlBytes: 3 * GB,
    volumeDlBytes: 3 * GB,
    validitySeconds: 3600,
    finalUnitAction: 'TERMINATE',
  });
  const db = formToDb(dbToForm(legacyDoc));
  /* UL 3 GB + DL 3 GB = 6 GB total */
  expect(db.quota.totalQuotaBytes).toBe(6 * GB);
  expect(db.quota.totalQuotaBytes).toBeGreaterThan(0);
});

describe('cycleUsage', () => {
  const GB = 1073741824;
  const MONTH = 2592000;

  test('36. provisioned minus consumed', () => {
    const u = cycleUsage({
      quota: { totalQuotaBytes: 10 * GB, validitySeconds: MONTH },
      usage: { consumedBytes: 3 * GB, periodStart: new Date().toISOString() },
    });
    expect(u.provisioned).toBe(10 * GB);
    expect(u.consumed).toBe(3 * GB);
    expect(u.remaining).toBe(7 * GB);
    expect(u.pct).toBeCloseTo(30);
    expect(u.stale).toBe(false);
  });

  test('37. subscriber the CHF has never seen → full allowance', () => {
    const u = cycleUsage({ quota: { totalQuotaBytes: 10 * GB } });
    expect(u.consumed).toBe(0);
    expect(u.remaining).toBe(10 * GB);
    expect(u.pct).toBe(0);
  });

  test('38. quota lowered below spend → remaining clamps at 0, not negative', () => {
    /* An active cycle: the CHF always stamps periodStart before counting. */
    const u = cycleUsage({
      quota: { totalQuotaBytes: 1 * GB, validitySeconds: MONTH },
      usage: { consumedBytes: 5 * GB, periodStart: new Date().toISOString() },
    });
    expect(u.remaining).toBe(0);
    expect(u.pct).toBe(100);
  });

  test('39. elapsed cycle is flagged stale (CHF rolls over on next session)', () => {
    const started = new Date(Date.now() - (MONTH + 60) * 1000).toISOString();
    const u = cycleUsage({
      quota: { totalQuotaBytes: 10 * GB, validitySeconds: MONTH },
      usage: { consumedBytes: 2 * GB, periodStart: started },
    });
    expect(u.stale).toBe(true);
  });

  test('40. no validitySeconds → never stale', () => {
    const u = cycleUsage({
      quota: { totalQuotaBytes: 10 * GB, validitySeconds: 0 },
      usage: { consumedBytes: 2 * GB, periodStart: '2020-01-01T00:00:00.000Z' },
    });
    expect(u.stale).toBe(false);
  });

  test('41. unprovisioned quota (0 bytes) does not divide by zero', () => {
    const u = cycleUsage({ quota: {}, usage: { consumedBytes: 5 * GB } });
    expect(u.provisioned).toBe(0);
    expect(u.remaining).toBe(0);
    expect(u.pct).toBe(0);
  });

  test('42. missing/garbage document is handled', () => {
    expect(cycleUsage(null).remaining).toBe(0);
    expect(cycleUsage({}).remaining).toBe(0);
    expect(cycleUsage({ quota: { totalQuotaBytes: 'x' } }).provisioned).toBe(0);
  });
});

describe('formToDb never writes the CHF-owned usage counter', () => {
  const GB = 1073741824;
  const fetched = {
    _id: '64f0c0ffee',
    __v: 3,
    supi: 'imsi-999700000000001',
    quota: {
      totalQuotaBytes: 10 * GB, validityPeriod: 'monthly',
      validitySeconds: 2592000, autoRenew: true, finalUnitAction: 'TERMINATE',
    },
    usage: { consumedBytes: 2 * GB, periodStart: '2026-09-01T00:00:00.000Z' },
  };

  test('43. usage is stripped from the PATCH body', () => {
    expect(formToDb(fetched)).not.toHaveProperty('usage');
  });

  test('44. regression: the edit-modal round trip cannot rewind the counter', () => {
    /* Exactly what Document.js does: load into the form, then save.  Before
     * the fix this carried the modal-open snapshot of usage back to the DB,
     * overwriting everything the CHF had counted in between. */
    const body = formToDb(dbToForm(fetched));
    expect(body).not.toHaveProperty('usage');
    expect(body.quota.totalQuotaBytes).toBe(10 * GB);
    expect(body.supi).toBe('imsi-999700000000001');
  });

  test('45. Mongoose bookkeeping (_id, __v) is not sent', () => {
    const body = formToDb(fetched);
    expect(body).not.toHaveProperty('_id');
    expect(body).not.toHaveProperty('__v');
  });
});

describe('cycleUsage — top-ups and expiry (shared with the TMF654 API)', () => {
  const GB = 1073741824;
  const MONTH = 2592000;
  const NOW = Date.UTC(2026, 8, 15, 12, 0, 0);
  const ago = s => new Date(NOW - s * 1000).toISOString();

  test('46. active: top-ups add to what remains', () => {
    const u = cycleUsage({
      quota: { totalQuotaBytes: 10 * GB, validitySeconds: MONTH },
      usage: { periodStart: ago(3600), consumedBytes: 9 * GB, topupBytes: 2 * GB },
    }, NOW);
    expect(u.state).toBe('active');
    expect(u.topup).toBe(2 * GB);
    expect(u.remaining).toBe(3 * GB);
    expect(u.pct).toBeCloseTo(75);
  });

  test('47. expired + autoRenew: next session gets the full allowance', () => {
    const u = cycleUsage({
      quota: { totalQuotaBytes: 10 * GB, validitySeconds: MONTH, autoRenew: true },
      usage: { periodStart: ago(MONTH + 60), consumedBytes: 10 * GB, topupBytes: 1 * GB },
    }, NOW);
    expect(u.stale).toBe(true);
    expect(u.remaining).toBe(10 * GB);
    /* The old cycle's figures do not carry into the next one. */
    expect(u.consumed).toBe(0);
    expect(u.topup).toBe(0);
  });

  test('48. expired, not renewing: nothing remains until a top-up', () => {
    const u = cycleUsage({
      quota: { totalQuotaBytes: 10 * GB, validitySeconds: MONTH, autoRenew: false },
      usage: { periodStart: ago(MONTH + 60), consumedBytes: 1 * GB },
    }, NOW);
    expect(u.remaining).toBe(0);
  });

  test('49. page and API agree: cycleUsage().remaining === nextGrant()', () => {
    const docs = [
      { quota: { totalQuotaBytes: 5 * GB } },
      { quota: { totalQuotaBytes: 5 * GB, validitySeconds: MONTH },
        usage: { periodStart: ago(60), consumedBytes: 2 * GB, topupBytes: GB } },
      { quota: { totalQuotaBytes: 5 * GB, validitySeconds: MONTH, autoRenew: true },
        usage: { periodStart: ago(MONTH + 1), consumedBytes: 5 * GB } },
      { quota: { totalQuotaBytes: 5 * GB, validitySeconds: MONTH },
        usage: { periodStart: ago(MONTH + 1), consumedBytes: 1 } },
    ];
    docs.forEach(d => expect(cycleUsage(d, NOW).remaining).toBe(nextGrant(d, NOW)));
  });
});

describe('bucket scope — what an allowance covers', () => {
  const { formToScope, scopeToForm, scopeLabel, hasScope, bucketIdFor } =
      require('../src/helpers/chf-quota-convert');

  test('50. empty form fields leave every dimension open', () => {
    expect(formToScope({ sst: '', sd: '', dnn: '', hplmn: '', roaming: '' }))
        .toBeNull();
    expect(formToScope({})).toBeNull();
    expect(formToScope(null)).toBeNull();
  });

  test('51. filled fields become the stored scope, SD lower-cased', () => {
    expect(formToScope({ sst: '1', sd: 'AABBCC', dnn: 'ims',
                         hplmn: '999-70', roaming: 'outBound' }))
      .toEqual({ snssai: { sst: 1, sd: 'aabbcc' }, dnn: 'ims',
                 hplmn: '999-70', roaming: 'outBound' });

    /* An SD without an SST is meaningless and is dropped with it. */
    expect(formToScope({ sd: 'aabbcc' })).toBeNull();

    /* SST 0 is a real value, not "empty". */
    expect(formToScope({ sst: '0' })).toEqual({ snssai: { sst: 0 } });
  });

  test('52. stored scope round-trips back into the form', () => {
    const doc = { scope: { snssai: { sst: 2, sd: '010203' }, dnn: 'internet',
                           hplmn: '001-01', roaming: 'home' } };
    expect(scopeToForm(doc)).toEqual({ sst: '2', sd: '010203',
      dnn: 'internet', hplmn: '001-01', roaming: 'home' });
    expect(formToScope(scopeToForm(doc))).toEqual(doc.scope);

    expect(scopeToForm({})).toEqual({ sst: '', sd: '', dnn: '', hplmn: '',
      roaming: '' });
  });

  test('53. the list says what a bucket covers', () => {
    expect(scopeLabel({})).toBe('everything');
    expect(hasScope({})).toBe(false);
    const doc = { scope: { snssai: { sst: 1, sd: 'aabbcc' }, dnn: 'ims',
                           roaming: 'outBound' } };
    expect(hasScope(doc)).toBe(true);
    expect(scopeLabel(doc)).toBe('slice 1/aabbcc \u00b7 ims \u00b7 roaming abroad');
  });

  test('54. the catch-all keeps the SUPI as its id; a scoped bucket gets its own', () => {
    const supi = 'imsi-999700000000001';
    expect(bucketIdFor({ supi }, null)).toBe(supi);

    const scoped = bucketIdFor({ supi }, { dnn: 'ims' });
    expect(scoped).not.toBe(supi);
    expect(scoped).toMatch(/^imsi-999700000000001-[0-9a-f]{12}$/);

    /* An existing bucket never changes id, whatever its scope becomes. */
    expect(bucketIdFor({ supi, bucketId: 'imsi-999700000000001-abc123def456' },
        { dnn: 'internet' })).toBe('imsi-999700000000001-abc123def456');
  });

  test('55. formToDb writes scope, priority and an id', () => {
    const { formToDb } = require('../src/helpers/chf-quota-convert');
    const base = { supi: 'imsi-999700000000001',
      quota: { totalVolume: 1, volumeUnit: 'GB', validityPeriod: 'monthly',
               autoRenew: true, finalUnitAction: 'TERMINATE' } };

    const plain = formToDb(Object.assign({}, base, {
      scopeForm: { sst: '', dnn: '' }, priority: 0 }));
    expect(plain.scope).toBeUndefined();
    expect(plain.bucketId).toBe(base.supi);
    expect(plain.priority).toBe(0);
    expect(plain.scopeForm).toBeUndefined();

    const scoped = formToDb(Object.assign({}, base, {
      scopeForm: { sst: '1', dnn: 'ims' }, priority: 30 }));
    expect(scoped.scope).toEqual({ snssai: { sst: 1 }, dnn: 'ims' });
    expect(scoped.priority).toBe(30);
    expect(scoped.bucketId).toMatch(/^imsi-999700000000001-[0-9a-f]{12}$/);
  });

  test('56. a pool shows its SIMs, and the form never writes them back', () => {
    const { dbToForm, formToDb } = require('../src/helpers/chf-quota-convert');
    const pool = { bucketId: 'pool-0123456789ab',
      members: ['imsi-999700000000001', 'imsi-999700000000002'],
      quota: { totalQuotaBytes: 1073741824, validityPeriod: 'monthly',
               validitySeconds: 2592000, autoRenew: true,
               finalUnitAction: 'TERMINATE' } };

    const form = dbToForm(pool);
    expect(form.membersText).toBe('imsi-999700000000001\nimsi-999700000000002');

    /* Members change through the TMF API only; the pool keeps its id. */
    const doc = formToDb(form);
    expect(doc).not.toHaveProperty('members');
    expect(doc).not.toHaveProperty('membersText');
    expect(doc.bucketId).toBe('pool-0123456789ab');
    expect(doc.quota.totalQuotaBytes).toBe(1073741824);

    /* And a SIM's own bucket has no such field. */
    expect(dbToForm({ supi: 'imsi-999700000000001', quota: pool.quota }))
      .not.toHaveProperty('membersText');
  });

  test('57. a pool form lists its SIMs one per line, or with commas', () => {
    const { membersFromText } = require('../src/helpers/chf-quota-convert');
    expect(membersFromText(' imsi-999700000000001\nimsi-999700000000002 \n\n'))
      .toEqual(['imsi-999700000000001', 'imsi-999700000000002']);
    expect(membersFromText('imsi-999700000000001, imsi-999700000000002'))
      .toEqual(['imsi-999700000000001', 'imsi-999700000000002']);
    expect(membersFromText('')).toEqual([]);
    expect(membersFromText(undefined)).toEqual([]);
  });

  test('58. a pool form becomes the TMF Bucket body, plan and members', () => {
    const { poolFormToTmf, DEFAULT_POOL_FORM } =
      require('../src/helpers/chf-quota-convert');
    const form = Object.assign({}, DEFAULT_POOL_FORM, {
      membersText: 'imsi-999700000000001\nimsi-999700000000002',
      priority: 20,
      quota: { totalVolume: 20, volumeUnit: 'GB', validityPeriod: 'monthly',
               autoRenew: true, finalUnitAction: 'TERMINATE' },
      scopeForm: { sst: '', sd: '', dnn: '', hplmn: '', roaming: '' },
    });

    const create = poolFormToTmf(form, true);
    expect(create).toEqual({
      logicalResource: [{ id: 'imsi-999700000000001' },
                        { id: 'imsi-999700000000002' }],
      allowance: { amount: 20 * 1073741824, units: 'octet' },
      validityPeriod: 'monthly', autoRenew: true,
      finalUnitAction: 'TERMINATE', priority: 20,
    });
    /* No scope on create is left out; on update it clears the scope. */
    expect(poolFormToTmf(form, false).scope).toBeNull();

    const scoped = poolFormToTmf(Object.assign({}, form, {
      quota: Object.assign({}, form.quota,
        { validityPeriod: 'custom', validitySeconds: 3600 }),
      scopeForm: { sst: '1', sd: '000001', dnn: 'internet',
                   hplmn: '999-70', roaming: 'home' },
    }), true);
    expect(scoped.validitySeconds).toBe(3600);
    expect(scoped.scope).toEqual({ sNSSAI: { sst: 1, sd: '000001' },
      dnn: 'internet', hPlmnId: { mcc: '999', mnc: '70' }, roaming: 'home' });
  });
});
