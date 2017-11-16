#ifndef __CORE_SCTP_H__
#define __CORE_SCTP_H__

#include "core_sock.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

CORE_DECLARE(status_t) sctp_open(sock_id *new,
        int family, int type,
        const char *local_host, c_uint16_t local_port,
        const char *remote_host, c_uint16_t remote_port,
        int flags);

CORE_DECLARE(int) sctp_write(sock_id id, const void *msg, size_t len,
        struct sockaddr *to, socklen_t tolen,
        c_uint32_t ppid, c_uint16_t stream_no);

#define CORE_SCTP_REMOTE_CLOSED     -2
CORE_DECLARE(int) sctp_read(sock_id id, void *msg, size_t len,
        struct sockaddr *from, socklen_t *fromlen,
        c_uint32_t *ppid, c_uint16_t *stream_no);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
