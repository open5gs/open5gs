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

#include "sbi-path.h"
#include "metrics.h"

static chf_context_t self;

int __chf_log_domain;

static OGS_POOL(chf_sess_pool, chf_sess_t);

/*
 * Per-SUPI session index entry (value type of self.supi_index_hash).
 *
 * Owns its own copy of the SUPI string, which doubles as the hash key —
 * ogs_hash stores the key pointer without copying, so the key must outlive
 * every entry that references it.  A sibling session freeing its own supi
 * must not invalidate the shared key, hence the dedicated copy here.
 *
 * The entry is created lazily on the first session for a SUPI and freed
 * when its last session is removed.
 */
typedef struct chf_supi_index_s {
    ogs_list_t  sess_list;   /* sessions for this SUPI, via chf_sess_t.supi_node */
    char       *supi;        /* owned copy; also the hash key */
} chf_supi_index_t;

static int context_initialized = 0;

static int chf_context_prepare(void);
static int chf_context_size_session_pool(void);

void chf_context_init(void)
{
    ogs_assert(context_initialized == 0);

    memset(&self, 0, sizeof(chf_context_t));

    ogs_log_install_domain(&__ogs_dbi_domain, "dbi", ogs_core()->log.level);
    ogs_log_install_domain(&__chf_log_domain, "chf", ogs_core()->log.level);

    ogs_pool_init(&chf_sess_pool, ogs_app()->pool.sess);

    ogs_list_init(&self.sess_list);

    self.charging_data_ref_hash = ogs_hash_make();
    ogs_assert(self.charging_data_ref_hash);

    self.supi_index_hash = ogs_hash_make();
    ogs_assert(self.supi_index_hash);

    /* Set defaults now so they are available even if parse_config is never
     * called (e.g. unit tests running without a YAML config file). */
    chf_context_prepare();

    context_initialized = 1;
}

void chf_context_final(void)
{
    ogs_assert(context_initialized == 1);

    chf_sess_remove_all();

    ogs_assert(self.charging_data_ref_hash);
    ogs_hash_destroy(self.charging_data_ref_hash);

    /* chf_sess_remove_all() frees every per-SUPI index entry as its last
     * session is removed, so the hash is empty here. */
    ogs_assert(self.supi_index_hash);
    ogs_hash_destroy(self.supi_index_hash);

    if (self.cdr.path) {
        ogs_free(self.cdr.path);
        self.cdr.path = NULL;
    }

    /* Free SUPI range strings */
    {
        int i;
        for (i = 0; i < self.num_of_supi_range; i++) {
            if (self.supi_range[i].start) {
                ogs_free(self.supi_range[i].start);
                self.supi_range[i].start = NULL;
            }
            if (self.supi_range[i].end) {
                ogs_free(self.supi_range[i].end);
                self.supi_range[i].end = NULL;
            }
        }
        self.num_of_supi_range = 0;
        self.num_of_slice = 0;
    }

    ogs_pool_final(&chf_sess_pool);

    context_initialized = 0;
}

chf_context_t *chf_self(void)
{
    return &self;
}

static int chf_context_prepare(void)
{
    int i;

    /* Free SUPI range strings allocated by any previous chf_context_parse_config()
     * call so that a config-reload does not leak them. On the first call from
     * chf_context_init() the context is already zeroed and the loop is a no-op. */
    for (i = 0; i < self.num_of_supi_range; i++) {
        if (self.supi_range[i].start) {
            ogs_free(self.supi_range[i].start);
            self.supi_range[i].start = NULL;
        }
        if (self.supi_range[i].end) {
            ogs_free(self.supi_range[i].end);
            self.supi_range[i].end = NULL;
        }
    }
    self.num_of_supi_range = 0;

    /* Slices carry no allocations, so a reset is enough. */
    self.num_of_slice = 0;

    /* Free CDR path string from any previous parse; the cdr.path may also be
     * freed here because chf_context_parse_config() re-allocates it from YAML
     * on every call.  On first call the pointer is already NULL (zeroed). */
    if (self.cdr.path) {
        ogs_free(self.cdr.path);
        self.cdr.path = NULL;
    }

    /* Default quota: 10 GiB / 24 h / TERMINATE
     * Overridden by chf.yaml charging.default_quota.{volume_mb,validity_seconds}
     * TS 32.291 §6.3.1.2 — GrantedUnit / FinalUnitIndication */
    self.default_quota.volume_bytes     = (uint64_t)10 * 1024 * 1024 * 1024;
    self.default_quota.validity_seconds = 86400;
    self.default_quota.final_unit_action = 0; /* TERMINATE */

    /* Grant slices: 50 MiB / 5 min, no early report below 5 MiB, and an
     * idle session gives its grant back after 60 s.
     * Overridden by charging.grant. */
    self.grant.max_volume_bytes      = (uint64_t)50 * 1024 * 1024;
    self.grant.max_validity_seconds  = 300;
    self.grant.min_report_bytes      = (uint64_t)5 * 1024 * 1024;
    self.grant.quota_holding_seconds = 60;

    self.reject_unprovisioned = false;
    self.well_known_dnn_cdr = true;

    /* ChargingNotify: on, looking for queued work every 5 s.
     * Overridden by charging.notify. */
    self.notify.enable       = true;
    self.notify.poll_seconds = 5;

    /* Pools: up to 10 SIMs share one.  Overridden by charging.pool. */
    self.pool.max_members = CHF_POOL_MAX_MEMBERS_DEFAULT;

    /* Session pool: 4 sessions per UE.  Overridden by
     * chf.session_pool_multiplier. */
    self.session_pool_multiplier = CHF_SESSION_POOL_MULTIPLIER_DEFAULT;

    /* CDR defaults: enabled, written to /var/log/open5gs/cdr as JSON, one
     * file per session.  JSON rather than BER because it is the format
     * someone can read without tooling, and a file per session because
     * that is what the CHF did before the ASN.1 encoder existed - an
     * upgrade changes nothing until charging.cdr.format says so.
     * TS 32.298 §5.1.5 — CHF CDR content */
    self.cdr.enable = true;
    self.cdr.formats = CHF_CDR_FMT_JSON;
    /* cdr.path already freed / NULL above; resolved to default in chf_cdr_init() */

    /* Thresholds that apply once aggregation is switched on.  1000 records
     * or 16 MB, whichever comes first. */
    self.cdr.json.aggregate   = false;
    self.cdr.json.max_records = 1000;
    self.cdr.json.max_bytes   = 16ULL * 1024 * 1024;
    self.cdr.asn1 = self.cdr.json;

    return OGS_OK;
}

static int chf_context_validation(void)
{
    return OGS_OK;
}

/*
 * Parse one `aggregation:` mapping into a filing policy.
 *
 * Keys the mapping leaves out keep whatever `f` already held, so a
 * per-format block need only state what differs from charging.cdr's own
 * aggregation defaults.
 */
static void chf_cdr_parse_filing(ogs_yaml_iter_t *parent,
        chf_cdr_filing_t *f)
{
    ogs_yaml_iter_t iter;

    ogs_assert(parent);
    ogs_assert(f);

    ogs_yaml_iter_recurse(parent, &iter);
    while (ogs_yaml_iter_next(&iter)) {
        const char *key = ogs_yaml_iter_key(&iter);
        const char *v;
        ogs_assert(key);

        v = ogs_yaml_iter_value(&iter);
        if (!v)
            continue;

        if (!strcmp(key, "enable")) {
            f->aggregate = (!strcmp(v, "true") || !strcmp(v, "yes") ||
                            !strcmp(v, "1"));
        } else if (!strcmp(key, "max_records")) {
            f->max_records = (uint32_t)atoll(v);
        } else if (!strcmp(key, "max_size_mb")) {
            long long mb = atoll(v);
            f->max_bytes = mb > 0 ? (uint64_t)mb * 1024 * 1024 : 0;
        } else {
            ogs_warn("Unknown charging.cdr aggregation key '%s' — ignored",
                     key);
        }
    }
}

int chf_context_parse_config(void)
{
    int rv;
    yaml_document_t *document = NULL;
    ogs_yaml_iter_t root_iter;

    document = ogs_app()->document;
    ogs_assert(document);

    rv = chf_context_prepare();
    if (rv != OGS_OK) return rv;

    ogs_yaml_iter_init(&root_iter, document);
    while (ogs_yaml_iter_next(&root_iter)) {
        const char *root_key = ogs_yaml_iter_key(&root_iter);
        ogs_assert(root_key);
        if (!strcmp(root_key, "chf")) {
            ogs_yaml_iter_t chf_iter;
            ogs_yaml_iter_recurse(&root_iter, &chf_iter);
            while (ogs_yaml_iter_next(&chf_iter)) {
                const char *chf_key = ogs_yaml_iter_key(&chf_iter);
                ogs_assert(chf_key);

                if (!strcmp(chf_key, "session_pool_multiplier")) {
                    const char *v = ogs_yaml_iter_value(&chf_iter);
                    unsigned long n = v ? strtoul(v, NULL, 10) : 0;
                    if (n < 1 || n > CHF_SESSION_POOL_MULTIPLIER_LIMIT)
                        ogs_warn("chf.session_pool_multiplier %s is outside "
                                 "1-%d — keeping %u", v ? v : "(empty)",
                                 CHF_SESSION_POOL_MULTIPLIER_LIMIT,
                                 self.session_pool_multiplier);
                    else
                        self.session_pool_multiplier = (uint32_t)n;
                } else if (!strcmp(chf_key, "charging")) {
                    ogs_yaml_iter_t charging_iter;
                    ogs_yaml_iter_recurse(&chf_iter, &charging_iter);
                    while (ogs_yaml_iter_next(&charging_iter)) {
                        const char *charging_key =
                            ogs_yaml_iter_key(&charging_iter);
                        ogs_assert(charging_key);

                        if (!strcmp(charging_key, "cdr")) {
                    ogs_yaml_iter_t cdr_iter;
                    ogs_yaml_iter_recurse(&charging_iter, &cdr_iter);
                    while (ogs_yaml_iter_next(&cdr_iter)) {
                        const char *cdr_key =
                            ogs_yaml_iter_key(&cdr_iter);
                        ogs_assert(cdr_key);

                        if (!strcmp(cdr_key, "enable")) {
                            const char *v = ogs_yaml_iter_value(&cdr_iter);
                            if (v)
                                self.cdr.enable =
                                    (!strcmp(v, "true") ||
                                     !strcmp(v, "yes") ||
                                     !strcmp(v, "1"));
                        } else if (!strcmp(cdr_key, "path")) {
                            const char *v = ogs_yaml_iter_value(&cdr_iter);
                            if (v) {
                                if (self.cdr.path) ogs_free(self.cdr.path);
                                self.cdr.path = ogs_strdup(v);
                            }
                        } else if (!strcmp(cdr_key, "format")) {
                            const char *v = ogs_yaml_iter_value(&cdr_iter);
                            if (v) {
                                if (!strcmp(v, "json"))
                                    self.cdr.formats = CHF_CDR_FMT_JSON;
                                else if (!strcmp(v, "asn1") ||
                                         !strcmp(v, "asn.1") ||
                                         !strcmp(v, "ber"))
                                    self.cdr.formats = CHF_CDR_FMT_ASN1;
                                else if (!strcmp(v, "both"))
                                    self.cdr.formats = CHF_CDR_FMT_JSON |
                                                       CHF_CDR_FMT_ASN1;
                                else if (!strcmp(v, "none"))
                                    self.cdr.formats = 0;
                                else
                                    ogs_error("Unknown charging.cdr.format "
                                              "'%s' — keeping json. Use "
                                              "json, asn1, both or none.", v);
                            }
                        } else if (!strcmp(cdr_key, "aggregation")) {
                            /* The default for both encodings. Parsed before
                             * the per-format blocks below only if it appears
                             * first in the file, so each format also starts
                             * from this value when its own block omits a
                             * key - see chf_cdr_parse_filing(). */
                            chf_cdr_parse_filing(&cdr_iter, &self.cdr.json);
                            self.cdr.asn1 = self.cdr.json;
                        } else if (!strcmp(cdr_key, "json") ||
                                   !strcmp(cdr_key, "asn1")) {
                            chf_cdr_filing_t *f =
                                !strcmp(cdr_key, "json") ? &self.cdr.json
                                                         : &self.cdr.asn1;
                            ogs_yaml_iter_t fmt_iter;
                            ogs_yaml_iter_recurse(&cdr_iter, &fmt_iter);
                            while (ogs_yaml_iter_next(&fmt_iter)) {
                                const char *fk = ogs_yaml_iter_key(&fmt_iter);
                                ogs_assert(fk);
                                if (!strcmp(fk, "aggregation"))
                                    chf_cdr_parse_filing(&fmt_iter, f);
                            }
                        }
                    }
                } else if (!strcmp(charging_key, "supi_ranges")) {
                    /* TS 29.510 §6.1.6.2.9 — SupiRange
                     * Parsed from chf.charging.supi_ranges in chf.yaml:
                     *   charging:
                     *     supi_ranges:
                     *       - start: "imsi-001010000000000"
                     *         end:   "imsi-001010000049999"
                     */
                    ogs_yaml_iter_t range_array, range_iter;
                    ogs_yaml_iter_recurse(&charging_iter, &range_array);
                    do {
                        if (ogs_yaml_iter_type(&range_array) ==
                                YAML_MAPPING_NODE) {
                            memcpy(&range_iter, &range_array,
                                   sizeof(ogs_yaml_iter_t));
                        } else if (ogs_yaml_iter_type(&range_array) ==
                                YAML_SEQUENCE_NODE) {
                            if (!ogs_yaml_iter_next(&range_array))
                                break;
                            ogs_yaml_iter_recurse(&range_array, &range_iter);
                        } else if (ogs_yaml_iter_type(&range_array) ==
                                YAML_SCALAR_NODE) {
                            break;
                        } else
                            ogs_assert_if_reached();

                        if (self.num_of_supi_range >=
                                OGS_MAX_NUM_OF_SUPI_RANGE) {
                            ogs_warn("Max SUPI ranges (%d) exceeded — "
                                     "ignoring remaining",
                                     OGS_MAX_NUM_OF_SUPI_RANGE);
                            break;
                        }

                        while (ogs_yaml_iter_next(&range_iter)) {
                            const char *rk =
                                ogs_yaml_iter_key(&range_iter);
                            ogs_assert(rk);
                            if (!strcmp(rk, "start")) {
                                const char *v =
                                    ogs_yaml_iter_value(&range_iter);
                                if (v)
                                    self.supi_range[
                                        self.num_of_supi_range].start =
                                            ogs_strdup(v);
                            } else if (!strcmp(rk, "end")) {
                                const char *v =
                                    ogs_yaml_iter_value(&range_iter);
                                if (v)
                                    self.supi_range[
                                        self.num_of_supi_range].end =
                                            ogs_strdup(v);
                            }
                        }
                        /* Only count if at least start was given */
                        if (self.supi_range[self.num_of_supi_range].start)
                            self.num_of_supi_range++;

                    } while (ogs_yaml_iter_type(&range_array) ==
                             YAML_SEQUENCE_NODE);

                } else if (!strcmp(charging_key, "slices")) {
                    /* NFProfile.allowedNssais — the slices this CHF
                     * instance serves (TS 33.518 §4.2.2.2.1).  Parsed from
                     * chf.charging.slices in chf.yaml:
                     *   charging:
                     *     slices:
                     *       - sst: 1
                     *         sd: 000001
                     * Empty means "every slice", which is the default.
                     */
                    ogs_yaml_iter_t slice_array, slice_iter;
                    ogs_yaml_iter_recurse(&charging_iter, &slice_array);
                    do {
                        ogs_s_nssai_t *s_nssai = NULL;

                        if (ogs_yaml_iter_type(&slice_array) ==
                                YAML_MAPPING_NODE) {
                            memcpy(&slice_iter, &slice_array,
                                   sizeof(ogs_yaml_iter_t));
                        } else if (ogs_yaml_iter_type(&slice_array) ==
                                YAML_SEQUENCE_NODE) {
                            if (!ogs_yaml_iter_next(&slice_array))
                                break;
                            ogs_yaml_iter_recurse(&slice_array, &slice_iter);
                        } else if (ogs_yaml_iter_type(&slice_array) ==
                                YAML_SCALAR_NODE) {
                            break;
                        } else
                            ogs_assert_if_reached();

                        if (self.num_of_slice >= OGS_MAX_NUM_OF_SLICE) {
                            ogs_warn("Max slices (%d) exceeded — "
                                     "ignoring remaining",
                                     OGS_MAX_NUM_OF_SLICE);
                            break;
                        }

                        s_nssai = &self.slice[self.num_of_slice];
                        s_nssai->sst = 0;
                        s_nssai->sd.v = OGS_S_NSSAI_NO_SD_VALUE;

                        while (ogs_yaml_iter_next(&slice_iter)) {
                            const char *sk = ogs_yaml_iter_key(&slice_iter);
                            const char *v;
                            ogs_assert(sk);

                            v = ogs_yaml_iter_value(&slice_iter);
                            if (!v)
                                continue;

                            if (!strcmp(sk, "sst")) {
                                s_nssai->sst = atoi(v);
                            } else if (!strcmp(sk, "sd")) {
                                s_nssai->sd = ogs_s_nssai_sd_from_string(v);
                            } else {
                                ogs_warn("Unknown charging.slices key "
                                         "'%s' — ignored", sk);
                            }
                        }

                        /* An SST of 0 is not a slice; TS 23.003 numbers
                         * the standardised SSTs from 1. */
                        if (s_nssai->sst)
                            self.num_of_slice++;
                        else
                            ogs_warn("charging.slices entry has no sst — "
                                     "ignored");

                    } while (ogs_yaml_iter_type(&slice_array) ==
                             YAML_SEQUENCE_NODE);

                } else if (!strcmp(charging_key, "grant")) {
                    ogs_yaml_iter_t grant_iter;
                    ogs_yaml_iter_recurse(&charging_iter, &grant_iter);
                    while (ogs_yaml_iter_next(&grant_iter)) {
                        const char *grant_key =
                            ogs_yaml_iter_key(&grant_iter);
                        const char *v = ogs_yaml_iter_value(&grant_iter);
                        ogs_assert(grant_key);
                        if (!v) continue;

                        if (!strcmp(grant_key, "max_volume_mb"))
                            self.grant.max_volume_bytes =
                                (uint64_t)strtoull(v, NULL, 10) * 1024 * 1024;
                        else if (!strcmp(grant_key, "max_validity_seconds"))
                            self.grant.max_validity_seconds =
                                (uint32_t)strtoul(v, NULL, 10);
                        else if (!strcmp(grant_key, "min_report_mb"))
                            self.grant.min_report_bytes =
                                (uint64_t)strtoull(v, NULL, 10) * 1024 * 1024;
                        else if (!strcmp(grant_key, "quota_holding_seconds"))
                            self.grant.quota_holding_seconds =
                                (uint32_t)strtoul(v, NULL, 10);
                        else
                            ogs_warn("unknown key `%s`", grant_key);
                    }

                } else if (!strcmp(charging_key, "notify")) {
                    ogs_yaml_iter_t notify_iter;
                    ogs_yaml_iter_recurse(&charging_iter, &notify_iter);
                    while (ogs_yaml_iter_next(&notify_iter)) {
                        const char *notify_key =
                            ogs_yaml_iter_key(&notify_iter);
                        const char *v = ogs_yaml_iter_value(&notify_iter);
                        ogs_assert(notify_key);
                        if (!v) continue;

                        if (!strcmp(notify_key, "enable"))
                            self.notify.enable =
                                (!strcmp(v, "true") || !strcmp(v, "yes") ||
                                 !strcmp(v, "1"));
                        else if (!strcmp(notify_key, "poll_seconds")) {
                            uint32_t secs = (uint32_t)strtoul(v, NULL, 10);
                            /* A zero would spin the timer; treat it as
                             * "do not poll" and say so. */
                            if (secs == 0) {
                                ogs_warn("charging.notify.poll_seconds 0 "
                                         "— ChargingNotify disabled");
                                self.notify.enable = false;
                            } else {
                                self.notify.poll_seconds = secs;
                            }
                        } else
                            ogs_warn("unknown key `%s`", notify_key);
                    }

                } else if (!strcmp(charging_key, "pool")) {
                    ogs_yaml_iter_t pool_iter;
                    ogs_yaml_iter_recurse(&charging_iter, &pool_iter);
                    while (ogs_yaml_iter_next(&pool_iter)) {
                        const char *pool_key = ogs_yaml_iter_key(&pool_iter);
                        const char *v = ogs_yaml_iter_value(&pool_iter);
                        ogs_assert(pool_key);
                        if (!v) continue;
                        if (!strcmp(pool_key, "max_members")) {
                            unsigned long n = strtoul(v, NULL, 10);
                            if (n < 1 || n > CHF_POOL_MAX_MEMBERS_LIMIT) {
                                ogs_warn("charging.pool.max_members %s is "
                                         "outside 1-%d — keeping %u", v,
                                         CHF_POOL_MAX_MEMBERS_LIMIT,
                                         self.pool.max_members);
                            } else {
                                self.pool.max_members = (uint32_t)n;
                            }
                        } else
                            ogs_warn("unknown key `%s`", pool_key);
                    }

                } else if (!strcmp(charging_key, "well_known_dnn")) {
                    ogs_yaml_iter_t wk_iter;
                    ogs_yaml_iter_recurse(&charging_iter, &wk_iter);
                    while (ogs_yaml_iter_next(&wk_iter)) {
                        const char *wk_key = ogs_yaml_iter_key(&wk_iter);
                        ogs_assert(wk_key);
                        if (!strcmp(wk_key, "cdr"))
                            self.well_known_dnn_cdr =
                                ogs_yaml_iter_bool(&wk_iter);
                        else
                            ogs_warn("unknown key `%s`", wk_key);
                    }

                } else if (!strcmp(charging_key, "unprovisioned")) {
                    const char *v = ogs_yaml_iter_value(&charging_iter);
                    if (v && !strcmp(v, "reject"))
                        self.reject_unprovisioned = true;
                    else if (v && strcmp(v, "default"))
                        ogs_warn("charging.unprovisioned: unknown value `%s` "
                                 "— using `default`", v);

                } else if (!strcmp(charging_key, "default_quota")) {
                            ogs_yaml_iter_t quota_iter;
                            ogs_yaml_iter_recurse(&charging_iter, &quota_iter);
                            while (ogs_yaml_iter_next(&quota_iter)) {
                                const char *quota_key =
                                    ogs_yaml_iter_key(&quota_iter);
                                ogs_assert(quota_key);

                                if (!strcmp(quota_key, "volume_mb")) {
                                    const char *v =
                                        ogs_yaml_iter_value(&quota_iter);
                                    if (v)
                                        self.default_quota.volume_bytes =
                                            (uint64_t)atoi(v) * 1024 * 1024;
                                } else if (!strcmp(quota_key,
                                        "validity_seconds")) {
                                    const char *v =
                                        ogs_yaml_iter_value(&quota_iter);
                                    if (v)
                                        self.default_quota.validity_seconds =
                                            (uint32_t)atoi(v);
                                } else if (!strcmp(quota_key,
                                        "final_unit_action")) {
                                    const char *v =
                                        ogs_yaml_iter_value(&quota_iter);
                                    if (v) {
                                        if (!strcmp(v, "REDIRECT"))
                                            self.default_quota
                                                .final_unit_action = 1;
                                        else if (!strcmp(v,
                                                "RESTRICT_ACCESS"))
                                            self.default_quota
                                                .final_unit_action = 2;
                                        else /* TERMINATE (default) */
                                            self.default_quota
                                                .final_unit_action = 0;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    rv = chf_context_validation();
    if (rv != OGS_OK) return rv;

    rv = chf_context_size_session_pool();
    if (rv != OGS_OK) return rv;

    return OGS_OK;
}

/*
 * Size the session pool from the config.  chf_context_init() creates it at
 * the default before chf.yaml is read; this re-creates it, still empty, if
 * the multiplier asks for another size.  Nothing can be in it yet: sessions
 * only arrive once the SBI server is open, after the config is parsed.
 */
static int chf_context_size_session_pool(void)
{
    uint64_t size = (uint64_t)ogs_global_conf()->max.ue *
                    self.session_pool_multiplier;

    if (size < 1 || size > INT32_MAX) {
        ogs_error("CHF session pool: global.max.ue %llu x "
                  "chf.session_pool_multiplier %u is out of range",
                  (unsigned long long)ogs_global_conf()->max.ue,
                  self.session_pool_multiplier);
        return OGS_ERROR;
    }

    if ((uint64_t)ogs_pool_size(&chf_sess_pool) != size) {
        if (ogs_pool_avail(&chf_sess_pool) != ogs_pool_size(&chf_sess_pool)) {
            ogs_error("CHF session pool: cannot resize with sessions in it");
            return OGS_ERROR;
        }
        ogs_pool_final(&chf_sess_pool);
        ogs_pool_init(&chf_sess_pool, (int)size);
    }

    ogs_info("CHF session pool: %d sessions (global.max.ue %llu x "
             "chf.session_pool_multiplier %u)", ogs_pool_size(&chf_sess_pool),
             (unsigned long long)ogs_global_conf()->max.ue,
             self.session_pool_multiplier);
    return OGS_OK;
}

/* ------------------------------------------------------------------ */
/* Session management                                                   */
/* ------------------------------------------------------------------ */

chf_sess_t *chf_sess_add(const char *supi, const char *charging_data_ref)
{
    chf_sess_t *sess = NULL;

    ogs_assert(supi);
    ogs_assert(charging_data_ref);

    ogs_pool_id_calloc(&chf_sess_pool, &sess);
    if (!sess) {
        ogs_error("Maximum number of sessions reached");
        return NULL;
    }

    sess->sbi.type = OGS_SBI_OBJ_SESS_TYPE;

    sess->supi = ogs_strdup(supi);
    if (!sess->supi) {
        ogs_error("[%s] ogs_strdup() OOM for SUPI", supi);
        ogs_pool_id_free(&chf_sess_pool, sess);
        return NULL;
    }

    sess->charging_data_ref = ogs_strdup(charging_data_ref);
    if (!sess->charging_data_ref) {
        ogs_error("[%s] ogs_strdup() OOM for charging_data_ref", supi);
        ogs_free(sess->supi);
        ogs_pool_id_free(&chf_sess_pool, sess);
        return NULL;
    }

    ogs_hash_set(self.charging_data_ref_hash,
            sess->charging_data_ref,
            strlen(sess->charging_data_ref),
            sess);

    ogs_list_add(&self.sess_list, sess);

    /* Add to the per-SUPI index, creating the entry lazily. */
    {
        chf_supi_index_t *idx = ogs_hash_get(self.supi_index_hash,
                sess->supi, strlen(sess->supi));
        if (!idx) {
            idx = ogs_calloc(1, sizeof(*idx));
            ogs_assert(idx);
            idx->supi = ogs_strdup(sess->supi);
            ogs_assert(idx->supi);
            ogs_list_init(&idx->sess_list);
            ogs_hash_set(self.supi_index_hash,
                    idx->supi, strlen(idx->supi), idx);
        }
        ogs_list_add(&idx->sess_list, &sess->supi_node);
    }

    chf_metrics_inst_global_inc(CHF_METR_GAUGE_SESSIONS_ACTIVE);

    ogs_info("[%s] CHF session added [ref:%s]",
            sess->supi, sess->charging_data_ref);

    return sess;
}

void chf_sess_remove(chf_sess_t *sess)
{
    ogs_assert(sess);

    ogs_list_remove(&self.sess_list, sess);

    ogs_sbi_object_free(&sess->sbi);

    /* Remove from the per-SUPI index before freeing sess->supi (the lookup
     * key).  Free the index entry once its last session is gone. */
    if (sess->supi) {
        chf_supi_index_t *idx = ogs_hash_get(self.supi_index_hash,
                sess->supi, strlen(sess->supi));
        if (idx) {
            ogs_list_remove(&idx->sess_list, &sess->supi_node);
            if (ogs_list_empty(&idx->sess_list)) {
                ogs_hash_set(self.supi_index_hash,
                        idx->supi, strlen(idx->supi), NULL);
                ogs_free(idx->supi);
                ogs_free(idx);
            }
        }
    }

    if (sess->charging_data_ref) {
        ogs_hash_set(self.charging_data_ref_hash,
                sess->charging_data_ref,
                strlen(sess->charging_data_ref),
                NULL);
        ogs_free(sess->charging_data_ref);
    }

    if (sess->supi)       ogs_free(sess->supi);
    if (sess->gpsi)       ogs_free(sess->gpsi);
    if (sess->dnn)        ogs_free(sess->dnn);
    if (sess->smf_id)     ogs_free(sess->smf_id);
    if (sess->smf_label)  ogs_free(sess->smf_label);
    if (sess->notify_uri) ogs_free(sess->notify_uri);

    ogs_pool_id_free(&chf_sess_pool, sess);

    chf_metrics_inst_global_dec(CHF_METR_GAUGE_SESSIONS_ACTIVE);
}

void chf_sess_remove_all(void)
{
    chf_sess_t *sess = NULL, *next = NULL;

    ogs_list_for_each_safe(&self.sess_list, next, sess)
        chf_sess_remove(sess);
}

chf_sess_t *chf_sess_find_by_id(ogs_pool_id_t id)
{
    return ogs_pool_find_by_id(&chf_sess_pool, id);
}

chf_sess_t *chf_sess_find_by_charging_data_ref(const char *charging_data_ref)
{
    ogs_assert(charging_data_ref);
    return ogs_hash_get(self.charging_data_ref_hash,
            charging_data_ref, strlen(charging_data_ref));
}

/*
 * chf_apply_quota — initialise session quota fields from a subscriber profile.
 *
 * The allowance is a single combined figure: the CHF response carries it as
 * grantedUnit.totalVolume so the UPF enforces one UL+DL limit — the subscriber
 * can never consume more than volume_bytes in total regardless of the
 * direction mix.
 *
 * Also seeds the billing period fields:
 *   period_start      — now (time of Create / renewal)
 *   period_length_sec — copy of validity_seconds for expiry detection
 *   auto_renew        — whether to reset usage and re-grant on expiry
 *
 * Resets final_unit to false.
 * Moved from nchf-handler.c to context.c so unit tests can call it directly.
 * TS 32.291 §6.3.1 — quota grant on Create.
 */
/* ------------------------------------------------------------------ */
/* FinalUnitAction naming (TS 32.291 §7.3)                             */
/*                                                                      */
/* One place to map the enum in each direction, shared by the           */
/* ChargingDataResponse builder, the MongoDB profile reader and the     */
/* metric label sites.                                                  */
/* ------------------------------------------------------------------ */
const char *chf_fua_to_string(int fua)
{
    switch (fua) {
    case 1:  return "REDIRECT";
    case 2:  return "RESTRICT_ACCESS";
    default: return "TERMINATE";
    }
}

int chf_fua_from_string(const char *fua)
{
    if (!fua)                             return 0;
    if (!strcmp(fua, "REDIRECT"))         return 1;
    if (!strcmp(fua, "RESTRICT_ACCESS"))  return 2;
    return 0; /* TERMINATE (default) */
}

const char *chf_fua_metric_label(int fua)
{
    switch (fua) {
    case 1:  return "redirect";
    case 2:  return "restrict";
    default: return "terminate";
    }
}

/* ------------------------------------------------------------------ */
/* Bucket selection (see chf_bucket_t in context.h)                     */
/* ------------------------------------------------------------------ */

/* One dimension: an empty scope value is open, otherwise the session
 * must carry that exact value. */
static bool dimension_matches(const char *scope_value, const char *key_value)
{
    if (!scope_value || !*scope_value)
        return true;                    /* bucket is open here */
    if (!key_value || !*key_value)
        return false;                   /* request did not say */
    return strcmp(scope_value, key_value) == 0;
}

bool chf_bucket_scope_matches(const chf_bucket_scope_t *scope,
        const chf_bucket_key_t *key)
{
    ogs_assert(scope);
    ogs_assert(key);

    if (scope->s_nssai.sst) {
        if (!key->s_nssai.sst)
            return false;               /* request carried no slice */
        if (scope->s_nssai.sst != key->s_nssai.sst)
            return false;
        /*
         * A bucket that names an SD means that one slice instance; a
         * bucket with only an SST covers every instance of that SST.
         */
        if (scope->s_nssai.sd.v != OGS_S_NSSAI_NO_SD_VALUE &&
            scope->s_nssai.sd.v != key->s_nssai.sd.v)
            return false;
    }

    if (!dimension_matches(scope->dnn, key->dnn))
        return false;
    if (!dimension_matches(scope->hplmn, key->hplmn))
        return false;
    if (!dimension_matches(scope->roaming, key->roaming))
        return false;

    return true;
}

int chf_bucket_select(const chf_bucket_t *buckets, int num,
        const chf_bucket_key_t *key)
{
    int i, best = -1;

    ogs_assert(key);
    if (!buckets || num <= 0)
        return -1;

    for (i = 0; i < num; i++) {
        if (!chf_bucket_scope_matches(&buckets[i].scope, key))
            continue;
        /* Strictly greater, so equal priorities keep the earlier (older)
         * bucket and the choice is stable - except that a SIM's own bucket
         * is preferred to its pool: an add-on bought for one phone should
         * be spent before the family's allowance, unless the operator
         * ranks them otherwise. */
        if (best < 0 || buckets[i].priority > buckets[best].priority ||
            (buckets[i].priority == buckets[best].priority &&
             buckets[best].shared && !buckets[i].shared))
            best = i;
    }

    return best;
}

void chf_apply_quota(chf_sess_t *sess, const chf_quota_t *quota)
{
    ogs_assert(sess);
    ogs_assert(quota);

    sess->granted_quota_total = quota->volume_bytes;
    sess->granted_time_sec  = quota->validity_seconds;
    sess->final_unit_action = quota->final_unit_action;
    sess->final_unit        = false;

    /* Billing period tracking */
    sess->period_start      = time(NULL);
    sess->period_length_sec = quota->validity_seconds;
    sess->auto_renew        = quota->auto_renew;

    /* Total volume granted (kilobytes; clamped so an absurd provisioned
     * quota cannot overflow the int metric API and abort the NF). */
    chf_metrics_inst_global_add(CHF_METR_CTR_QUOTA_GRANTED_KB,
            chf_metrics_clamp_u64(quota->volume_bytes / 1024));

    ogs_debug("[%s] Quota applied: total=%llu "
              "validity=%us auto_renew=%s action=%d",
            sess->supi ? sess->supi : "?",
            (unsigned long long)sess->granted_quota_total,
            sess->granted_time_sec,
            sess->auto_renew ? "yes" : "no",
            sess->final_unit_action);
}

/*
 * chf_check_quota_exhausted — returns true when accumulated usage meets or
 * exceeds the total granted volume.
 *
 * Sets sess->final_unit = true on first exhaustion detection.
 * Moved from nchf-handler.c to context.c for unit-test accessibility.
 * TS 32.291 §6.3.2.2 — FinalUnitIndication triggered on grant exhaustion.
 */
bool chf_check_quota_exhausted(chf_sess_t *sess)
{
    uint64_t total_used, total_granted;

    ogs_assert(sess);

    total_used    = sess->ul_octets + sess->dl_octets;
    total_granted = sess->granted_quota_total;

    if (total_used >= total_granted) {
        if (!sess->final_unit) {
            sess->final_unit = true;
            ogs_warn("[%s] Quota EXHAUSTED [ref:%s used:%llu granted:%llu]",
                    sess->supi, sess->charging_data_ref,
                    (unsigned long long)total_used,
                    (unsigned long long)total_granted);
            chf_metrics_inst_labeled_inc(CHF_METR_LBL_QUOTA_EXHAUSTED,
                    "per_session", sess->dnn, NULL);
            chf_metrics_inst_labeled_inc(CHF_METR_LBL_QUOTA_FINAL_UNIT,
                    chf_fua_metric_label(sess->final_unit_action), NULL, NULL);
        }
        return true;
    }
    return false;
}

bool chf_quota_revive(chf_sess_t *sess,
        uint64_t cycle_consumed, uint64_t cycle_allowance)
{
    uint64_t total_used;

    ogs_assert(sess);

    if (!sess->final_unit)
        return false;

    /* Both views have to agree there is room: the session's own ceiling,
     * which is what the response grant is measured against, and the cycle
     * counter, which spans every session of the cycle (Case E). */
    total_used = sess->ul_octets + sess->dl_octets;
    if (total_used >= sess->granted_quota_total)
        return false;
    if (cycle_consumed >= cycle_allowance)
        return false;

    sess->final_unit = false;
    ogs_info("[%s] Recharged after exhaustion - final unit cleared "
             "[ref:%s used:%llu granted:%llu cycle:%llu of %llu]",
            sess->supi, sess->charging_data_ref,
            (unsigned long long)total_used,
            (unsigned long long)sess->granted_quota_total,
            (unsigned long long)cycle_consumed,
            (unsigned long long)cycle_allowance);
    return true;
}

/*
 * chf_check_period_expired — returns true when the session's current billing
 * period has elapsed.
 *
 * Returns false immediately when period_length_sec == 0 (no expiry configured).
 *
 * Called on every CHF Update so the handler can decide to renew or exhaust.
 * TS 32.291 §6.3.2.2 — TIME_LIMIT trigger causes Update from UPF/SMF.
 */
bool chf_check_period_expired(chf_sess_t *sess)
{
    ogs_assert(sess);

    if (sess->period_length_sec == 0)
        return false; /* no expiry configured */

    /* Use >= so the expiry is detected at the exact boundary second.
     * The UPF fires TIME_LIMIT when validityTime reaches 0 (i.e. exactly at
     * period_start + period_length_sec).  Using strict > would miss that
     * instant and force a one-second slip before expiry is detected. */
    return (time(NULL) >= sess->period_start + (time_t)sess->period_length_sec);
}

/*
 * chf_supi_aggregate_used — total bytes consumed across all active sessions
 * for the given SUPI.
 *
 * Uses the per-SUPI index (self.supi_index_hash), so the cost is
 * O(sessions-for-this-SUPI) — typically 1–8 — rather than O(all-sessions).
 * This matters because the function is called on every Create and every
 * Update; a whole-list scan made that O(N) per interaction (≈O(N²) overall)
 * under high session density.
 *
 * Called BEFORE chf_sess_add() on Create so the new session is not yet in
 * the index and is therefore excluded from the sum automatically.
 *
 * Used on Update to detect cross-session exhaustion (TS 32.291 §5.2 Case E).
 *
 * Lives in context.c (not nchf-handler.c) so that unit tests can call it
 * directly without linking the full HTTP handler translation unit.
 */
uint64_t chf_supi_aggregate_used(const char *supi)
{
    chf_supi_index_t *idx;
    chf_sess_t *s;
    uint64_t total = 0;

    ogs_assert(supi);

    idx = ogs_hash_get(self.supi_index_hash, supi, strlen(supi));
    if (!idx)
        return 0;

    ogs_list_for_each_entry(&idx->sess_list, s, supi_node) {
        /* IMS or emergency traffic must not use up the data allowance. */
        if (s->quota_exempt)
            continue;
        total += s->ul_octets + s->dl_octets;
    }

    return total;
}

int chf_sess_foreach_by_bucket(const char *supi, const char *bucket_id,
        void (*cb)(chf_sess_t *sess, void *data), void *data)
{
    chf_supi_index_t *idx;
    chf_sess_t *s = NULL, *next = NULL;
    bool any_bucket;
    int n = 0;

    ogs_assert(supi);
    ogs_assert(cb);

    any_bucket = (!bucket_id || !*bucket_id);

    idx = ogs_hash_get(self.supi_index_hash, supi, strlen(supi));
    if (!idx)
        return 0;

    ogs_list_for_each_entry_safe(&idx->sess_list, next, s, supi_node) {
        /* A top-up says nothing to a session with no quota, and an
         * ABORT_CHARGING for a data plan must not end an emergency call. */
        if (s->quota_exempt)
            continue;
        if (!any_bucket && strcmp(chf_sess_bucket(s), bucket_id))
            continue;
        cb(s, data);
        n++;
    }

    return n;
}
