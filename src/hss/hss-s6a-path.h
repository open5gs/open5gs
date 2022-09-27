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

#ifndef HSS_S6A_PATH_H
#define HSS_S6A_PATH_H

#include "hss-context.h"

#ifdef __cplusplus
extern "C" {
#endif

/* HSS Sends Cancel Location Request to MME */
void hss_s6a_send_clr(char *imsi_bcd, char *mme_host, char *mme_realm, 
    uint32_t cancellation_type);

/* HSS Sends Insert Subscriber Data Request to MME */
int hss_s6a_send_idr(char *imsi_bcd, uint32_t idr_flags, uint32_t subdata_mask);

#ifdef __cplusplus
}
#endif

#endif /* HSS_S6A_PATH_H */
