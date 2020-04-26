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

#if !defined(OGS_PFCP_INSIDE) && !defined(OGS_PFCP_COMPILATION)
#error "This header cannot be included directly."
#endif

#ifndef OGS_PFCP_XACT_H
#define OGS_PFCP_XACT_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Transaction context
 */
typedef struct ogs_pfcp_xact_s {
    ogs_lnode_t     lnode;          /**< A node of list */
    ogs_index_t     index;
    
#define OGS_PFCP_LOCAL_ORIGINATOR  0
#define OGS_PFCP_REMOTE_ORIGINATOR 1
    uint8_t         org;            /**< Transaction' originator. 
                                         local or remote */

    uint32_t        xid;            /**< Transaction ID */
    ogs_pfcp_node_t *node;       /**< Relevant PFCP node context */

    void (*cb)(ogs_pfcp_xact_t *, void *); /**< Local timer expiration handler */
    void            *data;          /**< Transaction Data */

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

    void            *assoc_xact;    /**< Associated GTP transaction */
} ogs_pfcp_xact_t;

int ogs_pfcp_xact_init(ogs_timer_mgr_t *timer_mgr, int size);
int ogs_pfcp_xact_final(void);

ogs_pfcp_xact_t *ogs_pfcp_xact_local_create(ogs_pfcp_node_t *node,
        ogs_pfcp_header_t *hdesc, ogs_pkbuf_t *pkbuf,
        void (*cb)(ogs_pfcp_xact_t *xact, void *data), void *data);
ogs_pfcp_xact_t *ogs_pfcp_xact_remote_create(
        ogs_pfcp_node_t *node, uint32_t sqn);
void ogs_pfcp_xact_delete_all(ogs_pfcp_node_t *node);

int ogs_pfcp_xact_update_tx(ogs_pfcp_xact_t *xact,
        ogs_pfcp_header_t *hdesc, ogs_pkbuf_t *pkbuf);
int ogs_pfcp_xact_update_rx(ogs_pfcp_xact_t *xact, uint8_t type);

int ogs_pfcp_xact_commit(ogs_pfcp_xact_t *xact);

int ogs_pfcp_xact_receive(ogs_pfcp_node_t *node,
        ogs_pfcp_header_t *h, ogs_pfcp_xact_t **xact);

ogs_pfcp_xact_t *ogs_pfcp_xact_find(ogs_index_t index);
ogs_pfcp_xact_t *ogs_pfcp_xact_find_by_xid(
        ogs_pfcp_node_t *node, uint8_t type, uint32_t xid);

#ifdef __cplusplus
}
#endif

#endif /* OGS_PFCP_XACT_H */
