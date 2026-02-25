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

#ifndef LMF_SBI_PATH_H
#define LMF_SBI_PATH_H

#include "ogs-sbi.h"
#include "context.h"

#ifdef __cplusplus
extern "C" {
#endif

int lmf_sbi_open(void);
void lmf_sbi_close(void);

int lmf_amf_sbi_discover_and_send(
        ogs_sbi_service_type_e service_type,
        ogs_sbi_discovery_option_t *discovery_option,
        ogs_sbi_request_t *(*build)(lmf_location_request_t *location_request, void *data),
        lmf_location_request_t *location_request, void *data);

#ifdef __cplusplus
}
#endif

#endif /* LMF_SBI_PATH_H */

