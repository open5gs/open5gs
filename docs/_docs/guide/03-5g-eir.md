---
title: 5G-EIR (Equipment Identity Register)
---

## 1. Overview
---

`open5gs-eird` is a native 5G-EIR (5G Equipment Identity Register) network function. It registers with the NRF as NF type `5G_EIR` and exposes the standard `N5g-eir_EquipmentIdentityCheck` service (`n5g-eir-eic`, API version `v1`) defined in 3GPP TS 29.511.

The AMF can optionally consume this service during registration to check whether a UE's PEI (IMEI/IMEISV) is whitelisted, blacklisted, or greylisted. This check is **disabled by default**; existing deployments are unaffected unless `amf.eir.enabled` is explicitly turned on.

## 2. Starting the 5G-EIR
---

```bash
$ open5gs-eird -c /etc/open5gs/eir.yaml
```

`open5gs-eird` uses the same MongoDB database as the rest of Open5GS (`db_uri` in `eir.yaml`, e.g. `mongodb://localhost/open5gs`), reached through the existing `lib/dbi` layer — no separate database process is required.

## 3. MongoDB records
---

Equipment status records live in the `eir` collection. Each record requires `pei` and `status`; `supi` is optional and, when present, makes the record apply only to that subscriber's use of that device.

```javascript
// Generic record: any subscriber using this PEI is whitelisted
db.eir.insertOne({
  pei: "imeisv-4901542032375186",
  status: "WHITELISTED"
})

// Subscriber-specific override: this SUPI+PEI pair is blacklisted,
// even though the PEI is generically whitelisted above
db.eir.insertOne({
  pei: "imeisv-4901542032375186",
  supi: "imsi-001010123456789",
  status: "BLACKLISTED"
})

db.eir.insertOne({
  pei: "imei-490154203237518",
  status: "GREYLISTED"
})
```

`status` must be one of `WHITELISTED`, `BLACKLISTED`, or `GREYLISTED`. Lookup precedence is deterministic:

1. Exact `supi` + `pei` match
2. `pei`-only match (a record with no `supi` field)
3. No match — the AMF/EIR treats the equipment as unknown (`404 ERROR_EQUIPMENT_UNKNOWN`)

A record with an unrecognized `status` value, or more than one record matching the same lookup key, is treated as a database error — it is never interpreted as an implicit whitelist.

Recommended indexes:

```javascript
db.eir.createIndex({ pei: 1, supi: 1 }, { name: "eir_pei_supi_idx" })
db.eir.createIndex({ pei: 1 }, { name: "eir_pei_idx" })
```

## 4. AMF configuration
---

```yaml
amf:
  eir:
    enabled: false
    unknown_action: allow
    failure_action: allow
```

* `enabled` — when `false` (the default), the AMF never discovers 5G-EIR and never sends an equipment status request; registration behavior is unchanged from before this feature existed.
* `unknown_action` — applied when the 5G-EIR responds `404 ERROR_EQUIPMENT_UNKNOWN` (no applicable record). `allow` (default) continues registration; `reject` rejects it.
* `failure_action` — applied to operational failures: NRF discovery failure, SBI timeout, HTTP 5xx, or a malformed `200 OK` response. `allow` (default) continues registration; `reject` rejects it.

Regardless of `unknown_action`/`failure_action`, an explicit `200 OK` with `status: BLACKLISTED` always rejects the UE with 5GMM cause #6 (Illegal ME). `GREYLISTED` is logged and does not affect registration in this initial implementation.

The check runs once per initial registration attempt, after NAS security is established and the UE's PEI has been obtained (during Security Mode Complete processing) and before Registration Accept. It does not run for Service Request. If the UE did not provide a PEI, the check is skipped and registration proceeds as it did before this feature existed.

## 5. Limitations
---

This initial implementation supports exact PEI matching, exact SUPI+PEI overrides, and the `WHITELISTED`/`BLACKLISTED`/`GREYLISTED` statuses checked by the AMF at registration time. It does not include: EPC/MME S13, Diameter EIR, CEIR federation, TAC-range or wildcard rules, bulk import, or a WebUI/administrative API for managing EIR records — those records are managed directly in MongoDB.
