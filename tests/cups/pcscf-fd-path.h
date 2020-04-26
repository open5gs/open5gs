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

#ifndef PCSCF_FD_PATH_H
#define PCSCF_FD_PATH_H

#ifdef __cplusplus
extern "C" {
#endif

int pcscf_fd_init(void);
void pcscf_fd_final(void);

void pcscf_rx_send_aar(uint8_t **rx_sid, const char *ip,
        int qos_type, int flow_presence);
void pcscf_rx_send_str(uint8_t *rx_sid);

#ifdef __cplusplus
}
#endif

#endif /* PCSCF_FD_PATH_H */

