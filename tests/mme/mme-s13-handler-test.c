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

#include "test-common.h"
#include "../../src/mme/mme-s13-handler.h"

static ogs_diam_s13_eca_message_t eca_message_whitelist = { .equipment_status_code = OGS_DIAM_S13_EQUIPMENT_WHITELIST };
static ogs_diam_s13_eca_message_t eca_message_blacklist = { .equipment_status_code = OGS_DIAM_S13_EQUIPMENT_BLACKLIST };
static ogs_diam_s13_eca_message_t eca_message_greylist = { .equipment_status_code = OGS_DIAM_S13_EQUIPMENT_GREYLIST };

static ogs_nas_eir_t eir_config_allow_none = { .allow_whitelist = false, .allow_blacklist = false, .allow_greylist = false };
static ogs_nas_eir_t eir_config_allow_all = { .allow_whitelist = true, .allow_blacklist = true, .allow_greylist = true };
static ogs_nas_eir_t eir_config_allow_whitelist = { .allow_whitelist = true, .allow_blacklist = false, .allow_greylist = false };
static ogs_nas_eir_t eir_config_allow_blacklist = { .allow_whitelist = false, .allow_blacklist = true, .allow_greylist = false };
static ogs_nas_eir_t eir_config_allow_greylist = { .allow_whitelist = false, .allow_blacklist = false, .allow_greylist = true };

static void mme_s13_validate_s13_message(abts_case *tc, void *data)
{
    enum { NUM_TEST_CASES = 16 };

    typedef struct {
        ogs_diam_s13_message_t s13_message;
        uint8_t expectedResult;
    } TestCase;

    uint32_t TEST_ER_DIAMETER_FAILURE = ER_DIAMETER_LIMITED_SUCCESS;

    uint32_t ERROR_USER_UNKNOWN = OGS_DIAM_S13_ERROR_USER_UNKNOWN;
    uint32_t ERROR_UNKNOWN_EPS_SUBSCRIPTION = OGS_DIAM_S13_ERROR_UNKNOWN_EPS_SUBSCRIPTION;
    uint32_t ERROR_RAT_NOT_ALLOWED = OGS_DIAM_S13_ERROR_RAT_NOT_ALLOWED;
    uint32_t ERROR_ROAMING_NOT_ALLOWED = OGS_DIAM_S13_ERROR_ROAMING_NOT_ALLOWED;
    uint32_t AUTHENTICATION_DATA_UNAVAILABLE = OGS_DIAM_S13_AUTHENTICATION_DATA_UNAVAILABLE;
    uint32_t AUTHORIZATION_REJECTED = ER_DIAMETER_AUTHORIZATION_REJECTED;
    uint32_t UNABLE_TO_DELIVER = ER_DIAMETER_UNABLE_TO_DELIVER;
    uint32_t REALM_NOT_SERVED = ER_DIAMETER_REALM_NOT_SERVED;
    uint32_t UNABLE_TO_COMPLY = ER_DIAMETER_UNABLE_TO_COMPLY;
    uint32_t INVALID_AVP_VALUE = ER_DIAMETER_INVALID_AVP_VALUE;
    uint32_t AVP_UNSUPPORTED = ER_DIAMETER_AVP_UNSUPPORTED;
    uint32_t MISSING_AVP = ER_DIAMETER_MISSING_AVP;
    uint32_t RESOURCES_EXCEEDED = ER_DIAMETER_RESOURCES_EXCEEDED;
    uint32_t AVP_OCCURS_TOO_MANY_TIMES = ER_DIAMETER_AVP_OCCURS_TOO_MANY_TIMES;

    uint32_t TEST_UNKNOWN_CODE = 420;

    TestCase testCases[NUM_TEST_CASES] = {
        /* s13_message valid case */
        { { .result_code = ER_DIAMETER_SUCCESS }, OGS_OK },

        /* s13_message.exp_err cases */
        { { .result_code = TEST_ER_DIAMETER_FAILURE, .exp_err = &ERROR_USER_UNKNOWN }, OGS_NAS_EMM_CAUSE_PLMN_NOT_ALLOWED },
        { { .result_code = TEST_ER_DIAMETER_FAILURE, .exp_err = &ERROR_UNKNOWN_EPS_SUBSCRIPTION }, OGS_NAS_EMM_CAUSE_NO_SUITABLE_CELLS_IN_TRACKING_AREA },
        { { .result_code = TEST_ER_DIAMETER_FAILURE, .exp_err = &ERROR_RAT_NOT_ALLOWED }, OGS_NAS_EMM_CAUSE_ROAMING_NOT_ALLOWED_IN_THIS_TRACKING_AREA },
        { { .result_code = TEST_ER_DIAMETER_FAILURE, .exp_err = &ERROR_ROAMING_NOT_ALLOWED }, OGS_NAS_EMM_CAUSE_PLMN_NOT_ALLOWED },
        { { .result_code = TEST_ER_DIAMETER_FAILURE, .exp_err = &AUTHENTICATION_DATA_UNAVAILABLE }, OGS_NAS_EMM_CAUSE_NETWORK_FAILURE },

        /* s13_message.err cases */
        { { .result_code = TEST_ER_DIAMETER_FAILURE, .err = &AUTHORIZATION_REJECTED }, OGS_NAS_EMM_CAUSE_NO_SUITABLE_CELLS_IN_TRACKING_AREA },
        { { .result_code = TEST_ER_DIAMETER_FAILURE, .err = &UNABLE_TO_DELIVER }, OGS_NAS_EMM_CAUSE_NO_SUITABLE_CELLS_IN_TRACKING_AREA },
        { { .result_code = TEST_ER_DIAMETER_FAILURE, .err = &REALM_NOT_SERVED }, OGS_NAS_EMM_CAUSE_NO_SUITABLE_CELLS_IN_TRACKING_AREA },
        { { .result_code = TEST_ER_DIAMETER_FAILURE, .err = &UNABLE_TO_COMPLY }, OGS_NAS_EMM_CAUSE_NETWORK_FAILURE },
        { { .result_code = TEST_ER_DIAMETER_FAILURE, .err = &INVALID_AVP_VALUE }, OGS_NAS_EMM_CAUSE_NETWORK_FAILURE },
        { { .result_code = TEST_ER_DIAMETER_FAILURE, .err = &AVP_UNSUPPORTED }, OGS_NAS_EMM_CAUSE_NETWORK_FAILURE },
        { { .result_code = TEST_ER_DIAMETER_FAILURE, .err = &MISSING_AVP }, OGS_NAS_EMM_CAUSE_NETWORK_FAILURE },
        { { .result_code = TEST_ER_DIAMETER_FAILURE, .err = &RESOURCES_EXCEEDED }, OGS_NAS_EMM_CAUSE_NETWORK_FAILURE },
        { { .result_code = TEST_ER_DIAMETER_FAILURE, .err = &AVP_OCCURS_TOO_MANY_TIMES }, OGS_NAS_EMM_CAUSE_NETWORK_FAILURE },
        
        /* Special case */
        { { .result_code = TEST_ER_DIAMETER_FAILURE, .err = &TEST_UNKNOWN_CODE, .exp_err = &TEST_UNKNOWN_CODE }, OGS_NAS_EMM_CAUSE_SEVERE_NETWORK_FAILURE },
    };

    for (int i = 0; i < NUM_TEST_CASES; ++i) {
        TestCase testCase = testCases[i];
        uint8_t result = validate_s13_message(&testCase.s13_message);

        ABTS_INT_EQUAL(tc, testCase.expectedResult, result);
    }
}

static void mme_s13_validate_eca(abts_case *tc, void *data)
{
    enum { NUM_TEST_CASES = 9 };

    typedef struct {
        ogs_nas_eir_t eir_config;
        ogs_diam_s13_eca_message_t eca_message;
        uint8_t expectedResult;
    } TestCase;

    TestCase testCases[NUM_TEST_CASES] = {
        { eir_config_allow_whitelist, eca_message_whitelist, OGS_NAS_EMM_CAUSE_REQUEST_ACCEPTED },
        { eir_config_allow_blacklist, eca_message_blacklist, OGS_NAS_EMM_CAUSE_REQUEST_ACCEPTED },
        { eir_config_allow_greylist, eca_message_greylist, OGS_NAS_EMM_CAUSE_REQUEST_ACCEPTED },

        { eir_config_allow_none, eca_message_whitelist, OGS_NAS_EMM_CAUSE_ILLEGAL_ME },
        { eir_config_allow_none, eca_message_blacklist, OGS_NAS_EMM_CAUSE_ILLEGAL_ME },
        { eir_config_allow_none, eca_message_greylist, OGS_NAS_EMM_CAUSE_ILLEGAL_ME },

        { eir_config_allow_all, eca_message_whitelist, OGS_NAS_EMM_CAUSE_REQUEST_ACCEPTED },
        { eir_config_allow_all, eca_message_blacklist, OGS_NAS_EMM_CAUSE_REQUEST_ACCEPTED },
        { eir_config_allow_all, eca_message_greylist, OGS_NAS_EMM_CAUSE_REQUEST_ACCEPTED },
    };

    for (int i = 0; i < NUM_TEST_CASES; ++i) {
        TestCase testCase = testCases[i];
        uint8_t result = validate_eca(testCase.eca_message, testCase.eir_config);

        ABTS_INT_EQUAL(tc, testCase.expectedResult, result);
    }
}

abts_suite *test_mme_s13_handler(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, mme_s13_validate_eca, NULL);
    abts_run_test(suite, mme_s13_validate_s13_message, NULL);

    return suite;
}
