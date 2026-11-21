/*
 * TMF666 Account Management API v4 — BillingAccount.
 *
 *   GET    /accountManagement/v4/billingAccount[?fields&offset&limit&<attr>=]
 *   GET    /accountManagement/v4/billingAccount/{id}
 *   POST   /accountManagement/v4/billingAccount
 *   PATCH  /accountManagement/v4/billingAccount/{id}   (JSON Merge Patch)
 *   DELETE /accountManagement/v4/billingAccount/{id}
 *
 * A billing account groups the data buckets (SUPIs) that are charged
 * together, and names the Individual(s) responsible for it through
 * relatedParty.  Like the Individual it is a BSS record the CHF never reads.
 *
 * Copyright (C) 2026 by Rami Mohamed <ramrode@gmail.com>
 */

'use strict';

const express = require('express');
const crypto = require('crypto');
const C = require('./common');

const COLLECTION = 'billingAccount';

const READ_ONLY = ['id', 'href', '@type', '@baseType', 'lastModified'];

const FILTERABLE = {
  name: 'name',
  state: 'state',
  accountType: 'accountType',
  'relatedParty.id': 'relatedParty.id',
};

const STATES = ['active', 'suspended', 'closed'];
const ACCOUNT_TYPES = ['prepaid', 'postpaid'];

function validateRelatedParty(list) {
  if (!Array.isArray(list))
    throw new C.TmfError(400, 'Bad Request', "'relatedParty' must be an array");
  list.forEach((p, i) => {
    if (!C.isPlainObject(p) || typeof p.id !== 'string' || !p.id)
      throw new C.TmfError(400, 'Bad Request',
          `relatedParty[${i}] must be an object with a string 'id'`);
  });
}

function validate(body) {
  if (typeof body.name !== 'string' || !body.name.trim())
    throw new C.TmfError(400, 'Bad Request', "'name' is required");
  ['description', 'accountType', 'state'].forEach(a => {
    if (body[a] !== undefined && typeof body[a] !== 'string')
      throw new C.TmfError(400, 'Bad Request', `'${a}' must be a string`);
  });
  if (body.state !== undefined && STATES.indexOf(body.state) === -1)
    throw new C.TmfError(400, 'Bad Request',
        `'state' must be one of: ${STATES.join(', ')}`);
  if (body.accountType !== undefined &&
      ACCOUNT_TYPES.indexOf(body.accountType) === -1)
    throw new C.TmfError(400, 'Bad Request',
        `'accountType' must be one of: ${ACCOUNT_TYPES.join(', ')}`);
  if (body.relatedParty !== undefined)
    validateRelatedParty(body.relatedParty);
}

/* Persist only the reference fields, never a client-supplied href. */
function normaliseRelatedParty(list) {
  return (list || []).map(p => ({
    id: p.id,
    role: typeof p.role === 'string' ? p.role : 'owner',
    '@referredType': 'Individual',
  }));
}

function pick(body) {
  const out = {};
  ['name', 'description', 'accountType', 'state'].forEach(a => {
    if (body[a] !== undefined) out[a] = body[a];
  });
  if (body.relatedParty !== undefined)
    out.relatedParty = normaliseRelatedParty(body.relatedParty);
  return out;
}

function represent(req, doc) {
  const partyBase =
    `${req.protocol}://${req.get('host')}${req.baseUrl}`
      .replace(/\/accountManagement\/v4$/, '/partyManagement/v4');
  return Object.assign(
    { id: doc.id, href: C.hrefFor(req, COLLECTION, doc.id),
      '@type': 'BillingAccount' },
    pick(doc),
    { relatedParty: (doc.relatedParty || []).map(p => Object.assign({}, p, {
        href: `${partyBase}/individual/${encodeURIComponent(p.id)}` })) },
    doc.lastModified ? { lastModified: doc.lastModified } : {});
}

/* Every referenced Individual must exist; a dangling reference is a 400. */
async function assertPartiesExist(repos, relatedParty) {
  for (const p of relatedParty || []) {
    if (!await repos.individual.get(p.id))
      throw new C.TmfError(400, 'Bad Request',
          `relatedParty '${p.id}' does not exist`);
  }
}

/*
 * @param repos.billingAccount   repository keyed by 'id'
 * @param repos.individual       to validate relatedParty references
 * @param repos.bucket           to refuse deleting an account with buckets
 */
function createAccountRouter(repos) {
  const router = express.Router();

  router.get(`/${COLLECTION}`, C.handler(async (req, res) => {
    const { offset, limit } = C.paging(req.query);
    const filter = C.buildFilter(req.query, FILTERABLE);
    const { items, total } =
      await repos.billingAccount.list(filter, offset, limit);
    return C.sendList(res,
        items.map(d => C.project(represent(req, d), req.query.fields)), total);
  }));

  router.get(`/${COLLECTION}/:id`, C.handler(async (req, res) => {
    const doc = await repos.billingAccount.get(req.params.id);
    if (!doc) throw new C.TmfError(404, 'Not Found', 'Billing account not found');
    return res.json(C.project(represent(req, doc), req.query.fields));
  }));

  router.post(`/${COLLECTION}`, C.handler(async (req, res) => {
    const body = C.requireObjectBody(req);
    C.rejectReadOnly(body, READ_ONLY);
    validate(body);
    const doc = Object.assign(pick(body), {
      id: crypto.randomUUID(),
      state: body.state || 'active',
      accountType: body.accountType || 'prepaid',
      relatedParty: normaliseRelatedParty(body.relatedParty),
      lastModified: new Date().toISOString(),
    });
    await assertPartiesExist(repos, doc.relatedParty);
    const created = await repos.billingAccount.create(doc);
    return C.sendCreated(res, represent(req, created));
  }));

  router.patch(`/${COLLECTION}/:id`, C.handler(async (req, res) => {
    const patch = C.requireObjectBody(req);
    C.rejectReadOnly(patch, READ_ONLY);
    const current = await repos.billingAccount.get(req.params.id);
    if (!current)
      throw new C.TmfError(404, 'Not Found', 'Billing account not found');

    const merged = C.mergePatch(pick(current), patch);
    validate(merged);
    const next = Object.assign(pick(merged), {
      id: current.id, lastModified: new Date().toISOString() });
    await assertPartiesExist(repos, next.relatedParty);
    const saved = await repos.billingAccount.replace(req.params.id, next);
    return res.json(represent(req, saved));
  }));

  router.delete(`/${COLLECTION}/:id`, C.handler(async (req, res) => {
    if (await repos.bucket.exists({ billingAccountId: req.params.id }))
      throw new C.TmfError(409, 'Conflict',
          'Billing account still has buckets; move or delete them first');
    if (!await repos.billingAccount.remove(req.params.id))
      throw new C.TmfError(404, 'Not Found', 'Billing account not found');
    return res.status(204).end();
  }));

  return router;
}

module.exports = { createAccountRouter, validate };
