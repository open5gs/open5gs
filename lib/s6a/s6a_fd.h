#ifndef __S6A_FD_H__
#define __S6A_FD_H__

#include "core_errno.h"

#include "s6a_app.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

struct s6a_fd_config_t {
    /* Diameter Identity of the local peer (FQDN -- ASCII) */
	char *cnf_diamid; 
    /* Diameter realm of the local peer, default to realm part of cnf_diamid */
	char *cnf_diamrlm; 

    /* the local port for legacy Diameter (default: 3868) in host byte order */
	c_uint16_t cnf_port;
    /* the local port for Diameter/TLS (default: 5658) in host byte order */
	c_uint16_t cnf_port_tls;

    /* (supposedly) UTF-8, \0 terminated. 
     * The Diameter Identity of the remote peer. */
	char *pi_diamid; 
    c_uint16_t pic_port; /* port to connect to. 0: default. */
    
};

extern struct s6a_fd_config_t *s6a_fd_config;

CORE_DECLARE(int) s6a_fd_init();
CORE_DECLARE(void) s6a_fd_final();

CORE_DECLARE(char *) s6a_fd_hss_config();
CORE_DECLARE(char *) s6a_fd_mme_config();
CORE_DECLARE(status_t) s6a_fd_config_apply();

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ! __S6A_FD_H__ */
