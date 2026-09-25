/*
 * Copyright (C) 2026 by Sukchan Lee <acetcom@gmail.com>
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

#include "ogs-app.h"
#include "core/abts.h"

typedef struct eir_config_case_s {
    const char *yaml;
    ogs_eir_action_e initial_action;
    int result;
    ogs_eir_action_e expected_action;
} eir_config_case_t;

static const eir_config_case_t cases[] = {
    { "unknown_action: allow", OGS_EIR_ACTION_REJECT,
        OGS_OK, OGS_EIR_ACTION_ALLOW },
    { "unknown_action: reject", OGS_EIR_ACTION_ALLOW,
        OGS_OK, OGS_EIR_ACTION_REJECT },
    { "failure_action: allow", OGS_EIR_ACTION_REJECT,
        OGS_OK, OGS_EIR_ACTION_ALLOW },
    { "failure_action: reject", OGS_EIR_ACTION_ALLOW,
        OGS_OK, OGS_EIR_ACTION_REJECT },
    { "missing_pei_action: allow", OGS_EIR_ACTION_REJECT,
        OGS_OK, OGS_EIR_ACTION_ALLOW },
    { "missing_pei_action: reject", OGS_EIR_ACTION_ALLOW,
        OGS_OK, OGS_EIR_ACTION_REJECT },
    { "unknown_action: 'allow'", OGS_EIR_ACTION_REJECT,
        OGS_OK, OGS_EIR_ACTION_ALLOW },
    { "failure_action: \"reject\"", OGS_EIR_ACTION_ALLOW,
        OGS_OK, OGS_EIR_ACTION_REJECT },
    /* Invalid values must not replace an existing policy with a default. */
    { "unknown_action: Allow", OGS_EIR_ACTION_REJECT,
        OGS_ERROR, OGS_EIR_ACTION_REJECT },
    { "failure_action: REJECT", OGS_EIR_ACTION_ALLOW,
        OGS_ERROR, OGS_EIR_ACTION_ALLOW },
    { "missing_pei_action: deny", OGS_EIR_ACTION_REJECT,
        OGS_ERROR, OGS_EIR_ACTION_REJECT },
    { "unknown_action: ' allow'", OGS_EIR_ACTION_REJECT,
        OGS_ERROR, OGS_EIR_ACTION_REJECT },
    { "failure_action: 'reject '", OGS_EIR_ACTION_ALLOW,
        OGS_ERROR, OGS_EIR_ACTION_ALLOW },
    { "missing_pei_action: 1", OGS_EIR_ACTION_REJECT,
        OGS_ERROR, OGS_EIR_ACTION_REJECT },
    /* YAML null/empty scalars are strings, not a NULL value pointer. */
    { "unknown_action:", OGS_EIR_ACTION_REJECT,
        OGS_ERROR, OGS_EIR_ACTION_REJECT },
    { "failure_action: null", OGS_EIR_ACTION_REJECT,
        OGS_ERROR, OGS_EIR_ACTION_REJECT },
    { "missing_pei_action: ''", OGS_EIR_ACTION_REJECT,
        OGS_ERROR, OGS_EIR_ACTION_REJECT },
};

static void eir_config_parse_action(abts_case *tc, void *data)
{
    const eir_config_case_t *test = data;
    yaml_parser_t parser;
    yaml_document_t document;
    ogs_yaml_iter_t iter;
    ogs_eir_action_e action = test->initial_action;
    int rv;

    rv = yaml_parser_initialize(&parser);
    ABTS_TRUE(tc, rv);
    if (!rv)
        return;
    yaml_parser_set_input_string(&parser,
            (const unsigned char *)test->yaml, strlen(test->yaml));
    rv = yaml_parser_load(&parser, &document);
    yaml_parser_delete(&parser);
    ABTS_TRUE(tc, rv);
    if (!rv)
        return;

    ogs_yaml_iter_init(&iter, &document);
    rv = ogs_yaml_iter_next(&iter);
    ABTS_TRUE(tc, rv);
    if (rv) {
        rv = ogs_app_parse_eir_action(
                &iter, ogs_yaml_iter_key(&iter), &action);
        ABTS_INT_EQUAL(tc, test->result, rv);
        ABTS_INT_EQUAL(tc, test->expected_action, action);
    }

    yaml_document_delete(&document);
}

abts_suite *test_eir_config(abts_suite *suite)
{
    ogs_log_level_e saved_log_level;
    unsigned int i;

    suite = ADD_SUITE(suite)

    if (!ogs_log_find_domain("app"))
        ogs_log_install_domain(
                &__ogs_app_domain, "app", ogs_core()->log.level);
    saved_log_level = ogs_log_get_domain_level(__ogs_app_domain);
    ogs_log_set_domain_level(__ogs_app_domain, OGS_LOG_NONE);

    for (i = 0; i < OGS_ARRAY_SIZE(cases); i++)
        abts_run_test(suite, eir_config_parse_action, (void *)&cases[i]);

    ogs_log_set_domain_level(__ogs_app_domain, saved_log_level);
    return suite;
}
