/*
 * Copyright (C) 2026 by Rami Mohamed <ramrode@gmail.com>
 *
 * This file is part of Open5GS.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef CHF_CONTEXT_H
#define CHF_CONTEXT_H

#include <time.h>

#include "ogs-app.h"
#include "ogs-crypt.h"
#include "ogs-sbi.h"
#include "ogs-dbi.h"

#include "chf-sm.h"

#ifdef __cplusplus
extern "C" {
#endif

extern int __chf_log_domain;

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __chf_log_domain

/*
 * CHF_RESOURCE_NAME_CHARGINGDATA - Nchf_ConvergedCharging resource path
 * (TS 32.291, section 6.1)
 */
#define CHF_RESOURCE_NAME_CHARGINGDATA  "chargingdata"
#define CHF_RESOURCE_NAME_UPDATE        "update"
#define CHF_RESOURCE_NAME_RELEASE       "release"
#define CHF_API_VERSION                 "v3"

/*
 * chf_quota_t — quota profile for a subscriber.
 *
 * Populated from MongoDB chf.subscribers on Create.
 * Falls back to chf_context_t.default_quota when subscriber is not provisioned.
 *
 * The quota is a SINGLE total value shared dynamically between UL and DL.
 * CHF grants grantedUnit.totalVolume; the UPF deducts both UL and DL octets
 * from that single counter so UL+DL can never exceed volume_bytes.
 *
 * TS 32.291 §6.3.1.2 — GrantedUnit / FinalUnitIndication
 * TS 32.291 §6.3.2.2 — Update trigger on validityTime expiry
 */
typedef struct chf_quota_s {
    uint64_t volume_bytes;      /* total (UL+DL) quota in bytes              */
    uint32_t validity_seconds;  /* billing period length (0 = no expiry)     */
    bool     auto_renew;        /* reset usage and re-grant on period expiry  */
    int      final_unit_action; /* 0=TERMINATE, 1=REDIRECT, 2=RESTRICT       */

    /*
     * Billing-cycle usage counter, persisted per subscriber in
     * chf.subscribers.usage (see db-handler.c).
     *
     * consumed_bytes accumulates every UL+DL delta the SMF reports, across
     * ALL of the subscriber's sessions, and survives session teardown.  That
     * is what makes a cycle allowance actually hold: summing only the live
     * sessions (chf_supi_aggregate_used) loses everything a closed session
     * used, so a subscriber who reconnects would get a fresh full grant.
     *
     * period_start anchors the cycle.  It is subscriber-scoped, unlike
     * chf_sess_t.period_start which only tracks one session's grant.
     *
     * Both are zero for a subscriber that is not provisioned in
     * chf.subscribers; that case falls back to per-session accounting.
     */
    uint64_t consumed_bytes;    /* usage.consumedBytes — cycle to date       */
    time_t   period_start;      /* usage.periodStart (0 = cycle not started) */

    /*
     * Top-ups bought for the current cycle (usage.topupBytes), added through
     * the TMF654 topupBalance API.  They extend this cycle only: the CHF
     * zeroes them when it opens a new cycle, so a top-up never carries over.
     */
    uint64_t topup_bytes;
} chf_quota_t;

/* Everything the subscriber may use this cycle: plan allowance + top-ups. */
static ogs_inline uint64_t chf_quota_allowance(const chf_quota_t *quota)
{
    return quota->volume_bytes + quota->topup_bytes;
}

/* Remaining cycle allowance, floored at zero. */
static ogs_inline uint64_t chf_quota_remaining(const chf_quota_t *quota)
{
    uint64_t allowance = chf_quota_allowance(quota);
    return allowance > quota->consumed_bytes
         ? allowance - quota->consumed_bytes : 0;
}

/*
 * True when the subscriber's billing cycle has run out.
 * A zero validity_seconds means "no expiry"; a zero period_start means the
 * cycle has not been opened yet (first ever session).
 */
static ogs_inline bool chf_quota_period_expired(
        const chf_quota_t *quota, time_t now)
{
    if (!quota->validity_seconds || !quota->period_start)
        return false;
    return now >= quota->period_start + (time_t)quota->validity_seconds;
}

/*
 * Size one grant: the lesser of what the session may still use and the
 * configured slice.  A zero max_bytes grants the whole remainder.
 */
static ogs_inline uint64_t chf_grant_volume(uint64_t remaining,
        uint64_t max_bytes)
{
    return (max_bytes && remaining > max_bytes) ? max_bytes : remaining;
}

/*
 * volumeQuotaThreshold for one grant (TS 32.291 MultipleUnitInformation):
 * how much of the grant may be LEFT when the SMF reports, so the next grant
 * arrives before this one runs out.  10% of the grant, except that a grant
 * of min_report_bytes or less is reported only once it is used up (0).
 *
 * Without that floor, each report near the end of an allowance leaves 10%
 * of the one before - 4.5 MB, 450 kB, 45 kB, ... - and the last megabyte
 * costs a burst of Updates that each grant almost nothing.
 */
static ogs_inline uint64_t chf_grant_threshold(uint64_t grant,
        uint64_t min_report_bytes)
{
    return grant > min_report_bytes ? grant / 10 : 0;
}

/*
 * validityTime for one grant: the lesser of the time left in the billing
 * cycle and the configured cap.  A zero cycle_left means the cycle has no
 * end, and a zero max_seconds means no cap; 0 returned means "omit".
 */
static ogs_inline uint32_t chf_grant_validity(uint32_t cycle_left,
        uint32_t max_seconds)
{
    if (!max_seconds)
        return cycle_left;
    if (!cycle_left || cycle_left > max_seconds)
        return max_seconds;
    return cycle_left;
}

/*
 * quotaHoldingTime for one grant (TS 32.291 MultipleUnitInformation): how
 * long the session may sit idle holding this grant before the SMF has to
 * report.  It is what makes an idle session give its unused grant back to
 * the cycle rather than keeping it until validityTime runs out - so a
 * top-up, a bar or a plan change is picked up after the idle period
 * instead of after the full validity.
 *
 * Returned as configured, with two exceptions: 0 turns idle reporting off,
 * and a value that outlasts the grant itself is pointless, because the
 * validity timer would fire first - so it is capped at validity.  A zero
 * validity means the grant has no timer and no cap applies.
 */
static ogs_inline uint32_t chf_grant_holding(uint32_t validity,
        uint32_t holding_seconds)
{
    if (!holding_seconds)
        return 0;
    if (validity && holding_seconds > validity)
        return validity;
    return holding_seconds;
}

/* ------------------------------------------------------------------ */
/* CDR encoding and filing                                              */
/*                                                                      */
/* The CHF builds one TS 32.298 ChargingRecord per closing session and   */
/* can write it as BER, as JSON, or both.  BER is what TS 32.298         */
/* clause 6.1 mandates on the Bx interface; the JSON is the same record  */
/* with the same field names, for anything that would rather read text.  */
/* ------------------------------------------------------------------ */

#define CHF_CDR_FMT_JSON    (1 << 0)
#define CHF_CDR_FMT_ASN1    (1 << 1)

/*
 * How one encoding's records are filed.
 *
 * Individually, a closing session writes its own file, named after its
 * ChargingDataRef - easy to correlate, but one file per session.
 * Aggregated, records are appended to a shared file until a threshold
 * trips and the next record starts a new one.  Either threshold alone is
 * enough; 0 disables that one, and 0 for both means the file only ever
 * rolls when the day does.
 */
typedef struct chf_cdr_filing_s {
    bool     aggregate;     /* false: one file per CDR                   */
    uint32_t max_records;   /* roll after this many records (0 = no cap) */
    uint64_t max_bytes;     /* roll once the file reaches this (0 = none) */
} chf_cdr_filing_t;

/* ------------------------------------------------------------------ */
/* Quota buckets                                                        */
/*                                                                      */
/* A subscriber no longer has one allowance.  A bucket is an allowance  */
/* for a scope: a slice, a DNN, a home PLMN, and whether the subscriber */
/* is roaming.  Every dimension can be left open, and an open dimension */
/* matches anything - a bucket with all four open is the subscriber's   */
/* catch-all, which is what a plain per-SUPI plan becomes.              */
/*                                                                      */
/* All four are carried in the ChargingDataRequest (TS 32.291): the     */
/* slice in pduSessionInformation.networkSlicingInfo.sNSSAI, the DNN in */
/* dnnId, the home PLMN in hPlmnId, and roaming in                      */
/* userInformation.roamerInOut.  Which bucket a request draws from is   */
/* the CHF's own business logic; 3GPP standardises the request and the  */
/* grant, not the operator's balance model.                             */
/*                                                                      */
/* Selection is by the operator's priority, highest first, so a scoped  */
/* bucket beats the catch-all only because it was given a higher        */
/* number.  Equal priorities are settled by age, oldest first, so the   */
/* choice never depends on the order MongoDB happened to return.        */
/* ------------------------------------------------------------------ */

#define CHF_MAX_BUCKETS_PER_SUPI    16
#define CHF_MAX_BUCKET_ID_LEN       128
#define CHF_MAX_PLMN_STR_LEN        16      /* "MCC-MNC" */
#define CHF_MAX_ROAMING_STR_LEN     12      /* "home" | "inBound" | "outBound" */

/* What a session is, for the purpose of picking its bucket.  A member
 * left empty means the request did not say. */
typedef struct chf_bucket_key_s {
    const char     *supi;
    ogs_s_nssai_t   s_nssai;    /* sst 0 = not given */
    const char     *dnn;        /* NULL = not given */
    const char     *hplmn;      /* "MCC-MNC", NULL = not given */
    const char     *roaming;    /* NULL = not given */
} chf_bucket_key_t;

/* What a bucket applies to.  An empty member is open. */
typedef struct chf_bucket_scope_s {
    ogs_s_nssai_t   s_nssai;    /* sst 0 = any slice */
    char            dnn[OGS_MAX_DNN_LEN + 1];       /* "" = any DNN */
    char            hplmn[CHF_MAX_PLMN_STR_LEN];    /* "" = any PLMN */
    char            roaming[CHF_MAX_ROAMING_STR_LEN]; /* "" = any */
} chf_bucket_scope_t;

typedef struct chf_bucket_s {
    char                id[CHF_MAX_BUCKET_ID_LEN];
    chf_bucket_scope_t  scope;
    int                 priority;
    chf_quota_t         quota;
    /* A pool: a bucket shared by several SIMs, listed in its members. */
    bool                shared;
} chf_bucket_t;

/*
 * Pools.  A pool is a bucket document with a members array of SUPIs and no
 * supi of its own; every member draws on the one plan and counter.  A SIM
 * is a member of one pool at most (a unique index on members), and may
 * hold buckets of its own as well.  Only the first max_members members, in
 * list order, draw on a pool; any past that are treated as not members.
 */
#define CHF_POOL_MAX_MEMBERS_DEFAULT    10
#define CHF_POOL_MAX_MEMBERS_LIMIT      1000

/*
 * The session pool: how many charging sessions one CHF can hold at once,
 * global.max.ue x chf.session_pool_multiplier.  Every slot is allocated at
 * start-up.  The default, 4, is what every other NF allows a UE (upstream's
 * OGS_MAX_NUM_OF_SESS), so a CHF sized like the core can hold a session for
 * each PDU session the core can carry.
 */
#define CHF_SESSION_POOL_MULTIPLIER_DEFAULT  4
#define CHF_SESSION_POOL_MULTIPLIER_LIMIT    64

/*
 * True when a bucket's scope covers a session.
 *
 * An open dimension covers anything.  A dimension the bucket names but
 * the request does not carry does NOT match: a bucket for "slice 1" must
 * not be spent by a session whose slice is unknown.
 */
bool chf_bucket_scope_matches(const chf_bucket_scope_t *scope,
        const chf_bucket_key_t *key);

/*
 * Pick the bucket to charge: the matching one with the highest priority.
 * At equal priority a SIM's own bucket beats its pool, and otherwise the
 * earlier entry wins (the caller passes them oldest first).  Returns its
 * index, or -1 when none matches.
 */
int chf_bucket_select(const chf_bucket_t *buckets, int num,
        const chf_bucket_key_t *key);

typedef struct chf_context_s {
    ogs_list_t      sess_list;
    ogs_hash_t      *charging_data_ref_hash; /* keyed by ChargingDataRef UUID */

    /*
     * Per-SUPI session index — keyed by SUPI, value is a chf_supi_index_t
     * holding the list of that subscriber's active sessions.  Lets
     * chf_supi_aggregate_used() sum only one subscriber's sessions
     * (O(sessions-for-this-SUPI)) instead of scanning the whole sess_list
     * (O(all-sessions)) on every Create and every Update.  TS 32.291 §5.2.
     */
    ogs_hash_t      *supi_index_hash;

    /* Default quota applied when SUPI is not in chf.subscribers.
     * Parsed from chf.yaml charging.default_quota; hardcoded fallback: 10 GB. */
    chf_quota_t     default_quota;

    /*
     * Upper bounds on a single grant (chf.yaml charging.grant).
     *
     * Without them a session is granted everything left in the cycle, and
     * the SMF does not come back until 90% of that is used - so a plan
     * change, a newly provisioned SUPI or a second device sharing the
     * allowance is not seen for gigabytes.  Capping the volume and the
     * validityTime makes every session report back regularly, and each
     * report re-reads the subscriber's current plan.  0 disables a cap.
     */
    struct {
        uint64_t max_volume_bytes;
        uint32_t max_validity_seconds;
        uint64_t min_report_bytes;  /* see chf_grant_threshold() */
        uint32_t quota_holding_seconds; /* see chf_grant_holding() */
    } grant;

    /* Reject a Create for a SUPI with no chf.subscribers document instead
     * of granting default_quota (chf.yaml charging.unprovisioned: reject). */
    bool reject_unprovisioned;

    /* Write CDRs for well-known DNN sessions, which are never quota-managed
     * (chf.yaml charging.well_known_dnn.cdr). */
    bool well_known_dnn_cdr;

    /*
     * ChargingNotify (chf.yaml charging.notify) — how often the CHF looks
     * in chf.notifications for something the billing side wants a live
     * session to hear now.  See src/chf/notify-path.h.
     *
     * The poll is one indexed find against a collection that is empty
     * almost always, so a short interval is cheap; it is the upper bound
     * on how long a top-up takes to reach a session that is up.
     */
    struct {
        bool     enable;
        uint32_t poll_seconds;
    } notify;

    /*
     * Pools (chf.yaml charging.pool).  max_members is how many SIMs one
     * pool may have.  The CHF charges only that many of a pool's members,
     * and publishes the number in chf.settings so the WebUI refuses to add
     * more.  Keep it the same on every CHF.
     */
    struct {
        uint32_t max_members;
    } pool;

    /*
     * chf.yaml chf.session_pool_multiplier: sessions per UE.  The session
     * pool holds global.max.ue x this many, sized once the config has been
     * read.  A full pool answers a Create with 500 INSUFFICIENT_RESOURCES.
     */
    uint32_t session_pool_multiplier;

    /* CDR writer config.
     * Parsed from chf.yaml charging.cdr.
     * TS 32.298 §5.1.5 — CHF CDR content; §5.2.5 — its ASN.1 syntax. */
    struct {
        bool  enable;           /* Write CDR files (default: true)        */
        char *path;             /* Base directory (default: /var/log/open5gs/cdr) */

        /* Which encodings to write, as CHF_CDR_FMT_* bits.  Both carry the
         * same TS 32.298 ChargingRecord; they differ only in how it is
         * serialised, so enabling both costs a second file, not a second
         * record model. */
        int formats;

        /* How each encoding is filed.  Separate per format because the two
         * tend to be consumed differently: a mediation system wants one
         * aggregated BER stream, while an operator reading JSON by hand
         * wants a file per session. */
        chf_cdr_filing_t json;
        chf_cdr_filing_t asn1;
    } cdr;

    /* SUPI ranges served by this CHF instance.
     * Registered in NRF profile (chfInfo.supiRangeList).
     * Empty (num_of_supi_range == 0) means serve all SUPIs.
     * Parsed from chf.yaml charging.supi_ranges[].{start,end}.
     * TS 29.510 §6.1.6.2.9 — SupiRange */
    int num_of_supi_range;
    struct {
        char *start;
        char *end;
    } supi_range[OGS_MAX_NUM_OF_SUPI_RANGE];

    /*
     * Slices this CHF instance serves, registered as NFProfile.allowedNssais
     * so that an SMF asking for a CHF on a given S-NSSAI only discovers the
     * instances that serve it (TS 33.518 §4.2.2.2.1).
     *
     * Empty — the shipped default — means the instance is eligible for every
     * slice, which is what a single-CHF deployment wants and what every
     * existing chf.yaml gets.
     *
     * This is deliberately NOT part of chfInfo: TS 29.510's ChfInfo has no
     * S-NSSAI member at all, only SUPI, GPSI and PLMN ranges.  allowedNssais
     * is the NF-profile-level mechanism, and the NRF matches it for any NF
     * type, so slice restriction and SUPI-range restriction are independent
     * filters that AND together.
     *
     * Matching is exact on both SST and SD.  A CHF that lists `sst: 1` with
     * no SD serves the S-NSSAI "SST 1, no SD" and NOT "SST 1, SD 000001" —
     * unlike a quota bucket's scope, where an omitted dimension is open.
     * The two are different mechanisms: this one is 3GPP's NF discovery,
     * the other is our billing model.
     *
     * Parsed from chf.yaml charging.slices[].{sst,sd}.
     */
    int num_of_slice;
    ogs_s_nssai_t slice[OGS_MAX_NUM_OF_SLICE];
} chf_context_t;

/*
 * chf_sess_t — one per active Nchf_ConvergedCharging session.
 *
 * Created on POST /chargingdata (Create).
 * Deleted on POST /chargingdata/{ref}/release (Release).
 * In production this is the in-memory working cache; MongoDB is the
 * authoritative store.
 */
struct chf_sess_s {
    ogs_sbi_object_t sbi;
    ogs_pool_id_t id;

    /* Membership in the per-SUPI session list (chf_supi_index_t.sess_list).
     * Distinct from the global chf_context_t.sess_list, which links sessions
     * through ogs_sbi_object_t. */
    ogs_lnode_t supi_node;

    /* ChargingDataRef UUID assigned at Create — used as session key */
    char *charging_data_ref;

    /* Subscriber identity from ChargingDataRequest */
    char *supi;
    char *gpsi;

    /*
     * Who asked for the charging, from nfConsumerIdentification
     * (TS 32.291 §7.2.1).  Two fields because the two consumers of it
     * want different things:
     *
     *   smf_id     nFName, an NfInstanceId - a UUID.  This is what the
     *              CDR records as NetworkFunctionName, which TS 32.298
     *              requires to be a UUIDv4.
     *   smf_label  a Prometheus label value, so it has to be stable:
     *              nFFqdn, else nFIPv4Address, else nFIPv6Address, else
     *              the instance id.  An instance id is regenerated on
     *              every SMF start, so labelling on it would grow
     *              cardinality without bound.
     *
     * Either may be NULL when the SMF sent nothing usable.
     */
    char *smf_id;
    char *smf_label;

    /* PDU session context */
    uint8_t psi;    /* PDU Session ID */
    char *dnn;
    ogs_s_nssai_t s_nssai;

    /* Accumulated usage counters (updated on each Update) */
    uint64_t ul_octets;
    uint64_t dl_octets;
    uint32_t duration;

    /* Rating group / service identifier (from ChargingDataRequest) */
    uint32_t rating_group;
    uint32_t service_id;

    /* Granted quota from subscriber profile.
     * granted_quota_total == quota.volume_bytes
     * granted_time_sec    == quota.validity_seconds
     * final_unit_action   == quota.final_unit_action
     *
     * A single combined allowance, not a per-direction pair: the CHF grants
     * it as grantedUnit.totalVolume, so the UPF deducts both uplink and
     * downlink octets from one counter (TS 32.291 §6.3.1.2 — GrantedUnit;
     * §6.3.2.2 — FinalUnitIndication). */
    uint64_t granted_quota_total; /* UL+DL quota bytes                  */
    uint32_t granted_time_sec;   /* validity period length in seconds   */
    int      final_unit_action;  /* 0=TERMINATE,1=REDIRECT,2=RESTRICT   */
    bool     final_unit;         /* true when usage >= granted total     */

    /* Validity period / auto-renew (TS 32.291 §6.3.2.2 — time-limit trigger)
     *
     * When auto_renew is true and the billing period has expired (detected on
     * the next CHF Update triggered by the UPF's validityTime countdown), the
     * CHF resets the accumulated usage counters and issues a fresh grant for
     * the next period.  When auto_renew is false the session is exhausted.
     *
     * period_start      — Unix timestamp (seconds) when the current period began
     * period_length_sec — length of one billing period (0 = no expiry check)
     * auto_renew        — whether to reset and re-grant on period expiry
     */
    time_t   period_start;       /* seconds since epoch, set on Create  */
    uint32_t period_length_sec;  /* copy of granted_time_sec at Create  */
    bool     auto_renew;         /* from subscriber profile             */

    /*
     * Whether chf.subscribers held a profile for this SUPI at Create.
     *
     * An Update's quota re-read can fail for two different reasons - the
     * SUPI is not provisioned, or MongoDB is briefly unavailable - and they
     * need opposite handling.  Only a genuinely unprovisioned session may
     * fall back to the configured default quota; a provisioned one must
     * keep its last known grant, because re-applying the default would
     * clear final_unit and could hand an exhausted subscriber the default
     * allowance for the length of the outage.
     */
    bool     provisioned;

    /*
     * The session's DNN is well known (ims, sos, mcptt, mcx; see
     * ogs_sbi_dnn_is_well_known()), so it is outside quota management.  It
     * has no bucket, adds to no counter, counts towards no other session's
     * allowance, and is never exhausted or told to end.  Its usage is still
     * tracked here, because the CDR needs it.
     */
    bool     quota_exempt;

    /*
     * The bucket this session draws from, chosen at Create and kept for
     * the session's life.  Its counters - not the subscriber's - are what
     * every Update adds to and re-reads, so a subscriber can hold several
     * allowances at once (one per slice, DNN, PLMN or roaming state).
     * Empty when the SUPI has no bucket and the default quota applies.
     */
    char     bucket_id[CHF_MAX_BUCKET_ID_LEN];

    /*
     * Where to reach this session between reports (TS 32.291 §6.3.1.2,
     * ChargingDataRequest.notifyUri), so a top-up or a bar does not have
     * to wait for the session's next Update.  NULL when the SMF sent
     * none, in which case the CHF can only answer reports.
     * See src/chf/notify-path.c.
     */
    char *notify_uri;

    /*
     * A ChargingNotify ABORT_CHARGING has been sent for this session, so
     * the operator closed it rather than the quota or the UE.  It decides
     * the CDR's causeForRecordClosing; see the Release handler.
     */
    bool aborted;
};

/*
 * The bucket a session charges.
 *
 * Falls back to the SUPI, which is the catch-all bucket's id: that is
 * what an unprovisioned session needs when its subscriber is provisioned
 * while it runs, and what a document written before buckets existed is
 * found by.  A scoped bucket created mid-session applies from the
 * subscriber's next session.
 */
static ogs_inline const char *chf_sess_bucket(const chf_sess_t *sess)
{
    return sess->bucket_id[0] ? sess->bucket_id : sess->supi;
}

/* ------------------------------------------------------------------ */
/* Context lifecycle                                                    */
/* ------------------------------------------------------------------ */
void chf_context_init(void);
void chf_context_final(void);
chf_context_t *chf_self(void);

int chf_context_parse_config(void);

/* ------------------------------------------------------------------ */
/* Session management                                                   */
/* ------------------------------------------------------------------ */
chf_sess_t *chf_sess_add(const char *supi, const char *charging_data_ref);
void chf_sess_remove(chf_sess_t *sess);
void chf_sess_remove_all(void);

chf_sess_t *chf_sess_find_by_id(ogs_pool_id_t id);
chf_sess_t *chf_sess_find_by_charging_data_ref(const char *charging_data_ref);

/* ------------------------------------------------------------------ */
/* Quota engine helpers (TS 32.291 §6.3 / §5.2)                      */
/* ------------------------------------------------------------------ */

/*
 * Map a FinalUnitAction enum value to its TS 32.291 §7.3 string.
 * 0/other → "TERMINATE", 1 → "REDIRECT", 2 → "RESTRICT_ACCESS".
 */
const char *chf_fua_to_string(int fua);

/* Parse a TS 32.291 §7.3 FinalUnitAction string back to its enum value. */
int chf_fua_from_string(const char *fua);

/*
 * Lower-case FinalUnitAction name, for use as a bounded metric label:
 * "terminate" | "redirect" | "restrict".
 */
const char *chf_fua_metric_label(int fua);

/*
 * Initialise session quota from a subscriber profile: seeds
 * granted_quota_total, granted_time_sec, final_unit_action, period_start,
 * period_length_sec and auto_renew, and resets final_unit to false.
 * TS 32.291 §6.3.1.
 */
void chf_apply_quota(chf_sess_t *sess, const chf_quota_t *quota);

/*
 * Check whether accumulated usage meets or exceeds the granted total.
 * Sets sess->final_unit = true on first detection.
 * Returns true if exhausted.  TS 32.291 §6.3.2.2.
 */
bool chf_check_quota_exhausted(chf_sess_t *sess);

/*
 * Clear a session's final unit once a recharge has given it quota again.
 *
 * chf_check_quota_exhausted() only ever sets final_unit, so without this an
 * exhausted session keeps receiving a FinalUnitIndication after the operator
 * tops it up, and the SMF terminates it at the end of the grant anyway.
 * Clears only on the genuine transition - the session was exhausted, and
 * both its own ceiling and the cycle counter now leave room - so a session
 * that stays exhausted is not re-announced on every Update.
 *
 * cycle_consumed/cycle_allowance are the bucket's persisted counter and
 * allowance from the fresh read.  Returns true if final_unit was cleared.
 */
bool chf_quota_revive(chf_sess_t *sess,
        uint64_t cycle_consumed, uint64_t cycle_allowance);

/*
 * Check whether the session's current billing period has expired.
 * Returns false when period_length_sec == 0 (no expiry configured).
 * TS 32.291 §6.3.2.2 — TIME_LIMIT trigger.
 */
bool chf_check_period_expired(chf_sess_t *sess);

/*
 * Sum of ul_octets + dl_octets across all active sessions whose supi
 * matches the given SUPI string.  Called before chf_sess_add() on Create
 * (new session not yet in list) and inside handle_update() for Case E.
 * TS 32.291 §5.2.
 */
uint64_t chf_supi_aggregate_used(const char *supi);

/* What each grant for a quota-exempt session offers when grant.max_volume_mb
 * sets no cap.  There is no ceiling behind it; the next grant is the same. */
#define CHF_QUOTA_EXEMPT_GRANT_BYTES (1024ULL * 1024ULL * 1024ULL)

/*
 * Run cb() for each live session of this SUPI that draws from bucket_id,
 * or for all of them when bucket_id is NULL or empty.  Returns how many
 * were visited.
 *
 * Uses the same per-SUPI index as chf_supi_aggregate_used(), so it costs
 * O(sessions-for-this-SUPI).  cb() must not add or remove sessions.
 */
int chf_sess_foreach_by_bucket(const char *supi, const char *bucket_id,
        void (*cb)(chf_sess_t *sess, void *data), void *data);

#ifdef __cplusplus
}
#endif

#endif /* CHF_CONTEXT_H */
