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

#include "test-common.h"
#include "test-fd-path.h"

static ogs_diam_config_t diam_config;

static void test_diam_config(void)
{
    memset(&diam_config, 0, sizeof(ogs_diam_config_t));

    diam_config.cnf_diamid = "ims.localdomain";
    diam_config.cnf_diamrlm = "localdomain";
    diam_config.cnf_port = DIAMETER_PORT;
    diam_config.cnf_port_tls = DIAMETER_SECURE_PORT;
    diam_config.cnf_flags.no_sctp = 1;
    diam_config.cnf_flags.no_fwd = 1;
    diam_config.cnf_addr = "127.0.0.1";

    diam_config.ext[diam_config.num_of_ext].module =
        FD_EXT_DIR OGS_DIR_SEPARATOR_S "dbg_msg_dumps.fdx";
    diam_config.ext[diam_config.num_of_ext].conf = "0x8888";
    diam_config.num_of_ext++;
    diam_config.ext[diam_config.num_of_ext].module =
        FD_EXT_DIR OGS_DIR_SEPARATOR_S "dict_rfc5777.fdx";
    diam_config.num_of_ext++;
    diam_config.ext[diam_config.num_of_ext].module =
        FD_EXT_DIR OGS_DIR_SEPARATOR_S "dict_mip6i.fdx";
    diam_config.num_of_ext++;
    diam_config.ext[diam_config.num_of_ext].module =
        FD_EXT_DIR OGS_DIR_SEPARATOR_S "dict_nasreq.fdx";
    diam_config.num_of_ext++;
    diam_config.ext[diam_config.num_of_ext].module =
        FD_EXT_DIR OGS_DIR_SEPARATOR_S "dict_nas_mipv6.fdx";
    diam_config.num_of_ext++;
    diam_config.ext[diam_config.num_of_ext].module =
        FD_EXT_DIR OGS_DIR_SEPARATOR_S "dict_dcca.fdx";
    diam_config.num_of_ext++;
    diam_config.ext[diam_config.num_of_ext].module =
        FD_EXT_DIR OGS_DIR_SEPARATOR_S "dict_dcca_3gpp" \
        OGS_DIR_SEPARATOR_S "dict_dcca_3gpp.fdx";
    diam_config.num_of_ext++;

    diam_config.conn[diam_config.num_of_conn].identity = TEST_HSS_IDENTITY;
    diam_config.conn[diam_config.num_of_conn].addr = "127.0.0.8";
    diam_config.num_of_conn++;
    diam_config.conn[diam_config.num_of_conn].identity = TEST_PCRF_IDENTITY;
    diam_config.conn[diam_config.num_of_conn].addr = "127.0.0.9";
    diam_config.num_of_conn++;
}

int test_fd_init(void)
{
    int ret;
    struct disp_when data;

    test_diam_config();

    ret = ogs_diam_init(FD_MODE_CLIENT, NULL, &diam_config);
    ogs_assert(ret == 0);

    test_rx_init();
    test_cx_init();

    ret = ogs_diam_start();
    ogs_assert(ret == 0);

    return 0;
}

void test_fd_final(void)
{
    test_rx_final();
    test_cx_final();

    ogs_diam_final();
}
