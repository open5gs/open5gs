/*
 * Unit tests for webui/src/helpers/chf-query.js
 *
 * Covers the pagination/search query construction and X-Total-Count parsing
 * used to keep the CHF subscriber and session views bounded at scale.
 *
 * escapeRegex():
 *   1.  plain digits unchanged
 *   2.  regex metacharacters neutralised
 *   3.  null/undefined → empty string
 *
 * buildSubscriberParams():
 *   4.  page 0 → skip 0, limit=pageSize, sort=supi, no query
 *   5.  page 2, size 25 → skip 50
 *   6.  search adds a prefix-anchored, escaped $regex query
 *   7.  whitespace-only search is ignored
 *   8.  malicious regex input is escaped (no raw metacharacters)
 *
 * buildSessionParams():
 *   9.  sorts by -startTime (newest first)
 *  10.  search adds SUPI $regex query
 *
 * totalFromOtherInfo():
 *  11.  reads x-total-count header
 *  12.  missing header → undefined
 *  13.  non-numeric header → undefined
 *  14.  null otherInfo → undefined
 *  15.  "0" → 0
 */

'use strict';

const {
  escapeRegex,
  WINDOW_BUCKET_MS,
  windowIso,
  buildSessionFilter,
  buildSubscriberParams,
  buildSessionParams,
  countParams,
  totalFromOtherInfo,
  buildPoolParams,
} = require('../src/helpers/chf-query');

describe('escapeRegex', () => {
  test('1. plain digits unchanged', () => {
    expect(escapeRegex('imsi-99970')).toBe('imsi-99970');
  });
  test('2. metacharacters neutralised', () => {
    expect(escapeRegex('.*+?^${}()|[]\\')).toBe(
      '\\.\\*\\+\\?\\^\\$\\{\\}\\(\\)\\|\\[\\]\\\\'
    );
  });
  test('3. null/undefined → empty string', () => {
    expect(escapeRegex(null)).toBe('');
    expect(escapeRegex(undefined)).toBe('');
  });
});

describe('buildSubscriberParams', () => {
  test('4. page 0 defaults', () => {
    expect(buildSubscriberParams(0, 25, '')).toEqual({
      limit: 25,
      skip: 0,
      sort: 'supi',
    });
  });
  test('5. skip = page * pageSize', () => {
    expect(buildSubscriberParams(2, 25, '').skip).toBe(50);
  });
  test('6. search adds prefix-anchored escaped query, for own buckets and pools', () => {
    const p = buildSubscriberParams(0, 25, 'imsi-99970');
    /* A pool has no supi; it lists the SIM among its members. */
    expect(JSON.parse(p.query)).toEqual({ $or: [
      { supi: { $regex: '^imsi-99970' } },
      { members: { $regex: '^imsi-99970' } },
      { bucketId: { $regex: '^imsi-99970' } },
    ] });
  });
  test('7. whitespace-only search ignored', () => {
    expect(buildSubscriberParams(0, 25, '   ').query).toBeUndefined();
  });
  test('8. malicious regex input escaped', () => {
    const p = buildSubscriberParams(0, 25, '.*(a+)+$');
    const parsed = JSON.parse(p.query);
    for (const clause of parsed.$or) {
      const re = (clause.supi || clause.members || clause.bucketId).$regex;
      expect(re).toBe('^\\.\\*\\(a\\+\\)\\+\\$');
      // no unescaped metacharacters remain after the anchor
      expect(re).not.toMatch(/[^\\][.*+?(){}|[\]]/);
    }
  });
});

describe('buildSessionParams', () => {
  test('9. sorts newest-first', () => {
    expect(buildSessionParams(0, 25, '').sort).toBe('-startTime');
  });
  test('10. search adds SUPI query', () => {
    const p = buildSessionParams(1, 10, 'imsi-001');
    expect(p.skip).toBe(10);
    expect(JSON.parse(p.query)).toEqual({ supi: { $regex: '^imsi-001' } });
  });
});

describe('windowIso / buildSessionFilter', () => {
  const NOW = Date.UTC(2026, 8, 5, 15, 0, 0); // 2026-09-05T15:00:00Z

  test('16. windowIso subtracts hours', () => {
    expect(windowIso(1, NOW)).toBe('2026-09-05T14:00:00.000Z');
    expect(windowIso(6, NOW)).toBe('2026-09-05T09:00:00.000Z');
  });
  test('16a. windowIso is stable within a bucket', () => {
    /* Params are the redux crud cache key.  If this bound moved with every
     * call, no fetch would ever match a cached collection and the session
     * containers would re-dispatch on every store update. */
    expect(windowIso(1, NOW + 1)).toBe(windowIso(1, NOW));
    expect(windowIso(1, NOW + WINDOW_BUCKET_MS - 1)).toBe(windowIso(1, NOW));
  });
  test('16b. windowIso advances at the next bucket', () => {
    expect(windowIso(1, NOW + WINDOW_BUCKET_MS))
      .toBe('2026-09-05T14:01:00.000Z');
  });
  test('16c. buildSessionParams is deep-equal across calls in a bucket', () => {
    expect(buildSessionParams(0, 25, '', 1, NOW))
      .toEqual(buildSessionParams(0, 25, '', 1, NOW + 999));
    expect(countParams('session', '', 1, NOW))
      .toEqual(countParams('session', '', 1, NOW + 999));
  });
  test('17. filter: window only → startTime $gte', () => {
    expect(buildSessionFilter('', 1, NOW)).toEqual({
      startTime: { $gte: '2026-09-05T14:00:00.000Z' },
    });
  });
  test('18. filter: search only → supi regex', () => {
    expect(buildSessionFilter('imsi-1', 0, NOW)).toEqual({
      supi: { $regex: '^imsi-1' },
    });
  });
  test('19. filter: search + window → both', () => {
    expect(buildSessionFilter('imsi-1', 1, NOW)).toEqual({
      supi: { $regex: '^imsi-1' },
      startTime: { $gte: '2026-09-05T14:00:00.000Z' },
    });
  });
  test('20. filter: neither → undefined', () => {
    expect(buildSessionFilter('', 0, NOW)).toBeUndefined();
  });
  test('21. buildSessionParams applies window', () => {
    const p = buildSessionParams(0, 25, '', 1, NOW);
    expect(p.sort).toBe('-startTime');
    expect(JSON.parse(p.query)).toEqual({
      startTime: { $gte: '2026-09-05T14:00:00.000Z' },
    });
  });
});

describe('countParams', () => {
  test('22. session count with window', () => {
    const NOW = Date.UTC(2026, 8, 5, 15, 0, 0);
    const p = countParams('session', '', 1, NOW);
    expect(p.limit).toBe(1);
    expect(JSON.parse(p.query)).toEqual({
      startTime: { $gte: '2026-09-05T14:00:00.000Z' },
    });
  });
  test('23. subscriber count, no filter → no query', () => {
    const p = countParams('subscriber', '', 0);
    expect(p).toEqual({ limit: 1 });
  });
  test('24. subscriber count with search', () => {
    const p = countParams('subscriber', 'imsi-99', 0);
    expect(JSON.parse(p.query)).toEqual({ $or: [
      { supi: { $regex: '^imsi-99' } }, { members: { $regex: '^imsi-99' } },
      { bucketId: { $regex: '^imsi-99' } }] });
  });
});

describe('totalFromOtherInfo', () => {
  test('11. reads x-total-count', () => {
    expect(totalFromOtherInfo({ headers: { 'x-total-count': '4200' } })).toBe(
      4200
    );
  });
  test('12. missing header → undefined', () => {
    expect(totalFromOtherInfo({ headers: {} })).toBeUndefined();
  });
  test('13. non-numeric → undefined', () => {
    expect(
      totalFromOtherInfo({ headers: { 'x-total-count': 'nope' } })
    ).toBeUndefined();
  });
  test('14. null otherInfo → undefined', () => {
    expect(totalFromOtherInfo(null)).toBeUndefined();
    expect(totalFromOtherInfo(undefined)).toBeUndefined();
  });
  test('15. "0" → 0', () => {
    expect(totalFromOtherInfo({ headers: { 'x-total-count': '0' } })).toBe(0);
  });
});

describe('buildPoolParams', () => {
  test('25. the CHF Shared Pools view lists pools only, by id', () => {
    const p = buildPoolParams(1, 25, '');
    expect(p).toMatchObject({ limit: 25, skip: 25, sort: 'bucketId' });
    expect(JSON.parse(p.query)).toEqual({ members: { $exists: true } });
  });
  test('26. and finds them by a member\'s SUPI, escaped', () => {
    expect(JSON.parse(buildPoolParams(0, 25, 'imsi-999.').query)).toEqual({
      members: { $exists: true },
      $or: [{ bucketId: { $regex: '^imsi-999\\.' } },
            { members: { $regex: '^imsi-999\\.' } }] });
  });
  test('27. or by the pool\'s own id', () => {
    const q = JSON.parse(buildPoolParams(0, 25, 'pool-d8e1').query);
    expect(q.$or[0]).toEqual({ bucketId: { $regex: '^pool-d8e1' } });
    expect(q.members).toEqual({ $exists: true });
  });
  test('28. the CHF Quota search finds a pool by its id too', () => {
    const q = JSON.parse(buildSubscriberParams(0, 25, 'pool-d8e1').query);
    expect(q.$or).toContainEqual({ bucketId: { $regex: '^pool-d8e1' } });
  });
});
