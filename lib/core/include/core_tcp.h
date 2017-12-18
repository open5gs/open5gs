#ifndef __CORE_TCP_H__
#define __CORE_TCP_H__

#include "core_sock.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

CORE_DECLARE(status_t) tcp_open(sock_id *new,
        int family,
        const char *local_host, c_uint16_t local_port,
        const char *remote_host, c_uint16_t remote_port,
        int flags);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
