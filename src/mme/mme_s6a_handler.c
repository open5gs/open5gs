#define TRACE_MODULE _mme_s6a_handler

#include "core_debug.h"

#include "fd/s6a/s6a_message.h"
#include "nas_path.h"
#include "s1ap_path.h"

#include "mme_sm.h"
#include "mme_s6a_handler.h"

void mme_s6a_handle_aia(mme_ue_t *mme_ue, s6a_aia_message_t *aia_message)
{
    status_t rv;
    e_utran_vector_t *e_utran_vector = NULL;

    d_assert(mme_ue, return, "Null param");
    d_assert(aia_message, return, "Null param");
    e_utran_vector = &aia_message->e_utran_vector;
    d_assert(e_utran_vector, return, "Null param");

    mme_ue->xres_len = e_utran_vector->xres_len;
    memcpy(mme_ue->xres, e_utran_vector->xres, mme_ue->xres_len);
    memcpy(mme_ue->kasme, e_utran_vector->kasme, SHA256_DIGEST_SIZE);
    memcpy(mme_ue->rand, e_utran_vector->rand, RAND_LEN);

    rv = nas_send_authentication_request(mme_ue, e_utran_vector);
    d_assert(rv == CORE_OK,, "nas send failed");
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
