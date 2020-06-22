/*
 * Copyright (C) 2019,2020 by Sukchan Lee <acetcom@gmail.com>
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

#include "amf-path.h"

void amf_send_delete_session_or_de_register(amf_ue_t *amf_ue)
{
    ogs_assert(amf_ue);

    if (SESSION_CONTEXT_IS_AVAILABLE(amf_ue)) {
        amf_sbi_send_release_all_sm_contexts(amf_ue);
    } else {
        nas_5gs_send_de_registration_accept(amf_ue);
    }
}

void amf_send_delete_session_or_amf_ue_context_release(amf_ue_t *amf_ue)
{
    ogs_assert(amf_ue);

    if (SESSION_CONTEXT_IS_AVAILABLE(amf_ue)) {
        amf_sbi_send_release_all_sm_contexts(amf_ue);
    } else {
        ran_ue_t *ran_ue = amf_ue->ran_ue;
        if (ran_ue) {
            ngap_send_ue_context_release_command(ran_ue,
                    NGAP_Cause_PR_nas, NGAP_CauseNas_normal_release,
                    NGAP_UE_CTX_REL_UE_CONTEXT_REMOVE, 0);
        } else {
            ogs_warn("No NG Context");
        }
    }
}

void amf_send_delete_session_or_ran_ue_context_release(ran_ue_t *ran_ue)
{
    amf_ue_t *amf_ue = NULL;
    ogs_assert(ran_ue);

    amf_ue = ran_ue->amf_ue;
    if (amf_ue && SESSION_CONTEXT_IS_AVAILABLE(amf_ue)) {
        amf_sbi_send_release_all_sm_contexts(amf_ue);
    } else {
        ngap_send_ue_context_release_command(ran_ue,
                NGAP_Cause_PR_nas, NGAP_CauseNas_normal_release,
                NGAP_UE_CTX_REL_NG_CONTEXT_REMOVE, 0);
    }
}
