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
 * Minimal public API for /pdu-info
 */
#ifndef SMF_PDU_INFO_H
#define SMF_PDU_INFO_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif


#ifndef PDU_INFO_PAGE_SIZE_DEFAULT
#define PDU_INFO_PAGE_SIZE_DEFAULT 100U
#endif

size_t smf_dump_pdu_info(char *buf, size_t buflen,
        size_t page, size_t page_size);
size_t smf_dump_pdu_info_paged(char *buf, size_t buflen,
        size_t page, size_t page_size);
size_t smf_dump_upf_info(char *buf, size_t buflen,
        size_t page, size_t page_size);

/* UPF administrative (draining) state for safe scale-in. */
int smf_upf_admin_is_draining(const char *addr);
void smf_upf_admin_set(const char *addr, int draining);
size_t smf_handle_upf_admin(char *buf, size_t buflen,
        const char *method, const char *body, size_t body_len,
        int *status_code);
#ifdef __cplusplus
}
#endif

#endif 
