#define TRACE_MODULE _hss_context

#include "core_debug.h"
#include "core_lib.h"

#include <mongoc.h>

#include "s6a_lib.h"

#include "context.h"
#include "hss_context.h"

static hss_context_t self;
static int context_initialized = 0;

hss_context_t* hss_self()
{
    return &self;
}

status_t hss_context_init(void)
{
    d_assert(context_initialized == 0, return CORE_ERROR,
            "HSS context already has been context_initialized");

    /* Initialize HSS context */
    memset(&self, 0, sizeof(hss_context_t));

    if (mutex_create(&self.db_lock, MUTEX_DEFAULT) != CORE_OK)
    {
        d_error("Mutex creation failed");
        return CORE_ERROR;
    }

    context_initialized = 1;

	return CORE_OK;
}

status_t hss_context_final(void)
{
    d_assert(context_initialized == 1, return CORE_ERROR,
            "HSS context already has been finalized");

    mutex_delete(self.db_lock);

    context_initialized = 0;

	return CORE_OK;
}

static status_t hss_context_prepare()
{
    self.mme_s6a_port = DIAMETER_PORT;
    self.hss_s6a_port = DIAMETER_PORT;

    return CORE_OK;
}

static status_t hss_context_validation()
{
    if (self.s6a_config_path == NULL)
    {
        if (self.hss_s6a_addr == NULL)
        {
            d_error("No HSS.S6A_CONFIG_PATH or HSS.NETWORK.S6A_ADDR in '%s'",
                    context_self()->config.path);
            return CORE_ERROR;
        }
        if (self.mme_s6a_addr == NULL)
        {
            d_error("No HSS.S6A_CONFIG_PATH or MME.NETWORK.S6A_ADDR in '%s'",
                    context_self()->config.path);
            return CORE_ERROR;
        }
    }

    return CORE_OK;
}

status_t hss_context_parse_config()
{
    status_t rv;
    config_t *config = &context_self()->config;

    char *json = config->json;
    jsmntok_t *token = config->token;

    typedef enum {
        START, ROOT,
        HSS_START, HSS_ROOT,
        MME_START, MME_ROOT,
        SKIP, STOP
    } parse_state;
    parse_state state = START;
    parse_state stack = STOP;

    size_t root_tokens = 0;
    size_t hss_tokens = 0;
    size_t mme_tokens = 0;
    size_t skip_tokens = 0;
    int i, j, m, n;
    int arr, size;

    rv = hss_context_prepare();
    if (rv != CORE_OK) return rv;

    for (i = 0, j = 1; j > 0; i++, j--)
    {
        jsmntok_t *t = &token[i];

        j += t->size;

        switch (state)
        {
            case START:
            {
                state = ROOT;
                root_tokens = t->size;

                break;
            }
            case ROOT:
            {
                if (jsmntok_equal(json, t, "HSS") == 0)
                {
                    state = HSS_START;
                }
                else if (jsmntok_equal(json, t, "MME") == 0)
                {
                    state = MME_START;
                }
                else
                {
                    state = SKIP;
                    stack = ROOT;
                    skip_tokens = t->size;

                    root_tokens--;
                    if (root_tokens == 0) state = STOP;
                }

                break;
            }
            case HSS_START:
            {
                state = HSS_ROOT;
                hss_tokens = t->size;

                break;
            }
            case HSS_ROOT:
            {
                if (jsmntok_equal(json, t, "S6A_CONFIG_PATH") == 0)
                {
                    self.s6a_config_path = jsmntok_to_string(json, t+1);
                }
                else if (jsmntok_equal(json, t, "NETWORK") == 0)
                {
                    m = 1;
                    size = 1;

                    if ((t+1)->type == JSMN_ARRAY)
                    {
                        m = 2;
                    }

                    for (arr = 0; arr < size; arr++)
                    {
                        for (n = 1; n > 0; m++, n--)
                        {
                            n += (t+m)->size;

                            if (jsmntok_equal(json, t+m, "S6A_ADDR") == 0)
                            {
                                self.hss_s6a_addr = 
                                    jsmntok_to_string(json, t+m+1);
                            }
                            else if (jsmntok_equal(json, t+m, "S6A_PORT") == 0)
                            {
                                char *v = jsmntok_to_string(json, t+m+1);
                                if (v) self.hss_s6a_port = atoi(v);
                            }
                        }
                    }
                }

                state = SKIP;
                stack = HSS_ROOT;
                skip_tokens = t->size;

                hss_tokens--;
                if (hss_tokens == 0) stack = ROOT;
                break;
            }
            case MME_START:
            {
                state = MME_ROOT;
                mme_tokens = t->size;

                break;
            }
            case MME_ROOT:
            {
                if (jsmntok_equal(json, t, "NETWORK") == 0)
                {
                    m = 1;
                    size = 1;

                    if ((t+1)->type == JSMN_ARRAY)
                    {
                        m = 2;
                    }

                    for (arr = 0; arr < size; arr++)
                    {
                        for (n = 1; n > 0; m++, n--)
                        {
                            n += (t+m)->size;

                            if (jsmntok_equal(json, t+m, "S6A_ADDR") == 0)
                            {
                                self.mme_s6a_addr = 
                                    jsmntok_to_string(json, t+m+1);
                            }
                            else if (jsmntok_equal(json, t+m, "S6A_PORT") == 0)
                            {
                                char *v = jsmntok_to_string(json, t+m+1);
                                if (v) self.mme_s6a_port = atoi(v);
                            }
                        }
                    }
                }

                state = SKIP;
                stack = MME_ROOT;
                skip_tokens = t->size;

                mme_tokens--;
                if (mme_tokens == 0) stack = ROOT;
                break;
            }
            case SKIP:
            {
                skip_tokens += t->size;

                skip_tokens--;
                if (skip_tokens == 0) state = stack;
                break;
            }
            case STOP:
            {
                break;
            }
            default:
            {
                d_error("Failed to parse configuration in the state(%u)", 
                        state);
                break;
            }

        }
    }

    rv = hss_context_validation();
    if (rv != CORE_OK) return rv;

    return CORE_OK;
}

status_t hss_context_setup_trace_module()
{
    int context = context_self()->trace_level.context;
    int s6a = context_self()->trace_level.s6a;

    if (context)
    {
        extern int _context;
        d_trace_level(&_context, context);
        extern int _hss_context;
        d_trace_level(&_hss_context, context);
    }

    if (s6a)
    {
        extern int _hss_s6a_handler;
        d_trace_level(&_hss_s6a_handler, s6a);
        extern int _s6a;
        d_trace_level(&_s6a, s6a);
        extern int _s6a_init;
        d_trace_level(&_s6a_init, s6a);
    }

    return CORE_OK;
}

status_t hss_db_init()
{
    if (context_self()->db_client && context_self()->db_name)
    {
        self.subscriberCollection = mongoc_client_get_collection(
            context_self()->db_client, 
            context_self()->db_name, "subscribers");
        d_assert(self.subscriberCollection, return CORE_ERROR, 
            "Couldn't find Subscriber Collection in '%s'",
            context_self()->db_name)
    }

    return CORE_OK;
}

status_t hss_db_final()
{
    if (self.subscriberCollection)
    {
        mongoc_collection_destroy(self.subscriberCollection);
    }

    return CORE_OK;
}

status_t hss_db_auth_info(
    char *imsi_bcd, hss_db_auth_info_t *auth_info)
{
    status_t rv = CORE_OK;
    mongoc_cursor_t *cursor = NULL;
    bson_t *query = NULL;
    bson_error_t error;
    const bson_t *document;
    bson_iter_t iter;
    bson_iter_t inner_iter;
    char buf[HSS_KEY_LEN];
    char *utf8 = NULL;
    c_uint32_t length = 0;

    d_assert(imsi_bcd, return CORE_ERROR, "Null param");
    d_assert(auth_info, return CORE_ERROR, "Null param");

    mutex_lock(self.db_lock);

    query = BCON_NEW("imsi", BCON_UTF8(imsi_bcd));
    cursor = mongoc_collection_find_with_opts(
            self.subscriberCollection, query, NULL, NULL);

    mongoc_cursor_next(cursor, &document);
    if (mongoc_cursor_error(cursor, &error))
    {
        d_error("Cursor Failure: %s", error.message);

        rv = CORE_ERROR;
        goto out;
    }

    if (!bson_iter_init_find(&iter, document, "security"))
    {
        d_error("No 'security' field in this document");

        rv = CORE_ERROR;
        goto out;
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

out:
    if (query) bson_destroy(query);
    if (cursor) mongoc_cursor_destroy(cursor);

    mutex_unlock(self.db_lock);

    return rv;
}

status_t hss_db_update_rand_and_sqn(
    char *imsi_bcd, c_uint8_t *rand, c_uint64_t sqn)
{
    status_t rv = CORE_OK;
    bson_t *query = NULL;
    bson_t *update = NULL;
    bson_error_t error;
    char printable_rand[128];

    d_assert(rand, return CORE_ERROR, "Null param");
    core_hex_to_ascii(rand, RAND_LEN, printable_rand, sizeof(printable_rand));

    mutex_lock(self.db_lock);

    query = BCON_NEW("imsi", BCON_UTF8(imsi_bcd));
    update = BCON_NEW("$set",
            "{",
                "security.rand", printable_rand,
                "security.sqn", BCON_INT64(sqn),
            "}");

    if (!mongoc_collection_update(self.subscriberCollection,
            MONGOC_UPDATE_NONE, query, update, NULL, &error)) 
    {
        d_error("mongoc_collection_update() failure: %s", error.message);

        rv = CORE_ERROR;
    }

    if (query) bson_destroy(query);
    if (update) bson_destroy(update);

    mutex_unlock(self.db_lock);

    return rv;
}

status_t hss_db_increment_sqn(char *imsi_bcd)
{
    status_t rv = CORE_OK;
    bson_t *query = NULL;
    bson_t *update = NULL;
    bson_error_t error;
    c_uint64_t max_sqn = 0x7ffffffffff;

    mutex_lock(self.db_lock);

    query = BCON_NEW("imsi", BCON_UTF8(imsi_bcd));
    update = BCON_NEW("$inc",
            "{",
                "security.sqn", BCON_INT64(32),
            "}");
    if (!mongoc_collection_update(self.subscriberCollection,
            MONGOC_UPDATE_NONE, query, update, NULL, &error))
    {
        d_error("mongoc_collection_update() failure: %s", error.message);

        rv = CORE_ERROR;
        goto out;
    }
    bson_destroy(update);

    update = BCON_NEW("$bit",
            "{",
                "security.sqn", 
                "{", "and", BCON_INT64(max_sqn), "}",
            "}");
    if (!mongoc_collection_update(self.subscriberCollection,
            MONGOC_UPDATE_NONE, query, update, NULL, &error))
    {
        d_error("mongoc_collection_update() failure: %s", error.message);

        rv = CORE_ERROR;
    }

out:
    if (query) bson_destroy(query);
    if (update) bson_destroy(update);

    mutex_unlock(self.db_lock);

    return rv;
}

status_t hss_db_subscription_data(
    char *imsi_bcd, hss_db_subscription_data_t *subscription_data)
{
    status_t rv = CORE_OK;
    mongoc_cursor_t *cursor = NULL;
    bson_t *query = NULL;
    bson_error_t error;
    const bson_t *document;
    bson_iter_t iter;
    bson_iter_t child1_iter, child2_iter, child3_iter, child4_iter;
    const char *utf8 = NULL;
    c_uint32_t length = 0;

    d_assert(imsi_bcd, return CORE_ERROR, "Null param");
    d_assert(subscription_data, return CORE_ERROR, "Null param");

    mutex_lock(self.db_lock);

    query = BCON_NEW("imsi", BCON_UTF8(imsi_bcd));
    cursor = mongoc_collection_find_with_opts(
            self.subscriberCollection, query, NULL, NULL);

    mongoc_cursor_next(cursor, &document);
    if (mongoc_cursor_error(cursor, &error))
    {
        d_error("Cursor Failure: %s", error.message);

        rv = CORE_ERROR;
        goto out;
    }

    if (!bson_iter_init(&iter, document))
    {
        d_error("bson_iter_init failed in this document");

        rv = CORE_ERROR;
        goto out;
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

                d_assert(child1_key, goto out, "PDN is not ARRAY");
                pdn_index = atoi(child1_key);
                d_assert(pdn_index < MAX_NUM_OF_PDN,
                        goto out, "Overflow of PDN number(%d>%d)",
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

out:
    if (query) bson_destroy(query);
    if (cursor) mongoc_cursor_destroy(cursor);

    mutex_unlock(self.db_lock);

    return rv;
}
