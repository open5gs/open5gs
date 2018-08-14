#define TRACE_MODULE _pcrf_context

#include "core_debug.h"
#include "core_pkbuf.h"
#include "core_lib.h"

#include <mongoc.h>
#include <yaml.h>
#include "common/yaml_helper.h"

#include "fd/fd_lib.h"

#include "common/context.h"
#include "pcrf_context.h"

static pcrf_context_t self;
static fd_config_t g_fd_conf;

static int context_initialized = 0;

pcrf_context_t* pcrf_self()
{
    return &self;
}

status_t pcrf_context_init(void)
{
    status_t rv;
    d_assert(context_initialized == 0, return CORE_ERROR,
            "PCRF context already has been context_initialized");

    /* Initial FreeDiameter Config */
    memset(&g_fd_conf, 0, sizeof(fd_config_t));

    /* Initialize PCRF context */
    memset(&self, 0, sizeof(pcrf_context_t));
    self.fd_config = &g_fd_conf;

    rv = mutex_create(&self.db_lock, MUTEX_DEFAULT);
    d_assert(rv == CORE_OK, return CORE_ERROR, );

    rv = mutex_create(&self.hash_lock, MUTEX_DEFAULT);
    d_assert(rv == CORE_OK, return CORE_ERROR, );
    self.ip_hash = hash_make();

    context_initialized = 1;

	return CORE_OK;
}

status_t pcrf_context_final(void)
{
    d_assert(context_initialized == 1, return CORE_ERROR,
            "PCRF context already has been finalized");

    d_assert(self.ip_hash,, );
    hash_destroy(self.ip_hash);
    mutex_delete(self.hash_lock);

    mutex_delete(self.db_lock);

    context_initialized = 0;

	return CORE_OK;
}

static status_t pcrf_context_prepare()
{
    self.fd_config->cnf_port = DIAMETER_PORT;
    self.fd_config->cnf_port_tls = DIAMETER_SECURE_PORT;
    
    return CORE_OK;
}

static status_t pcrf_context_validation()
{
    if (self.fd_conf_path == NULL &&
        (self.fd_config->cnf_diamid == NULL ||
        self.fd_config->cnf_diamrlm == NULL ||
        self.fd_config->cnf_addr == NULL))
    {
        d_error("No pcrf.freeDiameter in '%s'",
                context_self()->config.path);
        return CORE_ERROR;
    }

    return CORE_OK;
}

status_t pcrf_context_parse_config()
{
    status_t rv;
    config_t *config = &context_self()->config;
    yaml_document_t *document = NULL;
    yaml_iter_t root_iter;

    d_assert(config, return CORE_ERROR,);
    document = config->document;
    d_assert(document, return CORE_ERROR,);

    rv = pcrf_context_prepare();
    if (rv != CORE_OK) return rv;

    yaml_iter_init(&root_iter, document);
    while(yaml_iter_next(&root_iter))
    {
        const char *root_key = yaml_iter_key(&root_iter);
        d_assert(root_key, return CORE_ERROR,);
        if (!strcmp(root_key, "pcrf"))
        {
            yaml_iter_t pcrf_iter;
            yaml_iter_recurse(&root_iter, &pcrf_iter);
            while(yaml_iter_next(&pcrf_iter))
            {
                const char *pcrf_key = yaml_iter_key(&pcrf_iter);
                d_assert(pcrf_key, return CORE_ERROR,);
                if (!strcmp(pcrf_key, "freeDiameter"))
                {
                    yaml_node_t *node = 
                        yaml_document_get_node(document, pcrf_iter.pair->value);
                    d_assert(node, return CORE_ERROR,);
                    if (node->type == YAML_SCALAR_NODE)
                    {
                        self.fd_conf_path = yaml_iter_value(&pcrf_iter);
                    }
                    else if (node->type == YAML_MAPPING_NODE)
                    {
                        yaml_iter_t fd_iter;
                        yaml_iter_recurse(&pcrf_iter, &fd_iter);

                        while(yaml_iter_next(&fd_iter))
                        {
                            const char *fd_key = yaml_iter_key(&fd_iter);
                            d_assert(fd_key, return CORE_ERROR,);
                            if (!strcmp(fd_key, "identity"))
                            {
                                self.fd_config->cnf_diamid = 
                                    yaml_iter_value(&fd_iter);
                            }
                            else if (!strcmp(fd_key, "realm"))
                            {
                                self.fd_config->cnf_diamrlm = 
                                    yaml_iter_value(&fd_iter);
                            }
                            else if (!strcmp(fd_key, "port"))
                            {
                                const char *v = yaml_iter_value(&fd_iter);
                                if (v) self.fd_config->cnf_port = atoi(v);
                            }
                            else if (!strcmp(fd_key, "sec_port"))
                            {
                                const char *v = yaml_iter_value(&fd_iter);
                                if (v) self.fd_config->cnf_port_tls = atoi(v);
                            }
                            else if (!strcmp(fd_key, "no_sctp"))
                            {
                                self.fd_config->cnf_flags.no_sctp =
                                    yaml_iter_bool(&fd_iter);
                            }
                            else if (!strcmp(fd_key, "listen_on"))
                            {
                                self.fd_config->cnf_addr = 
                                    yaml_iter_value(&fd_iter);
                            }
                            else if (!strcmp(fd_key, "load_extension"))
                            {
                                yaml_iter_t ext_array, ext_iter;
                                yaml_iter_recurse(&fd_iter, &ext_array);
                                do
                                {
                                    const char *module = NULL;
                                    const char *conf = NULL;

                                    if (yaml_iter_type(&ext_array) ==
                                        YAML_MAPPING_NODE)
                                    {
                                        memcpy(&ext_iter, &ext_array,
                                                sizeof(yaml_iter_t));
                                    }
                                    else if (yaml_iter_type(&ext_array) ==
                                        YAML_SEQUENCE_NODE)
                                    {
                                        if (!yaml_iter_next(&ext_array))
                                            break;
                                        yaml_iter_recurse(
                                                &ext_array, &ext_iter);
                                    }
                                    else if (yaml_iter_type(&ext_array) ==
                                        YAML_SCALAR_NODE)
                                    {
                                        break;
                                    }
                                    else
                                        d_assert(0, return CORE_ERROR,);

                                    while(yaml_iter_next(&ext_iter))
                                    {
                                        const char *ext_key =
                                            yaml_iter_key(&ext_iter);
                                        d_assert(ext_key,
                                                return CORE_ERROR,);
                                        if (!strcmp(ext_key, "module"))
                                        {
                                            module = yaml_iter_value(&ext_iter);
                                        }
                                        else if (!strcmp(ext_key, "conf"))
                                        {
                                            conf = yaml_iter_value(&ext_iter);
                                        }
                                        else
                                            d_warn("unknown key `%s`", ext_key);
                                    }

                                    if (module)
                                    {
                                        self.fd_config->
                                            ext[self.fd_config->num_of_ext].
                                                module = module;
                                        self.fd_config->
                                            ext[self.fd_config->num_of_ext].
                                                conf = conf;
                                        self.fd_config->num_of_ext++;
                                    }
                                } while(yaml_iter_type(&ext_array) ==
                                        YAML_SEQUENCE_NODE);
                            }
                            else if (!strcmp(fd_key, "connect"))
                            {
                                yaml_iter_t conn_array, conn_iter;
                                yaml_iter_recurse(&fd_iter, &conn_array);
                                do
                                {
                                    const char *identity = NULL;
                                    const char *addr = NULL;
                                    c_uint16_t port = 0;

                                    if (yaml_iter_type(&conn_array) ==
                                        YAML_MAPPING_NODE)
                                    {
                                        memcpy(&conn_iter, &conn_array,
                                                sizeof(yaml_iter_t));
                                    }
                                    else if (yaml_iter_type(&conn_array) ==
                                        YAML_SEQUENCE_NODE)
                                    {
                                        if (!yaml_iter_next(&conn_array))
                                            break;
                                        yaml_iter_recurse(&conn_array, &conn_iter);
                                    }
                                    else if (yaml_iter_type(&conn_array) ==
                                        YAML_SCALAR_NODE)
                                    {
                                        break;
                                    }
                                    else
                                        d_assert(0, return CORE_ERROR,);

                                    while(yaml_iter_next(&conn_iter))
                                    {
                                        const char *conn_key =
                                            yaml_iter_key(&conn_iter);
                                        d_assert(conn_key,
                                                return CORE_ERROR,);
                                        if (!strcmp(conn_key, "identity"))
                                        {
                                            identity = yaml_iter_value(&conn_iter);
                                        }
                                        else if (!strcmp(conn_key, "addr"))
                                        {
                                            addr = yaml_iter_value(&conn_iter);
                                        }
                                        else if (!strcmp(conn_key, "port"))
                                        {
                                            const char *v =
                                                yaml_iter_value(&conn_iter);
                                            if (v) port = atoi(v);
                                        }
                                        else
                                            d_warn("unknown key `%s`", conn_key);
                                    }

                                    if (identity && addr)
                                    {
                                        self.fd_config->
                                            conn[self.fd_config->num_of_conn].
                                                identity = identity;
                                        self.fd_config->
                                            conn[self.fd_config->num_of_conn].
                                                addr = addr;
                                        self.fd_config->
                                            conn[self.fd_config->num_of_conn].
                                                port = port;
                                        self.fd_config->num_of_conn++;
                                    }
                                } while(yaml_iter_type(&conn_array) ==
                                        YAML_SEQUENCE_NODE);
                            }
                            else
                                d_warn("unknown key `%s`", fd_key);
                        }
                    }
                }
                else
                    d_warn("unknown key `%s`", pcrf_key);
            }
        }
    }

    rv = pcrf_context_validation();
    if (rv != CORE_OK) return rv;

    return CORE_OK;
}

status_t pcrf_context_setup_trace_module()
{
    int app = context_self()->logger.trace.app;
    int diameter = context_self()->logger.trace.diameter;

    if (app)
    {
        extern int _pcrf_context;
        d_trace_level(&_pcrf_context, app);
    }

    if (diameter)
    {
        extern int _pcrf_fd_path;
        d_trace_level(&_pcrf_fd_path, diameter);
        extern int _pcrf_gx_path;
        d_trace_level(&_pcrf_gx_path, diameter);
        extern int _pcrf_rx_path;
        d_trace_level(&_pcrf_rx_path, diameter);
        extern int _fd_init;
        d_trace_level(&_fd_init, diameter);
        extern int _fd_logger;
        d_trace_level(&_fd_logger, diameter);
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

status_t pcrf_db_qos_data(
    c_int8_t *imsi_bcd, c_int8_t *apn, gx_message_t *gx_message)
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
    d_assert(gx_message, return CORE_ERROR, "Null param");

    mutex_lock(self.db_lock);

    query = BCON_NEW(
            "imsi", BCON_UTF8(imsi_bcd),
            "pdn.apn", BCON_UTF8(apn));
#if MONGOC_MAJOR_VERSION >= 1 && MONGOC_MINOR_VERSION >= 5
    opts = BCON_NEW(
            "projection", "{",
                "imsi", BCON_INT64(1),
                "pdn.$", BCON_INT64(1),
            "}"
            );
    cursor = mongoc_collection_find_with_opts(
            self.subscriberCollection, query, opts, NULL);
#else
    opts = BCON_NEW(
            "imsi", BCON_INT64(1),
            "pdn.$", BCON_INT64(1)
            );
    cursor = mongoc_collection_find(self.subscriberCollection,
            MONGOC_QUERY_NONE, 0, 0, 0, query, opts, NULL);
#endif

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

                pdn = &gx_message->pdn;
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

                            pcc_rule = &gx_message->pcc_rule[pcc_rule_index];
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
                                                        &child6_iter) * 1024;
                                                }
                                                else if (!strcmp(child6_key,
                                                        "uplink") &&
                                                    BSON_ITER_HOLDS_INT64(
                                                        &child6_iter))
                                                {
                                                    pcc_rule->qos.mbr.uplink =
                                                        bson_iter_int64(
                                                        &child6_iter) * 1024;
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
                                                        &child6_iter) * 1024;
                                                }
                                                else if (!strcmp(child6_key,
                                                        "uplink") &&
                                                    BSON_ITER_HOLDS_INT64(
                                                        &child6_iter))
                                                {
                                                    pcc_rule->qos.gbr.uplink =
                                                        bson_iter_int64(
                                                        &child6_iter) * 1024;
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
                                                flow->description =
                                                    core_malloc(length+1);
                                                core_cpystrn(
                                                    (char*)flow->description,
                                                    utf8, length+1);
                                            }
                                        }
                                        flow_index++;
                                    }
                                    pcc_rule->num_of_flow = flow_index;
                                }
                            }
                            /* Charing-Rule-Name is automatically configured */
                            if (pcc_rule->name)
                            {
                                d_error("PCC Rule Name has already "
                                        "been defined");
                                CORE_FREE(pcc_rule->name);
                            }
                            pcc_rule->name = core_calloc(
                                    1, MAX_PCC_RULE_NAME_LEN);
                            d_assert(pcc_rule->name, goto out,);
                            snprintf(pcc_rule->name, MAX_PCC_RULE_NAME_LEN,
                                    "%s%d", apn, pcc_rule_index+1);
                            pcc_rule->precedence = pcc_rule_index+1;
                            pcc_rule->flow_status = FLOW_STATUS_ENABLED;
                            pcc_rule_index++;
                        }
                        gx_message->num_of_pcc_rule = pcc_rule_index;
                    }
                }
            }
        }
    }

out:
    if (query) bson_destroy(query);
    if (opts) bson_destroy(opts);
    if (cursor) mongoc_cursor_destroy(cursor);

    mutex_unlock(self.db_lock);

    return rv;
}

status_t pcrf_sess_set_ipv4(const void *key, c_uint8_t *sid)
{
    d_assert(self.ip_hash, return CORE_ERROR,);

    mutex_lock(self.hash_lock);

    hash_set(self.ip_hash, key, IPV4_LEN, sid);

    mutex_unlock(self.hash_lock);

    return CORE_OK;
}
status_t pcrf_sess_set_ipv6(const void *key, c_uint8_t *sid)
{
    d_assert(self.ip_hash, return CORE_ERROR,);

    mutex_lock(self.hash_lock);

    hash_set(self.ip_hash, key, IPV6_LEN, sid);

    mutex_unlock(self.hash_lock);

    return CORE_OK;
}

c_uint8_t *pcrf_sess_find_by_ipv4(const void *key)
{
    c_uint8_t *sid = NULL;
    d_assert(key, return NULL,);

    mutex_lock(self.hash_lock);

    sid = (c_uint8_t *)hash_get(self.ip_hash, key, IPV4_LEN);

    mutex_unlock(self.hash_lock);
    
    return sid;
}

c_uint8_t *pcrf_sess_find_by_ipv6(const void *key)
{
    c_uint8_t *sid = NULL;
    d_assert(key, return NULL,);

    mutex_lock(self.hash_lock);

    sid = (c_uint8_t *)hash_get(self.ip_hash, key, IPV6_LEN);

    mutex_unlock(self.hash_lock);

    return sid;
}

