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

#include "ogs-dbi.h"

int ogs_dbi_msisdn_data(
        char *imsi_or_msisdn_bcd, ogs_msisdn_data_t *msisdn_data)
{
    int rv = OGS_OK;
    mongoc_cursor_t *cursor = NULL;
    bson_t *query = NULL;
    bson_error_t error;
    const bson_t *document;
    bson_iter_t iter;
    bson_iter_t child1_iter;
    const char *utf8 = NULL;
    uint32_t length = 0;

    ogs_assert(msisdn_data);
    ogs_assert(imsi_or_msisdn_bcd);

    memset(msisdn_data, 0, sizeof(*msisdn_data));

    query = BCON_NEW("$or",
            "[",
                "{", "imsi", BCON_UTF8(imsi_or_msisdn_bcd), "}",
                "{", "msisdn", BCON_UTF8(imsi_or_msisdn_bcd), "}",
            "]");
#if MONGOC_CHECK_VERSION(1, 5, 0)
    cursor = mongoc_collection_find_with_opts(
            ogs_mongoc()->collection.subscriber, query, NULL, NULL);
#else
    cursor = mongoc_collection_find(ogs_mongoc()->collection.subscriber,
            MONGOC_QUERY_NONE, 0, 0, 0, query, NULL, NULL);
#endif

    if (!mongoc_cursor_next(cursor, &document)) {
        ogs_error("[%s] Cannot find IMSI or MSISDN in DB", imsi_or_msisdn_bcd);

        rv = OGS_ERROR;
        goto out;
    }

    if (mongoc_cursor_error(cursor, &error)) {
        ogs_error("Cursor Failure: %s", error.message);

        rv = OGS_ERROR;
        goto out;
    }

    if (!bson_iter_init(&iter, document)) {
        ogs_error("bson_iter_init failed in this document");

        rv = OGS_ERROR;
        goto out;
    }

    while (bson_iter_next(&iter)) {
        const char *key = bson_iter_key(&iter);
        if (!strcmp(key, "imsi") &&
            BSON_ITER_HOLDS_UTF8(&iter)) {
            utf8 = bson_iter_utf8(&iter, &length);
            ogs_cpystrn(msisdn_data->imsi.bcd,
                    utf8, ogs_min(length, OGS_MAX_IMSI_BCD_LEN)+1);
            ogs_bcd_to_buffer(
                    msisdn_data->imsi.bcd,
                    msisdn_data->imsi.buf, &msisdn_data->imsi.len);
        } else if (!strcmp(key, "msisdn") &&
            BSON_ITER_HOLDS_ARRAY(&iter)) {
            int msisdn_index = 0;

            bson_iter_recurse(&iter, &child1_iter);
            while (bson_iter_next(&child1_iter)) {
                ogs_assert(msisdn_index < OGS_MAX_NUM_OF_MSISDN);

                if (BSON_ITER_HOLDS_UTF8(&child1_iter)) {
                    utf8 = bson_iter_utf8(&child1_iter, &length);
                    ogs_cpystrn(msisdn_data->msisdn[msisdn_index].bcd,
                            utf8, ogs_min(length, OGS_MAX_MSISDN_BCD_LEN)+1);
                    ogs_bcd_to_buffer(
                            msisdn_data->msisdn[msisdn_index].bcd,
                            msisdn_data->msisdn[msisdn_index].buf,
                            &msisdn_data->msisdn[msisdn_index].len);

                    msisdn_index++;
                }
            }
            msisdn_data->num_of_msisdn = msisdn_index;
        }
    }

out:
    if (query) bson_destroy(query);
    if (cursor) mongoc_cursor_destroy(cursor);

    return rv;
}

int ogs_dbi_ims_data(char *supi, ogs_ims_data_t *ims_data)
{
    int rv = OGS_OK;
    mongoc_cursor_t *cursor = NULL;
    bson_t *query = NULL;
    bson_error_t error;
    const bson_t *document;
    bson_iter_t iter;
    bson_iter_t child1_iter;
    const char *utf8 = NULL;
    uint32_t length = 0;

    char *supi_type = NULL;
    char *supi_id = NULL;

    ogs_assert(ims_data);
    ogs_assert(supi);

    memset(ims_data, 0, sizeof(*ims_data));

    supi_type = ogs_id_get_type(supi);
    ogs_assert(supi_type);
    supi_id = ogs_id_get_value(supi);
    ogs_assert(supi_id);

    query = BCON_NEW(supi_type, BCON_UTF8(supi_id));
#if MONGOC_CHECK_VERSION(1, 5, 0)
    cursor = mongoc_collection_find_with_opts(
            ogs_mongoc()->collection.subscriber, query, NULL, NULL);
#else
    cursor = mongoc_collection_find(ogs_mongoc()->collection.subscriber,
            MONGOC_QUERY_NONE, 0, 0, 0, query, NULL, NULL);
#endif

    if (!mongoc_cursor_next(cursor, &document)) {
        ogs_error("[%s] Cannot find IMSI in DB", supi);

        rv = OGS_ERROR;
        goto out;
    }

    if (mongoc_cursor_error(cursor, &error)) {
        ogs_error("Cursor Failure: %s", error.message);

        rv = OGS_ERROR;
        goto out;
    }

    if (!bson_iter_init(&iter, document)) {
        ogs_error("bson_iter_init failed in this document");

        rv = OGS_ERROR;
        goto out;
    }

    while (bson_iter_next(&iter)) {
        const char *key = bson_iter_key(&iter);
        if (!strcmp(key, "msisdn") &&
            BSON_ITER_HOLDS_ARRAY(&iter)) {
            int msisdn_index = 0;

            bson_iter_recurse(&iter, &child1_iter);
            while (bson_iter_next(&child1_iter)) {
                ogs_assert(msisdn_index < OGS_MAX_NUM_OF_MSISDN);

                if (BSON_ITER_HOLDS_UTF8(&child1_iter)) {
                    utf8 = bson_iter_utf8(&child1_iter, &length);
                    ogs_cpystrn(ims_data->msisdn[msisdn_index].bcd,
                            utf8, ogs_min(length, OGS_MAX_MSISDN_BCD_LEN)+1);
                    ogs_bcd_to_buffer(
                            ims_data->msisdn[msisdn_index].bcd,
                            ims_data->msisdn[msisdn_index].buf,
                            &ims_data->msisdn[msisdn_index].len);

                    msisdn_index++;
                }
            }
            ims_data->num_of_msisdn = msisdn_index;
        }
    }

out:
    if (query) bson_destroy(query);
    if (cursor) mongoc_cursor_destroy(cursor);

    ogs_free(supi_type);
    ogs_free(supi_id);

    return rv;
}
