/*
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

#include "ogs-app.h"
#include "core/abts.h"

#define SESSION_HEAD \
    "session:\n" \
    "  - name: internet\n" \
    "    type: 3\n" \
    "    ambr:\n" \
    "      downlink: { value: 1, unit: 3 }\n" \
    "      uplink: { value: 1, unit: 3 }\n" \
    "    qos:\n" \
    "      index: 9\n" \
    "      arp: { priority_level: 8, pre_emption_vulnerability: 1, " \
                "pre_emption_capability: 1 }\n" \
    "    pcc_rule:\n"

#define FULL_ARP \
    "arp: { priority_level: 1, pre_emption_vulnerability: 1, " \
          "pre_emption_capability: 1 }"

/* A skipped rule (no ARP) must not pass its flows to the next rule */
static const char *skipped_rule_flows =
    SESSION_HEAD
    "      - qos:\n"
    "          index: 1\n"
    "        flow:\n"
    "          - direction: 2\n"
    "            description: \"permit out udp from 10.0.0.1 to assigned\"\n"
    "      - qos:\n"
    "          index: 5\n"
    "          " FULL_ARP "\n"
    "        flow:\n"
    "          - direction: 1\n"
    "            description: \"permit out udp from 10.0.0.2 to assigned\"\n";

/* A skipped rule must not pass its QCI to a next rule that has none */
static const char *skipped_rule_qos =
    SESSION_HEAD
    "      - qos:\n"
    "          index: 1\n"
    "      - qos:\n"
    "          " FULL_ARP "\n";

/* A flow with only a direction must not merge with the next flow */
static const char *partial_flows =
    SESSION_HEAD
    "      - qos:\n"
    "          index: 5\n"
    "          " FULL_ARP "\n"
    "        flow:\n"
    "          - direction: 2\n"
    "          - description: \"permit out udp from 10.0.0.3 to assigned\"\n";

static ogs_session_data_t *parse(abts_case *tc, const char *yaml,
        yaml_document_t *document)
{
    yaml_parser_t parser;
    ogs_yaml_iter_t iter;
    ogs_supi_range_t supi_range;
    ogs_s_nssai_t s_nssai;
    ogs_app_policy_conf_t *policy_conf = NULL;
    ogs_app_slice_conf_t *slice_conf = NULL;
    ogs_app_session_conf_t *session_conf = NULL;
    ogs_log_level_e level = ogs_log_get_domain_level(__ogs_app_domain);
    int rv;

    rv = yaml_parser_initialize(&parser);
    ABTS_TRUE(tc, rv);
    yaml_parser_set_input_string(&parser,
            (const unsigned char *)yaml, strlen(yaml));
    rv = yaml_parser_load(&parser, document);
    yaml_parser_delete(&parser);
    ABTS_TRUE(tc, rv);
    if (!rv)
        return NULL;

    memset(&supi_range, 0, sizeof(supi_range));
    supi_range.num = 1;
    supi_range.start[0] = 999700000000001ULL;
    supi_range.end[0] = 999709999999999ULL;
    policy_conf = ogs_app_policy_conf_add(&supi_range, NULL);
    ABTS_PTR_NOTNULL(tc, policy_conf);

    s_nssai.sst = 1;
    s_nssai.sd.v = OGS_S_NSSAI_NO_SD_VALUE;
    slice_conf = ogs_app_slice_conf_add(policy_conf, &s_nssai);
    ABTS_PTR_NOTNULL(tc, slice_conf);

    ogs_yaml_iter_init(&iter, document);
    rv = ogs_yaml_iter_next(&iter);
    ABTS_TRUE(tc, rv);

    /* The skipped rules log a warning; keep the test output clean */
    ogs_log_set_domain_level(__ogs_app_domain, OGS_LOG_ERROR);
    rv = ogs_app_parse_session_conf(&iter, slice_conf);
    ogs_log_set_domain_level(__ogs_app_domain, level);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    session_conf = ogs_app_session_conf_find_by_dnn(slice_conf, "internet");
    ABTS_PTR_NOTNULL(tc, session_conf);
    return session_conf ? &session_conf->data : NULL;
}

static void pcc_rule_skipped_rule_flows(abts_case *tc, void *data)
{
    yaml_document_t document;
    ogs_session_data_t *session_data;

    ogs_app_config_init();
    session_data = parse(tc, skipped_rule_flows, &document);
    if (session_data) {
        ABTS_INT_EQUAL(tc, 1, session_data->num_of_pcc_rule);
        ABTS_STR_EQUAL(tc, "internet-g1", session_data->pcc_rule[0].name);
        ABTS_INT_EQUAL(tc, 5, session_data->pcc_rule[0].qos.index);
        ABTS_INT_EQUAL(tc, 1, session_data->pcc_rule[0].num_of_flow);
        ABTS_INT_EQUAL(tc, OGS_FLOW_DOWNLINK_ONLY,
                session_data->pcc_rule[0].flow[0].direction);
        ABTS_STR_EQUAL(tc, "permit out udp from 10.0.0.2 to assigned",
                session_data->pcc_rule[0].flow[0].description);
    }
    yaml_document_delete(&document);
    ogs_app_config_final();
}

static void pcc_rule_skipped_rule_qos(abts_case *tc, void *data)
{
    yaml_document_t document;
    ogs_session_data_t *session_data;

    ogs_app_config_init();
    session_data = parse(tc, skipped_rule_qos, &document);
    if (session_data)
        ABTS_INT_EQUAL(tc, 0, session_data->num_of_pcc_rule);
    yaml_document_delete(&document);
    ogs_app_config_final();
}

static void pcc_rule_partial_flows(abts_case *tc, void *data)
{
    yaml_document_t document;
    ogs_session_data_t *session_data;

    ogs_app_config_init();
    session_data = parse(tc, partial_flows, &document);
    if (session_data) {
        ABTS_INT_EQUAL(tc, 1, session_data->num_of_pcc_rule);
        ABTS_INT_EQUAL(tc, 0, session_data->pcc_rule[0].num_of_flow);
    }
    yaml_document_delete(&document);
    ogs_app_config_final();
}

abts_suite *test_pcc_rule_config(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, pcc_rule_skipped_rule_flows, NULL);
    abts_run_test(suite, pcc_rule_skipped_rule_qos, NULL);
    abts_run_test(suite, pcc_rule_partial_flows, NULL);

    return suite;
}
