/*
 * Copyright (C) 2019-2025 by Sukchan Lee <acetcom@gmail.com>
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

#ifndef PCF_NUDR_BUILD_H
#define PCF_NUDR_BUILD_H

#include "context.h"

#ifdef __cplusplus
extern "C" {
#endif

ogs_sbi_request_t *pcf_nudr_dr_build_query_am_data(
        pcf_ue_am_t *pcf_ue_am, void *data);

ogs_sbi_request_t *pcf_nudr_dr_build_query_sm_data(
        pcf_sess_t *sess, void *data);

#ifdef __cplusplus
}
#endif

#endif /* PCF_NUDR_BUILD_H */
