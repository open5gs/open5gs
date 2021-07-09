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

#include "ogs-diameter-cx.h"
#include "ogs-diameter-s6a.h"
#include "ogs-diameter-swx.h"

#include "ogs-diameter-rx.h"
#include "ogs-diameter-s6b.h"

int test_fd_init(void);
void test_fd_final(void);

int test_swx_init(void);
void test_swx_final(void);

void test_swx_send(test_sess_t *sess, bool handover_ind,
        int (*gtp_send)(test_sess_t *sess, bool handover_ind));

int test_s6b_init(void);
void test_s6b_final(void);

#ifdef __cplusplus
}
#endif

#endif /* TEST_FD_PATH_H */

