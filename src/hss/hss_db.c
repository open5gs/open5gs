#define TRACE_MODULE _hss_db

#include <mongoc.h>

#include "core_lib.h"

#include "context.h"
#include "hss_db.h"

static mongoc_collection_t *subscriberCollection = NULL;

status_t hss_db_init()
{
    if (context_self()->db_client && context_self()->db_name)
    {
        subscriberCollection = mongoc_client_get_collection(
            context_self()->db_client, 
            context_self()->db_name, "subscribers");
        d_assert(subscriberCollection, return CORE_ERROR, 
            "Couldn't find Subscriber Collection in '%s'",
            context_self()->db_name)
    }

    return CORE_OK;
}

status_t hss_db_final()
{
    if (subscriberCollection)
    {
        mongoc_collection_destroy(subscriberCollection);
    }

    return CORE_OK;
}

status_t hss_db_auth_info(
    char *imsi_bcd, hss_db_auth_info_t *auth_info)
{
    mongoc_cursor_t *cursor;
    bson_t *query;
    bson_error_t error;
    const bson_t *document;
    bson_iter_t iter;
    bson_iter_t inner_iter;
    char buf[HSS_KEY_LEN];
    char *utf8 = NULL;
    c_uint32_t length = 0;

    d_assert(imsi_bcd, return CORE_ERROR, "Null param");
    d_assert(auth_info, return CORE_ERROR, "Null param");

    query = BCON_NEW("imsi", BCON_UTF8(imsi_bcd));
    cursor = mongoc_collection_find_with_opts(
            subscriberCollection, query, NULL, NULL);

    mongoc_cursor_next(cursor, &document);
    if (mongoc_cursor_error(cursor, &error))
    {
        d_error("Cursor Failure: %s", error.message);

        bson_destroy(query);
        return CORE_ERROR;
    }

    if (!bson_iter_init_find(&iter, document, "security"))
    {
        d_error("No 'security' field in this document");

        bson_destroy(query);
        mongoc_cursor_destroy(cursor);
        return CORE_ERROR;

    }

    memset(auth_info, 0, sizeof(hss_db_auth_info_t));
    bson_iter_recurse(&iter, &inner_iter);
    while(bson_iter_next(&inner_iter))
    {
        const char *key = bson_iter_key(&inner_iter);

        if (!strcmp(key, "k") && BSON_ITER_HOLDS_UTF8(&inner_iter)) 
        {
            utf8 = (char *)bson_iter_utf8(&inner_iter, &length);
            memcpy(auth_info->k, CORE_HEX(utf8, length, buf), HSS_KEY_LEN);
        }
        else if (!strcmp(key, "op") && BSON_ITER_HOLDS_UTF8(&inner_iter)) 
        {
            utf8 = (char *)bson_iter_utf8(&inner_iter, &length);
            memcpy(auth_info->op, CORE_HEX(utf8, length, buf), HSS_KEY_LEN);
        }
        else if (!strcmp(key, "amf") && BSON_ITER_HOLDS_UTF8(&inner_iter)) 
        {
            utf8 = (char *)bson_iter_utf8(&inner_iter, &length);
            memcpy(auth_info->amf, CORE_HEX(utf8, length, buf), HSS_AMF_LEN);
        }
        else if (!strcmp(key, "rand") && BSON_ITER_HOLDS_UTF8(&inner_iter)) 
        {
            utf8 = (char *)bson_iter_utf8(&inner_iter, &length);
            memcpy(auth_info->rand, CORE_HEX(utf8, length, buf), RAND_LEN);
        }
        else if (!strcmp(key, "sqn") && BSON_ITER_HOLDS_INT64(&inner_iter))
        {
            auth_info->sqn = bson_iter_int64(&inner_iter);
        }
    }

    bson_destroy(query);
    mongoc_cursor_destroy(cursor);

    return CORE_OK;
}

status_t hss_db_update_rand_and_sqn(
    char *imsi_bcd, c_uint8_t *rand, c_uint64_t sqn)
{
    bson_t *query;
    bson_t *update;
    bson_error_t error;
    char printable_rand[128];

    d_assert(rand, return CORE_ERROR, "Null param");
    core_hex_to_ascii(rand, RAND_LEN, printable_rand, sizeof(printable_rand));

    query = BCON_NEW("imsi", BCON_UTF8(imsi_bcd));
    update = BCON_NEW("$set",
            "{",
                "security.rand", printable_rand,
                "security.sqn", BCON_INT64(sqn),
            "}");

    if (!mongoc_collection_update(subscriberCollection,
            MONGOC_UPDATE_NONE, query, update, NULL, &error)) 
    {
        d_error("mongoc_collection_update() failure: %s", error.message);

        bson_destroy(query);
        bson_destroy(update);
        return CORE_ERROR;
    }

    bson_destroy(query);
    bson_destroy(update);

    return CORE_OK;
}

status_t hss_db_increment_sqn(char *imsi_bcd)
{
    bson_t *query;
    bson_t *update;
    bson_error_t error;
    char printable_rand[128];
    c_uint64_t max_sqn = 0x7ffffffffff;

    d_assert(rand, return CORE_ERROR, "Null param");
    core_hex_to_ascii(rand, RAND_LEN, printable_rand, sizeof(printable_rand));

    query = BCON_NEW("imsi", BCON_UTF8(imsi_bcd));

    update = BCON_NEW("$inc",
            "{",
                "security.sqn", BCON_INT64(32),
            "}");
    if (!mongoc_collection_update(subscriberCollection,
            MONGOC_UPDATE_NONE, query, update, NULL, &error))
    {
        d_error("mongoc_collection_update() failure: %s", error.message);

        bson_destroy(query);
        bson_destroy(update);
        return CORE_ERROR;
    }
    bson_destroy(update);

    update = BCON_NEW("$bit",
            "{",
                "security.sqn", 
                "{", "and", BCON_INT64(max_sqn), "}",
            "}");
    if (!mongoc_collection_update(subscriberCollection,
            MONGOC_UPDATE_NONE, query, update, NULL, &error))
    {
        d_error("mongoc_collection_update() failure: %s", error.message);

        bson_destroy(query);
        bson_destroy(update);
        return CORE_ERROR;
    }
    bson_destroy(update);

    bson_destroy(query);

    return CORE_OK;
}

status_t hss_db_subscription_data(
    char *imsi_bcd, hss_db_subscription_data_t *subscription_data)
{
    mongoc_cursor_t *cursor;
    bson_t *query;
    bson_error_t error;
    const bson_t *document;
    bson_iter_t iter;
    bson_iter_t child1_iter, child2_iter, child3_iter, child4_iter;
    const char *utf8 = NULL;
    c_uint32_t length = 0;

    d_assert(imsi_bcd, return CORE_ERROR, "Null param");
    d_assert(subscription_data, return CORE_ERROR, "Null param");

    query = BCON_NEW("imsi", BCON_UTF8(imsi_bcd));
    cursor = mongoc_collection_find_with_opts(
            subscriberCollection, query, NULL, NULL);

    mongoc_cursor_next(cursor, &document);
    if (mongoc_cursor_error(cursor, &error))
    {
        d_error("Cursor Failure: %s", error.message);

        bson_destroy(query);
        return CORE_ERROR;
    }

    if (!bson_iter_init(&iter, document))
    {
        d_error("bson_iter_init failed in this document");

        bson_destroy(query);
        mongoc_cursor_destroy(cursor);
        return CORE_ERROR;

    }

    memset(subscription_data, 0, sizeof(hss_db_subscription_data_t));
    while(bson_iter_next(&iter))
    {
        const char *key = bson_iter_key(&iter);
        if (!strcmp(key, "access_restriction_data") &&
            BSON_ITER_HOLDS_INT32(&iter))
        {
            subscription_data->access_restriction_data =
                bson_iter_int32(&iter);

        }
        else if (!strcmp(key, "subscriber_status") &&
            BSON_ITER_HOLDS_INT32(&iter))
        {
            subscription_data->subscriber_status =
                bson_iter_int32(&iter);
        }
        else if (!strcmp(key, "network_access_mode") &&
            BSON_ITER_HOLDS_INT32(&iter))
        {
            subscription_data->network_access_mode =
                bson_iter_int32(&iter);
        }
        else if (!strcmp(key, "subscribed_rau_tau_timer") &&
            BSON_ITER_HOLDS_INT32(&iter))
        {
            subscription_data->subscribed_rau_tau_timer =
                bson_iter_int32(&iter);
        }
        else if (!strcmp(key, "ue_ambr") &&
            BSON_ITER_HOLDS_DOCUMENT(&iter))
        {
            bson_iter_recurse(&iter, &child1_iter);
            while(bson_iter_next(&child1_iter))
            {
                const char *child1_key = bson_iter_key(&child1_iter);
                if (!strcmp(child1_key, "max_bandwidth_ul") &&
                    BSON_ITER_HOLDS_INT32(&child1_iter))
                {
                    subscription_data->max_bandwidth_ul =
                        bson_iter_int32(&child1_iter);
                }
                else if (!strcmp(child1_key, "max_bandwidth_dl") &&
                    BSON_ITER_HOLDS_INT32(&child1_iter))
                {
                    subscription_data->max_bandwidth_dl =
                        bson_iter_int32(&child1_iter);
                }
            }
        }
        else if (!strcmp(key, "pdn") &&
            BSON_ITER_HOLDS_ARRAY(&iter))
        {
            int pdn_index = 0;

            bson_iter_recurse(&iter, &child1_iter);
            while(bson_iter_next(&child1_iter))
            {
                const char *child1_key = bson_iter_key(&child1_iter);
                pdn_t *pdn = NULL;

                d_assert(child1_key, return CORE_ERROR, "PDN is not ARRAY");
                pdn_index = atoi(child1_key);
                d_assert(pdn_index < MAX_NUM_OF_PDN,
                        return CORE_ERROR, "Overflow of PDN number(%d>%d)",
                        pdn_index, MAX_NUM_OF_PDN);

                pdn = &subscription_data->pdn[pdn_index];

                bson_iter_recurse(&child1_iter, &child2_iter);
                while(bson_iter_next(&child2_iter))
                {
                    const char *child2_key = bson_iter_key(&child2_iter);
                    if (!strcmp(child2_key, "apn") &&
                        BSON_ITER_HOLDS_UTF8(&child2_iter))
                    {
                        utf8 = bson_iter_utf8(&child2_iter, &length);
                        core_cpystrn(pdn->apn+1, utf8, length+1);
                        pdn->apn[0] = length;
                    }
                    else if (!strcmp(child2_key, "type") &&
                        BSON_ITER_HOLDS_INT32(&child2_iter))
                    {
                        pdn->s6a_type = bson_iter_int32(&child2_iter);
                    }
                    else if (!strcmp(child2_key, "qos") &&
                        BSON_ITER_HOLDS_DOCUMENT(&child2_iter))
                    {
                        bson_iter_recurse(&child2_iter, &child3_iter);
                        while(bson_iter_next(&child3_iter))
                        {
                            const char *child3_key =
                                bson_iter_key(&child3_iter);
                            if (!strcmp(child3_key, "qci") &&
                                BSON_ITER_HOLDS_INT32(&child3_iter))
                            {
                                pdn->qci = bson_iter_int32(&child3_iter);
                            }
                            else if (!strcmp(child3_key, "arp") &&
                                BSON_ITER_HOLDS_DOCUMENT(&child3_iter))
                            {
                                bson_iter_recurse(&child3_iter, &child4_iter);
                                while(bson_iter_next(&child4_iter))
                                {
                                    const char *child4_key =
                                        bson_iter_key(&child4_iter);
                                    if (!strcmp(child4_key, "priority_level") &&
                                        BSON_ITER_HOLDS_INT32(&child4_iter))
                                    {
                                        pdn->priority_level =
                                            bson_iter_int32(&child4_iter);
                                    }
                                    else if (!strcmp(child4_key,
                                                "pre_emption_capability") &&
                                        BSON_ITER_HOLDS_INT32(&child4_iter))
                                    {
                                        pdn->pre_emption_capability =
                                            bson_iter_int32(&child4_iter);
                                    }
                                    else if (!strcmp(child4_key,
                                                "pre_emption_vulnerability") &&
                                        BSON_ITER_HOLDS_INT32(&child4_iter))
                                    {
                                        pdn->pre_emption_vulnerability =
                                            bson_iter_int32(&child4_iter);
                                    }
                                }
                            }
                        }
                    }
                    else if (!strcmp(child2_key, "pdn_ambr") &&
                        BSON_ITER_HOLDS_DOCUMENT(&child2_iter))
                    {
                        bson_iter_recurse(&child2_iter, &child3_iter);
                        while(bson_iter_next(&child3_iter))
                        {
                            const char *child3_key =
                                bson_iter_key(&child3_iter);
                            if (!strcmp(child3_key, "max_bandwidth_ul") &&
                                BSON_ITER_HOLDS_INT32(&child3_iter))
                            {
                                pdn->max_bandwidth_ul =
                                    bson_iter_int32(&child3_iter);
                            }
                            else if (!strcmp(child3_key, "max_bandwidth_dl") &&
                                BSON_ITER_HOLDS_INT32(&child3_iter))
                            {
                                pdn->max_bandwidth_dl =
                                    bson_iter_int32(&child3_iter);
                            }
                        }
                    }

                }
            }

            subscription_data->num_of_pdn = pdn_index + 1;
        }
    }

    bson_destroy(query);
    mongoc_cursor_destroy(cursor);

    return CORE_OK;
}
