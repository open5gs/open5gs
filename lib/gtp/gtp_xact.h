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

#ifndef __GTP_XACT_H__
#define __GTP_XACT_H__

#include "gtp_message.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct gtp_node_s gtp_node_t;

/**
 * Transaction context
 */
typedef struct gtp_xact_s {
    ogs_lnode_t     node;           /**< A node of list */
    ogs_index_t     index;
    
#define GTP_LOCAL_ORIGINATOR  0
#define GTP_REMOTE_ORIGINATOR 1
    uint8_t         org;            /**< Transaction' originator. 
                                         local or remote */

    uint32_t        xid;            /**< Transaction ID */
    gtp_node_t      *gnode;         /**< Relevant GTP node context */

    int             step;           /**< Current step in the sequence.
                                         1 : Initial 
                                         2 : Triggered 
                                         3 : Triggered-Reply */
    struct {
        uint8_t     type;           /**< Message type history */
        ogs_pkbuf_t *pkbuf;         /**< Packet history */
    } seq[3];                       /**< history for the each step */

    ogs_timer_t     *tm_response;   /**< Timer waiting for next message */
    uint8_t         response_rcount;
    ogs_timer_t     *tm_holding;    /**< Timer waiting for holding message */
    uint8_t         holding_rcount;

    struct gtp_xact_s *assoc_xact; /**< Associated transaction */

#define GTP_XACT_STORE_SESSION(xact, session) \
    do { \
        ogs_assert((xact)); \
        ogs_assert((session)); \
        ((xact)->sess) = (session); \
    } while(0)

#define GTP_XACT_RETRIEVE_SESSION(xact) ((xact) ? ((xact)->sess) : NULL)
    void            *sess;          /**< Session Store */
} gtp_xact_t;

int gtp_xact_init(ogs_timer_mgr_t *timer_mgr);
int gtp_xact_final(void);

gtp_xact_t *gtp_xact_local_create(
        gtp_node_t *gnode, gtp_header_t *hdesc, ogs_pkbuf_t *pkbuf);
gtp_xact_t *gtp_xact_remote_create(
        gtp_node_t *gnode, uint32_t sqn);
void gtp_xact_delete_all(gtp_node_t *gnode);

int gtp_xact_update_tx(gtp_xact_t *xact,
        gtp_header_t *hdesc, ogs_pkbuf_t *pkbuf);
int gtp_xact_update_rx(gtp_xact_t *xact, uint8_t type);

int gtp_xact_commit(gtp_xact_t *xact);

int gtp_xact_receive(gtp_node_t *gnode, gtp_header_t *h, gtp_xact_t **xact);

gtp_xact_t *gtp_xact_find(ogs_index_t index);
gtp_xact_t *gtp_xact_find_by_xid(
        gtp_node_t *gnode, uint8_t type, uint32_t xid);
void gtp_xact_associate(gtp_xact_t *xact1, gtp_xact_t *xact2);
void gtp_xact_deassociate(gtp_xact_t *xact1, gtp_xact_t *xact2);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __GTP_XACT_H__ */
