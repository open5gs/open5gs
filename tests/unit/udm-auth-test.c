/*
 * Copyright (C) 2026 by Sukchan Lee <acetcom@gmail.com>
 *
 * This file is part of Open5GS.
 *
 * Open5GS is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Open5GS is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with Open5GS. If not, see <https://www.gnu.org/licenses/>.
 */

#include "udm/context.h"
#include "udm/nudm-handler.h"
#include "udm/sbi-path.h"
#include "core/abts.h"

#define TEST_SUCI "imsi-001010000000001"
#define TEST_AUSF_INSTANCE_ID "8f5c0e9e-4e2a-4b6b-9d5b-2f4e1c0a9b7d"

static int test_status;

static bool test_send_error(ogs_sbi_stream_t *stream, int status,
        ogs_sbi_message_t *message, const char *title,
        const char *detail, const char *cause)
{
    test_status = status;
    return true;
}

static int test_discover(OpenAPI_service_name_e service_name,
        ogs_sbi_discovery_option_t *discovery_option,
        ogs_sbi_request_t *(*build)(udm_ue_t *udm_ue, void *data),
        udm_ue_t *udm_ue, ogs_sbi_stream_t *stream, int state, void *data)
{
    return OGS_OK;
}

/*
 * Run the N13 generate-auth-data handler without an SBI server or a UDR.
 * Only the request boundary is exercised: an authenticated subscriber is
 * looked up by the UDM after this point, which is out of scope here.
 */
#define ogs_sbi_server_send_error(...) test_send_error(__VA_ARGS__)
#define udm_ue_sbi_discover_and_send(...) test_discover(__VA_ARGS__)
#include "udm/nudm-handler.c"

static char *make_serving_network_name(int len)
{
    char *s = ogs_calloc(1, len + 1);

    ogs_assert(s);
    memset(s, 'A', len);

    return s;
}

static bool handle_auth_request(
        udm_ue_t *udm_ue, char *serving_network_name)
{
    ogs_sbi_message_t recvmsg;
    OpenAPI_authentication_info_request_t AuthenticationInfoRequest;
    char stream;

    memset(&AuthenticationInfoRequest, 0, sizeof(AuthenticationInfoRequest));
    AuthenticationInfoRequest.serving_network_name = serving_network_name;
    AuthenticationInfoRequest.ausf_instance_id =
        (char *)TEST_AUSF_INSTANCE_ID;

    memset(&recvmsg, 0, sizeof(recvmsg));
    recvmsg.AuthenticationInfoRequest = &AuthenticationInfoRequest;

    test_status = 0;

    return udm_nudm_ueau_handle_get(
            udm_ue, (ogs_sbi_stream_t *)&stream, &recvmsg);
}

static void ue_init(udm_ue_t *udm_ue)
{
    memset(udm_ue, 0, sizeof(*udm_ue));
    udm_ue->suci = (char *)TEST_SUCI;
}

static void ue_final(udm_ue_t *udm_ue)
{
    if (udm_ue->serving_network_name)
        ogs_free(udm_ue->serving_network_name);
    if (udm_ue->ausf_instance_id)
        ogs_free(udm_ue->ausf_instance_id);
}

/*
 * TS 24.501 clause 9.12.1 caps the Serving Network Name at 1,020 octets.
 * The KDF stores parameter lengths in a uint16_t, so a 65,536-octet name
 * truncates to zero and trips the ogs_kdf_common() assertion. The request
 * must be rejected at the boundary instead of being stored.
 */
static void test_oversized_serving_network_name(abts_case *tc, void *data)
{
    udm_ue_t udm_ue;
    char *serving_network_name = make_serving_network_name(65536);

    ue_init(&udm_ue);

    ABTS_INT_EQUAL(tc, false,
            handle_auth_request(&udm_ue, serving_network_name));
    ABTS_INT_EQUAL(tc, OGS_SBI_HTTP_STATUS_BAD_REQUEST, test_status);
    ABTS_PTR_EQUAL(tc, NULL, udm_ue.serving_network_name);

    ue_final(&udm_ue);
    ogs_free(serving_network_name);
}

static void test_too_long_serving_network_name(abts_case *tc, void *data)
{
    udm_ue_t udm_ue;
    char *serving_network_name = make_serving_network_name(1021);

    ue_init(&udm_ue);

    ABTS_INT_EQUAL(tc, false,
            handle_auth_request(&udm_ue, serving_network_name));
    ABTS_INT_EQUAL(tc, OGS_SBI_HTTP_STATUS_BAD_REQUEST, test_status);
    ABTS_PTR_EQUAL(tc, NULL, udm_ue.serving_network_name);

    ue_final(&udm_ue);
    ogs_free(serving_network_name);
}

static void test_max_length_serving_network_name(abts_case *tc, void *data)
{
    udm_ue_t udm_ue;
    char *serving_network_name = make_serving_network_name(1020);

    ue_init(&udm_ue);

    ABTS_INT_EQUAL(tc, true,
            handle_auth_request(&udm_ue, serving_network_name));
    ABTS_INT_EQUAL(tc, 0, test_status);
    ABTS_PTR_NOTNULL(tc, udm_ue.serving_network_name);
    ABTS_INT_EQUAL(tc, 1020, strlen(udm_ue.serving_network_name));

    ue_final(&udm_ue);
    ogs_free(serving_network_name);
}

static void test_normal_serving_network_name(abts_case *tc, void *data)
{
    udm_ue_t udm_ue;
    char *serving_network_name = make_serving_network_name(32);

    ue_init(&udm_ue);

    ABTS_INT_EQUAL(tc, true,
            handle_auth_request(&udm_ue, serving_network_name));
    ABTS_INT_EQUAL(tc, 0, test_status);
    ABTS_PTR_NOTNULL(tc, udm_ue.serving_network_name);

    ue_final(&udm_ue);
    ogs_free(serving_network_name);
}

abts_suite *test_udm_auth(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    ogs_log_install_domain(&__udm_log_domain, "udm", ogs_core()->log.level);

    abts_run_test(suite, test_oversized_serving_network_name, NULL);
    abts_run_test(suite, test_too_long_serving_network_name, NULL);
    abts_run_test(suite, test_max_length_serving_network_name, NULL);
    abts_run_test(suite, test_normal_serving_network_name, NULL);

    return suite;
}
