/*
 * Copyright (C) 2022 by sysmocom - s.f.m.c. GmbH <info@sysmocom.de>
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

#include "ogs-metrics.h"

#include <netdb.h> /* AI_PASSIVE */
#include "prom.h"
#include "microhttpd.h"

#define DEFAULT_PROMETHEUS_HTTP_PORT       9090
#define MAX_LABELS 8

typedef struct ogs_metrics_context_s {
    //uint32_t        port;      /* METRICS local port */

    //ogs_sock_t      *metricsc_sock;     /* METRICS IPv4 Socket */
    ogs_socknode_t node;

    ogs_list_t  spec_list;
    struct MHD_Daemon *mhd_server;
} ogs_metrics_context_t;

typedef struct ogs_metrics_spec_s {
    ogs_metrics_context_t       *ctx; /* backpointer */
    ogs_list_t                  entry; /* included in ogs_metrics_context_t */
    ogs_metrics_metric_type_t   type;
    char                        *name;
    char                        *description;
    int                         initial_val;
    ogs_list_t                  inst_list; /* list of ogs_metrics_instance_t */
    unsigned int                num_labels;
    char                        *labels[MAX_LABELS];
    prom_metric_t               *prom;
} ogs_metrics_spec_t;

typedef struct ogs_metrics_inst_s {
    ogs_metrics_spec_t      *spec;  /* backpointer */
    ogs_list_t              entry; /* included in ogs_metrics_spec_t spec */
    unsigned int            num_labels;
    char                    *label_values[MAX_LABELS];
} ogs_metrics_inst_t;

static ogs_metrics_context_t self;
static int context_initialized = 0;
static OGS_POOL(metrics_spec_pool, ogs_metrics_spec_t);

void ogs_metrics_context_init(void)
{
    ogs_assert(context_initialized == 0);

    ogs_log_install_domain(&__ogs_metrics_domain, "metrics", ogs_core()->log.level);

    ogs_pool_init(&metrics_spec_pool, ogs_app()->metrics.max_specs);

    /* Initialize METRICS context */
    memset(&self, 0, sizeof(ogs_metrics_context_t));
    ogs_list_init(&self.spec_list);
    prom_collector_registry_default_init();

    context_initialized = 1;
}

void ogs_metrics_context_final(void)
{
    ogs_metrics_spec_t *spec = NULL, *next = NULL;
    ogs_assert(context_initialized == 1);

    if (self.mhd_server)
        ogs_metrics_context_close(&self);

    ogs_list_for_each_entry_safe(&self.spec_list, next, spec, entry) {
        ogs_metrics_spec_free(spec);
    }
    prom_collector_registry_destroy(PROM_COLLECTOR_REGISTRY_DEFAULT);

    if (self.node.addr) {
        ogs_freeaddrinfo(self.node.addr);
        self.node.addr = NULL;
    }

    ogs_pool_final(&metrics_spec_pool);

    context_initialized = 0;
}

ogs_metrics_context_t *ogs_metrics_self(void)
{
    return &self;
}

int ogs_metrics_context_parse_config(const char *local)
{
    int family = AF_UNSPEC;
    const char *hostname = NULL;
    uint16_t port = DEFAULT_PROMETHEUS_HTTP_PORT;
    ogs_sockaddr_t *addr = NULL;
    yaml_document_t *document = NULL;
    ogs_yaml_iter_t root_iter;
    const char *v;

    document = ogs_app()->document;
    ogs_assert(document);

    ogs_yaml_iter_init(&root_iter, document);
    while (ogs_yaml_iter_next(&root_iter)) {
        const char *root_key = ogs_yaml_iter_key(&root_iter);
        ogs_assert(root_key);
        if (local && !strcmp(root_key, local)) {
            ogs_yaml_iter_t local_iter;
            ogs_yaml_iter_recurse(&root_iter, &local_iter);
            while (ogs_yaml_iter_next(&local_iter)) {
                const char *local_key = ogs_yaml_iter_key(&local_iter);
                ogs_assert(local_key);
                if (!strcmp(local_key, "metrics")) {
                    ogs_yaml_iter_t metrics_iter;
                    ogs_yaml_iter_recurse(&local_iter, &metrics_iter);
                    while (ogs_yaml_iter_next(&metrics_iter)) {
                        const char *metrics_key = ogs_yaml_iter_key(&metrics_iter);
                        ogs_assert(metrics_key);
                        if (!strcmp(metrics_key, "addr")) {
                            if ((v = ogs_yaml_iter_value(&metrics_iter)))
                                hostname = v;
                        } else if (!strcmp(metrics_key, "port")) {
                            if ((v = ogs_yaml_iter_value(&metrics_iter)))
                                port = atoi(v);
                        }
                    }
                }
            }
        }
    }
    ogs_assert(OGS_OK ==
       ogs_addaddrinfo(&addr, family, hostname, port, AI_PASSIVE));
    if (self.node.addr)
        ogs_freeaddrinfo(self.node.addr);
    ogs_assert(OGS_OK == ogs_copyaddrinfo(&self.node.addr, addr));
    ogs_freeaddrinfo(addr);
    return OGS_OK;
}

static void mhd_server_run(short when, ogs_socket_t fd, void *data)
{
    struct MHD_Daemon *mhd_daemon = data;

    ogs_assert(mhd_daemon);
    MHD_run(mhd_daemon);
}

static void mhd_server_notify_connection(void *cls,
        struct MHD_Connection *connection,
        void **socket_context,
        enum MHD_ConnectionNotificationCode toe)
{
    struct MHD_Daemon *mhd_daemon = NULL;
    MHD_socket mhd_socket = INVALID_SOCKET;

    const union MHD_ConnectionInfo *mhd_info = NULL;
    struct {
        ogs_poll_t *read;
    } poll;

    switch (toe) {
        case MHD_CONNECTION_NOTIFY_STARTED:
            mhd_info = MHD_get_connection_info(
                    connection, MHD_CONNECTION_INFO_DAEMON);
            ogs_assert(mhd_info);
            mhd_daemon = mhd_info->daemon;
            ogs_assert(mhd_daemon);

            mhd_info = MHD_get_connection_info(
                    connection, MHD_CONNECTION_INFO_CONNECTION_FD);
            ogs_assert(mhd_info);
            mhd_socket = mhd_info->connect_fd;
            ogs_assert(mhd_socket != INVALID_SOCKET);

            poll.read = ogs_pollset_add(ogs_app()->pollset,
                    OGS_POLLIN, mhd_socket, mhd_server_run, mhd_daemon);
            ogs_assert(poll.read);
            *socket_context = poll.read;
            break;
        case MHD_CONNECTION_NOTIFY_CLOSED:
            poll.read = *socket_context;
            if (poll.read)
                ogs_pollset_remove(poll.read);
            break;
    }
}

#if MHD_VERSION >= 0x00097001
typedef enum MHD_Result _MHD_Result;
#else
typedef int _MHD_Result;
#endif

static _MHD_Result mhd_server_access_handler(void *cls, struct MHD_Connection *connection,
        const char *url, const char *method, const char *version,
        const char *upload_data, size_t *upload_data_size, void **con_cls) {

    const char *buf;
    struct MHD_Response *rsp;
    int ret;

    if (strcmp(method, "GET") != 0) {
        buf = "Invalid HTTP Method\n";
        rsp = MHD_create_response_from_buffer(strlen(buf), (void *)buf, MHD_RESPMEM_PERSISTENT);
        ret = MHD_queue_response(connection, MHD_HTTP_BAD_REQUEST, rsp);
        MHD_destroy_response(rsp);
        return ret;
    }
    if (strcmp(url, "/") == 0) {
        buf = "OK\n";
        rsp = MHD_create_response_from_buffer(strlen(buf), (void *)buf, MHD_RESPMEM_PERSISTENT);
        ret = MHD_queue_response(connection, MHD_HTTP_OK, rsp);
        MHD_destroy_response(rsp);
        return ret;
    }
    if (strcmp(url, "/metrics") == 0) {
        buf = prom_collector_registry_bridge(PROM_COLLECTOR_REGISTRY_DEFAULT);
        rsp = MHD_create_response_from_buffer(strlen(buf), (void *)buf, MHD_RESPMEM_MUST_FREE);
        ret = MHD_queue_response(connection, MHD_HTTP_OK, rsp);
        MHD_destroy_response(rsp);
        return ret;
    }
    buf = "Bad Request\n";
    rsp = MHD_create_response_from_buffer(strlen(buf), (void *)buf, MHD_RESPMEM_PERSISTENT);
    ret = MHD_queue_response(connection, MHD_HTTP_BAD_REQUEST, rsp);
    MHD_destroy_response(rsp);
    return ret;
}

static int ogs_metrics_context_mhd_server_start(ogs_metrics_context_t *ctx)
{
#define MAX_NUM_OF_MHD_OPTION_ITEM 8
    struct MHD_OptionItem mhd_ops[MAX_NUM_OF_MHD_OPTION_ITEM];
    const union MHD_DaemonInfo *mhd_info = NULL;
    int index = 0;
    char buf[OGS_ADDRSTRLEN];
    ogs_sockaddr_t *addr = ctx->node.addr;
    char *hostname = NULL;

    ogs_assert(addr);

#if MHD_VERSION >= 0x00095300
    unsigned int mhd_flags = MHD_USE_ERROR_LOG;
#else
    unsigned int mhd_flags = MHD_USE_DEBUG;
#endif

#if MHD_VERSION >= 0x00095300
    mhd_flags |= MHD_ALLOW_SUSPEND_RESUME;
#elif MHD_VERSION >= 0x00093400
    mhd_flags |= MHD_USE_SUSPEND_RESUME;
#else
    mhd_flags |= MHD_USE_PIPE_FOR_SHUTDOWN;
#endif

    if (addr->ogs_sa_family == AF_INET6)
        mhd_flags |= MHD_USE_IPv6;

    mhd_ops[index].option = MHD_OPTION_NOTIFY_CONNECTION;
    mhd_ops[index].value = (intptr_t)&mhd_server_notify_connection;
    mhd_ops[index].ptr_value = NULL;
    index++;

    mhd_ops[index].option = MHD_OPTION_SOCK_ADDR;
    mhd_ops[index].value = 0;
    mhd_ops[index].ptr_value = (void *)&addr->sa;
    index++;

    mhd_ops[index].option = MHD_OPTION_END;
    mhd_ops[index].value = 0;
    mhd_ops[index].ptr_value = NULL;
    index++;

    if (ctx->mhd_server) {
        ogs_error("Prometheus HTTP server is already opened!");
        return OGS_ERROR;
    }

    ctx->mhd_server = MHD_start_daemon(
                mhd_flags,
                0,
                NULL, NULL,
                mhd_server_access_handler, ctx,
                MHD_OPTION_ARRAY, mhd_ops,
                MHD_OPTION_END);
    if (!ctx->mhd_server) {
        ogs_error("Cannot start Prometheus HTTP server");
        return OGS_ERROR;
    }

    /* Setup poll for server listening socket */
    mhd_info = MHD_get_daemon_info(ctx->mhd_server, MHD_DAEMON_INFO_LISTEN_FD);
    ogs_assert(mhd_info);

    ctx->node.poll = ogs_pollset_add(ogs_app()->pollset,
            OGS_POLLIN, mhd_info->listen_fd, mhd_server_run, ctx->mhd_server);
    ogs_assert(ctx->node.poll);

    hostname = ogs_gethostname(addr);
    if (hostname)
        ogs_info("Prometheus mhd_server() [%s]:%d", hostname, OGS_PORT(addr));
    else
        ogs_info("Prometheus mhd_server() [%s]:%d", OGS_ADDR(addr, buf), OGS_PORT(addr));
    return OGS_OK;
}
void ogs_metrics_context_open(ogs_metrics_context_t *ctx)
{
    ogs_assert(ogs_metrics_context_mhd_server_start(ctx) == OGS_OK);
}

static int ogs_metrics_context_mhd_server_stop(ogs_metrics_context_t *ctx)
{
    ogs_assert(ctx);

    if (ctx->node.poll)
        ogs_pollset_remove(ctx->node.poll);

    if (ctx->mhd_server) {
        MHD_stop_daemon(ctx->mhd_server);
        ctx->mhd_server = NULL;
    }
    return OGS_OK;
}
void ogs_metrics_context_close(ogs_metrics_context_t *ctx)
{
    ogs_assert(ogs_metrics_context_mhd_server_stop(ctx) == OGS_OK);
}

ogs_metrics_spec_t *ogs_metrics_spec_new(
        ogs_metrics_context_t *ctx, ogs_metrics_metric_type_t type,
        const char *name, const char *description,
        int initial_val, unsigned int num_labels, const char ** labels)
{
    ogs_metrics_spec_t *spec;
    unsigned int i;

    ogs_assert(name);
    ogs_assert(description);
    ogs_assert(num_labels <= MAX_LABELS);

    ogs_pool_alloc(&metrics_spec_pool, &spec);
    ogs_assert(spec);
    memset(spec, 0, sizeof *spec);
    spec->ctx = ctx;
    ogs_list_init(&spec->inst_list);
    spec->type = type;
    spec->name = ogs_strdup(name);
    spec->description = ogs_strdup(description);
    spec->initial_val = initial_val;
    spec->num_labels = num_labels;
    for (i = 0; i < num_labels; i++) {
        ogs_assert(labels[i]);
        spec->labels[i] = ogs_strdup(labels[i]);
    }

    switch (type) {
    case OGS_METRICS_METRIC_TYPE_COUNTER:
        spec->prom = prom_counter_new(spec->name, spec->description,
                                      spec->num_labels, (const char **)spec->labels);
        break;
    case OGS_METRICS_METRIC_TYPE_GAUGE:
        spec->prom = prom_gauge_new(spec->name, spec->description,
                                    spec->num_labels, (const char **)spec->labels);
        break;
    default:
        ogs_assert_if_reached();
        break;
    }
    prom_collector_registry_must_register_metric(spec->prom);

    ogs_list_add(&ctx->spec_list, &spec->entry);
    return spec;
}

void ogs_metrics_spec_free(ogs_metrics_spec_t *spec)
{
    ogs_metrics_inst_t *inst = NULL, *next = NULL;
    unsigned int i;

    ogs_list_remove(&spec->ctx->spec_list, spec);

    ogs_list_for_each_entry_safe(&spec->inst_list, next, inst, entry) {
        ogs_metrics_inst_free(inst);
    }

    ogs_free(spec->name);
    ogs_free(spec->description);
    for (i = 0; i < spec->num_labels; i++)
        ogs_free(spec->labels[i]);

    ogs_pool_free(&metrics_spec_pool, spec);
}


ogs_metrics_inst_t *ogs_metrics_inst_new(
        ogs_metrics_spec_t *spec,
        unsigned int num_labels, const char **label_values)
{
    ogs_metrics_inst_t *inst;
    unsigned int i;

    ogs_assert(spec);
    ogs_assert(num_labels == spec->num_labels);

    inst = ogs_calloc(1, sizeof(ogs_metrics_inst_t));
    ogs_assert(inst);
    inst->spec = spec;
    inst->num_labels = num_labels;
    for (i = 0; i < num_labels; i++) {
        ogs_assert(label_values[i]);
        inst->label_values[i] = ogs_strdup(label_values[i]);
    }
    ogs_list_add(&spec->inst_list, &inst->entry);
    ogs_metrics_inst_reset(inst);
    return inst;
}

void ogs_metrics_inst_free(ogs_metrics_inst_t *inst)
{
    unsigned int i;

    ogs_list_remove(&inst->spec->inst_list, &inst->entry);

    for (i = 0; i < inst->num_labels; i++)
        ogs_free(inst->label_values[i]);

    ogs_free(inst);
}

void ogs_metrics_inst_set(ogs_metrics_inst_t *inst, int val)
{
    switch (inst->spec->type) {
    case OGS_METRICS_METRIC_TYPE_GAUGE:
        prom_gauge_set(inst->spec->prom, (double)val, (const char **)inst->label_values);
        break;
    default:
        ogs_assert_if_reached();
        break;
    }
}

void ogs_metrics_inst_reset(ogs_metrics_inst_t *inst)
{
    switch (inst->spec->type) {
    case OGS_METRICS_METRIC_TYPE_COUNTER:
        prom_counter_add(inst->spec->prom, 0.0, (const char **)inst->label_values);
        break;
    case OGS_METRICS_METRIC_TYPE_GAUGE:
        prom_gauge_set(inst->spec->prom, (double)inst->spec->initial_val, (const char **)inst->label_values);
        break;
    default:
        /* Other types have no way to reset */
        break;
    }
}

void ogs_metrics_inst_add(ogs_metrics_inst_t *inst, int val)
{
    switch (inst->spec->type) {
    case OGS_METRICS_METRIC_TYPE_COUNTER:
        ogs_assert(val >= 0);
        prom_counter_add(inst->spec->prom, (double)val, (const char **)inst->label_values);
        break;
    case OGS_METRICS_METRIC_TYPE_GAUGE:
        if (val >= 0)
            prom_gauge_add(inst->spec->prom, (double)val, (const char **)inst->label_values);
        else
            prom_gauge_sub(inst->spec->prom, (double)-1.0*(double)val, (const char **)inst->label_values);
        break;
    default:
        ogs_assert_if_reached();
        break;
    }
}
