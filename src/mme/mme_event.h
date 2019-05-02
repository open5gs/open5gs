#ifndef __MME_EVENT_H__
#define __MME_EVENT_H__

#include "ogs-core.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* forward declaration */
typedef enum {
    MME_EVT_BASE = OGS_FSM_USER_SIG,

    MME_EVT_S1AP_MESSAGE,
    MME_EVT_S1AP_DELAYED_SEND,
    MME_EVT_S1AP_LO_ACCEPT,
    MME_EVT_S1AP_LO_SCTP_COMM_UP,
    MME_EVT_S1AP_LO_CONNREFUSED,

    MME_EVT_EMM_MESSAGE,
    MME_EVT_ESM_MESSAGE,
    MME_EVT_S11_MESSAGE,
    MME_EVT_S6A_MESSAGE,

    MME_EVT_TOP,

} mme_event_e;

typedef long S1AP_ProcedureCode_t;
typedef struct S1AP_S1AP_PDU s1ap_message_t;
typedef struct _nas_message_t nas_message_t;
typedef struct _mme_enb_t mme_enb_t;
typedef struct _enb_ue_t enb_ue_t;
typedef struct _mme_ue_t mme_ue_t;
typedef struct _mme_sess_t mme_sess_t;
typedef struct _mme_bearer_t mme_bearer_t;

typedef struct mme_event_s {
    int id;
    void *pkbuf;

    ogs_sock_t *enb_sock;
    ogs_sockaddr_t *enb_addr;
    uint16_t inbound_streams;
    uint16_t outbound_streams;

    S1AP_ProcedureCode_t s1ap_code;
    s1ap_message_t *s1ap_message;

    uint8_t nas_type;
    nas_message_t *nas_message;

    mme_enb_t *enb;
    enb_ue_t *enb_ue;
    mme_ue_t *mme_ue;
    mme_sess_t *sess;
    mme_bearer_t *bearer;

    ogs_timer_t *timer;
} mme_event_t;

void mme_event_init(void);
void mme_event_term(void);
void mme_event_final(void);

mme_event_t *mme_event_new(mme_event_e id);
void mme_event_free(mme_event_t *e);

void mme_event_timeout(void *data);

#define mme_event_send(__ptr_e) \
    ogs_assert(ogs_queue_push(mme_self()->queue, ((__ptr_e))) == OGS_OK)

const char *mme_event_get_name(mme_event_t *e);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __MME_EVENT_H__ */
