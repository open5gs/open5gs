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

#include "hss-context.h"
#include "hss-fd-path.h"

static int initialized = 0;

int hss_initialize(void)
{
    int rv;

    hss_context_init();

    rv = hss_context_parse_config();
    if (rv != OGS_OK) return rv;

    rv = ogs_log_config_domain(
            ogs_app()->logger.domain, ogs_app()->logger.level);
    if (rv != OGS_OK) return rv;

    rv = ogs_dbi_init(ogs_app()->db_uri);
    if (rv != OGS_OK) return rv;

    rv = hss_fd_init();
    if (rv != OGS_OK) return OGS_ERROR;

    initialized = 1;

	return OGS_OK;
}

void hss_terminate(void)
{
    if (!initialized) return;

    hss_fd_final();

    ogs_dbi_final();
    hss_context_final();
	
	return;
}
