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

/*
 * CHF MongoDB data-access layer.
 *
 * Follows the Open5GS pattern from lib/dbi/ (subscription.c, session.c):
 *   - Include <mongoc.h> directly in this .c file only
 *   - Expose void* handles in the context struct to avoid polluting headers
 *   - Use BCON_NEW() for query/document construction
 *   - Use mongoc_collection_find_with_opts() for reads
 *   - Use insert_one / update_one / delete_one for writes
 *
 * 3GPP ref: TS 23.255 §6 — CHF interaction with OCS/data stores
 */

#include <time.h>

#include "db-handler.h"
#include "metrics.h"

/* Static handles for the three CHF-specific MongoDB collections.
 * Opened in chf_db_init(), destroyed in chf_db_final(). */
static mongoc_collection_t *subscribers_coll = NULL;
static mongoc_collection_t *sessions_coll    = NULL;
static mongoc_collection_t *cdrs_coll        = NULL;
static mongoc_collection_t *notifications_coll = NULL;

/* Record a MongoDB operation: count by collection/op/result + observe latency. */
static void chf_db_metric(const char *coll, const char *op, bool ok, int64_t t0)
{
    chf_metrics_inst_labeled_inc(CHF_METR_LBL_DB_OPS,
            coll, op, ok ? "ok" : "error");
    chf_metrics_inst_labeled_add(CHF_METR_LBL_DB_OP_DURATION,
            coll, op, NULL, (int)((ogs_get_monotonic_time() - t0) / 1000));
}

/*
 * Refresh the chf_provisioned_subscribers gauge, throttled to at most once
 * per 30s.  Driven by CHF Create traffic (called from the quota lookup) so no
 * dedicated timer is needed; countDocuments every 30s is cheap.
 */
static void chf_db_refresh_provisioned_gauge(void)
{
    static int64_t last_us = 0;
    int64_t now_us = ogs_get_monotonic_time();
    bson_t *filter;
    bson_error_t error;
    int64_t count;

    if (!subscribers_coll)
        return;
    if (last_us && (now_us - last_us) < (int64_t)30 * 1000000)
        return;
    last_us = now_us;

    filter = bson_new();
#if MONGOC_CHECK_VERSION(1, 9, 0)
    count = mongoc_collection_count_documents(
            subscribers_coll, filter, NULL, NULL, NULL, &error);
#else
    count = mongoc_collection_count(subscribers_coll,
            MONGOC_QUERY_NONE, filter, 0, 0, NULL, &error);
#endif
    bson_destroy(filter);

    if (count >= 0)
        chf_metrics_inst_global_set(CHF_METR_GAUGE_PROVISIONED_SUBS,
                (int)count);
    else
        ogs_warn("CHF DB: countDocuments(chf.subscribers) failed: %s",
                 error.message);
}

/* ------------------------------------------------------------------ */
/* Index management                                                     */
/* ------------------------------------------------------------------ */

/*
 * Run a createIndexes command (best-effort, non-fatal).
 *
 * createIndexes is idempotent: re-running with an identical spec is a no-op,
 * so this is safe on every startup.  Index names that match what the WebUI's
 * Mongoose models would create (e.g. "supi_1", "chargingDataRef_1") are used
 * where the key is the same, so the two paths converge on one index instead
 * of conflicting.  Takes ownership of `cmd` (destroys it).
 */
static void chf_db_run_create_indexes(
        mongoc_collection_t *coll, const char *coll_name, bson_t *cmd)
{
    bson_t reply;
    bson_error_t error;

    if (!coll || !cmd) {
        if (cmd) bson_destroy(cmd);
        return;
    }

    if (!mongoc_collection_command_simple(coll, cmd, NULL, &reply, &error))
        ogs_warn("CHF DB: createIndexes on %s failed (non-fatal): %s",
                 coll_name, error.message);
    else
        ogs_debug("CHF DB: indexes ensured on %s", coll_name);

    bson_destroy(&reply);
    bson_destroy(cmd);
}

/*
 * Ensure the indexes the CHF NF and the WebUI monitor rely on exist.
 *
 * Without these, the WebUI's paginated views (sort by startTime, filter by
 * supi / activity window) and the CHF's own per-session lookups degrade to
 * full collection scans as the number of sessions/subscribers grows.
 */
static void chf_db_ensure_indexes(void)
{
    /*
     * chf.sessions:
     *   chargingDataRef (unique)  — CHF Update/Release look up by ref
     *   startTime (desc)          — monitor sorts newest-first + activity window
     *   supi                      — monitor filters by subscriber
     */
    chf_db_run_create_indexes(sessions_coll, "chf.sessions",
        BCON_NEW("createIndexes", BCON_UTF8("chf.sessions"),
            "indexes", "[",
                "{", "key", "{", "chargingDataRef", BCON_INT32(1), "}",
                     "name", BCON_UTF8("chargingDataRef_1"),
                     "unique", BCON_BOOL(true), "}",
                "{", "key", "{", "startTime", BCON_INT32(-1), "}",
                     "name", BCON_UTF8("chf_startTime_desc"), "}",
                "{", "key", "{", "supi", BCON_INT32(1), "}",
                     "name", BCON_UTF8("chf_supi"), "}",
            "]"));

    /*
     * chf.subscribers, the same two indexes the WebUI's model declares, under
     * Mongoose's default names, so whichever starts first creates them and
     * the other's request is a no-op:
     *   supi     — bucket lookup on every CHF Create.  Not unique: one SUPI
     *              can hold a catch-all bucket and scoped ones.
     *   bucketId — unique and sparse: every usage write is keyed by it.
     *   members  — unique and sparse, on a pool's SIMs: a SIM can be in
     *              one pool only, and a Create finds its pool by it.
     * A unique supi index would refuse a subscriber's second bucket.
     */
    chf_db_run_create_indexes(subscribers_coll, "chf.subscribers",
        BCON_NEW("createIndexes", BCON_UTF8("chf.subscribers"),
            "indexes", "[",
                "{", "key", "{", "supi", BCON_INT32(1), "}",
                     "name", BCON_UTF8("supi_1"), "}",
                "{", "key", "{", "bucketId", BCON_INT32(1), "}",
                     "name", BCON_UTF8("bucketId_1"),
                     "unique", BCON_BOOL(true),
                     "sparse", BCON_BOOL(true), "}",
                "{", "key", "{", "members", BCON_INT32(1), "}",
                     "name", BCON_UTF8("members_1"),
                     "unique", BCON_BOOL(true),
                     "sparse", BCON_BOOL(true), "}",
            "]"));

    /*
     * chf.cdrs:
     *   supi + createdAt (desc) — CDR listing/export by subscriber and time.
     */
    chf_db_run_create_indexes(cdrs_coll, "chf.cdrs",
        BCON_NEW("createIndexes", BCON_UTF8("chf.cdrs"),
            "indexes", "[",
                "{", "key", "{", "supi", BCON_INT32(1), "}",
                     "name", BCON_UTF8("chf_cdr_supi"), "}",
                "{", "key", "{", "createdAt", BCON_INT32(-1), "}",
                     "name", BCON_UTF8("chf_cdr_createdAt_desc"), "}",
            "]"));
}

/* ------------------------------------------------------------------ */
/* Lifecycle                                                            */
/* ------------------------------------------------------------------ */

int chf_db_init(void)
{
    mongoc_client_t *client;
    const char *db_name;

    if (!ogs_mongoc() || !ogs_mongoc()->client || !ogs_mongoc()->name) {
        ogs_warn("MongoDB not available — CHF DB layer disabled");
        return OGS_OK;   /* non-fatal: CHF works in memory-only mode */
    }

    client  = (mongoc_client_t *)ogs_mongoc()->client;
    db_name = ogs_mongoc()->name;

    subscribers_coll = mongoc_client_get_collection(
            client, db_name, "chf.subscribers");
    if (!subscribers_coll) {
        ogs_error("Cannot get collection chf.subscribers");
        return OGS_ERROR;
    }

    sessions_coll = mongoc_client_get_collection(
            client, db_name, "chf.sessions");
    if (!sessions_coll) {
        ogs_error("Cannot get collection chf.sessions");
        return OGS_ERROR;
    }

    cdrs_coll = mongoc_client_get_collection(
            client, db_name, "chf.cdrs");
    if (!cdrs_coll) {
        ogs_error("Cannot get collection chf.cdrs");
        return OGS_ERROR;
    }

    /* chf.notifications is optional: the CHF works without it, it just
     * cannot be told about a top-up until the session next reports. */
    notifications_coll = mongoc_client_get_collection(
            client, db_name, "chf.notifications");
    if (!notifications_coll)
        ogs_warn("Cannot get collection chf.notifications — "
                 "a top-up or a bar will only reach a session at its "
                 "next report");

    chf_db_ensure_indexes();
    chf_db_publish_settings(client, db_name);

    ogs_info("CHF DB: collections ready "
             "(chf.subscribers / chf.sessions / chf.cdrs%s)",
            notifications_coll ? " / chf.notifications" : "");
    return OGS_OK;
}

void chf_db_final(void)
{
    if (notifications_coll) {
        mongoc_collection_destroy(notifications_coll);
        notifications_coll = NULL;
    }
    if (cdrs_coll) {
        mongoc_collection_destroy(cdrs_coll);
        cdrs_coll = NULL;
    }
    if (sessions_coll) {
        mongoc_collection_destroy(sessions_coll);
        sessions_coll = NULL;
    }
    if (subscribers_coll) {
        mongoc_collection_destroy(subscribers_coll);
        subscribers_coll = NULL;
    }
}

/* ------------------------------------------------------------------ */
/* Helper: current time in milliseconds (BSON Date uses int64 ms)     */
/* ------------------------------------------------------------------ */
static int64_t now_ms(void)
{
    return (int64_t)time(NULL) * 1000LL;
}

/* ------------------------------------------------------------------ */
/* Subscriber quota lookup                                              */
/* TS 32.291 §6.3.1.2 — quota lookup on Create                        */
/* ------------------------------------------------------------------ */

/*
 * bson_iter_u64() — read the value at `iter` as an unsigned 64-bit integer,
 * whatever numeric BSON type it happens to be stored as.
 *
 * Provisioned documents reach us from several writers — the WebUI (Mongoose
 * doubles), mongosh (int32 or double depending on magnitude), and hand-rolled
 * scripts (int64, or even a numeric string) — so every numeric field has to
 * accept all of them.  Handling the types per call site had drifted: some
 * keys accepted doubles and others silently ignored them, which read as 0.
 *
 * Returns false when the value is not numeric, leaving *out untouched.
 */
static bool bson_iter_u64(bson_iter_t *iter, uint64_t *out)
{
    if (BSON_ITER_HOLDS_DOUBLE(iter)) {
        double d = bson_iter_double(iter);
        *out = d > 0 ? (uint64_t)d : 0;
        return true;
    }
    if (BSON_ITER_HOLDS_INT64(iter)) {
        int64_t v = bson_iter_int64(iter);
        *out = v > 0 ? (uint64_t)v : 0;
        return true;
    }
    if (BSON_ITER_HOLDS_INT32(iter)) {
        int32_t v = bson_iter_int32(iter);
        *out = v > 0 ? (uint64_t)v : 0;
        return true;
    }
    if (BSON_ITER_HOLDS_UTF8(iter)) {
        uint32_t len;
        const char *str = bson_iter_utf8(iter, &len);
        if (str) {
            long long v = strtoll(str, NULL, 10);
            *out = v > 0 ? (uint64_t)v : 0;
            return true;
        }
    }
    return false;
}

/* ------------------------------------------------------------------ */
/* bucket_query()                                                       */
/*                                                                      */
/* Find one bucket by id.  A document written before buckets existed    */
/* carries no bucketId; its id is its SUPI, so match that too.  The     */
/* caller owns the returned bson_t.                                     */
/* ------------------------------------------------------------------ */
static bson_t *bucket_query(const char *bucket_id)
{
    ogs_assert(bucket_id);

    return BCON_NEW("$or", "[",
            "{", "bucketId", BCON_UTF8(bucket_id), "}",
            "{",
                "bucketId", "{", "$exists", BCON_BOOL(false), "}",
                "supi", BCON_UTF8(bucket_id),
            "}",
        "]");
}

/* ------------------------------------------------------------------ */
/* parse_quota_document()                                               */
/*                                                                      */
/* Fill a chf_quota_t from one chf.subscribers document: the plan and   */
/* the cycle counter.  Shared by the bucket loader and the per-bucket   */
/* re-read, which differ only in how they find the document.            */
/* ------------------------------------------------------------------ */
static void parse_quota_document(const bson_t *document, chf_quota_t *quota)
{
    bson_iter_t iter, quota_iter, usage_iter;

    ogs_assert(document);
    ogs_assert(quota);

    /* Initialise with safe defaults before overwriting from the document */
    memset(quota, 0, sizeof(*quota));
    quota->final_unit_action = 0; /* TERMINATE */

    if (bson_iter_init_find(&iter, document, "quota") &&
            BSON_ITER_HOLDS_DOCUMENT(&iter) &&
            bson_iter_recurse(&iter, &quota_iter)) {

        /*
         * Field priority (highest first):
         *
         *  1. totalQuotaBytes  — new unified field written by the updated WebUI.
         *                        A single total (UL+DL) value; CHF grants it as
         *                        grantedUnit.totalVolume so the UPF enforces the
         *                        combined limit across both directions.
         *
         *  2. volumeUlBytes + volumeDlBytes — legacy WebUI fields (pre-unification).
         *                        Summed to produce volume_bytes for backward compat.
         *
         *  3. volume_bytes     — direct-insert / unit-test scripts (snake_case).
         *
         *  4. autoRenew        — boolean; when true the CHF resets usage counters
         *                        and re-grants on billing period expiry.
         *
         *  5. validityPeriod   — human enum written by WebUI for display only;
         *                        the authoritative duration is validitySeconds.
         *                        Ignored by the C layer (WebUI converts to seconds).
         *
         * Field mapping (WebUI → chf_quota_t):
         *   totalQuotaBytes (Number)  → quota->volume_bytes  (primary)
         *   volumeUlBytes + volumeDlBytes → quota->volume_bytes  (legacy fallback)
         *   validitySeconds (Number)  → quota->validity_seconds
         *   autoRenew       (Bool)    → quota->auto_renew
         *   finalUnitAction (String)  → quota->final_unit_action
         */
        uint64_t ul_bytes = 0, dl_bytes = 0;
        bool has_total_quota  = false; /* totalQuotaBytes found */
        bool has_camel_volume = false; /* volumeUlBytes or volumeDlBytes found */

        while (bson_iter_next(&quota_iter)) {
            const char *key = bson_iter_key(&quota_iter);
            uint64_t v = 0;

            /* ---- Primary: unified total quota (new WebUI) ---- */
            if (!strcmp(key, "totalQuotaBytes")) {
                if (bson_iter_u64(&quota_iter, &v)) {
                    quota->volume_bytes = v;
                    has_total_quota = true;
                }

            /* ---- Legacy fallback: separate UL / DL bytes ---- */
            } else if (!strcmp(key, "volumeUlBytes")) {
                if (bson_iter_u64(&quota_iter, &ul_bytes))
                    has_camel_volume = true;
            } else if (!strcmp(key, "volumeDlBytes")) {
                if (bson_iter_u64(&quota_iter, &dl_bytes))
                    has_camel_volume = true;

            /* ---- Validity period length in seconds ---- */
            } else if (!strcmp(key, "validitySeconds") ||
                       !strcmp(key, "validity_seconds")) {
                if (bson_iter_u64(&quota_iter, &v))
                    quota->validity_seconds = (uint32_t)v;

            /* ---- Auto-renew flag ---- */
            } else if (!strcmp(key, "autoRenew")) {
                if (BSON_ITER_HOLDS_BOOL(&quota_iter))
                    quota->auto_renew = bson_iter_bool(&quota_iter);
                else if (bson_iter_u64(&quota_iter, &v))
                    quota->auto_renew = (v != 0);

            /* ---- Final unit action ----
             * Mongoose stores it as a string ("TERMINATE" | "REDIRECT" |
             * "RESTRICT_ACCESS"); direct-insert scripts may use the enum. */
            } else if (!strcmp(key, "finalUnitAction") ||
                       !strcmp(key, "final_unit_action")) {
                if (BSON_ITER_HOLDS_UTF8(&quota_iter)) {
                    uint32_t len;
                    quota->final_unit_action = chf_fua_from_string(
                            bson_iter_utf8(&quota_iter, &len));
                } else if (BSON_ITER_HOLDS_INT32(&quota_iter)) {
                    quota->final_unit_action = bson_iter_int32(&quota_iter);
                }

            /* ---- snake_case volume: very legacy / direct-insert scripts ---- */
            } else if (!strcmp(key, "volume_bytes")) {
                if (!has_total_quota && !has_camel_volume &&
                        bson_iter_u64(&quota_iter, &v))
                    quota->volume_bytes = v;
            }
        } /* while bson_iter_next */

        /*
         * Resolve volume_bytes from legacy UL+DL fields when totalQuotaBytes
         * was absent.  totalQuotaBytes always wins when present.
         */
        if (!has_total_quota && has_camel_volume)
            quota->volume_bytes = ul_bytes + dl_bytes;
    }

    /*
     * usage.* — billing-cycle counter (see chf_quota_t).  Absent on a
     * subscriber that has never had a session; both fields then stay zero
     * and the caller opens the cycle via chf_db_start_usage_period().
     */
    if (bson_iter_init_find(&iter, document, "usage") &&
            BSON_ITER_HOLDS_DOCUMENT(&iter) &&
            bson_iter_recurse(&iter, &usage_iter)) {

        while (bson_iter_next(&usage_iter)) {
            const char *key = bson_iter_key(&usage_iter);
            uint64_t v = 0;

            if (!strcmp(key, "consumedBytes")) {
                if (bson_iter_u64(&usage_iter, &v))
                    quota->consumed_bytes = v;
            } else if (!strcmp(key, "topupBytes")) {
                if (bson_iter_u64(&usage_iter, &v))
                    quota->topup_bytes = v;
            } else if (!strcmp(key, "periodStart")) {
                if (BSON_ITER_HOLDS_DATE_TIME(&usage_iter))
                    quota->period_start =
                        (time_t)(bson_iter_date_time(&usage_iter) / 1000);
                else if (bson_iter_u64(&usage_iter, &v))
                    quota->period_start = (time_t)v;
            }
        }
    }

}

/* ------------------------------------------------------------------ */
/* parse_bucket_document()                                              */
/*                                                                      */
/* A bucket is a document: its id, what it applies to, its priority and */
/* its plan.  A document written before buckets existed has none of the */
/* scope fields, which makes it the subscriber's catch-all with id =    */
/* SUPI - exactly what it meant when a subscriber had one allowance.    */
/* ------------------------------------------------------------------ */
static void parse_bucket_document(const bson_t *document,
        const char *supi, chf_bucket_t *bucket)
{
    bson_iter_t iter, scope_iter;
    uint32_t len;

    ogs_assert(document);
    ogs_assert(bucket);

    memset(bucket, 0, sizeof(*bucket));
    bucket->scope.s_nssai.sd.v = OGS_S_NSSAI_NO_SD_VALUE;

    if (bson_iter_init_find(&iter, document, "bucketId") &&
            BSON_ITER_HOLDS_UTF8(&iter))
        ogs_cpystrn(bucket->id, bson_iter_utf8(&iter, &len),
                sizeof(bucket->id));
    else
        ogs_cpystrn(bucket->id, supi, sizeof(bucket->id));

    if (bson_iter_init_find(&iter, document, "priority") &&
            BSON_ITER_HOLDS_NUMBER(&iter))
        bucket->priority = (int)bson_iter_as_int64(&iter);

    if (bson_iter_init_find(&iter, document, "scope") &&
            BSON_ITER_HOLDS_DOCUMENT(&iter) &&
            bson_iter_recurse(&iter, &scope_iter)) {

        while (bson_iter_next(&scope_iter)) {
            const char *key = bson_iter_key(&scope_iter);

            if (!strcmp(key, "snssai") &&
                    BSON_ITER_HOLDS_DOCUMENT(&scope_iter)) {
                bson_iter_t snssai_iter;
                if (!bson_iter_recurse(&scope_iter, &snssai_iter))
                    continue;
                while (bson_iter_next(&snssai_iter)) {
                    const char *sk = bson_iter_key(&snssai_iter);
                    if (!strcmp(sk, "sst") &&
                            BSON_ITER_HOLDS_NUMBER(&snssai_iter))
                        bucket->scope.s_nssai.sst =
                            (uint8_t)bson_iter_as_int64(&snssai_iter);
                    else if (!strcmp(sk, "sd") &&
                            BSON_ITER_HOLDS_UTF8(&snssai_iter))
                        bucket->scope.s_nssai.sd =
                            ogs_s_nssai_sd_from_string(
                                    bson_iter_utf8(&snssai_iter, &len));
                }
            } else if (!strcmp(key, "dnn") &&
                    BSON_ITER_HOLDS_UTF8(&scope_iter)) {
                ogs_cpystrn(bucket->scope.dnn,
                        bson_iter_utf8(&scope_iter, &len),
                        sizeof(bucket->scope.dnn));
            } else if (!strcmp(key, "hplmn") &&
                    BSON_ITER_HOLDS_UTF8(&scope_iter)) {
                ogs_cpystrn(bucket->scope.hplmn,
                        bson_iter_utf8(&scope_iter, &len),
                        sizeof(bucket->scope.hplmn));
            } else if (!strcmp(key, "roaming") &&
                    BSON_ITER_HOLDS_UTF8(&scope_iter)) {
                ogs_cpystrn(bucket->scope.roaming,
                        bson_iter_utf8(&scope_iter, &len),
                        sizeof(bucket->scope.roaming));
            }
        }
    }

    bucket->shared = chf_db_is_pool(document);

    parse_quota_document(document, &bucket->quota);
}

/* ------------------------------------------------------------------ */
/* Pools                                                                */
/* ------------------------------------------------------------------ */

bool chf_db_is_pool(const bson_t *document)
{
    bson_iter_t iter;

    return document &&
        bson_iter_init_find(&iter, document, "members") &&
        BSON_ITER_HOLDS_ARRAY(&iter);
}

int chf_db_pool_position(const bson_t *document, const char *supi)
{
    bson_iter_t iter, members;
    int i = 0;

    if (!supi || !chf_db_is_pool(document))
        return -1;
    if (!bson_iter_init_find(&iter, document, "members") ||
            !bson_iter_recurse(&iter, &members))
        return -1;

    while (bson_iter_next(&members)) {
        if (BSON_ITER_HOLDS_UTF8(&members) &&
                !strcmp(bson_iter_utf8(&members, NULL), supi))
            return i;
        i++;
    }
    return -1;
}

/*
 * Whether supi draws on this document.  Its own bucket always; a pool only
 * if supi is one of the first max_members members, and the pool has an id
 * to charge usage to.  Logs why a pool is passed over.
 */
static bool draws_on(const bson_t *document, const char *supi)
{
    bson_iter_t iter;
    int pos;

    if (!chf_db_is_pool(document)) {
        /* Matched on supi.  A document listing members is a pool even if
         * it also carries a supi, and is judged by its members alone. */
        return bson_iter_init_find(&iter, document, "supi") &&
            BSON_ITER_HOLDS_UTF8(&iter) &&
            !strcmp(bson_iter_utf8(&iter, NULL), supi);
    }

    pos = chf_db_pool_position(document, supi);
    if (pos < 0)
        return false;

    if (!bson_iter_init_find(&iter, document, "bucketId") ||
            !BSON_ITER_HOLDS_UTF8(&iter)) {
        ogs_warn("[%s] A pool with no bucketId cannot be charged — "
                 "skipped", supi);
        return false;
    }
    if ((uint32_t)pos >= chf_self()->pool.max_members) {
        ogs_warn("[%s] Member %d of pool [%s], past "
                 "charging.pool.max_members (%u) — not charged to it",
                 supi, pos + 1, bson_iter_utf8(&iter, NULL),
                 chf_self()->pool.max_members);
        return false;
    }
    return true;
}

/*
 * Write the settings the WebUI has to follow into chf.settings, so a limit
 * set in chf.yaml is enforced where members are added, not only here.
 * Several CHFs write the same document; the last to start wins, which is
 * why the setting should be the same on all of them.  Failure is logged
 * and not fatal: the WebUI then falls back to the shipped default.
 */
void chf_db_publish_settings(mongoc_client_t *client, const char *db_name)
{
    mongoc_collection_t *coll;
    bson_t *selector, *update, *opts;
    bson_error_t error;
    const char *me = (ogs_sbi_self()->nf_instance &&
            ogs_sbi_self()->nf_instance->id) ?
        ogs_sbi_self()->nf_instance->id : "";

    if (!client || !db_name)
        return;

    coll = mongoc_client_get_collection(client, db_name, "chf.settings");
    if (!coll)
        return;

    selector = BCON_NEW("_id", BCON_UTF8("pool"));
    update = BCON_NEW("$set", "{",
            "maxMembers", BCON_INT32((int32_t)chf_self()->pool.max_members),
            "publishedBy", BCON_UTF8(me),
            "updatedAt", BCON_DATE_TIME(now_ms()), "}");
    opts = BCON_NEW("upsert", BCON_BOOL(true));

    if (mongoc_collection_update_one(coll, selector, update, opts, NULL,
                &error))
        ogs_info("CHF DB: charging.pool.max_members %u published to "
                 "chf.settings", chf_self()->pool.max_members);
    else
        ogs_warn("CHF DB: cannot publish chf.settings: %s", error.message);

    bson_destroy(opts);
    bson_destroy(update);
    bson_destroy(selector);
    mongoc_collection_destroy(coll);
}

/* ------------------------------------------------------------------ */
/* chf_db_load_buckets()                                                */
/*                                                                      */
/* Every bucket a subscriber draws on - its own, and its pool if it is  */
/* a member - oldest first so that equal priorities resolve the same    */
/* way on every lookup.  Returns the count, 0 when the SUPI is not      */
/* provisioned, or OGS_ERROR when MongoDB could not be read - which     */
/* says nothing about whether it is provisioned.                        */
/* ------------------------------------------------------------------ */
int chf_db_load_buckets(const char *supi, chf_bucket_t *buckets, int max)
{
    mongoc_cursor_t *cursor = NULL;
    bson_t *query = NULL, *opts = NULL;
    bson_error_t error;
    const bson_t *document;
    int count = 0;
    int64_t t0;
    bool db_ok = true;

    ogs_assert(supi);
    ogs_assert(buckets);
    ogs_assert(max > 0);

    if (!subscribers_coll) {
        ogs_debug("[%s] chf.subscribers not available", supi);
        chf_metrics_inst_global_inc(CHF_METR_CTR_DB_UNAVAILABLE);
        return OGS_ERROR;
    }

    chf_db_refresh_provisioned_gauge();

    query = BCON_NEW("$or", "[",
            "{", "supi", BCON_UTF8(supi), "}",
            "{", "members", BCON_UTF8(supi), "}",
        "]");
    opts  = BCON_NEW("sort", "{", "_id", BCON_INT32(1), "}");

    t0 = ogs_get_monotonic_time();
#if MONGOC_CHECK_VERSION(1, 5, 0)
    cursor = mongoc_collection_find_with_opts(
            subscribers_coll, query, opts, NULL);
#else
    cursor = mongoc_collection_find(subscribers_coll,
            MONGOC_QUERY_NONE, 0, 0, 0, query, NULL, NULL);
#endif

    while (count < max && mongoc_cursor_next(cursor, &document)) {
        if (!draws_on(document, supi))
            continue;
        parse_bucket_document(document, supi, &buckets[count++]);
    }

    if (mongoc_cursor_error(cursor, &error)) {
        ogs_error("[%s] Cursor failure: %s", supi, error.message);
        db_ok = false;
        count = OGS_ERROR;
    } else if (count == max) {
        ogs_warn("[%s] More than %d buckets; the rest are ignored",
                supi, max);
    }

    chf_db_metric("subscribers", "find", db_ok, t0);
    mongoc_cursor_destroy(cursor);
    bson_destroy(opts);
    bson_destroy(query);

    return count;
}

/* ------------------------------------------------------------------ */
/* chf_db_get_bucket_quota()                                            */
/*                                                                      */
/* Re-read one bucket by its id, for an Update that needs the current   */
/* plan and counter.  A document written before buckets existed has no  */
/* bucketId, and is found by SUPI instead.  For a pool, *member says    */
/* whether supi still draws on it: false once it has been removed, or   */
/* when it is past charging.pool.max_members.                           */
/* ------------------------------------------------------------------ */
int chf_db_get_bucket_quota(const char *bucket_id, const char *supi,
        chf_quota_t *quota, bool *member)
{
    mongoc_cursor_t *cursor = NULL;
    bson_t *query = NULL;
    bson_error_t error;
    const bson_t *document;
    int rv = OGS_RETRY;
    int64_t t0;
    bool db_ok = true;

    ogs_assert(bucket_id);
    ogs_assert(quota);

    if (member)
        *member = true;

    if (!subscribers_coll) {
        chf_metrics_inst_global_inc(CHF_METR_CTR_DB_UNAVAILABLE);
        return OGS_ERROR;
    }

    query = bucket_query(bucket_id);

    t0 = ogs_get_monotonic_time();
#if MONGOC_CHECK_VERSION(1, 5, 0)
    cursor = mongoc_collection_find_with_opts(
            subscribers_coll, query, NULL, NULL);
#else
    cursor = mongoc_collection_find(subscribers_coll,
            MONGOC_QUERY_NONE, 0, 0, 0, query, NULL, NULL);
#endif

    if (!mongoc_cursor_next(cursor, &document)) {
        if (mongoc_cursor_error(cursor, &error)) {
            ogs_error("[%s] Cursor failure: %s", bucket_id, error.message);
            db_ok = false;
            rv = OGS_ERROR;
        } else {
            ogs_debug("[%s] Bucket is gone", bucket_id);
            rv = OGS_RETRY;
        }
        goto out;
    }

    if (member && supi && chf_db_is_pool(document))
        *member = draws_on(document, supi);

    parse_quota_document(document, quota);
    rv = OGS_OK;

out:
    chf_db_metric("subscribers", "find", db_ok, t0);
    mongoc_cursor_destroy(cursor);
    bson_destroy(query);
    return rv;
}

/* ------------------------------------------------------------------ */
/* chf_db_get_catchall_quota()                                          */
/*                                                                      */
/* For an Update from a session that started with no bucket of its own: */
/* what it may now be charged to.  The SIM's own catch-all if it has     */
/* one, as before; otherwise its pool, if it has joined one since and    */
/* the pool covers every session (no scope).  A scoped pool, like a      */
/* scoped bucket, applies from the SIM's next session, because only a    */
/* Create matches a session against a scope.  pool_id is set when the    */
/* answer is a pool, and left empty otherwise.                          */
/* ------------------------------------------------------------------ */
int chf_db_get_catchall_quota(const char *supi, chf_quota_t *quota,
        char *pool_id, size_t pool_id_size)
{
    mongoc_cursor_t *cursor = NULL;
    bson_t *query = NULL, *opts = NULL;
    bson_t *pool_doc = NULL;
    bson_error_t error;
    const bson_t *document;
    int rv = OGS_RETRY;
    int64_t t0;
    bool db_ok = true, own = false;

    ogs_assert(supi);
    ogs_assert(quota);
    ogs_assert(pool_id && pool_id_size);

    pool_id[0] = '\0';

    if (!subscribers_coll) {
        chf_metrics_inst_global_inc(CHF_METR_CTR_DB_UNAVAILABLE);
        return OGS_ERROR;
    }

    query = BCON_NEW("$or", "[",
            "{", "bucketId", BCON_UTF8(supi), "}",
            "{",
                "bucketId", "{", "$exists", BCON_BOOL(false), "}",
                "supi", BCON_UTF8(supi),
            "}",
            "{", "members", BCON_UTF8(supi), "}",
        "]");
    opts = BCON_NEW("sort", "{", "_id", BCON_INT32(1), "}");

    t0 = ogs_get_monotonic_time();
#if MONGOC_CHECK_VERSION(1, 5, 0)
    cursor = mongoc_collection_find_with_opts(
            subscribers_coll, query, opts, NULL);
#else
    cursor = mongoc_collection_find(subscribers_coll,
            MONGOC_QUERY_NONE, 0, 0, 0, query, NULL, NULL);
#endif

    while (mongoc_cursor_next(cursor, &document)) {
        if (!chf_db_is_pool(document)) {
            /* The SIM's own catch-all comes first, as it always has. */
            parse_quota_document(document, quota);
            own = true;
            break;
        }
        if (!pool_doc && draws_on(document, supi)) {
            chf_bucket_t b;
            parse_bucket_document(document, supi, &b);
            if (!b.scope.s_nssai.sst && !b.scope.dnn[0] &&
                    !b.scope.hplmn[0] && !b.scope.roaming[0])
                pool_doc = bson_copy(document);
        }
    }

    if (mongoc_cursor_error(cursor, &error)) {
        ogs_error("[%s] Cursor failure: %s", supi, error.message);
        db_ok = false;
        rv = OGS_ERROR;
    } else if (own) {
        rv = OGS_OK;
    } else if (pool_doc) {
        bson_iter_t iter;
        if (bson_iter_init_find(&iter, pool_doc, "bucketId") &&
                BSON_ITER_HOLDS_UTF8(&iter))
            ogs_cpystrn(pool_id, bson_iter_utf8(&iter, NULL), pool_id_size);
        parse_quota_document(pool_doc, quota);
        rv = OGS_OK;
    }

    if (pool_doc)
        bson_destroy(pool_doc);
    chf_db_metric("subscribers", "find", db_ok, t0);
    mongoc_cursor_destroy(cursor);
    bson_destroy(opts);
    bson_destroy(query);
    return rv;
}

int chf_db_add_usage(const char *bucket_id, uint64_t delta_bytes)
{
    bson_t *query = NULL, *update = NULL;
    bson_t reply;
    bson_iter_t it;
    bson_error_t error;
    bool ok;
    int64_t t0;
    int rv = OGS_OK;

    ogs_assert(bucket_id);

    if (!subscribers_coll)
        return OGS_RETRY;
    if (!delta_bytes)
        return OGS_OK;

    query  = bucket_query(bucket_id);
    update = BCON_NEW(
            "$inc", "{", "usage.consumedBytes",
                         BCON_INT64((int64_t)delta_bytes), "}",
            "$set", "{", "usage.updatedAt", BCON_DATE_TIME(now_ms()), "}");

    t0 = ogs_get_monotonic_time();
    ok = mongoc_collection_update_one(
            subscribers_coll, query, update, NULL, &reply, &error);

    if (!ok) {
        ogs_error("[%s] Cannot add usage: %s", bucket_id, error.message);
        rv = OGS_ERROR;
    } else if (bson_iter_init_find(&it, &reply, "matchedCount") &&
               bson_iter_as_int64(&it) == 0) {
        /* No such bucket - the SUPI is not provisioned, or the bucket was
         * deleted while the session was running. */
        rv = OGS_RETRY;
    }

    chf_db_metric("subscribers", "inc_usage", ok, t0);

    bson_destroy(&reply);
    bson_destroy(update);
    bson_destroy(query);
    return rv;
}

int chf_db_start_usage_period(const char *bucket_id)
{
    bson_t *query = NULL, *update = NULL;
    bson_error_t error;
    bool ok;
    int64_t t0, now = now_ms();

    ogs_assert(bucket_id);

    if (!subscribers_coll)
        return OGS_RETRY;

    query  = bucket_query(bucket_id);
    /* Top-ups belong to the cycle they were bought for; a new cycle starts
     * without them. */
    update = BCON_NEW("$set", "{",
            "usage.consumedBytes", BCON_INT64(0),
            "usage.topupBytes",    BCON_INT64(0),
            "usage.periodStart",   BCON_DATE_TIME(now),
            "usage.updatedAt",     BCON_DATE_TIME(now),
            "}");

    t0 = ogs_get_monotonic_time();
    ok = mongoc_collection_update_one(
            subscribers_coll, query, update, NULL, NULL, &error);
    if (!ok)
        ogs_error("[%s] Cannot start usage period: %s",
                bucket_id, error.message);

    chf_db_metric("subscribers", "reset_usage", ok, t0);

    bson_destroy(update);
    bson_destroy(query);
    return ok ? OGS_OK : OGS_ERROR;
}

int chf_db_open_usage_period(const char *bucket_id)
{
    bson_t *query = NULL, *update = NULL;
    bson_error_t error;
    bool ok;
    int64_t t0, now = now_ms();

    ogs_assert(bucket_id);

    if (!subscribers_coll)
        return OGS_RETRY;

    /* Only if no cycle is open: a concurrent Create may have opened one. */
    {
        bson_t *by_id = bucket_query(bucket_id);
        query = BCON_NEW("usage.periodStart", BCON_NULL);
        bson_concat(query, by_id);
        bson_destroy(by_id);
    }
    update = BCON_NEW("$set", "{",
            "usage.periodStart", BCON_DATE_TIME(now),
            "usage.updatedAt",   BCON_DATE_TIME(now),
            "}");

    t0 = ogs_get_monotonic_time();
    ok = mongoc_collection_update_one(
            subscribers_coll, query, update, NULL, NULL, &error);
    if (!ok)
        ogs_error("[%s] Cannot open usage period: %s",
                bucket_id, error.message);

    chf_db_metric("subscribers", "open_usage", ok, t0);

    bson_destroy(update);
    bson_destroy(query);
    return ok ? OGS_OK : OGS_ERROR;
}

int chf_db_session_create(chf_sess_t *sess)
{
    bson_t *doc = NULL;
    bson_error_t error;
    bool ok;
    int64_t t0;

    ogs_assert(sess);
    ogs_assert(sess->charging_data_ref);
    ogs_assert(sess->supi);

    if (!sessions_coll) {
        ogs_debug("[%s] sessions_coll not available", sess->supi);
        return OGS_OK; /* non-fatal in memory-only mode */
    }

    doc = BCON_NEW(
        "chargingDataRef",  BCON_UTF8(sess->charging_data_ref),
        "supi",             BCON_UTF8(sess->supi),
        "gpsi",             BCON_UTF8(sess->gpsi ? sess->gpsi : ""),
        "dnn",              BCON_UTF8(sess->dnn  ? sess->dnn  : ""),
        "pduSessionId",     BCON_INT32((int32_t)sess->psi),
        "ulOctets",         BCON_INT64((int64_t)sess->ul_octets),
        "dlOctets",         BCON_INT64((int64_t)sess->dl_octets),
        "totalOctets",      BCON_INT64((int64_t)(sess->ul_octets + sess->dl_octets)),
        "duration",         BCON_INT32((int32_t)sess->duration),
        "grantedTotalBytes",BCON_INT64((int64_t)sess->granted_quota_total),
        "autoRenew",        BCON_BOOL(sess->auto_renew),
        "periodLengthSec",  BCON_INT32((int32_t)sess->period_length_sec),
        "startTime",        BCON_DATE_TIME(now_ms()),
        "updatedAt",        BCON_DATE_TIME(now_ms())
    );

    t0 = ogs_get_monotonic_time();
#if MONGOC_CHECK_VERSION(1, 5, 0)
    ok = mongoc_collection_insert_one(
            sessions_coll, doc, NULL, NULL, &error);
#else
    ok = mongoc_collection_insert(sessions_coll,
            MONGOC_INSERT_NONE, doc, NULL, &error);
#endif
    chf_db_metric("sessions", "insert", ok, t0);

    if (!ok) {
        ogs_error("[%s] chf.sessions insert failed: %s",
                sess->supi, error.message);
        bson_destroy(doc);
        return OGS_ERROR;
    }

    bson_destroy(doc);
    ogs_debug("[%s] Session created in DB [ref:%s]",
            sess->supi, sess->charging_data_ref);
    return OGS_OK;
}

int chf_db_session_update(chf_sess_t *sess)
{
    bson_t *query  = NULL;
    bson_t *update = NULL;
    bson_error_t error;
    bool ok;
    int64_t t0;

    ogs_assert(sess);
    ogs_assert(sess->charging_data_ref);

    if (!sessions_coll) {
        ogs_debug("[%s] sessions_coll not available", sess->supi);
        return OGS_OK;
    }

    query = BCON_NEW("chargingDataRef",
                     BCON_UTF8(sess->charging_data_ref));

    update = BCON_NEW(
        "$set", "{",
            "ulOctets",    BCON_INT64((int64_t)sess->ul_octets),
            "dlOctets",    BCON_INT64((int64_t)sess->dl_octets),
            "totalOctets", BCON_INT64((int64_t)(sess->ul_octets + sess->dl_octets)),
            "duration",    BCON_INT32((int32_t)sess->duration),
            "updatedAt",   BCON_DATE_TIME(now_ms()),
        "}"
    );

    t0 = ogs_get_monotonic_time();
#if MONGOC_CHECK_VERSION(1, 5, 0)
    ok = mongoc_collection_update_one(
            sessions_coll, query, update, NULL, NULL, &error);
#else
    ok = mongoc_collection_update(sessions_coll,
            MONGOC_UPDATE_NONE, query, update, NULL, &error);
#endif
    chf_db_metric("sessions", "update", ok, t0);

    if (!ok) {
        ogs_error("[%s] chf.sessions update failed: %s",
                sess->supi, error.message);
        bson_destroy(query);
        bson_destroy(update);
        return OGS_ERROR;
    }

    bson_destroy(query);
    bson_destroy(update);
    ogs_debug("[%s] Session updated in DB [ref:%s ul:%llu dl:%llu dur:%u]",
            sess->supi, sess->charging_data_ref,
            (unsigned long long)sess->ul_octets,
            (unsigned long long)sess->dl_octets,
            sess->duration);
    return OGS_OK;
}

int chf_db_session_delete(chf_sess_t *sess)
{
    bson_t *query = NULL;
    bson_error_t error;
    bool ok;
    int64_t t0;

    ogs_assert(sess);
    ogs_assert(sess->charging_data_ref);

    if (!sessions_coll) {
        ogs_debug("[%s] sessions_coll not available", sess->supi);
        return OGS_OK;
    }

    query = BCON_NEW("chargingDataRef",
                     BCON_UTF8(sess->charging_data_ref));

    t0 = ogs_get_monotonic_time();
#if MONGOC_CHECK_VERSION(1, 5, 0)
    ok = mongoc_collection_delete_one(
            sessions_coll, query, NULL, NULL, &error);
#else
    ok = mongoc_collection_remove(sessions_coll,
            MONGOC_REMOVE_SINGLE_REMOVE, query, NULL, &error);
#endif
    chf_db_metric("sessions", "delete", ok, t0);

    if (!ok) {
        ogs_error("[%s] chf.sessions delete failed: %s",
                sess->supi, error.message);
        bson_destroy(query);
        return OGS_ERROR;
    }

    bson_destroy(query);
    ogs_debug("[%s] Session deleted from DB [ref:%s]",
            sess->supi, sess->charging_data_ref);
    return OGS_OK;
}

/* ------------------------------------------------------------------ */
/* CDR index                                                            */
/* ------------------------------------------------------------------ */

int chf_db_cdr_insert(chf_sess_t *sess, const char *cdr_file_path)
{
    bson_t *doc = NULL;
    bson_error_t error;
    bool ok;
    int64_t t0;

    ogs_assert(sess);
    ogs_assert(sess->charging_data_ref);

    if (!cdrs_coll) {
        ogs_debug("[%s] cdrs_coll not available", sess->supi);
        return OGS_OK;
    }

    doc = BCON_NEW(
        "chargingDataRef", BCON_UTF8(sess->charging_data_ref),
        "supi",            BCON_UTF8(sess->supi),
        "dnn",             BCON_UTF8(sess->dnn ? sess->dnn : ""),
        "file_path",       BCON_UTF8(cdr_file_path ? cdr_file_path : ""),
        "ulOctets",        BCON_INT64((int64_t)sess->ul_octets),
        "dlOctets",        BCON_INT64((int64_t)sess->dl_octets),
        "duration",        BCON_INT32((int32_t)sess->duration),
        "createdAt",       BCON_DATE_TIME(now_ms())
    );

    t0 = ogs_get_monotonic_time();
#if MONGOC_CHECK_VERSION(1, 5, 0)
    ok = mongoc_collection_insert_one(
            cdrs_coll, doc, NULL, NULL, &error);
#else
    ok = mongoc_collection_insert(cdrs_coll,
            MONGOC_INSERT_NONE, doc, NULL, &error);
#endif
    chf_db_metric("cdrs", "insert", ok, t0);

    if (!ok) {
        ogs_error("[%s] chf.cdrs insert failed: %s",
                sess->supi, error.message);
        bson_destroy(doc);
        return OGS_ERROR;
    }

    bson_destroy(doc);
    ogs_debug("[%s] CDR index inserted [ref:%s file:%s]",
            sess->supi, sess->charging_data_ref,
            cdr_file_path ? cdr_file_path : "(none)");
    return OGS_OK;
}

/* ------------------------------------------------------------------ */
/* Notification queue (chf.notifications)                               */
/*                                                                      */
/* The billing side leaves a row, and every CHF picks it up once on its */
/* poll timer and turns it into a ChargingNotify to the live sessions   */
/* it holds that draw from the bucket.  See the comment on              */
/* chf_db_take_notifications() in db-handler.h for why it is a queue,   */
/* and why no CHF deletes a row it has read.                            */
/* ------------------------------------------------------------------ */

/* Copy a UTF-8 field into a fixed buffer, truncating rather than
 * overflowing; the values come from outside the CHF. */
static void notify_copy_utf8(const bson_t *document, const char *key,
        char *out, size_t size)
{
    bson_iter_t iter;
    uint32_t len = 0;
    const char *str = NULL;

    out[0] = '\0';
    if (!bson_iter_init_find(&iter, document, key) ||
            !BSON_ITER_HOLDS_UTF8(&iter))
        return;

    str = bson_iter_utf8(&iter, &len);
    if (!str)
        return;

    if (len >= size)
        len = (uint32_t)size - 1;
    memcpy(out, str, len);
    out[len] = '\0';
}

/*
 * Add this CHF to seenBy on every row matching filter, and stamp seenAt
 * on the rows no CHF had seen yet.  $min keeps the earliest stamp, so a
 * row is aged from when the first CHF saw it, whichever that was.
 */
static bool notify_mark_seen(bson_t *filter, const char *me, const char *op)
{
    bson_t *update = NULL;
    bson_error_t error;
    int64_t t0;
    bool ok;

    update = BCON_NEW(
            "$addToSet", "{", "seenBy", BCON_UTF8(me), "}",
            "$min", "{", "seenAt", BCON_DATE_TIME(now_ms()), "}");
    ogs_assert(update);

    t0 = ogs_get_monotonic_time();
    ok = mongoc_collection_update_many(
            notifications_coll, filter, update, NULL, NULL, &error);
    chf_db_metric("notifications", op, ok, t0);

    if (!ok)
        ogs_error("chf.notifications: cannot mark rows seen: %s",
                error.message);

    bson_destroy(update);
    return ok;
}

int chf_db_take_notifications(chf_notification_t *out, int max,
        const char *me)
{
    mongoc_cursor_t *cursor = NULL;
    bson_t *query = NULL, *opts = NULL, *filter = NULL;
    bson_t ids = BSON_INITIALIZER;
    bson_error_t error;
    const bson_t *document;
    int count = 0, nids = 0;
    int64_t t0;
    bool db_ok = true;

    ogs_assert(out);
    ogs_assert(max > 0);
    ogs_assert(me && *me);

    if (!notifications_coll) {
        bson_destroy(&ids);
        return 0;
    }

    query = BCON_NEW("seenBy", "{", "$ne", BCON_UTF8(me), "}");
    opts  = BCON_NEW("sort", "{", "_id", BCON_INT32(1), "}",
                     "limit", BCON_INT64((int64_t)max));

    t0 = ogs_get_monotonic_time();
#if MONGOC_CHECK_VERSION(1, 5, 0)
    cursor = mongoc_collection_find_with_opts(
            notifications_coll, query, opts, NULL);
#else
    cursor = mongoc_collection_find(notifications_coll,
            MONGOC_QUERY_NONE, 0, max, 0, query, NULL, NULL);
#endif

    while (nids < max && mongoc_cursor_next(cursor, &document)) {
        chf_notification_t *n = &out[count];
        bson_iter_t iter;
        char key[16];
        const char *k = key;
        char type[32];

        /*
         * Every row read is marked below, whatever is wrong with it, so
         * none is read twice: a malformed row costs one log line, not one
         * per poll, and cannot crowd good rows out of the batch.
         */
        if (!bson_iter_init_find(&iter, document, "_id"))
            continue;
        bson_uint32_to_string((uint32_t)nids, &k, key, sizeof(key));
        if (!bson_append_value(&ids, k, -1, bson_iter_value(&iter)))
            continue;
        nids++;

        memset(n, 0, sizeof(*n));
        if (BSON_ITER_HOLDS_OID(&iter))
            bson_oid_to_string(bson_iter_oid(&iter), n->id);

        notify_copy_utf8(document, "supi", n->supi, sizeof(n->supi));
        notify_copy_utf8(document, "bucketId",
                n->bucket_id, sizeof(n->bucket_id));
        notify_copy_utf8(document, "reason", n->reason, sizeof(n->reason));
        notify_copy_utf8(document, "type", type, sizeof(type));

        n->abort_charging = !strcmp(type, "ABORT_CHARGING");

        if (!n->supi[0]) {
            ogs_error("chf.notifications row with no supi — skipped");
            continue;
        }

        count++;
    }

    if (mongoc_cursor_error(cursor, &error)) {
        ogs_error("chf.notifications cursor failure: %s", error.message);
        db_ok = false;
    }

    chf_db_metric("notifications", "find", db_ok, t0);
    mongoc_cursor_destroy(cursor);
    bson_destroy(opts);
    bson_destroy(query);

    if (!db_ok) {
        bson_destroy(&ids);
        return OGS_ERROR;
    }
    if (nids == 0) {
        bson_destroy(&ids);
        return 0;
    }

    /*
     * Mark before anything is delivered.  If the mark fails, nothing is
     * delivered either, and the same rows come back on the next poll; if
     * it succeeds and the CHF then stops, the rows are not read again.
     * Either way a session hears a row at most once.
     */
    filter = BCON_NEW("_id", "{", "$in", BCON_ARRAY(&ids), "}");
    bson_destroy(&ids);
    ogs_assert(filter);

    if (!notify_mark_seen(filter, me, "mark")) {
        bson_destroy(filter);
        return OGS_ERROR;
    }
    bson_destroy(filter);

    return count;
}

int chf_db_skip_notifications(const char *me)
{
    bson_t *filter = NULL;
    bool ok;

    ogs_assert(me && *me);

    if (!notifications_coll)
        return OGS_OK;

    filter = BCON_NEW("seenBy", "{", "$ne", BCON_UTF8(me), "}");
    ogs_assert(filter);
    ok = notify_mark_seen(filter, me, "skip");
    bson_destroy(filter);

    return ok ? OGS_OK : OGS_ERROR;
}

int chf_db_prune_notifications(uint32_t keep_seconds)
{
    bson_t *query = NULL;
    bson_t reply;
    bson_error_t error;
    bson_iter_t it;
    int64_t t0;
    int deleted = 0;
    bool ok;

    if (!notifications_coll)
        return 0;

    /* Only rows some CHF has seen carry seenAt, so a row is never pruned
     * before at least one CHF has acted on it. */
    query = BCON_NEW("seenAt", "{", "$lt",
            BCON_DATE_TIME(now_ms() - (int64_t)keep_seconds * 1000), "}");
    ogs_assert(query);

    t0 = ogs_get_monotonic_time();
    ok = mongoc_collection_delete_many(
            notifications_coll, query, NULL, &reply, &error);
    chf_db_metric("notifications", "delete", ok, t0);

    if (!ok)
        ogs_error("chf.notifications prune failed: %s", error.message);
    else if (bson_iter_init_find(&it, &reply, "deletedCount"))
        deleted = (int)bson_iter_as_int64(&it);

    bson_destroy(&reply);
    bson_destroy(query);
    return ok ? deleted : OGS_ERROR;
}
