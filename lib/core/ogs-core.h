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

#ifndef OGS_CORE_H
#define OGS_CORE_H

#include "core/core-config.h"

#define OGS_CORE_INSIDE

#define OGS_USE_TALLOC 1

#include "core/ogs-compat.h"
#include "core/ogs-macros.h"
#include "core/ogs-list.h"
#include "core/ogs-pool.h"
#include "core/ogs-abort.h"
#include "core/ogs-strings.h"
#include "core/ogs-errno.h"
#include "core/ogs-time.h"
#include "core/ogs-conv.h"
#include "core/ogs-log.h"
#include "core/ogs-pkbuf.h"
#include "core/ogs-memory.h"
#include "core/ogs-rand.h"
#include "core/ogs-uuid.h"
#include "core/ogs-rbtree.h"
#include "core/ogs-timer.h"
#include "core/ogs-thread.h"
#include "core/ogs-process.h"
#include "core/ogs-signal.h"
#include "core/ogs-sockaddr.h"
#include "core/ogs-socket.h"
#include "core/ogs-sockopt.h"
#include "core/ogs-sockpair.h"
#include "core/ogs-socknode.h"
#include "core/ogs-udp.h"
#include "core/ogs-tcp.h"
#include "core/ogs-queue.h"
#include "core/ogs-poll.h"
#include "core/ogs-notify.h"
#include "core/ogs-tlv.h"
#include "core/ogs-tlv-msg.h"
#include "core/ogs-env.h"
#include "core/ogs-fsm.h"
#include "core/ogs-hash.h"
#include "core/ogs-misc.h"
#include "core/ogs-getopt.h"
#include "core/ogs-3gpp-types.h"

#undef OGS_CORE_INSIDE

#ifdef __cplusplus
extern "C" {
#endif

extern int __ogs_mem_domain;
extern int __ogs_sock_domain;
extern int __ogs_event_domain;
extern int __ogs_thread_domain;
extern int __ogs_tlv_domain;

typedef struct {
    struct {
        int pool;
        int domain_pool;
        ogs_log_level_e level;
    } log;

    struct {
        int pool;
        int config_pool;
    } pkbuf;

    struct {
        int pool;
    } tlv;

} ogs_core_context_t;

void ogs_core_initialize(void);
void ogs_core_terminate(void);

ogs_core_context_t *ogs_core(void);

#ifdef __cplusplus
}
#endif

#endif /* OGS_CORE_H */
