#define TRACE_MODULE _s6a_fd_conf

#include "core_debug.h"
#include "core_lib.h"
#include "core_file.h"

#include "s6a_fd.h"

static struct s6a_fd_config_t g_conf;
struct s6a_fd_config_t *s6a_fd_config;

static char *s6a_fd_hss_conffile = SYSCONFDIR "hss_fd.conf";
static char *s6a_fd_mme_conffile = SYSCONFDIR "mme_fd.conf";

static int s6a_fd_config_apply_internal()
{
    struct peer_info fddpi;
    struct addrinfo hints, *ai;
    int ret;
    int disc = 0;

    fd_g_config->cnf_diamid = s6a_fd_config->cnf_diamid;
    fd_os_validate_DiameterIdentity(
            &fd_g_config->cnf_diamid, &fd_g_config->cnf_diamid_len, 1);
    fd_g_config->cnf_diamrlm = s6a_fd_config->cnf_diamrlm;
    fd_os_validate_DiameterIdentity(
            &fd_g_config->cnf_diamrlm, &fd_g_config->cnf_diamrlm_len, 1);
    if (s6a_fd_config->cnf_port)
        fd_g_config->cnf_port = s6a_fd_config->cnf_port;
    if (s6a_fd_config->cnf_port_tls)
        fd_g_config->cnf_port_tls = s6a_fd_config->cnf_port_tls;

    memset(&fddpi, 0, sizeof(fddpi));
    fddpi.config.pic_flags.persist = PI_PRST_ALWAYS;
    fddpi.config.pic_flags.pro3 = PI_P3_IP;
    fddpi.config.pic_flags.pro4 = PI_P4_TCP;
    fddpi.config.pic_flags.alg = PI_ALGPREF_TCP;
    fddpi.config.pic_flags.sec |= PI_SEC_NONE;

    fddpi.config.pic_port = s6a_fd_config->pic_port;
    fddpi.pi_diamid = s6a_fd_config->pi_diamid;

    fd_list_init( &fddpi.pi_endpoints, NULL );

    memset(&hints, 0, sizeof(hints));
    hints.ai_flags = AI_ADDRCONFIG | AI_NUMERICHOST;
    ret = getaddrinfo("127.0.0.1", NULL, &hints, &ai);
    if (ret) 
        return CORE_ERROR;
    
    fd_ep_add_merge( &fddpi.pi_endpoints, 
            ai->ai_addr, ai->ai_addrlen, EP_FL_CONF | (disc ?: EP_ACCEPTALL) );
    fd_peer_add ( &fddpi, NULL, NULL, NULL );

    freeaddrinfo(ai);

	return 0;
}

status_t s6a_fd_config_apply()
{
	char * buf = NULL, *b;
	size_t len = 0;
	
	CHECK_FCT( s6a_fd_config_apply_internal() );
	
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

#define HSS_IDENTITY "peer2.localdomain"
#define HSS_REALM "localdomain"
#define HSS_PORT 30868
#define HSS_SECURE_PORT 30869

#define MME_IDENTITY "peer1.localdomain"
#define MME_REALM "localdomain"
#define MME_PORT DIAMETER_PORT
#define MME_SECURE_PORT DIAMETER_SECURE_PORT

char *s6a_fd_hss_config()
{
    status_t rv;
    file_info_t file_info;
    char *conffile = s6a_fd_hss_conffile;
    
    memset(&g_conf, 0, sizeof(struct s6a_fd_config_t));
    s6a_fd_config = &g_conf;

    s6a_fd_config->cnf_diamid = HSS_IDENTITY;
    s6a_fd_config->cnf_diamrlm = HSS_REALM;
    s6a_fd_config->cnf_port = HSS_PORT;
    s6a_fd_config->cnf_port_tls = HSS_SECURE_PORT;
    s6a_fd_config->pi_diamid = MME_IDENTITY;
    s6a_fd_config->pic_port = MME_PORT;

    rv = file_stat(&file_info, conffile, FILE_INFO_TYPE);
    if (rv == CORE_OK && file_info.filetype == FILE_REG)
    {
        return conffile;
    }
   
    return NULL;
}

char *s6a_fd_mme_config()
{
    status_t rv;
    file_info_t file_info;
    char *conffile = s6a_fd_mme_conffile;

    memset(&g_conf, 0, sizeof(struct s6a_fd_config_t));
    s6a_fd_config = &g_conf;

    s6a_fd_config->cnf_diamid = MME_IDENTITY;
    s6a_fd_config->cnf_diamrlm = MME_REALM;
    s6a_fd_config->pi_diamid = HSS_IDENTITY;
    s6a_fd_config->pic_port = HSS_PORT;

    rv = file_stat(&file_info, conffile, FILE_INFO_TYPE);
    if (rv == CORE_OK && file_info.filetype == FILE_REG)
    {
        return conffile;
    }

    return NULL;
}
