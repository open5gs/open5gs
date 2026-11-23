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
 * SMF-side Nchf_ConvergedCharging response handlers.
 *
 * These functions process CHF HTTP responses and update smf_sess_t state,
 * including wiring CHF-granted quota into the PFCP URR so the UPF enforces
 * the correct volume and time limits.
 *
 * JSON parsing uses cJSON directly (no OpenAPI codegen for TS 32.291 yet).
 *
 * 3GPP references:
 *   TS 32.291 §6.3.1.2 — ChargingDataResponse (Create)
 *   TS 32.291 §6.3.2.2 — ChargingDataResponse (Update)
 *   TS 32.291 §6.3.3.2 — termination (Release)
 */

#include "nchf-handler.h"
#include "nchf-build.h"
#include "pfcp-path.h"
#include "sbi-path.h"
#include "local-path.h"

/*
 * How long a UE is asked to wait before asking for a session again after
 * being refused for an empty balance (T3396).  Long enough to stop a UE
 * that re-establishes on its own from retrying every second, short enough
 * that a top-up becomes usable in about a minute.
 */
#define SMF_NO_QUOTA_BACKOFF_SECONDS 60

/* ------------------------------------------------------------------ */
/* Internal grant structure parsed from ChargingDataResponse          */
/* ------------------------------------------------------------------ */
typedef struct {
    bool     has_total_volume;
    uint64_t total_volume;      /* grantedUnit.totalVolume (bytes)      */
    bool     has_validity_time;
    uint32_t validity_time;     /* validityTime (seconds)               */
    bool     has_holding_time;
    uint32_t holding_time;      /* quotaHoldingTime (seconds)           */
    bool     has_volume_threshold;
    uint32_t volume_threshold;  /* volumeQuotaThreshold (bytes left)    */
    bool     final_unit;        /* finalUnitIndication present          */
} nchf_grant_t;

/* ------------------------------------------------------------------ */
/* smf_nchf_parse_mui()                                               */
/*                                                                    */
/* Read multipleUnitInformation[0] out of the CHF's response.         */
/* Fills *grant and returns true on success.  Returns false when the  */
/* response carries no multipleUnitInformation - the caller treats    */
/* that as "no grant", not as an error.                               */
/*                                                                    */
/* The body is parsed into OpenAPI_charging_data_response_t by        */
/* lib/sbi/message.c, like every other service; this only walks it.   */
/* TS 32.291 §6.3.1.2 / §6.3.2.2.                                     */
/* ------------------------------------------------------------------ */
static bool smf_nchf_parse_mui(
        OpenAPI_charging_data_response_t *rsp, nchf_grant_t *grant)
{
    OpenAPI_multiple_unit_information_t *mui = NULL;
    OpenAPI_lnode_t *node = NULL;

    ogs_assert(grant);
    memset(grant, 0, sizeof(*grant));

    if (!rsp || !rsp->multiple_unit_information)
        return false;

    OpenAPI_list_for_each(rsp->multiple_unit_information, node) {
        mui = node->data;
        break;              /* one rating group per session — see §11 */
    }
    if (!mui)
        return false;

    /* grantedUnit.totalVolume — how much this session may now use */
    if (mui->granted_unit && mui->granted_unit->is_total_volume) {
        grant->has_total_volume = true;
        grant->total_volume = (uint64_t)mui->granted_unit->total_volume;
    }

    /* validityTime — optional */
    if (mui->is_validity_time && mui->validity_time > 0) {
        grant->has_validity_time = true;
        grant->validity_time = (uint32_t)mui->validity_time;
    }

    /* quotaHoldingTime — optional; how long the session may sit idle
     * holding this grant before it has to report (TS 32.291 §6.3.1.2).
     * 0 means the CHF does not want idle reporting. */
    if (mui->is_quota_holding_time && mui->quota_holding_time >= 0) {
        grant->has_holding_time = true;
        grant->holding_time = (uint32_t)mui->quota_holding_time;
    }

    /* volumeQuotaThreshold — optional; 0 is meaningful (report only when
     * the grant is used up), so presence is tracked separately. */
    if (mui->is_volume_quota_threshold && mui->volume_quota_threshold >= 0) {
        grant->has_volume_threshold = true;
        grant->volume_threshold =
            mui->volume_quota_threshold > UINT32_MAX
                ? UINT32_MAX : (uint32_t)mui->volume_quota_threshold;
    }

    /* finalUnitIndication — presence only; SMF must act on exhaustion */
    grant->final_unit = (mui->final_unit_indication != NULL);

    return true;
}

/* ------------------------------------------------------------------ */
/* nchf_urr_configure()                                               */
/*                                                                    */
/* Apply a parsed CHF grant to a PFCP URR.                           */
/*                                                                    */
/* Volume quota:   set vol_quota to total_volume, and a threshold so  */
/*                 the UPF reports before the grant runs out: with    */
/*                 volumeQuotaThreshold bytes left if the CHF sent    */
/*                 one (0 = no early report), else at 90 %.           */
/* Validity time:  set quota_validity_time; enable ISTM measurement  */
/*                 method so the UPF tracks session duration.        */
/*                                                                    */
/* The caller is responsible for the PFCP transaction that pushes    */
/* these values to the UPF (Session Establishment or Modification).  */
/* ------------------------------------------------------------------ */
static void nchf_urr_configure(ogs_pfcp_urr_t *urr, const nchf_grant_t *grant)
{
    ogs_assert(urr);
    ogs_assert(grant);

    if (grant->has_total_volume && grant->total_volume > 0) {
        /* Volume quota — triggers URR report when exhausted */
        urr->meas_method |= OGS_PFCP_MEASUREMENT_METHOD_VOLUME;
        urr->rep_triggers.volume_quota  = 1;
        urr->vol_quota.tovol            = 1;
        urr->vol_quota.total_volume     = grant->total_volume;

        /* Volume threshold — an early report, so the next grant arrives
         * before this one runs out.
         *
         * The CHF says how early with volumeQuotaThreshold, the volume to
         * have left when reporting (TS 32.291 MultipleUnitInformation).
         * 0, or a value that is not below the grant, means no early
         * report: the UPF reports when the grant is used up.  A CHF that
         * sends none gets the 90 % default.  Dividing before multiplying
         * keeps the intermediate value from overflowing on very large
         * grants. */
        if (grant->has_volume_threshold &&
                (grant->volume_threshold == 0 ||
                 grant->volume_threshold >= grant->total_volume)) {
            urr->rep_triggers.volume_threshold = 0;
            memset(&urr->vol_threshold, 0, sizeof(urr->vol_threshold));
        } else {
            urr->rep_triggers.volume_threshold = 1;
            urr->vol_threshold.tovol           = 1;
            urr->vol_threshold.total_volume    =
                grant->has_volume_threshold
                ? grant->total_volume - grant->volume_threshold
                : grant->total_volume / 10 * 9;
        }

        ogs_debug("[CHF URR] vol_quota=%" PRIu64 "  vol_threshold=%" PRIu64,
                urr->vol_quota.total_volume,
                urr->vol_threshold.total_volume);
    }

    if (grant->has_validity_time && grant->validity_time > 0) {
        /* Quota validity time — UPF reports when the grant timer expires */
        urr->meas_method |= OGS_PFCP_MEASUREMENT_METHOD_DURATION;
        urr->meas_info.istm               = 1;  /* measure start time */
        urr->rep_triggers.quota_validity_time = 1;
        urr->quota_validity_time          = grant->validity_time;

        ogs_debug("[CHF URR] quota_validity_time=%" PRIu32 "s",
                urr->quota_validity_time);
    }

    /* Quota holding time — the UPF reports once the session has been idle
     * this long, so the CHF can take the rest of the grant back into the
     * cycle instead of waiting out validityTime.  An absent or zero value
     * turns idle reporting off; the grant is then held until it is spent
     * or its validity expires. */
    if (grant->has_holding_time && grant->holding_time > 0) {
        urr->meas_method |= OGS_PFCP_MEASUREMENT_METHOD_DURATION;
        urr->meas_info.istm                = 1;
        urr->rep_triggers.quota_holding_time = 1;
        urr->quota_holding_time            = grant->holding_time;

        ogs_debug("[CHF URR] quota_holding_time=%" PRIu32 "s",
                urr->quota_holding_time);
    } else {
        urr->rep_triggers.quota_holding_time = 0;
        urr->quota_holding_time              = 0;
    }
}

/* ------------------------------------------------------------------ */
/* Create handler — processes CHF 201 Created                         */
/* ------------------------------------------------------------------ */

bool smf_nchf_convergedcharging_handle_create(
        smf_sess_t *sess, ogs_sbi_message_t *recvmsg)
{
    const char *location = NULL;
    const char *ref      = NULL;
    nchf_grant_t grant;
    smf_bearer_t *bearer = NULL;

    ogs_assert(sess);
    ogs_assert(recvmsg);

    /* Expect 201 Created (TS 32.291 §6.3.1.2) */
    if (recvmsg->res_status != OGS_SBI_HTTP_STATUS_CREATED) {
        ogs_error("[%s] CHF Create failed [status:%d]",
                smf_nchf_dnn(sess), recvmsg->res_status);
        return false;
    }

    /* Extract Location header — this is the ChargingDataRef resource URI.
     * ogs_sbi_parse_response() stores it in message.http.location. */
    location = recvmsg->http.location;
    if (!location) {
        ogs_error("[%s] CHF Create response missing Location header",
                smf_nchf_dnn(sess));
        return false;
    }

    /* ChargingDataRef = last path component of Location URL
     * e.g. "http://chf:7777/nchf-convergedcharging/v3/chargingdata/{uuid}"
     *                                                                  ^^^^
     */
    ref = strrchr(location, '/');
    if (!ref || *(ref + 1) == '\0') {
        ogs_error("[%s] CHF Create: cannot extract ref from Location [%s]",
                smf_nchf_dnn(sess), location);
        return false;
    }
    ref++; /* skip the leading '/' */

    CHF_STORE(sess, location, ref);

    ogs_info("[%s] CHF association established [ref:%s]",
            smf_nchf_dnn(sess), sess->chf_association.ref);

    /* -------------------------------------------------------------- */
    /* GAP 1+2: Parse grantedUnit / validityTime from response body   */
    /* and wire the values into the bearer URR so the UPF enforces    */
    /* the correct quota from the very first PFCP Session Establish.  */
    /* -------------------------------------------------------------- */
    if (smf_nchf_parse_mui(recvmsg->ChargingDataResponse, &grant)) {
        bearer = smf_default_bearer_in_sess(sess);
        if (bearer && bearer->urr) {
            nchf_urr_configure(bearer->urr, &grant);

            /* Ensure both UL and DL PDRs reference the URR so that traffic
             * in both directions is counted against the quota.
             * (The UL association is also added in context.c at session
             *  setup — this call is idempotent and harmless.) */
            if (bearer->ul_pdr)
                ogs_pfcp_pdr_associate_urr(bearer->ul_pdr, bearer->urr);
            if (bearer->dl_pdr)
                ogs_pfcp_pdr_associate_urr(bearer->dl_pdr, bearer->urr);

            if (grant.final_unit) {
                sess->chf.final_unit = true;

                if (!grant.has_total_volume || grant.total_volume == 0) {
                    /*
                     * Nothing left this cycle.  Refuse the PDU session
                     * instead of establishing one with no quota: with a
                     * zero grant the URR carries no volume quota, so the
                     * UPF would not report until its own threshold and the
                     * subscriber would get that much service unbilled by
                     * the plan.  The UE then backs off by its own rules
                     * rather than holding a session that cannot carry
                     * traffic.
                     *
                     * A FinalUnitIndication that still grants volume is a
                     * last allowance, not a refusal: that session is
                     * established and torn down when the grant runs out.
                     */
                    ogs_warn("[%s] CHF Create: no quota left — refusing the "
                             "PDU session", smf_nchf_dnn(sess));
                    sess->sm_data.establishment_reject_cause =
                        OGS_5GSM_CAUSE_INSUFFICIENT_RESOURCES;
                    sess->sm_data.establishment_reject_backoff =
                        SMF_NO_QUOTA_BACKOFF_SECONDS;
                    return false;
                }

                ogs_warn("[%s] CHF Create: FinalUnitIndication at session "
                         "start — this is the subscriber's last quota",
                        smf_nchf_dnn(sess));
            }
        } else {
            ogs_warn("[%s] CHF Create: grant received but no URR "
                     "on default bearer — quota not applied",
                    smf_nchf_dnn(sess));
        }
    } else {
        /* GAP-F: distinguish "no body" (unlimited / no quota control)
         * from "body present but carrying no grant" (CHF bug).
         *
         * TS 32.291 §6.3.1.2 requires multipleUnitInformation in the 201
         * Created response when online charging is active.  A missing body
         * may be intentional (unlimited quota policy) or a CHF defect.
         *
         * Policy:
         *   no ChargingDataResponse : CHF omitted the body — unlimited.
         *     URR keeps the 100 MiB early-warning threshold only (vol_quota
         *     is not set so the UPF never fires a quota-exhausted trigger;
         *     the threshold report will trigger a CHF Update that should
         *     return a proper grant).  Emit a warning so operators notice.
         *
         *   a response present but with no multipleUnitInformation
         *     (invalid JSON or missing multipleUnitInformation array).
         *     This is a CHF conformance failure.  Reject the session so the
         *     UE gets a clean error rather than running without any quota
         *     enforcement.  parse_mui() already logged the parse error. */
        if (recvmsg->ChargingDataResponse) {
            ogs_error("[%s] CHF Create: response body present but "
                      "multipleUnitInformation missing or invalid — "
                      "rejecting session (CHF non-conformant per "
                      "TS 32.291 §6.3.1.2)",
                    smf_nchf_dnn(sess));
            return false;
        }
        ogs_warn("[%s] CHF Create: no grant body — treating as unlimited "
                 "quota (vol_quota not set; only 100 MiB threshold active)",
                smf_nchf_dnn(sess));
    }

    return true;
}

/* ------------------------------------------------------------------ */
/* Update handler — processes CHF 200 OK                              */
/* ------------------------------------------------------------------ */

bool smf_nchf_convergedcharging_handle_update(
        smf_sess_t *sess, ogs_sbi_message_t *recvmsg)
{
    nchf_grant_t grant;
    smf_bearer_t *bearer  = NULL;
    ogs_pfcp_urr_t *urr   = NULL;
    uint64_t modify_flags = 0;

    /* Saved pre-update values for change detection */
    ogs_pfcp_measurement_method_t   prev_meas_method;
    ogs_pfcp_reporting_triggers_t   prev_rep_triggers;
    ogs_pfcp_quota_validity_time_t  prev_quota_validity_time;
    ogs_pfcp_quota_holding_time_t   prev_quota_holding_time;
    ogs_pfcp_volume_threshold_t     prev_vol_threshold;

    ogs_assert(sess);
    ogs_assert(recvmsg);

    /* Clear the in-flight guard unconditionally on any response (success or
     * error) so the next PFCP report can trigger a new CHF Update.  Cleared
     * here rather than in gsm-sm.c so the guard is always released even if
     * the caller ignores the return value.
     *
     * Note: chf.last_report is NOT touched here — build_usage_report()
     * advances it when the request is built.  See the comment there. */
    sess->sm_data.nchf_update_in_flight = false;

    /* Expect 200 OK (TS 32.291 §6.3.2.2) */
    if (recvmsg->res_status != OGS_SBI_HTTP_STATUS_OK) {
        ogs_error("[%s] CHF Update failed [ref:%s status:%d]",
                smf_nchf_dnn(sess), smf_nchf_ref(sess), recvmsg->res_status);
        return false;
    }

    ogs_debug("[%s] CHF Update OK [ref:%s]",
            smf_nchf_dnn(sess), smf_nchf_ref(sess));

    /* -------------------------------------------------------------- */
    /* GAP 1+2: Parse refreshed grant and push new URR params to UPF  */
    /* via a PFCP Session Modification.                               */
    /* -------------------------------------------------------------- */
    if (!smf_nchf_parse_mui(recvmsg->ChargingDataResponse, &grant)) {
        /* GAP-F: distinguish absent body from present-but-invalid body.
         *
         *   no ChargingDataResponse : CHF chose not to refresh (rare but
         *     acceptable — quota unchanged).  Debug log only.
         *
         *   a response present but multipleUnitInformation is
         *     missing or malformed.  This is a CHF conformance issue per
         *     TS 32.291 §6.3.2.2.  Warn so operators notice that the UPF's
         *     URR is running on the last known quota values with no refresh.
         *     We do NOT tear down the session — the subscriber already has
         *     service and the last quota snapshot is still valid.  The next
         *     PFCP threshold report will trigger another CHF Update attempt. */
        if (recvmsg->ChargingDataResponse)
            ogs_warn("[%s] CHF Update: response body present but "
                     "multipleUnitInformation missing/invalid — "
                     "URR quota not refreshed (stale values remain active) "
                     "[ref:%s]", smf_nchf_dnn(sess), smf_nchf_ref(sess));
        else
            ogs_debug("[%s] CHF Update: no grant body — URR unchanged",
                    smf_nchf_dnn(sess));
        return true;
    }

    if (grant.final_unit) {
        /*
         * Final Unit: the caller tears the session down on return,
         * whether or not the bearer has a URR, so this comes first.  Do NOT
         * update the URR first.  A final grant carries no new volume, so
         * nchf_urr_configure() would leave the old, already consumed quota
         * on the URR, and pushing that to the UPF only makes it report the
         * exhaustion again - a Session Report Request that races the
         * deletion that follows.
         */
        sess->chf.final_unit = true;
        return true;
    }

    bearer = smf_default_bearer_in_sess(sess);
    if (!bearer || !bearer->urr) {
        ogs_warn("[%s] CHF Update: grant received but no URR — skipping",
                smf_nchf_dnn(sess));
        return true;
    }
    urr = bearer->urr;

    /* Snapshot pre-update state for change detection */
    prev_meas_method          = urr->meas_method;
    prev_rep_triggers         = urr->rep_triggers;
    prev_quota_validity_time  = urr->quota_validity_time;
    prev_quota_holding_time   = urr->quota_holding_time;
    prev_vol_threshold        = urr->vol_threshold;

    nchf_urr_configure(urr, &grant);

    /* Ensure associations are in place (idempotent) */
    if (bearer->ul_pdr)
        ogs_pfcp_pdr_associate_urr(bearer->ul_pdr, urr);
    if (bearer->dl_pdr)
        ogs_pfcp_pdr_associate_urr(bearer->dl_pdr, urr);

    /* Build PFCP modify_flags by comparing old vs new URR fields.
     * Mirror the pattern in smf_gy_handle_cca_update_request(). */
    if (urr->meas_method != prev_meas_method)
        modify_flags |= OGS_PFCP_MODIFY_URR_MEAS_METHOD;

    if (urr->rep_triggers.quota_validity_time !=
            prev_rep_triggers.quota_validity_time ||
        urr->rep_triggers.time_quota      != prev_rep_triggers.time_quota ||
        urr->rep_triggers.volume_quota    != prev_rep_triggers.volume_quota ||
        urr->rep_triggers.time_threshold  != prev_rep_triggers.time_threshold ||
        urr->rep_triggers.volume_threshold !=
            prev_rep_triggers.volume_threshold ||
        urr->rep_triggers.quota_holding_time !=
            prev_rep_triggers.quota_holding_time)
        modify_flags |= OGS_PFCP_MODIFY_URR_REPORT_TRIGGER;

    if (urr->quota_validity_time != prev_quota_validity_time)
        modify_flags |= OGS_PFCP_MODIFY_URR_QUOTA_VALIDITY_TIME;

    if (urr->quota_holding_time != prev_quota_holding_time)
        modify_flags |= OGS_PFCP_MODIFY_URR_QUOTA_HOLDING_TIME;

    /* Volume quota is re-sent whenever one is set, even if the value is
     * unchanged — unlike the other fields this is not change-detection.
     * The UPF consumes the quota as traffic flows, so the usual case for an
     * Update is "quota exhausted, here is the same allowance again"; without
     * re-sending, the URR would stay exhausted and traffic would remain
     * blocked. */
    if (urr->vol_quota.tovol || urr->vol_quota.total_volume)
        modify_flags |= OGS_PFCP_MODIFY_URR_VOLUME_QUOTA;

    if (urr->vol_threshold.tovol != prev_vol_threshold.tovol ||
        urr->vol_threshold.total_volume != prev_vol_threshold.total_volume)
        modify_flags |= OGS_PFCP_MODIFY_URR_VOLUME_THRESH;

    if (modify_flags) {
        int rv;
        /* OGS_PFCP_MODIFY_URR signals that this is a URR-only update.
         * OGS_PFCP_MODIFY_UL_ONLY is intentionally NOT set here: it
         * controls PDR iteration for PDR-level changes (FAR updates, etc.)
         * and has no effect on the urr_list loop that serialises Update URR
         * IEs — adding it would only mislead readers into thinking only the
         * UL direction is updated, when in fact the single shared URR covers
         * both directions.  See lib/pfcp/build.c ogs_pfcp_build_update_urr(). */
        modify_flags |= OGS_PFCP_MODIFY_URR;

        /* stream is NULL here because the CHF Update response arrives
         * asynchronously — there is no N1/N2 message awaiting this PFCP
         * completion.  pfcp-path.c allows NULL when OGS_PFCP_MODIFY_URR
         * is set (the assertion was relaxed for exactly this use-case). */
        rv = smf_5gc_pfcp_send_all_pdr_modification_request(
                sess, NULL, modify_flags, 0, 0);
        if (rv != OGS_OK)
            ogs_warn("[%s] Failed to send PFCP URR modification [ref:%s]",
                    smf_nchf_dnn(sess), smf_nchf_ref(sess));
    }

    return true;
}

/* ------------------------------------------------------------------ */
/* Release handler — processes CHF 204 No Content                     */
/* ------------------------------------------------------------------ */

bool smf_nchf_convergedcharging_handle_release(
        smf_sess_t *sess, ogs_sbi_message_t *recvmsg)
{
    ogs_assert(sess);
    ogs_assert(recvmsg);

    /* Expect 204 No Content (TS 32.291 §6.3.3.2).  Anything else is
     * non-fatal: clear the association regardless so teardown proceeds. */
    if (recvmsg->res_status != OGS_SBI_HTTP_STATUS_NO_CONTENT)
        ogs_warn("[%s] CHF Release unexpected status [ref:%s status:%d] — "
                 "clearing association anyway",
                smf_nchf_dnn(sess), smf_nchf_ref(sess), recvmsg->res_status);
    else
        ogs_info("[%s] CHF Release OK [ref:%s]",
                smf_nchf_dnn(sess), smf_nchf_ref(sess));

    CHF_CLEAR(sess);
    sess->sm_data.nchf_release_in_flight = false;

    return true;
}

/* ------------------------------------------------------------------ */
/* ChargingNotify — the CHF reaching a live session                    */
/*                                                                      */
/* TS 32.291 §6.3.4: the CHF POSTs a ChargingNotifyRequest to the      */
/* notifyUri the SMF gave it, when something should reach the session  */
/* before its next report.  Two kinds:                                 */
/*                                                                      */
/*   REAUTHORIZATION — the quota changed.  A top-up, a larger plan, a  */
/*     new billing cycle.  The SMF reports at once, and the response   */
/*     to that report carries the grant that reflects the change.      */
/*                                                                      */
/*   ABORT_CHARGING — this session may not continue.  A bar, or a plan */
/*     revoked.  The SMF tears it down the same way it does when the   */
/*     quota runs out, so the UE is told rather than left with a       */
/*     session whose traffic is dropped.                               */
/*                                                                      */
/* Without this the CHF can only answer a report, and with grants      */
/* capped at 50 MiB and five minutes an idle session would not hear    */
/* about a top-up for up to five minutes.                              */
/* ------------------------------------------------------------------ */

void smf_nchf_convergedcharging_handle_charging_notify(
        smf_sess_t *sess, ogs_sbi_stream_t *stream,
        ogs_sbi_message_t *recvmsg)
{
    OpenAPI_charging_notify_request_t *notify = NULL;
    const char *type = NULL;
    bool abort_charging = false;
    int r;

    ogs_assert(sess);
    ogs_assert(stream);
    ogs_assert(recvmsg);

    notify = recvmsg->ChargingNotifyRequest;
    if (notify) {
        switch (notify->notification_type) {
        case OpenAPI_charging_notification_type_ABORT_CHARGING:
            abort_charging = true;
            type = "ABORT_CHARGING";
            break;
        case OpenAPI_charging_notification_type_REAUTHORIZATION:
            type = "REAUTHORIZATION";
            break;
        default:
            break;
        }
    }

    /* notificationType is mandatory (TS 32.291 §6.3.4).  An absent or
     * unknown one is refused rather than guessed at: the two kinds mean
     * opposite things, and acting on the wrong one either tears down a
     * paid-up session or keeps a barred one running. */
    if (!type) {
        ogs_error("[%s] ChargingNotify with no usable notificationType "
                  "[ref:%s]", smf_nchf_dnn(sess), smf_nchf_ref(sess));
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST, recvmsg,
                "No notificationType", smf_nchf_ref(sess), NULL));
        return;
    }

    /* Answer before acting.  What follows talks to the same CHF, and it
     * should not still be waiting on this response when that arrives. */
    ogs_assert(true == ogs_sbi_send_http_status_no_content(stream));

    if (abort_charging) {
        ogs_info("[%s] CHF aborted charging — releasing the PDU session "
                 "[ref:%s]", smf_nchf_dnn(sess), smf_nchf_ref(sess));

        /* Same teardown as a spent quota: the PFCP deletion first, so its
         * response carries the UPF's final usage for the CHF Release, and
         * for 5GC a PDU Session Release Command over N1/N2 after it. */
        sess->chf.final_unit = true;
        if (!sess->epc)
            sess->sm_data.fui_release_triggered = true;
        smf_trigger_session_release(sess, NULL,
                OGS_PFCP_DELETE_TRIGGER_LOCAL_INITIATED);
        return;
    }

    if (!CHF_ASSOCIATED(sess)) {
        ogs_warn("[%s] ChargingNotify for a session with no CHF association "
                 "— ignored", smf_nchf_dnn(sess));
        return;
    }

    if (sess->chf.final_unit) {
        /* Already on its way out on a FinalUnitIndication.  Reporting now
         * would ask for a grant the session will not live to use. */
        ogs_info("[%s] ChargingNotify after Final Unit — the session is "
                 "already being released [ref:%s]",
                smf_nchf_dnn(sess), smf_nchf_ref(sess));
        return;
    }

    if (sess->sm_data.nchf_update_in_flight) {
        /* An Update is already on its way and its response carries the
         * fresh grant, so the notification is satisfied by it.  Only one
         * Update may be outstanding per charging session - see
         * chf_handle_usage_report() in n4-handler.c. */
        ogs_debug("[%s] ChargingNotify while an Update is in flight — "
                  "that Update picks the change up [ref:%s]",
                smf_nchf_dnn(sess), smf_nchf_ref(sess));
        return;
    }

    /*
     * Report now.  The usage in it is whatever the UPF has reported since
     * the last Update, often nothing - the point is not the volume but the
     * answer, which re-reads the subscriber's plan and grants against it.
     * Usage the UPF has measured but not yet reported is not lost: it stays
     * in its counters and is billed by the next report.
     */
    if (sess->chf.trigger < SMF_CHF_TRIGGER_FORCED_REAUTH)
        sess->chf.trigger = SMF_CHF_TRIGGER_FORCED_REAUTH;

    ogs_info("[%s] CHF asked for re-authorisation — reporting now [ref:%s]",
            smf_nchf_dnn(sess), smf_nchf_ref(sess));

    sess->sm_data.nchf_update_in_flight = true;
    r = smf_sbi_discover_and_send(
            OpenAPI_service_name_nchf_convergedcharging, NULL,
            smf_nchf_convergedcharging_build_update,
            sess, NULL, 0, NULL);
    if (r != OGS_OK) {
        ogs_error("[%s] CHF Update send failed after ChargingNotify [ref:%s]",
                smf_nchf_dnn(sess), smf_nchf_ref(sess));
        sess->sm_data.nchf_update_in_flight = false;
    }
}

/* ------------------------------------------------------------------ */
/* Final Release — sent once, after the last usage is known           */
/* ------------------------------------------------------------------ */

void smf_nchf_send_release(smf_sess_t *sess)
{
    int r;

    ogs_assert(sess);

    if (!CHF_ASSOCIATED(sess) || sess->sm_data.nchf_release_in_flight)
        return;

    sess->sm_data.nchf_release_in_flight = true;
    r = smf_sbi_discover_and_send(
            OpenAPI_service_name_nchf_convergedcharging, NULL,
            smf_nchf_convergedcharging_build_release,
            sess, NULL, 0, NULL);
    if (r != OGS_OK)
        ogs_warn("[%s] CHF Release send failed [ref:%s]",
                smf_nchf_dnn(sess), smf_nchf_ref(sess));
}
