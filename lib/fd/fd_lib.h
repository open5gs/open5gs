#ifndef __FD_LIB_H__
#define __FD_LIB_H__

#include "core_errno.h"

#include "freeDiameter/freeDiameter-host.h"
#include "freeDiameter/libfdcore.h"

#include "fd_message.h"
#include "fd_logger.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* This is default diameter configuration if there is no config file 
 * The Configuration : No TLS, Only TCP */
typedef struct _fd_config_t {
    /* Diameter Identity of the local peer (FQDN -- ASCII) */
    const char *cnf_diamid; 
    /* Diameter realm of the local peer, default to realm part of cnf_diamid */
    const char *cnf_diamrlm; 
    /* IP address of the local peer */
    const char *cnf_addr;

    /* the local port for legacy Diameter (default: 3868) in host byte order */
    c_uint16_t cnf_port;
    /* the local port for Diameter/TLS (default: 5658) in host byte order */
    c_uint16_t cnf_port_tls;

	struct {
		unsigned no_sctp: 1;	/* disable the use of SCTP */
	} cnf_flags;

#define MAX_NUM_OF_FD_EXTENSION 32
    struct {
        const char *module;
        const char *conf;
    } ext[MAX_NUM_OF_FD_EXTENSION];
    int num_of_ext;

#define MAX_NUM_OF_FD_CONN 16
    /* (supposedly) UTF-8, \0 terminated. 
     * The Diameter Identity of the remote peer. */
    struct {
        const char *identity; 
        const char *addr; /* IP address of the remote peer */
        c_uint16_t port; /* port to connect to. 0: default. */
    } conn[MAX_NUM_OF_FD_CONN];
    int num_of_conn;
} fd_config_t;

CORE_DECLARE(int) fd_init(
        int mode, const char *conffile, fd_config_t *fd_config);
CORE_DECLARE(void) fd_final(void);

CORE_DECLARE(int) fd_config_init(fd_config_t *fd_config);

int fd_avp_search_avp ( struct avp * groupedavp, 
        struct dict_object * what, struct avp ** avp );

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ! __FD_LIB_H__ */
