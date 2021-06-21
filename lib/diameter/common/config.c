/*
 * Copyright (C) 2019 by Sukchan Lee <acetcom@gmail.com>
 *
 * This file is part of Open5GS.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "ogs-diameter-common.h"

int fd_ext_add(char *filename, char *conffile);
int fd_rtdisp_init(void);
int fd_ext_load(void);
int fd_msg_init(void);

static int diam_config_apply(ogs_diam_config_t *fd_config)
{
    struct addrinfo hints, *ai;
    int ret;
    int i;

    ogs_assert(fd_config->cnf_diamid);
    ogs_assert(fd_config->cnf_diamrlm);
    ogs_assert(fd_config->cnf_addr);

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
        ogs_error("getaddrinfo() [%s] failed(%d:%s)",
                fd_config->cnf_addr, errno, strerror(errno));
        return OGS_ERROR;
    }

    CHECK_FCT_DO( fd_ep_add_merge( &fd_g_config->cnf_endpoints,
            ai->ai_addr, ai->ai_addrlen, EP_FL_CONF), return OGS_ERROR );
    freeaddrinfo(ai);

    if (fd_config->cnf_port)
        fd_g_config->cnf_port = fd_config->cnf_port;
    if (fd_config->cnf_port_tls)
        fd_g_config->cnf_port_tls = fd_config->cnf_port_tls;

    if (fd_config->cnf_flags.no_sctp)
        fd_g_config->cnf_flags.no_sctp = fd_config->cnf_flags.no_sctp;

    if (fd_config->cnf_flags.no_fwd)
        fd_g_config->cnf_flags.no_fwd = fd_config->cnf_flags.no_fwd;

    /********************************************************************
     * Diameter Client
     */
    for (i = 0; i < fd_config->num_of_conn; i++)
    {
        int disc = 0;
        struct peer_info fddpi;

        ogs_assert(fd_config->conn[i].addr);

        memset(&fddpi, 0, sizeof(fddpi));
        fddpi.config.pic_flags.persist = PI_PRST_ALWAYS;
        fddpi.config.pic_flags.pro3 = PI_P3_IP;
        fddpi.config.pic_flags.pro4 = PI_P4_DEFAULT;
        fddpi.config.pic_flags.alg = PI_ALGPREF_SCTP;
        fddpi.config.pic_flags.sec |= PI_SEC_NONE;

        fddpi.config.pic_port = fd_config->conn[i].port;
        fddpi.pi_diamid = (DiamId_t)fd_config->conn[i].identity;

        fd_list_init( &fddpi.pi_endpoints, NULL );

        memset(&hints, 0, sizeof(hints));
        hints.ai_flags = AI_ADDRCONFIG | AI_NUMERICHOST;
        ret = getaddrinfo(fd_config->conn[i].addr, NULL, &hints, &ai);
        if (ret)
        {
            ogs_error("getaddrinfo() [%s] failed(%d:%s)",
                    fd_config->conn[i].addr, errno, strerror(errno));
            return OGS_ERROR;
        }
        
        CHECK_FCT_DO( fd_ep_add_merge(
                &fddpi.pi_endpoints, ai->ai_addr, ai->ai_addrlen,
                EP_FL_CONF | (disc ?: EP_ACCEPTALL) ), return OGS_ERROR);
        CHECK_FCT_DO( fd_peer_add ( &fddpi, NULL, NULL, NULL ),
                return OGS_ERROR);

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
        ogs_assert(fname);
        fd = fopen(fname, "r");
        if ((fd == NULL) && (*fname != '/'))
        {
            char *bkp = fname;
            fname = malloc(strlen(bkp) + strlen(DEFAULT_EXTENSIONS_PATH) + 2);
            ogs_assert(fname);
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
            ogs_assert(cfname);
            fd = fopen(cfname, "r");
            if ((fd == NULL) && (*cfname != '/'))
            {
                char *test = NULL;
                test = malloc( strlen(cfname) + strlen(DEFAULT_CONF_PATH) + 2);
                ogs_assert(test);
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

        CHECK_FCT_DO( fd_ext_add( fname, cfname ), return OGS_ERROR );
    }

	return OGS_OK;
}

int ogs_diam_config_init(ogs_diam_config_t *fd_config)
{
	char * buf = NULL, *b;
	size_t len = 0;
	
	CHECK_FCT( diam_config_apply(fd_config) );
	
	/* The following module use data from the configuration */
	CHECK_FCT( fd_rtdisp_init() );

    /* Now, load all dynamic extensions */
    CHECK_FCT( fd_ext_load()  );

	/* Display configuration */
	b = fd_conf_dump(&buf, &len, NULL);
    LOG_SPLIT(FD_LOG_NOTICE, NULL, 
            b ?: (char*)"<Error during configuration dump...>", NULL);
	free(buf);	
	
	/* Since some extensions might have modified the definitions from the dict_base_protocol, we only load the objects now */
	CHECK_FCT( fd_msg_init()    );
	
    return OGS_OK;
}

