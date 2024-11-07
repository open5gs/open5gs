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

#include "ogs-diameter-common.h"

static struct fd_hook_hdl *logger_hdl = NULL;
static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

static ogs_diam_logger_user_handler user_handler = NULL;

static void ogs_diam_logger_cb(enum fd_hook_type type, struct msg * msg,
    struct peer_hdr * peer, void * other, struct fd_hook_permsgdata *pmd,
    void * regdata);

int ogs_diam_logger_init()
{
    uint32_t mask_peers = HOOK_MASK( HOOK_PEER_CONNECT_SUCCESS );

    CHECK_FCT( fd_hook_register(
            mask_peers, ogs_diam_logger_cb, NULL, NULL, &logger_hdl) );

    return 0;
}

void ogs_diam_logger_final()
{
    if (logger_hdl) { CHECK_FCT_DO( fd_hook_unregister( logger_hdl ), ); }
}

void ogs_diam_logger_register(ogs_diam_logger_user_handler instance)
{
    user_handler = instance;
}

void ogs_diam_logger_unregister(void)
{
    user_handler = NULL;
}

/* The callback called when messages are received and sent */
static void ogs_diam_logger_cb(enum fd_hook_type type, struct msg * msg,
    struct peer_hdr * peer, void * other, struct fd_hook_permsgdata *pmd,
    void * regdata)
{
    const char * peer_name = peer ? peer->info.pi_diamid : "<unknown peer>";

    CHECK_POSIX_DO( pthread_mutex_lock(&mtx), );

    if (user_handler)
    {
        user_handler(type, msg, peer, other, pmd, regdata);
    }

    switch (type) {
/* peers */
    case HOOK_PEER_CONNECT_SUCCESS:
        {
            char protobuf[40];
            if (peer) {
                CHECK_FCT_DO(fd_peer_cnx_proto_info(peer, protobuf, sizeof(protobuf)), break );
            } else {
                protobuf[0] = '-';
                protobuf[1] = '\0';
            }
            ogs_info("CONNECTED TO '%s' (%s):", peer_name, protobuf);
        }
        break;
    default:
        ogs_warn("Unknown type(%d)", type);
        break;
    }

    CHECK_POSIX_DO( pthread_mutex_unlock(&mtx), );
}

