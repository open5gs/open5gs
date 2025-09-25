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
 * /ue-info — MME-side JSON exporter (Prometheus HTTP endpoint)
 *
 * License: AGPLv3+
 */

/* lib/metrics/prometheus/json_pager.h  */

#ifndef OGS_METRICS_PROM_JSON_PAGER_H
#define OGS_METRICS_PROM_JSON_PAGER_H

#include <stddef.h>
#include <stdbool.h>

/* Forward-declare so callers don't need to include cJSON.h here */
typedef struct cJSON cJSON;

/* Safe start_index = page * page_size with overflow guard. */
size_t json_pager_safe_start_index(bool no_paging, size_t page, size_t page_size);

/* Append trailing { "pager": { page, page_size, count, [truncated], prev?, next? } }.  */
void json_pager_add_trailing(cJSON *root,
                             bool no_paging,
                             size_t page,
                             size_t page_size,
                             size_t count,
                             bool has_next,
                             const char *base_path,
                             bool truncated);

/* Finalize JSON into buf and free root. */
size_t json_pager_finalize(cJSON *root, char *buf, size_t buflen);

/* Paging step helper
 * Returns: 1 -> CONTINUE (skip), 2 -> BREAK (page full; set has_next), 0 -> EMIT.
 */
static inline int
json_pager_advance(bool no_paging,
                   size_t idx, size_t start_index,
                   size_t emitted, size_t page_size,
                   bool *has_next)
{
    if (no_paging) return 0;
    if (idx < start_index) return 1;
    if (emitted == page_size) { if (has_next) *has_next = true; return 2; }
    return 0;
}

#endif /* OGS_METRICS_PROM_JSON_PAGER_H */


