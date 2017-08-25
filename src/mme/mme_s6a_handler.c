#define TRACE_MODULE _mme_s6a_handler

#include "core_debug.h"

#include "mme_s6a_handler.h"
#include "s6a_message.h"
#include "nas_path.h"

void mme_s6a_handle_aia(mme_ue_t *mme_ue, s6a_aia_message_t *aia_message)
{
    pkbuf_t *emmbuf = NULL;

    nas_message_t message;
    nas_authentication_request_t *authentication_request = 
        &message.emm.authentication_request;
    e_utran_vector_t *e_utran_vector = NULL;

    d_assert(mme_ue, return, "Null param");
    d_assert(aia_message, return, "Null param");
    e_utran_vector = &aia_message->e_utran_vector;
    d_assert(e_utran_vector, return, "Null param");

    mme_ue->xres_len = e_utran_vector->xres_len;
    memcpy(mme_ue->xres, e_utran_vector->xres, mme_ue->xres_len);
    memcpy(mme_ue->kasme, e_utran_vector->kasme, SHA256_DIGEST_SIZE);

    d_trace(3, "[NAS] Authentication request : UE[%s] <-- EMM\n",
             mme_ue->imsi_bcd);

    memset(&message, 0, sizeof(message));
    message.emm.h.protocol_discriminator = NAS_PROTOCOL_DISCRIMINATOR_EMM;
    message.emm.h.message_type = NAS_AUTHENTICATION_REQUEST;

    memcpy(authentication_request->authentication_parameter_rand.rand,
            e_utran_vector->rand, RAND_LEN);
    memcpy(authentication_request->authentication_parameter_autn.autn,
            e_utran_vector->autn, AUTN_LEN);
    authentication_request->authentication_parameter_autn.length = 
            AUTN_LEN;

    d_assert(nas_plain_encode(&emmbuf, &message) == CORE_OK && emmbuf,,);
    d_assert(nas_send_to_downlink_nas_transport(mme_ue, emmbuf) == CORE_OK,,);
}

void mme_s6a_handle_ula(mme_ue_t *mme_ue, s6a_ula_message_t *ula_message)
{
    s6a_subscription_data_t *subscription_data = NULL;

    d_assert(mme_ue, return, "Null param");
    d_assert(ula_message, return, "Null param");
    subscription_data = &ula_message->subscription_data;
    d_assert(subscription_data, return, "Null param");

    memcpy(&mme_ue->subscription_data,
            subscription_data, sizeof(s6a_subscription_data_t));
}
