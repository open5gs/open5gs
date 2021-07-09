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

#ifndef TEST_FD_PATH_H
#define TEST_FD_PATH_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ogs-diameter-rx.h"

int test_fd_init(void);
void test_fd_final(void);

int test_rx_init(void);
void test_rx_final(void);

int test_cx_init(void);
void test_cx_final(void);

void test_rx_send_aar_audio(uint8_t **rx_sid,
        test_sess_t *sess, int id_type, int qos_type, int flow_type);
void test_rx_send_aar_video(uint8_t **rx_sid, test_sess_t *sess, int id_type);
void test_rx_send_aar_ctrl(uint8_t **rx_sid, test_sess_t *sess, int id_type);
void test_rx_send_str(uint8_t *rx_sid);

void test_cx_send_uar(test_ue_t *test_ue, int id_type);

#ifdef __cplusplus
}
#endif

#endif /* TEST_FD_PATH_H */

