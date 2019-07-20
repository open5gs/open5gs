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

#ifndef MME_TIMER_H
#define MME_TIMER_H

#include "ogs-core.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    MME_TIMER_BASE = 0,

    MME_TIMER_CLI_CONN_TO_SRV,
    MME_TIMER_T3413,

    MME_TIMER_TOP,

} mme_timer_e;

void mme_timer_init(void);
void mme_timer_final(void);

#ifdef __cplusplus
}
#endif

#endif /* MME_TIMER_H */
