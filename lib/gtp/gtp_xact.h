#ifndef __GTP_XACT_H__
#define __GTP_XACT_H__

#include "core_pkbuf.h"
#include "core_list.h"
#include "core_index.h"
#include "core_net.h"
#include "core_timer.h"

#include "gtp_path.h"
#include "gtp_message.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * Transaction context
 */
typedef struct _gtp_xact_t {
    lnode_t         node;           /**< A node of list */
    index_t         index;
    
#define GTP_LOCAL_ORIGINATOR  0
#define GTP_REMOTE_ORIGINATOR 1
    c_uint8_t       org;            /**< Transaction' originator. 
                                         local or remote */

    c_uint32_t      xid;            /**< Transaction ID */
    
    net_sock_t      *sock;          /**< GTP Socket */
    gtp_node_t      *gnode;         /**< Relevant GTP node context */

    int             step;           /**< Current step in the sequence.
                                         1 : Initial 
                                         2 : Triggered 
                                         3 : Triggered-Reply */
    struct {
        c_uint8_t   type;           /**< Message type history */
        pkbuf_t     *pkbuf;         /**< Packet history */
    } seq[3];                       /**< history for the each step */

    tm_block_id     tm_response;    /**< Timer waiting for next message */
    c_uint8_t       response_rcount;
    tm_block_id     tm_duplicated;  /**< Timer waiting for duplicated message */
    c_uint8_t       duplicated_rcount;

    struct _gtp_xact_t *assoc_xact; /**< Associated transaction */
} gtp_xact_t;

CORE_DECLARE(status_t) gtp_xact_init(tm_service_t *tm_service,
        c_uintptr_t response_event, c_uintptr_t duplicated_event);
CORE_DECLARE(status_t) gtp_xact_final(void);

CORE_DECLARE(gtp_xact_t *) gtp_xact_local_create(
        net_sock_t *sock, gtp_node_t *gnode);
CORE_DECLARE(gtp_xact_t *) gtp_xact_remote_create(
        net_sock_t *sock, gtp_node_t *gnode, c_uint32_t sqn);
CORE_DECLARE(void) gtp_xact_delete_all(gtp_node_t *gnode);

CORE_DECLARE(status_t) gtp_xact_update_tx(gtp_xact_t *xact,
        c_uint8_t type, c_uint32_t teid, pkbuf_t *pkbuf);
CORE_DECLARE(status_t) gtp_xact_update_rx(gtp_xact_t *xact, c_uint8_t type);

CORE_DECLARE(status_t) gtp_xact_commit(gtp_xact_t *xact);
CORE_DECLARE(status_t) gtp_xact_timeout(index_t index, c_uintptr_t event);

CORE_DECLARE(status_t) gtp_xact_receive(
        net_sock_t *sock, gtp_node_t *gnode, pkbuf_t *pkbuf,
        gtp_xact_t **xact, gtp_message_t *gtp_message);

CORE_DECLARE(gtp_xact_t *) gtp_xact_find(index_t index);
CORE_DECLARE(gtp_xact_t *)gtp_xact_find_by_xid(
        gtp_node_t *gnode, c_uint8_t type, c_uint32_t xid);
CORE_DECLARE(void) gtp_xact_associate(gtp_xact_t *xact1, gtp_xact_t *xact2);
CORE_DECLARE(void) gtp_xact_deassociate(gtp_xact_t *xact1, gtp_xact_t *xact2);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __GTP_XACT_H__ */
