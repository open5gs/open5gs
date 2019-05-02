#include "fd/s6a/s6a_message.h"
#include "nas_path.h"
#include "s1ap_path.h"

#include "mme_sm.h"
#include "mme_s6a_handler.h"

void mme_s6a_handle_aia(mme_ue_t *mme_ue, s6a_aia_message_t *aia_message)
{
    int rv;
    e_utran_vector_t *e_utran_vector = NULL;

    ogs_assert(mme_ue);
    ogs_assert(aia_message);
    e_utran_vector = &aia_message->e_utran_vector;
    ogs_assert(e_utran_vector);

    mme_ue->xres_len = e_utran_vector->xres_len;
    memcpy(mme_ue->xres, e_utran_vector->xres, mme_ue->xres_len);
    memcpy(mme_ue->kasme, e_utran_vector->kasme, OGS_SHA256_DIGEST_SIZE);
    memcpy(mme_ue->rand, e_utran_vector->rand, RAND_LEN);

    rv = nas_send_authentication_request(mme_ue, e_utran_vector);
    ogs_assert(rv == OGS_OK);
}

void mme_s6a_handle_ula(mme_ue_t *mme_ue, s6a_ula_message_t *ula_message)
{
    s6a_subscription_data_t *subscription_data = NULL;

    ogs_assert(mme_ue);
    ogs_assert(ula_message);
    subscription_data = &ula_message->subscription_data;
    ogs_assert(subscription_data);

    memcpy(&mme_ue->subscription_data,
            subscription_data, sizeof(s6a_subscription_data_t));
}
