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
    ogs_pfcp_node_t *node;          /**< Relevant PFCP node context */

    /**< Local timer expiration handler & Data*/
    void (*cb)(ogs_pfcp_xact_t *, void *);
    void            *data;

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

    ogs_timer_t     *tm_delayed_commit; /**< Timer waiting for commit xact */

    void            *assoc_xact;    /**< Associated GTP transaction */
    ogs_pkbuf_t     *gtpbuf;        /**< GTP packet buffer */

    uint8_t         gtp_pti;        /**< GTP Procedure transaction identity */
    uint8_t         gtp_cause;      /**< GTP Cause Value */

    void            *assoc_stream;  /**< Associated SBI session */

    bool            epc;            /**< EPC or 5GC */

#define OGS_PFCP_MODIFY_SESSION ((uint64_t)1<<0)
#define OGS_PFCP_MODIFY_DL_ONLY ((uint64_t)1<<1)
#define OGS_PFCP_MODIFY_UL_ONLY ((uint64_t)1<<2)
#define OGS_PFCP_MODIFY_INDIRECT ((uint64_t)1<<3)
#define OGS_PFCP_MODIFY_UE_REQUESTED ((uint64_t)1<<4)
#define OGS_PFCP_MODIFY_NETWORK_REQUESTED ((uint64_t)1<<5)
#define OGS_PFCP_MODIFY_CREATE ((uint64_t)1<<6)
#define OGS_PFCP_MODIFY_REMOVE ((uint64_t)1<<7)
#define OGS_PFCP_MODIFY_EPC_TFT_UPDATE ((uint64_t)1<<8)
#define OGS_PFCP_MODIFY_EPC_QOS_UPDATE ((uint64_t)1<<9)
#define OGS_PFCP_MODIFY_TFT_NEW ((uint64_t)1<<10)
#define OGS_PFCP_MODIFY_TFT_ADD ((uint64_t)1<<11)
#define OGS_PFCP_MODIFY_TFT_REPLACE ((uint64_t)1<<12)
#define OGS_PFCP_MODIFY_TFT_DELETE ((uint64_t)1<<13)
#define OGS_PFCP_MODIFY_QOS_CREATE ((uint64_t)1<<14)
#define OGS_PFCP_MODIFY_QOS_MODIFY ((uint64_t)1<<15)
#define OGS_PFCP_MODIFY_QOS_DELETE ((uint64_t)1<<16)
#define OGS_PFCP_MODIFY_ACTIVATE ((uint64_t)1<<17)
#define OGS_PFCP_MODIFY_DEACTIVATE ((uint64_t)1<<18)
#define OGS_PFCP_MODIFY_END_MARKER ((uint64_t)1<<19)
#define OGS_PFCP_MODIFY_ERROR_INDICATION ((uint64_t)1<<20)
#define OGS_PFCP_MODIFY_XN_HANDOVER ((uint64_t)1<<21)
#define OGS_PFCP_MODIFY_N2_HANDOVER ((uint64_t)1<<22)
#define OGS_PFCP_MODIFY_HANDOVER_CANCEL ((uint64_t)1<<23)
#define OGS_PFCP_MODIFY_URR  ((uint64_t)1<<24) /* type of trigger */
#define OGS_PFCP_MODIFY_URR_MEAS_METHOD ((uint64_t)1<<25)
#define OGS_PFCP_MODIFY_URR_REPORT_TRIGGER ((uint64_t)1<<26)
#define OGS_PFCP_MODIFY_URR_VOLUME_THRESH ((uint64_t)1<<27)
#define OGS_PFCP_MODIFY_URR_TIME_THRESH ((uint64_t)1<<28)

    uint64_t        modify_flags;

#define OGS_PFCP_DELETE_TRIGGER_UE_REQUESTED 1
#define OGS_PFCP_DELETE_TRIGGER_PCF_INITIATED 2
#define OGS_PFCP_DELETE_TRIGGER_RAN_INITIATED 3
#define OGS_PFCP_DELETE_TRIGGER_SMF_INITIATED 4
#define OGS_PFCP_DELETE_TRIGGER_AMF_RELEASE_SM_CONTEXT 5
#define OGS_PFCP_DELETE_TRIGGER_AMF_UPDATE_SM_CONTEXT 6
    int             delete_trigger;
} ogs_pfcp_xact_t;

int ogs_pfcp_xact_init(void);
void ogs_pfcp_xact_final(void);

ogs_pfcp_xact_t *ogs_pfcp_xact_local_create(ogs_pfcp_node_t *node,
        ogs_pfcp_header_t *hdesc, ogs_pkbuf_t *pkbuf,
        void (*cb)(ogs_pfcp_xact_t *xact, void *data), void *data);
ogs_pfcp_xact_t *ogs_pfcp_xact_cycle(ogs_pfcp_xact_t *xact);
void ogs_pfcp_xact_delete_all(ogs_pfcp_node_t *node);

int ogs_pfcp_xact_update_tx(ogs_pfcp_xact_t *xact,
        ogs_pfcp_header_t *hdesc, ogs_pkbuf_t *pkbuf);

int ogs_pfcp_xact_commit(ogs_pfcp_xact_t *xact);
void ogs_pfcp_xact_delayed_commit(ogs_pfcp_xact_t *xact, ogs_time_t duration);

int ogs_pfcp_xact_receive(ogs_pfcp_node_t *node,
        ogs_pfcp_header_t *h, ogs_pfcp_xact_t **xact);

#ifdef __cplusplus
}
#endif

#endif /* OGS_PFCP_XACT_H */
