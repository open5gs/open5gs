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

#include "ogs-sctp.h"

int __ogs_sctp_domain;

void ogs_sctp_set_option(ogs_sockopt_t *option, ogs_socknode_t *node)
{
    ogs_assert(option);

    if (node) {
        if (node->option.sctp.heartbit_interval) {
            option->sctp.heartbit_interval =
                node->option.sctp.heartbit_interval;
            ogs_debug("[SCTP] heartbit_interval - [%d]",
                    option->sctp.heartbit_interval);
        }
        if (node->option.sctp.rto_initial) {
            option->sctp.rto_initial = node->option.sctp.rto_initial;
            ogs_debug("[SCTP] rto_initial - [%d]",
                    option->sctp.rto_initial);
        }
        if (node->option.sctp.rto_min) {
            option->sctp.rto_min = node->option.sctp.rto_min;
            ogs_debug("[SCTP] rto_min - [%d]",
                    option->sctp.rto_min);
        }
        if (node->option.sctp.rto_max) {
            option->sctp.rto_max = node->option.sctp.rto_max;
            ogs_debug("[SCTP] rto_max - [%d]", option->sctp.rto_max);
        }
        if (node->option.sctp.max_num_of_ostreams) {
            option->sctp.max_num_of_ostreams =
                node->option.sctp.max_num_of_ostreams;
            ogs_debug("[SCTP] max_num_of_ostreams - [%d]",
                    option->sctp.max_num_of_ostreams);
        }
        if (node->option.sctp.max_num_of_istreams) {
            option->sctp.max_num_of_istreams =
                node->option.sctp.max_num_of_istreams;
            ogs_debug("[SCTP] max_num_of_istreams - [%d]",
                    option->sctp.max_num_of_istreams);
        }
        if (node->option.sctp.max_attempts) {
            option->sctp.max_attempts = node->option.sctp.max_attempts;
            ogs_debug("[SCTP] max_attempts - [%d]",
                    option->sctp.max_attempts);
        }
        if (node->option.sctp.max_initial_timeout) {
            option->sctp.max_initial_timeout =
                node->option.sctp.max_initial_timeout;
            ogs_debug("[SCTP] max_initial_timeout - [%d]",
                    option->sctp.max_initial_timeout);
        }
    }
}

int ogs_sctp_recvdata(ogs_sock_t *sock, void *msg, size_t len,
        ogs_sockaddr_t *from, ogs_sctp_info_t *sinfo)
{
    int size;
    int flags = 0;

    do {
        size = ogs_sctp_recvmsg(sock, msg, len, from, sinfo, &flags);
        if (size < 0) {
            ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
                    "ogs_sctp_recvdata(%d)", size);
            return size;
        }

        if (flags & MSG_NOTIFICATION) {
            /* Nothing */
        }
        else if (flags & MSG_EOR) {
            break;
        }
        else {
            ogs_assert_if_reached();
        }
    } while(1);

    return size;
}
