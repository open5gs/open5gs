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

bool ogs_diam_peer_connected(void)
{
    struct fd_list *li;
    bool connected = false;

	CHECK_POSIX( pthread_rwlock_rdlock(&fd_g_peers_rw) );
    for (li = fd_g_peers.next; li != &fd_g_peers; li = li->next) {
        struct peer_hdr *p = (struct peer_hdr *)li->o;
        int state = fd_peer_get_state(p);

        ogs_assert(p);
        ogs_assert(p->info.pi_diamid);
        ogs_assert(p->info.pi_diamidlen);

        if (state == STATE_OPEN) {
            ogs_debug("'%s' STATE is OPEN", p->info.pi_diamid);
            connected = true;
        } else {
            ogs_debug("'%s' STATE[%d] is NOT open ", p->info.pi_diamid, state);
        }
    }
	CHECK_POSIX( pthread_rwlock_unlock(&fd_g_peers_rw) );

    return connected;
}

