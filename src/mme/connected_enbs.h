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
 * /connected-gnbs — MME-side JSON exporter (Prometheus HTTP endpoint)
 *
 * License: AGPLv3+
 */
#pragma once

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* JSON dumper for /connected-gnbs.
 * Returns number of bytes written (<= buflen-1), buffer is always NUL-terminated.
 */
size_t mme_dump_connected_enbs(char *buf, size_t buflen);

#ifdef __cplusplus
}
#endif

