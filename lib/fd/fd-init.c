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

#include "fd-logger.h"
#include "fd-lib.h"

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __base_fd_domain

static void fd_gnutls_log_func(int level, const char *str);
static void fd_log_func(int printlevel, const char *fname, int line, const char *format, va_list ap);

int fd_init(int mode, const char *conffile, fd_config_t *fd_config)
{
    int ret;

    gnutls_global_set_log_level(0);
    gnutls_global_set_log_function(fd_gnutls_log_func);

    fd_g_debug_lvl = FD_LOG_ERROR;
    ret = fd_log_handler_register(fd_log_func);
    if (ret != 0) 
    {
        ogs_error("fd_log_handler_register() failed");
        return ret;
    } 

    ret = fd_core_initialize();
    if (ret != 0) 
    {
        ogs_error("fd_core_initialize() failed");
        return ret;
    } 
    
	/* Parse the configuration file */
    if (conffile)
    {
        CHECK_FCT_DO( fd_core_parseconf(conffile), goto error );
    }
    else
    {
        CHECK_FCT_DO( fd_config_init(fd_config), goto error );
    }

    /* Initialize FD Message */
    CHECK_FCT( fd_message_init() );

    /* Initialize FD logger */
    CHECK_FCT_DO( fd_logger_init(mode), goto error );

	/* Start the servers */
	CHECK_FCT_DO( fd_core_start(), goto error );

	CHECK_FCT_DO( fd_core_waitstartcomplete(), goto error );

    CHECK_FCT( fd_logger_stats_start() );

    return 0;
error:
	CHECK_FCT_DO( fd_core_shutdown(),  );
	CHECK_FCT_DO( fd_core_wait_shutdown_complete(),  );

	return -1;
}

void fd_final()
{
    fd_logger_final();

	CHECK_FCT_DO( fd_core_shutdown(), ogs_error("fd_core_shutdown() failed") );
	CHECK_FCT_DO( fd_core_wait_shutdown_complete(), 
            ogs_error("fd_core_wait_shutdown_complete() failed"));
}

static void fd_gnutls_log_func(int level, const char *str)
{
    ogs_trace("gnutls[%d]: %s", level, str);
}

static void fd_log_func(int printlevel, const char *fname, int line, const char *format, va_list ap)
{
    char buffer[OGS_HUGE_LEN];
    int  ret = 0;

    ret = vsnprintf(buffer, OGS_HUGE_LEN, format, ap);
    if (ret < 0 || ret > OGS_HUGE_LEN)
    {
        ogs_error("vsnprintf() failed");
        return;
    }

    switch(printlevel) 
    {
	    case FD_LOG_ANNOYING: 
            ogs_trace("freeDiameter[%d]: %s:%u %s", printlevel, fname, line, buffer);
            break;  
	    case FD_LOG_DEBUG:
            ogs_trace("freeDiameter[%d]: %s:%u %s", printlevel, fname, line, buffer);
            break;  
	    case FD_LOG_NOTICE:
            ogs_trace("freeDiameter[%d]: %s:%u %s", printlevel, fname, line, buffer);
            break;
	    case FD_LOG_ERROR:
            ogs_error("%s:%d %s", fname, line, buffer);
            if (!strcmp(buffer, " - The certificate is expired."))
            {
                ogs_error("You can renew CERT as follows:");
                ogs_error("./support/freeDiameter/make_certs.sh "
                        "./install/etc/nextepc/freeDiameter");
            }
            break;
	    case FD_LOG_FATAL:
            {
                char *except = "Initiating freeDiameter shutdown sequence";
                if (strncmp(buffer, except, strlen(except)) == 0)
                    ogs_info("freeDiameter[%d]: %s:%u %s", printlevel, fname, line, buffer);
                else
                    ogs_fatal("%s:%d %s", fname, line, buffer);
            }
            break;
	    default:
            ogs_warn("%s:%d %s", fname, line, buffer);
            break;
    }
}
