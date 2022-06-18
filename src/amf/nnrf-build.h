/*
 * Copyright (C) 2019,2020 by Sukchan Lee <acetcom@gmail.com>
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

#ifndef AMF_NNRF_BUILD_H
#define AMF_NNRF_BUILD_H

#include "context.h"

#ifdef __cplusplus
extern "C" {
#endif

ogs_sbi_request_t *amf_nnrf_nfm_build_register(void);

ogs_sbi_request_t *amf_nnrf_disc_build_discover(
        char *nrf_id,
        OpenAPI_nf_type_e target_nf_type, OpenAPI_nf_type_e requester_nf_type);

#ifdef __cplusplus
}
#endif

#endif /* AMF_NNRF_BUILD_H */
