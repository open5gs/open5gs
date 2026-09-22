/*
 * Copyright (C) 2019-2022 by Sukchan Lee <acetcom@gmail.com>
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

#ifndef UDR_CONTEXT_H
#define UDR_CONTEXT_H

#include "ogs-app.h"
#include "ogs-dbi.h"
#include "ogs-sbi.h"

#include "udr-sm.h"

#ifdef __cplusplus
extern "C" {
#endif

extern int __udr_log_domain;

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __udr_log_domain

#define UDR_MAX_NUM_OF_MONITORED_RESOURCE_URI 8

/*
 * In-memory representation of a Nudr_DataRepository change subscription
 * (SubscriptionDataSubscriptions). Not persisted; consumers are expected
 * to re-create their subscription after a UDR restart.
 */
typedef struct udr_subscription_s {
    ogs_lnode_t lnode;

    char *id;                          /* UDR-allocated subscriptionId */
    char *callback_reference;
    char *original_callback_reference; /* optional */
    char *ue_id;                       /* optional */
    ogs_time_t expiry;                 /* 0 if not set */

    int num_of_monitored_resource_uri;
    char *monitored_resource_uri[UDR_MAX_NUM_OF_MONITORED_RESOURCE_URI];

    void *client;                      /* SBI client bound to callback_reference */
} udr_subscription_t;

typedef struct udr_context_s {
    int                 use_mongodb_change_stream;
    ogs_thread_mutex_t  db_lock;

    ogs_list_t          subscription_list;
} udr_context_t;

void udr_context_init(void);
void udr_context_final(void);
udr_context_t *udr_self(void);

int udr_context_parse_config(void);

int udr_db_poll_change_stream(void);
int udr_handle_change_event(const bson_t *document);

udr_subscription_t *udr_subscription_add(void);
udr_subscription_t *udr_subscription_find_by_id(const char *id);
void udr_subscription_remove(udr_subscription_t *subscription);
void udr_subscription_remove_all(void);

#ifdef __cplusplus
}
#endif

#endif /* UDR_CONTEXT_H */
