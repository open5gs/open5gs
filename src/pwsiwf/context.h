/*
 * Copyright (C) 2019-2025 by Sukchan Lee <acetcom@gmail.com>
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

#ifndef PWSIWS_CONTEXT_H
#define PWSIWS_CONTEXT_H

#include "ogs-app.h"
#include "ogs-sbi.h"
#include "ogs-sctp.h"
#include "ogs-ngap.h"
#include "sbcap/ogs-sbcap.h"

#ifdef __cplusplus
extern "C" {
#endif

extern int __pwsiws_log_domain;

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __pwsiws_log_domain

typedef struct pwsiws_connection_s pwsiws_connection_t;
typedef struct pwsiws_warning_s pwsiws_warning_t;

/* PWS-IWS Connection Structure */
typedef struct pwsiws_connection_s {
    ogs_lnode_t     lnode;

    ogs_pool_id_t   id;

    ogs_fsm_t       sm;         /* A state machine */

    ogs_sctp_sock_t sctp;       /* SCTP socket */
    ogs_sockaddr_t  *addr;      /* Remote address */

    struct {
        bool connected;         /* Connection established */
        bool authenticated;     /* Authentication completed */
    } state;

    int             max_num_of_ostreams;/* SCTP Max num of outbound streams */
    uint16_t        ostream_id;         /* ostream_id generator */

    /* Warning message queue */
    ogs_list_t      warning_list;

    /* Add TAI support fields for SBc handler compatibility */
    int num_of_supported_ta_list;
    ogs_eps_tai_t supported_ta_list[16];

} pwsiws_connection_t;

/* PWS-IWS Warning Message Structure */
typedef struct pwsiws_warning_s {
    ogs_lnode_t     lnode;

    ogs_pool_id_t   id;

    uint32_t        warning_id;         /* Unique warning identifier */
    uint32_t        message_id;         /* Message identifier */

    /* Warning message content */
    struct {
        // uint8_t     message_type;       /* Warning message type */
        // uint16_t    serial_number;      /* Serial number */
        // uint8_t     message_identifier[50]; /* Message identifier */
        // uint8_t     data_coding_scheme; /* Data coding scheme */
        // uint8_t     warning_security_information[50]; /* Security info */
        // uint8_t     channel_indicator;  /* Channel indicator */
        // uint8_t     warning_area_list[100]; /* Warning area list */
        // uint8_t     repetition_period;  /* Repetition period */
        // uint8_t     number_of_broadcasts_requested; /* Broadcast count */
        // uint8_t     warning_message_content[960]; /* Message content */
        // uint8_t     message_content_length; /* Content length */

        uint8_t     message_type;       /* Warning message type */
        uint16_t message_id;
        uint16_t serial_number;
        uint8_t warning_type;
        uint32_t no_of_tai;
        uint32_t repetition_period;
        uint32_t number_of_broadcast;
        uint8_t data_coding_scheme;
        uint32_t message_length;
        uint8_t message_contents[1024]; /* TODO: max 9600 */
    } warning_data;

    /* Broadcast status */
    struct {
        bool        broadcast_started;
        uint32_t    broadcast_count;
        uint32_t    max_broadcasts;
        ogs_time_t  last_broadcast_time;
        ogs_time_t  next_broadcast_time;
    } broadcast;

    /* Related context */
    ogs_pool_id_t   connection_id;

    /* SBI callback URIs */
    char            *warning_status_uri;
    char            *paging_nonuen2message_location;

} pwsiws_warning_t;

/* PWS-IWS Context Structure */
typedef struct pwsiws_context_s {
    /* PWS-IWS Configuration */
    uint16_t        pws_iws_port;       /* PWS-IWS SCTP Port */
    uint16_t        sbcap_port;         /* SBCAP Port */

    /* Connection Management */
    ogs_list_t      connection_list;    /* PWS-IWS Connection List */
    ogs_list_t      warning_list;       /* Active Warning List */

    ogs_hash_t      *connection_addr_hash; /* hash table for Connection Address */
    ogs_hash_t      *warning_id_hash;   /* hash table for Warning ID */

    /* SCTP Server Lists */
    ogs_list_t      pws_iws_list;       /* PWS-IWS IPv4 Server List */
    ogs_list_t      pws_iws_list6;      /* PWS-IWS IPv6 Server List */

    /* SBCAP Server Lists */
    ogs_list_t      sbcap_list;         /* SBCAP IPv4 Server List */

    /* Generator for unique identification */
    uint32_t        warning_id_generator; /* Warning ID generator */
    uint32_t        message_id_generator; /* Message ID generator */

    /* SBI client for AMF */
    ogs_sbi_client_t *amf_sbi;

    /* Timer configuration */
    struct {
        struct {
            ogs_time_t value;           /* Timer Value(Seconds) */
        } warning_broadcast, connection_timeout;
    } time;

} pwsiws_context_t;

void pwsiws_context_init(void);
void pwsiws_context_final(void);
pwsiws_context_t *pwsiws_self(void);

int pwsiws_context_parse_config(void);
int pwsiws_context_nf_info(void);

/* Connection Management */
pwsiws_connection_t *pwsiws_connection_add(ogs_sock_t *sock, ogs_sockaddr_t *addr);
void pwsiws_connection_remove(pwsiws_connection_t *connection);
void pwsiws_connection_remove_all(void);
pwsiws_connection_t *pwsiws_connection_find_by_addr(ogs_sockaddr_t *addr);
pwsiws_connection_t *pwsiws_connection_find_by_id(ogs_pool_id_t id);

/* Warning Message Management */
pwsiws_warning_t *pwsiws_warning_add(pwsiws_connection_t *connection);
void pwsiws_warning_remove(pwsiws_warning_t *warning);
void pwsiws_warning_remove_all(void);
pwsiws_warning_t *pwsiws_warning_find_by_id(ogs_pool_id_t id);
pwsiws_warning_t *pwsiws_warning_find_by_warning_id(uint32_t warning_id);
pwsiws_warning_t *pwsiws_warning_find_by_paging_nonuen2message_location(char *location);

/* Warning Message SBI Functions */
void pwsiws_warning_set_paging_nonuen2message_location(pwsiws_warning_t *warning, char *location);

/* Utility Functions */
uint32_t pwsiws_warning_id_alloc(void);
uint32_t pwsiws_message_id_alloc(void);

#ifdef __cplusplus
}
#endif

#endif /* PWSIWS_CONTEXT_H */ 