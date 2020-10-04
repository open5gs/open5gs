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

#ifndef UPF_GTP_PATH_H
#define UPF_GTP_PATH_H

#include "ogs-tun.h"
#include "ogs-gtp.h"

#ifdef __cplusplus
extern "C" {
#endif

int upf_gtp_init(void);
void upf_gtp_final(void);

int upf_gtp_open(void);
void upf_gtp_close(void);

#ifdef __cplusplus
}
#endif

#endif /* UPF_GTP_PATH_H */
