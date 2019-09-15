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

#ifndef PCRF_FD_PATH_H
#define PCRF_FD_PATH_H

#ifdef __cplusplus
extern "C" {
#endif

struct sess_state;
typedef struct ogs_diam_rx_message_s ogs_diam_rx_message_t;

int pcrf_fd_init(void);
void pcrf_fd_final(void);

int pcrf_gx_init(void);
void pcrf_gx_final(void);
int pcrf_rx_init(void);
void pcrf_rx_final(void);

int pcrf_gx_send_rar(
        uint8_t *gx_sid, uint8_t *rx_sid, ogs_diam_rx_message_t *rx_message);
int pcrf_rx_send_asr(
        uint8_t *rx_sid, uint32_t abort_cause);

#ifdef __cplusplus
}
#endif

#endif /* PCRF_FD_PATH_H */

