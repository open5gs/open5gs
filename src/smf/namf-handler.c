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

#include "sbi-path.h"
#include "binding.h"
#include "namf-handler.h"

bool smf_namf_comm_handler_n1_n2_message_transfer(
        smf_sess_t *sess, int state, ogs_sbi_message_t *recvmsg)
{
    ogs_assert(sess);
    ogs_assert(state);
    ogs_assert(recvmsg);

    switch (state) {
    case SMF_UE_REQUESTED_PDU_SESSION_ESTABLISHMENT:
        smf_qos_flow_binding(sess, NULL);
        break;

    case SMF_NETWORK_REQUESTED_QOS_FLOW_MODIFICATION:
        /* Nothing */
        break;

    default:
        ogs_fatal("Unexpected state [%d]", state);
        ogs_assert_if_reached();
    }

    return true;
}
