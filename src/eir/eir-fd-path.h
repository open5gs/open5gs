/*
 * Copyright (C) 2026 by LetMeConnect
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

#ifndef EIR_FD_PATH_H
#define EIR_FD_PATH_H

#include "context.h"

#ifdef __cplusplus
extern "C" {
#endif

/* S13 (TS 29.272) server: ME-Identity-Check-Request from the MME */
int eir_fd_init(void);
void eir_fd_final(void);

#ifdef __cplusplus
}
#endif

#endif /* EIR_FD_PATH_H */
