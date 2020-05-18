/*
 * Copyright (C) 2019 by Sukchan Lee <acetcom@gmail.com>
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

#ifndef OGS_SBI_H
#define OGS_SBI_H

#include "ogs-core.h"

#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstrict-prototypes"
#endif

#include "model/nf_profile.h"
#include "model/nf_group_cond.h"
#include "model/smf_info.h"
#include "model/problem_details.h"
#include "model/patch_item.h"
#include "model/subscription_data.h"
#include "model/notification_data.h"
#include "model/search_result.h"

#include "custom/links.h"

#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

#define OGS_SBI_INSIDE

#include "sbi/conv.h"
#include "sbi/message.h"

#include "sbi/server.h"
#include "sbi/client.h"
#include "sbi/context.h"

#include "sbi/nnrf-build.h"
#include "sbi/nnrf-handler.h"

#undef OGS_SBI_INSIDE

#ifdef __cplusplus
extern "C" {
#endif

extern int __ogs_sbi_domain;

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __ogs_sbi_domain

#ifdef __cplusplus
}
#endif

#endif /* OGS_SBI_H */
