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
    bson_t query;
    bson_error_t error;
    const bson_t *document;
    bson_iter_t iter;
    bson_iter_t inner_iter;

    d_assert(imsi_bcd, return CORE_ERROR, "Null param");
    d_assert(auth_info, return CORE_ERROR, "Null param");

    bson_init(&query);
    BSON_APPEND_UTF8(&query, "imsi", imsi_bcd);
    cursor = mongoc_collection_find_with_opts(
            subscriberCollection, &query, NULL, NULL);

    mongoc_cursor_next(cursor, &document);
    if (mongoc_cursor_error(cursor, &error))
    {
        d_error("Cursor Failure: %s", error.message);

        bson_destroy(&query);
        return CORE_ERROR;
    }

    if (!bson_iter_init_find(&iter, document, "security"))
    {
        d_error("No 'security' field in this document");

        bson_destroy(&query);
        mongoc_cursor_destroy(cursor);
        return CORE_ERROR;

    }

    bson_iter_recurse(&iter, &inner_iter);
    while(bson_iter_next(&inner_iter))
    {
        char buf[HSS_DB_KEY_LEN];
        char *utf8 = NULL;
        c_uint32_t length = 0;

        const char *key = bson_iter_key(&inner_iter);

        if (!strcmp(key, "k") && BSON_ITER_HOLDS_UTF8(&inner_iter)) 
        {
            utf8 = (char *)bson_iter_utf8(&inner_iter, &length);
            memcpy(auth_info->k, CORE_HEX(utf8, length, buf), HSS_DB_KEY_LEN);
        }
        else if (!strcmp(key, "op") && BSON_ITER_HOLDS_UTF8(&inner_iter)) 
        {
            utf8 = (char *)bson_iter_utf8(&inner_iter, &length);
            memcpy(auth_info->op, CORE_HEX(utf8, length, buf), HSS_DB_KEY_LEN);
        }
        else if (!strcmp(key, "amf") && BSON_ITER_HOLDS_UTF8(&inner_iter)) 
        {
            utf8 = (char *)bson_iter_utf8(&inner_iter, &length);
            memcpy(auth_info->amf, CORE_HEX(utf8, length, buf), HSS_DB_AMF_LEN);
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

    bson_destroy(&query);
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
                "seccurity", "{",
                    "rand", printable_rand,
                    "sqn", BCON_INT64(sqn),
                "}",
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

    d_assert(rand, return CORE_ERROR, "Null param");
    core_hex_to_ascii(rand, RAND_LEN, printable_rand, sizeof(printable_rand));

    query = BCON_NEW("imsi", BCON_UTF8(imsi_bcd));
    update = BCON_NEW("$inc",
            "{",
                "seccurity.sqn", BCON_INT64(64),
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
