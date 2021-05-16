/*
 * Copyright (C) 2019 by Sukchan Lee <acetcom@gmail.com>
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

#include "nas-path.h"
#include "s1ap-path.h"

#include "mme-sm.h"
#include "mme-s6a-handler.h"

void mme_s6a_handle_aia(mme_ue_t *mme_ue,
        ogs_diam_s6a_aia_message_t *aia_message)
{
    ogs_diam_e_utran_vector_t *e_utran_vector = NULL;

    ogs_assert(mme_ue);
    ogs_assert(aia_message);
    e_utran_vector = &aia_message->e_utran_vector;
    ogs_assert(e_utran_vector);

    mme_ue->xres_len = e_utran_vector->xres_len;
    memcpy(mme_ue->xres, e_utran_vector->xres, mme_ue->xres_len);
    memcpy(mme_ue->kasme, e_utran_vector->kasme, OGS_SHA256_DIGEST_SIZE);
    memcpy(mme_ue->rand, e_utran_vector->rand, OGS_RAND_LEN);
    memcpy(mme_ue->autn, e_utran_vector->autn, OGS_AUTN_LEN);

    CLEAR_MME_UE_TIMER(mme_ue->t3460);

    if (mme_ue->nas_eps.ksi == OGS_NAS_KSI_NO_KEY_IS_AVAILABLE)
        mme_ue->nas_eps.ksi = 0;

    ogs_assert(OGS_OK ==
        nas_eps_send_authentication_request(mme_ue));
}

void mme_s6a_handle_ula(mme_ue_t *mme_ue,
        ogs_diam_s6a_ula_message_t *ula_message)
{
    ogs_subscription_data_t *subscription_data = NULL;
    ogs_slice_data_t *slice_data = NULL;
    int i;

    ogs_assert(mme_ue);
    ogs_assert(ula_message);
    subscription_data = &ula_message->subscription_data;
    ogs_assert(subscription_data);

    ogs_assert(subscription_data->num_of_slice == 1);
    slice_data = &subscription_data->slice[0];

    memcpy(&mme_ue->ambr, &subscription_data->ambr, sizeof(ogs_bitrate_t));

    mme_session_remove_all(mme_ue);

    mme_ue->num_of_session = slice_data->num_of_session;
    mme_ue->context_identifier = slice_data->context_identifier;

    for (i = 0; i < slice_data->num_of_session; i++) {
        mme_ue->session[i].name = ogs_strdup(slice_data->session[i].name);
        ogs_assert(mme_ue->session[i].name);

        mme_ue->session[i].context_identifier =
            slice_data->session[i].context_identifier;

        mme_ue->session[i].session_type = slice_data->session[i].session_type;
        memcpy(&mme_ue->session[i].paa, &slice_data->session[i].paa,
                sizeof(mme_ue->session[i].paa));

        memcpy(&mme_ue->session[i].qos, &slice_data->session[i].qos,
                sizeof(mme_ue->session[i].qos));
        memcpy(&mme_ue->session[i].ambr, &slice_data->session[i].ambr,
                sizeof(mme_ue->session[i].ambr));

        memcpy(&mme_ue->session[i].smf_ip, &slice_data->session[i].smf_ip,
                sizeof(mme_ue->session[i].smf_ip));
    }
}
