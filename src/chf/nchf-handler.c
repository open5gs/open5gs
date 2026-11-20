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
#include "nchf-handler.h"
#include "db-handler.h"
#include "cdr-path.h"
#include "metrics.h"

/*
 * Nchf_ConvergedCharging — server-side handlers (TS 32.291)
 *
 * Phase 1d: Quota engine.
 *   - chf_apply_quota():          initialise granted volume/time/action on Create
 *   - chf_check_quota_exhausted(): detect UL+DL >= total granted
 *   - build_charging_data_response(): remaining quota + FinalUnitIndication
 *
 * Resource layout (TS 32.291 §6.1):
 *   POST   /nchf-convergedcharging/v3/chargingdata               → Create
 *   POST   /nchf-convergedcharging/v3/chargingdata/{ref}/update  → Update
 *   POST   /nchf-convergedcharging/v3/chargingdata/{ref}/release → Release
 */

/* ------------------------------------------------------------------ */
/* Quota engine — helpers now live in context.c for unit-test access   */
/* ------------------------------------------------------------------ */
/*
 * chf_apply_quota()        — see context.h / context.c
 * chf_check_quota_exhausted() — see context.h / context.c
 * chf_supi_aggregate_used()   — see context.h / context.c
 *
 * All three are declared in context.h so that unit tests can call them
 * without linking the full HTTP handler translation unit.
 */

/* ------------------------------------------------------------------ */
/* parse_used_units()                                                   */
/* Sum usedUnitContainer volumes/time out of a ChargingDataRequest       */
/* (TS 32.291 §6.3.2.2 / §6.3.3.2).  Shared by Update and Release,      */
/* which report the same multipleUnitUsage structure.                   */
/*                                                                      */
/* Usage across all rating groups is accumulated together; per-rating-  */
/* group accounting is a later phase.                                   */
/*                                                                      */
/* Negative values are ignored rather than accumulated.  The numbers    */
/* come from a peer, and casting a negative one to uint64_t wraps to a  */
/* near-2^64 value, which would instantly exhaust the quota, tear the   */
/* session down and write a corrupt CDR.                                */
/* ------------------------------------------------------------------ */
static void parse_used_units(const OpenAPI_charging_data_request_t *req,
        uint64_t *ul, uint64_t *dl, uint32_t *dur)
{
    OpenAPI_lnode_t *muu_node = NULL, *uuc_node = NULL;

    *ul = 0; *dl = 0; *dur = 0;

    if (!req || !req->multiple_unit_usage)
        return;

    OpenAPI_list_for_each(req->multiple_unit_usage, muu_node) {
        OpenAPI_multiple_unit_usage_t *muu = muu_node->data;
        if (!muu || !muu->used_unit_container)
            continue;

        OpenAPI_list_for_each(muu->used_unit_container, uuc_node) {
            OpenAPI_used_unit_container_t *uuc = uuc_node->data;
            if (!uuc)
                continue;

            if (uuc->is_uplink_volume && uuc->uplink_volume > 0)
                *ul += (uint64_t)uuc->uplink_volume;
            if (uuc->is_downlink_volume && uuc->downlink_volume > 0)
                *dl += (uint64_t)uuc->downlink_volume;
            if (uuc->is_time && uuc->time > 0)
                *dur += (uint32_t)uuc->time;
        }
    }
}

/* ------------------------------------------------------------------ */
/* parse_trigger()                                                      */
/*                                                                      */
/* TS 32.291 triggers[0].triggerType: why the SMF is reporting.  The    */
/* CHF does not act on it - the numbers decide what it grants - but a   */
/* log line that says "the grant ran out" rather than leaving an        */
/* operator to infer it from two volumes is worth the few bytes.        */
/*                                                                      */
/* Returns one of our own constants, never the peer's string: it goes   */
/* into a log line, and the generated enum covers values this CHF has   */
/* no opinion about.                                                    */
/* ------------------------------------------------------------------ */
static const char *parse_trigger(const OpenAPI_charging_data_request_t *req)
{
    OpenAPI_lnode_t *node = NULL;
    OpenAPI_charging_trigger_t *trigger = NULL;

    if (!req || !req->triggers)
        return NULL;

    OpenAPI_list_for_each(req->triggers, node) {
        trigger = node->data;
        break;
    }
    if (!trigger)
        return NULL;

    switch (trigger->trigger_type) {
    case OpenAPI_trigger_type_QUOTA_THRESHOLD:        return "QUOTA_THRESHOLD";
    case OpenAPI_trigger_type_QUOTA_EXHAUSTED:        return "QUOTA_EXHAUSTED";
    case OpenAPI_trigger_type_VALIDITY_TIME:          return "VALIDITY_TIME";
    case OpenAPI_trigger_type__FINAL:                 return "FINAL";
    case OpenAPI_trigger_type_QHT:                    return "QHT";
    case OpenAPI_trigger_type_FORCED_REAUTHORISATION:
        return "FORCED_REAUTHORISATION";
    case OpenAPI_trigger_type_OTHER_QUOTA_TYPE:       return "OTHER_QUOTA_TYPE";
    case OpenAPI_trigger_type_NULL:                   return NULL;
    default:                                          return "OTHER";
    }
}

/* ------------------------------------------------------------------ */
/* parse_snssai()                                                       */
/*                                                                      */
/* TS 29.571 Snssai: {"sst": <0..255>, "sd": "<6 hex digits>"}.        */
/* An absent or malformed sd leaves OGS_S_NSSAI_NO_SD_VALUE, which is  */
/* how Open5GS spells "no slice differentiator".                        */
/* ------------------------------------------------------------------ */
static void parse_snssai(const OpenAPI_snssai_t *snssai,
        ogs_s_nssai_t *s_nssai)
{
    ogs_assert(s_nssai);

    s_nssai->sst = 0;
    s_nssai->sd.v = OGS_S_NSSAI_NO_SD_VALUE;

    if (!snssai)
        return;

    s_nssai->sst = (uint8_t)snssai->sst;

    if (snssai->sd && strlen(snssai->sd) == 6) {
        char *end = NULL;
        unsigned long v = strtoul(snssai->sd, &end, 16);
        if (end && *end == '\0')
            s_nssai->sd.v = (uint32_t)v;
    }
}

/* ------------------------------------------------------------------ */
/* request_sst_is_valid()                                               */
/*                                                                      */
/* The model holds sst as an int and ogs_s_nssai_t as a uint8_t, so 257 */
/* would be recorded as SST 1 and drawn from SST 1's bucket.  With no   */
/* S-NSSAI in the request there is nothing to check.                    */
/* ------------------------------------------------------------------ */
static bool request_sst_is_valid(const OpenAPI_charging_data_request_t *req)
{
    OpenAPI_pdu_session_charging_information_t *info = NULL;
    OpenAPI_pdu_session_information_t *pdu_info = NULL;
    OpenAPI_snssai_t *snssai = NULL;

    ogs_assert(req);

    info = req->p_du_session_charging_information;
    pdu_info = info ? info->pdu_session_information : NULL;
    if (!pdu_info || !pdu_info->network_slicing_info)
        return true;

    snssai = pdu_info->network_slicing_info->s_nssai;
    if (!snssai || (snssai->sst >= 0 && snssai->sst <= 255))
        return true;

    ogs_error("Invalid sst [%d]", snssai->sst);
    return false;
}

/* ------------------------------------------------------------------ */
/* parse_session_info()                                                 */
/*                                                                      */
/* Read the session's details out of a ChargingDataRequest.             */
/*                                                                      */
/* TS 32.291 §7.2.1.1 carries them nested:                              */
/*   pDUSessionChargingInformation.userInformation.servedGPSI           */
/*   pDUSessionChargingInformation.pduSessionInformation.{              */
/*       pduSessionID, dnnId, networkSlicingInfo.sNSSAI, hPlmnId }      */
/*                                                                      */
/* The flat top-level spelling an early build of this SMF used is no    */
/* longer accepted: those keys are not in the schema, so the generated  */
/* parser drops them.  Both sides ship in the same patch set and move   */
/* together, and the nested form is what the specification defines.     */
/* ------------------------------------------------------------------ */
static void parse_session_info(const OpenAPI_charging_data_request_t *req,
        char **gpsi, char **dnn, int *psi, ogs_s_nssai_t *s_nssai,
        char *hplmn, size_t hplmn_size, char **roaming)
{
    OpenAPI_pdu_session_charging_information_t *info = NULL;
    OpenAPI_pdu_session_information_t *pdu_info = NULL;

    ogs_assert(req);

    if (s_nssai) {
        s_nssai->sst = 0;
        s_nssai->sd.v = OGS_S_NSSAI_NO_SD_VALUE;
    }

    info = req->p_du_session_charging_information;
    if (!info)
        return;

    if (info->user_information) {
        if (info->user_information->served_gpsi)
            *gpsi = info->user_information->served_gpsi;

        /* TS 32.291 RoamerInOut: IN_BOUND | OUT_BOUND.  Absent means
         * the subscriber is on its home network. */
        if (roaming) {
            switch (info->user_information->roamer_in_out) {
            case OpenAPI_roamer_in_out_IN_BOUND:
                *roaming = (char *)"inBound";
                break;
            case OpenAPI_roamer_in_out_OUT_BOUND:
                *roaming = (char *)"outBound";
                break;
            default:
                break;
            }
        }
    }

    pdu_info = info->pdu_session_information;
    if (!pdu_info)
        return;

    if (pdu_info->dnn_id)
        *dnn = pdu_info->dnn_id;

    *psi = pdu_info->pdu_session_id;

    if (s_nssai && pdu_info->network_slicing_info)
        parse_snssai(pdu_info->network_slicing_info->s_nssai, s_nssai);

    /* hPlmnId: {"mcc": "999", "mnc": "70"} — kept as "MCC-MNC" so a
     * bucket can name the home network it belongs to. */
    if (hplmn && hplmn_size && pdu_info->h_plmn_id &&
            pdu_info->h_plmn_id->mcc && pdu_info->h_plmn_id->mnc)
        ogs_snprintf(hplmn, hplmn_size, "%s-%s",
                pdu_info->h_plmn_id->mcc, pdu_info->h_plmn_id->mnc);
}


/* ------------------------------------------------------------------ */
/* sess_validity_time()                                                 */
/*                                                                      */
/* validityTime for the next grant, in seconds; 0 means omit it.        */
/*                                                                      */
/* TS 32.291 §6.3.1.2: the time remaining until the granted quota       */
/* expires.  That is at most the time left in the billing cycle - never */
/* the full cycle length, which would restart the UPF's timer on every  */
/* Update and make the cycle a rolling window - and at most the         */
/* configured cap, so an idle session still reports back and picks up   */
/* plan changes.  With the cycle already over it is omitted, and Case F */
/* in the Update handler deals with the expiry on the next report.      */
/* ------------------------------------------------------------------ */
static uint32_t sess_validity_time(const chf_sess_t *sess)
{
    uint32_t cycle_left = 0;

    if (sess->period_length_sec > 0) {
        time_t now     = time(NULL);
        time_t expires = sess->period_start + (time_t)sess->period_length_sec;

        if (now >= expires)
            return 0;
        cycle_left = (uint32_t)(expires - now);
    }

    return chf_grant_validity(cycle_left,
            chf_self()->grant.max_validity_seconds);
}

/* ------------------------------------------------------------------ */
/* Helper: build the ChargingDataResponse (multipleUnitInformation)     */
/*                                                                      */
/* TS 32.291 §6.3.1.2 / §6.3.2.2.  Filled into caller-owned storage so  */
/* ogs_sbi_build_response() can serialise it; the caller then calls     */
/* build_charging_data_response_finalize() to release the one list.     */
/* ------------------------------------------------------------------ */
struct chf_response_storage {
    OpenAPI_charging_data_response_t rsp;
    OpenAPI_multiple_unit_information_t mui;
    OpenAPI_granted_unit_t granted;
    OpenAPI_final_unit_indication_t fui;
    OpenAPI_list_t *mui_list;
    char *invocation_time;
};

static OpenAPI_final_unit_action_e fua_to_openapi(int fua)
{
    switch (fua) {
    case 1:  return OpenAPI_final_unit_action_REDIRECT;
    case 2:  return OpenAPI_final_unit_action_RESTRICT_ACCESS;
    default: return OpenAPI_final_unit_action_TERMINATE;
    }
}

static OpenAPI_charging_data_response_t *build_charging_data_response(
        chf_sess_t *sess, int invocation_seq,
        struct chf_response_storage *st)
{
    uint64_t total_used, total_granted, remaining;
    uint32_t validity;

    ogs_assert(sess);
    ogs_assert(st);

    memset(st, 0, sizeof(*st));

    /* invocationTimeStamp and invocationSequenceNumber are mandatory
     * (TS 32.291 §7.2.1.2).  The sequence echoes the request's, so the
     * SMF can pair an answer with the report that asked for it. */
    st->invocation_time = ogs_sbi_gmtime_string(ogs_time_now());
    st->rsp.invocation_time_stamp = st->invocation_time;
    st->rsp.invocation_sequence_number = invocation_seq;

    total_used    = sess->ul_octets + sess->dl_octets;
    total_granted = sess->granted_quota_total;
    remaining     = (total_used >= total_granted)
                  ? 0ULL : total_granted - total_used;
    if (sess->quota_exempt)
        remaining = CHF_QUOTA_EXEMPT_GRANT_BYTES;

    /* Grant one slice of the remainder, not all of it.  The session's
     * ceiling (granted_quota_total) is unchanged, so exhaustion is still
     * judged against the whole allowance; the slice only decides how soon
     * the SMF reports back.  See chf_context_t.grant. */
    remaining = chf_grant_volume(remaining, chf_self()->grant.max_volume_bytes);

    /* ratingGroup — TS 32.291 §6.3.1.2 / §6.3.2.2 */
    st->mui.rating_group = (int)sess->rating_group;

    /* grantedUnit.totalVolume — remaining quota in bytes.  The generated
     * field is a long, which is why patched/ gives TS 32.291's Uint64
     * references an explicit int64 format: the default mapping is int,
     * and the shipped default quota is 10 GB. */
    st->granted.is_total_volume = true;
    st->granted.total_volume = (long)remaining;
    st->mui.granted_unit = &st->granted;

    /* volumeQuotaThreshold — when the SMF should report, as the volume
     * still left of this grant.  Always sent with a grant, 0 included, so
     * the SMF follows the CHF rather than its own default.
     * See chf_grant_threshold(). */
    if (remaining > 0) {
        st->mui.is_volume_quota_threshold = true;
        st->mui.volume_quota_threshold = (long)chf_grant_threshold(
                remaining, chf_self()->grant.min_report_bytes);
    }

    /* validityTime — see sess_validity_time(); 0 means omit it. */
    validity = sess_validity_time(sess);
    if (validity > 0) {
        st->mui.is_validity_time = true;
        st->mui.validity_time = (int)validity;
    }

    /* quotaHoldingTime — how long the session may hold this grant while
     * idle before reporting, so an unused grant comes back to the cycle
     * long before validityTime.  Only meaningful with a grant to hold.
     * See chf_grant_holding(). */
    if (remaining > 0) {
        uint32_t holding = chf_grant_holding(validity,
                chf_self()->grant.quota_holding_seconds);
        if (holding > 0) {
            st->mui.is_quota_holding_time = true;
            st->mui.quota_holding_time = (int)holding;
        }
    }

    /* finalUnitIndication — only present when quota is exhausted.
     * TS 32.291 §6.3.2.2: SMF must act on FinalUnitAction. */
    if (sess->final_unit) {
        st->fui.final_unit_action = fua_to_openapi(sess->final_unit_action);
        st->mui.final_unit_indication = &st->fui;
    }

    st->mui_list = OpenAPI_list_create();
    ogs_assert(st->mui_list);
    OpenAPI_list_add(st->mui_list, &st->mui);
    st->rsp.multiple_unit_information = st->mui_list;

    return &st->rsp;
}

static void build_charging_data_response_finalize(
        struct chf_response_storage *st)
{
    ogs_assert(st);
    if (st->mui_list) {
        OpenAPI_list_free(st->mui_list);
        st->mui_list = NULL;
    }
    if (st->invocation_time) {
        ogs_free(st->invocation_time);
        st->invocation_time = NULL;
    }
}

/* ------------------------------------------------------------------ */
/* Create                                                               */
/* TS 32.291 §6.3.1 — Nchf_ConvergedCharging_Create                   */
/* ------------------------------------------------------------------ */
bool chf_nchf_convergedcharging_handle_create(
        ogs_sbi_stream_t *stream,
        ogs_sbi_request_t *request, ogs_sbi_message_t *message)
{
    chf_sess_t *sess = NULL;
    ogs_sbi_server_t *server = NULL;
    ogs_sbi_message_t sendmsg;
    ogs_sbi_header_t header;
    ogs_sbi_response_t *response = NULL;

    char *supi = NULL;
    char *dnn  = NULL;
    char *gpsi = NULL;
    char *smf_id = NULL;      /* nfConsumerIdentification.nFName (the CDR) */
    char *smf_label = NULL;   /* a stable value for the metric label      */
    char *notify_uri = NULL;  /* notifyUri — where ChargingNotify goes     */
    int   pdu_session_id = 0;
    int   rating_group   = 1; /* default RG if not in body */
    ogs_s_nssai_t s_nssai = { .sst = 0, .sd.v = OGS_S_NSSAI_NO_SD_VALUE };
    char *hplmn   = NULL;     /* "MCC-MNC" from hPlmnId */
    char *roaming = NULL;     /* "home" | "inBound" | "outBound" */
    char  hplmn_buf[CHF_MAX_PLMN_STR_LEN];
    char  bucket_id[CHF_MAX_BUCKET_ID_LEN] = "";
    bool  bucket_shared = false;  /* the chosen bucket is a pool */

    char charging_data_ref[OGS_UUID_FORMATTED_LENGTH + 1];
    ogs_uuid_t uuid;

    chf_quota_t quota;
    int db_rv = OGS_RETRY;
    bool quota_exempt = false;

    OpenAPI_charging_data_request_t *recvreq = NULL;
    struct chf_response_storage rst;

    ogs_assert(stream);
    ogs_assert(request);
    ogs_assert(message);

    server = ogs_sbi_server_from_stream(stream);
    ogs_assert(server);

    /* -------------------------------------------------------------- */
    /* Parse ChargingDataRequest body (TS 32.291 §6.3.1.2)            */
    /* -------------------------------------------------------------- */
    recvreq = message->ChargingDataRequest;

    if (recvreq) {
        /* TS 32.291 §7.2.1.1: the field is "subscriberIdentifier" */
        if (recvreq->subscriber_identifier)
            supi = recvreq->subscriber_identifier;

        hplmn_buf[0] = '\0';
        parse_session_info(recvreq, &gpsi, &dnn, &pdu_session_id, &s_nssai,
                hplmn_buf, sizeof(hplmn_buf), &roaming);
        if (hplmn_buf[0])
            hplmn = hplmn_buf;
        if (!roaming)
            roaming = (char *)"home";   /* no roamerInOut: home network */

        /*
         * TS 32.291 §7.2.1 — nfConsumerIdentification, the SMF.
         *
         * nFName is an NfInstanceId, so it is what the CDR records as
         * NetworkFunctionName.  The metric label needs something stable
         * instead, because an instance id changes on every SMF start:
         * prefer the FQDN, then the addresses, and fall back to the id
         * only when the SMF sent nothing else.
         */
        if (recvreq->nf_consumer_identification) {
            OpenAPI_nf_identification_t *nfc =
                recvreq->nf_consumer_identification;

            if (nfc->n_f_name && *nfc->n_f_name)
                smf_id = nfc->n_f_name;

            if (nfc->n_f_fqdn && *nfc->n_f_fqdn)
                smf_label = nfc->n_f_fqdn;
            else if (nfc->n_fipv4_address && *nfc->n_fipv4_address)
                smf_label = nfc->n_fipv4_address;
            else if (nfc->n_fipv6_address && *nfc->n_fipv6_address)
                smf_label = nfc->n_fipv6_address;
            else
                smf_label = smf_id;
        }

        /* notifyUri — where a ChargingNotify for this session goes
         * (TS 32.291 §6.3.1.2).  Optional: an SMF that sends none can
         * only be reached by answering its reports. */
        if (recvreq->notify_uri && *recvreq->notify_uri)
            notify_uri = recvreq->notify_uri;

        /* First ratingGroup from multipleUnitUsage */
        if (recvreq->multiple_unit_usage) {
            OpenAPI_lnode_t *node = NULL;
            OpenAPI_list_for_each(recvreq->multiple_unit_usage, node) {
                OpenAPI_multiple_unit_usage_t *muu = node->data;
                if (muu)
                    rating_group = muu->rating_group;
                break;
            }
        }
    }

    if (!supi) {
        /*
         * Two different faults used to share one message.  A body that did
         * not parse at all reached here too, and was reported as "No SUPI"
         * - which sent anyone debugging it after the subscriber field when
         * the real cause was, for example, a missing nfConsumerIdentification.
         */
        const char *why = recvreq ? "No SUPI in ChargingDataRequest"
                                  : "Invalid or missing ChargingDataRequest";
        ogs_error("%s", why);
        chf_metrics_inst_labeled_inc(CHF_METR_LBL_ERRORS,
                recvreq ? "no_supi" : "parse", NULL, NULL);
        chf_metrics_inst_labeled_inc(CHF_METR_LBL_SESSION_CREATE,
                "reject", dnn, smf_label);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST, message, why, NULL, NULL));
        return false;
    }

    /*
     * Upstream's NFs all validate a SUPI before using it, with the same
     * check.  Nothing here would assert on a bad one, it would be billed:
     * "imsi-", or an empty string, drew the default quota and reached the
     * CDR as the subscriber.
     */
    if (ogs_supi_is_valid(supi) == false) {
        ogs_error("[%s] Invalid SUPI", supi);
        chf_metrics_inst_labeled_inc(CHF_METR_LBL_ERRORS,
                "bad_supi", NULL, NULL);
        chf_metrics_inst_labeled_inc(CHF_METR_LBL_SESSION_CREATE,
                "reject", dnn, smf_label);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST, message,
                "Invalid SUPI", NULL, NULL));
        return false;
    }

    /*
     * The model holds pduSessionID as an int and the session as a uint8_t,
     * so 256 would reach the CDR and chf.sessions as session 0, and 257 as
     * session 1.  pduSessionInformation itself is optional: with none there
     * is no identity to check, and the session records 0.
     */
    if (recvreq->p_du_session_charging_information &&
            recvreq->p_du_session_charging_information->
                pdu_session_information &&
            ogs_pdu_session_id_is_valid(pdu_session_id) == false) {
        ogs_error("[%s] Invalid pduSessionID [%d]", supi, pdu_session_id);
        chf_metrics_inst_labeled_inc(CHF_METR_LBL_ERRORS,
                "bad_psi", NULL, NULL);
        chf_metrics_inst_labeled_inc(CHF_METR_LBL_SESSION_CREATE,
                "reject", dnn, smf_label);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST, message,
                "Invalid pduSessionID", NULL, NULL));
        return false;
    }

    if (request_sst_is_valid(recvreq) == false) {
        chf_metrics_inst_labeled_inc(CHF_METR_LBL_ERRORS,
                "bad_sst", NULL, NULL);
        chf_metrics_inst_labeled_inc(CHF_METR_LBL_SESSION_CREATE,
                "reject", dnn, smf_label);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST, message,
                "Invalid sst", NULL, NULL));
        return false;
    }

    /* -------------------------------------------------------------- */
    /* Well-known DNNs are not quota-managed, whatever the slice or     */
    /* roaming state: no bucket is looked up, so an empty balance or    */
    /* charging.unprovisioned: reject cannot refuse an emergency call.  */
    /* -------------------------------------------------------------- */
    quota_exempt = ogs_sbi_dnn_is_well_known(dnn);
    if (quota_exempt) {
        memset(&quota, 0, sizeof(quota));
        chf_metrics_inst_labeled_inc(CHF_METR_LBL_QUOTA_LOOKUP,
                "exempt", NULL, NULL);
        goto session_setup;
    }

    /* -------------------------------------------------------------- */
    /* Pick the bucket this session draws from.                        */
    /*                                                                  */
    /* A subscriber can hold several allowances - per slice, DNN, home  */
    /* PLMN or roaming state - so the one to charge is chosen from what */
    /* the request says about the session.  With no bucket at all the   */
    /* configured default quota applies, as before buckets existed.     */
    /* TS 32.291 §6.3.1.2 — GrantedUnit in response                   */
    /* -------------------------------------------------------------- */
    {
        chf_bucket_t buckets[CHF_MAX_BUCKETS_PER_SUPI];
        chf_bucket_key_t key;
        int num, chosen;

        memset(&key, 0, sizeof(key));
        key.supi    = supi;
        key.s_nssai = s_nssai;
        key.dnn     = dnn;
        key.hplmn   = hplmn;
        key.roaming = roaming;

        num = chf_db_load_buckets(supi, buckets, CHF_MAX_BUCKETS_PER_SUPI);
        if (num == OGS_ERROR) {
            db_rv = OGS_ERROR;      /* MongoDB unreadable */
        } else if (num == 0) {
            db_rv = OGS_RETRY;      /* SUPI not provisioned */
        } else {
            chosen = chf_bucket_select(buckets, num, &key);
            if (chosen < 0) {
                /*
                 * Provisioned, but no bucket covers this session - a
                 * subscriber with a slice-only plan attaching on another
                 * slice.  That is a provisioning gap rather than an empty
                 * balance, so it is treated like an unprovisioned SUPI and
                 * follows charging.unprovisioned.
                 */
                ogs_warn("[%s] %d bucket(s), none for this session "
                         "[dnn:%s sst:%u roaming:%s]", supi, num,
                         dnn ? dnn : "-", s_nssai.sst,
                         roaming ? roaming : "-");
                db_rv = OGS_RETRY;
            } else {
                quota = buckets[chosen].quota;
                ogs_cpystrn(bucket_id, buckets[chosen].id,
                        sizeof(bucket_id));
                bucket_shared = buckets[chosen].shared;
                db_rv = OGS_OK;
                if (bucket_shared)
                    ogs_info("[%s] Pool [%s] charged "
                             "[priority:%d of %d buckets]", supi,
                             bucket_id, buckets[chosen].priority, num);
                else if (num > 1)
                    ogs_info("[%s] Bucket [%s] charged "
                             "[priority:%d of %d buckets]", supi,
                             bucket_id, buckets[chosen].priority, num);
            }
        }
    }

    chf_metrics_inst_labeled_inc(CHF_METR_LBL_QUOTA_LOOKUP,
            db_rv == OGS_OK ? "db" : "default", NULL, NULL);

    /*
     * Which allowance was charged, and on what slice.  The whole point of
     * per-slice buckets is that a scoped one wins over the catch-all, and
     * until now the only way to confirm that in a running network was to
     * read the CHF log.  "kind" distinguishes a scoped bucket from the
     * catch-all (whose id is the SUPI) from the configured default.
     *
     * The slice label is bounded by the number of slices a deployment
     * runs, which is what keeps this safe to label on.
     */
    {
        char slice_label[CHF_METR_LABEL_LEN];
        const char *kind;

        if (db_rv != OGS_OK)
            kind = "default";
        else if (bucket_shared)
            kind = "shared";
        else if (bucket_id[0] && strcmp(bucket_id, supi) != 0)
            kind = "scoped";
        else
            kind = "catchall";

        if (!s_nssai.sst)
            ogs_cpystrn(slice_label, "none", sizeof(slice_label));
        else if (s_nssai.sd.v == OGS_S_NSSAI_NO_SD_VALUE)
            ogs_snprintf(slice_label, sizeof(slice_label), "%u",
                    s_nssai.sst);
        else
            ogs_snprintf(slice_label, sizeof(slice_label), "%u/%06x",
                    s_nssai.sst, s_nssai.sd.v);

        chf_metrics_inst_labeled_inc(CHF_METR_LBL_BUCKET_SELECTED,
                kind, slice_label, NULL);
    }
    if (db_rv == OGS_RETRY && chf_self()->reject_unprovisioned) {
        ogs_warn("[%s] Not provisioned in chf.subscribers — Create rejected "
                 "(charging.unprovisioned: reject)", supi);
        chf_metrics_inst_labeled_inc(CHF_METR_LBL_SESSION_CREATE,
                "reject", dnn, smf_label);
        /* TS 32.291 §6.1.7.3 — USER_UNKNOWN */
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream,
                OGS_SBI_HTTP_STATUS_NOT_FOUND, message,
                "Subscriber not provisioned", supi, "USER_UNKNOWN"));
        return false;
    }
    if (db_rv != OGS_OK) {
        /*
         * Not provisioned, or MongoDB could not be read: grant the
         * configured default.  Say so at WARNING - this is otherwise a
         * normal-looking Create, and a SUPI that was meant to be on a plan
         * (a typo, or deleted and not yet re-added) silently gets the
         * default allowance instead.  A DB failure is not rejected even
         * with charging.unprovisioned: reject, since it says nothing about
         * the SUPI and would refuse every subscriber during an outage.
         */
        quota = chf_self()->default_quota;
        ogs_warn("[%s] %s — granting the default quota "
                 "[%llu bytes / %u s]", supi,
                 db_rv == OGS_RETRY ? "Not provisioned in chf.subscribers"
                                    : "Subscriber lookup failed",
                 (unsigned long long)quota.volume_bytes,
                 quota.validity_seconds);
    }

    /* -------------------------------------------------------------- */
    /* Billing cycle bookkeeping for a provisioned subscriber.         */
    /*                                                                  */
    /* The grant for this session is whatever is LEFT of the cycle      */
    /* allowance, not the full allowance: consumed_bytes already covers */
    /* every earlier session, including ones that have since closed.    */
    /* -------------------------------------------------------------- */
    if (db_rv == OGS_OK) {
        time_t now = time(NULL);

        if (!quota.period_start) {
            /* First ever session for this subscriber — open the cycle. */
            chf_db_start_usage_period(bucket_id);
            quota.period_start   = now;
            quota.consumed_bytes = 0;
        } else if (chf_quota_period_expired(&quota, now)) {
            if (quota.auto_renew) {
                ogs_info("[%s] Cycle expired at Create — renewing "
                         "[consumed:%llu quota:%llu]", supi,
                         (unsigned long long)quota.consumed_bytes,
                         (unsigned long long)quota.volume_bytes);
                chf_db_start_usage_period(bucket_id);
                quota.period_start   = now;
                quota.consumed_bytes = 0;
                quota.topup_bytes    = 0;
                chf_metrics_inst_global_inc(CHF_METR_CTR_QUOTA_AUTO_RENEW);
            } else {
                /* Expired with no renewal: nothing left to grant. */
                ogs_warn("[%s] Cycle expired at Create and auto-renew is off "
                         "— granting 0", supi);
                quota.consumed_bytes = chf_quota_allowance(&quota);
            }
        }

        quota.volume_bytes = chf_quota_remaining(&quota);
    }

    /* -------------------------------------------------------------- */
    /* TS 32.291 §5.2 — multi-session cap for UNPROVISIONED subscribers */
    /* -------------------------------------------------------------- */
    if (db_rv != OGS_OK) {
        /*
         * No provisioned counter for this SUPI, so the only record of what
         * its other sessions have used is the sessions themselves.  Deduct
         * the live ones so concurrent sessions still share one allowance.
         *
         * Provisioned subscribers skip this: quota.volume_bytes above is
         * already the cycle remainder, and consumed_bytes covers the live
         * sessions too, so deducting again would double-count.
         *
         * Called before chf_sess_add() so the new session is not yet in
         * sess_list and is not counted.
         */
        uint64_t agg_used = chf_supi_aggregate_used(supi);
        if (agg_used > 0) {
            if (agg_used >= quota.volume_bytes) {
                ogs_warn("[%s] Create: quota fully consumed by concurrent "
                         "sessions [total:%llu agg_used:%llu] — "
                         "granting 0 with FUI",
                         supi,
                         (unsigned long long)quota.volume_bytes,
                         (unsigned long long)agg_used);
                quota.volume_bytes = 0;
            } else {
                ogs_info("[%s] Create: concurrent sessions used %llu bytes "
                         "— capping new grant to %llu (of %llu total)",
                         supi,
                         (unsigned long long)agg_used,
                         (unsigned long long)(quota.volume_bytes - agg_used),
                         (unsigned long long)quota.volume_bytes);
                quota.volume_bytes -= agg_used;
            }
        }
    }

session_setup:
    /* -------------------------------------------------------------- */
    /* Generate ChargingDataRef UUID and create in-memory session      */
    /* -------------------------------------------------------------- */
    ogs_uuid_get(&uuid);
    ogs_uuid_format(charging_data_ref, &uuid);

    sess = chf_sess_add(supi, charging_data_ref);
    if (!sess) {
        ogs_error("[%s] Cannot add CHF session", supi);
        chf_metrics_inst_labeled_inc(CHF_METR_LBL_ERRORS,
                "session_add", NULL, NULL);
        chf_metrics_inst_labeled_inc(CHF_METR_LBL_SESSION_CREATE,
                "reject", dnn, smf_label);
        /* TS 29.500 Table 5.2.7.2-1, as upstream's NFs now answer it */
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream,
                OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR, message,
                "Insufficient resources", "Cannot add CHF session",
                "INSUFFICIENT_RESOURCES"));
        return false;
    }

    /* Populate session identity fields */
    if (gpsi)
        sess->gpsi = ogs_strdup(gpsi);
    if (dnn)
        sess->dnn = ogs_strdup(dnn);
    if (smf_id)
        sess->smf_id = ogs_strdup(smf_id);
    if (smf_label)
        sess->smf_label = ogs_strdup(smf_label);
    if (notify_uri) {
        sess->notify_uri = ogs_strdup(notify_uri);
        ogs_assert(sess->notify_uri);
    }
    sess->psi          = (uint8_t)pdu_session_id;
    sess->rating_group = (uint32_t)rating_group;
    sess->s_nssai      = s_nssai;
    ogs_cpystrn(sess->bucket_id, bucket_id, sizeof(sess->bucket_id));
    sess->provisioned  = (db_rv == OGS_OK);

    if (quota_exempt) {
        sess->quota_exempt = true;
        sess->provisioned  = false;
        sess->period_start = time(NULL);    /* the CDR's start time */
        goto session_persist;
    }

    /* Apply quota — sets granted_quota_total, granted_time_sec,
     * final_unit_action, final_unit=false
     * TS 32.291 §6.3.1 — quota initialisation */
    chf_apply_quota(sess, &quota);

    /*
     * Re-anchor on the subscriber's cycle.
     *
     * chf_apply_quota() stamps period_start = now, which is right for an
     * unprovisioned SUPI whose only clock is the session itself.  A
     * provisioned subscriber's cycle started earlier, and validityTime in
     * the response is computed from period_start - so leaving it at "now"
     * would promise the UPF a full period from session start and push its
     * TIME_LIMIT trigger past the real cycle end.
     */
    if (db_rv == OGS_OK && quota.period_start)
        sess->period_start = quota.period_start;

    /* If the aggregate cap reduced the grant to zero, signal FUI now.
     * chf_apply_quota() always clears final_unit; set it after the call. */
    if (sess->granted_quota_total == 0) {
        sess->final_unit = true;
        chf_metrics_inst_labeled_inc(CHF_METR_LBL_QUOTA_FINAL_UNIT,
                chf_fua_metric_label(sess->final_unit_action), NULL, NULL);
        ogs_warn("[%s] Create: zero quota — FUI set immediately [ref:%s]",
                 sess->supi, sess->charging_data_ref);
    }

session_persist:
    /* -------------------------------------------------------------- */
    /* Persist to MongoDB (non-fatal: CHF works in memory-only mode)   */
    /* -------------------------------------------------------------- */
    chf_db_session_create(sess);

    /* -------------------------------------------------------------- */
    /* Build 201 Created response                                       */
    /* TS 32.291 §6.3.1.2 — Location header + ChargingDataResponse    */
    /* -------------------------------------------------------------- */
    memset(&header, 0, sizeof(header));
    header.service.name =
        (char *)OGS_SBI_SERVICE_NAME_NCHF_CONVERGEDCHARGING;
    header.api.version  = (char *)CHF_API_VERSION;
    header.resource.component[0] = (char *)CHF_RESOURCE_NAME_CHARGINGDATA;
    header.resource.component[1] = sess->charging_data_ref;

    memset(&sendmsg, 0, sizeof(sendmsg));
    sendmsg.http.location = ogs_sbi_server_uri(server, &header);

    sendmsg.ChargingDataResponse = build_charging_data_response(sess,
            message->ChargingDataRequest ?
                message->ChargingDataRequest->invocation_sequence_number : 0,
            &rst);

    response = ogs_sbi_build_response(&sendmsg, OGS_SBI_HTTP_STATUS_CREATED);
    ogs_assert(response);

    build_charging_data_response_finalize(&rst);

    ogs_assert(true == ogs_sbi_server_send_response(stream, response));

    ogs_free(sendmsg.http.location);

    chf_metrics_inst_labeled_inc(CHF_METR_LBL_SESSION_CREATE, "success",
            sess->dnn, sess->smf_label);

    if (quota_exempt)
        ogs_info("[%s] Nchf Create OK [ref:%s dnn:%s not quota-managed]",
                sess->supi, sess->charging_data_ref, sess->dnn);
    else
        ogs_info("[%s] Nchf Create OK [ref:%s granted:%llu of %llu bytes "
                 "validity:%us%s]",
                sess->supi, sess->charging_data_ref,
                (unsigned long long)chf_grant_volume(quota.volume_bytes,
                    chf_self()->grant.max_volume_bytes),
                (unsigned long long)quota.volume_bytes,
                sess_validity_time(sess),
                sess->provisioned ? "" : " default-quota");

    return true;
}

/* ------------------------------------------------------------------ */
/* Update                                                               */
/* TS 32.291 §6.3.2 — Nchf_ConvergedCharging_Update                   */
/* ------------------------------------------------------------------ */
bool chf_nchf_convergedcharging_handle_update(
        chf_sess_t *sess,
        ogs_sbi_stream_t *stream,
        ogs_sbi_request_t *request, ogs_sbi_message_t *message)
{
    ogs_sbi_response_t *response = NULL;
    ogs_sbi_message_t sendmsg;
    struct chf_response_storage rst;

    uint64_t delta_ul  = 0;
    uint64_t delta_dl  = 0;
    uint32_t delta_dur = 0;

    ogs_assert(sess);
    ogs_assert(stream);
    ogs_assert(request);
    ogs_assert(message);

    /*
     * A body that failed to parse arrives here with no ChargingDataRequest:
     * lib/sbi logs the parse error and dispatches the request anyway.
     * parse_used_units() treats that as zero usage, so this handler used to
     * answer 200, grant a fresh slice and drop whatever usage the SMF had
     * reported - silent under-billing for any peer whose body the model
     * rejects, such as one omitting a member R19 makes mandatory.  Refuse it
     * instead (TS 29.500 §5.2.7.2): the SMF learns the report was not
     * accepted, and nothing is granted against usage nobody has counted.
     */
    if (!message->ChargingDataRequest) {
        ogs_error("[%s] Update with no parseable ChargingDataRequest - "
                  "refused, usage not counted [ref:%s]",
                sess->supi, sess->charging_data_ref);
        chf_metrics_inst_labeled_inc(CHF_METR_LBL_ERRORS,
                "parse", NULL, NULL);
        chf_metrics_inst_labeled_inc(CHF_METR_LBL_SESSION_UPDATE,
                "reject", NULL, NULL);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST, message,
                "Invalid or missing ChargingDataRequest", NULL, NULL));
        return false;
    }

    /* Parse ChargingDataRequest body (TS 32.291 §6.3.2.2) */
    parse_used_units(message->ChargingDataRequest,
            &delta_ul, &delta_dl, &delta_dur);

    /* Accumulate into session counters
     * TS 32.291 §6.3.2.2 — usedUnitContainer accumulation */
    sess->ul_octets += delta_ul;
    sess->dl_octets += delta_dl;
    sess->duration  += delta_dur;

    /*
     * Add this report to the subscriber's billing-cycle counter before the
     * quota re-read below, so the consumed_bytes that comes back already
     * includes it and enforcement sees one consistent number.
     *
     * OGS_RETRY here means the SUPI is not provisioned; that case is
     * accounted per-session instead (see the enforcement block).
     *
     * A quota-exempt session has no bucket: chf_sess_bucket() would fall
     * back to the SUPI and charge the subscriber's catch-all.
     */
    if (!sess->quota_exempt) {
        chf_db_add_usage(chf_sess_bucket(sess), delta_ul + delta_dl);

        /* Reported usage this Update, kilobytes.  Clamped: the volumes come
         * from the peer's JSON, and an overflowing cast to int would trip
         * the ogs_metrics counter's val >= 0 assertion and abort the NF. */
        chf_metrics_inst_global_add(CHF_METR_CTR_QUOTA_USED_KB,
                chf_metrics_clamp_u64((delta_ul + delta_dl) / 1024));
    }

    {
        const char *trigger = parse_trigger(message->ChargingDataRequest);

        ogs_info("[%s] Nchf Update [ref:%s +ul:%llu +dl:%llu +dur:%u "
                 "total ul:%llu dl:%llu dur:%u trigger:%s]",
                sess->supi, sess->charging_data_ref,
                (unsigned long long)delta_ul, (unsigned long long)delta_dl,
                delta_dur,
                (unsigned long long)sess->ul_octets,
                (unsigned long long)sess->dl_octets,
                sess->duration,
                trigger ? trigger : "-");
    }

    if (sess->quota_exempt)
        goto send_response;

    /* -------------------------------------------------------------- */
    /* Quota enforcement (every Update)                                */
    /*                                                                  */
    /* One MongoDB read per Update serves several purposes:            */
    /*                                                                  */
    /*  Period expiry / auto-renew (F):                                */
    /*   F) The UPF sent this Update because validityTime expired.     */
    /*      If auto_renew is true: reset accumulated usage counters    */
    /*      and re-grant the full quota for a new period.              */
    /*      If auto_renew is false: exhaust the session (FUI).         */
    /*                                                                  */
    /*  Admin-change cases (A–D):                                      */
    /*   A) Admin increased quota, session not yet exhausted:          */
    /*      new_total > old_total → chf_apply_quota() raises the ceiling.  */
    /*   B) Admin decreased quota, session not yet exhausted:          */
    /*      new_total < old_total → may exhaust immediately.           */
    /*   C) Admin recharged after exhaustion:                          */
    /*      new_total > used → clear final_unit, grant remaining.      */
    /*   D) No admin change:                                            */
    /*      new_total == old_total → normal per-session check.         */
    /*                                                                  */
    /*  E) Cross-session aggregate exhaustion (TS 32.291 §5.2):       */
    /*      If the per-session FUI has not fired, check whether the    */
    /*      aggregate usage across all concurrent sessions for this    */
    /*      SUPI has reached the subscriber's total quota.             */
    /*                                                                  */
    /* If DB is unavailable, total_volume falls back to the in-memory  */
    /* per-session grant (pre-Phase-1 baseline behaviour).             */
    /* -------------------------------------------------------------- */
    {
        chf_quota_t fresh_quota;
        uint64_t old_total    = sess->granted_quota_total;
        uint64_t total_volume;
        bool     got_fresh;
        bool     member = true;
        int      fresh_rv;

        if (sess->bucket_id[0]) {
            fresh_rv = chf_db_get_bucket_quota(sess->bucket_id, sess->supi,
                            &fresh_quota, &member);
        } else {
            /* No bucket at Create.  If the SIM has since joined a pool that
             * covers every session, this session moves onto it now. */
            char joined[CHF_MAX_BUCKET_ID_LEN];
            fresh_rv = chf_db_get_catchall_quota(sess->supi, &fresh_quota,
                            joined, sizeof(joined));
            if (fresh_rv == OGS_OK && joined[0]) {
                ogs_cpystrn(sess->bucket_id, joined, sizeof(sess->bucket_id));
                ogs_info("[%s] Joined pool [%s] mid-session — charged to it "
                         "from now on [ref:%s]", sess->supi, sess->bucket_id,
                         sess->charging_data_ref);
            }
        }

        if (fresh_rv == OGS_OK && !member) {
            /*
             * The SIM was taken out of this session's pool, or the pool
             * now has more members than charging.pool.max_members and it
             * is past the limit.  Its sessions are to be released at once:
             * the WebUI queues an ABORT_CHARGING for that, and this ends a
             * session the notification did not reach, at its next report.
             * The usage it just reported was granted from the pool, and has
             * been counted to it above.
             */
            ogs_warn("[%s] No longer a member of pool [%s] — ending the "
                     "session [ref:%s]", sess->supi, sess->bucket_id,
                     sess->charging_data_ref);
            sess->granted_quota_total = sess->ul_octets + sess->dl_octets;
            sess->final_unit_action = 0;    /* TERMINATE */
            sess->final_unit = true;
            sess->aborted = true;           /* the CDR: managementIntervention */
            chf_metrics_inst_labeled_inc(CHF_METR_LBL_QUOTA_FINAL_UNIT,
                    chf_fua_metric_label(sess->final_unit_action),
                    NULL, NULL);
            goto send_response;
        }

        got_fresh    = (fresh_rv == OGS_OK);
        total_volume = got_fresh ? chf_quota_allowance(&fresh_quota)
                     : chf_self()->default_quota.volume_bytes;

        if (got_fresh && !sess->provisioned) {
            /* Provisioned after this session started on the default quota:
             * from here on it is charged to the plan (ceiling re-based
             * below). */
            ogs_info("[%s] Provisioned mid-session — moving to the plan "
                     "[ref:%s]", sess->supi, sess->charging_data_ref);
            sess->provisioned = true;
        }
        if (got_fresh && !fresh_quota.period_start) {
            /*
             * A live session but no open cycle: the SUPI was provisioned,
             * or deleted and re-added, while this session was up.  Open the
             * cycle now - otherwise it would stay unopened, never expire and
             * read as "not started" until the next Create.  The counter is
             * kept: it already holds this report, and nothing from before
             * the document existed.
             */
            chf_db_open_usage_period(chf_sess_bucket(sess));
            fresh_quota.period_start = time(NULL);
        }

        /* Case F: billing cycle expired ----------------------------------
         *
         * For a provisioned subscriber the cycle is anchored on
         * usage.periodStart, which spans sessions; the per-session
         * period_start is only a fallback for unprovisioned SUPIs. */
        if (got_fresh ? chf_quota_period_expired(&fresh_quota, time(NULL))
                      : chf_check_period_expired(sess)) {
            if (sess->auto_renew && !got_fresh && sess->provisioned) {
                /*
                 * Provisioned, cycle over, but the plan cannot be read
                 * (MongoDB unavailable, or the plan was deleted).
                 * Renewing now would mean granting the configured default
                 * quota (the real profile cannot be read) and clearing
                 * final_unit, while the persisted counter could not be
                 * reset - so the new cycle would start from the old total
                 * the moment the database returns.  Defer instead: the
                 * expiry is time-based and is detected again on the next
                 * Update, which renews properly once the read succeeds.
                 * Until then the existing grant keeps being enforced.
                 */
                ogs_warn("[%s] Billing cycle expired but the plan %s "
                         "— deferring auto-renew [ref:%s]", sess->supi,
                         fresh_rv == OGS_RETRY ? "was removed"
                                               : "could not be read",
                         sess->charging_data_ref);
            } else if (sess->auto_renew) {
                ogs_info("[%s] Billing cycle expired — auto-renewing "
                         "[ref:%s consumed:%llu quota:%llu]",
                         sess->supi, sess->charging_data_ref,
                         (unsigned long long)(got_fresh ?
                             fresh_quota.consumed_bytes :
                             sess->ul_octets + sess->dl_octets),
                         (unsigned long long)total_volume);

                if (got_fresh) {
                    /* Zero the persisted counter and re-anchor the cycle,
                     * otherwise Case E below would exhaust immediately on
                     * the carried-over consumed_bytes. */
                    chf_db_start_usage_period(chf_sess_bucket(sess));
                    fresh_quota.consumed_bytes = 0;
                    fresh_quota.topup_bytes    = 0;
                    fresh_quota.period_start   = time(NULL);
                }

                sess->ul_octets = 0;
                sess->dl_octets = 0;
                chf_apply_quota(sess, got_fresh ? &fresh_quota
                                    : &chf_self()->default_quota);
                if (got_fresh)
                    sess->period_start = fresh_quota.period_start;
                chf_metrics_inst_global_inc(CHF_METR_CTR_QUOTA_AUTO_RENEW);
                /* Fall through: the checks below now see zero used. */
            } else {
                ogs_info("[%s] Billing cycle expired — exhausting "
                         "(no auto-renew) [ref:%s]",
                         sess->supi, sess->charging_data_ref);
                sess->final_unit = true;
                chf_metrics_inst_labeled_inc(CHF_METR_LBL_QUOTA_FINAL_UNIT,
                        chf_fua_metric_label(sess->final_unit_action),
                        NULL, NULL);
                goto send_response;
            }
        }

        if (got_fresh) {
            /*
             * Re-base this session's ceiling on the cycle counter.
             *
             * granted_quota_total is what build_charging_data_response()
             * measures the session's own usage against, so setting it to
             * "usage so far + what the cycle has left" makes the response
             * grant exactly the cycle remainder.
             *
             * This also subsumes most of the old mid-session admin-change
             * handling: raising or lowering totalQuotaBytes moves the
             * remainder, and the next Update picks it up.  The one thing it
             * does not do on its own is Case C - reviving a session that had
             * already exhausted - because final_unit is set-only and would
             * keep a FinalUnitIndication on every response after the
             * recharge.  chf_quota_revive() below handles that.
             */
            sess->granted_quota_total = (sess->ul_octets + sess->dl_octets)
                                      + chf_quota_remaining(&fresh_quota);
            sess->final_unit_action = fresh_quota.final_unit_action;
            sess->auto_renew        = fresh_quota.auto_renew;
            /* Keep validityTime counting down to the real cycle end. */
            if (fresh_quota.period_start)
                sess->period_start = fresh_quota.period_start;
            sess->period_length_sec = fresh_quota.validity_seconds;

            if (sess->granted_quota_total != old_total)
                ogs_debug("[%s] Cycle ceiling re-based [ref:%s old:%llu "
                          "new:%llu consumed:%llu of %llu]",
                        sess->supi, sess->charging_data_ref,
                        (unsigned long long)old_total,
                        (unsigned long long)sess->granted_quota_total,
                        (unsigned long long)fresh_quota.consumed_bytes,
                        (unsigned long long)fresh_quota.volume_bytes);

            /* Case C: recharged after exhaustion.  Must come after the
             * re-base, so it sees the new ceiling, and before the checks
             * below, which only ever set final_unit. */
            chf_quota_revive(sess, fresh_quota.consumed_bytes, total_volume);

        } else if (sess->provisioned) {
            /*
             * Provisioned at Create, but the re-read found no plan: either
             * MongoDB is unavailable, or the plan was deleted while this
             * session was up.  Either way keep the last known ceiling.
             * Falling through to the default-quota branch below would call
             * chf_apply_quota(), which clears final_unit - reviving an
             * exhausted subscriber on the default allowance.  The per-
             * session check below still enforces the existing ceiling, and
             * the unprovisioned policy applies from the next Create.
             */
            if (fresh_rv == OGS_RETRY)
                ogs_warn("[%s] Plan removed from chf.subscribers while the "
                         "session is up — keeping its last limit until it "
                         "ends; usage is no longer counted to a plan "
                         "[ref:%s]", sess->supi, sess->charging_data_ref);
            else
                ogs_warn("[%s] Quota re-read failed for a provisioned "
                         "subscriber — keeping the last grant [ref:%s]",
                         sess->supi, sess->charging_data_ref);

        } else if (chf_self()->default_quota.volume_bytes != old_total) {
            /* Unprovisioned: the only ceiling available is the configured
             * default, re-applied if it has moved since this session
             * started (a config reload). */
            chf_apply_quota(sess, &chf_self()->default_quota);
        }

        /* Per-session exhaustion (TS 32.291 §6.3.2.2) */
        chf_check_quota_exhausted(sess);

        /* Case E: cycle allowance exhausted (TS 32.291 §5.2)
         *
         * For a provisioned subscriber the authority is the persisted
         * counter: it spans every session of the cycle, including ones that
         * have already closed.  Without a provisioned document there is
         * nothing persisted, so fall back to summing the live sessions -
         * which is all the information that exists for that SUPI. */
        if (!sess->final_unit) {
            uint64_t agg_used = got_fresh
                              ? fresh_quota.consumed_bytes
                              : chf_supi_aggregate_used(sess->supi);
            if (agg_used >= total_volume) {
                ogs_warn("[%s] Cycle quota exhausted [ref:%s total:%llu "
                         "used:%llu source:%s]",
                         sess->supi, sess->charging_data_ref,
                         (unsigned long long)total_volume,
                         (unsigned long long)agg_used,
                         got_fresh ? "counter" : "live-sessions");
                sess->final_unit = true;
                chf_metrics_inst_labeled_inc(CHF_METR_LBL_QUOTA_EXHAUSTED,
                        "aggregate", sess->dnn, NULL);
                chf_metrics_inst_labeled_inc(CHF_METR_LBL_QUOTA_FINAL_UNIT,
                        chf_fua_metric_label(sess->final_unit_action),
                        NULL, NULL);
            }
        }
    }

send_response:

    chf_metrics_inst_labeled_inc(CHF_METR_LBL_SESSION_UPDATE,
            "success", NULL, NULL);

    /* -------------------------------------------------------------- */
    /* Persist updated usage to MongoDB                                 */
    /* -------------------------------------------------------------- */
    chf_db_session_update(sess);

    /* -------------------------------------------------------------- */
    /* Build 200 OK response with remaining GrantedUnit                */
    /* TS 32.291 §6.3.2.2 — ChargingDataResponse                      */
    /* -------------------------------------------------------------- */
    memset(&sendmsg, 0, sizeof(sendmsg));

    sendmsg.ChargingDataResponse = build_charging_data_response(sess,
            message->ChargingDataRequest ?
                message->ChargingDataRequest->invocation_sequence_number : 0,
            &rst);

    response = ogs_sbi_build_response(&sendmsg, OGS_SBI_HTTP_STATUS_OK);
    ogs_assert(response);

    build_charging_data_response_finalize(&rst);

    ogs_assert(true == ogs_sbi_server_send_response(stream, response));

    return true;
}

/* ------------------------------------------------------------------ */
/* Release                                                              */
/* TS 32.291 §6.3.3 — Nchf_ConvergedCharging_Release                  */
/* ------------------------------------------------------------------ */
bool chf_nchf_convergedcharging_handle_release(
        chf_sess_t *sess,
        ogs_sbi_stream_t *stream,
        ogs_sbi_request_t *request, ogs_sbi_message_t *message)
{
    ogs_sbi_response_t *response = NULL;
    ogs_sbi_message_t sendmsg;

    uint64_t final_ul  = 0;
    uint64_t final_dl  = 0;
    uint32_t final_dur = 0;

    ogs_assert(sess);
    ogs_assert(stream);
    ogs_assert(request);
    ogs_assert(message);

    /*
     * A body that did not parse still releases the session - unlike Update,
     * refusing a Release would leave a CHF session the SMF has already let
     * go of, with nothing that will ever end it.  But it is not silent any
     * more: the final interval cannot be billed, and an operator needs to
     * see that rather than find a short CDR.
     */
    if (!message->ChargingDataRequest) {
        ogs_error("[%s] Release with no parseable ChargingDataRequest - "
                  "releasing, but the final usage report is lost [ref:%s]",
                sess->supi, sess->charging_data_ref);
        chf_metrics_inst_labeled_inc(CHF_METR_LBL_ERRORS,
                "parse", NULL, NULL);
    }

    /* Parse the final usage report if present (TS 32.291 §6.3.3.2) */
    parse_used_units(message->ChargingDataRequest,
            &final_ul, &final_dl, &final_dur);
    sess->ul_octets += final_ul;
    sess->dl_octets += final_dl;
    sess->duration  += final_dur;

    /* The closing report counts against the cycle like any other: this is
     * the last chance to record it, since the session row is deleted below
     * and its octets would otherwise vanish from the subscriber's total. */
    if (!sess->quota_exempt)
        chf_db_add_usage(chf_sess_bucket(sess), final_ul + final_dl);

    chf_metrics_inst_labeled_inc(CHF_METR_LBL_SESSION_RELEASE,
            sess->final_unit ? "volume_limit" : "normal", NULL, NULL);

    ogs_info("[%s] Nchf Release [ref:%s final ul:%llu dl:%llu dur:%u]",
            sess->supi, sess->charging_data_ref,
            (unsigned long long)sess->ul_octets,
            (unsigned long long)sess->dl_octets,
            sess->duration);

    /* -------------------------------------------------------------- */
    /* Write CDR JSON file (TS 23.255 §7 / TS 32.298 §5.1.1)         */
    /* Then insert the CDR index entry in MongoDB with the file path.  */
    /* -------------------------------------------------------------- */
    if (sess->quota_exempt && !chf_self()->well_known_dnn_cdr) {
        ogs_info("[%s] No CDR: charging.well_known_dnn.cdr is off [ref:%s]",
                sess->supi, sess->charging_data_ref);
    } else if (sess->quota_exempt || chf_cdr_is_billable(sess)) {
        /* TS 32.298 §5.1.1.5.2 CauseForRecClosing.  The abort comes first:
         * when the operator closed the session, that is what ended it, and
         * an exhausted session would have been gone already. */
        const char *cause = sess->aborted   ? "managementIntervention"
                          : sess->final_unit ? "volumeLimit"
                                             : "normalRelease";
        char *cdr_file = chf_cdr_write(sess, cause);
        chf_db_cdr_insert(sess, cdr_file); /* cdr_file may be NULL */
        if (cdr_file) ogs_free(cdr_file);
    } else {
        ogs_info("[%s] No CDR: nothing granted and nothing used [ref:%s]",
                sess->supi, sess->charging_data_ref);
    }
    chf_db_session_delete(sess);

    /* -------------------------------------------------------------- */
    /* Build 204 No Content response (TS 32.291 §6.3.3.2)             */
    /* -------------------------------------------------------------- */
    memset(&sendmsg, 0, sizeof(sendmsg));
    response = ogs_sbi_build_response(&sendmsg, OGS_SBI_HTTP_STATUS_NO_CONTENT);
    ogs_assert(response);
    ogs_assert(true == ogs_sbi_server_send_response(stream, response));

    chf_sess_remove(sess);

    return true;
}
