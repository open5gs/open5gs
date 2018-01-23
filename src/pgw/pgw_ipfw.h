#ifndef __PGW_IPFW_H__
#define __PGW_IPFW_H__

#include "core_debug.h"

#include "pgw_context.h"

CORE_DECLARE(status_t) pgw_compile_packet_filter(
        pgw_rule_t *pf, c_int8_t *description);

CORE_DECLARE(pgw_bearer_t*) pgw_bearer_find_by_packet(pkbuf_t *pkt);

#endif /* __PGW_IPFW_H__ */
