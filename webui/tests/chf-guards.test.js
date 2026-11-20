/*
 * Server-side guard: API clients must not be able to write usage.*, the
 * CHF-owned billing-cycle counter.
 */
'use strict';

const {
  chfSubscriberProtectUsage, chfSubscriberProtectMembers,
} = require('../server/chf-guards');

function run(body, method = 'PATCH') {
  const req = { method, body };
  let nextCalled = false;
  chfSubscriberProtectUsage(req, {}, () => { nextCalled = true; });
  return { body: req.body, nextCalled };
}

describe('chfSubscriberProtectUsage', () => {
  test('1. strips a usage object', () => {
    const { body, nextCalled } = run({
      quota: { totalQuotaBytes: 5 },
      usage: { consumedBytes: 0 },
    });
    expect(body).not.toHaveProperty('usage');
    expect(body.quota.totalQuotaBytes).toBe(5);
    expect(nextCalled).toBe(true);
  });

  test('2. strips dotted paths that would target the nested field', () => {
    /* Mongoose document.set() applies these straight to usage.* */
    const { body } = run({
      'usage.consumedBytes': 0,
      'usage.periodStart': '2020-01-01',
      'quota.autoRenew': false,
    });
    expect(body).not.toHaveProperty(['usage.consumedBytes']);
    expect(body).not.toHaveProperty(['usage.periodStart']);
    expect(body['quota.autoRenew']).toBe(false);
  });

  test('3. does not strip keys that merely start with "usage"', () => {
    const { body } = run({ usageNote: 'keep me' });
    expect(body.usageNote).toBe('keep me');
  });

  test('4. applies to create (POST) as well as update', () => {
    const { body } = run({ supi: 'imsi-1', usage: { consumedBytes: 9 } }, 'POST');
    expect(body).not.toHaveProperty('usage');
    expect(body.supi).toBe('imsi-1');
  });

  test('5. reads with no body are a no-op and still continue', () => {
    expect(run(undefined, 'GET').nextCalled).toBe(true);
    expect(run(null, 'GET').nextCalled).toBe(true);
  });
});

describe('chfSubscriberProtectMembers', () => {
  const runMembers = body => {
    const req = { method: 'PATCH', body };
    let nextCalled = false;
    chfSubscriberProtectMembers(req, {}, () => { nextCalled = true; });
    return { body: req.body, nextCalled };
  };

  test('1. a pool\'s members cannot be changed through the REST resource', () => {
    /* They change through the TMF API, which releases a removed member's
     * sessions and keeps a SIM in one pool. */
    const { body, nextCalled } = runMembers({
      members: ['imsi-001010000000009'],
      'members.0': 'imsi-001010000000009',
      quota: { totalQuotaBytes: 5 },
    });
    expect(body).not.toHaveProperty('members');
    expect(body).not.toHaveProperty(['members.0']);
    expect(body.quota.totalQuotaBytes).toBe(5);
    expect(nextCalled).toBe(true);
  });

  test('2. leaves other keys, and a body-less request, alone', () => {
    expect(runMembers({ membersNote: 'keep' }).body.membersNote).toBe('keep');
    expect(runMembers(undefined).nextCalled).toBe(true);
  });
});
