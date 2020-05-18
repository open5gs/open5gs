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

#include "ogs-core.h"
#include "core/abts.h"

abts_suite *test_list(abts_suite *suite);
abts_suite *test_pool(abts_suite *suite);
abts_suite *test_strings(abts_suite *suite);
abts_suite *test_time(abts_suite *suite);
abts_suite *test_conv(abts_suite *suite);
abts_suite *test_log(abts_suite *suite);
abts_suite *test_pkbuf(abts_suite *suite);
abts_suite *test_memory(abts_suite *suite);
abts_suite *test_rbtree(abts_suite *suite);
abts_suite *test_timer(abts_suite *suite);
abts_suite *test_thread(abts_suite *suite);
abts_suite *test_socket(abts_suite *suite);
abts_suite *test_queue(abts_suite *suite);
abts_suite *test_poll(abts_suite *suite);
abts_suite *test_tlv(abts_suite *suite);
abts_suite *test_fsm(abts_suite *suite);
abts_suite *test_hash(abts_suite *suite);
abts_suite *test_uuid(abts_suite *suite);

const struct testlist {
    abts_suite *(*func)(abts_suite *suite);
} alltests[] = {
    {test_list},
    {test_pool},
    {test_strings},
    {test_time},
    {test_conv},
    {test_log},
    {test_pkbuf},
    {test_memory},
    {test_rbtree},
    {test_timer},
    {test_thread},
    {test_socket},
    {test_queue},
    {test_poll},
    {test_tlv},
    {test_fsm},
    {test_hash},
    {test_uuid},
    {NULL},
};

static void terminate(void)
{
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
    ogs_pkbuf_default_init(&config);
    ogs_pkbuf_default_create(&config);
    atexit(terminate);

    rv = ogs_log_config_domain(optarg.domain_mask, optarg.log_level);
    if (rv != OGS_OK) return rv;

    for (i = 0; alltests[i].func; i++)
        suite = alltests[i].func(suite);

    return abts_report(suite);
}
