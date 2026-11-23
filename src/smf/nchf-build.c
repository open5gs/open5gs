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
 * SMF Nchf_ConvergedCharging request builders.
 *
 * The bodies are the generated TS 32.291 models (lib/sbi/openapi/model/
 * charging_*), filled here and serialised by ogs_sbi_build_request() the
 * way every other service does it.  The structs are on the stack; only
 * lists, and the few strings the helpers allocate, need freeing.
 *
 * 3GPP references:
 *   TS 32.291 §6.3.1 — Nchf_ConvergedCharging_Create
 *   TS 32.291 §6.3.2 — Nchf_ConvergedCharging_Update
 *   TS 32.291 §6.3.3 — Nchf_ConvergedCharging_Release
 *   TS 32.291 §7     — ChargingDataRequest schema
 */

#include "nchf-build.h"

/* CHF API version string (v3 per TS 32.291).
 * OGS_SBI_API_V1/V2 exist but not V3 — define locally. */
#define SMF_NCHF_API_VERSION    "v3"

/* ------------------------------------------------------------------ */
/* Internal helpers                                                     */
/* ------------------------------------------------------------------ */

/*
 * build_message_header() — populate the ogs_sbi_message_t header fields
 * common to all three CHF operations.
 *
 * component[0] = "chargingdata"
 * component[1] = ChargingDataRef (NULL for Create)
 * component[2] = "update" | "release" | NULL
 */
static void build_message_header(
        ogs_sbi_message_t *message,
        const char *component1,
        const char *component2)
{
    ogs_assert(message);

    memset(message, 0, sizeof(*message));
    message->h.method       = (char *)OGS_SBI_HTTP_METHOD_POST;
    message->h.service.name =
        (char *)OGS_SBI_SERVICE_NAME_NCHF_CONVERGEDCHARGING;
    message->h.api.version  = (char *)SMF_NCHF_API_VERSION;

    message->h.resource.component[0] =
        (char *)OGS_SBI_RESOURCE_NAME_CHARGINGDATA;
    message->h.resource.component[1] = (char *)component1;
    message->h.resource.component[2] = (char *)component2;
}

/*
 * Where fill_nf_consumer() formats the SBI addresses.  The strings have to
 * outlive the call, because the request is serialised after it returns.
 */
struct nf_consumer_addrs {
    char v4[OGS_ADDRSTRLEN];
    char v6[OGS_ADDRSTRLEN];
};

/*
 * fill_nf_consumer() — nfConsumerIdentification, which TS 32.291 §7.2.1.1
 * lists as required on every ChargingDataRequest, not just the first.
 *
 * Each member carries the one thing it is defined to carry:
 *
 *   nFName          the NF instance id.  TS 32.291 types this as an
 *                   NfInstanceId, which TS 29.571 defines as a UUID, and
 *                   TS 32.298 in turn requires a UUIDv4 where the CDR
 *                   records it as a NetworkFunctionName.  This used to
 *                   send the FQDN or the SBI address, which fits the
 *                   length constraint and so encoded cleanly, but is not
 *                   an NfInstanceId and left the CDR non-conformant.
 *   nFFqdn          the FQDN, when the SBI advertise address resolves to
 *                   one.  This is where a readable name belongs.
 *   nFIPv4Address   the SBI address peers actually see, and nFIPv6Address
 *   nFIPv6Address   likewise.  Between them these carry everything nFName
 *                   used to, so nothing is lost by correcting it.
 *
 * nodeFunctionality is the only member the schema insists on, so the
 * object is always sent even if the NF instance somehow has no id.
 *
 * On the CHF side the id is what reaches the CDR; the metric label now
 * prefers the FQDN or the address, since an instance id is regenerated on
 * every start and would grow label cardinality without bound (see
 * src/chf/nchf-handler.c).
 */
static void fill_nf_consumer(OpenAPI_nf_identification_t *nfc,
        struct nf_consumer_addrs *addrs)
{
    ogs_sbi_nf_instance_t *self_nf = NULL;

    ogs_assert(nfc);
    ogs_assert(addrs);

    memset(nfc, 0, sizeof(*nfc));
    nfc->node_functionality = OpenAPI_node_functionality_SMF;

    self_nf = ogs_sbi_self()->nf_instance;
    if (!self_nf)
        return;

    if (self_nf->id && *self_nf->id)
        nfc->n_f_name = self_nf->id;

    if (self_nf->fqdn && *self_nf->fqdn)
        nfc->n_f_fqdn = self_nf->fqdn;

    if (self_nf->num_of_ipv4 > 0 && self_nf->ipv4[0])
        nfc->n_fipv4_address =
            (char *)OGS_ADDR(self_nf->ipv4[0], addrs->v4);

    if (self_nf->num_of_ipv6 > 0 && self_nf->ipv6[0])
        nfc->n_fipv6_address =
            (char *)OGS_ADDR(self_nf->ipv6[0], addrs->v6);
}

/*
 * build_notify_uri() — where the CHF reaches this session between reports
 * (TS 32.291 §6.3.1.2, ChargingDataRequest.notifyUri).  Without it the CHF
 * can only answer a report, so a top-up or a bar waits for one: with a
 * grant capped at 50 MiB and five minutes, up to five minutes.
 *
 * The CHF POSTs a ChargingNotifyRequest here and the SMF answers by
 * reporting at once (triggerType FORCED_REAUTHORISATION), which is what
 * picks the new quota up.  The path carries smContextRef, so the
 * notification lands on this session and no other.
 *
 * Returns an ogs_free()able string, or NULL.
 */
static char *build_notify_uri(smf_sess_t *sess)
{
    ogs_sbi_server_t *server = ogs_sbi_server_first();
    ogs_sbi_header_t header;

    ogs_assert(sess);

    if (!server || !sess->sm_context_ref)
        return NULL;

    memset(&header, 0, sizeof(header));
    header.service.name = (char *)OGS_SBI_SERVICE_NAME_NSMF_CALLBACK;
    header.api.version = (char *)OGS_SBI_API_V1;
    header.resource.component[0] =
            (char *)OGS_SBI_RESOURCE_NAME_CHARGING_NOTIFY;
    header.resource.component[1] = sess->sm_context_ref;

    return ogs_sbi_server_uri(server, &header);
}

/* ------------------------------------------------------------------ */
/* charging_information()                                               */
/*                                                                      */
/* TS 32.291 ChargingDataRequest.pDUSessionChargingInformation.  The    */
/* session's own details live here, not at the top level: charging id,  */
/* GPSI and roaming state, then pduSessionInformation with the PSI,     */
/* DNN, RAT type, home PLMN and S-NSSAI.                                */
/*                                                                      */
/* Everything is wired into caller-owned storage so there is nothing to */
/* free but `mcc`/`mnc`, which the caller releases.                     */
/* ------------------------------------------------------------------ */
struct charging_info_storage {
    OpenAPI_pdu_session_charging_information_t info;
    OpenAPI_pdu_session_information_t pdu_info;
    OpenAPI_user_information_t user_info;
    OpenAPI_network_slicing_info_t slicing;
    OpenAPI_snssai_t snssai;
    OpenAPI_plmn_id_t hplmn;
    char sd_str[7];         /* 6 hex chars + NUL */
    char *mcc;
    char *mnc;
};

static OpenAPI_pdu_session_charging_information_t *charging_information(
        smf_sess_t *sess, smf_ue_t *smf_ue, struct charging_info_storage *st)
{
    bool roaming;

    ogs_assert(sess);
    ogs_assert(smf_ue);
    ogs_assert(st);

    memset(st, 0, sizeof(*st));

    st->info.is_charging_id = true;
    st->info.charging_id = (int)sess->charging.id;

    /*
     * roamerInOut (TS 32.291 UserInformation).  The subscriber's home PLMN
     * comes from its SUPI, the serving one from the AMF, so a difference
     * means roaming.  Which way round depends on where this SMF sits: an
     * H-SMF serves its own subscriber on a visited network (OUT_BOUND),
     * while a serving SMF sees another operator's subscriber on ours
     * (IN_BOUND).  Equal PLMNs are not roaming, and the field is left out
     * entirely - the CHF reads "absent" as the home network.
     */
    roaming = memcmp(&sess->home_plmn_id, &sess->serving_plmn_id,
                    OGS_PLMN_ID_LEN) != 0;

    if (smf_ue->gpsi || roaming) {
        if (smf_ue->gpsi)
            st->user_info.served_gpsi = smf_ue->gpsi;
        if (roaming)
            st->user_info.roamer_in_out = HOME_ROUTED_ROAMING_IN_HSMF(sess) ?
                OpenAPI_roamer_in_out_OUT_BOUND :
                OpenAPI_roamer_in_out_IN_BOUND;
        st->info.user_information = &st->user_info;
    }

    st->pdu_info.pdu_session_id = sess->psi;
    if (sess->session.name)
        st->pdu_info.dnn_id = sess->session.name;

    /* TS 29.571 §5.4.1 Snssai: {"sst": N, "sd": "XXXXXX"} */
    st->snssai.sst = sess->s_nssai.sst;
    if (sess->s_nssai.sd.v != OGS_S_NSSAI_NO_SD_VALUE) {
        ogs_snprintf(st->sd_str, sizeof(st->sd_str),
                "%06x", sess->s_nssai.sd.v);
        st->snssai.sd = st->sd_str;
    }
    st->slicing.s_nssai = &st->snssai;
    st->pdu_info.network_slicing_info = &st->slicing;

    if (sess->sbi_rat_type != OpenAPI_rat_type_NULL)
        st->pdu_info.rat_type = sess->sbi_rat_type;

    /* hPlmnId — the subscriber's home network, so a CHF serving several
     * operators can tell whose allowance this is (TS 29.571 PlmnId). */
    st->mcc = ogs_plmn_id_mcc_string(&sess->home_plmn_id);
    st->mnc = ogs_plmn_id_mnc_string(&sess->home_plmn_id);
    if (st->mcc && st->mnc) {
        st->hplmn.mcc = st->mcc;
        st->hplmn.mnc = st->mnc;
        st->pdu_info.h_plmn_id = &st->hplmn;
    }

    st->info.pdu_session_information = &st->pdu_info;

    return &st->info;
}

static void charging_information_finalize(struct charging_info_storage *st)
{
    if (st->mcc) ogs_free(st->mcc);
    if (st->mnc) ogs_free(st->mnc);
    st->mcc = st->mnc = NULL;
}

/* ------------------------------------------------------------------ */
/* Create — POST /nchf-convergedcharging/v3/chargingdata             */
/* ------------------------------------------------------------------ */

ogs_sbi_request_t *smf_nchf_convergedcharging_build_create(
        smf_sess_t *sess, void *data)
{
    smf_ue_t *smf_ue = NULL;
    ogs_sbi_message_t message;
    ogs_sbi_request_t *request = NULL;

    OpenAPI_charging_data_request_t ChargingDataRequest;
    OpenAPI_nf_identification_t NFConsumerIdentification;
    struct charging_info_storage st;

    struct nf_consumer_addrs nfc_addrs;
    char *notify_uri = NULL;
    char *invocation_time = NULL;

    ogs_assert(sess);
    ogs_assert(sess->session.name);

    smf_ue = smf_ue_find_by_id(sess->smf_ue_id);
    if (!smf_ue) {
        ogs_error("[%s] smf_ue not found", sess->session.name);
        return NULL;
    }
    if (!smf_ue->supi) {
        ogs_error("[%s] No SUPI — cannot build CHF Create",
                sess->session.name);
        return NULL;
    }

    /*
     * ChargingDataRequest (initial) — TS 32.291 §7.2.1.1:
     *   subscriberIdentifier           — SUPI "imsi-<digits>"
     *   nfConsumerIdentification       — this SMF
     *   notifyUri                      — where ChargingNotify goes
     *   pDUSessionChargingInformation  — the session: charging id, GPSI,
     *                                    PSI, DNN, RAT type and S-NSSAI
     * No usedUnitContainer on initial Create.
     */
    memset(&ChargingDataRequest, 0, sizeof(ChargingDataRequest));

    ChargingDataRequest.subscriber_identifier = smf_ue->supi;
    ChargingDataRequest.p_du_session_charging_information =
        charging_information(sess, smf_ue, &st);

    /* invocationTimeStamp and invocationSequenceNumber are mandatory
     * (TS 32.291 §7.2.1.1 required list).  This is the first request of
     * the charging session, so the sequence starts at 0. */
    invocation_time = ogs_sbi_gmtime_string(ogs_time_now());
    ChargingDataRequest.invocation_time_stamp = invocation_time;
    ChargingDataRequest.invocation_sequence_number = 0;

    notify_uri = build_notify_uri(sess);
    if (notify_uri)
        ChargingDataRequest.notify_uri = notify_uri;
    else
        ogs_error("[%s] no notifyUri — the CHF will not be able to "
                  "reach this session between reports", sess->session.name);

    fill_nf_consumer(&NFConsumerIdentification, &nfc_addrs);
    ChargingDataRequest.nf_consumer_identification = &NFConsumerIdentification;

    build_message_header(&message, NULL, NULL);
    message.ChargingDataRequest = &ChargingDataRequest;

    request = ogs_sbi_build_request(&message);
    if (!request)
        ogs_error("[%s] ogs_sbi_build_request() failed", sess->session.name);
    else
        ogs_debug("[%s] CHF Create request built [SUPI:%s PSI:%u]",
                sess->session.name, smf_ue->supi, sess->psi);

    charging_information_finalize(&st);
    if (notify_uri) ogs_free(notify_uri);
    if (invocation_time) ogs_free(invocation_time);

    return request;
}

/* ------------------------------------------------------------------ */
/* trigger_type_from_smf()                                              */
/*                                                                      */
/* TS 32.291 Trigger: why the SMF is reporting.  The UPF says so in the */
/* PFCP Usage Report Trigger, which n4-handler.c maps onto              */
/* SMF_CHF_TRIGGER_*; without it the CHF has to guess from the numbers  */
/* whether a report means "threshold crossed", "grant used up" or "the  */
/* timer expired", and the three call for different answers.            */
/*                                                                      */
/* Every one of these is reported as it happens, so the category is     */
/* always IMMEDIATE_REPORT.  A Release always reports FINAL, whatever   */
/* the last UPF trigger was.                                            */
/* ------------------------------------------------------------------ */
static OpenAPI_trigger_type_e trigger_type_from_smf(uint8_t trigger)
{
    switch (trigger) {
    case SMF_CHF_TRIGGER_QUOTA_HOLDING:
        return OpenAPI_trigger_type_QHT;
    case SMF_CHF_TRIGGER_QUOTA_THRESHOLD:
        return OpenAPI_trigger_type_QUOTA_THRESHOLD;
    case SMF_CHF_TRIGGER_QUOTA_EXHAUSTED:
        return OpenAPI_trigger_type_QUOTA_EXHAUSTED;
    case SMF_CHF_TRIGGER_VALIDITY_TIME:
        return OpenAPI_trigger_type_VALIDITY_TIME;
    case SMF_CHF_TRIGGER_FORCED_REAUTH:
        return OpenAPI_trigger_type_FORCED_REAUTHORISATION;
    case SMF_CHF_TRIGGER_FINAL:
        return OpenAPI_trigger_type__FINAL;
    default:
        return OpenAPI_trigger_type_NULL;
    }
}

/* ------------------------------------------------------------------ */
/* Update  — POST /nchf-convergedcharging/v3/chargingdata/{ref}/update */
/* Release — POST /nchf-convergedcharging/v3/chargingdata/{ref}/release */
/*                                                                    */
/* Both carry the same ChargingDataRequest shape (TS 32.291 §7.2.1.1):*/
/*   subscriberIdentifier, pDUSessionChargingInformation,             */
/*   multipleUnitUsage = [{ratingGroup, usedUnitContainer[]}],        */
/*   triggers = [{triggerType, triggerCategory}]                      */
/* and differ only in the trailing URI component, so they share one   */
/* builder.                                                           */
/* ------------------------------------------------------------------ */
static ogs_sbi_request_t *build_usage_report(
        smf_sess_t *sess, const char *operation)
{
    smf_ue_t *smf_ue = NULL;
    ogs_sbi_message_t message;
    ogs_sbi_request_t *request = NULL;

    OpenAPI_charging_data_request_t ChargingDataRequest;
    OpenAPI_nf_identification_t NFConsumerIdentification;
    OpenAPI_multiple_unit_usage_t MultipleUnitUsage;
    OpenAPI_used_unit_container_t UsedUnitContainer;
    OpenAPI_charging_trigger_t Trigger;
    OpenAPI_list_t *MultipleUnitUsageList = NULL;
    OpenAPI_list_t *UsedUnitContainerList = NULL;
    OpenAPI_list_t *TriggerList = NULL;
    struct charging_info_storage st;

    OpenAPI_trigger_type_e trigger_type;
    uint64_t delta_ul, delta_dl;
    uint32_t delta_sec;
    char *invocation_time = NULL;
    struct nf_consumer_addrs nfc_addrs;

    ogs_assert(sess);
    ogs_assert(operation);

    if (!CHF_ASSOCIATED(sess)) {
        ogs_error("[%s] CHF %s — no chf_association",
                smf_nchf_dnn(sess), operation);
        return NULL;
    }

    smf_ue = smf_ue_find_by_id(sess->smf_ue_id);
    if (!smf_ue || !smf_ue->supi) {
        ogs_error("[%s] No smf_ue/SUPI — cannot build CHF %s",
                smf_nchf_dnn(sess), operation);
        return NULL;
    }

    /* Delta usage since the last CHF report (TS 32.291 §6.3.2.2).
     * sess->chf.duration accumulates PFCP duration_measurement.u32, which is
     * already in seconds (TS 29.244 §8.2.86) — no unit conversion needed. */
    delta_ul  = sess->chf.ul_octets  - sess->chf.last_report.ul_octets;
    delta_dl  = sess->chf.dl_octets  - sess->chf.last_report.dl_octets;
    delta_sec = (uint32_t)(sess->chf.duration - sess->chf.last_report.duration);

    memset(&ChargingDataRequest, 0, sizeof(ChargingDataRequest));
    memset(&MultipleUnitUsage, 0, sizeof(MultipleUnitUsage));
    memset(&UsedUnitContainer, 0, sizeof(UsedUnitContainer));
    memset(&Trigger, 0, sizeof(Trigger));

    ChargingDataRequest.subscriber_identifier = smf_ue->supi;
    ChargingDataRequest.p_du_session_charging_information =
        charging_information(sess, smf_ue, &st);

    fill_nf_consumer(&NFConsumerIdentification, &nfc_addrs);
    ChargingDataRequest.nf_consumer_identification = &NFConsumerIdentification;

    /* invocationTimeStamp and invocationSequenceNumber are mandatory
     * (TS 32.291 §7.2.1.1 required list).  The sequence is per charging
     * session and shared with localSequenceNumber below. */
    invocation_time = ogs_sbi_gmtime_string(ogs_time_now());
    ChargingDataRequest.invocation_time_stamp = invocation_time;
    ChargingDataRequest.invocation_sequence_number = (int)(sess->nchf_seq + 1);

    /* TS 32.291 §7.2.2.2.1 UsedUnitContainer — volume and time for one
     * rating group.  localSequenceNumber is per charging session, so
     * Update and Release share sess->nchf_seq rather than separate
     * counters. */
    UsedUnitContainer.local_sequence_number = (int)(sess->nchf_seq + 1);
    UsedUnitContainer.is_uplink_volume   = true;
    UsedUnitContainer.uplink_volume      = (long)delta_ul;
    UsedUnitContainer.is_downlink_volume = true;
    UsedUnitContainer.downlink_volume    = (long)delta_dl;
    UsedUnitContainer.is_total_volume    = true;
    UsedUnitContainer.total_volume       = (long)(delta_ul + delta_dl);
    UsedUnitContainer.is_time            = true;
    UsedUnitContainer.time               = (int)delta_sec;

    UsedUnitContainerList = OpenAPI_list_create();
    ogs_assert(UsedUnitContainerList);
    OpenAPI_list_add(UsedUnitContainerList, &UsedUnitContainer);

    /* Rating Group 1 — default for single-RG 5G DNN charging */
    MultipleUnitUsage.rating_group = 1;
    MultipleUnitUsage.used_unit_container = UsedUnitContainerList;

    MultipleUnitUsageList = OpenAPI_list_create();
    ogs_assert(MultipleUnitUsageList);
    OpenAPI_list_add(MultipleUnitUsageList, &MultipleUnitUsage);
    ChargingDataRequest.multiple_unit_usage = MultipleUnitUsageList;

    /* Why this report is being sent.  A Release is always FINAL; an
     * Update carries whatever the UPF reported. */
    trigger_type = trigger_type_from_smf(
            strcmp(operation, OGS_SBI_RESOURCE_NAME_RELEASE) == 0
                ? SMF_CHF_TRIGGER_FINAL : sess->chf.trigger);
    if (trigger_type != OpenAPI_trigger_type_NULL) {
        Trigger.trigger_type = trigger_type;
        Trigger.trigger_category = OpenAPI_trigger_category_IMMEDIATE_REPORT;
        TriggerList = OpenAPI_list_create();
        ogs_assert(TriggerList);
        OpenAPI_list_add(TriggerList, &Trigger);
        ChargingDataRequest.triggers = TriggerList;
    }
    /* else the UPF gave no reason we can name; omit it */

    /* Consumed: the next Update reports its own reason, not this one. */
    sess->chf.trigger = SMF_CHF_TRIGGER_NONE;

    build_message_header(&message, sess->chf_association.ref, operation);
    message.ChargingDataRequest = &ChargingDataRequest;

    request = ogs_sbi_build_request(&message);

    charging_information_finalize(&st);
    if (invocation_time) ogs_free(invocation_time);
    if (TriggerList) OpenAPI_list_free(TriggerList);
    OpenAPI_list_free(UsedUnitContainerList);
    OpenAPI_list_free(MultipleUnitUsageList);

    if (!request) {
        ogs_error("[%s] ogs_sbi_build_request() failed", smf_nchf_dnn(sess));
        return NULL;
    }

    /*
     * Advance the reported-usage baseline now that the request is built and
     * about to be sent — the same point at which gy-path.c advances it (see
     * smf_gy_send_ccr()).
     *
     * Doing this on the *response* instead would silently drop any usage the
     * UPF reported while the request was in flight: the response handler
     * would snapshot the then-current counters, so those octets would never
     * appear in any request.  Advancing here keeps every octet accounted for
     * exactly once, whichever order the PFCP reports and CHF responses
     * interleave in.
     */
    sess->nchf_seq++;
    sess->chf.last_report.ul_octets = sess->chf.ul_octets;
    sess->chf.last_report.dl_octets = sess->chf.dl_octets;
    sess->chf.last_report.duration  = sess->chf.duration;

    ogs_debug("[%s] CHF %s built [ref:%s seq:%u dUL=%llu dDL=%llu dt=%us]",
            smf_nchf_dnn(sess), operation, sess->chf_association.ref,
            sess->nchf_seq,
            (unsigned long long)delta_ul,
            (unsigned long long)delta_dl,
            delta_sec);

    return request;
}

ogs_sbi_request_t *smf_nchf_convergedcharging_build_update(
        smf_sess_t *sess, void *data)
{
    return build_usage_report(sess, OGS_SBI_RESOURCE_NAME_UPDATE);
}

ogs_sbi_request_t *smf_nchf_convergedcharging_build_release(
        smf_sess_t *sess, void *data)
{
    return build_usage_report(sess, OGS_SBI_RESOURCE_NAME_RELEASE);
}
