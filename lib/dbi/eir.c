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

#define EIR_SETUP_MAX_TIME_MS 5000

static int eir_collection_info(bool *exists, bool *validation)
{
    int rv = OGS_OK;
    bson_t *opts;
    const bson_t *document;
    bson_iter_t iter, options;
    bson_error_t error;
    mongoc_cursor_t *cursor;

    *exists = false;
    *validation = false;
    opts = BCON_NEW("filter", "{", "name", BCON_UTF8("eir"), "}",
            "maxTimeMS", BCON_INT32(EIR_SETUP_MAX_TIME_MS));
    cursor = mongoc_database_find_collections_with_opts(
            ogs_mongoc()->database, opts);
    bson_destroy(opts);

    while (mongoc_cursor_next(cursor, &document)) {
        if (*exists) {
            ogs_error("Multiple EIR collection metadata records returned");
            rv = OGS_ERROR;
            break;
        }
        if (!bson_iter_init_find(&iter, document, "name") ||
                !BSON_ITER_HOLDS_UTF8(&iter) ||
                strcmp(bson_iter_utf8(&iter, NULL), "eir")) {
            ogs_error("Missing or unexpected EIR collection metadata name");
            rv = OGS_ERROR;
            break;
        }
        if (!bson_iter_init_find(&iter, document, "type") ||
                !BSON_ITER_HOLDS_UTF8(&iter) ||
                strcmp(bson_iter_utf8(&iter, NULL), "collection")) {
            ogs_error("EIR namespace is not a regular collection");
            rv = OGS_ERROR;
            break;
        }
        if (!bson_iter_init_find(&iter, document, "options") ||
                !BSON_ITER_HOLDS_DOCUMENT(&iter) ||
                !bson_iter_recurse(&iter, &options)) {
            ogs_error("Missing or invalid EIR collection options");
            rv = OGS_ERROR;
            break;
        }

        *exists = true;
        while (bson_iter_next(&options)) {
            const char *key = bson_iter_key(&options);

            /* Preserve explicit empty validators and disabled validation. */
            if (!strcmp(key, "validator") ||
                    !strcmp(key, "validationLevel") ||
                    !strcmp(key, "validationAction"))
                *validation = true;
        }
    }

    if (mongoc_cursor_error(cursor, &error)) {
        ogs_error("Cannot inspect EIR collection: %s", error.message);
        rv = OGS_ERROR;
    }
    mongoc_cursor_destroy(cursor);

    if (rv == OGS_OK)
        ogs_debug("EIR collection metadata [exists:%d,validation:%d]",
                *exists, *validation);

    return rv;
}

static bool eir_set_validator(bool create, bson_error_t *error)
{
    bool ok;
    bson_t *command;

    command = BCON_NEW(create ? "create" : "collMod", BCON_UTF8("eir"),
            "validator", "{", "$jsonSchema", "{",
                "bsonType", BCON_UTF8("object"),
                "required", "[", BCON_UTF8(OGS_EIR_PEI_STRING),
                    BCON_UTF8(OGS_EIR_STATUS_STRING), "]",
                "properties", "{",
                    OGS_EIR_PEI_STRING, "{",
                        "bsonType", BCON_UTF8("string"), "}",
                    OGS_EIR_SUPI_STRING, "{", "bsonType", "[",
                        BCON_UTF8("string"), BCON_UTF8("null"), "]", "}",
                    OGS_EIR_STATUS_STRING, "{", "enum", "[",
                        BCON_UTF8("WHITELISTED"),
                        BCON_UTF8("BLACKLISTED"),
                        BCON_UTF8("GREYLISTED"), "]", "}",
                "}",
            "}", "}",
            "validationLevel", BCON_UTF8("strict"),
            "validationAction", BCON_UTF8("error"),
            "maxTimeMS", BCON_INT32(EIR_SETUP_MAX_TIME_MS));
    ok = mongoc_database_command_simple(
            ogs_mongoc()->database, command, NULL, NULL, error);
    bson_destroy(command);

    if (ok) {
        if (create)
            ogs_info("Created EIR collection with default validator");
        else
            ogs_info("Installed default EIR validator");
    }

    return ok;
}

static bool eir_index_is_unique(const bson_t *document)
{
    bson_iter_t iter, key, collation;
    const char *second_key;

    if (!bson_iter_init_find(&iter, document, "unique") ||
            !BSON_ITER_HOLDS_BOOL(&iter) || !bson_iter_bool(&iter)) {
        ogs_warn("EIR index has no explicit unique option");
        return false;
    }
    if (bson_has_field(document, "partialFilterExpression")) {
        ogs_warn("EIR partial index does not cover all equipment records");
        return false;
    }
    if (!bson_iter_init_find(&iter, document, "key") ||
            !BSON_ITER_HOLDS_DOCUMENT(&iter) ||
            !bson_iter_recurse(&iter, &key)) {
        ogs_error("EIR index has missing or invalid key metadata");
        return false;
    }

    if (!bson_iter_next(&key)) {
        ogs_error("EIR index has no key fields");
        return false;
    }
    if (!strcmp(bson_iter_key(&key), OGS_EIR_PEI_STRING))
        second_key = OGS_EIR_SUPI_STRING;
    else if (!strcmp(bson_iter_key(&key), OGS_EIR_SUPI_STRING))
        second_key = OGS_EIR_PEI_STRING;
    else {
        ogs_error("EIR index does not begin with PEI or SUPI");
        return false;
    }

    if (!BSON_ITER_HOLDS_NUMBER(&key) ||
            (bson_iter_as_int64(&key) != 1 &&
             bson_iter_as_int64(&key) != -1) ||
            !bson_iter_next(&key) ||
            strcmp(bson_iter_key(&key), second_key) ||
            !BSON_ITER_HOLDS_NUMBER(&key) ||
            (bson_iter_as_int64(&key) != 1 &&
             bson_iter_as_int64(&key) != -1) || bson_iter_next(&key)) {
        ogs_error("EIR index does not have exactly the PEI and SUPI keys");
        return false;
    }

    if (bson_iter_init_find(&iter, document, "collation")) {
        if (!BSON_ITER_HOLDS_DOCUMENT(&iter) ||
                !bson_iter_recurse(&iter, &collation) ||
                !bson_iter_find(&collation, "locale") ||
                !BSON_ITER_HOLDS_UTF8(&collation) ||
                strcmp(bson_iter_utf8(&collation, NULL), "simple")) {
            ogs_error("EIR index does not use simple collation");
            return false;
        }
    }

    /*
     * A compound sparse index includes every valid EIR record: at least
     * pei exists, including general records with missing or null supi.
     * Hidden indexes also continue to enforce uniqueness.
     */
    return true;
}

static int eir_find_unique_index(bool *found)
{
    int rv = OGS_OK;
    bson_t *opts;
    const bson_t *document;
    bson_error_t error;
    mongoc_cursor_t *cursor;

    *found = false;
    opts = BCON_NEW("maxTimeMS", BCON_INT32(EIR_SETUP_MAX_TIME_MS));
    cursor = mongoc_collection_find_indexes_with_opts(
            ogs_mongoc()->collection.eir, opts);
    bson_destroy(opts);

    while (mongoc_cursor_next(cursor, &document)) {
        if (eir_index_is_unique(document))
            *found = true;
    }
    if (mongoc_cursor_error(cursor, &error)) {
        ogs_error("Cannot inspect EIR indexes: %s", error.message);
        rv = OGS_ERROR;
    }
    mongoc_cursor_destroy(cursor);

    return rv;
}

static int eir_create_index(void)
{
    bool found, ok;
    bson_t *command;
    bson_error_t error;

    if (eir_find_unique_index(&found) != OGS_OK) {
        ogs_error("eir_find_unique_index() failed");
        return OGS_ERROR;
    }
    if (found) {
        ogs_info("Preserving existing EIR PEI/SUPI unique index");
        return OGS_OK;
    }

    ogs_debug("No suitable EIR unique index; creating pei_supi_unique");

    command = BCON_NEW("createIndexes", BCON_UTF8("eir"),
            "indexes", "[", "{",
                "key", "{", OGS_EIR_PEI_STRING, BCON_INT32(1),
                    OGS_EIR_SUPI_STRING, BCON_INT32(1), "}",
                "name", BCON_UTF8("pei_supi_unique"),
                "unique", BCON_BOOL(true),
                "collation", "{", "locale", BCON_UTF8("simple"), "}",
            "}", "]",
            "maxTimeMS", BCON_INT32(EIR_SETUP_MAX_TIME_MS));
    ok = mongoc_database_command_simple(
            ogs_mongoc()->database, command, NULL, NULL, &error);
    bson_destroy(command);

    if (!ok) {
        /* Another EIR may have created an equivalent index meanwhile. */
        if (eir_find_unique_index(&found) == OGS_OK && found) {
            ogs_info("EIR unique index found after concurrent creation");
            return OGS_OK;
        }
        ogs_error("Cannot create EIR unique index: %s", error.message);
        return OGS_ERROR;
    }

    ogs_info("EIR unique index pei_supi_unique is ready");
    return OGS_OK;
}

int ogs_dbi_eir_init(void)
{
    int rv = OGS_OK;
    bool exists, validation;
    bson_error_t error;

    if (eir_collection_info(&exists, &validation) != OGS_OK) {
        ogs_error("eir_collection_info() failed");
        return OGS_ERROR;
    }

    if (exists) {
        ogs_debug("Preserving existing EIR collection");
        if (validation)
            ogs_info("Preserving existing EIR validation settings");
    }

    if (!exists) {
        /* Create the collection with validation before creating indexes. */
        if (eir_set_validator(true, &error)) {
            exists = true;
            validation = true;
        } else if (eir_collection_info(&exists, &validation) != OGS_OK ||
                !exists) {
            ogs_error("Cannot create EIR collection: %s", error.message);
            return OGS_ERROR;
        } else {
            ogs_info("Preserving concurrently created EIR collection");
            if (validation)
                ogs_info("Preserving existing EIR validation settings");
        }
    }

    if (!validation && !eir_set_validator(false, &error)) {
        /* Recheck a concurrent initializer; do not ignore other failures. */
        if (eir_collection_info(&exists, &validation) != OGS_OK ||
                !exists || !validation) {
            ogs_error("Cannot install EIR validator: %s", error.message);
            rv = OGS_ERROR;
        } else {
            ogs_info("Preserving concurrently installed EIR validation");
        }
    }

    if (eir_create_index() != OGS_OK) {
        ogs_error("eir_create_index() failed");
        rv = OGS_ERROR;
    }

    return rv;
}

static ogs_dbi_eir_status_t eir_status_from_string(const char *status)
{
    if (!status) {
        ogs_error("Missing EIR equipment status string");
        return OGS_DBI_EIR_STATUS_UNKNOWN;
    }
    if (!strcmp(status, "WHITELISTED"))
        return OGS_DBI_EIR_STATUS_WHITELISTED;
    if (!strcmp(status, "BLACKLISTED"))
        return OGS_DBI_EIR_STATUS_BLACKLISTED;
    if (!strcmp(status, "GREYLISTED"))
        return OGS_DBI_EIR_STATUS_GREYLISTED;

    ogs_warn("Unsupported EIR equipment status string");
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
            ogs_error("Cannot query EIR records: %s", error.message);
            rv = OGS_ERROR;
        } else {
            ogs_warn("No EIR record matches the equipment query");
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

        if (!strcmp(key, OGS_EIR_PEI_STRING)) {
            if (BSON_ITER_HOLDS_UTF8(&iter)) {
                utf8 = bson_iter_utf8(&iter, &length);
                record->pei = ogs_strndup(utf8, length);
            } else {
                ogs_warn("Invalid EIR PEI field type [%d]",
                        bson_iter_type(&iter));
            }
        } else if (!strcmp(key, OGS_EIR_SUPI_STRING)) {
            if (BSON_ITER_HOLDS_UTF8(&iter)) {
                utf8 = bson_iter_utf8(&iter, &length);
                record->supi = ogs_strndup(utf8, length);
            } else if (!BSON_ITER_HOLDS_NULL(&iter)) {
                /* MongoDB equality also matches elements of an array. */
                ogs_warn("Invalid EIR SUPI field type [%d]",
                        bson_iter_type(&iter));
                ogs_dbi_eir_record_free(record);
                rv = OGS_ERROR;
                goto out;
            }
        } else if (!strcmp(key, OGS_EIR_STATUS_STRING)) {
            if (BSON_ITER_HOLDS_UTF8(&iter)) {
                utf8 = bson_iter_utf8(&iter, &length);
                record->status = eir_status_from_string(utf8);
            } else {
                ogs_warn("Invalid EIR status field type [%d]",
                        bson_iter_type(&iter));
            }
        }
    }

    if (!record->pei || record->status == OGS_DBI_EIR_STATUS_UNKNOWN) {
        ogs_warn("Invalid EIR record [pei:%s,status:%d]",
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
        ogs_error("Cannot check EIR record uniqueness: %s", error.message);
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

    memset(record, 0, sizeof(*record));

    if (supi) {
        query = BCON_NEW(
                OGS_EIR_PEI_STRING, BCON_UTF8(pei),
                OGS_EIR_SUPI_STRING, BCON_UTF8(supi));
        rv = eir_find_one(query, record);
        bson_destroy(query);

        if (rv != OGS_NOTFOUND) {
            ogs_warn("No EIR record matches the equipment query");
            return rv;
        }

        ogs_debug("No EIR PEI/SUPI record; trying generic equipment record");
    }

    /*
     * Matches both a missing `supi` field and an explicit `supi: null`,
     * per MongoDB equality-match semantics.
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
