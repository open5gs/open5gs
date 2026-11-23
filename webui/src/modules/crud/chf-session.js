/*
 * Redux fetch action creators for CHF active sessions (read-only).
 *
 * Sessions are written by the CHF NF; the WebUI only reads them.
 * express-restify-mongoose v6 uses model.modelName as the route name (no
 * pluralisation), so ChfSession is mounted at /db/ChfSession (not
 * /db/ChfSessions).  Non-GET verbs are blocked by the preMiddleware in db.js.
 *
 * 3GPP: TS 23.255 §6 — active session monitoring
 */

import {
  fetchCollection,
  fetchDocument
} from './actions';

export const MODEL = 'chfsessions';
export const URL   = '/ChfSession';

export const fetchChfSessions = (params = {}) =>
  fetchCollection(MODEL, URL, params, { idProperty: 'chargingDataRef' });

export const fetchChfSession = (ref, params = {}) =>
  fetchDocument(MODEL, ref, `${URL}/${ref}`, params, { idProperty: 'chargingDataRef' });
