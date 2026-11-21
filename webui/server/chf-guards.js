/*
 * Request guards for the CHF WebUI REST resources.
 *
 * Kept out of routes/db.js so they can be unit-tested without Express or a
 * live Mongoose connection.
 *
 * Copyright (C) 2026 by Rami Mohamed <ramrode@gmail.com>
 */

'use strict';

/*
 * usage.* on a CHF subscriber is owned exclusively by the CHF NF: it holds
 * the billing-cycle counter the CHF advances with an atomic $inc on every
 * usage report (src/chf/db-handler.c).  No API client may write it.
 *
 * Without this guard an admin edit silently rewinds the counter.  The edit
 * modal loads the whole document when it opens and PATCHes it back on save,
 * so whatever the CHF counted while the modal was open is overwritten with
 * the stale snapshot - handing the subscriber that usage for free, or
 * resurrecting a pre-rollover periodStart.
 *
 * Enforced server-side because the WebUI is not the only client: any curl
 * script can PATCH this resource.  Both the "usage" object and dotted paths
 * such as "usage.consumedBytes" are removed; Mongoose's document.set() would
 * otherwise apply the dotted form straight to the nested field.
 *
 * Mounted as preMiddleware so it runs for every verb, including the
 * deprecated POST-to-item and PUT update routes.  On reads there is no body
 * and it is a no-op.
 */
function chfSubscriberProtectUsage(req, res, next) {
  const body = req.body;
  if (body && typeof body === 'object') {
    Object.keys(body).forEach(key => {
      if (key === 'usage' || key.startsWith('usage.'))
        delete body[key];
    });
  }
  return next();
}

/*
 * A pool's members are changed through the TMF API only
 * (PATCH /api/tmf-api/prepayBalanceManagement/v4/bucket/{id}), which is
 * where the rules are: the limit set in chf.yaml, one pool per SIM, and a
 * ChargingNotify that releases a removed member's sessions at once.  Here
 * they are kept as they are, the same way as the usage counter, so the edit
 * modal saving a whole document back cannot change them.  It also means a
 * pool cannot be created through this resource.
 */
function chfSubscriberProtectMembers(req, res, next) {
  const body = req.body;
  if (body && typeof body === 'object') {
    Object.keys(body).forEach(key => {
      if (key === 'members' || key.startsWith('members.'))
        delete body[key];
    });
  }
  return next();
}

module.exports = { chfSubscriberProtectUsage, chfSubscriberProtectMembers };
