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

static void test_basic(abts_case *tc, void *data)
{
    int domain_id = -1;
    int core_level = ogs_log_get_domain_level(ogs_log_get_domain_id("core"));

    ogs_log_set_domain_level(ogs_log_get_domain_id("core"), OGS_LOG_ERROR);
    ogs_log_install_domain(&domain_id, "core", OGS_LOG_INFO);
    ABTS_INT_EQUAL(tc, 1, domain_id);
    ABTS_INT_EQUAL(tc, OGS_LOG_INFO, ogs_log_get_domain_level(domain_id));
    ogs_log_set_domain_level(domain_id, OGS_LOG_DEBUG);
    ABTS_INT_EQUAL(tc, OGS_LOG_DEBUG, ogs_log_get_domain_level(domain_id));
    ogs_log_install_domain(&domain_id, "CORE1", OGS_LOG_INFO);
    ABTS_INT_EQUAL(tc, OGS_LOG_INFO, ogs_log_get_domain_level(domain_id));
    ogs_log_install_domain(&domain_id, "COR2", OGS_LOG_WARN);
    ogs_log_set_domain_level(domain_id, OGS_LOG_ERROR);
    ABTS_INT_EQUAL(tc, OGS_LOG_ERROR, ogs_log_get_domain_level(domain_id));
    ogs_log_set_domain_level(ogs_log_get_domain_id("core"), OGS_LOG_ERROR);
    ogs_log_install_domain(&domain_id, "CORE1", OGS_LOG_WARN);
    ABTS_INT_EQUAL(tc, OGS_LOG_WARN, ogs_log_get_domain_level(domain_id));
    ogs_log_install_domain(&domain_id, "COR2", OGS_LOG_INFO);
    ABTS_INT_EQUAL(tc, OGS_LOG_INFO, ogs_log_get_domain_level(domain_id));
    ogs_log_install_domain(&domain_id, "COR3", OGS_LOG_INFO);
    ogs_log_set_domain_level(domain_id, OGS_LOG_NONE);
    ABTS_INT_EQUAL(tc, OGS_LOG_NONE, ogs_log_get_domain_level(domain_id));
    ogs_log_install_domain(&domain_id, "core", OGS_LOG_FULL);
    ABTS_INT_EQUAL(tc, 1, domain_id);
    ABTS_INT_EQUAL(tc, OGS_LOG_FULL, ogs_log_get_domain_level(domain_id));
    domain_id = ogs_log_get_domain_id("core");
    ABTS_INT_EQUAL(tc, 1, domain_id);
    ABTS_INT_EQUAL(tc, OGS_LOG_FULL, ogs_log_get_domain_level(domain_id));
    ogs_log_set_mask_level("COR2  COR3", OGS_LOG_ERROR);

    ABTS_INT_EQUAL(tc, OGS_LOG_ERROR,
            ogs_log_get_domain_level(ogs_log_get_domain_id("COR2")));
    ABTS_INT_EQUAL(tc, OGS_LOG_ERROR,
            ogs_log_get_domain_level(ogs_log_get_domain_id("COR3")));
    ABTS_INT_EQUAL(tc, OGS_LOG_FULL,
            ogs_log_get_domain_level(ogs_log_get_domain_id("core")));
    ABTS_INT_EQUAL(tc, OGS_LOG_WARN,
            ogs_log_get_domain_level(ogs_log_get_domain_id("CORE1")));
    
    ogs_log_set_mask_level("COR2:COR3:COREE:core:CORE1", ogs_core()->log.level);
    ABTS_INT_EQUAL(tc, ogs_core()->log.level,
            ogs_log_get_domain_level(ogs_log_get_domain_id("COR2")));
    ABTS_INT_EQUAL(tc, ogs_core()->log.level,
            ogs_log_get_domain_level(ogs_log_get_domain_id("COR3")));
    ABTS_INT_EQUAL(tc, ogs_core()->log.level,
            ogs_log_get_domain_level(ogs_log_get_domain_id("core")));
    ABTS_INT_EQUAL(tc, ogs_core()->log.level,
            ogs_log_get_domain_level(ogs_log_get_domain_id("CORE1")));

    ogs_log_set_domain_level(ogs_log_get_domain_id("core"), core_level);

#if 0
    ogs_error("alksdfjklsadf %d, %s", 555, "bbbbbb");
    ogs_error("sdofsadljflkasdjflksadfjlksadfjlkasdkjfasd\n");
    ogs_msleep(30);
    ogs_warn("555555 %d, %s", 3333, "bbbbbb");
    ogs_fatal("ddd");
    ogs_log_print(OGS_LOG_INFO, "219380912839012830192830912380192390\n");
    ogs_usleep(3000);
    ogs_info("29380912839012830192830912380192390");
    ogs_fatal("eee");

    ogs_log_hexdump(OGS_LOG_WARN, "\x12\x34\x56\x78\x12\x34\x56"
                "\x12\x34\x56\x78\x12\x34\x56"
                "\x12\x34\x56\x78\x12\x34\x56"
                "\x12\x34\x56\x78\x12\x34\x56"
                "\x12\x34\x56\x78\x12\x34\x56"
                "\x12\x34\x56\x78\x12\x34\x56"
                "\x12\x34\x56\x78\x12\x34\x56", 49);

    ogs_log_hexdump(OGS_LOG_INFO, "\x12\x34\x56\x78\x12\x34\x56"
                "\x12\x34\x56\x78\x12\x34\x56"
                "\x12\x34\x56\x78\x12\x34\x56"
                "\x12\x34\x56\x78\x12\x34\x56"
                "\x12\x34\x56\x78", 32);
    ogs_log_message(OGS_LOG_FATAL, OGS_ECONNREFUSED, "Error occurred");
    ogs_log_message(OGS_LOG_WARN, OGS_EAGAIN, "Error occurred");
    ogs_info("timezone : %d", (int)ogs_timezone());
#endif
}

abts_suite *test_log(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, test_basic, NULL);

    return suite;
}
