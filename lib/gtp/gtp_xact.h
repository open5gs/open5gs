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
 * Transaction Configuration
 */
typedef struct _gtp_xact_ctx_t {
    c_uint32_t g_xact_id;

    tm_service_t *tm_service;
    c_uintptr_t event;
} gtp_xact_ctx_t;

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

    c_uint8_t       type;           /**< Save transmitted GTP message type */
    pkbuf_t         *pkbuf;         /**< Save transmiited packet */

    tm_block_id     tm_wait;        /**< Timer waiting for next message */
    c_uint8_t       retry_count;    /**< Retry count waiting for next message */

    struct _gtp_xact_t *assoc_xact; /**< Associated transaction */
} gtp_xact_t;

CORE_DECLARE(status_t) gtp_xact_init(gtp_xact_ctx_t *context, 
    tm_service_t *tm_service, c_uintptr_t event);
CORE_DECLARE(status_t) gtp_xact_final(void);

CORE_DECLARE(gtp_xact_t *) gtp_xact_local_create(
        net_sock_t *sock, gtp_node_t *gnode);
CORE_DECLARE(gtp_xact_t *) gtp_xact_remote_create(
        net_sock_t *sock, gtp_node_t *gnode, c_uint32_t sqn);
CORE_DECLARE(void) gtp_xact_delete_all(gtp_node_t *gnode);

CORE_DECLARE(status_t) gtp_xact_update_tx(gtp_xact_t *xact,
        c_uint8_t type, c_uint32_t teid, pkbuf_t *pkbuf, c_uintptr_t event);
CORE_DECLARE(status_t) gtp_xact_update_rx(gtp_xact_t *xact,
        c_uint8_t type, c_uintptr_t event);

CORE_DECLARE(status_t) gtp_xact_commit(gtp_xact_t *xact);
CORE_DECLARE(status_t) gtp_xact_timeout(index_t index);

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
