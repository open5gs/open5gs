#define TRACE_MODULE _s6a_conf

#include "core_debug.h"
#include "core_lib.h"

#include "s6a_app.h"

#include "freeDiameter/freeDiameter-host.h"
#include "freeDiameter/libfdcore.h"

int s6a_conf_parse()
{
    struct peer_info fddpi;
    struct addrinfo hints, *ai;
    int ret;
    int disc = 0;

	/* Resolve hostname if not provided */
    fd_g_config->cnf_diamid = "peer1.localdomain";
    fd_os_validate_DiameterIdentity(&fd_g_config->cnf_diamid, &fd_g_config->cnf_diamid_len, 1);
	
	/* Handle the realm part */
	fd_g_config->cnf_diamrlm = "localdomain";
    fd_os_validate_DiameterIdentity(&fd_g_config->cnf_diamrlm, &fd_g_config->cnf_diamrlm_len, 1);

    memset(&fddpi, 0, sizeof(fddpi));
    fddpi.config.pic_flags.persist = PI_PRST_ALWAYS;
    fddpi.config.pic_flags.pro3 = PI_P3_IP;
    fddpi.config.pic_flags.pro4 = PI_P4_TCP;
    fddpi.config.pic_flags.alg = PI_ALGPREF_TCP;
    fddpi.config.pic_flags.sec |= PI_SEC_NONE;
    fddpi.pi_diamid = "peer2.localdomain";
    fddpi.config.pic_port = (uint16_t)30868;

    memset(&hints, 0, sizeof(hints));
    hints.ai_flags = AI_ADDRCONFIG | AI_NUMERICHOST;
    ret = getaddrinfo("127.0.0.1", NULL, &hints, &ai);
    if (ret) { return CORE_ERROR; }
    
    fd_list_init( &fddpi.pi_endpoints, NULL );
    fd_ep_add_merge( &fddpi.pi_endpoints, ai->ai_addr, ai->ai_addrlen, EP_FL_CONF | (disc ?: EP_ACCEPTALL) );
    freeaddrinfo(ai);

    fd_peer_add ( &fddpi, NULL, NULL, NULL );
	
	return 0;
}

status_t s6a_config_init(s6a_config_t *config)
{
	char * buf = NULL, *b;
	size_t len = 0;
	
	CHECK_FCT( s6a_conf_parse() );
	
	/* The following module use data from the configuration */
    int fd_rtdisp_init(void);
	fd_rtdisp_init();

	/* Display configuration */
	b = fd_conf_dump(&buf, &len, NULL);
    LOG_SPLIT(FD_LOG_DEBUG, NULL, b ?: "<Error during configuration dump...>", NULL);
	free(buf);	
	
	/* Since some extensions might have modified the definitions from the dict_base_protocol, we only load the objects now */
    int fd_msg_init(void);
	fd_msg_init();
	
    return CORE_OK;
}
