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

#ifndef LMF_LOCATION_DETERMINATION_H
#define LMF_LOCATION_DETERMINATION_H

#include "context.h"
#include "nrppa-types.h"

#ifdef __cplusplus
extern "C" {
#endif

int lmf_location_determine(lmf_location_request_t *location_request);
int lmf_location_determine_ecid(
        lmf_location_request_t *location_request,
        lmf_nrppa_ecid_response_t *ecid_response);

#ifdef __cplusplus
}
#endif

#endif /* LMF_LOCATION_DETERMINATION_H */

