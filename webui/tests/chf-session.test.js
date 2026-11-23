/*
 * Unit tests for the ChfSession Mongoose model.
 *
 * Tests cover:
 *   1. Valid session document passes validation
 *   2. ulOctets, dlOctets, totalOctets and duration default to 0
 *   3. grantedTotalBytes is stored correctly (matches db-handler.c field name)
 *   4. autoRenew and periodLengthSec are stored correctly
 *
 * No MongoDB connection is required — only schema-level validation.
 *
 * Schema alignment: db-handler.c writes these fields to chf.sessions:
 *   chargingDataRef, supi, gpsi, dnn, pduSessionId,
 *   ulOctets, dlOctets, totalOctets, duration,
 *   grantedTotalBytes, autoRenew, periodLengthSec,
 *   startTime, updatedAt
 */

'use strict';

const ChfSession = require('../server/models/chf-session');

describe('ChfSession model', () => {
  function makeDoc(overrides) {
    return new ChfSession(
      Object.assign(
        {
          chargingDataRef:   'ref-abc-001',
          supi:              'imsi-001010000000001',
          gpsi:              'msisdn-447700900001',
          dnn:               'internet',
          pduSessionId:      1,
          startTime:         new Date(),
          ulOctets:          0,
          dlOctets:          0,
          totalOctets:       0,
          duration:          0,
          grantedTotalBytes: 10737418240,   /* 10 GiB */
          autoRenew:         true,
          periodLengthSec:   2592000,
        },
        overrides
      )
    );
  }

  test('1. valid session document passes validation', async () => {
    const doc = makeDoc({});
    await expect(doc.validate()).resolves.toBeUndefined();
  });

  test('2. ulOctets, dlOctets, totalOctets and duration default to 0', () => {
    const doc = new ChfSession({
      chargingDataRef: 'ref-abc-002',
      supi: 'imsi-001010000000003',
    });
    expect(doc.ulOctets).toBe(0);
    expect(doc.dlOctets).toBe(0);
    expect(doc.totalOctets).toBe(0);
    expect(doc.duration).toBe(0);
  });

  test('3. grantedTotalBytes is stored correctly', () => {
    const BYTES = 5 * 1073741824;   /* 5 GiB */
    const doc = makeDoc({ grantedTotalBytes: BYTES });
    expect(doc.grantedTotalBytes).toBe(BYTES);
  });

  test('4. autoRenew and periodLengthSec are stored correctly', () => {
    const doc = makeDoc({ autoRenew: false, periodLengthSec: 86400 });
    expect(doc.autoRenew).toBe(false);
    expect(doc.periodLengthSec).toBe(86400);
  });
});
