/*
 * Copyright (C) 2026 by Erol Yağız Aydın <ygzaydns@gmail.com>
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

#define TEST_EIR_SUPI "imsi-001011234567895"
#define TEST_EIR_OTHER_SUPI "imsi-001019999999999"
#define TEST_EIR_PEI "imeisv-4901542032375186"
#define TEST_EIR_UNKNOWN_PEI "imeisv-9999999999999999"

static void generic_whitelist_and_pair_blacklist_func(
        abts_case *tc, void *data)
{
    int rv;
    ogs_dbi_eir_record_t record;

    rv = test_db_insert_eir(TEST_EIR_PEI, NULL, "WHITELISTED");
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    rv = test_db_insert_eir(TEST_EIR_PEI, TEST_EIR_SUPI, "BLACKLISTED");
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Pair-specific record overrides the generic PEI record */
    rv = ogs_dbi_eir_check_equipment(TEST_EIR_SUPI, TEST_EIR_PEI, &record);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    ABTS_INT_EQUAL(tc, OGS_DBI_EIR_STATUS_BLACKLISTED, record.status);
    ogs_dbi_eir_record_free(&record);

    /* A different SUPI using the same PEI falls back to the generic record */
    rv = ogs_dbi_eir_check_equipment(
            TEST_EIR_OTHER_SUPI, TEST_EIR_PEI, &record);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    ABTS_INT_EQUAL(tc, OGS_DBI_EIR_STATUS_WHITELISTED, record.status);
    ogs_dbi_eir_record_free(&record);

    /* PEI-only lookup (no SUPI given) also resolves to the generic record */
    rv = ogs_dbi_eir_check_equipment(NULL, TEST_EIR_PEI, &record);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    ABTS_INT_EQUAL(tc, OGS_DBI_EIR_STATUS_WHITELISTED, record.status);
    ogs_dbi_eir_record_free(&record);

    rv = test_db_remove_eir(TEST_EIR_PEI, TEST_EIR_SUPI);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = test_db_remove_eir(TEST_EIR_PEI, NULL);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
}

static void pair_whitelist_overrides_generic_blacklist_func(
        abts_case *tc, void *data)
{
    int rv;
    ogs_dbi_eir_record_t record;

    rv = test_db_insert_eir(TEST_EIR_PEI, NULL, "BLACKLISTED");
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    rv = test_db_insert_eir(TEST_EIR_PEI, TEST_EIR_SUPI, "WHITELISTED");
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    rv = ogs_dbi_eir_check_equipment(TEST_EIR_SUPI, TEST_EIR_PEI, &record);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    ABTS_INT_EQUAL(tc, OGS_DBI_EIR_STATUS_WHITELISTED, record.status);
    ogs_dbi_eir_record_free(&record);

    rv = ogs_dbi_eir_check_equipment(
            TEST_EIR_OTHER_SUPI, TEST_EIR_PEI, &record);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    ABTS_INT_EQUAL(tc, OGS_DBI_EIR_STATUS_BLACKLISTED, record.status);
    ogs_dbi_eir_record_free(&record);

    rv = test_db_remove_eir(TEST_EIR_PEI, TEST_EIR_SUPI);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = test_db_remove_eir(TEST_EIR_PEI, NULL);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
}

static void greylisted_func(abts_case *tc, void *data)
{
    int rv;
    ogs_dbi_eir_record_t record;

    rv = test_db_insert_eir(TEST_EIR_PEI, NULL, "GREYLISTED");
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    rv = ogs_dbi_eir_check_equipment(TEST_EIR_SUPI, TEST_EIR_PEI, &record);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    ABTS_INT_EQUAL(tc, OGS_DBI_EIR_STATUS_GREYLISTED, record.status);
    ogs_dbi_eir_record_free(&record);

    rv = test_db_remove_eir(TEST_EIR_PEI, NULL);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
}

static void unknown_equipment_func(abts_case *tc, void *data)
{
    int rv;
    ogs_dbi_eir_record_t record;

    rv = ogs_dbi_eir_check_equipment(
            TEST_EIR_SUPI, TEST_EIR_UNKNOWN_PEI, &record);
    ABTS_INT_EQUAL(tc, OGS_NOTFOUND, rv);
}

static void invalid_status_func(abts_case *tc, void *data)
{
    int rv;
    ogs_dbi_eir_record_t record;

    rv = test_db_insert_eir(TEST_EIR_PEI, NULL, "BOGUS_STATUS");
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* A malformed record must never be treated as an implicit whitelist */
    rv = ogs_dbi_eir_check_equipment(TEST_EIR_SUPI, TEST_EIR_PEI, &record);
    ABTS_INT_EQUAL(tc, OGS_ERROR, rv);

    rv = test_db_remove_eir(TEST_EIR_PEI, NULL);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
}

static void ambiguous_duplicate_func(abts_case *tc, void *data)
{
    int rv;
    ogs_dbi_eir_record_t record;

    rv = test_db_insert_eir(TEST_EIR_PEI, NULL, "WHITELISTED");
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = test_db_insert_eir(TEST_EIR_PEI, NULL, "BLACKLISTED");
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Two generic records for the same PEI must not be silently resolved */
    rv = ogs_dbi_eir_check_equipment(TEST_EIR_SUPI, TEST_EIR_PEI, &record);
    ABTS_INT_EQUAL(tc, OGS_ERROR, rv);

    rv = test_db_remove_eir(TEST_EIR_PEI, NULL);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
}

abts_suite *test_eir(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, generic_whitelist_and_pair_blacklist_func, NULL);
    abts_run_test(suite,
            pair_whitelist_overrides_generic_blacklist_func, NULL);
    abts_run_test(suite, greylisted_func, NULL);
    abts_run_test(suite, unknown_equipment_func, NULL);
    abts_run_test(suite, invalid_status_func, NULL);
    abts_run_test(suite, ambiguous_duplicate_func, NULL);

    return suite;
}
