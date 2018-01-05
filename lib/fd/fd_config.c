#define TRACE_MODULE _fd_config

#include "core_debug.h"
#include "core_lib.h"
#include "core_file.h"

#include "fd_lib.h"

static status_t fd_config_apply(fd_config_t *fd_config)
{
    struct addrinfo hints, *ai;
    int ret;
    int i;

    d_assert(fd_config->cnf_diamid, return CORE_ERROR,);
    d_assert(fd_config->cnf_diamrlm, return CORE_ERROR,);
    d_assert(fd_config->cnf_addr, return CORE_ERROR,);

    /********************************************************************
     * Diameter Server
     */
    fd_g_config->cnf_diamid = (DiamId_t)fd_config->cnf_diamid;
    fd_os_validate_DiameterIdentity(
            &fd_g_config->cnf_diamid, &fd_g_config->cnf_diamid_len, 1);
    fd_g_config->cnf_diamrlm = (DiamId_t)fd_config->cnf_diamrlm;
    fd_os_validate_DiameterIdentity(
            &fd_g_config->cnf_diamrlm, &fd_g_config->cnf_diamrlm_len, 1);

    memset(&hints, 0, sizeof(hints));
    hints.ai_flags = AI_PASSIVE | AI_NUMERICHOST;
    ret = getaddrinfo(fd_config->cnf_addr, NULL, &hints, &ai);
    if (ret)
    {
        d_error("getaddrinfo() [%s] failed(%d:%s)",
                fd_config->cnf_addr, errno, strerror(errno));
        return CORE_ERROR;
    }

    CHECK_FCT_DO( fd_ep_add_merge( &fd_g_config->cnf_endpoints,
            ai->ai_addr, ai->ai_addrlen, EP_FL_CONF), return CORE_ERROR );
    freeaddrinfo(ai);

    if (fd_config->cnf_port)
        fd_g_config->cnf_port = fd_config->cnf_port;
    if (fd_config->cnf_port_tls)
        fd_g_config->cnf_port_tls = fd_config->cnf_port_tls;

    if (fd_config->cnf_flags.no_sctp)
        fd_g_config->cnf_flags.no_sctp = fd_config->cnf_flags.no_sctp;

    /********************************************************************
     * Diameter Client
     */
    for (i = 0; i < fd_config->num_of_conn; i++)
    {
        int disc = 0;
        struct peer_info fddpi;

        d_assert(fd_config->conn[i].addr, return CORE_ERROR,);

        memset(&fddpi, 0, sizeof(fddpi));
        fddpi.config.pic_flags.persist = PI_PRST_ALWAYS;
        fddpi.config.pic_flags.pro3 = PI_P3_IP;
        fddpi.config.pic_flags.pro4 = PI_P4_TCP;
        fddpi.config.pic_flags.alg = PI_ALGPREF_TCP;
        fddpi.config.pic_flags.sec |= PI_SEC_NONE;

        fddpi.config.pic_port = fd_config->conn[i].port;
        fddpi.pi_diamid = (DiamId_t)fd_config->conn[i].identity;

        fd_list_init( &fddpi.pi_endpoints, NULL );

        memset(&hints, 0, sizeof(hints));
        hints.ai_flags = AI_ADDRCONFIG | AI_NUMERICHOST;
        ret = getaddrinfo(fd_config->conn[i].addr, NULL, &hints, &ai);
        if (ret)
        {
            d_error("getaddrinfo() [%s] failed(%d:%s)",
                    fd_config->conn[i].addr, errno, strerror(errno));
            return CORE_ERROR;
        }
        
        CHECK_FCT_DO( fd_ep_add_merge(
                &fddpi.pi_endpoints, ai->ai_addr, ai->ai_addrlen,
                EP_FL_CONF | (disc ?: EP_ACCEPTALL) ), return CORE_ERROR);
        CHECK_FCT_DO( fd_peer_add ( &fddpi, NULL, NULL, NULL ),
                return CORE_ERROR);

        freeaddrinfo(ai);
    }

    /********************************************************************
     * Load Extension
     */
    for (i = 0; i < fd_config->num_of_ext; i++)
    {
        char *fname = NULL;
        char *cfname = NULL;
        FILE *fd;

        fname = strdup(fd_config->ext[i].module);
        d_assert(fname, return CORE_ERROR,);
        fd = fopen(fname, "r");
        if ((fd == NULL) && (*fname != '/'))
        {
            char *bkp = fname;
            fname = malloc(strlen(bkp) + strlen(DEFAULT_EXTENSIONS_PATH) + 2);
            d_assert(fname, return CORE_ERROR,);
            sprintf(fname, DEFAULT_EXTENSIONS_PATH "/%s", bkp);
            fd = fopen(fname, "r");
            if (fd == NULL)
            {
                free(fname);
                fname = bkp;
            }
            else
            {
                free(bkp);
            }
        }

        if (fd)
            fclose(fd);

        cfname = (char *)fd_config->ext[i].conf;
        if (cfname)
        {
            cfname = strdup(fd_config->ext[i].conf);
            d_assert(cfname, return CORE_ERROR,);
            fd = fopen(cfname, "r");
            if ((fd == NULL) && (*cfname != '/'))
            {
                char *test = NULL;
                test = malloc( strlen(cfname) + strlen(DEFAULT_CONF_PATH) + 2);
                d_assert(test, return CORE_ERROR,);
                sprintf(test, DEFAULT_CONF_PATH "/%s", cfname);
                fd = fopen(test, "r");
                if (fd)
                {
                    free(cfname);
                    cfname=test;
                }
                else
                {
                    free(test);
                }
            }

            if (fd)
                fclose(fd);
        }

        extern int fd_ext_add( char * filename, char * conffile );
        CHECK_FCT_DO( fd_ext_add( fname, cfname ), return CORE_ERROR );
    }

	return CORE_OK;
}

status_t fd_config_init(fd_config_t *fd_config)
{
	char * buf = NULL, *b;
	size_t len = 0;
	
	CHECK_FCT( fd_config_apply(fd_config) );
	
	/* The following module use data from the configuration */
    int fd_rtdisp_init(void);
	CHECK_FCT( fd_rtdisp_init() );

    /* Now, load all dynamic extensions */
    int fd_ext_load();
    CHECK_FCT( fd_ext_load()  );

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

