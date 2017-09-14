#define TRACE_MODULE _s1ap_path

#include "core_debug.h"

#include "mme_event.h"

#include "nas_security.h"
#include "nas_path.h"

#include "s1ap_conv.h"
#include "s1ap_build.h"
#include "s1ap_path.h"

static int _s1ap_accept_cb(net_sock_t *net_sock, void *data);

status_t s1ap_open(void)
{
    char buf[INET_ADDRSTRLEN];
    int rc;

    rc = net_listen_ext(&mme_self()->s1ap_sock, 
            SOCK_STREAM, IPPROTO_SCTP, SCTP_S1AP_PPID,
            mme_self()->s1ap_addr, mme_self()->s1ap_port);
    if (rc != 0)
    {
        d_error("Can't establish S1-ENB(port:%d) path(%d:%s)",
            mme_self()->s1ap_port, errno, strerror(errno));
        mme_self()->s1ap_sock = NULL;
        return CORE_ERROR;
    }

    rc = net_register_sock(
            mme_self()->s1ap_sock, _s1ap_accept_cb, NULL);
    if (rc != 0)
    {
        d_error("Can't establish S1-ENB path(%d:%s)",
            errno, strerror(errno));
        net_close(mme_self()->s1ap_sock);
        mme_self()->s1ap_sock = NULL;
        return CORE_ERROR;
    }

    d_trace(1, "s1_enb_listen() %s:%d\n", 
        INET_NTOP(&mme_self()->s1ap_addr, buf), mme_self()->s1ap_port);

    return CORE_OK;
}

status_t s1ap_close()
{
    d_assert(mme_self(), return CORE_ERROR, "Null param");
    d_assert(mme_self()->s1ap_sock != NULL, return CORE_ERROR,
            "S1-ENB path already opened");
    net_unregister_sock(mme_self()->s1ap_sock);
    net_close(mme_self()->s1ap_sock);
    mme_self()->s1ap_sock = NULL;

    return CORE_OK;
}

static int _s1ap_accept_cb(net_sock_t *net_sock, void *data)
{
    char buf[INET_ADDRSTRLEN];
    ssize_t r;
    net_sock_t *remote_sock;

    d_assert(net_sock, return -1, "Null param");

    r = net_accept(&remote_sock, net_sock, 0);
    if (r > 0)
    {
        d_trace(1, "eNB-S1 accepted[%s] in s1_path module\n", 
            INET_NTOP(&remote_sock->remote.sin_addr.s_addr, buf));

        event_t e;
        event_set(&e, MME_EVT_S1AP_LO_ACCEPT);
        event_set_param1(&e, (c_uintptr_t)remote_sock);
        /* FIXME : how to close remote_sock */
        mme_event_send(&e);
    }
    else
    {
        d_error("net_accept failed(r = %d, errno = %d)", r, errno);
    }

    return r;
}

static status_t s1ap_recv(net_sock_t *sock, pkbuf_t *pkbuf)
{
    event_t e;

    d_assert(sock, return CORE_ERROR, "Null param");
    d_assert(pkbuf, return CORE_ERROR, "Null param");

    d_trace(10, "S1AP_PDU is received from eNB-Inf\n");
    d_trace_hex(10, pkbuf->payload, pkbuf->len);

    event_set(&e, MME_EVT_S1AP_MESSAGE);
    event_set_param1(&e, (c_uintptr_t)sock->app_index);
    event_set_param2(&e, (c_uintptr_t)pkbuf);
    return mme_event_send(&e);
}

int _s1ap_recv_cb(net_sock_t *sock, void *data)
{
    status_t rv;
    pkbuf_t *pkbuf;
    ssize_t r;

    d_assert(sock, return -1, "Null param");

    pkbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    d_assert(pkbuf, return -1, "Can't allocate pkbufuf");

    r = net_read(sock, pkbuf->payload, pkbuf->len, 0);
    if (r == -2)
    {
        pkbuf_free(pkbuf);
    }
    else if (r <= 0)
    {
        pkbuf_free(pkbuf);

        if (sock->sndrcv_errno == EAGAIN)
        {
            d_warn("net_read failed(%d:%s)",
                    sock->sndrcv_errno, strerror(sock->sndrcv_errno));
            return 0;
        } 
        else if (sock->sndrcv_errno == ECONNREFUSED)
        {
            d_warn("net_read failed(%d:%s)",
                    sock->sndrcv_errno, strerror(sock->sndrcv_errno));
        }
        else
        {
            d_error("net_read failed(%d:%s)",
                    sock->sndrcv_errno, strerror(sock->sndrcv_errno));
        }

        event_t e;

        event_set(&e, MME_EVT_S1AP_LO_CONNREFUSED);
        event_set_param1(&e, (c_uintptr_t)sock->app_index);
        mme_event_send(&e);

        return -1;
    }
    else
    {
        pkbuf->len = r;

        rv = s1ap_recv(sock, pkbuf);
        if (rv != CORE_OK)
        {
            pkbuf_free(pkbuf);
            d_error("s1_recv() failed");
            return -1;
        }
    }

    return 0;
}

status_t s1ap_send(net_sock_t *s, pkbuf_t *pkbuf)
{
    char buf[INET_ADDRSTRLEN];

    ssize_t sent;

    d_assert(s, return CORE_ERROR, "Null param");
    d_assert(pkbuf, return CORE_ERROR, "Null param");

    sent = net_send(s, pkbuf->payload, pkbuf->len);
    d_trace(10,"Sent %d->%d bytes to [%s:%d]\n", 
            pkbuf->len, sent, INET_NTOP(&s->remote.sin_addr.s_addr, buf), 
            ntohs(s->remote.sin_port));
    d_trace_hex(10, pkbuf->payload, pkbuf->len);
    if (sent < 0 || sent != pkbuf->len)
    {
        d_error("net_send error (%d:%s)", 
                s->sndrcv_errno, strerror(s->sndrcv_errno));
        return CORE_ERROR;
    }
    pkbuf_free(pkbuf);

    return CORE_OK;
}

status_t s1ap_send_to_enb(mme_enb_t *enb, pkbuf_t *pkbuf)
{
    status_t rv = CORE_ERROR;
    d_assert(enb,,);
    d_assert(pkbuf,,);
    d_assert(enb->s1ap_sock,,);

    rv = s1ap_send(enb->s1ap_sock, pkbuf);
    if (rv != CORE_OK)
    {
        d_error("s1_send error");
        pkbuf_free(pkbuf);
    }

    return rv;
}

status_t s1ap_send_to_esm(mme_ue_t *mme_ue, pkbuf_t *esmbuf)
{
    event_t e;
    nas_esm_header_t *h = NULL;

    mme_sess_t *sess = NULL;
    mme_bearer_t *bearer = NULL;
    c_uint8_t pti = NAS_PROCEDURE_TRANSACTION_IDENTITY_UNASSIGNED;
    c_uint8_t ebi = NAS_EPS_BEARER_IDENTITY_UNASSIGNED;

    d_assert(mme_ue, return CORE_ERROR, "Null param");
    d_assert(esmbuf, return CORE_ERROR, "Null param");
    h = esmbuf->payload;
    d_assert(h, return CORE_ERROR, "Null param");

    pti = h->procedure_transaction_identity;
    ebi = h->eps_bearer_identity;

    if (h->message_type == NAS_PDN_DISCONNECT_REQUEST)
    {
        nas_pdn_disconnect_request_t *pdn_disconnect_request = 
            esmbuf->payload + sizeof(nas_esm_header_t);
        nas_linked_eps_bearer_identity_t *linked_eps_bearer_identity =
            &pdn_disconnect_request->linked_eps_bearer_identity;

        bearer = mme_bearer_find_by_ue_ebi(mme_ue,
                linked_eps_bearer_identity->eps_bearer_identity);
        d_assert(bearer, return CORE_ERROR,
                "Invalid pti(%d) and ebi(%d)\n", pti, ebi);
        sess = bearer->sess;
        d_assert(sess, return CORE_ERROR, "Null param");
        sess->pti = pti;
    }
    else
    {
        if (ebi != NAS_EPS_BEARER_IDENTITY_UNASSIGNED)
            bearer = mme_bearer_find_by_ue_ebi(mme_ue, ebi);
        else if (pti != NAS_PROCEDURE_TRANSACTION_IDENTITY_UNASSIGNED)
        {
            sess = mme_sess_find_by_pti(mme_ue, pti);
            if (!sess)
                sess = mme_sess_add(mme_ue, pti);
            d_assert(sess, return CORE_ERROR, "Null param");
            bearer = mme_default_bearer_in_sess(sess);
        }
        else
            d_assert(0, return CORE_ERROR,
                    "Invalid pti(%d) and ebi(%d)\n", pti, ebi);
    }
    d_assert(bearer, return CORE_ERROR, "Null param");

    event_set(&e, MME_EVT_ESM_MESSAGE);
    event_set_param1(&e, (c_uintptr_t)bearer->index);
    event_set_param2(&e, (c_uintptr_t)esmbuf);
    mme_event_send(&e);

    return CORE_OK;
}

status_t s1ap_send_to_nas(enb_ue_t *enb_ue, S1ap_NAS_PDU_t *nasPdu)
{
    nas_security_header_t *sh = NULL;
    nas_security_header_type_t security_header_type;

    nas_emm_header_t *h = NULL;
    pkbuf_t *nasbuf = NULL;
    event_t e;

    d_assert(enb_ue, return CORE_ERROR, "Null param");
    d_assert(nasPdu, return CORE_ERROR, "Null param");

    /* The Packet Buffer(pkbuf_t) for NAS message MUST make a HEADROOM. 
     * When calculating AES_CMAC, we need to use the headroom of the packet. */
    nasbuf = pkbuf_alloc(NAS_HEADROOM, nasPdu->size);
    d_assert(nasbuf, return CORE_ERROR, "Null param");
    memcpy(nasbuf->payload, nasPdu->buf, nasPdu->size);

    sh = nasbuf->payload;
    d_assert(sh, return CORE_ERROR, "Null param");

    memset(&security_header_type, 0, sizeof(nas_security_header_type_t));
    switch(sh->security_header_type)
    {
        case NAS_SECURITY_HEADER_PLAIN_NAS_MESSAGE:
            break;
        case NAS_SECURITY_HEADER_FOR_SERVICE_REQUEST_MESSAGE:
            security_header_type.service_request = 1;
            break;
        case NAS_SECURITY_HEADER_INTEGRITY_PROTECTED:
            security_header_type.integrity_protected = 1;
            d_assert(pkbuf_header(nasbuf, -6) == CORE_OK,
                    return CORE_ERROR, "pkbuf_header error");
            break;
        case NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_CIPHERED:
            security_header_type.integrity_protected = 1;
            security_header_type.ciphered = 1;
            d_assert(pkbuf_header(nasbuf, -6) == CORE_OK,
                    return CORE_ERROR, "pkbuf_header error");
            break;
        case NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_NEW_SECURITY_CONTEXT:
            security_header_type.integrity_protected = 1;
            security_header_type.new_security_context = 1;
            d_assert(pkbuf_header(nasbuf, -6) == CORE_OK,
                    return CORE_ERROR, "pkbuf_header error");
            break;
        case NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_CIPHTERD_WITH_NEW_INTEGRITY_CONTEXT:
            security_header_type.integrity_protected = 1;
            security_header_type.ciphered = 1;
            security_header_type.new_security_context = 1;
            d_assert(pkbuf_header(nasbuf, -6) == CORE_OK,
                    return CORE_ERROR, "pkbuf_header error");
            break;
        default:
            d_error("Not implemented(securiry header type:0x%x)", 
                    sh->security_header_type);
            return CORE_ERROR;
    }

    if (enb_ue->mme_ue)
    {
        d_assert(nas_security_decode(
            enb_ue->mme_ue, security_header_type, nasbuf) == CORE_OK,
            pkbuf_free(nasbuf);return CORE_ERROR, "nas_security_decode failed");
    }

    h = nasbuf->payload;
    d_assert(h, pkbuf_free(nasbuf); return CORE_ERROR, "Null param");
    if (h->protocol_discriminator == NAS_PROTOCOL_DISCRIMINATOR_EMM)
    {
        event_set(&e, MME_EVT_EMM_MESSAGE);
        event_set_param1(&e, (c_uintptr_t)enb_ue->index);
        event_set_param2(&e, (c_uintptr_t)security_header_type.type);
        event_set_param3(&e, (c_uintptr_t)nasbuf);
        mme_event_send(&e);
    }
    else if (h->protocol_discriminator == NAS_PROTOCOL_DISCRIMINATOR_ESM)
    {
        mme_ue_t *mme_ue = enb_ue->mme_ue;
        d_assert(mme_ue, return CORE_ERROR, "Null param");
        s1ap_send_to_esm(mme_ue, nasbuf);
    }
    else
        d_assert(0, pkbuf_free(nasbuf); return CORE_ERROR,
                "Unknown protocol:%d", h->protocol_discriminator);

    return CORE_OK;
}

status_t s1ap_send_initial_context_setup_request(mme_ue_t *mme_ue)
{
    status_t rv;
    pkbuf_t *s1apbuf = NULL;

    d_assert(mme_ue, return CORE_ERROR, "Null param");

    rv = s1ap_build_initial_context_setup_request(&s1apbuf, mme_ue, NULL);
    d_assert(rv == CORE_OK && s1apbuf, return CORE_ERROR, "s1ap build error");

    rv = nas_send_to_enb(mme_ue, s1apbuf);
    d_assert(rv == CORE_OK, return CORE_ERROR, "s1ap send error");

    return CORE_OK;
}

status_t s1ap_send_ue_context_release_commmand(
        enb_ue_t *enb_ue, S1ap_Cause_t *cause)
{
    status_t rv;
    mme_enb_t *enb = NULL;
    pkbuf_t *s1apbuf = NULL;

    d_assert(enb_ue, return CORE_ERROR, "Null param");
    d_assert(cause, return CORE_ERROR, "Null param");
    enb = enb_ue->enb;
    d_assert(enb, return CORE_ERROR, "Null param");

    rv = s1ap_build_ue_context_release_commmand(&s1apbuf, enb_ue, cause);
    d_assert(rv == CORE_OK && s1apbuf, return CORE_ERROR, "s1ap build error");

    rv = s1ap_send_to_enb(enb, s1apbuf);
    d_assert(rv == CORE_OK,, "s1ap send error");
    
    return CORE_OK;
}

status_t s1ap_send_path_switch_ack(mme_ue_t *mme_ue)
{
    status_t rv;
    pkbuf_t *s1apbuf = NULL;

    d_assert(mme_ue, return CORE_ERROR, "Null param");

    rv = s1ap_build_path_switch_ack(&s1apbuf, mme_ue);
    d_assert(rv == CORE_OK && s1apbuf, return CORE_ERROR, "s1ap build error");

    rv = nas_send_to_enb(mme_ue, s1apbuf);
    d_assert(rv == CORE_OK, return CORE_ERROR, "s1ap send error");
    
    return CORE_OK;
}

status_t s1ap_send_path_switch_failure(mme_enb_t *enb,
    c_uint32_t enb_ue_s1ap_id, c_uint32_t mme_ue_s1ap_id, S1ap_Cause_t *cause)
{
    status_t rv;
    pkbuf_t *s1apbuf = NULL;

    d_assert(cause, return CORE_ERROR, "Null param");
    d_assert(enb, return CORE_ERROR, "Null param");

    rv = s1ap_build_path_switch_failure(&s1apbuf,
            enb_ue_s1ap_id, mme_ue_s1ap_id, cause);
    d_assert(rv == CORE_OK && s1apbuf, return CORE_ERROR, "s1ap build error");

    rv = s1ap_send_to_enb(enb, s1apbuf);
    d_assert(rv == CORE_OK,, "s1ap send error");
    
    return rv;
}
status_t s1ap_send_handover_request(
        mme_ue_t *mme_ue, S1ap_HandoverRequiredIEs_t *ies)
{
    status_t rv;
    pkbuf_t *s1apbuf = NULL;
    S1ap_TargetID_t *targetID = NULL;

    c_uint32_t enb_id;
    mme_enb_t *target_enb = NULL;
    enb_ue_t *target_ue = NULL;

    d_assert(mme_ue, return CORE_ERROR,);
    d_assert(ies, return CORE_ERROR,);

    targetID = &ies->targetID;
    d_assert(targetID, return CORE_ERROR,);

    switch(targetID->present)
    {
        case S1ap_TargetID_PR_targeteNB_ID:
        {
            s1ap_ENB_ID_to_uint32(
                &targetID->choice.targeteNB_ID.global_S1ap_ENB_ID.eNB_ID,
                &enb_id);
            break;
        }
        default:
        {
            d_error("Not implemented(%d)", targetID->present);
            return CORE_ERROR;
        }
    }

    target_enb = mme_enb_find_by_enb_id(enb_id);
    d_assert(target_enb, return CORE_ERROR,
            "Cannot find target eNB = %d", enb_id);

    target_ue = enb_ue_add(target_enb);
    d_assert(target_ue, return CORE_ERROR,);

    mme_ue_associate_target_ue(mme_ue, target_ue);

    rv = s1ap_build_handover_request(&s1apbuf, mme_ue, target_ue, ies);
    d_assert(rv == CORE_OK && s1apbuf,
            enb_ue_remove(target_ue); return CORE_ERROR, "s1ap build error");

    rv = s1ap_send_to_enb(target_enb, s1apbuf);
    d_assert(rv == CORE_OK, enb_ue_remove(target_ue), "s1ap send error");

    return rv;
}

status_t s1ap_send_handover_command(mme_ue_t *mme_ue)
{
    status_t rv;
    pkbuf_t *s1apbuf = NULL;

    mme_enb_t *enb = NULL;
    enb_ue_t *enb_ue = NULL;

    d_assert(mme_ue, return CORE_ERROR,);
    enb_ue = mme_ue->enb_ue;
    d_assert(enb_ue, return CORE_ERROR,);
    enb = enb_ue->enb;
    d_assert(enb, return CORE_ERROR,);

    rv = s1ap_build_handover_command(&s1apbuf, enb_ue);
    d_assert(rv == CORE_OK && s1apbuf, return CORE_ERROR, "s1ap build error");

    rv = s1ap_send_to_enb(enb, s1apbuf);
    d_assert(rv == CORE_OK,, "s1ap send error");

    return rv;
}

status_t s1ap_send_mme_status_transfer(
        enb_ue_t *enb_ue, S1ap_ENBStatusTransferIEs_t *ies)
{
    status_t rv;
    pkbuf_t *s1apbuf = NULL;

    mme_enb_t *enb = NULL;

    d_assert(enb_ue, return CORE_ERROR,);
    enb = enb_ue->enb;
    d_assert(enb, return CORE_ERROR,);

    rv = s1ap_build_mme_status_transfer(&s1apbuf, enb_ue, ies);
    d_assert(rv == CORE_OK && s1apbuf, return CORE_ERROR, "s1ap build error");

    rv = s1ap_send_to_enb(enb, s1apbuf);
    d_assert(rv == CORE_OK,, "s1ap send error");

    return rv;
}
