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

#include "context.h"

int __base_s1ap_domain;
int __base_nas_domain;
int __base_gtp_domain;
int __base_fd_domain;

static base_context_t self = {
    .gtp.node.pool = 512,
    .gtp.xact.pool = 512,
};

void base_initialize(void)
{
    ogs_log_install_domain(&__base_s1ap_domain, "s1ap", ogs_core()->log.level);
    ogs_log_install_domain(&__base_nas_domain, "nas", ogs_core()->log.level);
    ogs_log_install_domain(&__base_gtp_domain, "gtp", ogs_core()->log.level);
    ogs_log_install_domain(&__base_fd_domain, "fd", ogs_core()->log.level);
}

void base_finalize(void)
{
}

base_context_t *base_self(void)
{
    return &self;
}
