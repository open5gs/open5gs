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
    bson_iter_t child1_iter, child2_iter, child3_iter, child4_iter, child5_iter;
    bson_iter_t child6_iter, child7_iter, child8_iter, child9_iter;
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
        } else if (!strcmp(key, "ifc") &&
            BSON_ITER_HOLDS_ARRAY(&iter)) {
            int ifc_index = 0;
            bson_iter_recurse(&iter, &child2_iter);
            while (bson_iter_next(&child2_iter)) {
                ogs_assert(ifc_index < OGS_MAX_NUM_OF_IFC);
                bson_iter_recurse(&child2_iter, &child3_iter);
                while (bson_iter_next(&child3_iter)) {
                    const char *child3_key = bson_iter_key(&child3_iter);
                    if (!strcmp(child3_key, "priority") &&
                        BSON_ITER_HOLDS_INT32(&child3_iter)) {
                        ims_data->ifc[ifc_index].priority =
                            bson_iter_int32(&child3_iter);
                    } else if (!strcmp(child3_key, "application_server") &&
                        BSON_ITER_HOLDS_DOCUMENT(&child3_iter)) {
                        bson_iter_recurse(&child3_iter, &child4_iter);
                        while (bson_iter_next(&child4_iter)) {
                            const char *child4_key =
                                bson_iter_key(&child4_iter);
                            if (!strcmp(child4_key, "server_name") &&
                                BSON_ITER_HOLDS_UTF8(&child4_iter)) {
                                utf8 = bson_iter_utf8(&child4_iter, &length);
                                ims_data->ifc[ifc_index]
                                    .application_server.server_name =
                                    ogs_strndup(utf8, length);
                            } else if (!strcmp(child4_key, "default_handling")
                                    && BSON_ITER_HOLDS_INT32(&child4_iter)) {
                                ims_data->ifc[ifc_index]
                                    .application_server.default_handling =
                                    bson_iter_int32(&child4_iter);
                            }
                        }
                    } else if (!strcmp(child3_key, "trigger_point") &&
                               BSON_ITER_HOLDS_DOCUMENT(&child3_iter)) {
                        bson_iter_recurse(&child3_iter, &child5_iter);
                        while (bson_iter_next(&child5_iter)) {
                            const char *child5_key =
                                bson_iter_key(&child5_iter);
                            if (!strcmp(child5_key, "condition_type_cnf") &&
                                BSON_ITER_HOLDS_INT32(&child5_iter)) {
                                ims_data->ifc[ifc_index]
                                    .trigger_point.condition_type_cnf =
                                    bson_iter_int32(&child5_iter);
                            } else if (!strcmp(child5_key, "spt") &&
                                       BSON_ITER_HOLDS_ARRAY(&child5_iter)) {
                                int spt_index = 0;
                                bson_iter_recurse(&child5_iter, &child6_iter);
                                while (bson_iter_next(&child6_iter)) {
                                    ogs_assert(spt_index < OGS_MAX_NUM_OF_SPT);
                                    bson_iter_recurse(&child6_iter,
                                                      &child7_iter);
                                    while (bson_iter_next(&child7_iter)) {
                                        const char *child7_key =
                                            bson_iter_key(&child7_iter);
                                        if (!strcmp(child7_key,
                                                    "condition_negated") &&
                                            BSON_ITER_HOLDS_INT32(
                                                &child7_iter)) {
                                            ims_data->ifc[ifc_index]
                                                .trigger_point
                                                .spt[spt_index]
                                                .condition_negated =
                                                bson_iter_int32(
                                                    &child7_iter);
                                        } else if (!strcmp(child7_key, "group")
                                                && BSON_ITER_HOLDS_INT32(
                                                       &child7_iter)) {
                                            ims_data->ifc[ifc_index]
                                                .trigger_point
                                                .spt[spt_index]
                                                .group = bson_iter_int32(
                                                &child7_iter);
                                        } else if (!strcmp(child7_key,
                                                           "method") &&
                                                   BSON_ITER_HOLDS_UTF8(
                                                       &child7_iter)) {
                                            utf8 = bson_iter_utf8(&child7_iter,
                                                                  &length);
                                            ims_data->ifc[ifc_index]
                                                .trigger_point
                                                .spt[spt_index]
                                                .method =
                                                ogs_strndup(utf8, length);
                                            ims_data->ifc[ifc_index]
                                                .trigger_point
                                                .spt[spt_index]
                                                .type = OGS_SPT_HAS_METHOD;
                                        } else if (!strcmp(child7_key,
                                                           "session_case") &&
                                                   BSON_ITER_HOLDS_INT32(
                                                       &child7_iter)) {
                                            ims_data->ifc[ifc_index]
                                                .trigger_point
                                                .spt[spt_index]
                                                .session_case =
                                                bson_iter_int32(
                                                    &child7_iter);
                                            ims_data->ifc[ifc_index]
                                                .trigger_point
                                                .spt[spt_index]
                                                .type =
                                                    OGS_SPT_HAS_SESSION_CASE;
                                        } else if (!strcmp(child7_key,
                                                           "sip_header") &&
                                                   BSON_ITER_HOLDS_DOCUMENT(
                                                       &child7_iter)) {
                                            bson_iter_recurse(&child7_iter,
                                                              &child8_iter);
                                            while (bson_iter_next(
                                                        &child8_iter)) {
                                                const char *child8_key =
                                                    bson_iter_key(
                                                        &child8_iter);
                                                if (!strcmp(child8_key,
                                                            "header") &&
                                                    BSON_ITER_HOLDS_UTF8(
                                                        &child8_iter)) {
                                                    utf8 = bson_iter_utf8(
                                                        &child8_iter,
                                                        &length);
                                                    ims_data->ifc[ifc_index]
                                                        .trigger_point
                                                        .spt[spt_index]
                                                        .header =
                                                        ogs_strndup(utf8,
                                                                    length);
                                                } else if (!strcmp(child8_key,
                                                                   "content") &&
                                                           BSON_ITER_HOLDS_UTF8(
                                                               &child8_iter)) {
                                                    utf8 = bson_iter_utf8(
                                                        &child8_iter,
                                                        &length);
                                                    ims_data->ifc[ifc_index]
                                                        .trigger_point
                                                        .spt[spt_index]
                                                        .header_content =
                                                        ogs_strndup(utf8,
                                                                    length);
                                                }
                                            }
                                            ims_data->ifc[ifc_index]
                                                .trigger_point
                                                .spt[spt_index]
                                                .type = OGS_SPT_HAS_SIP_HEADER;
                                        } else if (!strcmp(child7_key,
                                                           "sdp_line") &&
                                                   BSON_ITER_HOLDS_DOCUMENT(
                                                       &child7_iter)) {
                                            bson_iter_recurse(&child7_iter,
                                                              &child9_iter);
                                            while (bson_iter_next(
                                                        &child9_iter)) {
                                                const char *child9_key =
                                                    bson_iter_key(
                                                        &child9_iter);
                                                if (!strcmp(child9_key,
                                                            "line") &&
                                                    BSON_ITER_HOLDS_UTF8(
                                                        &child9_iter)) {
                                                    utf8 = bson_iter_utf8(
                                                        &child9_iter,
                                                        &length);
                                                    ims_data->ifc[ifc_index]
                                                        .trigger_point
                                                        .spt[spt_index]
                                                        .sdp_line =
                                                        ogs_strndup(utf8,
                                                                    length);
                                                } else if (!strcmp(child9_key,
                                                                   "content") &&
                                                           BSON_ITER_HOLDS_UTF8(
                                                               &child9_iter)) {
                                                    utf8 = bson_iter_utf8(
                                                        &child9_iter,
                                                        &length);
                                                    ims_data->ifc[ifc_index]
                                                        .trigger_point
                                                        .spt[spt_index]
                                                        .sdp_line_content =
                                                        ogs_strndup(utf8,
                                                                    length);
                                                }
                                            }
                                            ims_data->ifc[ifc_index]
                                                .trigger_point
                                                .spt[spt_index]
                                                .type = OGS_SPT_HAS_SDP_LINE;
                                        } else if (!strcmp(child7_key,
                                                           "request_uri") &&
                                                   BSON_ITER_HOLDS_UTF8(
                                                       &child7_iter)) {
                                            utf8 = bson_iter_utf8(&child7_iter,
                                                                  &length);
                                            ims_data->ifc[ifc_index]
                                                .trigger_point
                                                .spt[spt_index]
                                                .request_uri =
                                                ogs_strndup(utf8, length);
                                            ims_data->ifc[ifc_index]
                                                .trigger_point
                                                .spt[spt_index]
                                                .type = OGS_SPT_HAS_REQUEST_URI;
                                        }
                                    }
                                    spt_index++;
                                }
                                ims_data->ifc->trigger_point.num_of_spt =
                                    spt_index;
                            }
                        }
                    }
                }
                ifc_index++;
            }
            ims_data->num_of_ifc = ifc_index;
        }
    }

out:
    if (query) bson_destroy(query);
    if (cursor) mongoc_cursor_destroy(cursor);

    ogs_free(supi_type);
    ogs_free(supi_id);

    return rv;
}
