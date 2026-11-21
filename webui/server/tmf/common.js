/*
 * Shared TM Forum REST conventions (TMF630 API Design Guidelines, Part 1).
 *
 * Everything the three resource families have in common lives here so each
 * resource module only describes its own attributes:
 *
 *   - id / href / @type on every representation
 *   - GET lists: ?fields= projection, ?offset=&limit= paging, attribute
 *     filtering, X-Total-Count / X-Result-Count, 206 for a partial page
 *   - POST -> 201 + Location;  PATCH -> JSON Merge Patch (RFC 7386);
 *     DELETE -> 204
 *   - errors as the TMF Error object {code, reason, message, status}
 *
 * Copyright (C) 2026 by Rami Mohamed <ramrode@gmail.com>
 */

'use strict';

const express = require('express');

const DEFAULT_LIMIT = 100;
const MAX_LIMIT = 1000;

/* Attributes a ?fields= projection always keeps, per TMF630. */
const ALWAYS_KEPT = ['id', 'href', '@type', '@baseType', '@schemaLocation'];

/* Query parameters with a meaning of their own, never treated as filters. */
const RESERVED_PARAMS = new Set(['fields', 'offset', 'limit', 'sort']);

/*
 * JSON body parsing for TMF routes.  The WebUI's global parser only accepts
 * application/json, so a PATCH sent as application/merge-patch+json - the
 * content type TMF630 prescribes for merge patches - would otherwise arrive
 * with an empty body and silently change nothing.
 */
const jsonBody = express.json({
  type: ['application/json', 'application/merge-patch+json'],
  limit: '256kb',
});

/* ------------------------------------------------------------------ */
/* Errors                                                               */
/* ------------------------------------------------------------------ */

class TmfError extends Error {
  constructor(status, reason, message) {
    super(message || reason);
    this.status = status;
    this.reason = reason;
  }
}

function sendError(res, status, reason, message) {
  return res.status(status).json({
    '@type': 'Error',
    code: String(status),
    reason,
    message: message || reason,
    status: String(status),
  });
}

/*
 * Wrap an async route handler so a thrown TmfError becomes a TMF Error
 * response, and anything unexpected becomes a 500 without leaking internals.
 */
function handler(fn) {
  return (req, res) => {
    Promise.resolve(fn(req, res)).catch(err => {
      if (err instanceof TmfError)
        return sendError(res, err.status, err.reason, err.message);
      /* Duplicate key on a unique index - e.g. a SUPI provisioned twice. */
      if (err && err.code === 11000)
        return sendError(res, 409, 'Conflict', 'Resource already exists');
      // eslint-disable-next-line no-console
      console.error('[tmf]', err && err.stack ? err.stack : err);
      return sendError(res, 500, 'Internal Server Error');
    });
  };
}

/* ------------------------------------------------------------------ */
/* Access control                                                       */
/* ------------------------------------------------------------------ */

/*
 * Who is calling, from the stored account rather than the token.  A WebUI
 * token carries the username and roles it was issued with and never
 * expires, so a token issued before an admin was demoted, or before the
 * account was deleted, would otherwise keep its old rights here.  Upstream
 * made the same change for /api/db/Account (fe0010336).  The caller is
 * looked up by the token's _id on every request; a token whose account no
 * longer exists gets a 401.
 *
 * @param accounts  { get(id) } -> { username, roles } or null
 */
function loadCaller(accounts) {
  return (req, res, next) => {
    const id = req.user && req.user._id;
    if (!id)
      return sendError(res, 401, 'Unauthorized', 'The token names no account');
    Promise.resolve(accounts.get(String(id))).then(account => {
      if (!account)
        return sendError(res, 401, 'Unauthorized',
            'The account this token was issued to no longer exists');
      req.caller = { username: account.username, roles: account.roles || [] };
      return next();
    }).catch(next);
  };
}

/*
 * Reads need any existing account; anything that changes state needs the
 * admin role on the stored account (loadCaller).  This is a provisioning
 * API: an ordinary WebUI user must not be able to change another
 * subscriber's allowance or top it up.
 */
function requireAdminForWrites(req, res, next) {
  if (req.method === 'GET' || req.method === 'HEAD')
    return next();
  const roles = (req.caller && req.caller.roles) || [];
  if (roles.indexOf('admin') === -1)
    return sendError(res, 403, 'Forbidden',
        'The admin role is required to modify resources');
  return next();
}

/* ------------------------------------------------------------------ */
/* Representation helpers                                               */
/* ------------------------------------------------------------------ */

/* Absolute href for a resource, from the mount point of the current router. */
function hrefFor(req, collection, id) {
  const base = `${req.protocol}://${req.get('host')}${req.baseUrl}`;
  return `${base}/${collection}/${encodeURIComponent(id)}`;
}

/*
 * Apply a ?fields=a,b projection.  Top-level attributes only, as TMF630
 * describes; the identifying attributes are always retained.
 */
function project(resource, fieldsParam) {
  if (!fieldsParam) return resource;
  const wanted = new Set(
    String(fieldsParam).split(',').map(f => f.trim()).filter(Boolean));
  const out = {};
  Object.keys(resource).forEach(key => {
    if (ALWAYS_KEPT.indexOf(key) !== -1 || wanted.has(key))
      out[key] = resource[key];
  });
  return out;
}

/* Parse ?offset=&limit=, clamping to sane bounds. */
function paging(query) {
  const offset = Math.max(0, parseInt(query.offset, 10) || 0);
  let limit = parseInt(query.limit, 10);
  if (!Number.isFinite(limit) || limit <= 0) limit = DEFAULT_LIMIT;
  return { offset, limit: Math.min(limit, MAX_LIMIT) };
}

/*
 * Build an equality filter from query parameters.
 *
 * Only attributes the resource declares as filterable are honoured.  The
 * values reach MongoDB as plain strings, and restricting the keys means a
 * caller cannot smuggle an operator in (?$where=, ?usage.consumedBytes[$gt]=)
 * or filter on attributes the resource does not expose.
 *
 * `filterable` maps an API attribute name to its storage path.
 */
function buildFilter(query, filterable) {
  const filter = {};
  Object.keys(query).forEach(key => {
    if (RESERVED_PARAMS.has(key)) return;
    const path = filterable[key];
    if (!path)
      throw new TmfError(400, 'Bad Request',
          `Filtering on '${key}' is not supported`);
    const value = query[key];
    if (typeof value !== 'string')
      throw new TmfError(400, 'Bad Request',
          `Filter '${key}' must be a single plain value`);
    filter[path] = value;
  });
  return filter;
}

/* Send a list with the TMF paging headers; 206 when it is a partial page. */
function sendList(res, items, total) {
  res.set('X-Total-Count', String(total));
  res.set('X-Result-Count', String(items.length));
  return res.status(items.length < total ? 206 : 200).json(items);
}

function sendCreated(res, resource) {
  res.set('Location', resource.href);
  return res.status(201).json(resource);
}

/* ------------------------------------------------------------------ */
/* JSON Merge Patch (RFC 7386)                                          */
/* ------------------------------------------------------------------ */

function isPlainObject(v) {
  return v !== null && typeof v === 'object' && !Array.isArray(v);
}

/*
 * Apply a merge patch: objects merge recursively, null deletes a member,
 * and anything else (including arrays) replaces the target wholesale.
 */
function mergePatch(target, patch) {
  if (!isPlainObject(patch)) return patch;
  const out = isPlainObject(target) ? Object.assign({}, target) : {};
  Object.keys(patch).forEach(key => {
    if (patch[key] === null) delete out[key];
    else out[key] = mergePatch(out[key], patch[key]);
  });
  return out;
}

/* Reject a body that is not a JSON object before any handler reads it. */
function requireObjectBody(req) {
  if (!isPlainObject(req.body))
    throw new TmfError(400, 'Bad Request', 'Request body must be a JSON object');
  return req.body;
}

/* Reject attempts to set attributes the server owns. */
function rejectReadOnly(body, readOnly) {
  const bad = readOnly.filter(a => Object.prototype.hasOwnProperty.call(body, a));
  if (bad.length)
    throw new TmfError(400, 'Bad Request',
        `Read-only attribute(s) cannot be set: ${bad.join(', ')}`);
}

module.exports = {
  DEFAULT_LIMIT,
  MAX_LIMIT,
  TmfError,
  jsonBody,
  sendError,
  handler,
  loadCaller,
  requireAdminForWrites,
  hrefFor,
  project,
  paging,
  buildFilter,
  sendList,
  sendCreated,
  mergePatch,
  isPlainObject,
  requireObjectBody,
  rejectReadOnly,
};
