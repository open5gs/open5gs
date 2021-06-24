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

int __ogs_diam_domain;

static void diam_gnutls_log_func(int level, const char *str);
static void diam_log_func(int printlevel, const char *format, va_list ap);

int ogs_diam_init(int mode, const char *conffile, ogs_diam_config_t *fd_config)
{
    int ret;

    gnutls_global_set_log_level(0);
    gnutls_global_set_log_function(diam_gnutls_log_func);

    fd_g_debug_lvl = FD_LOG_ERROR;
    ret = fd_log_handler_register(diam_log_func);
    if (ret != 0) {
        ogs_error("fd_log_handler_register() failed");
        return ret;
    } 

    ret = fd_core_initialize();
    if (ret != 0) {
        ogs_error("fd_core_initialize() failed");
        return ret;
    } 
    
	/* Parse the configuration file */
    if (conffile) {
        CHECK_FCT_DO( fd_core_parseconf(conffile), goto error );
    } else {
        CHECK_FCT_DO( ogs_diam_config_init(fd_config), goto error );
    }

    /* Initialize FD Message */
    CHECK_FCT( ogs_diam_message_init() );

    /* Initialize FD logger */
    CHECK_FCT_DO( ogs_diam_logger_init(mode), goto error );

    return 0;
error:
	CHECK_FCT_DO( fd_core_shutdown(),  );
	CHECK_FCT_DO( fd_core_wait_shutdown_complete(),  );

	return -1;
}

int ogs_diam_start(void)
{
	/* Start the servers */
	CHECK_FCT_DO( fd_core_start(), goto error );

	CHECK_FCT_DO( fd_core_waitstartcomplete(), goto error );

    CHECK_FCT( ogs_diam_logger_stats_start() );

    return 0;
error:
	CHECK_FCT_DO( fd_core_shutdown(),  );
	CHECK_FCT_DO( fd_core_wait_shutdown_complete(),  );

	return -1;
}

void ogs_diam_final()
{
    ogs_diam_logger_final();

	CHECK_FCT_DO( fd_core_shutdown(), ogs_error("fd_core_shutdown() failed") );
	CHECK_FCT_DO( fd_core_wait_shutdown_complete(), 
            ogs_error("fd_core_wait_shutdown_complete() failed"));
}

static void diam_gnutls_log_func(int level, const char *str)
{
    ogs_trace("gnutls[%d]: %s", level, str);
}

static void diam_log_func(int printlevel, const char *format, va_list ap)
{
    char buffer[OGS_HUGE_LEN*2];
    int  ret = 0;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-nonliteral"
    ret = ogs_vsnprintf(buffer, OGS_HUGE_LEN*2, format, ap);
#pragma GCC diagnostic pop
    if (ret < 0 || ret > OGS_HUGE_LEN*2) {
        ogs_error("vsnprintf() failed[ret=%d]", ret);
        return;
    }

#define diam_log_printf(level, ...) \
    ogs_log_printf(level, OGS_LOG_DOMAIN, 0, NULL, 0, NULL, 0, __VA_ARGS__)

    switch(printlevel) {
    case FD_LOG_ANNOYING: 
        diam_log_printf(OGS_LOG_TRACE, "[%d] %s\n", printlevel, buffer);
        break;  
    case FD_LOG_DEBUG:
        diam_log_printf(OGS_LOG_TRACE, "[%d] %s\n", printlevel, buffer);
        break;  
    case FD_LOG_INFO:
        diam_log_printf(OGS_LOG_TRACE, "[%d] %s\n", printlevel, buffer);
        break;
    case FD_LOG_NOTICE:
        diam_log_printf(OGS_LOG_DEBUG, "%s\n", buffer);
        break;
    case FD_LOG_ERROR:
        diam_log_printf(OGS_LOG_ERROR, "%s\n", buffer);
        if (!strcmp(buffer, " - The certificate is expired.")) {
            ogs_error("You can renew CERT as follows:");
            ogs_error("./support/freeDiameter/make_certs.sh "
                    "./install/etc/open5gs/freeDiameter");
        }
        break;
    case FD_LOG_FATAL:
        diam_log_printf(OGS_LOG_FATAL, "%s\n", buffer);
        ogs_assert_if_reached();
        break;
    default:
        diam_log_printf(OGS_LOG_ERROR, "[%d] %s\n", printlevel, buffer);
        break;
    }
}
