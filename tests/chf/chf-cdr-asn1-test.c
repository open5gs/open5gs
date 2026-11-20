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
 * The TS 32.298 record, and its BER encoding.
 *
 * These tests work on the record itself rather than on a file, so they
 * can say what the encoding contains rather than only that it was
 * written.  What they cover:
 *
 *   - the record a session becomes: which fields are present, and that
 *     the mandatory ones are always there
 *   - the encoding's outermost bytes, which must be the context tag
 *     [200] that TS 32.298 assigns chargingFunctionRecord
 *   - that what we encode decodes back identically, through the same
 *     asn1c codec a mediation system would use
 *   - volumes above 2^32, which is where DataVolumeOctets would break if
 *     it had been generated as a 32-bit type
 *   - the causeForRecClosing mapping, in both directions
 */

#include <string.h>

#include "ogs-sbi.h"
#include "chf/context.h"
#include "chf/cdr-record.h"
#include "core/abts.h"

static chf_sess_t *make_sess(const char *supi, const char *ref,
        uint64_t ul, uint64_t dl, uint32_t dur)
{
    chf_sess_t *sess = chf_sess_add(supi, ref);

    if (!sess)
        return NULL;

    sess->gpsi          = ogs_strdup("msisdn-447700900001");
    sess->dnn           = ogs_strdup("internet");
    sess->smf_id        = ogs_strdup("11111111-2222-3333-4444-555555555555");
    sess->psi           = 5;
    sess->rating_group  = 10;
    sess->service_id    = 7;
    sess->ul_octets     = ul;
    sess->dl_octets     = dl;
    sess->duration      = dur;
    sess->s_nssai.sst   = 1;
    sess->s_nssai.sd.v  = 0x010203;
    sess->period_start  = time(NULL) - (time_t)dur;

    return sess;
}

/* The ChargingRecord inside the CHOICE, or NULL. */
static CHFCDR_ChargingRecord_t *record_of(CHFCDR_CHFRecord_t *rec)
{
    if (!rec || rec->present != CHFCDR_CHFRecord_PR_chargingFunctionRecord)
        return NULL;
    return rec->choice.chargingFunctionRecord;
}

/* ------------------------------------------------------------------ */
/* The record a session becomes                                         */
/* ------------------------------------------------------------------ */
static void test_asn1_record_fields(abts_case *tc, void *data)
{
    chf_sess_t *sess;
    CHFCDR_CHFRecord_t *rec;
    CHFCDR_ChargingRecord_t *cr;
    CHFCDR_MultipleUnitUsage_t *muu;
    CHFCDR_UsedUnitContainer_t *uuc;

    sess = make_sess("imsi-001010000000001", "asn1-fields-0001",
                     512000, 2048000, 300);
    ABTS_PTR_NOTNULL(tc, sess);

    rec = chf_cdr_record_build(sess, "normalRelease", 1);
    ABTS_PTR_NOTNULL(tc, rec);

    cr = record_of(rec);
    ABTS_PTR_NOTNULL(tc, cr);

    /* recordType [0] — TS 32.298 reserves 200 for the CHF domain. */
    ABTS_INT_EQUAL(tc, 200, (int)cr->recordType);

    /* The mandatory fields, which must be present whatever the session
     * did or did not carry. */
    ABTS_INT_EQUAL(tc, 1, cr->recordingNetworkFunctionID.size > 0 ? 1 : 0);
    ABTS_PTR_NOTNULL(tc, cr->nFunctionConsumerInformation);
    ABTS_INT_EQUAL(tc, CHFCDR_NetworkFunctionality_sMF,
            (int)cr->nFunctionConsumerInformation->networkFunctionality);
    ABTS_INT_EQUAL(tc, 9, (int)cr->recordOpeningTime.size);
    ABTS_INT_EQUAL(tc, 300, (int)cr->duration);
    ABTS_INT_EQUAL(tc, 0, (int)cr->causeForRecClosing);  /* normalRelease */

    /* subscriberIdentifier [2] — the SUPI, as eND-USER-IMSI. */
    ABTS_PTR_NOTNULL(tc, cr->subscriberIdentifier);
    ABTS_INT_EQUAL(tc, CHFCDR_SubscriptionIDType_eND_USER_IMSI,
            (int)cr->subscriberIdentifier->subscriptionIDType);
    ABTS_STR_EQUAL(tc, "imsi-001010000000001",
            (const char *)cr->subscriberIdentifier->subscriptionIDData.buf);

    /* The SMF that asked for the charging. */
    ABTS_PTR_NOTNULL(tc,
            cr->nFunctionConsumerInformation->networkFunctionName);
    ABTS_STR_EQUAL(tc, "11111111-2222-3333-4444-555555555555",
            (const char *)
            cr->nFunctionConsumerInformation->networkFunctionName->buf);

    /* chargingSessionIdentifier [16] — the ChargingDataRef, whole. */
    ABTS_PTR_NOTNULL(tc, cr->chargingSessionIdentifier);
    ABTS_STR_EQUAL(tc, "asn1-fields-0001",
            (const char *)cr->chargingSessionIdentifier->buf);

    /* The volumes, in listOfMultipleUnitUsage [5]. */
    ABTS_PTR_NOTNULL(tc, cr->listOfMultipleUnitUsage);
    ABTS_INT_EQUAL(tc, 1, cr->listOfMultipleUnitUsage->list.count);
    muu = cr->listOfMultipleUnitUsage->list.array[0];
    ABTS_INT_EQUAL(tc, 10, (int)muu->ratingGroup);
    ABTS_PTR_NOTNULL(tc, muu->usedUnitContainers);
    ABTS_INT_EQUAL(tc, 1, muu->usedUnitContainers->list.count);
    uuc = muu->usedUnitContainers->list.array[0];
    ABTS_PTR_NOTNULL(tc, uuc->dataVolumeUplink);
    ABTS_INT_EQUAL(tc, 512000, (int)*uuc->dataVolumeUplink);
    ABTS_PTR_NOTNULL(tc, uuc->dataVolumeDownlink);
    ABTS_INT_EQUAL(tc, 2048000, (int)*uuc->dataVolumeDownlink);
    ABTS_PTR_NOTNULL(tc, uuc->dataTotalVolume);
    ABTS_INT_EQUAL(tc, 512000 + 2048000, (int)*uuc->dataTotalVolume);
    ABTS_PTR_NOTNULL(tc, uuc->serviceIdentifier);
    ABTS_INT_EQUAL(tc, 7, (int)*uuc->serviceIdentifier);

    /* pDUSessionChargingInformation [13] — what the session was. */
    ABTS_PTR_NOTNULL(tc, cr->pDUSessionChargingInformation);
    ABTS_INT_EQUAL(tc, 5,
            (int)cr->pDUSessionChargingInformation->pDUSessionId);
    ABTS_PTR_NOTNULL(tc,
            cr->pDUSessionChargingInformation->dataNetworkNameIdentifier);
    ABTS_STR_EQUAL(tc, "internet", (const char *)
            cr->pDUSessionChargingInformation
              ->dataNetworkNameIdentifier->buf);
    ABTS_PTR_NOTNULL(tc,
            cr->pDUSessionChargingInformation->networkSliceInstanceID);
    ABTS_INT_EQUAL(tc, 1, (int)
            cr->pDUSessionChargingInformation->networkSliceInstanceID->sST);
    ABTS_PTR_NOTNULL(tc,
            cr->pDUSessionChargingInformation->networkSliceInstanceID->sD);
    ABTS_INT_EQUAL(tc, 3, (int)
            cr->pDUSessionChargingInformation->networkSliceInstanceID
              ->sD->size);

    /* The GPSI arrived as msisdn-..., so it is an E.164 InvolvedParty. */
    ABTS_PTR_NOTNULL(tc, cr->pDUSessionChargingInformation->userIdentifier);
    ABTS_INT_EQUAL(tc, CHFCDR_InvolvedParty_PR_iSDN_E164,
            (int)cr->pDUSessionChargingInformation->userIdentifier->present);
    ABTS_STR_EQUAL(tc, "447700900001", (const char *)
            cr->pDUSessionChargingInformation->userIdentifier
              ->choice.iSDN_E164.buf);

    chf_cdr_record_free(rec);
    chf_sess_remove(sess);
}

/* ------------------------------------------------------------------ */
/* The encoding, and that it decodes back                               */
/* ------------------------------------------------------------------ */
static void test_asn1_ber_roundtrip(abts_case *tc, void *data)
{
    chf_sess_t *sess;
    CHFCDR_CHFRecord_t *rec;
    CHFCDR_CHFRecord_t *back = NULL;
    CHFCDR_ChargingRecord_t *cr;
    uint8_t *ber = NULL;
    int len;
    asn_dec_rval_t dr;

    sess = make_sess("imsi-001010000000002", "asn1-roundtrip-0002",
                     111111, 222222, 600);
    ABTS_PTR_NOTNULL(tc, sess);

    rec = chf_cdr_record_build(sess, "volumeLimit", 42);
    ABTS_PTR_NOTNULL(tc, rec);

    len = chf_cdr_record_encode_ber(rec, &ber);
    ABTS_INT_EQUAL(tc, 1, len > 0 ? 1 : 0);
    ABTS_PTR_NOTNULL(tc, ber);

    /*
     * The first bytes must be the context-specific constructed tag 200.
     * A tag that large needs the multi-byte form: 0xbf introduces it and
     * 0x81 0x48 carries 200 as two 7-bit groups ((1 << 7) | 0x48).  This
     * is the one part of the encoding a reader identifies a CHF CDR by,
     * so it is worth asserting on the bytes rather than trusting the
     * codec.
     */
    ABTS_INT_EQUAL(tc, 1, len > 3 ? 1 : 0);
    if (len > 3) {
        ABTS_INT_EQUAL(tc, 0xbf, ber[0]);
        ABTS_INT_EQUAL(tc, 0x81, ber[1]);
        ABTS_INT_EQUAL(tc, 0x48, ber[2]);
    }

    /* Decode it back with the same codec a mediation system would use. */
    dr = ber_decode(NULL, &asn_DEF_CHFCDR_CHFRecord, (void **)&back,
            ber, (size_t)len);
    ABTS_INT_EQUAL(tc, RC_OK, dr.code);
    ABTS_INT_EQUAL(tc, len, (int)dr.consumed);
    ABTS_PTR_NOTNULL(tc, back);

    cr = record_of(back);
    ABTS_PTR_NOTNULL(tc, cr);
    ABTS_INT_EQUAL(tc, 200, (int)cr->recordType);
    ABTS_INT_EQUAL(tc, 16, (int)cr->causeForRecClosing); /* volumeLimit */
    ABTS_INT_EQUAL(tc, 600, (int)cr->duration);
    ABTS_PTR_NOTNULL(tc, cr->localRecordSequenceNumber);
    ABTS_INT_EQUAL(tc, 42, (int)*cr->localRecordSequenceNumber);
    ABTS_PTR_NOTNULL(tc, cr->subscriberIdentifier);
    ABTS_STR_EQUAL(tc, "imsi-001010000000002",
            (const char *)cr->subscriberIdentifier->subscriptionIDData.buf);
    ABTS_PTR_NOTNULL(tc, cr->listOfMultipleUnitUsage);
    ABTS_INT_EQUAL(tc, 1, cr->listOfMultipleUnitUsage->list.count);
    ABTS_INT_EQUAL(tc, 111111, (int)*cr->listOfMultipleUnitUsage->list
            .array[0]->usedUnitContainers->list.array[0]->dataVolumeUplink);

    ASN_STRUCT_FREE(asn_DEF_CHFCDR_CHFRecord, back);
    ogs_free(ber);
    chf_cdr_record_free(rec);
    chf_sess_remove(sess);
}

/* ------------------------------------------------------------------ */
/* Volumes past 2^32                                                    */
/*                                                                      */
/* DataVolumeOctets is an unconstrained INTEGER, which asn1c maps to a   */
/* native long.  On the 64-bit targets open5gs builds for that holds     */
/* the full range; this is the test that would fail first if the type    */
/* were ever generated as a 32-bit one.                                 */
/* ------------------------------------------------------------------ */
static void test_asn1_volumes_past_32_bits(abts_case *tc, void *data)
{
    chf_sess_t *sess;
    CHFCDR_CHFRecord_t *rec;
    CHFCDR_CHFRecord_t *back = NULL;
    CHFCDR_UsedUnitContainer_t *uuc;
    uint8_t *ber = NULL;
    int len;
    asn_dec_rval_t dr;

    const uint64_t ul = 5ULL * 1024 * 1024 * 1024;   /* 5 GiB */
    const uint64_t dl = 9ULL * 1024 * 1024 * 1024;   /* 9 GiB */

    sess = make_sess("imsi-001010000000003", "asn1-bigvol-0003",
                     ul, dl, 86400);
    ABTS_PTR_NOTNULL(tc, sess);

    rec = chf_cdr_record_build(sess, "normalRelease", 7);
    ABTS_PTR_NOTNULL(tc, rec);

    len = chf_cdr_record_encode_ber(rec, &ber);
    ABTS_INT_EQUAL(tc, 1, len > 0 ? 1 : 0);

    dr = ber_decode(NULL, &asn_DEF_CHFCDR_CHFRecord, (void **)&back,
            ber, (size_t)len);
    ABTS_INT_EQUAL(tc, RC_OK, dr.code);
    ABTS_PTR_NOTNULL(tc, back);

    uuc = record_of(back)->listOfMultipleUnitUsage->list.array[0]
            ->usedUnitContainers->list.array[0];

    ABTS_INT_EQUAL(tc, 1, (uint64_t)*uuc->dataVolumeUplink == ul ? 1 : 0);
    ABTS_INT_EQUAL(tc, 1, (uint64_t)*uuc->dataVolumeDownlink == dl ? 1 : 0);
    ABTS_INT_EQUAL(tc, 1,
            (uint64_t)*uuc->dataTotalVolume == ul + dl ? 1 : 0);

    ASN_STRUCT_FREE(asn_DEF_CHFCDR_CHFRecord, back);
    ogs_free(ber);
    chf_cdr_record_free(rec);
    chf_sess_remove(sess);
}

/* ------------------------------------------------------------------ */
/* causeForRecClosing, both ways                                        */
/* ------------------------------------------------------------------ */
static void test_asn1_cause_mapping(abts_case *tc, void *data)
{
    /* The three TS 32.298 values the CHF can close a session for. */
    ABTS_INT_EQUAL(tc, 0,  (int)chf_cdr_cause_value("normalRelease"));
    ABTS_INT_EQUAL(tc, 16, (int)chf_cdr_cause_value("volumeLimit"));
    ABTS_INT_EQUAL(tc, 20,
            (int)chf_cdr_cause_value("managementIntervention"));

    /* No cause, and an unrecognised one, both record normalRelease
     * rather than inventing a value. */
    ABTS_INT_EQUAL(tc, 0, (int)chf_cdr_cause_value(NULL));
    ABTS_INT_EQUAL(tc, 0, (int)chf_cdr_cause_value("somethingElse"));

    ABTS_STR_EQUAL(tc, "normalRelease", chf_cdr_cause_name(0));
    ABTS_STR_EQUAL(tc, "volumeLimit", chf_cdr_cause_name(16));
    ABTS_STR_EQUAL(tc, "managementIntervention", chf_cdr_cause_name(20));

    /* A value the module does not name has no name, so the JSON writes
     * the number instead of a guess. */
    ABTS_PTR_EQUAL(tc, NULL, (void *)chf_cdr_cause_name(99));
}

/* ------------------------------------------------------------------ */
/* A session with nothing optional in it                                */
/*                                                                      */
/* The SMF need not send a GPSI, a DNN or a slice.  The record must      */
/* still be valid, which means every mandatory field present and the     */
/* absent ones simply absent.                                           */
/* ------------------------------------------------------------------ */
static void test_asn1_minimal_session(abts_case *tc, void *data)
{
    chf_sess_t *sess;
    CHFCDR_CHFRecord_t *rec;
    CHFCDR_ChargingRecord_t *cr;
    uint8_t *ber = NULL;
    int len;

    sess = chf_sess_add("imsi-001010000000004", "asn1-minimal-0004");
    ABTS_PTR_NOTNULL(tc, sess);
    sess->psi = 1;

    rec = chf_cdr_record_build(sess, NULL, 1);
    ABTS_PTR_NOTNULL(tc, rec);

    cr = record_of(rec);
    ABTS_PTR_NOTNULL(tc, cr);

    /* Present because mandatory. */
    ABTS_INT_EQUAL(tc, 200, (int)cr->recordType);
    ABTS_INT_EQUAL(tc, 1, cr->recordingNetworkFunctionID.size > 0 ? 1 : 0);
    ABTS_PTR_NOTNULL(tc, cr->nFunctionConsumerInformation);
    ABTS_INT_EQUAL(tc, 9, (int)cr->recordOpeningTime.size);

    /* Absent because the SMF said nothing. */
    ABTS_PTR_EQUAL(tc, NULL, cr->pDUSessionChargingInformation->userIdentifier);
    ABTS_PTR_EQUAL(tc, NULL, cr->pDUSessionChargingInformation->dataNetworkNameIdentifier);
    ABTS_PTR_EQUAL(tc, NULL, cr->pDUSessionChargingInformation->networkSliceInstanceID);
    ABTS_PTR_EQUAL(tc, NULL, cr->nFunctionConsumerInformation->networkFunctionName);

    /* And it still encodes. */
    len = chf_cdr_record_encode_ber(rec, &ber);
    ABTS_INT_EQUAL(tc, 1, len > 0 ? 1 : 0);

    if (ber)
        ogs_free(ber);
    chf_cdr_record_free(rec);
    chf_sess_remove(sess);
}

/* ------------------------------------------------------------------ */
/* All three closing causes survive the encoding                        */
/*                                                                      */
/* The cause is the one field a billing system keys revenue assurance   */
/* off, and it is an INTEGER with named values, so a mapping slip is    */
/* invisible in the JSON and silent in the BER.  Round-trip each.       */
/* ------------------------------------------------------------------ */
static void test_asn1_all_causes_round_trip(abts_case *tc, void *data)
{
    static const struct {
        const char *name;
        long value;
    } causes[] = {
        { "normalRelease",          0  },
        { "volumeLimit",            16 },
        { "managementIntervention", 20 },
    };
    size_t i;

    for (i = 0; i < sizeof(causes) / sizeof(causes[0]); i++) {
        chf_sess_t *sess;
        CHFCDR_CHFRecord_t *rec;
        CHFCDR_CHFRecord_t *back = NULL;
        uint8_t *ber = NULL;
        char ref[64];
        int len;
        asn_dec_rval_t dr;

        snprintf(ref, sizeof(ref), "asn1-cause-%zu", i);
        sess = make_sess("imsi-001010000000005", ref, 100, 200, 10);
        ABTS_PTR_NOTNULL(tc, sess);

        rec = chf_cdr_record_build(sess, causes[i].name, 1);
        ABTS_PTR_NOTNULL(tc, rec);
        ABTS_INT_EQUAL(tc, (int)causes[i].value,
                (int)record_of(rec)->causeForRecClosing);

        len = chf_cdr_record_encode_ber(rec, &ber);
        ABTS_INT_EQUAL(tc, 1, len > 0 ? 1 : 0);

        dr = ber_decode(NULL, &asn_DEF_CHFCDR_CHFRecord, (void **)&back,
                ber, (size_t)len);
        ABTS_INT_EQUAL(tc, RC_OK, dr.code);
        ABTS_PTR_NOTNULL(tc, back);
        ABTS_INT_EQUAL(tc, (int)causes[i].value,
                (int)record_of(back)->causeForRecClosing);

        /* And the name the JSON would print is the one we asked for. */
        ABTS_STR_EQUAL(tc, causes[i].name,
                chf_cdr_cause_name(record_of(back)->causeForRecClosing));

        ASN_STRUCT_FREE(asn_DEF_CHFCDR_CHFRecord, back);
        if (ber)
            ogs_free(ber);
        chf_cdr_record_free(rec);
        chf_sess_remove(sess);
    }
}

/* ------------------------------------------------------------------ */
/* The SliceDifferentiator on the wire                                  */
/*                                                                      */
/* A slice with an SD is the case the rig exercised but the unit tests   */
/* did not: the SD is three octets inside SingleNSSAI, and a byte-order */
/* or length slip would still decode as "some SD".  Assert the octets.  */
/* ------------------------------------------------------------------ */
static void test_asn1_slice_differentiator_octets(abts_case *tc, void *data)
{
    chf_sess_t *sess;
    CHFCDR_CHFRecord_t *rec;
    CHFCDR_CHFRecord_t *back = NULL;
    CHFCDR_SingleNSSAI_t *nssai;
    uint8_t *ber = NULL;
    int len;
    asn_dec_rval_t dr;

    sess = make_sess("imsi-001010000000006", "asn1-sd-0006", 100, 200, 10);
    ABTS_PTR_NOTNULL(tc, sess);
    sess->s_nssai.sst = 1;
    sess->s_nssai.sd.v = 0x010203;

    rec = chf_cdr_record_build(sess, "normalRelease", 1);
    ABTS_PTR_NOTNULL(tc, rec);

    /* Most significant octet first, as TS 23.003 writes an SD. */
    nssai = record_of(rec)->pDUSessionChargingInformation
                ->networkSliceInstanceID;
    ABTS_PTR_NOTNULL(tc, nssai);
    ABTS_PTR_NOTNULL(tc, nssai->sD);
    ABTS_INT_EQUAL(tc, 3, (int)nssai->sD->size);
    ABTS_INT_EQUAL(tc, 0x01, nssai->sD->buf[0]);
    ABTS_INT_EQUAL(tc, 0x02, nssai->sD->buf[1]);
    ABTS_INT_EQUAL(tc, 0x03, nssai->sD->buf[2]);

    len = chf_cdr_record_encode_ber(rec, &ber);
    ABTS_INT_EQUAL(tc, 1, len > 0 ? 1 : 0);

    dr = ber_decode(NULL, &asn_DEF_CHFCDR_CHFRecord, (void **)&back,
            ber, (size_t)len);
    ABTS_INT_EQUAL(tc, RC_OK, dr.code);
    ABTS_PTR_NOTNULL(tc, back);

    nssai = record_of(back)->pDUSessionChargingInformation
                ->networkSliceInstanceID;
    ABTS_PTR_NOTNULL(tc, nssai);
    ABTS_INT_EQUAL(tc, 1, (int)nssai->sST);
    ABTS_PTR_NOTNULL(tc, nssai->sD);
    ABTS_INT_EQUAL(tc, 3, (int)nssai->sD->size);
    ABTS_INT_EQUAL(tc, 0x01, nssai->sD->buf[0]);
    ABTS_INT_EQUAL(tc, 0x02, nssai->sD->buf[1]);
    ABTS_INT_EQUAL(tc, 0x03, nssai->sD->buf[2]);

    ASN_STRUCT_FREE(asn_DEF_CHFCDR_CHFRecord, back);
    if (ber)
        ogs_free(ber);
    chf_cdr_record_free(rec);
    chf_sess_remove(sess);
}

/* ------------------------------------------------------------------ */
/* The TS 32.298 TimeStamp is BCD, not binary                           */
/*                                                                      */
/* Nine octets: BCD YYMMDDhhmmss, an ASCII sign, then BCD hhmm of the    */
/* offset.  The CHF writes UTC, so the offset is +0000.  A binary        */
/* encoding would still be nine octets and still decode, so this is     */
/* worth asserting octet by octet.                                      */
/* ------------------------------------------------------------------ */
static void test_asn1_timestamp_is_bcd(abts_case *tc, void *data)
{
    chf_sess_t *sess;
    CHFCDR_CHFRecord_t *rec;
    CHFCDR_TimeStamp_t *ts;
    struct tm tm_utc;
    time_t when;
    int i;

    sess = make_sess("imsi-001010000000007", "asn1-bcd-0007", 0, 0, 0);
    ABTS_PTR_NOTNULL(tc, sess);

    /* A fixed instant, so the expected digits are known. */
    sess->period_start = 1767225600;    /* 2026-01-01T00:00:00Z */
    sess->duration = 0;

    rec = chf_cdr_record_build(sess, "normalRelease", 1);
    ABTS_PTR_NOTNULL(tc, rec);

    ts = &record_of(rec)->recordOpeningTime;
    ABTS_INT_EQUAL(tc, 9, (int)ts->size);

    when = sess->period_start;
    ABTS_PTR_NOTNULL(tc, gmtime_r(&when, &tm_utc));

    /* Each of the first six octets is two BCD digits, so every nibble is
     * a decimal digit — never 0x0a..0x0f. */
    for (i = 0; i < 6; i++) {
        ABTS_TRUE(tc, ((ts->buf[i] >> 4) & 0x0f) <= 9);
        ABTS_TRUE(tc, (ts->buf[i] & 0x0f) <= 9);
    }

    ABTS_INT_EQUAL(tc, (tm_utc.tm_year + 1900) % 100,
            ((ts->buf[0] >> 4) & 0x0f) * 10 + (ts->buf[0] & 0x0f));
    ABTS_INT_EQUAL(tc, tm_utc.tm_mon + 1,
            ((ts->buf[1] >> 4) & 0x0f) * 10 + (ts->buf[1] & 0x0f));
    ABTS_INT_EQUAL(tc, tm_utc.tm_mday,
            ((ts->buf[2] >> 4) & 0x0f) * 10 + (ts->buf[2] & 0x0f));
    ABTS_INT_EQUAL(tc, tm_utc.tm_hour,
            ((ts->buf[3] >> 4) & 0x0f) * 10 + (ts->buf[3] & 0x0f));
    ABTS_INT_EQUAL(tc, tm_utc.tm_min,
            ((ts->buf[4] >> 4) & 0x0f) * 10 + (ts->buf[4] & 0x0f));
    ABTS_INT_EQUAL(tc, tm_utc.tm_sec,
            ((ts->buf[5] >> 4) & 0x0f) * 10 + (ts->buf[5] & 0x0f));

    /* Sign is ASCII, offset is zero because the CHF writes UTC. */
    ABTS_INT_EQUAL(tc, '+', ts->buf[6]);
    ABTS_INT_EQUAL(tc, 0, ts->buf[7]);
    ABTS_INT_EQUAL(tc, 0, ts->buf[8]);

    chf_cdr_record_free(rec);
    chf_sess_remove(sess);
}

abts_suite *test_chf_cdr_asn1(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, test_asn1_record_fields,         NULL);
    abts_run_test(suite, test_asn1_ber_roundtrip,         NULL);
    abts_run_test(suite, test_asn1_volumes_past_32_bits,  NULL);
    abts_run_test(suite, test_asn1_cause_mapping,         NULL);
    abts_run_test(suite, test_asn1_minimal_session,       NULL);
    abts_run_test(suite, test_asn1_all_causes_round_trip, NULL);
    abts_run_test(suite, test_asn1_slice_differentiator_octets, NULL);
    abts_run_test(suite, test_asn1_timestamp_is_bcd,      NULL);

    return suite;
}
