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

#include "test-app.h"
#include "mme/mme-context.h"

abts_suite *test_s1ap_message(abts_suite *suite);
abts_suite *test_nas_message(abts_suite *suite);
abts_suite *test_gtp_message(abts_suite *suite);
abts_suite *test_sbi_message(abts_suite *suite);
abts_suite *test_security(abts_suite *suite);
abts_suite *test_crash(abts_suite *suite);

const struct testlist {
    abts_suite *(*func)(abts_suite *suite);
} alltests[] = {
    {test_s1ap_message},
    {test_nas_message},
    {test_gtp_message},
    {test_sbi_message},
    {test_security},
    {test_crash},
    {NULL},
};

static void terminate(void)
{
    mme_context_final();

    ogs_pkbuf_default_destroy();

    ogs_core_terminate();
}

int main(int argc, const char *const argv[])
{
    int rv, i, opt;
    ogs_getopt_t options;
    struct {
        char *log_level;
        char *domain_mask;
    } optarg;
    const char *argv_out[argc+2]; /* '-e error' is always added */
    
    abts_suite *suite = NULL;
    ogs_pkbuf_config_t config;

    rv = abts_main(argc, argv, argv_out);
    if (rv != OGS_OK) return rv;

    memset(&optarg, 0, sizeof(optarg));
    ogs_getopt_init(&options, (char**)argv_out);

    while ((opt = ogs_getopt(&options, "e:m:")) != -1) {
        switch (opt) {
        case 'e':
            optarg.log_level = options.optarg;
            break;
        case 'm':
            optarg.domain_mask = options.optarg;
            break;
        case '?':
        default:
            fprintf(stderr, "%s: should not be reached\n", OGS_FUNC);
            return OGS_ERROR;
        }
    }

    ogs_core_initialize();
    ogs_app_setup_log();

    ogs_pkbuf_default_init(&config);
    ogs_pkbuf_default_create(&config);

    ogs_app_context_init();
    mme_context_init();

    atexit(terminate);

    rv = ogs_log_config_domain(optarg.domain_mask, optarg.log_level);
    if (rv != OGS_OK) return rv;

    for (i = 0; alltests[i].func; i++)
        suite = alltests[i].func(suite);

    return abts_report(suite);
}
