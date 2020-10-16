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

#include "ogs-diameter-rx.h"

int pcscf_fd_init(void);
void pcscf_fd_final(void);

void pcscf_rx_send_aar_audio(uint8_t **rx_sid,
        test_sess_t *sess, int id_type, int qos_type, int flow_type);
void pcscf_rx_send_aar_video(uint8_t **rx_sid, test_sess_t *sess, int id_type);
void pcscf_rx_send_aar_ctrl(uint8_t **rx_sid, test_sess_t *sess, int id_type);
void pcscf_rx_send_str(uint8_t *rx_sid);

#ifdef __cplusplus
}
#endif

#endif /* PCSCF_FD_PATH_H */

