/*
 * Copyright (C) 2019-2023 by Sukchan Lee <acetcom@gmail.com>
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
    if (!supi_type) {
        ogs_error("Invalid supi=%s", supi);
        return OGS_ERROR;
    }
    supi_id = ogs_id_get_value(supi);
    if (!supi_id) {
        ogs_error("Invalid supi=%s", supi);
        ogs_free(supi_type);
        return OGS_ERROR;
    }

    query = BCON_NEW(supi_type, BCON_UTF8(supi_id));
#if MONGOC_CHECK_VERSION(1, 5, 0)
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

    if (!bson_iter_init_find(&iter, document, OGS_SECURITY_STRING)) {
        ogs_error("No '" OGS_SECURITY_STRING "' field in this document");

        rv = OGS_ERROR;
        goto out;
    }

    memset(auth_info, 0, sizeof(ogs_dbi_auth_info_t));
    bson_iter_recurse(&iter, &inner_iter);
    while (bson_iter_next(&inner_iter)) {
        const char *key = bson_iter_key(&inner_iter);

        if (!strcmp(key, OGS_K_STRING) && BSON_ITER_HOLDS_UTF8(&inner_iter)) {
            utf8 = (char *)bson_iter_utf8(&inner_iter, &length);
            ogs_ascii_to_hex(utf8, length, buf, sizeof(buf));
            memcpy(auth_info->k, buf, OGS_KEY_LEN);
        } else if (!strcmp(key, OGS_OPC_STRING) &&
                BSON_ITER_HOLDS_UTF8(&inner_iter)) {
            utf8 = (char *)bson_iter_utf8(&inner_iter, &length);
            auth_info->use_opc = 1;
            ogs_ascii_to_hex(utf8, length, buf, sizeof(buf));
            memcpy(auth_info->opc, buf, OGS_KEY_LEN);
        } else if (!strcmp(key, OGS_OP_STRING) &&
                BSON_ITER_HOLDS_UTF8(&inner_iter)) {
            utf8 = (char *)bson_iter_utf8(&inner_iter, &length);
            ogs_ascii_to_hex(utf8, length, buf, sizeof(buf));
            memcpy(auth_info->op, buf, OGS_KEY_LEN);
        } else if (!strcmp(key, OGS_AMF_STRING) &&
                BSON_ITER_HOLDS_UTF8(&inner_iter)) {
            utf8 = (char *)bson_iter_utf8(&inner_iter, &length);
            ogs_ascii_to_hex(utf8, length, buf, sizeof(buf));
            memcpy(auth_info->amf, buf, OGS_AMF_LEN);
        } else if (!strcmp(key, OGS_RAND_STRING) &&
                BSON_ITER_HOLDS_UTF8(&inner_iter)) {
            utf8 = (char *)bson_iter_utf8(&inner_iter, &length);
            ogs_ascii_to_hex(utf8, length, buf, sizeof(buf));
            memcpy(auth_info->rand, buf, OGS_RAND_LEN);
        } else if (!strcmp(key, OGS_SQN_STRING) &&
                BSON_ITER_HOLDS_INT64(&inner_iter)) {
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
                OGS_SECURITY_STRING "." OGS_SQN_STRING, BCON_INT64(sqn),
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
                OGS_IMEISV_STRING, BCON_UTF8(imeisv),
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

int ogs_dbi_update_mme(char *supi, char *mme_host, char *mme_realm,
    bool purge_flag)
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

    ogs_debug("SUPI type: %s, SUPI id: %s, mme_host: %s, mme_realm: %s",
            supi_type, supi_id, mme_host, mme_realm);

    query = BCON_NEW(supi_type, BCON_UTF8(supi_id));
    update = BCON_NEW("$set",
            "{",
                OGS_MME_HOST_STRING, BCON_UTF8(mme_host),
                OGS_MME_REALM_STRING, BCON_UTF8(mme_realm),
                OGS_MME_TIMESTAMP_STRING, BCON_INT64(ogs_time_now()),
                OGS_PURGE_FLAG_STRING, BCON_BOOL(purge_flag),
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
                OGS_SECURITY_STRING "." OGS_SQN_STRING, BCON_INT64(32),
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
                OGS_SECURITY_STRING "." OGS_SQN_STRING,
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

    ogs_assert(subscription_data);
    ogs_assert(supi);

    memset(subscription_data, 0, sizeof(*subscription_data));

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
        if (!strcmp(key, OGS_MSISDN_STRING) &&
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

        } else if (!strcmp(key, OGS_IMSI_STRING) &&
            BSON_ITER_HOLDS_UTF8(&iter)) {
            utf8 = bson_iter_utf8(&iter, &length);
            subscription_data->imsi =
                ogs_strndup(utf8, ogs_min(length, OGS_MAX_IMSI_BCD_LEN) + 1);
            ogs_assert(subscription_data->imsi);
        } else if (!strcmp(key, OGS_ACCESS_RESTRICTION_DATA_STRING) &&
            BSON_ITER_HOLDS_INT32(&iter)) {
            subscription_data->access_restriction_data =
                bson_iter_int32(&iter);
        } else if (!strcmp(key, OGS_SUBSCRIBER_STATUS_STRING) &&
            BSON_ITER_HOLDS_INT32(&iter)) {
            subscription_data->subscriber_status =
                bson_iter_int32(&iter);
        } else if (!strcmp(key, OGS_OPERATOR_DETERMINED_BARRING_STRING) &&
            BSON_ITER_HOLDS_INT32(&iter)) {
            subscription_data->operator_determined_barring =
                bson_iter_int32(&iter);
        } else if (!strcmp(key, OGS_NETWORK_ACCESS_MODE_STRING) &&
            BSON_ITER_HOLDS_INT32(&iter)) {
            subscription_data->network_access_mode =
                bson_iter_int32(&iter);
        } else if (!strcmp(key, OGS_SUBSCRIBED_RAU_TAU_TIMER_STRING) &&
            BSON_ITER_HOLDS_INT32(&iter)) {
            subscription_data->subscribed_rau_tau_timer =
                bson_iter_int32(&iter);
        } else if (!strcmp(key, OGS_AMBR_STRING) &&
                BSON_ITER_HOLDS_DOCUMENT(&iter)) {
            bson_iter_recurse(&iter, &child1_iter);
            while (bson_iter_next(&child1_iter)) {
                const char *child1_key = bson_iter_key(&child1_iter);
                if (!strcmp(child1_key, OGS_DOWNLINK_STRING) &&
                        BSON_ITER_HOLDS_DOCUMENT(&child1_iter)) {
                    uint8_t unit = 0;
                    int n;

                    bson_iter_recurse(&child1_iter, &child2_iter);
                    while (bson_iter_next(&child2_iter)) {
                        const char *child2_key = bson_iter_key(&child2_iter);
                        if (!strcmp(child2_key, OGS_VALUE_STRING) &&
                            BSON_ITER_HOLDS_INT32(&child2_iter)) {
                            subscription_data->ambr.downlink =
                                bson_iter_int32(&child2_iter);
                        } else if (!strcmp(child2_key, OGS_UNIT_STRING) &&
                            BSON_ITER_HOLDS_INT32(&child2_iter)) {
                            unit = bson_iter_int32(&child2_iter);
                        }
                    }

                    for (n = 0; n < unit; n++)
                        subscription_data->ambr.downlink *= 1000;
                } else if (!strcmp(child1_key, OGS_UPLINK_STRING) &&
                        BSON_ITER_HOLDS_DOCUMENT(&child1_iter)) {
                    uint8_t unit = 0;
                    int n;

                    bson_iter_recurse(&child1_iter, &child2_iter);
                    while (bson_iter_next(&child2_iter)) {
                        const char *child2_key = bson_iter_key(&child2_iter);
                        if (!strcmp(child2_key, OGS_VALUE_STRING) &&
                            BSON_ITER_HOLDS_INT32(&child2_iter)) {
                            subscription_data->ambr.uplink =
                                bson_iter_int32(&child2_iter);
                        } else if (!strcmp(child2_key, OGS_UNIT_STRING) &&
                            BSON_ITER_HOLDS_INT32(&child2_iter)) {
                            unit = bson_iter_int32(&child2_iter);
                        }
                    }

                    for (n = 0; n < unit; n++)
                        subscription_data->ambr.uplink *= 1000;
                }

            }
        } else if (!strcmp(key, OGS_SLICE_STRING) &&
                BSON_ITER_HOLDS_ARRAY(&iter)) {

            bson_iter_recurse(&iter, &child1_iter);
            while (bson_iter_next(&child1_iter)) {
                ogs_slice_data_t *slice_data = NULL;
                bool sst_presence = false;

                ogs_assert(
                        subscription_data->num_of_slice < OGS_MAX_NUM_OF_SLICE);

                slice_data = &subscription_data->slice[
                                subscription_data->num_of_slice];

                slice_data->s_nssai.sst = 0;
                slice_data->s_nssai.sd.v = OGS_S_NSSAI_NO_SD_VALUE;

                bson_iter_recurse(&child1_iter, &child2_iter);
                while (bson_iter_next(&child2_iter)) {
                    const char *child2_key = bson_iter_key(&child2_iter);

                    if (!strcmp(child2_key, OGS_SST_STRING) &&
                        BSON_ITER_HOLDS_INT32(&child2_iter)) {
                        slice_data->s_nssai.sst = bson_iter_int32(&child2_iter);
                        sst_presence = true;
                    } else if (!strcmp(child2_key, OGS_SD_STRING) &&
                        BSON_ITER_HOLDS_UTF8(&child2_iter)) {
                        utf8 = bson_iter_utf8(&child2_iter, &length);
                        ogs_assert(utf8);
                        slice_data->s_nssai.sd =
                            ogs_s_nssai_sd_from_string(utf8);
                    } else if (!strcmp(child2_key,
                                OGS_DEFAULT_INDICATOR_STRING) &&
                        BSON_ITER_HOLDS_BOOL(&child2_iter)) {
                        slice_data->default_indicator =
                            bson_iter_bool(&child2_iter);
                    } else if (!strcmp(child2_key, OGS_SESSION_STRING) &&
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
                                if (!strcmp(child4_key, OGS_NAME_STRING) &&
                                    BSON_ITER_HOLDS_UTF8(&child4_iter)) {
                                    utf8 = bson_iter_utf8(
                                            &child4_iter, &length);
                                    session->name = ogs_strndup(utf8, length);
                                    ogs_assert(session->name);
                                } else if (!strcmp(child4_key,
                                            OGS_TYPE_STRING) &&
                                    BSON_ITER_HOLDS_INT32(&child4_iter)) {
                                    session->session_type =
                                        bson_iter_int32(&child4_iter);
                                } else if (!strcmp(child4_key,
                                            OGS_QOS_STRING) &&
                                    BSON_ITER_HOLDS_DOCUMENT(&child4_iter)) {
                                    bson_iter_recurse(
                                            &child4_iter, &child5_iter);
                                    while (bson_iter_next(&child5_iter)) {
                                        const char *child5_key =
                                            bson_iter_key(&child5_iter);
                                        if (!strcmp(child5_key,
                                                    OGS_INDEX_STRING) &&
                                            BSON_ITER_HOLDS_INT32(
                                                &child5_iter)) {
                                            session->qos.index =
                                                bson_iter_int32(&child5_iter);
                                        } else if (!strcmp(child5_key,
                                                    OGS_ARP_STRING) &&
                                            BSON_ITER_HOLDS_DOCUMENT(
                                                &child5_iter)) {
                                            bson_iter_recurse(
                                                    &child5_iter, &child6_iter);
                                            while (bson_iter_next(
                                                        &child6_iter)) {
                                                const char *child6_key =
                                                    bson_iter_key(&child6_iter);
                                                if (!strcmp(child6_key,
                                                    OGS_PRIORITY_LEVEL_STRING) &&
                                                    BSON_ITER_HOLDS_INT32(
                                                        &child6_iter)) {
                                                    session->qos.arp.
                                                        priority_level =
                                                        bson_iter_int32(
                                                                &child6_iter);
                                                } else if (!strcmp(child6_key,
                                                    OGS_PRE_EMPTION_CAPABILITY_STRING) &&
                                                    BSON_ITER_HOLDS_INT32(
                                                        &child6_iter)) {
                                                    session->qos.arp.
                                                        pre_emption_capability =
                                                            bson_iter_int32(
                                                                &child6_iter);
                                                } else if (!strcmp(child6_key,
                                                    OGS_PRE_EMPTION_VULNERABILITY_STRING) &&
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
                                } else if (!strcmp(child4_key,
                                            OGS_AMBR_STRING) &&
                                    BSON_ITER_HOLDS_DOCUMENT(&child4_iter)) {
                                    bson_iter_recurse(
                                            &child4_iter, &child5_iter);

                                    while (bson_iter_next(&child5_iter)) {
                                        const char *child5_key =
                                            bson_iter_key(&child5_iter);
                                        if (!strcmp(child5_key,
                                                    OGS_DOWNLINK_STRING) &&
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
                                                            OGS_VALUE_STRING) &&
                                                    BSON_ITER_HOLDS_INT32(
                                                        &child6_iter)) {
                                                    session->ambr.downlink =
                                                        bson_iter_int32(
                                                                &child6_iter);
                                                } else if (!strcmp(child6_key,
                                                            OGS_UNIT_STRING) &&
                                                    BSON_ITER_HOLDS_INT32(
                                                        &child6_iter)) {
                                                    unit = bson_iter_int32(
                                                            &child6_iter);
                                                }
                                            }

                                            for (n = 0; n < unit; n++)
                                                session->ambr.downlink *= 1000;
                                        } else if (!strcmp(child5_key,
                                                    OGS_UPLINK_STRING) &&
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
                                                            OGS_VALUE_STRING) &&
                                                    BSON_ITER_HOLDS_INT32(
                                                        &child6_iter)) {
                                                    session->ambr.uplink =
                                                        bson_iter_int32(
                                                                &child6_iter);
                                                } else if (!strcmp(child6_key,
                                                            OGS_UNIT_STRING) &&
                                                    BSON_ITER_HOLDS_INT32(
                                                        &child6_iter)) {
                                                    unit = bson_iter_int32(
                                                            &child6_iter);
                                                }
                                            }

                                            for (n = 0; n < unit; n++)
                                                session->ambr.uplink *= 1000;
                                        }
                                    }
                                } else if (!strcmp(child4_key,
                                            OGS_SMF_STRING) &&
                                    BSON_ITER_HOLDS_DOCUMENT(&child4_iter)) {
                                    bson_iter_recurse(
                                            &child4_iter, &child5_iter);
                                    while (bson_iter_next(&child5_iter)) {
                                        const char *child5_key =
                                            bson_iter_key(&child5_iter);
                                        if (!strcmp(child5_key,
                                                    OGS_IPV4_STRING) &&
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
                                        } else if (!strcmp(child5_key,
                                                    OGS_IPV6_STRING) &&
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
                                } else if (!strcmp(child4_key, OGS_UE_STRING) &&
                                    BSON_ITER_HOLDS_DOCUMENT(&child4_iter)) {
                                    bson_iter_recurse(
                                            &child4_iter, &child5_iter);
                                    while (bson_iter_next(&child5_iter)) {
                                        const char *child5_key =
                                            bson_iter_key(&child5_iter);
                                        if (!strcmp(child5_key,
                                                    OGS_IPV4_STRING) &&
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
                                        } else if (!strcmp(child5_key,
                                                    OGS_IPV6_STRING) &&
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
                                } else if (!strcmp(child4_key,
                                            OGS_IPV4_FRAMED_ROUTES_STRING) &&
                                    BSON_ITER_HOLDS_ARRAY(&child4_iter)) {
                                    int i;

                                    if (session->ipv4_framed_routes) {
                                        for (i = 0; i < OGS_MAX_NUM_OF_FRAMED_ROUTES_IN_PDI; i++) {
                                            if (!session->ipv4_framed_routes[i])
                                                break;
                                            ogs_free(session->
                                                    ipv4_framed_routes[i]);
                                        }
                                    } else {
                                        session->ipv4_framed_routes =
                                            ogs_calloc(
                                                OGS_MAX_NUM_OF_FRAMED_ROUTES_IN_PDI,
                                                sizeof(session->
                                                    ipv4_framed_routes[0]));
                                    }
                                    bson_iter_recurse(
                                            &child4_iter, &child5_iter);
                                    i = 0;
                                    while (bson_iter_next(&child5_iter)) {
                                        const char *v;

                                        if (i >= OGS_MAX_NUM_OF_FRAMED_ROUTES_IN_PDI)
                                            break;

                                        if (!BSON_ITER_HOLDS_UTF8(&child5_iter))
                                            continue;
                                        v = bson_iter_utf8(&child5_iter, &length);
                                        session->ipv4_framed_routes[i++] =
                                            ogs_strdup(v);
                                    }
                                } else if (!strcmp(child4_key,
                                            OGS_IPV6_FRAMED_ROUTES_STRING) &&
                                    BSON_ITER_HOLDS_ARRAY(&child4_iter)) {
                                    int i;

                                    if (session->ipv6_framed_routes) {
                                        for (i = 0; i < OGS_MAX_NUM_OF_FRAMED_ROUTES_IN_PDI; i++) {
                                            if (!session->ipv6_framed_routes[i])
                                                break;
                                            ogs_free(session->
                                                    ipv6_framed_routes[i]);
                                        }
                                    } else {
                                        session->ipv6_framed_routes =
                                            ogs_calloc(
                                                OGS_MAX_NUM_OF_FRAMED_ROUTES_IN_PDI,
                                                sizeof(session->
                                                    ipv6_framed_routes[0]));
                                    }
                                    bson_iter_recurse(
                                            &child4_iter, &child5_iter);
                                    i = 0;
                                    while (bson_iter_next(&child5_iter)) {
                                        const char *v;

                                        if (i >= OGS_MAX_NUM_OF_FRAMED_ROUTES_IN_PDI)
                                            break;

                                        if (!BSON_ITER_HOLDS_UTF8(&child5_iter))
                                            continue;
                                        v = bson_iter_utf8(&child5_iter, &length);
                                        session->ipv6_framed_routes[i++] =
                                            ogs_strdup(v);
                                    }
                                }
                            }
                            slice_data->num_of_session++;
                        }
                    }
                }

                if (!sst_presence) {
                    ogs_error("No SST");
                    continue;
                }

                subscription_data->num_of_slice++;
            }
        } else if (!strcmp(key, OGS_MME_HOST_STRING) &&
            BSON_ITER_HOLDS_UTF8(&iter)) {
            utf8 = bson_iter_utf8(&iter, &length);
            subscription_data->mme_host =
                ogs_strndup(utf8, ogs_min(length, OGS_MAX_FQDN_LEN) + 1);
            ogs_assert(subscription_data->mme_host);
        } else if (!strcmp(key, OGS_MME_REALM_STRING) &&
            BSON_ITER_HOLDS_UTF8(&iter)) {
            utf8 = bson_iter_utf8(&iter, &length);
            subscription_data->mme_realm =
                ogs_strndup(utf8, ogs_min(length, OGS_MAX_FQDN_LEN) + 1);
            ogs_assert(subscription_data->mme_realm);
        } else if (!strcmp(key, OGS_PURGE_FLAG_STRING) &&
            BSON_ITER_HOLDS_BOOL(&iter)) {
            subscription_data->purge_flag = bson_iter_bool(&iter);
        }
    }

out:
    if (query) bson_destroy(query);
    if (cursor) mongoc_cursor_destroy(cursor);

    ogs_free(supi_type);
    ogs_free(supi_id);

    return rv;
}
