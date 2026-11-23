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

#include "cdr-json.h"

/* ------------------------------------------------------------------ */
/* Rendering the ASN.1 leaf types                                       */
/* ------------------------------------------------------------------ */

/* An OCTET STRING / IA5String / UTF8String carrying text. */
static void add_text(cJSON *obj, const char *name, const OCTET_STRING_t *os)
{
    char *s;

    if (!os || !os->buf)
        return;

    /* The codec NUL-terminates what it allocates, but a decoded string
     * need not be, so copy with an explicit length. */
    s = ogs_calloc(1, os->size + 1);
    if (!s)
        return;
    memcpy(s, os->buf, os->size);
    cJSON_AddStringToObject(obj, name, s);
    ogs_free(s);
}

/*
 * TS 32.298 TimeStamp: BCD YYMMDDhhmmss, ASCII sign, BCD hhmm offset.
 * Rendered as ISO-8601.  A two-digit year is read as 20YY, which is the
 * only reading that makes sense for records written now and matches what
 * every other CDR reader assumes.
 */
#define UNBCD(b)    ((((b) >> 4) & 0x0f) * 10 + ((b) & 0x0f))

static void add_timestamp(cJSON *obj, const char *name,
        const CHFCDR_TimeStamp_t *ts)
{
    char iso[40];
    const uint8_t *v;
    int off_min;

    if (!ts || !ts->buf || ts->size < 9)
        return;

    v = ts->buf;
    off_min = UNBCD(v[7]) * 60 + UNBCD(v[8]);

    if (off_min == 0) {
        ogs_snprintf(iso, sizeof(iso),
                "20%02d-%02d-%02dT%02d:%02d:%02dZ",
                UNBCD(v[0]), UNBCD(v[1]), UNBCD(v[2]),
                UNBCD(v[3]), UNBCD(v[4]), UNBCD(v[5]));
    } else {
        ogs_snprintf(iso, sizeof(iso),
                "20%02d-%02d-%02dT%02d:%02d:%02d%c%02d:%02d",
                UNBCD(v[0]), UNBCD(v[1]), UNBCD(v[2]),
                UNBCD(v[3]), UNBCD(v[4]), UNBCD(v[5]),
                v[6] == (uint8_t)'-' ? '-' : '+',
                UNBCD(v[7]), UNBCD(v[8]));
    }

    cJSON_AddStringToObject(obj, name, iso);
}

/* SliceDifferentiator: 3 octets, written the way TS 23.003 writes an SD. */
static void add_slice_differentiator(cJSON *obj, const char *name,
        const CHFCDR_SliceDifferentiator_t *sd)
{
    char hex[7];

    if (!sd || !sd->buf || sd->size < 3)
        return;

    ogs_snprintf(hex, sizeof(hex), "%02x%02x%02x",
            sd->buf[0], sd->buf[1], sd->buf[2]);
    cJSON_AddStringToObject(obj, name, hex);
}

/*
 * The ASN.1 name for a value, or NULL when the value has none.
 *
 * Only the values the CHF writes are named.  Anything else falls through
 * to a number, which is honest: inventing a name for a value we never
 * produce would be a guess a reader might trust.
 */
static const char *record_type_name(long v)
{
    return v == 200 ? "chargingFunctionRecord" : NULL;
}

static const char *subscription_id_type_name(long v)
{
    switch (v) {
    case CHFCDR_SubscriptionIDType_eND_USER_E164:     return "eND-USER-E164";
    case CHFCDR_SubscriptionIDType_eND_USER_IMSI:     return "eND-USER-IMSI";
    case CHFCDR_SubscriptionIDType_eND_USER_SIP_URI:  return "eND-USER-SIP-URI";
    case CHFCDR_SubscriptionIDType_eND_USER_NAI:      return "eND-USER-NAI";
    case CHFCDR_SubscriptionIDType_eND_USER_PRIVATE:  return "eND-USER-PRIVATE";
    default:                                          return NULL;
    }
}

static const char *network_functionality_name(long v)
{
    switch (v) {
    case CHFCDR_NetworkFunctionality_cHF:   return "cHF";
    case CHFCDR_NetworkFunctionality_sMF:   return "sMF";
    case CHFCDR_NetworkFunctionality_aMF:   return "aMF";
    case CHFCDR_NetworkFunctionality_sMSF:  return "sMSF";
    default:                                return NULL;
    }
}

/* A named value: its name when it has one, otherwise the number. */
static void add_named(cJSON *obj, const char *name, long v,
        const char *(*lookup)(long))
{
    const char *s = lookup(v);

    if (s)
        cJSON_AddStringToObject(obj, name, s);
    else
        cJSON_AddNumberToObject(obj, name, (double)v);
}

/* ------------------------------------------------------------------ */
/* Rendering the constructed types                                      */
/* ------------------------------------------------------------------ */

static cJSON *render_involved_party(const CHFCDR_InvolvedParty_t *ip)
{
    cJSON *o = cJSON_CreateObject();

    if (!o)
        return NULL;

    switch (ip->present) {
    case CHFCDR_InvolvedParty_PR_sIP_URI:
        add_text(o, "sIP-URI", &ip->choice.sIP_URI);
        break;
    case CHFCDR_InvolvedParty_PR_tEL_URI:
        add_text(o, "tEL-URI", &ip->choice.tEL_URI);
        break;
    case CHFCDR_InvolvedParty_PR_uRN:
        add_text(o, "uRN", &ip->choice.uRN);
        break;
    case CHFCDR_InvolvedParty_PR_iSDN_E164:
        add_text(o, "iSDN-E164", &ip->choice.iSDN_E164);
        break;
    case CHFCDR_InvolvedParty_PR_externalId:
        add_text(o, "externalId", &ip->choice.externalId);
        break;
    default:
        break;
    }

    return o;
}

static cJSON *render_used_unit_container(
        const CHFCDR_UsedUnitContainer_t *u)
{
    cJSON *o = cJSON_CreateObject();

    if (!o)
        return NULL;

    if (u->serviceIdentifier)
        cJSON_AddNumberToObject(o, "serviceIdentifier",
                (double)*u->serviceIdentifier);
    if (u->time)
        cJSON_AddNumberToObject(o, "time", (double)*u->time);
    if (u->dataTotalVolume)
        cJSON_AddNumberToObject(o, "dataTotalVolume",
                (double)*u->dataTotalVolume);
    if (u->dataVolumeUplink)
        cJSON_AddNumberToObject(o, "dataVolumeUplink",
                (double)*u->dataVolumeUplink);
    if (u->dataVolumeDownlink)
        cJSON_AddNumberToObject(o, "dataVolumeDownlink",
                (double)*u->dataVolumeDownlink);
    if (u->localSequenceNumber)
        cJSON_AddNumberToObject(o, "localSequenceNumber",
                (double)*u->localSequenceNumber);

    return o;
}

static cJSON *render_multiple_unit_usage(
        const CHFCDR_MultipleUnitUsage_t *m)
{
    cJSON *o = cJSON_CreateObject();
    cJSON *arr;
    int i;

    if (!o)
        return NULL;

    cJSON_AddNumberToObject(o, "ratingGroup", (double)m->ratingGroup);

    if (!m->usedUnitContainers)
        return o;

    arr = cJSON_AddArrayToObject(o, "usedUnitContainers");
    if (!arr)
        return o;

    for (i = 0; i < m->usedUnitContainers->list.count; i++) {
        cJSON *item = render_used_unit_container(
                m->usedUnitContainers->list.array[i]);
        if (item)
            cJSON_AddItemToArray(arr, item);
    }

    return o;
}

static cJSON *render_pdu_session_info(
        const CHFCDR_PDUSessionChargingInformation_t *p)
{
    cJSON *o = cJSON_CreateObject();

    if (!o)
        return NULL;

    cJSON_AddNumberToObject(o, "pDUSessionChargingID",
            (double)p->pDUSessionChargingID);

    if (p->userIdentifier) {
        cJSON *ip = render_involved_party(p->userIdentifier);
        if (ip)
            cJSON_AddItemToObject(o, "userIdentifier", ip);
    }

    cJSON_AddNumberToObject(o, "pDUSessionId", (double)p->pDUSessionId);

    if (p->networkSliceInstanceID) {
        cJSON *n = cJSON_CreateObject();
        if (n) {
            cJSON_AddNumberToObject(n, "sST",
                    (double)p->networkSliceInstanceID->sST);
            add_slice_differentiator(n, "sD",
                    p->networkSliceInstanceID->sD);
            cJSON_AddItemToObject(o, "networkSliceInstanceID", n);
        }
    }

    add_text(o, "dataNetworkNameIdentifier", p->dataNetworkNameIdentifier);
    add_timestamp(o, "pDUSessionstartTime", p->pDUSessionstartTime);
    add_timestamp(o, "pDUSessionstopTime", p->pDUSessionstopTime);

    return o;
}

static cJSON *render_charging_record(const CHFCDR_ChargingRecord_t *cr)
{
    cJSON *o = cJSON_CreateObject();
    cJSON *arr;
    int i;

    if (!o)
        return NULL;

    add_named(o, "recordType", cr->recordType, record_type_name);
    add_text(o, "recordingNetworkFunctionID",
            &cr->recordingNetworkFunctionID);

    if (cr->subscriberIdentifier) {
        cJSON *s = cJSON_CreateObject();
        if (s) {
            add_named(s, "subscriptionIDType",
                    cr->subscriberIdentifier->subscriptionIDType,
                    subscription_id_type_name);
            add_text(s, "subscriptionIDData",
                    &cr->subscriberIdentifier->subscriptionIDData);
            cJSON_AddItemToObject(o, "subscriberIdentifier", s);
        }
    }

    if (cr->nFunctionConsumerInformation) {
        cJSON *n = cJSON_CreateObject();
        if (n) {
            add_named(n, "networkFunctionality",
                    cr->nFunctionConsumerInformation->networkFunctionality,
                    network_functionality_name);
            add_text(n, "networkFunctionName",
                    cr->nFunctionConsumerInformation->networkFunctionName);
            cJSON_AddItemToObject(o, "nFunctionConsumerInformation", n);
        }
    }

    if (cr->listOfMultipleUnitUsage) {
        arr = cJSON_AddArrayToObject(o, "listOfMultipleUnitUsage");
        if (arr) {
            for (i = 0; i < cr->listOfMultipleUnitUsage->list.count; i++) {
                cJSON *item = render_multiple_unit_usage(
                        cr->listOfMultipleUnitUsage->list.array[i]);
                if (item)
                    cJSON_AddItemToArray(arr, item);
            }
        }
    }

    add_timestamp(o, "recordOpeningTime", &cr->recordOpeningTime);
    cJSON_AddNumberToObject(o, "duration", (double)cr->duration);
    add_named(o, "causeForRecClosing", cr->causeForRecClosing,
            chf_cdr_cause_name);

    if (cr->localRecordSequenceNumber)
        cJSON_AddNumberToObject(o, "localRecordSequenceNumber",
                (double)*cr->localRecordSequenceNumber);

    if (cr->pDUSessionChargingInformation) {
        cJSON *p = render_pdu_session_info(
                cr->pDUSessionChargingInformation);
        if (p)
            cJSON_AddItemToObject(o, "pDUSessionChargingInformation", p);
    }

    add_text(o, "chargingSessionIdentifier", cr->chargingSessionIdentifier);

    return o;
}

char *chf_cdr_json_render(const CHFCDR_CHFRecord_t *rec)
{
    cJSON *root = NULL;
    cJSON *inner = NULL;
    char *flat = NULL;

    ogs_assert(rec);

    if (rec->present != CHFCDR_CHFRecord_PR_chargingFunctionRecord ||
            !rec->choice.chargingFunctionRecord) {
        ogs_error("CDR: record has no chargingFunctionRecord to render");
        return NULL;
    }

    root = cJSON_CreateObject();
    if (!root)
        return NULL;

    /* The CHOICE is kept in the JSON, so the record names itself the way
     * the ASN.1 does rather than starting at the ChargingRecord. */
    inner = render_charging_record(rec->choice.chargingFunctionRecord);
    if (!inner) {
        cJSON_Delete(root);
        return NULL;
    }
    cJSON_AddItemToObject(root, "chargingFunctionRecord", inner);

    flat = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);

    if (!flat) {
        ogs_error("CDR: cJSON_PrintUnformatted failed");
        return NULL;
    }

    /* open5gs points cJSON at ogs_malloc (see lib/third-party/cjson),
     * so this is already ogs-owned and the caller frees it with
     * ogs_free() like anything else. */
    return flat;
}
