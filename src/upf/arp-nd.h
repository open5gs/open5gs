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

#include "upf-config.h"

#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif

#ifdef HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif

#ifdef HAVE_NET_ETHERNET_H
#include <net/ethernet.h>
#endif

#define MAX_ND_SIZE 128

#ifdef __cplusplus
extern "C" {
#endif

void set_source_mac(uint8_t *data);
bool is_arp_req(uint8_t *data, uint len);
uint8_t arp_reply(uint8_t *reply_data, uint8_t *request_data, uint len,
        const uint8_t *mac);
bool is_nd_req(uint8_t *data, uint len);
uint8_t nd_reply(uint8_t *reply_data, uint8_t *request_data, uint len,
        const uint8_t *mac);

#ifdef __cplusplus
}
#endif
