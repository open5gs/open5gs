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

#include "mme/ogs-sctp.h"

#include "fd/fd-lib.h"

#include "app/application.h"
#include "app/context.h"

#include "app-init.h"

#include "test-packet.h"
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

    epc_child_terminate();

    app_will_terminate();

    if (mme_thread) ogs_thread_destroy(mme_thread);
    if (hss_thread) ogs_thread_destroy(hss_thread);
    if (sgw_thread) ogs_thread_destroy(sgw_thread);
    if (pgw_thread) ogs_thread_destroy(pgw_thread);
    if (pcrf_thread) ogs_thread_destroy(pcrf_thread);

    app_did_terminate();

    pcscf_fd_final();
    ogs_sctp_final();

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
    mme_thread = epc_child_create("nextepc-mmed", argv);

    ogs_sctp_init(context_self()->config.usrsctp.udp_port);

    rv = pcscf_fd_init();
    ogs_assert(rv == OGS_OK);

    rv = app_did_initialize();
    ogs_assert(rv == OGS_OK);
}

int main(int argc, char **argv)
{
    int i;
    abts_suite *suite = NULL;

    atexit(terminate);
    test_main(argc, argv, "sample-volte.conf", initialize);

    for (i = 0; alltests[i].func; i++)
        suite = alltests[i].func(suite);

    return abts_report(suite);
}
