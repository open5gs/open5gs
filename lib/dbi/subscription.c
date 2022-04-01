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

int ogs_dbi_auth_info(char *supi, ogs_dbi_auth_info_t *auth_info)
{
    int rv = OGS_OK;
    mongoc_cursor_t *cursor = NULL;
    bson_t *query = NULL;
    bson_error_t error;
    const bson_t *document;
    bson_iter_t iter;
    bson_iter_t inner_iter;
    char buf[OGS_KEY_LEN];
    char *utf8 = NULL;
    uint32_t length = 0;

    char *supi_type = NULL;
    char *supi_id = NULL;

    ogs_assert(supi);
    ogs_assert(auth_info);

    supi_type = ogs_id_get_type(supi);
    ogs_assert(supi_type);
    supi_id = ogs_id_get_value(supi);
    ogs_assert(supi_id);

    query = BCON_NEW(supi_type, BCON_UTF8(supi_id));
#if MONGOC_MAJOR_VERSION >= 1 && MONGOC_MINOR_VERSION >= 5
    cursor = mongoc_collection_find_with_opts(
            ogs_mongoc()->collection.subscriber, query, NULL, NULL);
#else
    cursor = mongoc_collection_find(ogs_mongoc()->collection.subscriber,
            MONGOC_QUERY_NONE, 0, 0, 0, query, NULL, NULL);
#endif

    if (!mongoc_cursor_next(cursor, &document)) {
        ogs_info("[%s] Cannot find IMSI in DB", supi);

        rv = OGS_ERROR;
        goto out;
    }

    if (mongoc_cursor_error(cursor, &error)) {
        ogs_error("Cursor Failure: %s", error.message);

        rv = OGS_ERROR;
        goto out;
    }

    if (!bson_iter_init_find(&iter, document, "security")) {
        ogs_error("No 'security' field in this document");

        rv = OGS_ERROR;
        goto out;
    }

    memset(auth_info, 0, sizeof(ogs_dbi_auth_info_t));
    bson_iter_recurse(&iter, &inner_iter);
    while (bson_iter_next(&inner_iter)) {
        const char *key = bson_iter_key(&inner_iter);

        if (!strcmp(key, "k") && BSON_ITER_HOLDS_UTF8(&inner_iter)) {
            utf8 = (char *)bson_iter_utf8(&inner_iter, &length);
            memcpy(auth_info->k, OGS_HEX(utf8, length, buf), OGS_KEY_LEN);
        } else if (!strcmp(key, "opc") && BSON_ITER_HOLDS_UTF8(&inner_iter)) {
            utf8 = (char *)bson_iter_utf8(&inner_iter, &length);
            auth_info->use_opc = 1;
            memcpy(auth_info->opc, OGS_HEX(utf8, length, buf), OGS_KEY_LEN);
        } else if (!strcmp(key, "op") && BSON_ITER_HOLDS_UTF8(&inner_iter)) {
            utf8 = (char *)bson_iter_utf8(&inner_iter, &length);
            memcpy(auth_info->op, OGS_HEX(utf8, length, buf), OGS_KEY_LEN);
        } else if (!strcmp(key, "amf") && BSON_ITER_HOLDS_UTF8(&inner_iter)) {
            utf8 = (char *)bson_iter_utf8(&inner_iter, &length);
            memcpy(auth_info->amf, OGS_HEX(utf8, length, buf), OGS_AMF_LEN);
        } else if (!strcmp(key, "rand") && BSON_ITER_HOLDS_UTF8(&inner_iter)) {
            utf8 = (char *)bson_iter_utf8(&inner_iter, &length);
            memcpy(auth_info->rand, OGS_HEX(utf8, length, buf), OGS_RAND_LEN);
        } else if (!strcmp(key, "sqn") && BSON_ITER_HOLDS_INT64(&inner_iter)) {
            auth_info->sqn = bson_iter_int64(&inner_iter);
        }
    }

out:
    if (query) bson_destroy(query);
    if (cursor) mongoc_cursor_destroy(cursor);

    ogs_free(supi_type);
    ogs_free(supi_id);

    return rv;
}

int ogs_dbi_update_sqn(char *supi, uint64_t sqn)
{
    int rv = OGS_OK;
    bson_t *query = NULL;
    bson_t *update = NULL;
    bson_error_t error;

    char *supi_type = NULL;
    char *supi_id = NULL;

    ogs_assert(supi);

    supi_type = ogs_id_get_type(supi);
    ogs_assert(supi_type);
    supi_id = ogs_id_get_value(supi);
    ogs_assert(supi_id);

    query = BCON_NEW(supi_type, BCON_UTF8(supi_id));
    update = BCON_NEW("$set",
            "{",
                "security.sqn", BCON_INT64(sqn),
            "}");

    if (!mongoc_collection_update(ogs_mongoc()->collection.subscriber,
            MONGOC_UPDATE_NONE, query, update, NULL, &error)) {
        ogs_error("mongoc_collection_update() failure: %s", error.message);

        rv = OGS_ERROR;
    }

    if (query) bson_destroy(query);
    if (update) bson_destroy(update);

    ogs_free(supi_type);
    ogs_free(supi_id);

    return rv;
}

int ogs_dbi_update_imeisv(char *supi, char *imeisv)
{
    int rv = OGS_OK;
    bson_t *query = NULL;
    bson_t *update = NULL;
    bson_error_t error;

    char *supi_type = NULL;
    char *supi_id = NULL;

    ogs_assert(supi);

    supi_type = ogs_id_get_type(supi);
    ogs_assert(supi_type);
    supi_id = ogs_id_get_value(supi);
    ogs_assert(supi_id);

    ogs_debug("SUPI type: %s, SUPI id: %s, imeisv: %s",
            supi_type, supi_id, imeisv);

    query = BCON_NEW(supi_type, BCON_UTF8(supi_id));
    update = BCON_NEW("$set",
            "{",
                "imeisv", BCON_UTF8(imeisv),
            "}");
    if (!mongoc_collection_update(ogs_mongoc()->collection.subscriber,
            MONGOC_UPDATE_UPSERT, query, update, NULL, &error)) {
        ogs_error("mongoc_collection_update() failure: %s", error.message);

        rv = OGS_ERROR;
    }

    if (query) bson_destroy(query);
    if (update) bson_destroy(update);

    ogs_free(supi_type);
    ogs_free(supi_id);

    return rv;
}

int ogs_dbi_increment_sqn(char *supi)
{
    int rv = OGS_OK;
    bson_t *query = NULL;
    bson_t *update = NULL;
    bson_error_t error;
    uint64_t max_sqn = OGS_MAX_SQN;

    char *supi_type = NULL;
    char *supi_id = NULL;

    ogs_assert(supi);

    supi_type = ogs_id_get_type(supi);
    ogs_assert(supi_type);
    supi_id = ogs_id_get_value(supi);
    ogs_assert(supi_id);

    query = BCON_NEW(supi_type, BCON_UTF8(supi_id));
    update = BCON_NEW("$inc",
            "{",
                "security.sqn", BCON_INT64(32),
            "}");
    if (!mongoc_collection_update(ogs_mongoc()->collection.subscriber,
            MONGOC_UPDATE_NONE, query, update, NULL, &error)) {
        ogs_error("mongoc_collection_update() failure: %s", error.message);

        rv = OGS_ERROR;
        goto out;
    }
    bson_destroy(update);

    update = BCON_NEW("$bit",
            "{",
                "security.sqn", 
                "{", "and", BCON_INT64(max_sqn), "}",
            "}");
    if (!mongoc_collection_update(ogs_mongoc()->collection.subscriber,
            MONGOC_UPDATE_NONE, query, update, NULL, &error)) {
        ogs_error("mongoc_collection_update() failure: %s", error.message);

        rv = OGS_ERROR;
    }

out:
    if (query) bson_destroy(query);
    if (update) bson_destroy(update);

    ogs_free(supi_type);
    ogs_free(supi_id);

    return rv;
}

int ogs_dbi_subscription_data(char *supi,
        ogs_subscription_data_t *subscription_data)
{
    int rv = OGS_OK;
    mongoc_cursor_t *cursor = NULL;
    bson_t *query = NULL;
    bson_error_t error;
    const bson_t *document;
    bson_iter_t iter;
    bson_iter_t child1_iter, child2_iter, child3_iter;
    bson_iter_t child4_iter, child5_iter, child6_iter;
    const char *utf8 = NULL;
    uint32_t length = 0;

    char *supi_type = NULL;
    char *supi_id = NULL;

    ogs_subscription_data_t zero_data;

    ogs_assert(subscription_data);
    ogs_assert(supi);

    memset(&zero_data, 0, sizeof(zero_data));

    /* subscription_data should be initialized to zero */
    ogs_assert(memcmp(subscription_data, &zero_data, sizeof(zero_data)) == 0);

    supi_type = ogs_id_get_type(supi);
    ogs_assert(supi_type);
    supi_id = ogs_id_get_value(supi);
    ogs_assert(supi_id);

    query = BCON_NEW(supi_type, BCON_UTF8(supi_id));
#if MONGOC_MAJOR_VERSION >= 1 && MONGOC_MINOR_VERSION >= 5
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
                    ogs_cpystrn(subscription_data->msisdn[msisdn_index].bcd,
                            utf8, ogs_min(length, OGS_MAX_MSISDN_BCD_LEN)+1);
                    ogs_bcd_to_buffer(
                            subscription_data->msisdn[msisdn_index].bcd,
                            subscription_data->msisdn[msisdn_index].buf,
                            &subscription_data->msisdn[msisdn_index].len);

                    msisdn_index++;
                }
            }
            subscription_data->num_of_msisdn = msisdn_index;

        } else if (!strcmp(key, "access_restriction_data") &&
            BSON_ITER_HOLDS_INT32(&iter)) {
            subscription_data->access_restriction_data =
                bson_iter_int32(&iter);
        } else if (!strcmp(key, "subscriber_status") &&
            BSON_ITER_HOLDS_INT32(&iter)) {
            subscription_data->subscriber_status =
                bson_iter_int32(&iter);
        } else if (!strcmp(key, "network_access_mode") &&
            BSON_ITER_HOLDS_INT32(&iter)) {
            subscription_data->network_access_mode =
                bson_iter_int32(&iter);
        } else if (!strcmp(key, "subscribed_rau_tau_timer") &&
            BSON_ITER_HOLDS_INT32(&iter)) {
            subscription_data->subscribed_rau_tau_timer =
                bson_iter_int32(&iter);
        } else if (!strcmp(key, "ambr") && BSON_ITER_HOLDS_DOCUMENT(&iter)) {
            bson_iter_recurse(&iter, &child1_iter);
            while (bson_iter_next(&child1_iter)) {
                const char *child1_key = bson_iter_key(&child1_iter);
                if (!strcmp(child1_key, "downlink") &&
                        BSON_ITER_HOLDS_DOCUMENT(&child1_iter)) {
                    uint8_t unit = 0;
                    int n;

                    bson_iter_recurse(&child1_iter, &child2_iter);
                    while (bson_iter_next(&child2_iter)) {
                        const char *child2_key = bson_iter_key(&child2_iter);
                        if (!strcmp(child2_key, "value") &&
                            BSON_ITER_HOLDS_INT32(&child2_iter)) {
                            subscription_data->ambr.downlink =
                                bson_iter_int32(&child2_iter);
                        } else if (!strcmp(child2_key, "unit") &&
                            BSON_ITER_HOLDS_INT32(&child2_iter)) {
                            unit = bson_iter_int32(&child2_iter);
                        }
                    }

                    for (n = 0; n < unit; n++)
                        subscription_data->ambr.downlink *= 1024;
                } else if (!strcmp(child1_key, "uplink") &&
                        BSON_ITER_HOLDS_DOCUMENT(&child1_iter)) {
                    uint8_t unit = 0;
                    int n;

                    bson_iter_recurse(&child1_iter, &child2_iter);
                    while (bson_iter_next(&child2_iter)) {
                        const char *child2_key = bson_iter_key(&child2_iter);
                        if (!strcmp(child2_key, "value") &&
                            BSON_ITER_HOLDS_INT32(&child2_iter)) {
                            subscription_data->ambr.uplink =
                                bson_iter_int32(&child2_iter);
                        } else if (!strcmp(child2_key, "unit") &&
                            BSON_ITER_HOLDS_INT32(&child2_iter)) {
                            unit = bson_iter_int32(&child2_iter);
                        }
                    }

                    for (n = 0; n < unit; n++)
                        subscription_data->ambr.uplink *= 1024;
                }

            }
        } else if (!strcmp(key, "slice") && BSON_ITER_HOLDS_ARRAY(&iter)) {

            bson_iter_recurse(&iter, &child1_iter);
            while (bson_iter_next(&child1_iter)) {
                ogs_slice_data_t *slice_data = NULL;

                ogs_assert(
                        subscription_data->num_of_slice < OGS_MAX_NUM_OF_SLICE);

                slice_data = &subscription_data->slice[
                                subscription_data->num_of_slice];

                slice_data->s_nssai.sst = 0;
                slice_data->s_nssai.sd.v = OGS_S_NSSAI_NO_SD_VALUE;

                bson_iter_recurse(&child1_iter, &child2_iter);
                while (bson_iter_next(&child2_iter)) {
                    const char *child2_key = bson_iter_key(&child2_iter);

                    if (!strcmp(child2_key, "sst") &&
                        BSON_ITER_HOLDS_INT32(&child2_iter)) {
                        slice_data->s_nssai.sst = bson_iter_int32(&child2_iter);
                    } else if (!strcmp(child2_key, "sd") &&
                        BSON_ITER_HOLDS_UTF8(&child2_iter)) {
                        utf8 = bson_iter_utf8(&child2_iter, &length);
                        ogs_assert(utf8);
                        slice_data->s_nssai.sd =
                            ogs_s_nssai_sd_from_string(utf8);
                    } else if (!strcmp(child2_key, "default_indicator") &&
                        BSON_ITER_HOLDS_BOOL(&child2_iter)) {
                        slice_data->default_indicator =
                            bson_iter_bool(&child2_iter);
                    } else if (!strcmp(child2_key, "session") &&
                        BSON_ITER_HOLDS_ARRAY(&child2_iter)) {

                        bson_iter_recurse(&child2_iter, &child3_iter);
                        while (bson_iter_next(&child3_iter)) {
                            ogs_session_t *session = NULL;

                            ogs_assert(
                                slice_data->num_of_session <
                                    OGS_MAX_NUM_OF_SESS);
                            session = &slice_data->session
                                [slice_data->num_of_session];

                            bson_iter_recurse(&child3_iter, &child4_iter);
                            while (bson_iter_next(&child4_iter)) {
                                const char *child4_key =
                                    bson_iter_key(&child4_iter);
                                if (!strcmp(child4_key, "name") &&
                                    BSON_ITER_HOLDS_UTF8(&child4_iter)) {
                                    utf8 = bson_iter_utf8(
                                            &child4_iter, &length);
                                    session->name = ogs_strndup(utf8, length);
                                    ogs_assert(session->name);
                                } else if (!strcmp(child4_key, "type") &&
                                    BSON_ITER_HOLDS_INT32(&child4_iter)) {
                                    session->session_type =
                                        bson_iter_int32(&child4_iter);
                                } else if (!strcmp(child4_key, "qos") &&
                                    BSON_ITER_HOLDS_DOCUMENT(&child4_iter)) {
                                    bson_iter_recurse(
                                            &child4_iter, &child5_iter);
                                    while (bson_iter_next(&child5_iter)) {
                                        const char *child5_key =
                                            bson_iter_key(&child5_iter);
                                        if (!strcmp(child5_key, "index") &&
                                            BSON_ITER_HOLDS_INT32(
                                                &child5_iter)) {
                                            session->qos.index =
                                                bson_iter_int32(&child5_iter);
                                        } else if (!strcmp(child5_key, "arp") &&
                                            BSON_ITER_HOLDS_DOCUMENT(
                                                &child5_iter)) {
                                            bson_iter_recurse(
                                                    &child5_iter, &child6_iter);
                                            while (bson_iter_next(
                                                        &child6_iter)) {
                                                const char *child6_key =
                                                    bson_iter_key(&child6_iter);
                                                if (!strcmp(child6_key,
                                                            "priority_level") &&
                                                    BSON_ITER_HOLDS_INT32(
                                                        &child6_iter)) {
                                                    session->qos.arp.
                                                        priority_level =
                                                        bson_iter_int32(
                                                                &child6_iter);
                                                } else if (!strcmp(child6_key,
                                                    "pre_emption_capability") &&
                                                    BSON_ITER_HOLDS_INT32(
                                                        &child6_iter)) {
                                                    session->qos.arp.
                                                        pre_emption_capability =
                                                            bson_iter_int32(
                                                                &child6_iter);
                                                } else if (!strcmp(child6_key,
                                                "pre_emption_vulnerability") &&
                                                    BSON_ITER_HOLDS_INT32(
                                                        &child6_iter)) {
                                                    session->qos.arp.
                                                    pre_emption_vulnerability =
                                                        bson_iter_int32(
                                                                &child6_iter);
                                                }
                                            }
                                        }
                                    }
                                } else if (!strcmp(child4_key, "ambr") &&
                                    BSON_ITER_HOLDS_DOCUMENT(&child4_iter)) {
                                    bson_iter_recurse(
                                            &child4_iter, &child5_iter);

                                    while (bson_iter_next(&child5_iter)) {
                                        const char *child5_key =
                                            bson_iter_key(&child5_iter);
                                        if (!strcmp(child5_key, "downlink") &&
                                                BSON_ITER_HOLDS_DOCUMENT(
                                                    &child5_iter)) {
                                            uint8_t unit = 0;
                                            int n;

                                            bson_iter_recurse(
                                                    &child5_iter, &child6_iter);
                                            while (bson_iter_next(
                                                        &child6_iter)) {
                                                const char *child6_key =
                                                    bson_iter_key(&child6_iter);
                                                if (!strcmp(child6_key,
                                                            "value") &&
                                                    BSON_ITER_HOLDS_INT32(
                                                        &child6_iter)) {
                                                    session->ambr.downlink =
                                                        bson_iter_int32(
                                                                &child6_iter);
                                                } else if (!strcmp(child6_key,
                                                            "unit") &&
                                                    BSON_ITER_HOLDS_INT32(
                                                        &child6_iter)) {
                                                    unit = bson_iter_int32(
                                                            &child6_iter);
                                                }
                                            }

                                            for (n = 0; n < unit; n++)
                                                session->ambr.downlink *= 1024;
                                        } else if (!strcmp(child5_key,
                                                    "uplink") &&
                                                BSON_ITER_HOLDS_DOCUMENT(
                                                    &child5_iter)) {
                                            uint8_t unit = 0;
                                            int n;

                                            bson_iter_recurse(
                                                    &child5_iter, &child6_iter);
                                            while (bson_iter_next(
                                                        &child6_iter)) {
                                                const char *child6_key =
                                                    bson_iter_key(&child6_iter);
                                                if (!strcmp(child6_key,
                                                            "value") &&
                                                    BSON_ITER_HOLDS_INT32(
                                                        &child6_iter)) {
                                                    session->ambr.uplink =
                                                        bson_iter_int32(
                                                                &child6_iter);
                                                } else if (!strcmp(child6_key,
                                                            "unit") &&
                                                    BSON_ITER_HOLDS_INT32(
                                                        &child6_iter)) {
                                                    unit = bson_iter_int32(
                                                            &child6_iter);
                                                }
                                            }

                                            for (n = 0; n < unit; n++)
                                                session->ambr.uplink *= 1024;
                                        }
                                    }
                                } else if (!strcmp(child4_key, "smf") &&
                                    BSON_ITER_HOLDS_DOCUMENT(&child4_iter)) {
                                    bson_iter_recurse(
                                            &child4_iter, &child5_iter);
                                    while (bson_iter_next(&child5_iter)) {
                                        const char *child5_key =
                                            bson_iter_key(&child5_iter);
                                        if (!strcmp(child5_key, "addr") &&
                                            BSON_ITER_HOLDS_UTF8(
                                                &child5_iter)) {
                                            ogs_ipsubnet_t ipsub;
                                            const char *v = bson_iter_utf8(
                                                    &child5_iter, &length);
                                            rv = ogs_ipsubnet(&ipsub, v, NULL);
                                            if (rv == OGS_OK) {
                                                session->smf_ip.ipv4 = 1;
                                                session->smf_ip.addr =
                                                    ipsub.sub[0];
                                            }
                                        } else if (!strcmp(
                                                    child5_key, "addr6") &&
                                            BSON_ITER_HOLDS_UTF8(
                                                &child5_iter)) {
                                            ogs_ipsubnet_t ipsub;
                                            const char *v = bson_iter_utf8(
                                                    &child5_iter, &length);
                                            rv = ogs_ipsubnet(&ipsub, v, NULL);
                                            if (rv == OGS_OK) {
                                                session->smf_ip.ipv6 = 1;
                                                memcpy(session->smf_ip.addr6,
                                                        ipsub.sub,
                                                        sizeof(ipsub.sub));
                                            }
                                        }
                                    }
                                } else if (!strcmp(child4_key, "ue") &&
                                    BSON_ITER_HOLDS_DOCUMENT(&child4_iter)) {
                                    bson_iter_recurse(
                                            &child4_iter, &child5_iter);
                                    while (bson_iter_next(&child5_iter)) {
                                        const char *child5_key =
                                            bson_iter_key(&child5_iter);
                                        if (!strcmp(child5_key, "addr") &&
                                            BSON_ITER_HOLDS_UTF8(
                                                &child5_iter)) {
                                            ogs_ipsubnet_t ipsub;
                                            const char *v = bson_iter_utf8(
                                                    &child5_iter, &length);
                                            rv = ogs_ipsubnet(&ipsub, v, NULL);
                                            if (rv == OGS_OK) {
                                                session->ue_ip.ipv4 = true;
                                                session->ue_ip.addr =
                                                    ipsub.sub[0];
                                            }
                                        } else if (!strcmp(
                                                    child5_key, "addr6") &&
                                            BSON_ITER_HOLDS_UTF8(
                                                &child5_iter)) {
                                            ogs_ipsubnet_t ipsub;
                                            const char *v = bson_iter_utf8(
                                                    &child5_iter, &length);
                                            rv = ogs_ipsubnet(&ipsub, v, NULL);
                                            if (rv == OGS_OK) {
                                                session->ue_ip.ipv6 = true;
                                                memcpy(session->ue_ip.addr6,
                                                    ipsub.sub, OGS_IPV6_LEN);
                                            }

                                        }
                                    }
                                }
                            }
                            slice_data->num_of_session++;
                        }
                    }
                }
                subscription_data->num_of_slice++;
            }
        }
    }

out:
    if (query) bson_destroy(query);
    if (cursor) mongoc_cursor_destroy(cursor);

    ogs_free(supi_type);
    ogs_free(supi_id);

    return rv;
}
