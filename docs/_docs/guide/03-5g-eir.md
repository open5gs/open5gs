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

For Debian/Ubuntu packages, install the optional NF explicitly:

```bash
$ sudo apt install open5gs-eir
```

It uses the same MongoDB database as the rest of Open5GS. For a source installation, start it with:

```bash
$ open5gs-eird -c /etc/open5gs/eir.yaml
```

On startup, the 5G-EIR automatically prepares missing indexes and a basic
validator in the configured MongoDB database. Existing indexes and validation
settings are preserved. If preparation fails, a warning identifies the problem
and EIR continues with runtime record validation; check these warnings before
provisioning devices. See [Data integrity](#data-integrity) for customization.

### Step 2 — Add a device to the list

Records live in MongoDB, in a collection called `eir`. Connect to the database
configured in `eir.yaml`, for example `mongosh mongodb://localhost/open5gs`.
With the default constraints prepared at startup, you can add records directly
with `mongosh` (or another MongoDB tool):

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

Blacklist rejection uses 5GMM cause #6 (`Illegal ME`). Rejection under
`unknown_action` or `missing_pei_action` uses #7 (`5GS services not allowed`).
Rejection under `failure_action` uses #90 (`Payload was not forwarded`),
including request transmission failures, timeouts, and invalid EIR responses.

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

A record with an unrecognized `status`, or more than one record matching the same
lookup, causes an HTTP `500` response. A malformed subscriber-specific match does
not fall back to a general record. The AMF applies `failure_action` to these errors;
with `allow`, registration can still proceed.

### Data integrity

After connecting to the database configured in `eir.yaml`, the 5G-EIR checks and
prepares its collection before opening its SBI service. Only the EIR NF performs
this setup; starting other Open5GS NFs does not create EIR constraints.

- If the `eir` collection is absent, EIR creates it with the default validator.
- If a suitable unique index on `(pei, supi)` is absent, EIR attempts to add it.
  An equivalent existing index is accepted even when its name differs.
- If an existing collection has no validator or explicit validation settings,
  EIR attempts to add the default validator.
- Existing indexes are never dropped or changed. Existing validators and
  explicit validation settings are preserved, including an empty validator,
  `validationLevel: "off"`, or `validationAction: "warn"`.

**Default unique index.** EIR creates the following index when needed. You do not
need to run this command after successful automatic preparation; it is shown for
reference and for operators who prepare the database themselves:

```javascript
db.eir.createIndex(
  { pei: 1, supi: 1 },
  { name: "pei_supi_unique", unique: true, collation: { locale: "simple" } }
)
```

This supports the lookup queries and prevents concurrent writers from creating
multiple records for the same `(pei, supi)`. It allows both a general record and
subscriber-specific overrides for a PEI. A missing `supi` and an explicit `null`
share the same index key, allowing only one general record per PEI. Do not use an
empty string for a general record's SUPI.

For an existing collection, review its indexes with `db.eir.getIndexes()` and
resolve duplicate records if automatic index creation fails. An older
non-partial unique index on `pei` alone prevents subscriber-specific overrides;
replacing that index requires an explicit operator migration. An index that
applies only to some records or uses different comparison rules is not assumed
to provide the default uniqueness constraint.

**Default validator and customization.** The default validator requires a string
PEI and a supported status, and permits a string, omitted, or null SUPI. EIR adds
these rules automatically when validation settings are absent. You can use the
following command to install or customize the rules yourself:

```javascript
db.runCommand({
  collMod: "eir",
  validator: {
    $jsonSchema: {
      bsonType: "object",
      required: ["pei", "status"],
      properties: {
        pei: { bsonType: "string" },
        supi: { bsonType: ["string", "null"] },
        status: { enum: ["WHITELISTED", "BLACKLISTED", "GREYLISTED"] }
      }
    }
  },
  validationLevel: "strict",
  validationAction: "error"
})
```

This command replaces the collection's existing validator. Inspect
`db.getCollectionInfos({ name: "eir" })` first and preserve any additional
operator rules. Your validator and validation settings remain in place across
EIR restarts. Complete schema customization before starting EIR: checking for
missing settings and adding them are separate operations, so concurrent schema
changes during startup are not coordinated.

Automatic preparation needs metadata-read permissions and, when settings are
missing, permission to create the collection or indexes and modify validation.
You can instead prepare the database using an administration account and run EIR
with record-read and metadata-read permissions. A metadata lookup failure is
reported as a failure, never interpreted as an absent index or validator.

If checking or creating constraints fails, EIR logs a warning and continues.
INFO logs identify constraints prepared at startup and existing settings that
were preserved. Check these messages together with any setup warnings.
Preparation commands limit server-side work to five seconds. For a large
existing collection, create the index manually if this limit is exceeded.
Correct missing permissions, duplicate records, or conflicting settings, then
restart EIR or complete the setup manually. A database connection failure still
prevents startup. Successful EIR startup alone does not guarantee that every
constraint was installed.

Neither startup nor the validator repairs or removes existing invalid records.
Runtime checks remain necessary for legacy or imported data. Without a suitable
index, lookups may scan the collection, and runtime duplicate detection cannot
prevent concurrent conflicting writes. Provisioning tools must also validate
the supported PEI/SUPI formats described below.

**Registration timing.** The check runs once per initial registration attempt, after NAS security is established and the device's PEI has been obtained, and before Registration Accept is sent. It does not run on Service Request.

**Current limitations.** This implementation supports `imei-` (15 digits), `imeisv-` (16 digits), and IMSI-based SUPIs (`imsi-`, 6–15 digits). Matching uses the supplied PEI string exactly; IMEI and IMEISV are not normalized to the same device identity. GPSI-based lookup, other PEI/SUPI formats, and optional feature negotiation are not implemented. It does not include EPC/MME S13, Diameter EIR, CEIR federation, TAC-range/wildcard rules, bulk import, or a WebUI for managing `eir` records.
