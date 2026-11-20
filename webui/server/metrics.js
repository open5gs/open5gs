/*
 * WebUI-plane Prometheus metrics for CHF provisioning.
 *
 * Subscriber CRUD happens in this Node server (not in the C CHF NF), so
 * provisioning-rate observability must be emitted here.  This exposes
 *
 *   chf_provisioning_ops_total{op="create|update|delete",result="success|error"}
 *
 * plus the default process metrics, at GET /metrics (unauthenticated, for a
 * Prometheus scrape).
 *
 * prom-client is an OPTIONAL dependency: it is loaded lazily and, if it is not
 * installed, every function here degrades to a no-op and /metrics returns a
 * short hint.  This keeps the WebUI running with or without it; to enable the
 * metrics simply `npm install prom-client` in webui/.
 *
 * Copyright (C) 2026 by Rami Mohamed <ramrode@gmail.com>
 */

'use strict';

let client = null;
let registry = null;
let provisioningOps = null;

try {
  // eslint-disable-next-line global-require
  client = require('prom-client');
} catch (e) {
  client = null;
}

if (client) {
  registry = new client.Registry();
  client.collectDefaultMetrics({ register: registry, prefix: 'chf_webui_' });

  provisioningOps = new client.Counter({
    name: 'chf_provisioning_ops_total',
    help: 'CHF subscriber provisioning operations via the WebUI',
    labelNames: ['op', 'result'],
    registers: [registry],
  });
}

/* Map an HTTP method to a provisioning op name; null for reads/other. */
function opFromMethod(method) {
  switch ((method || '').toUpperCase()) {
    case 'POST': return 'create';
    case 'PUT':
    case 'PATCH': return 'update';
    case 'DELETE': return 'delete';
    default: return null;
  }
}

/*
 * express-restify-mongoose postProcess hook for the ChfSubscriber resource.
 * Counts create/update/delete by outcome, then continues the chain.
 */
function subscriberPostProcess(req, res, next) {
  try {
    if (provisioningOps) {
      const op = opFromMethod(req.method);
      if (op) {
        const ok = res.statusCode >= 200 && res.statusCode < 400;
        provisioningOps.inc({ op, result: ok ? 'success' : 'error' });
      }
    }
  } catch (e) {
    /* never let metrics break the request */
  }
  if (typeof next === 'function') next();
}

/* GET /metrics handler (Prometheus text exposition). */
async function handler(req, res) {
  if (!registry) {
    res.set('Content-Type', 'text/plain');
    return res.status(200).send(
      '# prom-client not installed; run `npm install prom-client` in webui/ ' +
      'to enable CHF WebUI provisioning metrics.\n'
    );
  }
  try {
    res.set('Content-Type', registry.contentType);
    return res.status(200).send(await registry.metrics());
  } catch (e) {
    return res.status(500).send(String(e));
  }
}

module.exports = {
  enabled: !!client,
  subscriberPostProcess,
  handler,
};
