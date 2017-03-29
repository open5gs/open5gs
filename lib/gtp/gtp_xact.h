#ifndef __GTP_XACT_H__
#define __GTP_XACT_H__

#include "core_pkbuf.h"
#include "core_list.h"
#include "core_net.h"
#include "core_timer.h"

#include "gtp_path.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * Transaction Configuration
 */
typedef struct _gtp_xact_ctx_t {
    c_uint32_t g_xact_id;
    void *pool;

    tm_service_t *tm_service;
    c_uintptr_t event;
    c_uint32_t duration;
    int retry_count;
} gtp_xact_ctx_t;

/**
 * Transaction context
 */
typedef struct _gtp_xact_t {
    lnode_t         node;           /**< A node of list */

    c_uint32_t      xid;            /**< Transaction ID */
    
#define GTP_LOCAL_ORIGINATOR  0
#define GTP_REMOTE_ORIGINATOR 1
    c_uint8_t       org;            /**< Transaction' originator. 
                                         local or remote */
    c_uint8_t       type;           /**< GTPv2-C Message Type */
    net_sock_t      *sock;          /**< GTP Socket */
    gtp_node_t      *gnode;         /**< Relevant GTP node context */
    pkbuf_t         *pkbuf;         /**< Relevant GTP node context */
    tm_block_id     tm_wait;        /**< Timer waiting for next message */
    int             retry_count;    /**< Retry count waiting for next message */

} gtp_xact_t;

/**
 * Initialize the transaction framework
 */
CORE_DECLARE(status_t) gtp_xact_init(gtp_xact_ctx_t *context, 
    tm_service_t *tm_service, c_uintptr_t event, c_uint32_t duration, 
    int retry_count);

/**
 * Finalize the transaction framework
 */
CORE_DECLARE(status_t) gtp_xact_final(void);

/**
 * Create a new transaction which was initiated by local ASN node.
 */
CORE_DECLARE(gtp_xact_t *) gtp_xact_new_local(gtp_xact_ctx_t *context, 
    c_uint8_t type, net_sock_t *sock, gtp_node_t *gnode, pkbuf_t *pkbuf);

/**
 * Create a new transaction which was initiated by remote node
 */
CORE_DECLARE(gtp_xact_t *) gtp_xact_new_remote(gtp_xact_ctx_t *context, 
    net_sock_t *sock, gtp_node_t *gnode, pkbuf_t *pkbuf);

/**
 * Delete a transaction
 */
CORE_DECLARE(status_t) gtp_xact_delete(gtp_xact_t *xact);

/**
 * Update the transaction with the new packet to be sent for the next step
 */
CORE_DECLARE(status_t) gtp_xact_update_tx(gtp_xact_t *xact, pkbuf_t *pkb);

/**
 * Update the transaction with the new received packet for the next step
 */
CORE_DECLARE(status_t) gtp_xact_update_rx(gtp_xact_t *xact);

/**
 * Apply and commit the updated of the transcation
 */
CORE_DECLARE(status_t) gtp_xact_commit(gtp_xact_t *xact);

/**
 * Find the transaction with the given ASN header
 */
CORE_DECLARE(gtp_xact_t *) gtp_xact_find(gtp_node_t *gnode, pkbuf_t *pkbuf);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __GTP_XACT_H__ */
