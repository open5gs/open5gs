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

#include "fd/fd-lib.h"
#include "mme/ogs-sctp.h"

#include "app/application.h"
#include "app/context.h"

#include "app-init.h"
#include "test-packet.h"
#include "test-app.h"

abts_suite *test_mo_idle(abts_suite *suite);
abts_suite *test_mt_idle(abts_suite *suite);
abts_suite *test_mo_active(abts_suite *suite);
abts_suite *test_mt_active(abts_suite *suite);
abts_suite *test_mo_sms(abts_suite *suite);
abts_suite *test_mt_sms(abts_suite *suite);
abts_suite *test_crash(abts_suite *suite);

const struct testlist {
    abts_suite *(*func)(abts_suite *suite);
} alltests[] = {
    {test_mo_idle},
    {test_mt_idle},
    {test_mo_active},
    {test_mt_active},
    {test_mo_sms},
    {test_mt_sms},
    {test_crash},
    {NULL},
};

static ogs_thread_t *pcrf_thread = NULL;
static ogs_thread_t *pgw_thread = NULL;
static ogs_thread_t *sgw_thread = NULL;
static ogs_thread_t *hss_thread = NULL;
ogs_socknode_t *sgsap = NULL;

static void terminate(void)
{
    ogs_msleep(50);

    epc_child_terminate();

    app_will_terminate();

    ogs_info("MME try to terminate");
    mme_terminate();
    testvlr_sgsap_close(sgsap);
    ogs_sctp_final();
    ogs_info("MME terminate...done");

    if (hss_thread) ogs_thread_destroy(hss_thread);
    if (sgw_thread) ogs_thread_destroy(sgw_thread);
    if (pgw_thread) ogs_thread_destroy(pgw_thread);
    if (pcrf_thread) ogs_thread_destroy(pcrf_thread);

    app_did_terminate();

    base_finalize();
    ogs_core_finalize();
}

static void initialize(char **argv)
{
    int rv;

    ogs_core_initialize();
    base_initialize();

    rv = app_will_initialize(argv);
    ogs_assert(rv == OGS_OK);

    pcrf_thread = epc_child_create("nextepc-pcrfd", argv);
    pgw_thread = epc_child_create("nextepc-pgwd", argv);
    sgw_thread = epc_child_create("nextepc-sgwd", argv);
    hss_thread = epc_child_create("nextepc-hssd", argv);

    ogs_info("MME try to initialize");
    ogs_sctp_init(context_self()->config.usrsctp.udp_port);
    sgsap = testvlr_sgsap_server("127.0.0.2");
    ogs_assert(sgsap);

    rv = mme_initialize();
    ogs_assert(rv == OGS_OK);
    ogs_info("MME initialize...done");

    rv = app_did_initialize();
    ogs_assert(rv == OGS_OK);
}

int main(int argc, char **argv)
{
    int i;
    abts_suite *suite = NULL;

    atexit(terminate);
    test_main(argc, argv, "sample-csfb.conf", initialize);

    for (i = 0; alltests[i].func; i++)
        suite = alltests[i].func(suite);

    return abts_report(suite);
}
