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

#ifndef PWSIWF_SAI_NONUEN2_BUILD_H
#define PWSIWF_SAI_NONUEN2_BUILD_H

#include "context.h"
#include "sbi-path.h"

#ifdef __cplusplus
extern "C" {
#endif

ogs_sbi_request_t *pwsiwf_sai_nonuen2_comm_build_nonuen2_message_transfer(
        pwsiwf_sai_warning_t *warning, pwsiwf_sai_nonuen2_message_transfer_param_t *param);

ogs_sbi_request_t *pwsiwf_sai_nonuen2_callback_build_warning_status(
        pwsiwf_sai_warning_t *warning, void *data);


#ifdef __cplusplus
}
#endif

#endif /* PWSIWF_SAI_NONUEN2_BUILD_H */