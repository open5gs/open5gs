/*
 * Mapping between a CHF subscriber document (chf.subscribers) and a TMF654
 * Bucket, plus the top-up rule.  Pure except applyTopup, which only touches
 * storage through a repository - so all of it is unit-testable.
 *
 * A Bucket here is a data balance: one SUPI's, or a pool's, which several
 * SIMs share (isShared, and one logicalResource per SIM).  It is published
 * as @type "ChfDataBucket" / @baseType "Bucket" (the TMF630 polymorphism
 * pattern), carrying the standard Bucket attributes plus the plan attributes
 * TMF654 leaves to extensions.
 *
 * Money rules, which must agree with the CHF NF (src/chf):
 *
 *  - remainingValue is what the CHF would grant at the subscriber's NEXT
 *    session, derived with the same cycle rules as chf_quota_period_expired()
 *    and chf_quota_remaining().  An expired cycle that auto-renews therefore
 *    shows the full allowance; one that does not shows zero.
 *
 *  - usage.* (consumedBytes, periodStart, topupBytes) is the CHF's live
 *    billing counter.  A Bucket PATCH never writes it.  The only API path that
 *    does is a top-up, and only through an atomic conditional update.
 *
 * Copyright (C) 2026 by Rami Mohamed <ramrode@gmail.com>
 */

'use strict';

const C = require('./common');
/* cycleState / nextGrant live in the helper the CHF Quota page also uses, so
 * the page and this API cannot publish different balances. */
const {
  BYTES_PER_MB, BYTES_PER_GB, BYTES_PER_TB, PERIOD_SECONDS,
  cycleState, nextGrant,
} = require('../../src/helpers/chf-quota-convert');

const SUPI_RE = /^imsi-\d{15}$/;
const PERIODS = ['daily', 'weekly', 'monthly', 'custom'];
const FINAL_UNIT_ACTIONS = ['TERMINATE', 'REDIRECT', 'RESTRICT_ACCESS'];

/* Server-owned Bucket attributes; setting any of them is a 400. */
const READ_ONLY = ['id', 'href', '@type', '@baseType', '@schemaLocation',
  'name', 'status', 'usageType', 'isShared', 'remainingValue', 'consumedValue',
  'topupValue', 'validFor'];

/* Writable attributes on PATCH.  logicalResource only on a pool, where it
 * is the list of members; a SIM's own bucket keeps its SUPI. */
const PATCHABLE = ['allowance', 'validityPeriod', 'validitySeconds',
    'scope', 'priority',
  'autoRenew', 'finalUnitAction', 'partyAccount', 'logicalResource'];

/* What the CHF charges a pool to when chf.settings says nothing: the
 * shipped charging.pool.max_members. */
const DEFAULT_POOL_MAX_MEMBERS = 10;

/* Largest byte count a JSON number carries exactly. */
const MAX_OCTETS = Number.MAX_SAFE_INTEGER;

const UNIT_FACTORS = {
  octet: 1, octets: 1, byte: 1, bytes: 1, b: 1,
  mb: BYTES_PER_MB, gb: BYTES_PER_GB, tb: BYTES_PER_TB,
};

/*
 * TMF Quantity {amount, units} -> integer octets.  Binary multiples (1 GB =
 * 2^30), matching the CHF Quota page.  Responses always use "octet".
 */
function quantityToOctets(q, attr, { allowZero = true } = {}) {
  if (!C.isPlainObject(q))
    throw new C.TmfError(400, 'Bad Request',
        `'${attr}' must be a Quantity {amount, units}`);
  const factor = UNIT_FACTORS[String(q.units || 'octet').toLowerCase()];
  if (!factor)
    throw new C.TmfError(400, 'Bad Request',
        `'${attr}.units' must be one of: octet, MB, GB, TB`);
  const amount = Number(q.amount);
  if (!Number.isFinite(amount) || amount < 0 || (!allowZero && amount === 0))
    throw new C.TmfError(400, 'Bad Request',
        `'${attr}.amount' must be a ${allowZero ? 'non-negative' : 'positive'} number`);
  const octets = Math.round(amount * factor);
  if (octets > MAX_OCTETS)
    throw new C.TmfError(400, 'Bad Request', `'${attr}' is too large`);
  return octets;
}

const octets = amount => ({ amount, units: 'octet' });

/* ------------------------------------------------------------------ */
/* Representation                                                       */
/* ------------------------------------------------------------------ */

function toBucket(req, doc, now) {
  const quota = doc.quota || {};
  const usage = doc.usage || {};
  const state = cycleState(doc, now);
  const accountBase =
    `${req.protocol}://${req.get('host')}${req.baseUrl}`
      .replace(/\/prepayBalanceManagement\/v4$/, '/accountManagement/v4');

  const id = bucketId(doc);
  const pool = isPool(doc);
  const bucket = {
    id,
    href: C.hrefFor(req, 'bucket', id),
    '@type': 'ChfDataBucket',
    '@baseType': 'Bucket',
    name: pool
      ? `Shared data allowance, ${doc.members.length} SIMs${scopeSuffix(doc)}`
      : `Data allowance ${doc.supi}${scopeSuffix(doc)}`,
    usageType: 'data',
    isShared: pool,
    status: state,
    remainingValue: octets(nextGrant(doc, now)),
    logicalResource: membersOf(doc).map(supi => ({
      id: supi, value: supi, '@referredType': 'SUPI' })),

    /* Extension attributes: the plan behind the balance. */
    allowance: octets(Number(quota.totalQuotaBytes) || 0),
    consumedValue: octets(state === 'active'
        ? Number(usage.consumedBytes) || 0 : 0),
    topupValue: octets(state === 'active'
        ? Number(usage.topupBytes) || 0 : 0),
    validityPeriod: quota.validityPeriod || 'monthly',
    validitySeconds: Number(quota.validitySeconds) || 0,
    autoRenew: quota.autoRenew === true,
    finalUnitAction: quota.finalUnitAction || 'TERMINATE',

    /* What this allowance covers, and which bucket wins when several do.
     * An absent member is open. */
    scope: toScope(doc),
    priority: Number(doc.priority) || 0,
  };

  if (state !== 'notStarted') {
    const start = new Date(usage.periodStart);
    bucket.validFor = { startDateTime: start.toISOString() };
    if (bucket.validitySeconds > 0)
      bucket.validFor.endDateTime =
        new Date(start.getTime() + bucket.validitySeconds * 1000).toISOString();
  }

  if (doc.billingAccountId)
    bucket.partyAccount = {
      id: doc.billingAccountId,
      href: `${accountBase}/billingAccount/${encodeURIComponent(doc.billingAccountId)}`,
      '@referredType': 'BillingAccount',
    };

  return bucket;
}

/* ------------------------------------------------------------------ */
/* Bucket identity and scope                                            */
/* ------------------------------------------------------------------ */

const ROAMING_STATES = ['home', 'inBound', 'outBound'];
const SD_RE = /^[0-9a-fA-F]{6}$/;
const PLMN_RE = /^\d{3}-\d{2,3}$/;
const DNN_RE = /^[A-Za-z0-9][A-Za-z0-9.\-]{0,99}$/;

/* The catch-all bucket is addressed by its SUPI, as it was before buckets
 * existed; a scoped one, and a pool, carry their own id. */
function bucketId(doc) {
  return doc.bucketId || doc.supi;
}

/* A pool is a bucket with a members array; it has no supi of its own. */
function isPool(doc) {
  return Array.isArray(doc.members) && doc.members.length > 0;
}

/* The SIMs that draw on a bucket. */
function membersOf(doc) {
  return isPool(doc) ? doc.members.slice() : [doc.supi];
}

function hasScope(doc) {
  const sc = doc.scope || {};
  return !!(sc.dnn || sc.hplmn || sc.roaming ||
      (sc.snssai && sc.snssai.sst !== undefined && sc.snssai.sst !== null));
}

/* Stored scope -> the API shape, leaving out what is open. */
function toScope(doc) {
  const sc = doc.scope || {};
  const out = {};
  if (sc.snssai && sc.snssai.sst !== undefined && sc.snssai.sst !== null) {
    out.sNSSAI = { sst: Number(sc.snssai.sst) };
    if (sc.snssai.sd) out.sNSSAI.sd = sc.snssai.sd;
  }
  if (sc.dnn) out.dnn = sc.dnn;
  if (sc.hplmn) out.hPlmnId = plmnToObject(sc.hplmn);
  if (sc.roaming) out.roaming = sc.roaming;
  return out;
}

function plmnToObject(str) {
  const [mcc, mnc] = String(str).split('-');
  return { mcc, mnc };
}

/* A short human tag for the bucket name: "(slice 1, dnn ims)". */
function scopeSuffix(doc) {
  const sc = toScope(doc);
  const parts = [];
  if (sc.sNSSAI)
    parts.push(`slice ${sc.sNSSAI.sst}${sc.sNSSAI.sd ? '/' + sc.sNSSAI.sd : ''}`);
  if (sc.dnn) parts.push(`dnn ${sc.dnn}`);
  if (sc.hPlmnId) parts.push(`plmn ${sc.hPlmnId.mcc}-${sc.hPlmnId.mnc}`);
  if (sc.roaming) parts.push(sc.roaming);
  return parts.length ? ` (${parts.join(', ')})` : '';
}

/*
 * Request scope -> stored scope.  Everything is optional; what is left
 * out stays open.  Returns null when the request names no dimension,
 * which makes the bucket the subscriber's catch-all.
 */
function scopeToStored(value) {
  if (value === undefined || value === null) return null;
  if (!C.isPlainObject(value))
    throw new C.TmfError(400, 'Bad Request', "'scope' must be an object");

  const known = ['sNSSAI', 'dnn', 'hPlmnId', 'roaming'];
  const unknown = Object.keys(value).filter(k => known.indexOf(k) === -1);
  if (unknown.length)
    throw new C.TmfError(400, 'Bad Request',
        `Unknown scope attribute(s): ${unknown.join(', ')}`);

  const stored = {};

  if (value.sNSSAI !== undefined && value.sNSSAI !== null) {
    const s = value.sNSSAI;
    if (!C.isPlainObject(s) || !Number.isInteger(s.sst) ||
        s.sst < 0 || s.sst > 255)
      throw new C.TmfError(400, 'Bad Request',
          "'scope.sNSSAI.sst' must be an integer between 0 and 255");
    stored.snssai = { sst: s.sst };
    if (s.sd !== undefined && s.sd !== null) {
      if (typeof s.sd !== 'string' || !SD_RE.test(s.sd))
        throw new C.TmfError(400, 'Bad Request',
            "'scope.sNSSAI.sd' must be 6 hex digits");
      stored.snssai.sd = s.sd.toLowerCase();
    }
  }

  if (value.dnn !== undefined && value.dnn !== null) {
    if (typeof value.dnn !== 'string' || !DNN_RE.test(value.dnn))
      throw new C.TmfError(400, 'Bad Request', "'scope.dnn' is not a DNN");
    stored.dnn = value.dnn;
  }

  if (value.hPlmnId !== undefined && value.hPlmnId !== null) {
    const p = value.hPlmnId;
    if (!C.isPlainObject(p) || typeof p.mcc !== 'string' ||
        typeof p.mnc !== 'string' || !PLMN_RE.test(`${p.mcc}-${p.mnc}`))
      throw new C.TmfError(400, 'Bad Request',
          "'scope.hPlmnId' must be { mcc: '3 digits', mnc: '2-3 digits' }");
    stored.hplmn = `${p.mcc}-${p.mnc}`;
  }

  if (value.roaming !== undefined && value.roaming !== null) {
    if (ROAMING_STATES.indexOf(value.roaming) === -1)
      throw new C.TmfError(400, 'Bad Request',
          `'scope.roaming' must be one of: ${ROAMING_STATES.join(', ')}`);
    stored.roaming = value.roaming;
  }

  return Object.keys(stored).length ? stored : null;
}

/* Priority: an integer, higher wins. */
function priorityOf(value) {
  if (value === undefined || value === null) return 0;
  if (!Number.isInteger(value))
    throw new C.TmfError(400, 'Bad Request', "'priority' must be an integer");
  return value;
}

/* ------------------------------------------------------------------ */
/* Plan attributes -> quota.* storage                                   */
/* ------------------------------------------------------------------ */

/*
 * Plan attributes -> the quota.* storage paths the CHF reads, as a $set map.
 *
 * On create every path is written, with defaults for what the request omits.
 * On PATCH only the paths the request names are written.  That distinction
 * matters: the CHF treats an ABSENT autoRenew as "no renewal", so filling in
 * defaults on a PATCH that only changed, say, finalUnitAction would silently
 * switch renewal on for a document that never had the field.
 *
 * @param body     the request body (plan attributes)
 * @param stored   the stored quota sub-document (PATCH) or null (create)
 * @returns        { 'quota.totalQuotaBytes': n, ... }
 */
function planToQuotaSet(body, stored) {
  const create = !stored;
  const set = {};
  const put = (attr, value) => { set[`quota.${attr}`] = value; };

  if (body.allowance !== undefined)
    put('totalQuotaBytes', quantityToOctets(body.allowance, 'allowance'));
  else if (create)
    throw new C.TmfError(400, 'Bad Request', "'allowance' is required");

  /* validityPeriod and validitySeconds are resolved together: a preset
   * period dictates the seconds, and only 'custom' takes them from the
   * request. */
  if (create || body.validityPeriod !== undefined ||
      body.validitySeconds !== undefined) {
    const period = body.validityPeriod !== undefined ? body.validityPeriod
      : (stored && stored.validityPeriod) || 'monthly';
    if (PERIODS.indexOf(period) === -1)
      throw new C.TmfError(400, 'Bad Request',
          `'validityPeriod' must be one of: ${PERIODS.join(', ')}`);

    let seconds;
    if (period === 'custom') {
      seconds = body.validitySeconds !== undefined
        ? Number(body.validitySeconds)
        : Number(stored && stored.validitySeconds);
      if (!Number.isInteger(seconds) || seconds <= 0)
        throw new C.TmfError(400, 'Bad Request',
            "A 'custom' validityPeriod requires a positive integer 'validitySeconds'");
    } else {
      /* Presets are authoritative; a stray validitySeconds is ignored, as
       * the CHF Quota page does. */
      seconds = PERIOD_SECONDS[period];
    }
    put('validityPeriod', period);
    put('validitySeconds', seconds);
  }

  if (body.autoRenew !== undefined) {
    if (typeof body.autoRenew !== 'boolean')
      throw new C.TmfError(400, 'Bad Request', "'autoRenew' must be a boolean");
    put('autoRenew', body.autoRenew);
  } else if (create) {
    put('autoRenew', true);
  }

  if (body.finalUnitAction !== undefined) {
    if (FINAL_UNIT_ACTIONS.indexOf(body.finalUnitAction) === -1)
      throw new C.TmfError(400, 'Bad Request',
          `'finalUnitAction' must be one of: ${FINAL_UNIT_ACTIONS.join(', ')}`);
    put('finalUnitAction', body.finalUnitAction);
  } else if (create) {
    put('finalUnitAction', 'TERMINATE');
  }

  return set;
}

/* The SUPI a new bucket is for, taken from logicalResource. */
function supiFromLogicalResource(body) {
  const lr = body.logicalResource;
  const first = Array.isArray(lr) && lr.length ? lr[0] : null;
  const supi = first && (first.id || first.value);
  if (typeof supi !== 'string' || !SUPI_RE.test(supi))
    throw new C.TmfError(400, 'Bad Request',
        "'logicalResource[0].id' must be a SUPI of the form imsi-<15 digits>");
  return supi;
}

/*
 * Every SUPI in logicalResource, in order: one for a SIM's own bucket,
 * several for a pool.  Each must be a SUPI, and none may repeat.
 */
function supisFromLogicalResource(lr) {
  if (!Array.isArray(lr) || !lr.length)
    throw new C.TmfError(400, 'Bad Request',
        "'logicalResource' must list at least one SIM");
  const supis = lr.map((item, i) => {
    const supi = C.isPlainObject(item) && (item.id || item.value);
    if (typeof supi !== 'string' || !SUPI_RE.test(supi))
      throw new C.TmfError(400, 'Bad Request',
          `'logicalResource[${i}].id' must be a SUPI of the form imsi-<15 digits>`);
    return supi;
  });
  const twice = supis.find((s, i) => supis.indexOf(s) !== i);
  if (twice)
    throw new C.TmfError(400, 'Bad Request',
        `'${twice}' is listed more than once in 'logicalResource'`);
  return supis;
}

/* partyAccount -> billing account id; null clears the link. */
function partyAccountId(value) {
  if (value === null) return null;
  if (!C.isPlainObject(value) || typeof value.id !== 'string' || !value.id)
    throw new C.TmfError(400, 'Bad Request',
        "'partyAccount' must be an object with a string 'id'");
  return value.id;
}

/* ------------------------------------------------------------------ */
/* Top-up                                                               */
/* ------------------------------------------------------------------ */

const TOPUP_ATTEMPTS = 3;

/*
 * Add `bytes` to a subscriber's balance for the current billing cycle.
 *
 *  - active cycle:        usage.topupBytes += bytes (atomic $inc).
 *  - no cycle yet, or an expired one: open a new cycle now with the top-up
 *    already in it (consumed 0, topup = bytes, periodStart = now).  A top-up
 *    on an expired balance therefore reactivates it - including on a plan
 *    that does not auto-renew, which has no other way back.
 *
 * The CHF advances the same document concurrently: it $incs consumedBytes on
 * every usage report and re-stamps periodStart when it renews a cycle.  So
 * every write is conditional on the periodStart this call just read.  If the
 * CHF renewed the cycle in between, the condition misses and the decision is
 * re-made against the new cycle, rather than applying the top-up to a cycle
 * that no longer exists.  consumedBytes is never read-modified-written.
 *
 * Resolves to { outcome: 'applied', reactivated } | { outcome: 'notFound' }
 * | { outcome: 'conflict' } (the cycle kept moving on every attempt).
 */
async function applyTopup(repo, bucket, bytes, now) {
  for (let attempt = 0; attempt < TOPUP_ATTEMPTS; attempt++) {
    const doc = await repo.get(bucket);
    if (!doc) return { outcome: 'notFound' };

    const usage = doc.usage || {};
    const stamp = new Date(now);
    /* Conditional on the cycle this attempt read, so a renewal by the CHF
     * in between is noticed rather than overwritten. */
    const filter = doc.bucketId ? { bucketId: doc.bucketId }
                                : { supi: doc.supi };
    filter['usage.periodStart'] =
        usage.periodStart == null ? null : usage.periodStart;

    const active = cycleState(doc, now) === 'active';
    const update = active
      ? { $inc: { 'usage.topupBytes': bytes },
          $set: { 'usage.updatedAt': stamp } }
      : { $set: { 'usage.consumedBytes': 0, 'usage.topupBytes': bytes,
                  'usage.periodStart': stamp, 'usage.updatedAt': stamp } };

    if (await repo.update(filter, update) === 1)
      return { outcome: 'applied', reactivated: !active };
  }
  return { outcome: 'conflict' };
}

module.exports = {
  bucketId,
  isPool,
  membersOf,
  supisFromLogicalResource,
  DEFAULT_POOL_MAX_MEMBERS,
  hasScope,
  toScope,
  scopeToStored,
  priorityOf,
  SUPI_RE,
  READ_ONLY,
  PATCHABLE,
  quantityToOctets,
  cycleState,
  nextGrant,
  toBucket,
  planToQuotaSet,
  supiFromLogicalResource,
  partyAccountId,
  applyTopup,
};
