#ifndef __FD_CONTEXT_H__
#define __FD_CONTEXT_H__

#include "core_errno.h"

#include "freeDiameter/freeDiameter-host.h"
#include "freeDiameter/libfdcore.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* This is default diameter configuration if there is no config file 
 * The Configuration : No TLS, Only TCP */
struct fd_context_t {
    /* Diameter Identity of the local peer (FQDN -- ASCII) */
    char *cnf_diamid; 
    /* Diameter realm of the local peer, default to realm part of cnf_diamid */
    char *cnf_diamrlm; 
    /* IP address of the local peer */
    char *cnf_addr;

    /* the local port for legacy Diameter (default: 3868) in host byte order */
    c_uint16_t cnf_port;
    /* the local port for Diameter/TLS (default: 5658) in host byte order */
    c_uint16_t cnf_port_tls;

    /* (supposedly) UTF-8, \0 terminated. 
     * The Diameter Identity of the remote peer. */
    char *pi_diamid; 
    char *pi_addr; /* IP address of the remote peer */
    c_uint16_t pic_port; /* port to connect to. 0: default. */
    
#define FD_MODE_SERVER   0x1
#define FD_MODE_CLIENT   0x2
    int mode;        /* default FD_MODE_SERVER | FD_MODE_CLIENT */
    
    int duration; /* default 10 */
    struct fd_stats {
        unsigned long long nb_echoed; /* server */
        unsigned long long nb_sent;   /* client */
        unsigned long long nb_recv;   /* client */
        unsigned long long nb_errs;   /* client */
        unsigned long shortest;  /* fastest answer, in microseconds */
        unsigned long longest;   /* slowest answer, in microseconds */
        unsigned long avg;       /* average answer time, in microseconds */
    } stats;

    pthread_mutex_t stats_lock;
};

CORE_DECLARE(int) fd_context_init(int mode);
CORE_DECLARE(struct fd_context_t*) fd_self();

CORE_DECLARE(status_t) fd_set_default_context(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ! __FD_CONTEXT_H__ */
