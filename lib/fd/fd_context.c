#define TRACE_MODULE _fd_context

#include "core_debug.h"
#include "core_lib.h"
#include "core_file.h"

#include "fd_context.h"

static struct fd_context_t self;

int fd_context_init(int mode)
{
    memset(&self, 0, sizeof(struct fd_context_t));

    self.mode = mode;

	/* Set the default values */
	self.vendor_id  = 10415;	/* 3GPP Vendor ID */
	self.duration   = 10;       /* 10 seconds */
	
	return 0;
}

struct fd_context_t* fd_self()
{
    return &self;
}

static int fd_default_context()
{
    struct peer_info fddpi;
    struct addrinfo hints, *ai;
    int ret;
    int disc = 0;

    /* disable SCTP */
    fd_g_config->cnf_flags.no_sctp = 1;

    fd_g_config->cnf_diamid = self.cnf_diamid;
    fd_os_validate_DiameterIdentity(
            &fd_g_config->cnf_diamid, &fd_g_config->cnf_diamid_len, 1);
    fd_g_config->cnf_diamrlm = self.cnf_diamrlm;
    fd_os_validate_DiameterIdentity(
            &fd_g_config->cnf_diamrlm, &fd_g_config->cnf_diamrlm_len, 1);
    if (self.cnf_addr == NULL)
        return CORE_ERROR;

    memset(&hints, 0, sizeof(hints));
    hints.ai_flags = AI_PASSIVE | AI_NUMERICHOST;
    if (self.cnf_addr == NULL)
        return CORE_ERROR;
    ret = getaddrinfo(self.cnf_addr, NULL, &hints, &ai);
    if (ret)
        return CORE_ERROR;

    fd_ep_add_merge( &fd_g_config->cnf_endpoints,
            ai->ai_addr, ai->ai_addrlen, EP_FL_CONF),
    freeaddrinfo(ai);

    if (self.cnf_port)
        fd_g_config->cnf_port = self.cnf_port;
    if (self.cnf_port_tls)
        fd_g_config->cnf_port_tls = self.cnf_port_tls;

    memset(&fddpi, 0, sizeof(fddpi));
    fddpi.config.pic_flags.persist = PI_PRST_ALWAYS;
    fddpi.config.pic_flags.pro3 = PI_P3_IP;
    fddpi.config.pic_flags.pro4 = PI_P4_TCP;
    fddpi.config.pic_flags.alg = PI_ALGPREF_TCP;
    fddpi.config.pic_flags.sec |= PI_SEC_NONE;

    fddpi.config.pic_port = self.pic_port;
    fddpi.pi_diamid = self.pi_diamid;

    fd_list_init( &fddpi.pi_endpoints, NULL );

    memset(&hints, 0, sizeof(hints));
    hints.ai_flags = AI_ADDRCONFIG | AI_NUMERICHOST;
    if (self.pi_addr == NULL)
        return CORE_ERROR;
    ret = getaddrinfo(self.pi_addr, NULL, &hints, &ai);
    if (ret) 
        return CORE_ERROR;
    
    fd_ep_add_merge( &fddpi.pi_endpoints, 
            ai->ai_addr, ai->ai_addrlen, EP_FL_CONF | (disc ?: EP_ACCEPTALL) );
    fd_peer_add ( &fddpi, NULL, NULL, NULL );

    freeaddrinfo(ai);

	return 0;
}

status_t fd_set_default_context()
{
	char * buf = NULL, *b;
	size_t len = 0;
	
	CHECK_FCT( fd_default_context() );
	
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
