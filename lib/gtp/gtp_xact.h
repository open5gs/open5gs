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

#define GTP_MAX_MESSAGE_TYPE 256

/**
 * Transaction context
 */
typedef struct _gtp_xact_ctx_t {
    lnode_t         node;           /**< A node of list */

    c_uint32_t      xid;            /**< Transaction ID */
    
#define GTP_LOCAL_ORIGINATOR  0
#define GTP_REMOTE_ORIGINATOR 1
    c_uint8_t       org;            /**< Transaction' originator. 
                                         local or remote */
    gtp_node_t      *gnode;         /**< RElevant GTP node context */
    void            *xctx;          /**< Relevant context */
    tm_block_id     tm_wait;        /**< Timer waiting for next message */
    int             retry_count;    /**< Retry count waiting for next message */

} gtp_xact_ctx_t;

/**
 * Initialize the transaction framework
 */
CORE_DECLARE(status_t) gtp_xact_init(void);

/**
 * Finalize the transaction framework
 */
CORE_DECLARE(status_t) gtp_xact_final(void);

/**
 * Create a new transaction which was initiated by local ASN node.
 */
CORE_DECLARE(status_t) gtp_xact_new_local(gtp_xact_ctx_t **xact, c_uint8_t type,
    pkbuf_t *pkb, gtp_node_t *gnode);

/**
 * Create a new transaction which was initiated by remote node
 */
CORE_DECLARE(status_t) gtp_xact_new_remote(gtp_xact_ctx_t **xact);

/**
 * Delete a transaction
 */
CORE_DECLARE(status_t) gtp_xact_delete(gtp_xact_ctx_t *xact);

/**
 * Update the transaction with the new packet to be sent for the next step
 */
CORE_DECLARE(status_t) gtp_xact_update_tx(gtp_xact_ctx_t *xact, pkbuf_t *pkb);

/**
 * Update the transaction with the new received packet for the next step
 */
CORE_DECLARE(status_t) gtp_xact_update_rx(gtp_xact_ctx_t *xact);

/**
 * Apply and commit the updated of the transcation
 */
CORE_DECLARE(status_t) gtp_xact_commit(gtp_xact_ctx_t *xact);

/**
 * Find the transaction with the given ASN header
 */
CORE_DECLARE(gtp_xact_ctx_t *) gtp_xact_find();

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __GTP_XACT_H__ */
