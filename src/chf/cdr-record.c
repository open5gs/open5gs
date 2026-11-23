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

#include <time.h>

#include "cdr-record.h"

/* ------------------------------------------------------------------ */
/* TS 32.298 CauseForRecClosing                                         */
/*                                                                      */
/* The three the CHF can close a session for.  The names are the ones    */
/* the ASN.1 gives the values, so the JSON and the BER say the same      */
/* thing - the JSON by name, the BER by number.                         */
/* ------------------------------------------------------------------ */

#define CHF_CDR_CAUSE_NORMAL_RELEASE            0
#define CHF_CDR_CAUSE_VOLUME_LIMIT              16
#define CHF_CDR_CAUSE_MANAGEMENT_INTERVENTION   20

long chf_cdr_cause_value(const char *cause)
{
    if (!cause)
        return CHF_CDR_CAUSE_NORMAL_RELEASE;
    if (!strcmp(cause, "volumeLimit"))
        return CHF_CDR_CAUSE_VOLUME_LIMIT;
    if (!strcmp(cause, "managementIntervention"))
        return CHF_CDR_CAUSE_MANAGEMENT_INTERVENTION;
    if (!strcmp(cause, "normalRelease"))
        return CHF_CDR_CAUSE_NORMAL_RELEASE;

    ogs_warn("CDR: unknown causeForRecClosing '%s' — recording "
             "normalRelease", cause);
    return CHF_CDR_CAUSE_NORMAL_RELEASE;
}

const char *chf_cdr_cause_name(long value)
{
    switch (value) {
    case CHF_CDR_CAUSE_NORMAL_RELEASE:          return "normalRelease";
    case CHF_CDR_CAUSE_VOLUME_LIMIT:            return "volumeLimit";
    case CHF_CDR_CAUSE_MANAGEMENT_INTERVENTION:
        return "managementIntervention";
    default:                                    return NULL;
    }
}

/* ------------------------------------------------------------------ */
/* TS 32.298 TimeStamp                                                  */
/*                                                                      */
/* Nine octets: BCD YYMMDDhhmmss, then the sign of the offset to UTC as  */
/* ASCII, then BCD hhmm of that offset.  The CHF keeps time in UTC, so   */
/* the local time it writes is UTC and the offset is +0000 - correct     */
/* rather than merely convenient, since the pair is what makes the       */
/* timestamp unambiguous.                                               */
/* ------------------------------------------------------------------ */

#define BCD(n)  (uint8_t)((((n) / 10) << 4) | ((n) % 10))

static int set_timestamp(CHFCDR_TimeStamp_t *ts, time_t t)
{
    struct tm tm_utc;
    uint8_t v[9];

    ogs_assert(ts);

    if (!gmtime_r(&t, &tm_utc))
        return OGS_ERROR;

    v[0] = BCD((tm_utc.tm_year + 1900) % 100);
    v[1] = BCD(tm_utc.tm_mon + 1);
    v[2] = BCD(tm_utc.tm_mday);
    v[3] = BCD(tm_utc.tm_hour);
    v[4] = BCD(tm_utc.tm_min);
    v[5] = BCD(tm_utc.tm_sec);
    v[6] = (uint8_t)'+';
    v[7] = BCD(0);
    v[8] = BCD(0);

    if (OCTET_STRING_fromBuf(ts, (const char *)v, sizeof(v)) != 0)
        return OGS_ERROR;

    return OGS_OK;
}

static CHFCDR_TimeStamp_t *new_timestamp(time_t t)
{
    CHFCDR_TimeStamp_t *ts = ogs_calloc(1, sizeof(*ts));
    if (!ts)
        return NULL;
    if (set_timestamp(ts, t) != OGS_OK) {
        ogs_free(ts);
        return NULL;
    }
    return ts;
}

/* ------------------------------------------------------------------ */
/* Building the record                                                  */
/* ------------------------------------------------------------------ */

/*
 * listOfMultipleUnitUsage [5] — one MultipleUnitUsage for the session's
 * rating group, holding one UsedUnitContainer with the volumes.
 *
 * The CHF grants a single combined allowance rather than a per-direction
 * pair, so dataTotalVolume is what it actually charged against; the
 * uplink and downlink figures are reported alongside it because the UPF
 * counts them separately and a mediation system expects to see them.
 */
static int add_unit_usage(CHFCDR_ChargingRecord_t *cr,
        const chf_sess_t *sess, uint32_t local_seq)
{
    CHFCDR_MultipleUnitUsage_t *muu = NULL;
    CHFCDR_UsedUnitContainer_t *uuc = NULL;

    muu = ogs_calloc(1, sizeof(*muu));
    if (!muu)
        goto fail;
    uuc = ogs_calloc(1, sizeof(*uuc));
    if (!uuc)
        goto fail;

    muu->ratingGroup = (CHFCDR_RatingGroupId_t)sess->rating_group;

    if (sess->service_id) {
        uuc->serviceIdentifier = ogs_calloc(1, sizeof(*uuc->serviceIdentifier));
        if (!uuc->serviceIdentifier)
            goto fail;
        *uuc->serviceIdentifier =
            (CHFCDR_ServiceIdentifier_t)sess->service_id;
    }

    uuc->time = ogs_calloc(1, sizeof(*uuc->time));
    if (!uuc->time)
        goto fail;
    *uuc->time = (CHFCDR_CallDuration_t)sess->duration;

    uuc->dataTotalVolume = ogs_calloc(1, sizeof(*uuc->dataTotalVolume));
    if (!uuc->dataTotalVolume)
        goto fail;
    *uuc->dataTotalVolume =
        (CHFCDR_DataVolumeOctets_t)(sess->ul_octets + sess->dl_octets);

    uuc->dataVolumeUplink = ogs_calloc(1, sizeof(*uuc->dataVolumeUplink));
    if (!uuc->dataVolumeUplink)
        goto fail;
    *uuc->dataVolumeUplink = (CHFCDR_DataVolumeOctets_t)sess->ul_octets;

    uuc->dataVolumeDownlink = ogs_calloc(1, sizeof(*uuc->dataVolumeDownlink));
    if (!uuc->dataVolumeDownlink)
        goto fail;
    *uuc->dataVolumeDownlink = (CHFCDR_DataVolumeOctets_t)sess->dl_octets;

    uuc->localSequenceNumber =
        ogs_calloc(1, sizeof(*uuc->localSequenceNumber));
    if (!uuc->localSequenceNumber)
        goto fail;
    *uuc->localSequenceNumber = (CHFCDR_LocalSequenceNumber_t)local_seq;

    muu->usedUnitContainers = ogs_calloc(1, sizeof(*muu->usedUnitContainers));
    if (!muu->usedUnitContainers)
        goto fail;
    if (ASN_SEQUENCE_ADD(&muu->usedUnitContainers->list, uuc) != 0)
        goto fail;
    uuc = NULL;   /* owned by the list now */

    cr->listOfMultipleUnitUsage =
        ogs_calloc(1, sizeof(*cr->listOfMultipleUnitUsage));
    if (!cr->listOfMultipleUnitUsage)
        goto fail;
    if (ASN_SEQUENCE_ADD(&cr->listOfMultipleUnitUsage->list, muu) != 0)
        goto fail;

    return OGS_OK;

fail:
    if (uuc)
        ASN_STRUCT_FREE(asn_DEF_CHFCDR_UsedUnitContainer, uuc);
    if (muu)
        ASN_STRUCT_FREE(asn_DEF_CHFCDR_MultipleUnitUsage, muu);
    return OGS_ERROR;
}

/*
 * A 32-bit FNV-1a of a NUL-terminated string.
 *
 * This is deliberately our own four lines rather than a library call.
 * The value it produces is written into a CDR, so it has to mean the same
 * thing next year as it does today; a hash belonging to lib/core does not
 * promise that, and upstream proved the point by replacing the chained
 * hash tables with Robin Hood hashing and deleting the
 * ogs_hashfunc_default() this used to call.  FNV-1a is fixed by its
 * definition, so the derived id is now stable across upstream churn.
 *
 * Note for anyone comparing old records with new: the id a given
 * ChargingDataRef maps to changed when this replaced the library hash.
 * It is a derived correlator, not an identity - chargingSessionIdentifier
 * [16] carries the UUID whole and is what a reader should join on.
 */
static uint32_t chf_cdr_fnv1a32(const char *s)
{
    uint32_t h = 2166136261U;       /* FNV offset basis */

    for (; *s; s++) {
        h ^= (uint32_t)(unsigned char)*s;
        h *= 16777619U;             /* FNV prime */
    }
    return h;
}

/*
 * pDUSessionChargingInformation [13] — what the session was.
 *
 * pDUSessionChargingID is mandatory and must identify the session.  The
 * CHF's own key is the ChargingDataRef UUID, which does not fit a
 * 32-bit ChargingID, so the id is derived from it by hashing and the
 * UUID itself is carried whole in chargingSessionIdentifier [16].  A
 * reader correlating records should use that field, not this one.
 */
static int add_pdu_session_info(CHFCDR_ChargingRecord_t *cr,
        const chf_sess_t *sess, time_t start, time_t stop)
{
    CHFCDR_PDUSessionChargingInformation_t *p;

    p = ogs_calloc(1, sizeof(*p));
    if (!p)
        return OGS_ERROR;

    p->pDUSessionChargingID = (CHFCDR_ChargingID_t)
        chf_cdr_fnv1a32(sess->charging_data_ref);
    p->pDUSessionId = (CHFCDR_PDUSessionId_t)sess->psi;

    /* userIdentifier [1] — the GPSI, when the SMF sent one.  A 5G GPSI is
     * either an MSISDN or an external id (TS 23.003 clause 28.8), and
     * open5gs carries it prefixed: "msisdn-<digits>" or "extid-<nai>". */
    if (sess->gpsi && sess->gpsi[0]) {
        const char *digits = NULL;

        p->userIdentifier = ogs_calloc(1, sizeof(*p->userIdentifier));
        if (!p->userIdentifier)
            goto fail;

        if (!strncmp(sess->gpsi, "msisdn-", 7)) {
            digits = sess->gpsi + 7;
            p->userIdentifier->present =
                CHFCDR_InvolvedParty_PR_iSDN_E164;
            if (OCTET_STRING_fromString(
                        &p->userIdentifier->choice.iSDN_E164, digits) != 0)
                goto fail;
        } else {
            digits = !strncmp(sess->gpsi, "extid-", 6) ?
                sess->gpsi + 6 : sess->gpsi;
            p->userIdentifier->present =
                CHFCDR_InvolvedParty_PR_externalId;
            if (OCTET_STRING_fromString(
                        &p->userIdentifier->choice.externalId, digits) != 0)
                goto fail;
        }
    }

    /* networkSliceInstanceID [7] — the S-NSSAI the session ran on.
     * Absent when the SMF sent no networkSlicingInfo. */
    if (sess->s_nssai.sst) {
        p->networkSliceInstanceID =
            ogs_calloc(1, sizeof(*p->networkSliceInstanceID));
        if (!p->networkSliceInstanceID)
            goto fail;
        p->networkSliceInstanceID->sST =
            (CHFCDR_SliceServiceType_t)sess->s_nssai.sst;

        if (sess->s_nssai.sd.v != OGS_S_NSSAI_NO_SD_VALUE) {
            uint8_t sd[3];
            sd[0] = (uint8_t)(sess->s_nssai.sd.v >> 16);
            sd[1] = (uint8_t)(sess->s_nssai.sd.v >> 8);
            sd[2] = (uint8_t)(sess->s_nssai.sd.v);

            p->networkSliceInstanceID->sD =
                ogs_calloc(1, sizeof(*p->networkSliceInstanceID->sD));
            if (!p->networkSliceInstanceID->sD)
                goto fail;
            if (OCTET_STRING_fromBuf(p->networkSliceInstanceID->sD,
                        (const char *)sd, sizeof(sd)) != 0)
                goto fail;
        }
    }

    if (sess->dnn && sess->dnn[0]) {
        p->dataNetworkNameIdentifier =
            ogs_calloc(1, sizeof(*p->dataNetworkNameIdentifier));
        if (!p->dataNetworkNameIdentifier)
            goto fail;
        if (OCTET_STRING_fromString(p->dataNetworkNameIdentifier,
                    sess->dnn) != 0)
            goto fail;
    }

    p->pDUSessionstartTime = new_timestamp(start);
    if (!p->pDUSessionstartTime)
        goto fail;
    p->pDUSessionstopTime = new_timestamp(stop);
    if (!p->pDUSessionstopTime)
        goto fail;

    cr->pDUSessionChargingInformation = p;
    return OGS_OK;

fail:
    ASN_STRUCT_FREE(asn_DEF_CHFCDR_PDUSessionChargingInformation, p);
    return OGS_ERROR;
}

CHFCDR_CHFRecord_t *chf_cdr_record_build(const chf_sess_t *sess,
        const char *cause, uint32_t local_seq)
{
    CHFCDR_CHFRecord_t *rec = NULL;
    CHFCDR_ChargingRecord_t *cr = NULL;
    const char *own_id;
    time_t stop, start;

    ogs_assert(sess);

    if (!sess->charging_data_ref) {
        ogs_error("CDR: session has no ChargingDataRef");
        return NULL;
    }

    /* recordingNetworkFunctionID [1] is mandatory and must be a UUID
     * (TS 32.298: NetworkFunctionName, 1..36 characters).  That is this
     * CHF's own NF instance id, which exists from registration onwards. */
    own_id = (ogs_sbi_self()->nf_instance && ogs_sbi_self()->nf_instance->id) ?
        ogs_sbi_self()->nf_instance->id : NULL;
    if (!own_id) {
        ogs_error("[%s] CDR: no NF instance id to record as "
                  "recordingNetworkFunctionID", sess->supi);
        return NULL;
    }

    stop = time(NULL);
    /* period_start is when the current billing period opened, which is
     * the session's own start for a session that never renewed.  Falling
     * back to stop - duration keeps the pair consistent when it is
     * unset. */
    start = sess->period_start ? sess->period_start :
        stop - (time_t)sess->duration;

    rec = ogs_calloc(1, sizeof(*rec));
    if (!rec)
        return NULL;

    /* -findirect-choice: a constructed CHOICE alternative is a pointer. */
    rec->present = CHFCDR_CHFRecord_PR_chargingFunctionRecord;
    rec->choice.chargingFunctionRecord = ogs_calloc(1, sizeof(*cr));
    if (!rec->choice.chargingFunctionRecord)
        goto fail;
    cr = rec->choice.chargingFunctionRecord;

    /* recordType [0] — chargingFunctionRecord (200).  TS 32.298 reserves
     * values from 200 for the Charging Function domain. */
    cr->recordType = 200;

    if (OCTET_STRING_fromString(&cr->recordingNetworkFunctionID,
                own_id) != 0)
        goto fail;

    /* subscriberIdentifier [2] — the SUPI.  open5gs carries it as
     * "imsi-<digits>", which is what eND-USER-IMSI means here. */
    if (sess->supi && sess->supi[0]) {
        cr->subscriberIdentifier =
            ogs_calloc(1, sizeof(*cr->subscriberIdentifier));
        if (!cr->subscriberIdentifier)
            goto fail;
        cr->subscriberIdentifier->subscriptionIDType =
            CHFCDR_SubscriptionIDType_eND_USER_IMSI;
        if (OCTET_STRING_fromString(
                    &cr->subscriberIdentifier->subscriptionIDData,
                    sess->supi) != 0)
            goto fail;
    }

    /* nFunctionConsumerInformation [3] — who asked for the charging, i.e.
     * the SMF.  Mandatory, so the functionality is always recorded even
     * when the SMF sent no nfConsumerIdentification to name it with. */
    cr->nFunctionConsumerInformation =
        ogs_calloc(1, sizeof(*cr->nFunctionConsumerInformation));
    if (!cr->nFunctionConsumerInformation)
        goto fail;
    cr->nFunctionConsumerInformation->networkFunctionality =
        CHFCDR_NetworkFunctionality_sMF;
    if (sess->smf_id && sess->smf_id[0]) {
        cr->nFunctionConsumerInformation->networkFunctionName =
            ogs_calloc(1,
                sizeof(*cr->nFunctionConsumerInformation->networkFunctionName));
        if (!cr->nFunctionConsumerInformation->networkFunctionName)
            goto fail;
        if (OCTET_STRING_fromString(
                cr->nFunctionConsumerInformation->networkFunctionName,
                sess->smf_id) != 0)
            goto fail;
    }

    if (add_unit_usage(cr, sess, local_seq) != OGS_OK)
        goto fail;

    if (set_timestamp(&cr->recordOpeningTime, start) != OGS_OK)
        goto fail;

    cr->duration = (CHFCDR_CallDuration_t)sess->duration;
    cr->causeForRecClosing = chf_cdr_cause_value(cause);

    cr->localRecordSequenceNumber =
        ogs_calloc(1, sizeof(*cr->localRecordSequenceNumber));
    if (!cr->localRecordSequenceNumber)
        goto fail;
    *cr->localRecordSequenceNumber =
        (CHFCDR_LocalSequenceNumber_t)local_seq;

    if (add_pdu_session_info(cr, sess, start, stop) != OGS_OK)
        goto fail;

    /* chargingSessionIdentifier [16] — the ChargingDataRef, whole.  This
     * is the field that correlates a CDR with the Nchf session that
     * produced it. */
    cr->chargingSessionIdentifier =
        ogs_calloc(1, sizeof(*cr->chargingSessionIdentifier));
    if (!cr->chargingSessionIdentifier)
        goto fail;
    if (OCTET_STRING_fromString(cr->chargingSessionIdentifier,
                sess->charging_data_ref) != 0)
        goto fail;

    return rec;

fail:
    chf_cdr_record_free(rec);
    return NULL;
}

void chf_cdr_record_free(CHFCDR_CHFRecord_t *rec)
{
    if (!rec)
        return;
    ASN_STRUCT_FREE(asn_DEF_CHFCDR_CHFRecord, rec);
}

/* ------------------------------------------------------------------ */
/* DER encoding                                                         */
/* ------------------------------------------------------------------ */

struct ber_sink {
    uint8_t *buf;
    size_t   size;      /* allocated */
    size_t   len;       /* used */
    bool     failed;
};

static int ber_sink_write(const void *data, size_t size, void *key)
{
    struct ber_sink *s = key;

    if (s->failed)
        return -1;

    if (s->len + size > s->size) {
        size_t want = s->size ? s->size * 2 : 1024;
        uint8_t *bigger;

        while (want < s->len + size)
            want *= 2;

        bigger = ogs_realloc(s->buf, want);
        if (!bigger) {
            s->failed = true;
            return -1;
        }
        s->buf = bigger;
        s->size = want;
    }

    memcpy(s->buf + s->len, data, size);
    s->len += size;
    return 0;
}

int chf_cdr_record_encode_ber(const CHFCDR_CHFRecord_t *rec, uint8_t **out)
{
    struct ber_sink sink = { NULL, 0, 0, false };
    asn_enc_rval_t er;

    ogs_assert(rec);
    ogs_assert(out);

    *out = NULL;

    /* der_encode() takes a non-const pointer but does not modify the
     * structure. */
    er = der_encode(&asn_DEF_CHFCDR_CHFRecord, (void *)rec,
            ber_sink_write, &sink);

    if (er.encoded < 0 || sink.failed) {
        ogs_error("CDR: DER encoding failed at %s",
                er.failed_type && er.failed_type->name ?
                er.failed_type->name : "(unknown type)");
        if (sink.buf)
            ogs_free(sink.buf);
        return OGS_ERROR;
    }

    if ((size_t)er.encoded != sink.len) {
        ogs_error("CDR: DER encoder reported %ld bytes but wrote %zu",
                (long)er.encoded, sink.len);
        if (sink.buf)
            ogs_free(sink.buf);
        return OGS_ERROR;
    }

    *out = sink.buf;
    return (int)sink.len;
}
