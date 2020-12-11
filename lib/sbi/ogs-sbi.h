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
#include "model/authentication_info.h"
#include "model/authentication_info_request.h"
#include "model/authentication_info_result.h"
#include "model/authentication_subscription.h"
#include "model/ue_authentication_ctx.h"
#include "model/confirmation_data.h"
#include "model/confirmation_data_response.h"
#include "model/auth_event.h"
#include "model/amf3_gpp_access_registration.h"
#include "model/access_and_mobility_subscription_data.h"
#include "model/smf_selection_subscription_data.h"
#include "model/ue_context_in_smf_data.h"
#include "model/sm_context_create_data.h"
#include "model/sm_context_created_data.h"
#include "model/sm_context_create_error.h"
#include "model/sm_context_update_data.h"
#include "model/sm_context_updated_data.h"
#include "model/sm_context_update_error.h"
#include "model/sm_context_release_data.h"
#include "model/sm_context_released_data.h"
#include "model/session_management_subscription_data.h"
#include "model/n1_n2_message_transfer_req_data.h"
#include "model/n1_n2_message_transfer_rsp_data.h"
#include "model/sm_context_status_notification.h"
#include "model/policy_association.h"
#include "model/am_policy_data.h"
#include "model/sm_policy_context_data.h"
#include "model/sm_policy_decision.h"
#include "model/sm_policy_data.h"

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

#include "sbi/path.h"

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
