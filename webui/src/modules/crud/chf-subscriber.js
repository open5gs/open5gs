/*
 * Redux CRUD action creators for CHF subscriber quota profiles.
 *
 * Mirrors the pattern in modules/crud/subscriber.js.
 * express-restify-mongoose v6 uses model.modelName as the route name (no
 * pluralisation), so ChfSubscriber is mounted at /db/ChfSubscriber (not
 * /db/ChfSubscribers).  The URL constant below reflects this.
 *
 * 3GPP: TS 23.255 §5.2 — CHF subscriber management
 */

import {
  fetchCollection,
  fetchDocument,
  createDocument,
  updateDocument,
  deleteDocument
} from './actions';

export const MODEL = 'chfsubscribers';
export const URL   = '/ChfSubscriber';

export const fetchChfSubscribers = (params = {}) =>
  fetchCollection(MODEL, URL, params, { idProperty: 'bucketId' });

export const fetchChfSubscriber = (id, params = {}) =>
  fetchDocument(MODEL, id, `${URL}/${id}`, params, { idProperty: 'bucketId' });

export const createChfSubscriber = (params = {}, data = {}) =>
  createDocument(MODEL, URL, params, data, { idProperty: 'bucketId' });

export const updateChfSubscriber = (id, params = {}, data = {}) =>
  updateDocument(MODEL, id, `${URL}/${id}`, params, data, { idProperty: 'bucketId' });

export const deleteChfSubscriber = (id, params = {}) =>
  deleteDocument(MODEL, id, `${URL}/${id}`, params, { idProperty: 'bucketId' });
