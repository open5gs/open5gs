#define TRACE_MODULE _pcrf_context

#include "core_debug.h"
#include "core_lib.h"

#include <mongoc.h>

#include "fd_lib.h"

#include "context.h"
#include "pcrf_context.h"

static pcrf_context_t self;
static int context_initialized = 0;

pcrf_context_t* pcrf_self()
{
    return &self;
}

status_t pcrf_context_init(void)
{
    d_assert(context_initialized == 0, return CORE_ERROR,
            "PCRF context already has been context_initialized");

    /* Initialize PCRF context */
    memset(&self, 0, sizeof(pcrf_context_t));

    if (mutex_create(&self.db_lock, MUTEX_DEFAULT) != CORE_OK)
    {
        d_error("Mutex creation failed");
        return CORE_ERROR;
    }

    context_initialized = 1;

	return CORE_OK;
}

status_t pcrf_context_final(void)
{
    d_assert(context_initialized == 1, return CORE_ERROR,
            "PCRF context already has been finalized");

    mutex_delete(self.db_lock);

    context_initialized = 0;

	return CORE_OK;
}

static status_t pcrf_context_prepare()
{
    return CORE_OK;
}

static status_t pcrf_context_validation()
{
    if (self.fd_conf_path == NULL)
    {
        d_error("No PCRF.FD_CONF_PATH in '%s'",
                context_self()->config.path);
        return CORE_ERROR;
    }

    return CORE_OK;
}

status_t pcrf_context_parse_config()
{
    status_t rv;
    config_t *config = &context_self()->config;

    char *json = config->json;
    jsmntok_t *token = config->token;

    typedef enum {
        START, ROOT,
        PCRF_START, PCRF_ROOT,
        SKIP, STOP
    } parse_state;
    parse_state state = START;
    parse_state stack = STOP;

    size_t root_tokens = 0;
    size_t pcrf_tokens = 0;
    size_t skip_tokens = 0;
    int i, j;

    rv = pcrf_context_prepare();
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
                if (jsmntok_equal(json, t, "PCRF") == 0)
                {
                    state = PCRF_START;
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
            case PCRF_START:
            {
                state = PCRF_ROOT;
                pcrf_tokens = t->size;

                break;
            }
            case PCRF_ROOT:
            {
                if (jsmntok_equal(json, t, "FD_CONF_PATH") == 0)
                {
                    self.fd_conf_path = jsmntok_to_string(json, t+1);
                }

                state = SKIP;
                stack = PCRF_ROOT;
                skip_tokens = t->size;

                pcrf_tokens--;
                if (pcrf_tokens == 0) stack = ROOT;
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

    rv = pcrf_context_validation();
    if (rv != CORE_OK) return rv;

    return CORE_OK;
}

status_t pcrf_context_setup_trace_module()
{
    int fd = context_self()->trace_level.fd;
    int others = context_self()->trace_level.others;

    if (fd)
    {
        if (fd <= 1) fd_g_debug_lvl = FD_LOG_ERROR;
        else if (fd <= 3) fd_g_debug_lvl = FD_LOG_NOTICE;
        else if (fd <= 5) fd_g_debug_lvl = FD_LOG_DEBUG;
        else fd_g_debug_lvl = FD_LOG_ANNOYING;

        extern int _pcrf_fd_path;
        d_trace_level(&_pcrf_fd_path, fd);
        extern int _fd_init;
        d_trace_level(&_fd_init, fd);
        extern int _fd_logger;
        d_trace_level(&_fd_logger, fd);
    }

    if (others)
    {
        extern int _mutex;
        d_trace_level(&_mutex, others);
        extern int _pkbuf;
        d_trace_level(&_pkbuf, others);

        extern int _context;
        d_trace_level(&_context, others);
        extern int _pcrf_context;
        d_trace_level(&_pcrf_context, others);
    }

    return CORE_OK;
}

status_t pcrf_db_init()
{
    if (context_self()->db_client && context_self()->db_name)
    {
        self.subscriberCollection = mongoc_client_get_collection(
            context_self()->db_client, 
            context_self()->db_name, "subscribers");
        d_assert(self.subscriberCollection, return CORE_ERROR, 
            "Couldn't find Subscriber Collection in '%s'",
            context_self()->db_name);
#if 0
        {
        gx_pdn_data_t pdn_data;
        pcrf_db_pdn_data("123", "internet", &pdn_data);
        }
#endif
    }

    return CORE_OK;
}

status_t pcrf_db_final()
{
    if (self.subscriberCollection)
    {
        mongoc_collection_destroy(self.subscriberCollection);
    }

    return CORE_OK;
}

status_t pcrf_db_pdn_data(
        c_int8_t *imsi_bcd, c_int8_t *apn, gx_pdn_data_t *pdn_data)
{
    status_t rv = CORE_OK;
    mongoc_cursor_t *cursor = NULL;
    bson_t *query = NULL;
    bson_t *opts = NULL;
    bson_error_t error;
    const bson_t *document;
    bson_iter_t iter;
    bson_iter_t child1_iter, child2_iter, child3_iter;
    bson_iter_t child4_iter, child5_iter, child6_iter;
    const char *utf8 = NULL;
    c_uint32_t length = 0;

    d_assert(imsi_bcd, return CORE_ERROR, "Null param");
    d_assert(apn, return CORE_ERROR, "Null param");
    d_assert(pdn_data, return CORE_ERROR, "Null param");

    mutex_lock(self.db_lock);

    query = BCON_NEW(
            "imsi", BCON_UTF8(imsi_bcd),
            "pdn.apn", BCON_UTF8(apn));
    opts = BCON_NEW(
            "projection", "{",
                "imsi", BCON_INT64(1),
                "pdn.$", BCON_INT64(1),
            "}"
            );
    cursor = mongoc_collection_find_with_opts(
            self.subscriberCollection, query, opts, NULL);

    if (!mongoc_cursor_next(cursor, &document))
    {
        d_error("Cannot find IMSI(%s)+APN(%s) in DB", imsi_bcd, apn);

        rv = CORE_ERROR;
        goto out;
    }

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

    memset(pdn_data, 0, sizeof(gx_pdn_data_t));
    while(bson_iter_next(&iter))
    {
        const char *key = bson_iter_key(&iter);
        if (!strcmp(key, "pdn") &&
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
                d_assert(pdn_index == 0, goto out, 
                        "Invalid PDN Index(%d)", pdn_index);

                pdn = &pdn_data->pdn;
                bson_iter_recurse(&child1_iter, &child2_iter);
                while(bson_iter_next(&child2_iter))
                {
                    const char *child2_key = bson_iter_key(&child2_iter);
                    if (!strcmp(child2_key, "apn") &&
                        BSON_ITER_HOLDS_UTF8(&child2_iter))
                    {
                        utf8 = bson_iter_utf8(&child2_iter, &length);
                        core_cpystrn(pdn->apn, utf8,
                            c_min(length, MAX_APN_LEN)+1);
                    }
                    else if (!strcmp(child2_key, "type") &&
                        BSON_ITER_HOLDS_INT32(&child2_iter))
                    {
                        pdn->pdn_type = bson_iter_int32(&child2_iter);
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
                                pdn->qos.qci = bson_iter_int32(&child3_iter);
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
                                        pdn->qos.arp.priority_level =
                                            bson_iter_int32(&child4_iter);
                                    }
                                    else if (!strcmp(child4_key,
                                                "pre_emption_capability") &&
                                        BSON_ITER_HOLDS_INT32(&child4_iter))
                                    {
                                        pdn->qos.arp.pre_emption_capability =
                                            bson_iter_int32(&child4_iter);
                                    }
                                    else if (!strcmp(child4_key,
                                                "pre_emption_vulnerability") &&
                                        BSON_ITER_HOLDS_INT32(&child4_iter))
                                    {
                                        pdn->qos.arp.pre_emption_vulnerability =
                                            bson_iter_int32(&child4_iter);
                                    }
                                }
                            }
                        }
                    }
                    else if (!strcmp(child2_key, "ambr") &&
                        BSON_ITER_HOLDS_DOCUMENT(&child2_iter))
                    {
                        bson_iter_recurse(&child2_iter, &child3_iter);
                        while(bson_iter_next(&child3_iter))
                        {
                            const char *child3_key =
                                bson_iter_key(&child3_iter);
                            if (!strcmp(child3_key, "uplink") &&
                                BSON_ITER_HOLDS_INT64(&child3_iter))
                            {
                                pdn->ambr.uplink =
                                    bson_iter_int64(&child3_iter) * 1024;
                            }
                            else if (!strcmp(child3_key, "downlink") &&
                                BSON_ITER_HOLDS_INT64(&child3_iter))
                            {
                                pdn->ambr.downlink =
                                    bson_iter_int64(&child3_iter) * 1024;
                            }
                        }
                    }
                    else if (!strcmp(child2_key, "pcc_rule") &&
                        BSON_ITER_HOLDS_ARRAY(&child2_iter))
                    {
                        int pcc_rule_index = 0;

                        bson_iter_recurse(&child2_iter, &child3_iter);
                        while(bson_iter_next(&child3_iter))
                        {
                            const char *child3_key =
                                bson_iter_key(&child3_iter);
                            pcc_rule_t *pcc_rule = NULL;

                            d_assert(child3_key, goto out,
                                    "PCC RULE is not ARRAY");
                            pcc_rule_index = atoi(child3_key);
                            d_assert(pcc_rule_index < MAX_NUM_OF_PCC_RULE,
                                    goto out,
                                    "Overflow of PCC RULE number(%d>%d)",
                                    pcc_rule_index, MAX_NUM_OF_PCC_RULE);

                            pcc_rule = &pdn_data->pcc_rule[pcc_rule_index];
                            bson_iter_recurse(&child3_iter, &child4_iter);
                            while(bson_iter_next(&child4_iter))
                            {
                                const char *child4_key =
                                    bson_iter_key(&child4_iter);

                                if (!strcmp(child4_key, "qos") &&
                                    BSON_ITER_HOLDS_DOCUMENT(&child4_iter))
                                {
                                    bson_iter_recurse(
                                            &child4_iter, &child5_iter);
                                    while(bson_iter_next(&child5_iter))
                                    {
                                        const char *child5_key =
                                            bson_iter_key(&child5_iter);
                                        if (!strcmp(child5_key, "qci") &&
                                            BSON_ITER_HOLDS_INT32(&child5_iter))
                                        {
                                            pcc_rule->qos.qci =
                                                bson_iter_int32(&child5_iter);
                                        }
                                        else if (!strcmp(child5_key, "arp") &&
                                            BSON_ITER_HOLDS_DOCUMENT(
                                                &child5_iter))
                                        {
                                            bson_iter_recurse(
                                                &child5_iter, &child6_iter);
                                            while(bson_iter_next(&child6_iter))
                                            {
                                                const char *child6_key =
                                                    bson_iter_key(&child6_iter);
                                                if (!strcmp(child6_key,
                                                        "priority_level") &&
                                                    BSON_ITER_HOLDS_INT32(
                                                        &child6_iter))
                                                {
                                                    pcc_rule->qos.arp.
                                                        priority_level =
                                                        bson_iter_int32(
                                                            &child6_iter);
                                                }
                                                else if (!strcmp(child6_key,
                                                    "pre_emption_capability") &&
                                                    BSON_ITER_HOLDS_INT32(
                                                        &child6_iter))
                                                {
                                                    pcc_rule->qos.arp.
                                                    pre_emption_capability =
                                                        bson_iter_int32(
                                                            &child6_iter);
                                                }
                                                else if (!strcmp(child6_key,
                                                            "pre_emption_vulnerability") &&
                                                    BSON_ITER_HOLDS_INT32(&child6_iter))
                                                {
                                                    pcc_rule->qos.arp.
                                                    pre_emption_vulnerability =
                                                        bson_iter_int32(
                                                            &child6_iter);
                                                }
                                            }
                                        }
                                        else if (!strcmp(child5_key, "mbr") &&
                                            BSON_ITER_HOLDS_DOCUMENT(
                                                &child5_iter))
                                        {
                                            bson_iter_recurse(
                                                &child5_iter, &child6_iter);
                                            while(bson_iter_next(&child6_iter))
                                            {
                                                const char *child6_key =
                                                    bson_iter_key(&child6_iter);
                                                if (!strcmp(child6_key,
                                                        "downlink") &&
                                                    BSON_ITER_HOLDS_INT64(
                                                        &child6_iter))
                                                {
                                                    pcc_rule->qos.mbr.downlink =
                                                        bson_iter_int64(
                                                            &child6_iter);
                                                }
                                                else if (!strcmp(child6_key,
                                                        "uplink") &&
                                                    BSON_ITER_HOLDS_INT64(
                                                        &child6_iter))
                                                {
                                                    pcc_rule->qos.mbr.uplink =
                                                        bson_iter_int64(
                                                            &child6_iter);
                                                }
                                            }
                                        }
                                        else if (!strcmp(child5_key, "gbr") &&
                                            BSON_ITER_HOLDS_DOCUMENT(
                                                &child5_iter))
                                        {
                                            bson_iter_recurse(&child5_iter,
                                                &child6_iter);
                                            while(bson_iter_next(&child6_iter))
                                            {
                                                const char *child6_key =
                                                    bson_iter_key(&child6_iter);
                                                if (!strcmp(child6_key,
                                                        "downlink") &&
                                                    BSON_ITER_HOLDS_INT64(
                                                        &child6_iter))
                                                {
                                                    pcc_rule->qos.gbr.downlink =
                                                        bson_iter_int64(
                                                            &child6_iter);
                                                }
                                                else if (!strcmp(child6_key,
                                                        "uplink") &&
                                                    BSON_ITER_HOLDS_INT64(
                                                        &child6_iter))
                                                {
                                                    pcc_rule->qos.gbr.uplink =
                                                        bson_iter_int64(
                                                            &child6_iter);
                                                }
                                            }
                                        }
                                    }
                                }
                                else if (!strcmp(child4_key, "flow") &&
                                    BSON_ITER_HOLDS_ARRAY(&child4_iter))
                                {
                                    int flow_index = 0;

                                    bson_iter_recurse(&child4_iter,
                                        &child5_iter);
                                    while(bson_iter_next(&child5_iter))
                                    {
                                        const char *child5_key =
                                            bson_iter_key(&child5_iter);
                                        flow_t *flow = NULL;

                                        d_assert(child5_key, goto out,
                                                "FLOW is not ARRAY");
                                        flow_index = atoi(child5_key);
                                        d_assert(flow_index < MAX_NUM_OF_FLOW,
                                            goto out,
                                            "Overflow of FLOW number(%d>%d)",
                                            flow_index, MAX_NUM_OF_FLOW);

                                        flow = &pcc_rule->flow[flow_index];
                                        bson_iter_recurse(
                                            &child5_iter, &child6_iter);
                                        while(bson_iter_next(&child6_iter))
                                        {
                                            const char *child6_key =
                                                bson_iter_key(&child6_iter);
                                            if (!strcmp(child6_key, "direction") &&
                                                BSON_ITER_HOLDS_INT32(
                                                    &child6_iter))
                                            {
                                                flow->direction =
                                                    bson_iter_int32(
                                                        &child6_iter);
                                            }
                                            else if (!strcmp(child6_key,
                                                        "description") &&
                                                BSON_ITER_HOLDS_UTF8(
                                                    &child6_iter))
                                            {
                                                utf8 = bson_iter_utf8(
                                                        &child6_iter, &length);
                                                core_cpystrn(
                                                (char*)flow->description,
                                                utf8,
                                                c_min(length,
                                                MAX_FLOW_DESCRIPTION_LEN)+1);
                                            }
                                        }
                                        flow_index++;
                                    }
                                    pcc_rule->num_of_flow = flow_index;
                                }
                            }
                            pcc_rule_index++;
                        }
                        pdn_data->num_of_pcc_rule = pcc_rule_index;
                    }
                }
            }
        }
    }

out:
    if (query) bson_destroy(query);
    if (cursor) mongoc_cursor_destroy(cursor);

    mutex_unlock(self.db_lock);

    return rv;
}
