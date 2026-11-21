/*
 * CHF quota unit-conversion helpers.
 *
 * Shared between Document.js (container logic) and unit tests.
 *
 * The CHF Mongoose model (chf-subscriber.js) stores:
 *   quota.totalQuotaBytes  — total (UL+DL) quota in bytes
 *   quota.validityPeriod   — 'daily' | 'weekly' | 'monthly' | 'custom'
 *   quota.validitySeconds  — canonical period length (authoritative for C)
 *   quota.autoRenew        — boolean
 *   quota.finalUnitAction  — 'TERMINATE' | 'REDIRECT' | 'RESTRICT_ACCESS'
 *
 * The Edit form shows:
 *   quota.totalVolume      — numeric value (the user enters)
 *   quota.volumeUnit       — 'MB' | 'GB' | 'TB'
 *   quota.validityPeriod   — dropdown value
 *   quota.validitySeconds  — visible only when validityPeriod === 'custom'
 *   quota.autoRenew        — checkbox
 *   quota.finalUnitAction  — dropdown
 *
 * Binary prefixes (IEC 80000-13):
 *   1 MB = 1 048 576 bytes   (2^20)
 *   1 GB = 1 073 741 824 bytes (2^30)
 *   1 TB = 1 099 511 627 776 bytes (2^40)
 *
 * Max safe integer for MongoDB Number (IEEE-754 double): 2^53 − 1
 * (~9 PB as bytes) — comfortably covers any realistic quota.
 *
 * 3GPP: TS 32.291 §6.3.1.2 — GrantedUnit / FinalUnitIndication
 *       TS 32.291 §6.3.2.2 — TIME_LIMIT trigger / validityTime
 */

'use strict';

const BYTES_PER_MB = 1048576;           /* 2^20 */
const BYTES_PER_GB = 1073741824;        /* 2^30 */
const BYTES_PER_TB = 1099511627776;     /* 2^40 */

/** Seconds in each validity period preset */
const PERIOD_SECONDS = {
  daily:   86400,
  weekly:  604800,
  monthly: 2592000,   /* 30 days */
};

/** Fallbacks used whenever a document or form omits a quota field. */
const DEFAULT_PERIOD  = 'monthly';
const DEFAULT_SECONDS = PERIOD_SECONDS[DEFAULT_PERIOD];
const DEFAULT_BYTES   = BYTES_PER_GB * 10;

/*
 * Form state shown for a new subscriber, and the fallback whenever a
 * document carries no quota.  Exported so Document.js uses this one
 * definition rather than keeping a second copy in sync by hand.
 * Frozen because it is shared by reference across every caller.
 */
const DEFAULT_SCOPE_FORM = Object.freeze({
  sst: '', sd: '', dnn: '', hplmn: '', roaming: '',
});

const DEFAULT_QUOTA_FORM = Object.freeze({
  priority: 0,
  scopeForm: DEFAULT_SCOPE_FORM,
  quota: Object.freeze({
    totalVolume:     10,
    volumeUnit:      'GB',
    validityPeriod:  DEFAULT_PERIOD,
    validitySeconds: DEFAULT_SECONDS,
    autoRenew:       true,
    finalUnitAction: 'TERMINATE',
  }),
});

/* A new shared pool: the same plan defaults, and its SIMs, one per line. */
const DEFAULT_POOL_FORM = Object.freeze(
    Object.assign({}, DEFAULT_QUOTA_FORM, { membersText: '' }));

/** Round to N decimal places and strip trailing zeros. */
function round(v, decimals) {
  return parseFloat(v.toFixed(decimals));
}

/**
 * bytesToDisplay — choose the best unit (MB / GB / TB) and value for a
 * byte count so it renders as a small, readable number.
 *
 * @param {number} bytes
 * @returns {{ value: number, unit: 'MB'|'GB'|'TB' }}
 */
function bytesToDisplay(bytes) {
  if (bytes == null || bytes < 0) return { value: 0, unit: 'GB' };
  if (bytes >= BYTES_PER_TB) return { value: round(bytes / BYTES_PER_TB, 6), unit: 'TB' };
  if (bytes >= BYTES_PER_GB) return { value: round(bytes / BYTES_PER_GB, 6), unit: 'GB' };
  return { value: round(bytes / BYTES_PER_MB, 6), unit: 'MB' };
}

/**
 * displayToBytes — convert a numeric display value + unit back to bytes.
 *
 * @param {number} value
 * @param {'MB'|'GB'|'TB'} unit
 * @returns {number}  byte count (integer)
 */
function displayToBytes(value, unit) {
  const v = value != null ? value : 0;
  const factor = unit === 'TB' ? BYTES_PER_TB
               : unit === 'GB' ? BYTES_PER_GB
               : BYTES_PER_MB;
  return Math.round(v * factor);
}

/**
 * dbToForm — convert a raw MongoDB ChfSubscriber document to the form
 * state expected by Edit.js.
 *
 * Handles both the new unified schema (totalQuotaBytes) and legacy documents
 * that still have volumeUlBytes + volumeDlBytes.
 *
 * @param {object} data  Raw Mongoose document (may be null/undefined)
 * @returns {object}     Form-ready object
 */
function dbToForm(data) {
  if (!data || !data.quota)
    return Object.assign({}, DEFAULT_QUOTA_FORM,
        data ? { scopeForm: scopeToForm(data), priority: data.priority || 0 }
             : {});

  const q = data.quota;

  /* Resolve total bytes — new field takes precedence over legacy UL+DL sum */
  let totalBytes;
  if (q.totalQuotaBytes != null) {
    totalBytes = q.totalQuotaBytes;
  } else if (q.volumeUlBytes != null || q.volumeDlBytes != null) {
    totalBytes = (q.volumeUlBytes || 0) + (q.volumeDlBytes || 0);
  } else {
    totalBytes = DEFAULT_BYTES;
  }

  const { value, unit } = bytesToDisplay(totalBytes);

  /* Resolve validity period */
  const period  = q.validityPeriod || DEFAULT_PERIOD;
  const seconds = (q.validitySeconds != null)
                  ? q.validitySeconds
                  : (PERIOD_SECONDS[period] || DEFAULT_SECONDS);

  return {
    ...data,
    /* A pool's SIMs, one per line, for the read-only field the form shows. */
    ...(Array.isArray(data.members)
      ? { membersText: data.members.join('\n') } : {}),
    quota: {
      totalVolume:     value,
      volumeUnit:      unit,
      validityPeriod:  period,
      validitySeconds: seconds,
      autoRenew:       (q.autoRenew != null) ? q.autoRenew : true,
      finalUnitAction: q.finalUnitAction || 'TERMINATE',
    },
    /* The scope fields the form edits, and which bucket wins. */
    scopeForm: scopeToForm(data),
    priority: Number(data.priority) || 0,
  };
}

/**
 * formToDb — convert Edit.js form state back to a raw MongoDB document.
 *
 * Writes totalQuotaBytes in bytes and resolves validitySeconds from
 * the validityPeriod preset (or uses the user-entered value for 'custom').
 *
 * @param {object} formData  Form state from Edit.js
 * @returns {object}         Document ready for the Mongoose REST API
 */
function formToDb(formData) {
  const q      = formData.quota || {};
  const unit   = q.volumeUnit   || 'GB';
  const period = q.validityPeriod || DEFAULT_PERIOD;

  /* Resolve canonical validitySeconds: presets come from the table, only
   * 'custom' takes the user-entered value. */
  const validitySeconds = period === 'custom'
    ? ((q.validitySeconds != null) ? q.validitySeconds : DEFAULT_SECONDS)
    : (PERIOD_SECONDS[period] || DEFAULT_SECONDS);

  /*
   * Never send usage back.  It is the CHF's live billing-cycle counter, and
   * formData only holds the snapshot fetched when the edit modal opened
   * (dbToForm spreads the whole document through, and the form keeps extra
   * keys).  Writing it would rewind whatever the CHF counted in between.
   * The server strips it too (routes/db.js); this avoids sending it at all.
   *
   * _id / __v are Mongoose bookkeeping and are likewise not the form's to
   * write.
   */
  /* A pool's members are not the form's to write either: they change
   * through the TMF API, and the server keeps them as they are. */
  // eslint-disable-next-line no-unused-vars
  const { usage, _id, __v, scopeForm, membersText, members, ...rest } = formData;

  const scope = formToScope(scopeForm);
  const doc = {
    ...rest,
    quota: {
      totalQuotaBytes: displayToBytes(q.totalVolume, unit),
      validityPeriod:  period,
      validitySeconds: validitySeconds,
      autoRenew:       (q.autoRenew != null) ? q.autoRenew : true,
      finalUnitAction: q.finalUnitAction || 'TERMINATE',
    },
    priority: Number(formData.priority) || 0,
  };

  /* An empty scope means the bucket covers everything, so the member is
   * removed rather than stored empty - that is what the CHF reads as
   * open, and what a catch-all bucket looks like. */
  if (scope) doc.scope = scope;
  else delete doc.scope;

  doc.bucketId = bucketIdFor(doc, scope);

  return doc;
}

/* ------------------------------------------------------------------ */
/* Bucket scope                                                         */
/*                                                                      */
/* A bucket covers a scope: a slice, a DNN, a home PLMN, a roaming      */
/* state.  What it leaves out is open and matches anything, so a bucket */
/* with an empty scope is the subscriber's catch-all - which is what    */
/* every document was before buckets existed.                           */
/* ------------------------------------------------------------------ */

const ROAMING_LABELS = Object.freeze({
  home: 'home only',
  inBound: 'inbound roamers',
  outBound: 'roaming abroad',
});

/** Does this document name any dimension? */
function hasScope(doc) {
  const sc = (doc && doc.scope) || {};
  return !!(sc.dnn || sc.hplmn || sc.roaming ||
      (sc.snssai && sc.snssai.sst != null));
}

/**
 * scopeLabel — what a bucket covers, in words, for the list.
 *
 * @param {object} doc  ChfSubscriber document
 * @returns {string}    e.g. "slice 1/aabbcc · internet · roaming abroad"
 */
function scopeLabel(doc) {
  const sc = (doc && doc.scope) || {};
  const parts = [];
  if (sc.snssai && sc.snssai.sst != null)
    parts.push('slice ' + sc.snssai.sst + (sc.snssai.sd ? '/' + sc.snssai.sd : ''));
  if (sc.dnn) parts.push(sc.dnn);
  if (sc.hplmn) parts.push('PLMN ' + sc.hplmn);
  if (sc.roaming) parts.push(ROAMING_LABELS[sc.roaming] || sc.roaming);
  return parts.length ? parts.join(' \u00b7 ') : 'everything';
}

/**
 * formToScope — the edit form's scope fields to the stored shape.
 * Empty fields are left out, which leaves that dimension open.
 *
 * @param {object} form  { sst, sd, dnn, hplmn, roaming }
 * @returns {object|null}
 */
function formToScope(form) {
  const f = form || {};
  const scope = {};

  if (f.sst !== '' && f.sst != null && !Number.isNaN(Number(f.sst))) {
    scope.snssai = { sst: Number(f.sst) };
    if (f.sd) scope.snssai.sd = String(f.sd).toLowerCase();
  }
  if (f.dnn) scope.dnn = f.dnn;
  if (f.hplmn) scope.hplmn = f.hplmn;
  if (f.roaming) scope.roaming = f.roaming;

  return Object.keys(scope).length ? scope : null;
}

/** Stored scope back to the flat fields the form edits. */
function scopeToForm(doc) {
  const sc = (doc && doc.scope) || {};
  return {
    sst: sc.snssai && sc.snssai.sst != null ? String(sc.snssai.sst) : '',
    sd: (sc.snssai && sc.snssai.sd) || '',
    dnn: sc.dnn || '',
    hplmn: sc.hplmn || '',
    roaming: sc.roaming || '',
  };
}

/**
 * bucketIdFor — the id a bucket is stored and addressed under.
 *
 * The catch-all keeps the SUPI, so a subscriber with a single plan is
 * addressed exactly as before; a scoped bucket gets a random suffix.  An
 * existing bucket keeps the id it already has.
 */
function bucketIdFor(doc, scope) {
  if (doc && doc.bucketId) return doc.bucketId;
  if (!scope) return doc.supi;
  const suffix = Math.random().toString(16).slice(2, 14).padEnd(12, '0');
  return doc.supi + '-' + suffix;
}

/**
 * fmtVolume — format a byte count for display in list views.
 *
 * @param {number|null} bytes
 * @returns {string}  e.g. "10.00 GB" or "512.00 MB" or "1.50 TB" or "—"
 */
function fmtVolume(bytes) {
  if (bytes == null) return '—';
  const { value, unit } = bytesToDisplay(bytes);
  return value.toFixed(2) + ' ' + unit;
}

/**
 * cycleState — where a subscriber's billing cycle stands.
 *
 * Mirrors chf_quota_period_expired() in the CHF (src/chf/context.h): no
 * periodStart means the cycle has not been opened; a zero validitySeconds
 * means it never expires.
 *
 * @param {object} sub  ChfSubscriber document
 * @param {number} now  epoch ms
 * @returns {'notStarted'|'active'|'expired'}
 */
function cycleState(sub, now) {
  const usage = (sub && sub.usage) || {};
  const quota = (sub && sub.quota) || {};
  if (!usage.periodStart) return 'notStarted';
  const seconds = Number(quota.validitySeconds) || 0;
  if (seconds > 0 &&
      now >= new Date(usage.periodStart).getTime() + seconds * 1000)
    return 'expired';
  return 'active';
}

/**
 * nextGrant — the data the CHF would grant at the subscriber's next session.
 *
 * This is the one definition of "remaining balance", shared by the CHF Quota
 * page and the TMF654 Bucket API (server/tmf/bucket-map.js), so the two can
 * never show different numbers.  It follows the CHF's Create path:
 *
 *   notStarted  the first session opens an empty cycle      -> allowance
 *   expired     renewal opens a fresh cycle, else nothing   -> allowance | 0
 *   active      allowance + top-ups - consumed, floored at 0
 *
 * autoRenew must be strictly true: the CHF zero-initialises its quota and
 * only sets auto_renew when the field is present, so absent means no renewal.
 *
 * @param {object} sub  ChfSubscriber document
 * @param {number} now  epoch ms
 * @returns {number} bytes
 */
function nextGrant(sub, now) {
  const quota = (sub && sub.quota) || {};
  const usage = (sub && sub.usage) || {};
  const allowance = Number(quota.totalQuotaBytes) || 0;

  switch (cycleState(sub, now)) {
    case 'notStarted':
      return allowance;
    case 'expired':
      return quota.autoRenew === true ? allowance : 0;
    default: {
      const total = allowance + (Number(usage.topupBytes) || 0);
      return Math.max(0, total - (Number(usage.consumedBytes) || 0));
    }
  }
}

/**
 * cycleUsage — the figures the CHF Quota page shows for one subscriber.
 *
 * remaining is nextGrant(): what the subscriber can still use.  consumed and
 * topup describe the CURRENT cycle only, so both read 0 once the cycle has
 * expired - the CHF opens the next one (and zeroes them) on the subscriber's
 * next session.  stale flags that window, so the page can explain it.
 *
 * @param {object} sub  ChfSubscriber document
 * @param {number} [now=Date.now()]  epoch ms
 * @returns {{provisioned: number, topup: number, consumed: number,
 *            remaining: number, pct: number, state: string,
 *            periodStart: (Date|string|null), stale: boolean}}
 */
function cycleUsage(sub, now) {
  const at = typeof now === 'number' ? now : Date.now();
  const quota = (sub && sub.quota) || {};
  const usage = (sub && sub.usage) || {};
  const state = cycleState(sub, at);
  const active = state === 'active';

  const provisioned = Math.max(0, Number(quota.totalQuotaBytes) || 0);
  const topup = active ? Math.max(0, Number(usage.topupBytes) || 0) : 0;
  const consumed = active ? Math.max(0, Number(usage.consumedBytes) || 0) : 0;
  const allowance = provisioned + topup;
  const pct = allowance > 0 ? Math.min(100, (consumed / allowance) * 100) : 0;

  return {
    provisioned,
    topup,
    consumed,
    remaining: nextGrant(sub, at),
    pct,
    state,
    periodStart: usage.periodStart || null,
    stale: state === 'expired',
  };
}

/**
 * fmtPeriod — human-readable validity period for list views.
 *
 * @param {string|null} period  e.g. 'daily', 'weekly', 'monthly', 'custom'
 * @param {number|null} seconds  used when period === 'custom'
 * @returns {string}
 */
function fmtPeriod(period, seconds) {
  if (!period) return '—';
  switch (period) {
    case 'daily':   return 'Daily (24 h)';
    case 'weekly':  return 'Weekly (7 d)';
    case 'monthly': return 'Monthly (30 d)';
    case 'custom':  return seconds ? `Custom (${seconds} s)` : 'Custom';
    default:        return period;
  }
}

/* ------------------------------------------------------------------ */
/* Pools                                                                */
/* ------------------------------------------------------------------ */

/**
 * The SIMs a pool form lists: one per line, or separated by commas or
 * spaces, trimmed, in order, empty entries dropped.
 */
function membersFromText(text) {
  return String(text == null ? '' : text)
    .split(/[\s,]+/)
    .map(s => s.trim())
    .filter(Boolean);
}

/**
 * A pool form -> the TMF654 Bucket body the WebUI's TMF API takes.
 *
 * A pool is created and changed through the TMF API, not the REST resource
 * the rest of this page uses: that is where a SIM is kept to one pool, the
 * member limit from chf.yaml applies, and a SIM taken out has its sessions
 * on the pool released at once.  The plan fields go the same way, so a plan
 * change reaches every member's live sessions too.
 *
 * On create the scope is left out when the form names none; on update it
 * is sent as null, which clears it, as the REST path's form does.
 *
 * @param {object}  formData  Edit.js form state for a pool
 * @param {boolean} create    true for a new pool
 * @returns {object}          { logicalResource, allowance, ... }
 */
function poolFormToTmf(formData, create) {
  const doc = formToDb(formData);
  const q = doc.quota;
  const body = {
    logicalResource: membersFromText(formData.membersText).map(id => ({ id })),
    allowance: { amount: q.totalQuotaBytes, units: 'octet' },
    validityPeriod: q.validityPeriod,
    autoRenew: q.autoRenew,
    finalUnitAction: q.finalUnitAction,
    priority: doc.priority,
  };
  if (q.validityPeriod === 'custom') body.validitySeconds = q.validitySeconds;

  const scope = doc.scope;
  if (scope) {
    const out = {};
    if (scope.snssai) {
      out.sNSSAI = { sst: scope.snssai.sst };
      if (scope.snssai.sd) out.sNSSAI.sd = scope.snssai.sd;
    }
    if (scope.dnn) out.dnn = scope.dnn;
    if (scope.hplmn) {
      const [mcc, mnc] = String(scope.hplmn).split('-');
      out.hPlmnId = { mcc, mnc };
    }
    if (scope.roaming) out.roaming = scope.roaming;
    body.scope = out;
  } else if (!create) {
    body.scope = null;
  }
  return body;
}

module.exports = {
  DEFAULT_QUOTA_FORM,
  DEFAULT_POOL_FORM,
  membersFromText,
  poolFormToTmf,
  DEFAULT_SCOPE_FORM,
  hasScope,
  scopeLabel,
  formToScope,
  scopeToForm,
  bucketIdFor,
  cycleState,
  nextGrant,
  cycleUsage,
  BYTES_PER_MB,
  BYTES_PER_GB,
  BYTES_PER_TB,
  PERIOD_SECONDS,
  bytesToDisplay,
  displayToBytes,
  dbToForm,
  formToDb,
  fmtVolume,
  fmtPeriod,
};
