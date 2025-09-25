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

/* lib/metrics/prometheus/json_pager.c */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "ogs-core.h"
#include "ogs-proto.h"
#include "sbi/openapi/external/cJSON.h"

#include "metrics/prometheus/json_pager.h"

size_t
json_pager_safe_start_index(bool no_paging, size_t page, size_t page_size)
{
    if (no_paging) return 0;
    if (page_size != 0 && page > SIZE_MAX / page_size)
        return SIZE_MAX; /* treat as beyond end */
    return page * page_size;
}

void
json_pager_add_trailing(cJSON *root,
                        bool no_paging,
                        size_t page,
                        size_t page_size,
                        size_t count,
                        bool has_next,
                        const char *base_path,
                        bool truncated)
{
    cJSON *pager = cJSON_CreateObject();
    cJSON_AddNumberToObject(pager, "page",      (double)(no_paging ? 0 : page));
    cJSON_AddNumberToObject(pager, "page_size", (double)(no_paging ? 0 : page_size));
    cJSON_AddNumberToObject(pager, "count",     (double)count);

    /* Only add "truncated" when true */
    if (truncated)
        cJSON_AddBoolToObject(pager, "truncated", 1);

    if (!no_paging) {
        if (page > 0) {
            char prev[128];
            snprintf(prev, sizeof prev, "%s?page=%zu", base_path, page - 1);
            cJSON_AddStringToObject(pager, "prev", prev);
        }
        if (has_next) {
            char next[128];
            snprintf(next, sizeof next, "%s?page=%zu", base_path, page + 1);
            cJSON_AddStringToObject(pager, "next", next);
        }
    }

    cJSON_AddItemToObject(root, "pager", pager);
}

size_t
json_pager_finalize(cJSON *root, char *buf, size_t buflen)
{
    size_t outlen = 0;

    if (cJSON_PrintPreallocated(root, buf, (int)buflen, 0)) {
        outlen = strlen(buf);
        cJSON_Delete(root);
        return outlen;
    }

    char *tmp = cJSON_PrintUnformatted(root);
    if (!tmp) {
        if (buflen >= 3) {
            memcpy(buf, "{}", 3);
            cJSON_Delete(root);
            return 2;
        }
        if (buflen) buf[0] = '\0';
        cJSON_Delete(root);
        return 0;
    }

    size_t need = strlen(tmp);
    if (need + 1 <= buflen) {
        memcpy(buf, tmp, need + 1);
        outlen = need;
    } else {
        if (buflen >= 3) {
            memcpy(buf, "{}", 3);
            outlen = 2;
        } else if (buflen) {
            buf[0] = '\0';
        }
    }
    cJSON_free(tmp);
    cJSON_Delete(root);
    return outlen;
}

