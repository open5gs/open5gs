#define TRACE_MODULE _s6a_config

#include "core_debug.h"
#include "core_lib.h"
#include "core_file.h"

#include "s6a_lib.h"

static struct s6a_config_t g_conf;
struct s6a_config_t *s6a_config;

static int s6a_config_apply_internal()
{
    struct peer_info fddpi;
    struct addrinfo hints, *ai;
    int ret;
    int disc = 0;

    fd_g_config->cnf_diamid = s6a_config->cnf_diamid;
    fd_os_validate_DiameterIdentity(
            &fd_g_config->cnf_diamid, &fd_g_config->cnf_diamid_len, 1);
    fd_g_config->cnf_diamrlm = s6a_config->cnf_diamrlm;
    fd_os_validate_DiameterIdentity(
            &fd_g_config->cnf_diamrlm, &fd_g_config->cnf_diamrlm_len, 1);
    if (s6a_config->cnf_addr == NULL)
        return CORE_ERROR;

    memset(&hints, 0, sizeof(hints));
    hints.ai_flags = AI_PASSIVE | AI_NUMERICHOST;
    if (s6a_config->cnf_addr == NULL)
        return CORE_ERROR;
    ret = getaddrinfo(s6a_config->cnf_addr, NULL, &hints, &ai);
    if (ret)
        return CORE_ERROR;

    fd_ep_add_merge( &fd_g_config->cnf_endpoints,
            ai->ai_addr, ai->ai_addrlen, EP_FL_CONF),
    freeaddrinfo(ai);

    if (s6a_config->cnf_port)
        fd_g_config->cnf_port = s6a_config->cnf_port;
    if (s6a_config->cnf_port_tls)
        fd_g_config->cnf_port_tls = s6a_config->cnf_port_tls;

    memset(&fddpi, 0, sizeof(fddpi));
    fddpi.config.pic_flags.persist = PI_PRST_ALWAYS;
    fddpi.config.pic_flags.pro3 = PI_P3_IP;
    fddpi.config.pic_flags.pro4 = PI_P4_TCP;
    fddpi.config.pic_flags.alg = PI_ALGPREF_TCP;
    fddpi.config.pic_flags.sec |= PI_SEC_NONE;

    fddpi.config.pic_port = s6a_config->pic_port;
    fddpi.pi_diamid = s6a_config->pi_diamid;

    fd_list_init( &fddpi.pi_endpoints, NULL );

    memset(&hints, 0, sizeof(hints));
    hints.ai_flags = AI_ADDRCONFIG | AI_NUMERICHOST;
    if (s6a_config->pi_addr == NULL)
        return CORE_ERROR;
    ret = getaddrinfo(s6a_config->pi_addr, NULL, &hints, &ai);
    if (ret) 
        return CORE_ERROR;
    
    fd_ep_add_merge( &fddpi.pi_endpoints, 
            ai->ai_addr, ai->ai_addrlen, EP_FL_CONF | (disc ?: EP_ACCEPTALL) );
    fd_peer_add ( &fddpi, NULL, NULL, NULL );

    freeaddrinfo(ai);

	return 0;
}

status_t s6a_config_apply()
{
	char * buf = NULL, *b;
	size_t len = 0;
	
	CHECK_FCT( s6a_config_apply_internal() );
	
	/* The following module use data from the configuration */
    int fd_rtdisp_init(void);
	CHECK_FCT( fd_rtdisp_init() );

	/* Display configuration */
	b = fd_conf_dump(&buf, &len, NULL);
    LOG_SPLIT(FD_LOG_NOTICE, NULL, 
            b ?: "<Error during configuration dump...>", NULL);
	free(buf);	
	
	/* Since some extensions might have modified the definitions from the dict_base_protocol, we only load the objects now */
    int fd_msg_init(void);
	CHECK_FCT( fd_msg_init()    );
	
    return CORE_OK;
}

int s6a_config_init(void)
{
    memset(&g_conf, 0, sizeof(struct s6a_config_t));
    s6a_config = &g_conf;

	/* Set the default values */
	s6a_config->vendor_id  = 10415;	/* 3GPP Vendor ID */
	s6a_config->appli_id   = 16777251;	/* 3GPP S6A Application ID */
	s6a_config->duration   = 10; /* 10 seconds */
	
	return 0;
}
