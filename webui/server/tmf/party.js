/*
 * TMF632 Party Management API v4 — Individual.
 *
 *   GET    /partyManagement/v4/individual[?fields&offset&limit&<attr>=]
 *   GET    /partyManagement/v4/individual/{id}
 *   POST   /partyManagement/v4/individual
 *   PATCH  /partyManagement/v4/individual/{id}      (JSON Merge Patch)
 *   DELETE /partyManagement/v4/individual/{id}
 *
 * An Individual is the person behind one or more billing accounts.  It is a
 * BSS record only: the CHF never reads it.
 *
 * Copyright (C) 2026 by Rami Mohamed <ramrode@gmail.com>
 */

'use strict';

const express = require('express');
const crypto = require('crypto');
const C = require('./common');

const COLLECTION = 'individual';

/* Server-owned attributes a client may not set. */
const READ_ONLY = ['id', 'href', '@type', '@baseType', 'lastUpdate'];

const FILTERABLE = {
  givenName: 'givenName',
  familyName: 'familyName',
  fullName: 'fullName',
  status: 'status',
};

const STATUSES = ['initialized', 'validated', 'deceased'];

/* Attributes accepted on create/patch, with their expected JSON types. */
const STRING_ATTRS = ['givenName', 'familyName', 'middleName', 'fullName',
  'formattedName', 'legalName', 'preferredGivenName', 'title', 'gender',
  'nationality', 'maritalStatus', 'status', 'birthDate'];
const ARRAY_ATTRS = ['contactMedium', 'externalReference', 'partyCharacteristic',
  'individualIdentification', 'otherName', 'relatedParty'];

function validate(body) {
  STRING_ATTRS.forEach(a => {
    if (body[a] !== undefined && typeof body[a] !== 'string')
      throw new C.TmfError(400, 'Bad Request', `'${a}' must be a string`);
  });
  ARRAY_ATTRS.forEach(a => {
    if (body[a] !== undefined && !Array.isArray(body[a]))
      throw new C.TmfError(400, 'Bad Request', `'${a}' must be an array`);
  });
  if (body.status !== undefined && STATUSES.indexOf(body.status) === -1)
    throw new C.TmfError(400, 'Bad Request',
        `'status' must be one of: ${STATUSES.join(', ')}`);
  if (!body.givenName && !body.familyName && !body.fullName)
    throw new C.TmfError(400, 'Bad Request',
        'One of givenName, familyName or fullName is required');
}

/* Only the attributes this resource defines are persisted. */
function pick(body) {
  const out = {};
  STRING_ATTRS.concat(ARRAY_ATTRS).forEach(a => {
    if (body[a] !== undefined) out[a] = body[a];
  });
  return out;
}

function represent(req, doc) {
  return Object.assign(
    { id: doc.id, href: C.hrefFor(req, COLLECTION, doc.id),
      '@type': 'Individual' },
    pick(doc),
    doc.lastUpdate ? { lastUpdate: doc.lastUpdate } : {});
}

/*
 * @param repos.individual       repository keyed by 'id'
 * @param repos.billingAccount   used to refuse deleting a referenced party
 */
function createPartyRouter(repos) {
  const router = express.Router();

  router.get(`/${COLLECTION}`, C.handler(async (req, res) => {
    const { offset, limit } = C.paging(req.query);
    const filter = C.buildFilter(req.query, FILTERABLE);
    const { items, total } = await repos.individual.list(filter, offset, limit);
    return C.sendList(res,
        items.map(d => C.project(represent(req, d), req.query.fields)), total);
  }));

  router.get(`/${COLLECTION}/:id`, C.handler(async (req, res) => {
    const doc = await repos.individual.get(req.params.id);
    if (!doc) throw new C.TmfError(404, 'Not Found', 'Individual not found');
    return res.json(C.project(represent(req, doc), req.query.fields));
  }));

  router.post(`/${COLLECTION}`, C.handler(async (req, res) => {
    const body = C.requireObjectBody(req);
    C.rejectReadOnly(body, READ_ONLY);
    validate(body);
    const doc = Object.assign(pick(body), {
      id: crypto.randomUUID(),
      status: body.status || 'initialized',
      lastUpdate: new Date().toISOString(),
    });
    const created = await repos.individual.create(doc);
    return C.sendCreated(res, represent(req, created));
  }));

  router.patch(`/${COLLECTION}/:id`, C.handler(async (req, res) => {
    const patch = C.requireObjectBody(req);
    C.rejectReadOnly(patch, READ_ONLY);
    const current = await repos.individual.get(req.params.id);
    if (!current) throw new C.TmfError(404, 'Not Found', 'Individual not found');

    const merged = C.mergePatch(pick(current), patch);
    validate(merged);
    const saved = await repos.individual.replace(req.params.id,
        Object.assign(pick(merged), {
          id: current.id, lastUpdate: new Date().toISOString() }));
    return res.json(represent(req, saved));
  }));

  router.delete(`/${COLLECTION}/:id`, C.handler(async (req, res) => {
    if (await repos.billingAccount.exists({ 'relatedParty.id': req.params.id }))
      throw new C.TmfError(409, 'Conflict',
          'Individual is referenced by a billing account');
    if (!await repos.individual.remove(req.params.id))
      throw new C.TmfError(404, 'Not Found', 'Individual not found');
    return res.status(204).end();
  }));

  return router;
}

module.exports = { createPartyRouter, validate, represent };
