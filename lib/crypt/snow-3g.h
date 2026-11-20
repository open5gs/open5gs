/*
 * Copyright (C) 2026 by Sukchan Lee <acetcom@gmail.com>
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

#if !defined(OGS_CRYPT_INSIDE) && !defined(OGS_CRYPT_COMPILATION)
#error "This header cannot be included directly."
#endif

#ifndef SNOW_3G_H
#define SNOW_3G_H

#include "ogs-core.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Compute a 4-byte UIA2 MAC over length bits of data. */
void snow_3g_f9(uint8_t *key, uint32_t count, uint32_t fresh, uint32_t dir,
        uint8_t *data, uint64_t length, uint8_t *out);

#ifdef __cplusplus
}
#endif

#endif /* SNOW_3G_H */
