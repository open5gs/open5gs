---
title: 5G-EIR (Equipment Identity Register)
---

## 1. What is the 5G-EIR, and do I need it?
---

The 5G-EIR is the network function that lets your core check *which devices* are allowed to connect — separately from checking *which subscribers* are allowed to connect (that's what your subscriber database and AUSF/UDM already do).

Think of it like a phone's IMEI blocklist: every device has a unique hardware identity (its IMEI, or IMEISV), and the EIR lets you keep a list of devices that are trusted (whitelisted), banned (blacklisted), or merely flagged for monitoring (greylisted) — regardless of which SIM is inserted into them.

**You probably want this if:**
- A device has been reported lost or stolen and you want to block it from your network even if someone puts a new SIM in it.
- You need to comply with a regulatory requirement to support equipment identity checks.
- You want to keep a known-good device inventory and reject anything not on it.

**You can skip this if:**
- You're just running a lab/test core, or your subscriber database is already enough access control for your use case.

Open5GS ships this as its own small network function, `open5gs-eird`, and it is **completely optional and off by default** — if you don't configure it, your core behaves exactly as it always has.

## 2. How it fits into a registration
---

When it's turned on, here's what happens when a UE (phone/device) tries to register with your network:

1. The UE goes through the normal authentication and security setup with the AMF, exactly as before.
2. Once the AMF knows the device's PEI (its IMEI/IMEISV — the AMF asks for this as part of security setup if it doesn't have it yet), it asks the 5G-EIR: *"is this device okay?"*
3. The 5G-EIR looks the device up and replies with one of three statuses:
   - **Whitelisted** → registration continues normally.
   - **Greylisted** → registration continues, but a warning is logged so an operator can notice.
   - **Blacklisted** → the AMF rejects the device with cause "Illegal ME" — the same rejection a real network sends to a blocked phone.
4. If the device isn't in the list at all, or the 5G-EIR can't be reached, the AMF falls back to whatever policy you've configured (allow by default, so a hiccup in the EIR never accidentally locks legitimate users out).

None of this touches the SIM/subscriber side of registration — it's purely a check on the physical device.

## 3. Quick start
---

### Step 1 — Start the 5G-EIR

It uses the same MongoDB database as the rest of Open5GS, so there's nothing extra to install or configure beyond a normal Open5GS setup:

```bash
$ open5gs-eird -c /etc/open5gs/eir.yaml
```

### Step 2 — Add a device to the list

Records live in MongoDB, in a collection called `eir`. You can add them with `mongosh` (or any MongoDB tool/GUI you're comfortable with):

```javascript
// Block a specific device (by its IMEISV), for every subscriber
db.eir.insertOne({
  pei: "imeisv-4901542032375186",
  status: "BLACKLISTED"
})

// Trust a specific device
db.eir.insertOne({
  pei: "imei-490154203237518",
  status: "WHITELISTED"
})
```

You can optionally narrow a record to a single subscriber by adding a `supi` field — see [Section 5](#5-going-further-per-subscriber-overrides) if you need that.

### Step 3 — Turn the check on in the AMF

By default the AMF doesn't ask the 5G-EIR anything. Turn it on in `amf.yaml`:

```yaml
amf:
  eir:
    enabled: true
    unknown_action: allow   # what to do if the device isn't in the list at all
    failure_action: allow   # what to do if the 5G-EIR can't be reached
```

That's it — restart the AMF, and blacklisted devices will now be rejected at registration.

## 4. Choosing your `allow`/`reject` policies
---

`unknown_action` and `failure_action` control what happens when the 5G-EIR *doesn't* give a clear "trusted"/"blocked" answer:

| Setting | When it applies | `allow` (default) | `reject` |
|---|---|---|---|
| `unknown_action` | The device isn't in the `eir` collection at all | Let it register | Block it |
| `failure_action` | The 5G-EIR can't be reached, times out, or returns an error | Let it register | Block it |
| `missing_pei_action` | The AMF never got the device's PEI | Let it register | Block it |

The safe starting point for most deployments is to leave everything on `allow`: a network hiccup or a device you simply haven't classified yet should never lock out real users. Switch a policy to `reject` only once you're confident your device inventory (and your EIR's uptime) is complete enough that "unknown" really should mean "not allowed here."

A `BLACKLISTED` device is **always** rejected, regardless of these settings — that one is never in doubt.

## 5. Going further: per-subscriber overrides
---

Normally a device's status applies no matter which SIM is in it. But you can override that for one specific subscriber by adding a `supi` field to a record:

```javascript
// This device is trusted in general...
db.eir.insertOne({
  pei: "imeisv-4901542032375186",
  status: "WHITELISTED"
})

// ...except for this one subscriber, whose combination is blacklisted
db.eir.insertOne({
  pei: "imeisv-4901542032375186",
  supi: "imsi-001010123456789",
  status: "BLACKLISTED"
})
```

A record naming a specific subscriber always wins over a general (device-only) record. This is useful if, say, a shared or loaned device is fine in general but should be blocked for one particular account.

## 6. Technical reference
---

The details below are for operators writing tooling around the `eir` collection, or curious about the internals — you don't need them to use the feature.

**Lookup precedence.** For a given `(supi, pei)`, the 5G-EIR checks, in order:
1. An exact `supi` + `pei` match.
2. A `pei`-only match (a record with `supi` missing or `null`).
3. If neither exists, the device is treated as unknown (`404`, cause `ERROR_EQUIPMENT_UNKNOWN`).

A record with an unrecognized `status`, or more than one record matching the same lookup, is treated as a database error — never as an implicit allow.

**Data integrity.** `open5gs-eird` automatically sets up, at startup:
- A unique index so at most one subscriber-specific record can exist per `(pei, supi)` pair.
- A unique index so at most one general record can exist per `pei`.
- A schema validator requiring `pei` (string) and `status` (one of `WHITELISTED`/`BLACKLISTED`/`GREYLISTED`), rejecting malformed writes at the database level.

A general (device-only) record's `supi` field must be either omitted or explicitly `null` (not an empty string) — both are treated identically.

**Registration timing.** The check runs once per initial registration attempt, after NAS security is established and the device's PEI has been obtained, and before Registration Accept is sent. It does not run on Service Request.

**Current limitations.** This implementation covers PEI/SUPI-exact matching and the three standard statuses. It does not (yet) include: EPC/MME S13, Diameter EIR, CEIR federation, TAC-range/wildcard rules, bulk import, or a WebUI — records are managed directly in MongoDB for now.
