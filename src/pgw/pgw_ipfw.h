#ifndef __PGW_IPFW_H__
#define __PGW_IPFW_H__

#include "pgw_context.h"

int pgw_compile_packet_filter(pgw_rule_t *pf, char *description);
pgw_bearer_t *pgw_bearer_find_by_packet(ogs_pkbuf_t *pkt);

#endif /* __PGW_IPFW_H__ */
