#define TRACE_MODULE _s1enbmsg

#include "core_debug.h"
#include "core_pkbuf.h"
#include "core_lib.h"

#include "context.h"
#include "s1ap_build.h"
#include "s1ap_conv.h"
#include "s1ap_path.h"

net_sock_t *tests1ap_enb_connect(void)
{
    status_t rv;
    mme_ctx_t *mme = mme_self();
    net_sock_t *sock = NULL;

    if (!mme) return NULL;

    rv = net_open_with_addr(&sock, mme->mme_local_addr, "127.0.0.1", 0, 
            mme->s1ap_port, SOCK_SEQPACKET, IPPROTO_SCTP, 0);
    if (rv != CORE_OK) return NULL;

    return sock;
}

status_t tests1ap_enb_close(net_sock_t *sock)
{
    return net_close(sock);
}

int tests1ap_enb_send(net_sock_t *sock, pkbuf_t *sendbuf)
{
    return s1ap_send(sock, sendbuf);
}

int tests1ap_enb_read(net_sock_t *sock, pkbuf_t *recvbuf)
{
    int rc = 0;

    while(1)
    {
        rc = net_read(sock, recvbuf->payload, recvbuf->len, 0);
        if (rc == -2) 
        {
            continue;
        }
        else if (rc <= 0)
        {
            if (sock->sndrcv_errno == EAGAIN)
            {
                continue;
            }
            break;
        }
        else
        {
            break;
        }
    }

    return rc;
}


status_t tests1ap_build_setup_req(pkbuf_t **pkbuf, c_uint32_t enb_id)
{
    int erval = -1;

    s1ap_message_t message;
    S1ap_S1SetupRequestIEs_t *ies;
    S1ap_PLMNidentity_t *plmnIdentity;
    S1ap_SupportedTAs_Item_t *supportedTA;

    memset(&message, 0, sizeof(s1ap_message_t));

    ies = &message.s1ap_S1SetupRequestIEs;

    s1ap_uint32_to_ENB_ID(S1ap_ENB_ID_PR_macroENB_ID, enb_id, 
        &ies->global_ENB_ID.eNB_ID);
    s1ap_buffer_to_OCTET_STRING(
        &mme_self()->plmn_id, PLMN_ID_LEN, &ies->global_ENB_ID.pLMNidentity);

    supportedTA = (S1ap_SupportedTAs_Item_t *)
        core_calloc(1, sizeof(S1ap_SupportedTAs_Item_t));
    s1ap_uint16_to_OCTET_STRING(
            mme_self()->tracking_area_code, &supportedTA->tAC);
    plmnIdentity = (S1ap_PLMNidentity_t *)
        core_calloc(1, sizeof(S1ap_PLMNidentity_t));
    s1ap_buffer_to_OCTET_STRING(
        &mme_self()->plmn_id, PLMN_ID_LEN, plmnIdentity);
    ASN_SEQUENCE_ADD(&supportedTA->broadcastPLMNs, plmnIdentity);

    ASN_SEQUENCE_ADD(&ies->supportedTAs, supportedTA);

    ies->defaultPagingDRX = mme_self()->default_paging_drx;

    message.direction = S1AP_PDU_PR_initiatingMessage;
    message.procedureCode = S1ap_ProcedureCode_id_S1Setup;

    erval = s1ap_encode_pdu(pkbuf, &message);
    s1ap_free_pdu(&message);

    if (erval < 0)
    {
        d_error("s1ap_encode_error : (%d)", erval);
        return CORE_ERROR;
    }

    return CORE_OK;
}


status_t tests1ap_build_initial_ue_msg(pkbuf_t **pkbuf)
{
    char *payload = 
        "000c405800000500 0800020001001a00"
        "302f177ca0b38802 0741020809101010"
        "3254869104e060c0 4000050221d011d1"
        "5c0a003103e5e034 9011035758a65d01"
        "00004300060000f1 105ba00064400800"
        "00f1101079baf000 86400130";
    char hexbuf[MAX_SDU_LEN];
    
    *pkbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    if (!(*pkbuf)) return CORE_ERROR;

    (*pkbuf)->len = 92;
    memcpy((*pkbuf)->payload, CORE_HEX(payload, strlen(payload), hexbuf),
            (*pkbuf)->len);

    return CORE_OK;
}

status_t tests1ap_build_authentication_response(pkbuf_t **pkbuf)
{
    char *payload = 
        "000d403e00000500 000005c00100009d"
        "000800020001001a 001211177c0bca9d"
        "030753086a91970e 838fd07900644008"
        "0000f1101079baf0 004340060000f110"
        "5ba0";
    char hexbuf[MAX_SDU_LEN];
    
    *pkbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    if (!(*pkbuf)) return CORE_ERROR;

    (*pkbuf)->len = 66;
    memcpy((*pkbuf)->payload, CORE_HEX(payload, strlen(payload), hexbuf),
            (*pkbuf)->len);

    return CORE_OK;
}

status_t tests1ap_build_security_mode_complete(pkbuf_t **pkbuf)
{
    char *payload = 
        "000d403500000500 000005c00100009d"
        "000800020001001a 000908476b8f5f64"
        "00075e0064400800 00f1101079baf000"
        "4340060000f1105b a0";
    char hexbuf[MAX_SDU_LEN];
    
    *pkbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    if (!(*pkbuf)) return CORE_ERROR;

    (*pkbuf)->len = 57;
    memcpy((*pkbuf)->payload, CORE_HEX(payload, strlen(payload), hexbuf),
            (*pkbuf)->len);

    return CORE_OK;
}
