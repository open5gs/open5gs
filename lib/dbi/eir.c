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

#include "ogs-dbi.h"

static bool value_is_all_digits(const char *value)
{
    size_t i;

    if (!value || !value[0])
        return false;

    for (i = 0; value[i]; i++)
        if (!isdigit((unsigned char)value[i]))
            return false;

    return true;
}

/* IMEI: TAC(8) + serial(6) + check digit(1) = 15 digits. */
#define OGS_EIR_IMEI_DIGITS     15
/* IMEISV: TAC(8) + serial(6) + software version(2) = 16 digits. */
#define OGS_EIR_IMEISV_DIGITS   16
/* IMSI: MCC(3) + MNC(2-3) + MSIN, up to 15 digits total. */
#define OGS_EIR_IMSI_MIN_DIGITS 6
#define OGS_EIR_IMSI_MAX_DIGITS 15

bool ogs_dbi_eir_pei_is_valid(const char *pei)
{
    char *type = NULL, *value = NULL;
    bool rc = false;

    if (!pei || !ogs_id_get_type_value(pei, &type, &value))
        goto cleanup;

    if (!value_is_all_digits(value))
        goto cleanup;

    if (!strcmp(type, "imei"))
        rc = (strlen(value) == OGS_EIR_IMEI_DIGITS);
    else if (!strcmp(type, "imeisv"))
        rc = (strlen(value) == OGS_EIR_IMEISV_DIGITS);

cleanup:
    ogs_free(type);
    ogs_free(value);
    return rc;
}

bool ogs_dbi_eir_supi_is_valid(const char *supi)
{
    char *type = NULL, *value = NULL;
    bool rc = false;

    if (!supi || !ogs_id_get_type_value(supi, &type, &value))
        goto cleanup;

    if (!value_is_all_digits(value))
        goto cleanup;

    if (!strcmp(type, "imsi"))
        rc = (strlen(value) >= OGS_EIR_IMSI_MIN_DIGITS &&
              strlen(value) <= OGS_EIR_IMSI_MAX_DIGITS);

cleanup:
    ogs_free(type);
    ogs_free(value);
    return rc;
}

static ogs_dbi_eir_status_t eir_status_from_string(const char *status)
{
    if (!status)
        return OGS_DBI_EIR_STATUS_UNKNOWN;
    if (!strcmp(status, "WHITELISTED"))
        return OGS_DBI_EIR_STATUS_WHITELISTED;
    if (!strcmp(status, "BLACKLISTED"))
        return OGS_DBI_EIR_STATUS_BLACKLISTED;
    if (!strcmp(status, "GREYLISTED"))
        return OGS_DBI_EIR_STATUS_GREYLISTED;

    return OGS_DBI_EIR_STATUS_UNKNOWN;
}

static int eir_find_one(bson_t *query, ogs_dbi_eir_record_t *record)
{
    int rv;
    mongoc_cursor_t *cursor = NULL;
    bson_error_t error;
    const bson_t *document = NULL;
    bson_iter_t iter;
    const char *utf8 = NULL;
    uint32_t length = 0;

#if MONGOC_CHECK_VERSION(1, 5, 0)
    cursor = mongoc_collection_find_with_opts(
            ogs_mongoc()->collection.eir, query, NULL, NULL);
#else
    cursor = mongoc_collection_find(ogs_mongoc()->collection.eir,
            MONGOC_QUERY_NONE, 0, 0, 0, query, NULL, NULL);
#endif

    if (!mongoc_cursor_next(cursor, &document)) {
        if (mongoc_cursor_error(cursor, &error)) {
            ogs_error("Cursor Failure: %s", error.message);
            rv = OGS_ERROR;
        } else {
            rv = OGS_NOTFOUND;
        }
        goto out;
    }

    memset(record, 0, sizeof(*record));

    if (!bson_iter_init(&iter, document)) {
        ogs_error("bson_iter_init() failed in EIR document");
        rv = OGS_ERROR;
        goto out;
    }

    while (bson_iter_next(&iter)) {
        const char *key = bson_iter_key(&iter);

        if (!strcmp(key, OGS_EIR_PEI_STRING) &&
                BSON_ITER_HOLDS_UTF8(&iter)) {
            utf8 = bson_iter_utf8(&iter, &length);
            record->pei = ogs_strndup(utf8, length);
        } else if (!strcmp(key, OGS_EIR_SUPI_STRING) &&
                BSON_ITER_HOLDS_UTF8(&iter)) {
            utf8 = bson_iter_utf8(&iter, &length);
            record->supi = ogs_strndup(utf8, length);
        } else if (!strcmp(key, OGS_EIR_STATUS_STRING) &&
                BSON_ITER_HOLDS_UTF8(&iter)) {
            utf8 = bson_iter_utf8(&iter, &length);
            record->status = eir_status_from_string(utf8);
        }
    }

    if (!record->pei || record->status == OGS_DBI_EIR_STATUS_UNKNOWN) {
        ogs_error("Invalid EIR record [pei:%s,status:%d]",
                record->pei ? record->pei : "NULL", record->status);
        ogs_dbi_eir_record_free(record);
        rv = OGS_ERROR;
        goto out;
    }

    if (mongoc_cursor_next(cursor, &document)) {
        ogs_error("Ambiguous EIR records for the same query");
        ogs_dbi_eir_record_free(record);
        rv = OGS_ERROR;
        goto out;
    }

    if (mongoc_cursor_error(cursor, &error)) {
        ogs_error("Cursor Failure: %s", error.message);
        ogs_dbi_eir_record_free(record);
        rv = OGS_ERROR;
        goto out;
    }

    rv = OGS_OK;

out:
    if (cursor)
        mongoc_cursor_destroy(cursor);

    return rv;
}

int ogs_dbi_eir_check_equipment(
        const char *supi, const char *pei, ogs_dbi_eir_record_t *record)
{
    int rv;
    bson_t *query;

    ogs_assert(pei);
    ogs_assert(record);

    if (supi) {
        query = BCON_NEW(
                OGS_EIR_PEI_STRING, BCON_UTF8(pei),
                OGS_EIR_SUPI_STRING, BCON_UTF8(supi));
        rv = eir_find_one(query, record);
        bson_destroy(query);

        if (rv != OGS_NOTFOUND)
            return rv;
    }

    /*
     * Matches both a missing `supi` field and an explicit `supi: null`,
     * per MongoDB equality-match semantics; kept consistent with the
     * eir_generic_unique partial index in ogs-mongoc.c, which cannot use
     * $exists: false (unsupported in partialFilterExpression).
     */
    query = BCON_NEW(
            OGS_EIR_PEI_STRING, BCON_UTF8(pei),
            OGS_EIR_SUPI_STRING, "{", "$eq", BCON_NULL, "}");
    rv = eir_find_one(query, record);
    bson_destroy(query);

    return rv;
}

void ogs_dbi_eir_record_free(ogs_dbi_eir_record_t *record)
{
    ogs_assert(record);

    if (record->pei)
        ogs_free(record->pei);
    if (record->supi)
        ogs_free(record->supi);

    memset(record, 0, sizeof(*record));
}
