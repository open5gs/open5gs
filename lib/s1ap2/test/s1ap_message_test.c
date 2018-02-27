#define TRACE_MODULE __test__test

#include "core_debug.h"
#include "core_pkbuf.h"
#include "core_lib.h"

#include "3gpp_types.h"

#include "s1ap_message.h"
#if 0
#include "mme/s1ap_build.h"
#endif
#include "s1ap_conv.h"

#include "testutil.h"
#if 0
#include "testpacket.h"
#endif

static void s1ap_message_test1(abts_case *tc, void *data)
{
    /* S1SetupRequest */
    char *payload = 
        "0011002d000004003b00090000f11040"
        "54f64010003c400903004a4c542d3632"
        "3100400007000c0e4000f11000894001"
        "00";

    pkbuf_t *pkbuf;
    s1ap_message_t message;
    struct S1AP_InitiatingMessage *initiatingMessage;
    S1AP_S1SetupRequest_t *pdu;
    int i;
    int result;
    char hexbuf[MAX_SDU_LEN];

    pkbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    ABTS_PTR_NOTNULL(tc, pkbuf);
    pkbuf->len = 49;
    memcpy(pkbuf->payload, 
            CORE_HEX(payload, strlen(payload), hexbuf), pkbuf->len);

    result = s1ap_decode_pdu(&message, pkbuf);
    ABTS_INT_EQUAL(tc, 0, result);

    initiatingMessage = message.choice.initiatingMessage;
    pdu = &initiatingMessage->value.choice.S1SetupRequest;

    asn_fprint(stdout, &asn_DEF_S1AP_S1AP_PDU, &message);

    for (i = 0; i < pdu->protocolIEs.list.count; i++)
    {
        S1AP_S1SetupRequestIEs_t *ie = 
            pdu->protocolIEs.list.array[i];
        switch(ie->value.present)
        {
            case S1SetupResponseIEs__value_PR_MMEname:
            {
                S1AP_Global_ENB_ID_t *Global_ENB_ID =
                    &ie->value.choice.Global_ENB_ID;
                asn_fprint(stdout, &asn_DEF_S1AP_Global_ENB_ID, Global_ENB_ID);
                break;
            }
            case S1SetupResponseIEs__value_PR_ServedGUMMEIs:
            {
                S1AP_ENBname_t *ENBname = 
                    &ie->value.choice.ENBname;
                asn_fprint(stdout, &asn_DEF_S1AP_ENBname, ENBname);
                break;
            }
            case S1SetupResponseIEs__value_PR_RelativeMMECapacity:
            {
                S1AP_SupportedTAs_t	*SupportedTAs =
                    &ie->value.choice.SupportedTAs;
                asn_fprint(stdout, &asn_DEF_S1AP_SupportedTAs, SupportedTAs);
                break;
            }
            case S1SetupResponseIEs__value_PR_MMERelaySupportIndicator:
            {
                S1AP_PagingDRX_t *PagingDRX =
                    &ie->value.choice.PagingDRX;
                asn_fprint(stdout, &asn_DEF_S1AP_PagingDRX, PagingDRX);
                break;
            }
            default:
                d_warn("Unknown Procotol ID[%d]\n", ie->id);
                break;
        }
    }

    s1ap_free_pdu(&message);

    pkbuf_free(pkbuf);
}

#if 0
static void s1ap_message_test1(abts_case *tc, void *data)
{
    /* S1SetupRequest */
    char *payload = 
        "0011002d000004003b00090000f11040"
        "54f64010003c400903004a4c542d3632"
        "3100400007000c0e4000f11000894001"
        "00";
#if 0
    char *payload = 
        "000e001500000200 0240020060005c00 084000005b000220 04";
    char *payload = 
        "000e0025"
        "0000020002400200 60005c0018400200 5b00046064000400 5b00046063000300"
        "5b0002402c";
    char *payload = 
        "000e"
        "0025000002000240 020000005c001840 02005b0004606400 04005b0004606300"
        "03005b0002402c";
#endif

    pkbuf_t *pkbuf;
    s1ap_message_t message;
    int result;
    char hexbuf[MAX_SDU_LEN];

    pkbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    ABTS_PTR_NOTNULL(tc, pkbuf);
    pkbuf->len = 49;
#if 0
    pkbuf->len = 25;
    pkbuf->len = 41;
    pkbuf->len = 41;
#endif
    memcpy(pkbuf->payload, 
            CORE_HEX(payload, strlen(payload), hexbuf), pkbuf->len);

    result = s1ap_decode_pdu(&message, pkbuf);
    ABTS_INT_EQUAL(tc, 0, result);
    s1ap_free_pdu(&message);

    pkbuf_free(pkbuf);
}

static void s1ap_message_test2(abts_case *tc, void *data)
{
    /* InitialUE(Attach Request) */
    char *payload = 
        "000c406f000006000800020001001a00"
        "3c3b17df675aa8050741020bf600f110"
        "000201030003e605f070000010000502"
        "15d011d15200f11030395c0a003103e5"
        "e0349011035758a65d0100e0c1004300"
        "060000f1103039006440080000f1108c"
        "3378200086400130004b00070000f110"
        "000201";

    s1ap_message_t message;
    pkbuf_t *pkbuf;
    int result;
    char hexbuf[MAX_SDU_LEN];

    pkbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    ABTS_PTR_NOTNULL(tc, pkbuf);
    pkbuf->len = 115;
    memcpy(pkbuf->payload, 
            CORE_HEX(payload, strlen(payload), hexbuf), pkbuf->len);

    result = s1ap_decode_pdu(&message, pkbuf);
    ABTS_INT_EQUAL(tc, 0, result);
    s1ap_free_pdu(&message);

    pkbuf_free(pkbuf);
}

static void s1ap_message_test3(abts_case *tc, void *data)
{
    /* initial context setup response */
    char *payload = 
        "2009002500000300004005c0020000bf"
        "0008400200010033400f000032400a0a"
        "1f0a0123c601000908";

    s1ap_message_t message;
    pkbuf_t *pkbuf;
    int result;
    char hexbuf[MAX_SDU_LEN];

    pkbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    ABTS_PTR_NOTNULL(tc, pkbuf);
    pkbuf->len = 41;
    memcpy(pkbuf->payload, 
            CORE_HEX(payload, strlen(payload), hexbuf), pkbuf->len);

    result = s1ap_decode_pdu(&message, pkbuf);
    ABTS_INT_EQUAL(tc, 0, result);
    s1ap_free_pdu(&message);

    pkbuf_free(pkbuf);
}

status_t s1ap_build_setup_failure(
        pkbuf_t **pkbuf, S1AP_Cause_PR group, long cause, long time_to_wait)
{
    int erval;

    s1ap_message_t message;
    struct S1AP_UnsuccessfulOutcome *unsuccessfulOutcome = NULL;
    S1AP_S1SetupFailure_t *s1SetupFailure = NULL;
    S1AP_S1SetupFailureIEs_t *ies = NULL;

    memset(&message, 0, sizeof(s1ap_message_t));

    unsuccessfulOutcome = message.choice.unsuccessfulOutcome;
    s1SetupFailure = &unsuccessfulOutcome->value.choice.S1SetupFailure;

    memset(&message, 0, sizeof(s1ap_message_t));

    ies = core_calloc(1, sizeof(S1AP_S1SetupFailureIEs_t));
    ies->value.choice.Cause.present = group;
    ies->value.choice.Cause.choice.radioNetwork = cause;
    d_trace(5, "    Gruop[%d] Cause[%d] TimeToWait[%ld]\n",
            group, cause, time_to_wait);

#if 0
    if (time_to_wait > -1)
    {
        ies->presenceMask |= S1AP_S1SETUPFAILUREIES_TIMETOWAIT_PRESENT;
        ies->timeToWait = time_to_wait;
    }
#endif
    printf("__LINE__ = %d\n", __LINE__);
    ASN_SEQUENCE_ADD(&s1SetupFailure, ies);

    printf("__LINE__ = %d\n", __LINE__);
    message.choice.unsuccessfulOutcome->procedureCode = 
        S1AP_ProcedureCode_S1AP_id_S1Setup;

    printf("__LINE__ = %d\n", __LINE__);
    erval = s1ap_encode_pdu(pkbuf, &message);
    printf("__LINE__ = %d\n", __LINE__);
    s1ap_free_pdu(&message);
    printf("__LINE__ = %d\n", __LINE__);

    if (erval < 0)
    {
        d_error("s1ap_encode_error : (%d)", erval);
        return CORE_ERROR;
    }

    return CORE_OK;
}

#define MAX_PLMN_ID                 6
#define GRP_PER_MME                 256    /* According to spec it is 65535 */
#define CODE_PER_MME                256    /* According to spec it is 256 */
typedef struct _served_gummei {
    c_uint32_t      num_of_plmn_id;
    plmn_id_t       plmn_id[MAX_PLMN_ID];

    c_uint32_t      num_of_mme_gid;
    c_uint16_t      mme_gid[GRP_PER_MME];
    c_uint32_t      num_of_mme_code;
    c_uint8_t       mme_code[CODE_PER_MME];
} served_gummei_t;

status_t s1ap_build_setup_rsp(pkbuf_t **pkbuf)
{
    int erval;
    int i, j;

    s1ap_message_t message;
    struct S1AP_SuccessfulOutcome *successfulOutcome = NULL;
    S1AP_S1SetupResponse_t *s1setupResponse = NULL;
    S1AP_S1SetupResponseIEs_t *ies = NULL;
    S1AP_ServedGUMMEIsItem_t *servedGUMMEI;
    S1AP_PLMNidentity_t *plmnIdentity;
    S1AP_MME_Group_ID_t *mmeGroupId;
    S1AP_MME_Code_t *mmeCode;

    memset(&message, 0, sizeof(s1ap_message_t));

    successfulOutcome = message.choice.successfulOutcome;
    s1setupResponse = &successfulOutcome->value.choice.S1SetupResponse;

    for (i = 0; i < 1; i++)
    {
        servedGUMMEI = (S1AP_ServedGUMMEIsItem_t *)
            core_calloc(1, sizeof(S1AP_ServedGUMMEIsItem_t));

        served_gummei_t *served_gummei = NULL, s;
        served_gummei = &s;
        memset(served_gummei, 0, sizeof(served_gummei_t));
        served_gummei->num_of_plmn_id = 1;
        plmn_id_build(&served_gummei->plmn_id[0], 1, 1, 2);
        served_gummei->num_of_mme_gid = 1;
        served_gummei->mme_gid[0] = 2;
        served_gummei->num_of_mme_code = 1;
        served_gummei->mme_code[0] = 1;

        for (j = 0; j < served_gummei->num_of_plmn_id; j++)
        {
            plmnIdentity = (S1AP_PLMNidentity_t *)
                core_calloc(1, sizeof(S1AP_PLMNidentity_t));
            s1ap_buffer_to_OCTET_STRING(
                    &served_gummei->plmn_id[j], PLMN_ID_LEN, plmnIdentity);
            ASN_SEQUENCE_ADD(&servedGUMMEI->servedPLMNs, plmnIdentity);
            d_trace(5, "    PLMN_ID[MCC:%d MNC:%d]\n",
                plmn_id_mcc(&served_gummei->plmn_id[j]),
                plmn_id_mnc(&served_gummei->plmn_id[j]));
        }

        for (j = 0; j < served_gummei->num_of_mme_gid; j++)
        {
            mmeGroupId = (S1AP_MME_Group_ID_t *)
                core_calloc(1, sizeof(S1AP_MME_Group_ID_t));
            s1ap_uint16_to_OCTET_STRING(
                served_gummei->mme_gid[j], mmeGroupId);
            ASN_SEQUENCE_ADD(&servedGUMMEI->servedGroupIDs, mmeGroupId);
            d_trace(5, "    MME Group[%d]\n", served_gummei->mme_gid[j]);
        }

        for (j = 0; j < served_gummei->num_of_mme_code; j++)
        {
            mmeCode = (S1AP_MME_Code_t *)
                core_calloc(1, sizeof(S1AP_MME_Code_t));
            s1ap_uint8_to_OCTET_STRING(
                served_gummei->mme_code[j], mmeCode);
            ASN_SEQUENCE_ADD(&servedGUMMEI->servedMMECs, mmeCode);
            d_trace(5, "    MME Code[%d]\n", served_gummei->mme_code[j]);
        }
        ASN_SEQUENCE_ADD(&s1setupResponse->protocolIEs.list, servedGUMMEI);
    }

    ies->relativeMMECapacity = 44;

    message.procedureCode = S1ap_ProcedureCode_id_S1Setup;
    message.direction = S1AP_PDU_PR_successfulOutcome;

    erval = s1ap_encode_pdu(pkbuf, &message);
    s1ap_free_pdu(&message);

    if (erval < 0)
    {
        d_error("s1ap_encode_error : (%d)", erval);
        return CORE_ERROR;
    }

    return CORE_OK;
}

static void s1ap_message_test4(abts_case *tc, void *data)
{
    s1ap_message_t message;
    status_t rv;
    pkbuf_t *pkbuf;
    int result;

    rv = s1ap_build_setup_failure(&pkbuf, 2, 0, 0);

    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    ABTS_PTR_NOTNULL(tc, pkbuf);
    ABTS_PTR_NOTNULL(tc, pkbuf->payload);
#if 0
    ABTS_INT_EQUAL(tc, 27, pkbuf->len);

    result = s1ap_decode_pdu(&message, pkbuf);
    ABTS_INT_EQUAL(tc, 0, result);

    s1ap_free_pdu(&message);
    pkbuf_free(pkbuf);
#endif
}

static void s1ap_message_test5(abts_case *tc, void *data)
{
    s1ap_message_t message;
    status_t rv;
    pkbuf_t *pkbuf;
    int result;

    rv = tests1ap_build_setup_req(&pkbuf, S1ap_ENB_ID_PR_macroENB_ID, 0x54f64);

    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    ABTS_PTR_NOTNULL(tc, pkbuf);
    ABTS_PTR_NOTNULL(tc, pkbuf->payload);
    ABTS_INT_EQUAL(tc, 35, pkbuf->len);

    result = s1ap_decode_pdu(&message, pkbuf);
    ABTS_INT_EQUAL(tc, 0, result);

    s1ap_free_pdu(&message);
    pkbuf_free(pkbuf);
}

static void s1ap_message_test6(abts_case *tc, void *data)
{
    pkbuf_t *s1apbuf = NULL;
    int i;
    s1ap_message_t message;
    S1ap_DownlinkNASTransport_IEs_t *ies = 
        &message.s1ap_DownlinkNASTransport_IEs;
    S1ap_NAS_PDU_t *nasPdu = &ies->nas_pdu;
    char buffer[1024];
    char *_result = 
    "000b4080 8c000003 00000002 00010008 00020001 001a0079 78efefef efefefef"
    "efefefef efefefef efefefef efefefef efefefef efefefef efefefef efefefef"
    "efefefef efefefef efefefef efefefef efefefef efefefef efefefef efefefef"
    "efefefef efefefef efefefef efefefef efefefef efefefef efefefef efefefef"
    "efefefef efefefef efefefef efefefef ef";

    memset(&message, 0, sizeof(s1ap_message_t));

    ies->mme_ue_s1ap_id = 1;
    ies->eNB_UE_S1AP_ID = 1;

    nasPdu->size = 120;
    nasPdu->buf = core_calloc(nasPdu->size, sizeof(c_uint8_t));
    for (i = 0; i < nasPdu->size; i++)
        nasPdu->buf[i] = 0xef;

    message.procedureCode = S1ap_ProcedureCode_id_downlinkNASTransport;
    message.direction = S1AP_PDU_PR_initiatingMessage;

    s1ap_encode_pdu(&s1apbuf, &message);
    s1ap_free_pdu(&message);
    ABTS_TRUE(tc, memcmp(CORE_HEX(_result, strlen(_result), buffer),
            s1apbuf->payload, s1apbuf->len) == 0);

    pkbuf_free(s1apbuf);
}
#endif

abts_suite *test_s1ap_message(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, s1ap_message_test1, NULL);
#if 0
    abts_run_test(suite, s1ap_message_test2, NULL);
    abts_run_test(suite, s1ap_message_test3, NULL);
    abts_run_test(suite, s1ap_message_test4, NULL);
    abts_run_test(suite, s1ap_message_test5, NULL);
    abts_run_test(suite, s1ap_message_test6, NULL);
#endif

    return suite;
}
