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
 * CHF CDR writer.
 *
 * A closing session becomes one TS 32.298 ChargingRecord (cdr-record.c),
 * which is then written in whichever encodings charging.cdr.format asks
 * for: BER through asn1c, JSON through cdr-json.c, or both.  The record
 * is built once, so the two files always say the same thing.
 *
 * Each encoding is filed under its own policy, either one file per
 * session or many records to a file:
 *
 *   individual   {base}/YYYY/MM/DD/cdr-{chargingDataRef}.{json,ber}
 *   aggregated   {base}/YYYY/MM/DD/cdr-{YYYYMMDDhhmmss}-{seq}.{json,ber}
 *
 * An aggregated file is appended to until a threshold would be crossed,
 * then the next record starts a new one.  Both encodings concatenate
 * cleanly: BER records are self-delimiting, and the JSON is one record
 * per line (JSON Lines), so neither needs a framing layer of its own.
 *
 * Every record is flushed as it is written.  CDRs are billing data, and
 * an aggregated file that loses its tail to a crash loses real records,
 * not just the last one.
 */

#include <sys/stat.h>
#include <time.h>
#include <errno.h>

#include "cdr-path.h"
#include "cdr-record.h"
#include "cdr-json.h"
#include "metrics.h"

#define CHF_CDR_DEFAULT_BASE_PATH   "/var/log/open5gs/cdr"

/* Maximum number of bytes (including NUL) for any CDR-related path.
 * Typical paths are well under 256 bytes.  1024 gives comfortable headroom
 * even for deeply-nested or customised base directories. */
#define CHF_CDR_PATH_MAX            1024

/* Static CDR base directory (resolved at init from context config) */
static char *cdr_base_path = NULL;

/*
 * localRecordSequenceNumber (TS 32.298): unique within one node, which a
 * counter that advances per record satisfies.  It restarts at 1 on
 * restart - the field is scoped to the node and its recordOpeningTime,
 * so a restart is a new scope.
 */
static uint32_t cdr_local_seq = 0;

/* ------------------------------------------------------------------ */
/* One output stream per encoding                                       */
/* ------------------------------------------------------------------ */

typedef struct cdr_sink_s {
    const char *label;      /* for logs: "json" or "asn1"              */
    const char *ext;        /* file extension                          */
    int         format_bit; /* CHF_CDR_FMT_*                           */

    /* Open aggregate file, when this sink aggregates. NULL when closed. */
    FILE       *fp;
    char       *path;
    uint32_t    records;    /* records in the open file                */
    uint64_t    bytes;      /* bytes in the open file                  */
    char        day[48];    /* "YYYY/MM/DD" the open file belongs to   */
    uint32_t    roll_seq;   /* distinguishes files opened in one second */
} cdr_sink_t;

static cdr_sink_t sink_json = {
    .label = "json", .ext = "json", .format_bit = CHF_CDR_FMT_JSON,
    .fp = NULL, .path = NULL, .records = 0, .bytes = 0,
    .day = "", .roll_seq = 0,
};
static cdr_sink_t sink_asn1 = {
    .label = "asn1", .ext = "ber", .format_bit = CHF_CDR_FMT_ASN1,
    .fp = NULL, .path = NULL, .records = 0, .bytes = 0,
    .day = "", .roll_seq = 0,
};

static const chf_cdr_filing_t *sink_policy(const cdr_sink_t *sink)
{
    return sink == &sink_json ? &chf_self()->cdr.json
                              : &chf_self()->cdr.asn1;
}

/* ------------------------------------------------------------------ */
/* Helper: create a directory path recursively (like mkdir -p)         */
/* ------------------------------------------------------------------ */
static int mkdir_p(const char *path, mode_t mode)
{
    char tmp[CHF_CDR_PATH_MAX];
    char *p = NULL;
    size_t len;
    int rc;

    rc = snprintf(tmp, sizeof(tmp), "%s", path);
    if (rc < 0 || rc >= (int)sizeof(tmp)) {
        ogs_error("CDR mkdir_p: path too long (max %d chars): %s",
                  (int)sizeof(tmp) - 1, path);
        return OGS_ERROR;
    }
    len = (size_t)rc;
    if (len > 0 && tmp[len - 1] == '/')
        tmp[len - 1] = '\0';

    for (p = tmp + 1; *p; p++) {
        if (*p == '/') {
            *p = '\0';
            if (mkdir(tmp, mode) != 0 && errno != EEXIST)
                return OGS_ERROR;
            *p = '/';
        }
    }
    if (mkdir(tmp, mode) != 0 && errno != EEXIST)
        return OGS_ERROR;
    return OGS_OK;
}

/* Today's directory, created if needed. Returns OGS_OK and fills dir. */
static int today_dir(char *dir, size_t dirsz, char *day, size_t daysz,
        time_t now)
{
    struct tm tm_today;

    gmtime_r(&now, &tm_today);

    if ((size_t)snprintf(day, daysz, "%04d/%02d/%02d",
                tm_today.tm_year + 1900, tm_today.tm_mon + 1,
                tm_today.tm_mday) >= daysz)
        return OGS_ERROR;

    if ((size_t)snprintf(dir, dirsz, "%s/%s", cdr_base_path, day) >= dirsz) {
        ogs_error("CDR directory path too long (max %d chars) — "
                  "base='%s' date='%s'", (int)dirsz - 1, cdr_base_path, day);
        return OGS_ERROR;
    }

    if (mkdir_p(dir, 0755) != OGS_OK) {
        ogs_error("Cannot create CDR directory: %s — %s",
                dir, strerror(errno));
        return OGS_ERROR;
    }

    return OGS_OK;
}

/* ------------------------------------------------------------------ */
/* Aggregated filing                                                    */
/* ------------------------------------------------------------------ */

static void sink_close(cdr_sink_t *sink)
{
    if (!sink->fp)
        return;

    fclose(sink->fp);
    ogs_info("CDR %s file closed: %s (%u records, %llu bytes)",
            sink->label, sink->path ? sink->path : "(unknown)",
            sink->records, (unsigned long long)sink->bytes);

    sink->fp = NULL;
    if (sink->path) {
        ogs_free(sink->path);
        sink->path = NULL;
    }
    sink->records = 0;
    sink->bytes = 0;
}

static int sink_open(cdr_sink_t *sink, time_t now)
{
    char dir[CHF_CDR_PATH_MAX];
    char day[48];
    char path[CHF_CDR_PATH_MAX + 64];
    struct tm tm_now;

    if (today_dir(dir, sizeof(dir), day, sizeof(day), now) != OGS_OK)
        return OGS_ERROR;

    gmtime_r(&now, &tm_now);

    /* The open time names the file, so a listing sorts chronologically.
     * roll_seq separates files that open within the same second, which
     * happens when a small max_records is set. */
    if ((size_t)snprintf(path, sizeof(path),
                "%s/cdr-%04d%02d%02d%02d%02d%02d-%u.%s",
                dir,
                tm_now.tm_year + 1900, tm_now.tm_mon + 1, tm_now.tm_mday,
                tm_now.tm_hour, tm_now.tm_min, tm_now.tm_sec,
                sink->roll_seq++, sink->ext) >= sizeof(path)) {
        ogs_error("CDR %s aggregate path too long", sink->label);
        return OGS_ERROR;
    }

    sink->fp = fopen(path, "ab");
    if (!sink->fp) {
        ogs_error("Cannot open CDR %s file %s — %s",
                sink->label, path, strerror(errno));
        return OGS_ERROR;
    }

    sink->path = ogs_strdup(path);
    if (!sink->path) {
        fclose(sink->fp);
        sink->fp = NULL;
        return OGS_ERROR;
    }

    sink->records = 0;
    sink->bytes = 0;
    ogs_cpystrn(sink->day, day, sizeof(sink->day));

    ogs_info("CDR %s file opened: %s", sink->label, path);
    return OGS_OK;
}

/*
 * Whether appending `len` more bytes should start a new file.
 *
 * Checked before the write, so a threshold is a ceiling rather than
 * something a file drifts past.  A file that is still empty is never
 * rolled, otherwise a record larger than max_bytes could never be
 * written at all.
 */
static const char *sink_roll_reason(const cdr_sink_t *sink,
        const chf_cdr_filing_t *policy, size_t len, const char *day)
{
    if (!sink->fp)
        return NULL;

    if (strcmp(sink->day, day) != 0)
        return "day";       /* the day turned over */

    if (!sink->records)
        return NULL;        /* nothing in it yet */

    if (policy->max_records && sink->records + 1 > policy->max_records)
        return "records";

    if (policy->max_bytes && sink->bytes + len > policy->max_bytes)
        return "size";

    return NULL;
}

static char *sink_write_aggregated(cdr_sink_t *sink,
        const chf_cdr_filing_t *policy,
        const void *data, size_t len, time_t now)
{
    char day[48];
    struct tm tm_today;

    gmtime_r(&now, &tm_today);
    snprintf(day, sizeof(day), "%04d/%02d/%02d",
            tm_today.tm_year + 1900, tm_today.tm_mon + 1, tm_today.tm_mday);

    {
        const char *reason = sink_roll_reason(sink, policy, len, day);
        if (reason) {
            chf_metrics_inst_labeled_inc(CHF_METR_LBL_CDR_FILE_ROLLED,
                    sink->label, reason, NULL);
            sink_close(sink);
        }
    }

    if (!sink->fp && sink_open(sink, now) != OGS_OK)
        return NULL;

    if (fwrite(data, 1, len, sink->fp) != len) {
        ogs_error("Short write to CDR %s file %s — %s",
                sink->label, sink->path, strerror(errno));
        /* Close it: a half-written record makes the rest of the file
         * undecodable, and a fresh file is the only safe continuation. */
        sink_close(sink);
        return NULL;
    }

    if (fflush(sink->fp) != 0)
        ogs_error("Cannot flush CDR %s file %s — %s",
                sink->label, sink->path, strerror(errno));

    sink->records++;
    sink->bytes += len;

    return ogs_strdup(sink->path);
}

/* ------------------------------------------------------------------ */
/* Individual filing                                                    */
/* ------------------------------------------------------------------ */

static char *sink_write_individual(cdr_sink_t *sink,
        const chf_sess_t *sess, const void *data, size_t len, time_t now)
{
    char dir[CHF_CDR_PATH_MAX];
    char day[48];
    char path[CHF_CDR_PATH_MAX + 64];
    FILE *fp;

    if (today_dir(dir, sizeof(dir), day, sizeof(day), now) != OGS_OK)
        return NULL;

    if ((size_t)snprintf(path, sizeof(path), "%s/cdr-%s.%s",
                dir, sess->charging_data_ref, sink->ext) >= sizeof(path)) {
        ogs_error("[%s] CDR %s file path too long — dir='%s'",
                sess->supi, sink->label, dir);
        return NULL;
    }

    fp = fopen(path, "wb");
    if (!fp) {
        ogs_error("[%s] Cannot open CDR %s file %s — %s",
                sess->supi, sink->label, path, strerror(errno));
        return NULL;
    }

    if (fwrite(data, 1, len, fp) != len) {
        ogs_error("[%s] Short write to CDR %s file %s — %s",
                sess->supi, sink->label, path, strerror(errno));
        fclose(fp);
        return NULL;
    }

    fclose(fp);
    return ogs_strdup(path);
}

static char *sink_write(cdr_sink_t *sink, const chf_sess_t *sess,
        const void *data, size_t len, time_t now)
{
    const chf_cdr_filing_t *policy = sink_policy(sink);

    if (policy->aggregate)
        return sink_write_aggregated(sink, policy, data, len, now);

    return sink_write_individual(sink, sess, data, len, now);
}

/* ------------------------------------------------------------------ */
/* Lifecycle                                                            */
/* ------------------------------------------------------------------ */

static const char *format_name(int formats)
{
    switch (formats & (CHF_CDR_FMT_JSON | CHF_CDR_FMT_ASN1)) {
    case CHF_CDR_FMT_JSON:                      return "json";
    case CHF_CDR_FMT_ASN1:                      return "asn1";
    case CHF_CDR_FMT_JSON | CHF_CDR_FMT_ASN1:   return "both";
    default:                                    return "none";
    }
}

static void log_filing(const char *label, const chf_cdr_filing_t *f)
{
    if (!f->aggregate) {
        ogs_info("CDR %s filing: one file per session", label);
        return;
    }

    ogs_info("CDR %s filing: aggregated, rolling at %u records / %llu MB "
             "(0 = no limit)", label, f->max_records,
             (unsigned long long)(f->max_bytes / (1024 * 1024)));
}

void chf_cdr_init(void)
{
    const char *cfg_path = chf_self()->cdr.path;

    if (cfg_path && cfg_path[0] != '\0') {
        cdr_base_path = ogs_strdup(cfg_path);
    } else {
        cdr_base_path = ogs_strdup(CHF_CDR_DEFAULT_BASE_PATH);
    }

    ogs_assert(cdr_base_path);

    cdr_local_seq = 0;
    sink_json.roll_seq = 0;
    sink_asn1.roll_seq = 0;

    ogs_info("CDR base directory: %s (enabled=%s format=%s)",
            cdr_base_path,
            chf_self()->cdr.enable ? "yes" : "no",
            format_name(chf_self()->cdr.formats));

    if (!chf_self()->cdr.enable)
        return;

    if (chf_self()->cdr.formats & CHF_CDR_FMT_JSON)
        log_filing("json", &chf_self()->cdr.json);
    if (chf_self()->cdr.formats & CHF_CDR_FMT_ASN1)
        log_filing("asn1", &chf_self()->cdr.asn1);
    if (!chf_self()->cdr.formats)
        ogs_warn("CDR writing is enabled but charging.cdr.format selects "
                 "no encoding — no CDRs will be written");
}

void chf_cdr_final(void)
{
    /* Close before freeing the base path: an open aggregate file still
     * has buffered records in it. */
    sink_close(&sink_json);
    sink_close(&sink_asn1);

    if (cdr_base_path) {
        ogs_free(cdr_base_path);
        cdr_base_path = NULL;
    }
}

/* ------------------------------------------------------------------ */
/* chf_cdr_write                                                        */
/* ------------------------------------------------------------------ */

char *chf_cdr_write(chf_sess_t *sess, const char *cause)
{
    CHFCDR_CHFRecord_t *rec = NULL;
    char *json = NULL;
    uint8_t *ber = NULL;
    int ber_len = 0;
    char *json_path = NULL;
    char *ber_path = NULL;
    char *result = NULL;
    size_t written = 0;
    int wanted = 0, done = 0;
    time_t now;
    int64_t t0;

    ogs_assert(sess);

    if (!chf_self()->cdr.enable) {
        ogs_debug("[%s] CDR writing disabled", sess->supi);
        return NULL;
    }

    if (!chf_self()->cdr.formats) {
        ogs_debug("[%s] CDR writing enabled but no encoding selected",
                sess->supi);
        return NULL;
    }

    t0 = ogs_get_monotonic_time();
    now = time(NULL);

    rec = chf_cdr_record_build(sess, cause, ++cdr_local_seq);
    if (!rec) {
        ogs_error("[%s] Cannot build CDR record [ref:%s]",
                sess->supi, sess->charging_data_ref);
        chf_metrics_inst_labeled_inc(CHF_METR_LBL_CDR_WRITTEN,
                "error", NULL, NULL);
        chf_metrics_inst_labeled_inc(CHF_METR_LBL_ERRORS, "cdr", NULL, NULL);
        return NULL;
    }

    if (chf_self()->cdr.formats & CHF_CDR_FMT_JSON) {
        wanted++;
        json = chf_cdr_json_render(rec);
        if (json) {
            size_t len = strlen(json);
            char *line = ogs_malloc(len + 2);

            /* One record per line, so an aggregated file reads back a
             * record at a time and an individual file still ends in a
             * newline. */
            if (line) {
                memcpy(line, json, len);
                line[len] = '\n';
                line[len + 1] = '\0';

                json_path = sink_write(&sink_json, sess, line, len + 1, now);
                if (json_path) {
                    done++;
                    written += len + 1;
                }
                chf_metrics_inst_labeled_inc(CHF_METR_LBL_CDR_ENCODED,
                        sink_json.label, json_path ? "success" : "error",
                        NULL);
                ogs_free(line);
            }
        } else {
            ogs_error("[%s] Cannot render CDR as JSON [ref:%s]",
                    sess->supi, sess->charging_data_ref);
            chf_metrics_inst_labeled_inc(CHF_METR_LBL_CDR_ENCODED,
                    sink_json.label, "error", NULL);
        }
    }

    if (chf_self()->cdr.formats & CHF_CDR_FMT_ASN1) {
        wanted++;
        ber_len = chf_cdr_record_encode_ber(rec, &ber);
        if (ber_len > 0) {
            ber_path = sink_write(&sink_asn1, sess, ber, (size_t)ber_len, now);
            if (ber_path) {
                done++;
                written += (size_t)ber_len;
            }
            chf_metrics_inst_labeled_inc(CHF_METR_LBL_CDR_ENCODED,
                    sink_asn1.label, ber_path ? "success" : "error", NULL);
        } else {
            ogs_error("[%s] Cannot encode CDR as BER [ref:%s]",
                    sess->supi, sess->charging_data_ref);
            chf_metrics_inst_labeled_inc(CHF_METR_LBL_CDR_ENCODED,
                    sink_asn1.label, "error", NULL);
        }
    }

    /* A record counts as written only when every selected encoding wrote
     * it: a BER file that is missing records the JSON has is exactly the
     * kind of divergence an operator needs to hear about. */
    chf_metrics_inst_labeled_inc(CHF_METR_LBL_CDR_WRITTEN,
            done == wanted ? "success" : "error", NULL, NULL);
    if (done != wanted)
        chf_metrics_inst_labeled_inc(CHF_METR_LBL_ERRORS, "cdr", NULL, NULL);

    chf_metrics_inst_global_add(CHF_METR_CTR_CDR_BYTES,
            chf_metrics_clamp_u64((uint64_t)written));
    chf_metrics_inst_global_add(CHF_METR_HIST_CDR_WRITE_DURATION,
            (int)((ogs_get_monotonic_time() - t0) / 1000));

    if (done)
        ogs_info("[%s] CDR written: %s%s%s "
                 "(UL=%llu DL=%llu dur=%us cause=%s seq=%u)",
                sess->supi,
                json_path ? json_path : "",
                (json_path && ber_path) ? " + " : "",
                ber_path ? ber_path : "",
                (unsigned long long)sess->ul_octets,
                (unsigned long long)sess->dl_octets,
                sess->duration,
                cause ? cause : "normalRelease",
                cdr_local_seq);

    /*
     * The CDR index in MongoDB holds one path.  JSON wins when both are
     * written, because that is the file a person opens; the BER file is
     * named the same way beside it, or is the aggregate stream its own
     * log line names.
     */
    if (json_path) {
        result = json_path;
        json_path = NULL;
    } else if (ber_path) {
        result = ber_path;
        ber_path = NULL;
    }

    if (json_path) ogs_free(json_path);
    if (ber_path) ogs_free(ber_path);
    if (json) ogs_free(json);
    if (ber) ogs_free(ber);
    chf_cdr_record_free(rec);

    return result;
}
