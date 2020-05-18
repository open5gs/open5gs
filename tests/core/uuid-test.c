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

static void uuid_test1(abts_case *tc, void *data)
{
    ogs_uuid_t uuid;
    ogs_uuid_t uuid2;
    char buf[OGS_UUID_FORMATTED_LENGTH + 1];

    ogs_uuid_get(&uuid);
    ogs_uuid_format(buf, &uuid);

    ogs_uuid_parse(&uuid2, buf);
    ABTS_ASSERT(tc, "parse produced a different UUID",
             memcmp(&uuid, &uuid2, sizeof(uuid)) == 0);
}

static void uuid_test2(abts_case *tc, void *data)
{
    ogs_uuid_t uuid;
    ogs_uuid_t uuid2;

    /* generate two of them quickly */
    ogs_uuid_get(&uuid);
    ogs_uuid_get(&uuid2);

    ABTS_ASSERT(tc, "generated the same UUID twice",
             memcmp(&uuid, &uuid2, sizeof(uuid)) != 0);
}

abts_suite *test_uuid(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, uuid_test1, NULL);
    abts_run_test(suite, uuid_test2, NULL);

    return suite;
}
