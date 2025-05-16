/*
 * Copyright (C) 2023 by Ryan Dimsey <ryan@omnitouch.com.au>
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

#include "nas-path.h"
#include "s1ap-path.h"
#include "sgsap-path.h"
#include "mme-path.h"

#include "mme-sm.h"
#include "mme-s13-handler.h"

/* Unfortunately fd doesn't distinguish
 * between result-code and experimental-result-code.
 *
 * However, e.g. 5004 has different meaning
 * if used in result-code than in experimental-result-code */
static uint8_t emm_cause_from_diameter(
                const uint32_t *dia_err, const uint32_t *dia_exp_err);
static bool is_valid_equipment(uint32_t equipment_status_code, ogs_nas_eir_t eir_config);

uint8_t validate_s13_message(ogs_diam_s13_message_t *s13_message) {
    ogs_assert(s13_message);

    if (s13_message->result_code != ER_DIAMETER_SUCCESS) {
        ogs_warn("ME-Identity-Check failed [%d]",
                    s13_message->result_code);
        return emm_cause_from_diameter(s13_message->err, s13_message->exp_err);
    }

    return OGS_OK;
}

uint8_t validate_eca(ogs_diam_s13_eca_message_t eca_message, ogs_nas_eir_t eir_config) {
    if (is_valid_equipment(eca_message.equipment_status_code, eir_config)) {
        ogs_info("ME-Identity-Check accepted for equiptment status code '%d'", eca_message.equipment_status_code); 
        return OGS_NAS_EMM_CAUSE_REQUEST_ACCEPTED;
    }
    else {
        ogs_info("ME-Identity-Check rejected for equiptment status code '%d'", eca_message.equipment_status_code);
        return OGS_NAS_EMM_CAUSE_ILLEGAL_ME;
    }
}

uint8_t mme_s13_handle_eca(
        mme_ue_t *mme_ue, ogs_diam_s13_message_t *s13_message)
{
    uint8_t s13_validation_result = validate_s13_message(s13_message);

    if (OGS_OK != s13_validation_result) {
        return s13_validation_result;
    }

    return validate_eca(s13_message->eca_message, mme_self()->eir);
}

/* 3GPP TS 29.272 Annex A; Table A.1:
 * Mapping from S6a error codes to NAS Cause Codes */
static uint8_t emm_cause_from_diameter(
                const uint32_t *dia_err, const uint32_t *dia_exp_err)
{
    if (dia_exp_err) {
        switch (*dia_exp_err) {
        case OGS_DIAM_S13_ERROR_USER_UNKNOWN:                   /* 5001 */
            return OGS_NAS_EMM_CAUSE_PLMN_NOT_ALLOWED;
        case OGS_DIAM_S13_ERROR_UNKNOWN_EPS_SUBSCRIPTION:       /* 5420 */
            return OGS_NAS_EMM_CAUSE_NO_SUITABLE_CELLS_IN_TRACKING_AREA;
        case OGS_DIAM_S13_ERROR_RAT_NOT_ALLOWED:                /* 5421 */
            return OGS_NAS_EMM_CAUSE_ROAMING_NOT_ALLOWED_IN_THIS_TRACKING_AREA;
        case OGS_DIAM_S13_ERROR_ROAMING_NOT_ALLOWED:            /* 5004 */
            return OGS_NAS_EMM_CAUSE_PLMN_NOT_ALLOWED;
        case OGS_DIAM_S13_AUTHENTICATION_DATA_UNAVAILABLE:      /* 4181 */
            return OGS_NAS_EMM_CAUSE_NETWORK_FAILURE;
        }
    }
    if (dia_err) {
        switch (*dia_err) {
        case ER_DIAMETER_AUTHORIZATION_REJECTED:                /* 5003 */
        case ER_DIAMETER_UNABLE_TO_DELIVER:                     /* 3002 */
        case ER_DIAMETER_REALM_NOT_SERVED:                      /* 3003 */
            return OGS_NAS_EMM_CAUSE_NO_SUITABLE_CELLS_IN_TRACKING_AREA;
        case ER_DIAMETER_UNABLE_TO_COMPLY:                      /* 5012 */
        case ER_DIAMETER_INVALID_AVP_VALUE:                     /* 5004 */
        case ER_DIAMETER_AVP_UNSUPPORTED:                       /* 5001 */
        case ER_DIAMETER_MISSING_AVP:                           /* 5005 */
        case ER_DIAMETER_RESOURCES_EXCEEDED:                    /* 5006 */
        case ER_DIAMETER_AVP_OCCURS_TOO_MANY_TIMES:             /* 5009 */
            return OGS_NAS_EMM_CAUSE_NETWORK_FAILURE;
        }
    }

    ogs_error("Unexpected Diameter Result Code %d/%d, defaulting to severe "
              "network failure",
              dia_err ? *dia_err : -1, dia_exp_err ? *dia_exp_err : -1);
    return OGS_NAS_EMM_CAUSE_SEVERE_NETWORK_FAILURE;
}

static bool is_valid_equipment(uint32_t equipment_status_code, ogs_nas_eir_t eir_config) {
    bool is_valid = false;

    if ((OGS_DIAM_S13_EQUIPMENT_WHITELIST == equipment_status_code) && 
        (eir_config.allow_whitelist)) {
        is_valid = true;
    } else if ((OGS_DIAM_S13_EQUIPMENT_GREYLIST == equipment_status_code) && 
        (eir_config.allow_greylist)) {
        is_valid = true;
    } else if ((OGS_DIAM_S13_EQUIPMENT_BLACKLIST == equipment_status_code) && 
        (eir_config.allow_blacklist)) {
        is_valid = true;
    }

    return is_valid;
}