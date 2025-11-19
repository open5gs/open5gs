/*
 * Copyright (C) 2019 by Sukchan Lee <acetcom@gmail.com>
 * Copyright (C) 2022 by sysmocom - s.f.m.c. GmbH <info@sysmocom.de>
 * Copyright (C) 2023 by Sukchan Lee <acetcom@gmail.com>
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

#if !defined(OGS_GTP_INSIDE) && !defined(OGS_GTP_COMPILATION)
#error "This header cannot be included directly."
#endif

#ifndef OGS_GTP_XACT_H
#define OGS_GTP_XACT_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * p225-226 Chapter 7.6 in TS 29.274 V15.9.0
 *
 * A Sequence Number used for a Command message shall have the most significant
 * bit set to 1. A Sequence Number in a message, which was triggered by
 * a Command message, as well as respective Triggered Reply message
 * shall have the same Sequence Number as the Command message
 * (i.e. shall also have the most significant bit set to 1).
 *
 * This setting of the most significant bit of the Sequence Number is done
 * to avoid potential clashes between the Sequence Number selected for
 * a Command message, and the Sequence Number selected by a GTPv2 peer
 * for a Request message, which was not triggered by a Command message.
 *
 * A Sequence Number used for a Request message, which was not triggered
 * by a Command message shall have the most significant bit set to 0.
 */
#define OGS_GTP_MIN_XACT_ID             1
#define OGS_GTP_CMD_XACT_ID             0x800000

#define OGS_GTP1_MIN_XACT_ID             0
#define OGS_GTP1_MAX_XACT_ID             65535

/**
 * Transaction context
 */
typedef struct ogs_gtp_xact_s {
    ogs_lnode_t     node;           /**< A node of list */

    ogs_pool_id_t   id;

    /*
     * Issues #3240
     *
     * SMF->SGW-C->MME: First Update Bearer Request
     * MME->UE:         First Modify EPS bearer context request
     * SMF->SGW-C->MME: Second Update Bearer Request
     * MME->UE:         Second Modify EPS bearer context request
     * UE->MME:         First Modify EPS bearer context accept
     * MME->SGW-C->SMF: First Update Bearer Response
     * UE->MME:         Second Modify EPS bearer context accept
     * MME->SGW-C->SMF: Second Update Bearer Response
     *
     * In the above situation, while NAS-ESM messages are exchanged
     * between the MME and UE, the bearer may have multiple transactions
     * that need to be managed. to_update_node is used as a node
     * in the Transaction List related to Update Bearer Request/Response.
     */
    ogs_lnode_t     to_update_node;

    ogs_pool_id_t   index;

    uint8_t gtp_version;            /**< 1 or 2 */

#define OGS_GTP_LOCAL_ORIGINATOR  0
#define OGS_GTP_REMOTE_ORIGINATOR 1
    uint8_t         org;            /**< Transaction' originator.
                                         local or remote */

    uint32_t        xid;            /**< Transaction ID */
    ogs_gtp_node_t  *gnode;         /**< Relevant GTP node context */

    void (*cb)(ogs_gtp_xact_t *, void *); /**< Local timer expiration handler */
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

    ogs_timer_t     *tm_peer;       /**< Timer waiting for peer message */
    void (*peer_cb)(ogs_gtp_xact_t *, void *); /**< timer expiration handler */
    void            *peer_data;     /**< Peer timeout data */

    uint32_t        local_teid;     /**< Local TEID,
                                         expected in reply from peer */

    ogs_pool_id_t   assoc_xact_id;  /**< Associated GTP transaction ID */
    void            *pfcp_xact;     /**< Associated PFCP transaction */

    ogs_pool_id_t   enb_ue_id;

#define OGS_GTP_MODIFY_TFT_UPDATE ((uint64_t)1<<0)
#define OGS_GTP_MODIFY_QOS_UPDATE ((uint64_t)1<<1)
    uint64_t        update_flags;

#define OGS_GTP_DELETE_NO_ACTION                                1
#define OGS_GTP_DELETE_SEND_AUTHENTICATION_REQUEST              2
#define OGS_GTP_DELETE_SEND_DETACH_ACCEPT                       3
#define OGS_GTP_DELETE_SEND_DEACTIVATE_BEARER_CONTEXT_REQUEST   4
#define OGS_GTP_DELETE_SEND_RELEASE_WITH_UE_CONTEXT_REMOVE      5
#define OGS_GTP_DELETE_SEND_RELEASE_WITH_S1_REMOVE_AND_UNLINK   6
#define OGS_GTP_DELETE_HANDLE_PDN_CONNECTIVITY_REQUEST          7
#define OGS_GTP_DELETE_IN_PATH_SWITCH_REQUEST                   8

    int             delete_action;

#define OGS_GTP_RELEASE_SEND_UE_CONTEXT_RELEASE_COMMAND     1
#define OGS_GTP_RELEASE_S1_CONTEXT_REMOVE_BY_LO_CONNREFUSED 2
#define OGS_GTP_RELEASE_S1_CONTEXT_REMOVE_BY_RESET_ALL      3
#define OGS_GTP_RELEASE_S1_CONTEXT_REMOVE_BY_RESET_PARTIAL  4
    int             release_action;

#define OGS_GTP_DELETE_INDIRECT_HANDOVER_COMPLETE 1
#define OGS_GTP_DELETE_INDIRECT_HANDOVER_CANCEL 2
    int             delete_indirect_action;

#define OGS_GTP_CREATE_IN_ATTACH_REQUEST 1
#define OGS_GTP_CREATE_IN_UPLINK_NAS_TRANSPORT 2
#define OGS_GTP_CREATE_IN_PATH_SWITCH_REQUEST 3
#define OGS_GTP_CREATE_IN_TRACKING_AREA_UPDATE 4 /* 3GPP TS 33.401 9.1.2 */
    int             create_action;

#define OGS_GTP_MODIFY_IN_PATH_SWITCH_REQUEST 1
#define OGS_GTP_MODIFY_IN_E_RAB_MODIFICATION 2
    int             modify_action;
} ogs_gtp_xact_t;

int ogs_gtp_xact_init(void);
void ogs_gtp_xact_final(void);

ogs_gtp_xact_t *ogs_gtp1_xact_local_create(ogs_gtp_node_t *gnode,
        ogs_gtp1_header_t *hdesc, ogs_pkbuf_t *pkbuf,
        void (*cb)(ogs_gtp_xact_t *xact, void *data), void *data);
ogs_gtp_xact_t *ogs_gtp_xact_local_create(ogs_gtp_node_t *gnode,
        ogs_gtp2_header_t *hdesc, ogs_pkbuf_t *pkbuf,
        void (*cb)(ogs_gtp_xact_t *xact, void *data), void *data);

ogs_gtp_xact_t *ogs_gtp_xact_find_by_id(ogs_pool_id_t id);
void ogs_gtp_xact_delete_all(ogs_gtp_node_t *gnode);

int ogs_gtp1_xact_update_tx(ogs_gtp_xact_t *xact,
        ogs_gtp1_header_t *hdesc, ogs_pkbuf_t *pkbuf);
int ogs_gtp_xact_update_tx(ogs_gtp_xact_t *xact,
        ogs_gtp2_header_t *hdesc, ogs_pkbuf_t *pkbuf);

int ogs_gtp_xact_commit(ogs_gtp_xact_t *xact);

int ogs_gtp1_xact_receive(ogs_gtp_node_t *gnode,
        ogs_gtp1_header_t *h, ogs_gtp_xact_t **xact);
int ogs_gtp_xact_receive(ogs_gtp_node_t *gnode,
        ogs_gtp2_header_t *h, ogs_gtp_xact_t **xact);

void ogs_gtp_xact_associate(ogs_gtp_xact_t *xact1, ogs_gtp_xact_t *xact2);
void ogs_gtp_xact_deassociate(ogs_gtp_xact_t *xact1, ogs_gtp_xact_t *xact2);

#ifdef __cplusplus
}
#endif

#endif /* OGS_GTP_XACT_H */
