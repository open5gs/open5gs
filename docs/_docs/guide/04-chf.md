---
title: CHF (Charging Function)
---

## 1. What is the CHF, and do I need it?
---

The CHF is the network function that decides how much data a subscriber may use. The SMF asks it for a quota before a 5G session is set up and tells it what the session has used. The CHF keeps each balance in MongoDB, tells the SMF when a balance has run out, and writes a charging data record (CDR) when the session ends.

**You probably want this if:**
- You need data limits per subscriber, per slice or per DNN, enforced by the core.
- You need usage records for billing.

**You can skip this if:**
- You're running a lab core and don't need limits or records.

Open5GS ships this as its own network function, `open5gs-chfd`. It is **optional**, and it is not installed by the `open5gs` package.

> **Charging starts by itself.** As soon as a CHF registers with the NRF, the SMF charges every new 5G session, with no SMF configuration. A subscriber you haven't given a balance gets the default quota, 10 GB as shipped. 4G sessions keep using Gy and are not affected.

## 2. How a session is charged
---

1. A UE asks for a PDU session. Before setting it up, the SMF asks the CHF for quota.
2. The CHF picks the subscriber's balance, called a *bucket*, and grants a slice of it, 50 MB at most as shipped.
3. As the UE uses data, the SMF reports it and gets the next slice.
4. When the balance is empty, the session is released and the UE is told why. A new session is refused with 5GSM cause 26 and a 60-second back-off until the balance is topped up.
5. When the session ends, the CHF writes a CDR.

`ims`, `sos`, `mcptt` and `mcx` (Mission Critical Services) sessions are never limited by a balance, and are set up even if no CHF is available.

## 3. Quick start
---

### Step 1 — Install and start the CHF

For Debian/Ubuntu packages, install the optional NF explicitly. The package starts the service:

```bash
$ sudo apt install open5gs-chf
```

For a source installation, start it with:

```bash
$ ./install/bin/open5gs-chfd
```

On macOS and FreeBSD, add the loopback address `127.0.0.30` first, as the platform notes show.

Check that it registered with the NRF:

```bash
$ grep "NF registered" /var/log/open5gs/chf.log
```

### Step 2 — Give a subscriber a balance

In the WebUI, open **CHF Quota** and add a bucket for the subscriber's SUPI, such as `imsi-999700000000001`. You can also use the [API](#5-apis). A bucket's SUPI must be `imsi-` followed by 15 digits.

To share one balance between several SIMs, open **CHF Shared Pools** instead.

You can skip this step to try things out. The subscriber then gets the default quota.

### Step 3 — Bring up a UE

The SMF log shows the session being charged:

```
[smf] INFO: [internet] CHF association established [ref:729484ac-dc52-41f1-9367-2ff2daf8fe6f]
```

The WebUI's **CHF Sessions** page lists the live sessions.

### Step 4 — Read the record

When the session ends, its CDR is in a directory for that day:

```bash
$ ls /var/log/open5gs/cdr/2026/11/09/
cdr-729484ac-dc52-41f1-9367-2ff2daf8fe6f.json
```

## 4. Basic configuration
---

### chf.yaml

```yaml
chf:
  sbi:
    server:
      - address: 127.0.0.30
        port: 7777
    client:
      scp:
        - uri: http://127.0.0.200:7777
  metrics:
    server:
      - address: 127.0.0.30
        port: 9090
  charging:
    default_quota:
      volume_mb: 10240
    unprovisioned: default
    cdr:
      enable: true
      path: /var/log/open5gs/cdr
      format: json
```

The settings you are most likely to change, under `charging:`:

| Setting | As shipped | What it does |
|---|---|---|
| `default_quota.volume_mb` | `10240` | The balance of a subscriber with no bucket |
| `unprovisioned` | `default` | `reject` refuses a subscriber with no bucket instead |
| `cdr.enable` | `true` | Whether to write CDRs |
| `cdr.path` | `/var/log/open5gs/cdr` | Where they go. Nothing deletes old ones |
| `cdr.format` | `json` | `json`, `asn1` (the BER that TS 32.298 specifies for billing systems), `both` or `none` |
| `well_known_dnn.cdr` | `true` | Whether `ims`, `sos`, `mcptt` and `mcx` sessions get CDRs |
| `grant.max_volume_mb` | `50` | The largest slice granted at once. `0` means no cap |
| `grant.max_validity_seconds` | `300` | A session reports at least this often |
| `grant.quota_holding_seconds` | `60` | A session idle this long reports and gives back what it has |
| `notify.poll_seconds` | `5` | How long a top-up can take to reach a live session. Keep it the same on every CHF |
| `pool.max_members` | `10` | How many SIMs can share one pool. Keep it the same on every CHF |

One setting sits outside `charging:`, directly under `chf:`. `session_pool_multiplier`, 4 as shipped, is how many charging sessions the CHF holds per UE: `global.max.ue` × 4. Raise it to give the CHF more room without raising `global.max.ue` for every NF. Each session takes about 2.4 KB, allocated at start-up.

Every setting is described in the comments of the installed `chf.yaml`.

### smf.yaml

The SMF needs no configuration to use a CHF. To change what happens when there isn't one, add:

```yaml
smf:
  ctf:
    enabled: auto
```

| `enabled` | With a CHF | With no CHF |
|---|---|---|
| `auto` (default) | Sessions are charged | Sessions run uncharged |
| `yes` | Sessions are charged | Sessions are refused, except `ims`, `sos`, `mcptt` and `mcx` |
| `no` | No charging | No charging |

`ctf.enabled` also controls Gy for 4G sessions, so `no` turns off both.

## 5. APIs
---

### Provisioning, on the WebUI

Balances are managed through TM Forum Open APIs served by the WebUI, on port 9999 by default. Reading needs any WebUI account, and writing needs an `admin` account.

Get a token first:

```bash
$ BASE=http://localhost:9999
$ CSRF=$(curl -s -c jar $BASE/api/auth/csrf | python3 -c 'import json,sys; print(json.load(sys.stdin)["csrfToken"])')
$ curl -s -b jar -c jar -o /dev/null -H "X-CSRF-Token: $CSRF" -H 'Content-Type: application/json' \
    -d '{"username":"<username>","password":"<password>"}' $BASE/api/auth/login
$ TOKEN=$(curl -s -b jar $BASE/api/auth/session | python3 -c 'import json,sys; print(json.load(sys.stdin)["authToken"])')
```

Give a subscriber 10 GB:

```bash
$ curl -s -H "Authorization: Bearer $TOKEN" -H 'Content-Type: application/json' \
    -d '{"logicalResource":[{"id":"imsi-999700000000001"}],"allowance":{"amount":10,"units":"GB"}}' \
    $BASE/api/tmf-api/prepayBalanceManagement/v4/bucket
```

Add 1 GB to it for the current cycle. The `Idempotency-Key` stops a retry from crediting it twice:

```bash
$ curl -s -H "Authorization: Bearer $TOKEN" -H 'Content-Type: application/json' -H 'Idempotency-Key: order-1' \
    -d '{"bucket":{"id":"imsi-999700000000001"},"amount":{"amount":1,"units":"GB"}}' \
    $BASE/api/tmf-api/prepayBalanceManagement/v4/topupBalance
```

Share one balance between several SIMs, such as a family's phones. A bucket with more than one SIM is a *pool*: all of them draw on it. A SIM can be in one pool only:

```bash
$ curl -s -H "Authorization: Bearer $TOKEN" -H 'Content-Type: application/json' \
    -d '{"logicalResource":[{"id":"imsi-999700000000001"},{"id":"imsi-999700000000002"}],"allowance":{"amount":20,"units":"GB"}}' \
    $BASE/api/tmf-api/prepayBalanceManagement/v4/bucket
```

The answer has the pool's id, `pool-` and 12 hex digits. To add or remove SIMs, send the whole new list. A SIM you take out has its sessions on the pool released at once:

```bash
$ curl -s -X PATCH -H "Authorization: Bearer $TOKEN" -H 'Content-Type: application/json' \
    -d '{"logicalResource":[{"id":"imsi-999700000000001"},{"id":"imsi-999700000000003"}]}' \
    $BASE/api/tmf-api/prepayBalanceManagement/v4/bucket/<pool id>
```

The WebUI's **CHF Shared Pools** page does the same. Press the button at the bottom right, list the SIMs one per line, and set the allowance. The page lists each pool with its id, and you can search it by pool id or by a member's SUPI. Click a pool to change its members or its plan, or to delete it. On **CHF Quota**, a search for a SIM shows its pool next to its own buckets, with a **Manage shared pool** link to it. The page saves through the API, so the same rules apply.

Replace a subscriber's SIM. Every bucket of the old SUPI moves to the new one, with its plan and what is left of it, the new SIM takes the old one's place in a pool, and the old SIM's sessions are released:

```bash
$ curl -s -H "Authorization: Bearer $TOKEN" -H 'Content-Type: application/json' -H 'Idempotency-Key: swap-1' \
    -d '{"logicalResource":{"id":"imsi-999700000000001"},"receiverLogicalResource":{"id":"imsi-999700000000002"}}' \
    $BASE/api/tmf-api/prepayBalanceManagement/v4/simSwap
```

The new SIM must not have a bucket yet. A swap moves balances only: add the new SIM in the WebUI's **Subscriber** page as usual, and remove the old one there, or it carries on with the default quota.

| Path, under `/api/tmf-api/` | Methods | Purpose |
|---|---|---|
| `prepayBalanceManagement/v4/bucket[/{id}]` | GET, POST, PATCH, DELETE | A subscriber's balance and plan, or a pool's |
| `prepayBalanceManagement/v4/topupBalance[/{id}]` | GET, POST | Add data for the current cycle |
| `prepayBalanceManagement/v4/simSwap[/{id}]` | GET, POST | Move a subscriber's balances to a new SIM |
| `partyManagement/v4/individual[/{id}]` | GET, POST, PATCH, DELETE | Users |
| `accountManagement/v4/billingAccount[/{id}]` | GET, POST, PATCH, DELETE | Billing accounts |

### Charging, between the SMF and the CHF

The SMF uses `Nchf_ConvergedCharging` (TS 32.291), in cleartext HTTP/2 by default:

| Request | Answer | When |
|---|---|---|
| `POST /nchf-convergedcharging/v3/chargingdata` | `201` and a `Location` | A session starts |
| `POST /nchf-convergedcharging/v3/chargingdata/{ref}/update` | `200` | A session reports usage |
| `POST /nchf-convergedcharging/v3/chargingdata/{ref}/release` | `204` | A session ends |

You don't normally call these yourself. To check that a CHF is answering:

```bash
$ curl -s --http2-prior-knowledge -i -H 'Content-Type: application/json' -d '{
    "subscriberIdentifier": "imsi-999700000000001",
    "nfConsumerIdentification": {"nFName": "test", "nodeFunctionality": "SMF"},
    "invocationTimeStamp": "2026-11-09T10:00:00Z",
    "invocationSequenceNumber": 1
  }' http://127.0.0.30:7777/nchf-convergedcharging/v3/chargingdata
```

A `201` opens a charging session, so release it afterwards with the `Location` it returned.

To reach a session before its next report, such as after a top-up, the CHF sends `ChargingNotify` to the SMF. A top-up made through the API does this for you.

### Metrics

Prometheus metrics are on the address in `metrics.server`:

```bash
$ curl -s http://127.0.0.30:9090/metrics | grep '^chf_'
```

## 6. Reading CDRs
---

With `format: json`, each session has a file of the TS 32.298 record written as JSON. With `format: asn1`, the same record is BER-encoded, and this decodes it:

```bash
$ open5gs-chf-cdr-decode /var/log/open5gs/cdr/2026/11/09/cdr-*.ber
```

## 7. Turning charging off
---

Stop the CHF, and new sessions run uncharged:

```bash
$ sudo systemctl disable --now open5gs-chfd
```

To keep charging off even with a CHF registered, set `ctf.enabled: no` in `smf.yaml` and restart the SMF. This also turns off Gy.

## 8. Troubleshooting
---

| What you see | Likely cause |
|---|---|
| The UE gets `INSUFFICIENT_RESOURCES` and retries after 60 s | The balance is empty. Top it up |
| The UE gets `NETWORK_FAILURE` | `ctf.enabled: yes` and no CHF is available, or the CHF didn't answer, or its session pool is full |
| The SMF warns that `ims`, `sos`, `mcptt` or `mcx` was "set up without charging" | The CHF refused or didn't answer, or its session pool is full. These sessions are set up anyway |
| Sessions aren't charged | The CHF hasn't registered with the NRF, `ctf.enabled` is `no`, or it's an `ims`, `sos`, `mcptt` or `mcx` session, which use no balance |
| A session stops being charged after the CHF restarts | Live sessions aren't restored. They are charged again when they are next set up |
| The AMF logs an error for each refused session | Expected. It logs every refused retry |
| A `Create` gets `400 Invalid SUPI` | The SUPI is malformed. It should look like `imsi-999700000000001` |

## 9. Limits
---

- A session counts against one bucket under one rating group.
- CDRs are written to disk. There is no Bx or Ga transfer, and nothing rotates or deletes them.
- The session pool holds `global.max.ue` × `session_pool_multiplier` sessions, 4 per UE as shipped. When it is full, new sessions are refused, except `ims`, `sos`, `mcptt` and `mcx`, which run uncharged.
- Only IMSI subscribers can be given a bucket. A subscriber with another kind of SUPI gets the default quota.
- When a shared pool's balance runs out, each of its members' live sessions can use up to one more slice, 50 MB as shipped, before it is stopped. There are no limits per member inside a pool.
- A CHF restart loses live sessions, but not balances.
- Home-routed roaming has not been tested.
