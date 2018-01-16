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
    status_t rv;
    s6a_subscription_data_t *subscription_data = NULL;

    d_assert(mme_ue, return, "Null param");
    d_assert(ula_message, return, "Null param");
    subscription_data = &ula_message->subscription_data;
    d_assert(subscription_data, return, "Null param");

    memcpy(&mme_ue->subscription_data,
            subscription_data, sizeof(s6a_subscription_data_t));

    if (FSM_CHECK(&mme_ue->sm, emm_state_initial_context_setup))
    {
        if (mme_ue->nas_eps.type == MME_EPS_TYPE_ATTACH_REQUEST)
        {
            rv = nas_send_emm_to_esm(mme_ue, &mme_ue->pdn_connectivity_request);
            d_assert(rv == CORE_OK,, "nas_send_emm_to_esm failed");
        }
        else if (mme_ue->nas_eps.type == MME_EPS_TYPE_TAU_REQUEST)
        {
            rv = nas_send_tau_accept(mme_ue);
            d_assert(rv == CORE_OK, return, "nas send failed");
        }
        else if (mme_ue->nas_eps.type == MME_EPS_TYPE_SERVICE_REQUEST)
        {
            rv = s1ap_send_initial_context_setup_request(mme_ue);
            d_assert(rv == CORE_OK, return, "s1ap send failed");
        }
        else
            d_assert(0,, "Invalid Type(%d)", mme_ue->nas_eps.type);
    }
    else
        d_assert(0,, "Invaild EMM state");
}
