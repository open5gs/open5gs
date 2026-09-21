/*
 * Copyright (C) 2019-2026 by Arthur SC Chan <arthur.chan@adalogics.com>
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

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "fuzzing.h"
#include "ogs-sbi.h"

#include "model/access_and_mobility_subscription_data.h"
#include "model/dnn_configuration.h"
#include "model/event_notification.h"
#include "model/event_subscription.h"
#include "model/events_notification.h"
#include "model/hsmf_update_data.h"
#include "model/mdt_configuration.h"
#include "model/media_component.h"
#include "model/media_component_rm.h"
#include "model/pcc_rule.h"
#include "model/pdu_session_create_data.h"
#include "model/pdu_session_created_data.h"
#include "model/policy_association_request.h"
#include "model/sm_context_update_data.h"
#include "model/sm_policy_context_data.h"
#include "model/sm_policy_decision.h"
#include "model/subscr_cond.h"
#include "model/ue_context.h"
#include "model/vsmf_update_data.h"

#define kMinInputLength 2
#define kMaxInputLength 16384

#define TRY_MODEL(name)                                                     \
    do {                                                                    \
        cJSON *json = cJSON_Parse(text);                                    \
        if (json) {                                                         \
            OpenAPI_##name##_t *model =                                     \
                OpenAPI_##name##_parseFromJSON(json);                       \
            if (model) {                                                    \
                OpenAPI_##name##_free(model);                               \
            }                                                               \
            cJSON_Delete(json);                                             \
        }                                                                   \
    } while (0)

extern int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size)
{
    char *text;

    if (Size < kMinInputLength || Size > kMaxInputLength) {
        return 1;
    }

    if (!initialized) {
        initialize();
        ogs_log_install_domain(&__ogs_sbi_domain, "sbi", OGS_LOG_NONE);
    }

    /* cJSON_Parse needs a NUL-terminated string, as the HTTP layer provides. */
    text = ogs_malloc(Size + 1);
    if (text == NULL) {
        return 0;
    }
    memcpy(text, Data, Size);
    text[Size] = '\0';

    /* AMF and SMF context transfer */
    TRY_MODEL(ue_context);
    TRY_MODEL(sm_context_update_data);
    TRY_MODEL(pdu_session_create_data);
    TRY_MODEL(pdu_session_created_data);
    TRY_MODEL(hsmf_update_data);
    TRY_MODEL(vsmf_update_data);

    /* Subscription and event exposure */
    TRY_MODEL(event_subscription);
    TRY_MODEL(events_notification);
    TRY_MODEL(event_notification);
    TRY_MODEL(subscr_cond);

    /* Subscriber data from UDM/UDR */
    TRY_MODEL(access_and_mobility_subscription_data);
    TRY_MODEL(dnn_configuration);
    TRY_MODEL(mdt_configuration);

    /* Policy control */
    TRY_MODEL(sm_policy_context_data);
    TRY_MODEL(sm_policy_decision);
    TRY_MODEL(policy_association_request);
    TRY_MODEL(pcc_rule);
    TRY_MODEL(media_component);
    TRY_MODEL(media_component_rm);

    ogs_free(text);

    return 0;
}
