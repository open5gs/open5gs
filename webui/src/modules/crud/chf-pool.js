/*
 * Shared pools from the CHF Quota page, through the WebUI's TMF654 API.
 *
 * The rest of the page saves through the REST resource /api/db/ChfSubscriber.
 * A pool does not: creating one, changing its members, changing its plan and
 * deleting it all go through /api/tmf-api/prepayBalanceManagement/v4/bucket,
 * because that is where the pool rules live (one pool per SIM, the member
 * limit from chf.yaml) and where the CHF is told: a SIM taken out has its
 * sessions on the pool released at once, and a plan change, a new member or
 * a delete reaches the members' live sessions.  The REST resource keeps a
 * pool's members as they are.
 *
 * The calls carry the same session credentials as the REST ones: the JWT,
 * which the TMF API requires (writes need the admin role), and the CSRF token.
 *
 * Copyright (C) 2026 by Rami Mohamed <ramrode@gmail.com>
 */

import axios from 'axios';
import Session from 'modules/auth/session';

const BASE = '/api/tmf-api/prepayBalanceManagement/v4/bucket';

function headers() {
  const session = (new Session()).session || {};
  const h = { 'X-CSRF-TOKEN': session.csrfToken };
  if (session.authToken) h.Authorization = 'Bearer ' + session.authToken;
  return h;
}

const call = (method, url, data) =>
  axios({ method, url, data, headers: headers() });

export const createPool = body => call('post', BASE, body);

export const updatePool = (id, body) =>
  call('patch', `${BASE}/${encodeURIComponent(id)}`, body);

export const deletePool = id =>
  call('delete', `${BASE}/${encodeURIComponent(id)}`);

/* A pool is a bucket whose id the API gave it: pool-<12 hex>. */
export const isPoolId = id => /^pool-[0-9a-f]{12}$/.test(String(id || ''));

/* What to tell the operator when a call fails: the TMF Error object's
 * reason and message, or what the HTTP layer said. */
export function poolError(error) {
  const response = (error && error.response) || {};
  const data = response.data || {};
  return {
    title: data.reason || response.status || 'Error',
    message: data.message || response.statusText ||
        (error && error.message) || 'The request failed',
  };
}
