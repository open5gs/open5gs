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

#include "pcscf-fd-path.h"

abts_suite *test_volte(abts_suite *suite);

const struct testlist {
    abts_suite *(*func)(abts_suite *suite);
} alltests[] = {
    {test_volte},
    {NULL},
};

static ogs_thread_t *pcrf_thread = NULL;
static ogs_thread_t *pgw_thread = NULL;
static ogs_thread_t *sgw_thread = NULL;
static ogs_thread_t *hss_thread = NULL;
static ogs_thread_t *mme_thread = NULL;

static void terminate(void)
{
    ogs_msleep(50);

    test_child_terminate();

    if (mme_thread) ogs_thread_destroy(mme_thread);
    if (hss_thread) ogs_thread_destroy(hss_thread);
    if (sgw_thread) ogs_thread_destroy(sgw_thread);
    if (pgw_thread) ogs_thread_destroy(pgw_thread);
    if (pcrf_thread) ogs_thread_destroy(pcrf_thread);

    pcscf_fd_final();

    ogs_sctp_final();

    test_app_final();
    ogs_app_terminate();
}

static void initialize(const char *const argv[])
{
    int rv;

    test_no_mme_self = true;

    rv = ogs_app_initialize(NULL, argv);
    ogs_assert(rv == OGS_OK);

    pcrf_thread = test_child_create("pcrf", argv);
    pgw_thread = test_child_create("pgw", argv);
    sgw_thread = test_child_create("sgw", argv);
    hss_thread = test_child_create("hss", argv);
    mme_thread = test_child_create("mme", argv);

    test_app_init();
    ogs_sctp_init(ogs_config()->usrsctp.udp_port);

    rv = pcscf_fd_init();
    ogs_assert(rv == OGS_OK);
}

int main(int argc, const char *const argv[])
{
    int i;
    abts_suite *suite = NULL;

    atexit(terminate);
    test_app_run(argc, argv, "volte.yaml", initialize);

    for (i = 0; alltests[i].func; i++)
        suite = alltests[i].func(suite);

    return abts_report(suite);
}
