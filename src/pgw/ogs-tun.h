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

#ifndef OGS_TUN_H
#define OGS_TUN_H

#include "ogs-core.h"

#ifdef __cplusplus
extern "C" {
#endif

ogs_socket_t ogs_tun_open(char *ifname, int len, int is_tap);
int ogs_tun_set_ip(char *ifname, ogs_ipsubnet_t *gw,  ogs_ipsubnet_t *sub);

#ifdef __cplusplus
}
#endif

#endif /* OGS_TUN_H */
