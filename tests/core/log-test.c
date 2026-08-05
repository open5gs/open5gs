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

static void test_format(abts_case *tc, void *data)
{
    ogs_log_format_e saved = ogs_log_get_format();
    int core_domain;

    core_domain = ogs_log_get_domain_id("core");
    ABTS_INT_EQUAL(tc, OGS_LOG_FORMAT_TEXT, ogs_log_get_format());

    ABTS_INT_EQUAL(tc, OGS_LOG_FORMAT_TEXT,
            ogs_log_format_from_string(NULL));
    ABTS_INT_EQUAL(tc, OGS_LOG_FORMAT_TEXT,
            ogs_log_format_from_string("text"));
    ABTS_INT_EQUAL(tc, OGS_LOG_FORMAT_TEXT,
            ogs_log_format_from_string("TEXT"));
    ABTS_INT_EQUAL(tc, OGS_LOG_FORMAT_TEXT,
            ogs_log_format_from_string("garbage"));
    ABTS_INT_EQUAL(tc, OGS_LOG_FORMAT_JSON,
            ogs_log_format_from_string("json"));
    ABTS_INT_EQUAL(tc, OGS_LOG_FORMAT_JSON,
            ogs_log_format_from_string("JSON"));

    ogs_log_set_format(OGS_LOG_FORMAT_JSON);
    ABTS_INT_EQUAL(tc, OGS_LOG_FORMAT_JSON, ogs_log_get_format());
    ogs_log_set_format(OGS_LOG_FORMAT_TEXT);
    ABTS_INT_EQUAL(tc, OGS_LOG_FORMAT_TEXT, ogs_log_get_format());

    (void)core_domain;
    ogs_log_set_format(saved);
}

static void test_json_escape(abts_case *tc, void *data)
{
    char buf[1024];
    char *end;
    int core_domain;
    const char *needle;

    core_domain = ogs_log_get_domain_id("core");

    end = ogs_log_render_json(buf, sizeof buf, OGS_LOG_INFO, core_domain,
            0, "src/core/init.c", 42, "amf_initialize",
            "hello \"world\" \\ \b\f\n\r\t end");
    ABTS_PTR_NOTNULL(tc, end);

    ABTS_PTR_NOTNULL(tc, strstr(buf, "\"file\":\"src/core/init.c\""));
    ABTS_PTR_NOTNULL(tc, strstr(buf, "\"line\":42"));
    ABTS_PTR_NOTNULL(tc, strstr(buf, "\"func\":\"amf_initialize\""));
    ABTS_PTR_NOTNULL(tc, strstr(buf, "\"level\":\"INFO\""));
    ABTS_PTR_NOTNULL(tc, strstr(buf, "\"domain\":\"core\""));

    needle = strstr(buf, "\"message\":\"");
    ABTS_PTR_NOTNULL(tc, needle);
    needle += strlen("\"message\":\"");
    ABTS_PTR_NOTNULL(tc, strstr(needle, "hello \\\"world\\\" \\\\ \\b\\f\\n\\r\\t end"));

    end = ogs_log_render_json(buf, sizeof buf, OGS_LOG_INFO, core_domain,
            0, NULL, 0, NULL, "no metadata");
    ABTS_PTR_NOTNULL(tc, end);
    ABTS_PTR_EQUAL(tc, NULL, strstr(buf, "\"file\""));
    ABTS_PTR_EQUAL(tc, NULL, strstr(buf, "\"line\""));
    ABTS_PTR_EQUAL(tc, NULL, strstr(buf, "\"func\""));
    ABTS_PTR_NOTNULL(tc, strstr(buf, "\"message\":\"no metadata\""));

    end = ogs_log_render_json(buf, sizeof buf, OGS_LOG_INFO, core_domain,
            OGS_ECONNREFUSED, "f.c", 1, "f", "boom");
    ABTS_PTR_NOTNULL(tc, end);
    ABTS_PTR_NOTNULL(tc, strstr(buf, "\"err\":"));
    ABTS_PTR_NOTNULL(tc, strstr(buf, "\"errstr\""));
}

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
    abts_run_test(suite, test_format, NULL);
    abts_run_test(suite, test_json_escape, NULL);

    return suite;
}
