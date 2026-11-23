/*
 * Helpers for paginated / filtered CHF collection queries.
 *
 * These translate the WebUI's page + search state into the query-string
 * params understood by express-restify-mongoose (limit / skip / sort / query)
 * and read the X-Total-Count response header back out.
 *
 * Kept separate from the containers so the (security-sensitive) query
 * construction and the header parsing are unit-testable in isolation.
 * CommonJS (module.exports) to match the other helpers and the no-transform
 * jest config; imported from components via the usual ESM interop.
 *
 * Copyright (C) 2026 by Rami Mohamed <ramrode@gmail.com>
 */

'use strict';

/*
 * Escape a string for safe inclusion in a MongoDB $regex.
 *
 * The search box feeds user input into a server-side $regex; without
 * escaping, a user could inject regex metacharacters (ReDoS via a
 * catastrophic pattern, or unintended matches).  We neutralise every
 * regex-special character.
 */
function escapeRegex(input) {
  return String(input == null ? '' : input).replace(
    /[.*+?^${}()|[\]\\]/g,
    '\\$&'
  );
}

/*
 * The filter for a search on the CHF Quota view: a SUPI finds the SIM's own
 * buckets and the pool it is in, whose document lists it among its members
 * and has no supi of its own; a bucket id - a pool's, pool-<12 hex>, among
 * them - finds that bucket.  Each clause is prefix-anchored and escaped, so
 * each can use its index.
 */
function subscriberFilter(search) {
  const trimmed = (search || '').trim();
  if (!trimmed) return undefined;
  const re = { $regex: '^' + escapeRegex(trimmed) };
  return { $or: [{ supi: re }, { members: re }, { bucketId: re }] };
}

/*
 * Build the query-string params for a paginated CHF subscriber fetch.
 *
 *   page      0-based page index
 *   pageSize  documents per page
 *   search    raw SUPI search text (may be empty; matched as a prefix)
 *
 * SUPI is anchored as a prefix ("^<escaped>") so the server can use the
 * unique SUPI index rather than an unanchored full scan, and the input is
 * regex-escaped.  When search is empty no query filter is sent.
 */
function buildSubscriberParams(page, pageSize, search) {
  const params = {
    limit: pageSize,
    skip: page * pageSize,
    sort: 'supi',
  };

  const filter = subscriberFilter(search);
  if (filter) params.query = JSON.stringify(filter);

  return params;
}

/*
 * The query-string params for a page of the CHF Shared Pools view: pools only,
 * found by their id or a member's SUPI, sorted by pool id.
 */
function buildPoolParams(page, pageSize, search) {
  const params = {
    limit: pageSize,
    skip: page * pageSize,
    sort: 'bucketId',
  };
  const trimmed = (search || '').trim();
  const filter = { members: { $exists: true } };
  if (trimmed) {
    const re = { $regex: '^' + escapeRegex(trimmed) };
    filter.$or = [{ bucketId: re }, { members: re }];
  }
  params.query = JSON.stringify(filter);
  return params;
}

/*
 * Granularity of the sliding activity window, in milliseconds.
 *
 * The window's lower bound is snapped DOWN to a multiple of this value so
 * that repeated calls return the same string for the whole bucket.  That
 * stability is load-bearing, not cosmetic: the resulting params object is
 * the cache key for the redux crud store (selectCollection matches
 * collections by deep-equal params).  A bound derived from a raw Date.now()
 * changes on every render, so no fetch ever matches a cached collection —
 * the selector reports needsFetch forever, the containers re-dispatch on
 * every store update (a self-sustaining request loop), and the store's
 * collections list grows by one entry per request.
 *
 * One minute keeps the window visibly current while bounding both effects.
 */
const WINDOW_BUCKET_MS = 60 * 1000;

/*
 * ISO-8601 timestamp for (now - hours), used as a startTime lower bound.
 * Snapped down to WINDOW_BUCKET_MS so the value is stable within a bucket.
 * `now` is injectable for testing; defaults to Date.now().
 */
function windowIso(hours, now) {
  const base = typeof now === 'number' ? now : Date.now();
  const bucketed = Math.floor(base / WINDOW_BUCKET_MS) * WINDOW_BUCKET_MS;
  return new Date(bucketed - hours * 3600 * 1000).toISOString();
}

/*
 * Build the MongoDB filter object for a session query.
 *
 *   search       SUPI prefix (regex-escaped) — narrows to one subscriber
 *   windowHours  when > 0, only sessions started within the last N hours
 *
 * Returns undefined when neither filter applies (so the caller can omit the
 * ?query param entirely).
 */
function buildSessionFilter(search, windowHours, now) {
  const filter = {};
  const trimmed = (search || '').trim();
  if (trimmed) filter.supi = { $regex: '^' + escapeRegex(trimmed) };
  if (windowHours && windowHours > 0)
    filter.startTime = { $gte: windowIso(windowHours, now) };
  return Object.keys(filter).length ? filter : undefined;
}

/*
 * Build the query-string params for a paginated CHF session fetch.
 * Sessions are ordered newest-first so the monitor shows the most recent
 * page.  `windowHours` bounds the view to recently-activated sessions and
 * `search` narrows to one subscriber; both use indexed fields.
 */
function buildSessionParams(page, pageSize, search, windowHours, now) {
  const params = {
    limit: pageSize,
    skip: page * pageSize,
    sort: '-startTime',
  };

  const filter = buildSessionFilter(search, windowHours, now);
  if (filter) params.query = JSON.stringify(filter);

  return params;
}

/*
 * Build params for a cheap "count only" fetch: one document plus the
 * X-Total-Count header gives the total matching the (optional) filter
 * without downloading the collection.  `kind` selects the filter shape.
 */
function countParams(kind, search, windowHours, now) {
  const params = { limit: 1 };
  let filter;
  if (kind === 'session') filter = buildSessionFilter(search, windowHours, now);
  else filter = subscriberFilter(search);
  if (filter) params.query = JSON.stringify(filter);
  return params;
}

/*
 * Read the total document count from a fetched collection's otherInfo.
 *
 * The crud reducer stores the axios response (minus data) as otherInfo, so
 * the express-restify-mongoose X-Total-Count header is reachable here.
 * Returns a non-negative integer, or undefined when the header is absent
 * (e.g. older server, or a single-document fetch).
 */
function totalFromOtherInfo(otherInfo) {
  const headers = (otherInfo && otherInfo.headers) || {};
  // Axios lower-cases response header names.
  const raw = headers['x-total-count'];
  if (raw === undefined || raw === null || raw === '') return undefined;
  const n = parseInt(raw, 10);
  return Number.isNaN(n) || n < 0 ? undefined : n;
}

module.exports = {
  buildPoolParams,
  WINDOW_BUCKET_MS,
  escapeRegex,
  windowIso,
  buildSessionFilter,
  buildSubscriberParams,
  buildSessionParams,
  countParams,
  totalFromOtherInfo,
};
