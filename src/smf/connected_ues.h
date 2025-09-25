/*
 * Copyright (C) 2025 by Juraj Elias <juraj.elias@gmail.com>
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

/*
 * Minimal public API for connected_ues
 */
#ifndef SMF_CONNECTED_UES_H
#define SMF_CONNECTED_UES_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Fills buf with a compact JSON array.
 * Returns the number of bytes written (excluding the terminating NUL). */
size_t smf_dump_connected_ues(char *buf, size_t buflen);

#ifdef __cplusplus
}
#endif

#endif /* SMF_CONNECTED_UES_H */


