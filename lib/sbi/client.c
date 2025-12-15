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

#include "ogs-sbi.h"

#include "curl/curl.h"

typedef struct sockinfo_s {
    ogs_poll_t *poll;
    curl_socket_t sockfd;
    int action;
    CURL *easy;
    ogs_sbi_client_t *client;
} sockinfo_t;

typedef struct connection_s {
    ogs_lnode_t lnode;

    void *data;

    char *method;

    int num_of_header;
    char **headers;
    struct curl_slist *header_list;
    struct curl_slist *resolve_list;

    char *content;

    char *memory;
    size_t size;
    bool memory_overflow;

    char *location;
    char *producer_id;

    ogs_timer_t *timer;
    CURL *easy;

    char error[CURL_ERROR_SIZE];

    ogs_sbi_client_t *client;
    ogs_sbi_client_cb_f client_cb;
} connection_t;

static OGS_POOL(client_pool, ogs_sbi_client_t);
static OGS_POOL(sockinfo_pool, sockinfo_t);
static OGS_POOL(connection_pool, connection_t);

static size_t write_cb(void *contents, size_t size, size_t nmemb, void *data);
static size_t header_cb(void *ptr, size_t size, size_t nmemb, void *data);
static int sock_cb(CURL *e, curl_socket_t s, int what, void *cbp, void *sockp);
static int multi_timer_cb(CURLM *multi, long timeout_ms, void *cbp);
static void multi_timer_expired(void *data);

static connection_t *connection_add(
        ogs_sbi_client_t *client, ogs_sbi_client_cb_f client_cb,
        ogs_sbi_request_t *request, void *data);
static void connection_remove(connection_t *conn);
static void connection_free(connection_t *conn);
static void connection_remove_all(ogs_sbi_client_t *client);
static void connection_timer_expired(void *data);

void ogs_sbi_client_init(int num_of_sockinfo_pool, int num_of_connection_pool)
{
    curl_global_init(CURL_GLOBAL_DEFAULT);

    ogs_list_init(&ogs_sbi_self()->client_list);
    ogs_pool_init(&client_pool, ogs_app()->pool.nf);

    ogs_pool_init(&sockinfo_pool, num_of_sockinfo_pool);
    ogs_pool_init(&connection_pool, num_of_connection_pool);

}
void ogs_sbi_client_final(void)
{
    ogs_sbi_client_remove_all();

    ogs_pool_final(&client_pool);
    ogs_pool_final(&sockinfo_pool);
    ogs_pool_final(&connection_pool);

    curl_global_cleanup();
}

ogs_sbi_client_t *ogs_sbi_client_add(
        OpenAPI_uri_scheme_e scheme,
        char *fqdn, uint16_t fqdn_port,
        ogs_sockaddr_t *addr, ogs_sockaddr_t *addr6)
{
    ogs_sbi_client_t *client = NULL;
    CURLM *multi = NULL;

    ogs_assert(scheme);
    ogs_assert(fqdn || addr || addr6);

    ogs_pool_alloc(&client_pool, &client);
    if (!client) {
        ogs_error("No memory in client_pool");
        return NULL;
    }
    memset(client, 0, sizeof(ogs_sbi_client_t));

    client->scheme = scheme;

    client->insecure_skip_verify =
        ogs_sbi_self()->tls.client.insecure_skip_verify;
    if (ogs_sbi_self()->tls.client.cacert)
        client->cacert = ogs_strdup(ogs_sbi_self()->tls.client.cacert);

    if (ogs_sbi_self()->tls.client.private_key)
        client->private_key =
            ogs_strdup(ogs_sbi_self()->tls.client.private_key);
    if (ogs_sbi_self()->tls.client.cert)
        client->cert = ogs_strdup(ogs_sbi_self()->tls.client.cert);
    if (ogs_sbi_self()->tls.client.sslkeylog)
        client->sslkeylog =
            ogs_strdup(ogs_sbi_self()->tls.client.sslkeylog);

    if (ogs_sbi_self()->local_if)
       client->local_if = ogs_strdup(ogs_sbi_self()->local_if);

    ogs_debug("ogs_sbi_client_add [%s]", OpenAPI_uri_scheme_ToString(scheme));
    OGS_OBJECT_REF(client);

    if (fqdn)
        ogs_assert((client->fqdn = ogs_strdup(fqdn)));
    client->fqdn_port = fqdn_port;
    if (addr)
        ogs_assert(OGS_OK == ogs_copyaddrinfo(&client->addr, addr));
    if (addr6)
        ogs_assert(OGS_OK == ogs_copyaddrinfo(&client->addr6, addr6));

    client->t_curl = ogs_timer_add(
            ogs_app()->timer_mgr, multi_timer_expired, client);
    if (!client->t_curl) {
        ogs_error("ogs_timer_add() failed");
        ogs_pool_free(&client_pool, client);
        return NULL;
    }

    multi = client->multi = curl_multi_init();
    ogs_assert(multi);
    curl_multi_setopt(multi, CURLMOPT_SOCKETFUNCTION, sock_cb);
    curl_multi_setopt(multi, CURLMOPT_SOCKETDATA, client);
    curl_multi_setopt(multi, CURLMOPT_TIMERFUNCTION, multi_timer_cb);
    curl_multi_setopt(multi, CURLMOPT_TIMERDATA, client);
#ifdef CURLMOPT_MAX_CONCURRENT_STREAMS
    curl_multi_setopt(multi, CURLMOPT_MAX_CONCURRENT_STREAMS,
                        ogs_app()->pool.stream);
#endif

    ogs_list_init(&client->connection_list);

    ogs_list_add(&ogs_sbi_self()->client_list, client);

    ogs_debug("CLEINT added with Ref [%d]", client->reference_count);

    return client;
}

void ogs_sbi_client_remove(ogs_sbi_client_t *client)
{
    char buf[OGS_ADDRSTRLEN];

    ogs_assert(client);

    ogs_debug("CLEINT UnRef [%d]", client->reference_count);
    if (client->fqdn)
        ogs_debug("- fqdn [%s:%d]", client->fqdn, client->fqdn_port);
    if (client->resolve)
        ogs_debug("- resolve [%s]", client->resolve);
    if (client->addr)
        ogs_debug("- addr [%s:%d]",
                OGS_ADDR(client->addr, buf), OGS_PORT(client->addr));
    if (client->addr6)
        ogs_debug("- addr6 [%s:%d]",
                OGS_ADDR(client->addr6, buf), OGS_PORT(client->addr6));

    /* ogs_sbi_client_t is always created with reference context */
    if (OGS_OBJECT_IS_REF(client)) {
        OGS_OBJECT_UNREF(client);
        return;
    }

    ogs_debug("CLEINT removed [%d]", client->reference_count);

    ogs_list_remove(&ogs_sbi_self()->client_list, client);

    connection_remove_all(client);

    ogs_assert(client->t_curl);
    ogs_timer_delete(client->t_curl);
    client->t_curl = NULL;

    ogs_assert(client->multi);
    curl_multi_cleanup(client->multi);

    if (client->cacert)
        ogs_free(client->cacert);
    if (client->private_key)
        ogs_free(client->private_key);
    if (client->cert)
        ogs_free(client->cert);
    if (client->sslkeylog)
        ogs_free(client->sslkeylog);
    if (client->local_if)
        ogs_free(client->local_if);

    if (client->fqdn)
        ogs_free(client->fqdn);
    if (client->resolve)
        ogs_free(client->resolve);

    if (client->addr)
        ogs_freeaddrinfo(client->addr);
    if (client->addr6)
        ogs_freeaddrinfo(client->addr6);

    ogs_pool_free(&client_pool, client);
}

void ogs_sbi_client_remove_all(void)
{
    ogs_sbi_client_t *client = NULL, *next_client = NULL;

    ogs_list_for_each_safe(&ogs_sbi_self()->client_list, next_client, client)
        ogs_sbi_client_remove(client);
}

ogs_sbi_client_t *ogs_sbi_client_find(
        OpenAPI_uri_scheme_e scheme,
        char *fqdn, uint16_t fqdn_port,
        ogs_sockaddr_t *addr, ogs_sockaddr_t *addr6)
{
    ogs_sbi_client_t *client = NULL;

    ogs_assert(scheme);

    ogs_list_for_each(&ogs_sbi_self()->client_list, client) {
        if (client->scheme != scheme)
            continue;

        if (fqdn) {
            if (!client->fqdn)
                continue;
            if (strcmp(client->fqdn, fqdn) != 0)
                continue;

            if (fqdn_port) {
                if (!client->fqdn_port)
                    continue;
                if (client->fqdn_port != fqdn_port)
                    continue;
            }
        }
        if (addr) {
            if (!client->addr)
                continue;
            if (ogs_sockaddr_is_equal(client->addr, addr) == false)
                continue;
        }
        if (addr6) {
            if (!client->addr6)
                continue;
            if (ogs_sockaddr_is_equal(client->addr6, addr6) == false)
                continue;
        }

        break;
    }

    return client;
}

void ogs_sbi_client_stop(ogs_sbi_client_t *client)
{
    connection_t *conn = NULL;

    ogs_assert(client);

    ogs_list_for_each(&client->connection_list, conn) {
        ogs_assert(conn->client_cb);
        conn->client_cb(OGS_DONE, NULL, conn->data);
    }
}

void ogs_sbi_client_stop_all(void)
{
    ogs_sbi_client_t *client = NULL;

    ogs_list_for_each(&ogs_sbi_self()->client_list, client)
        ogs_sbi_client_stop(client);
}

#define mycase(code) \
  case code: s = OGS_STRINGIFY(code)

static void mcode_or_die(const char *where, CURLMcode code)
{
    if(CURLM_OK != code) {
        const char *s;
        switch(code) {
            mycase(CURLM_BAD_HANDLE); break;
            mycase(CURLM_BAD_EASY_HANDLE); break;
            mycase(CURLM_OUT_OF_MEMORY); break;
            mycase(CURLM_INTERNAL_ERROR); break;
            mycase(CURLM_UNKNOWN_OPTION); break;
            mycase(CURLM_LAST); break;
            default: s = "CURLM_unknown"; break;
            mycase(CURLM_BAD_SOCKET);
            ogs_error("ERROR: %s returns %s", where, s);
            /* ignore this error */
            return;
        }
        ogs_fatal("ERROR: %s returns %s", where, s);
        ogs_assert_if_reached();
    }
}

static char *add_params_to_uri(CURL *easy, char *uri, ogs_hash_t *params)
{
    ogs_hash_index_t *hi;
    int has_params = 0;
    const char *fp = "?", *np = "&";

    ogs_assert(easy);
    ogs_assert(uri);
    ogs_assert(params);
    ogs_assert(ogs_hash_count(params));

    has_params = (strchr(uri, '?') != NULL);

    for (hi = ogs_hash_first(params); hi; hi = ogs_hash_next(hi)) {
        const char *key = NULL;
        char *key_esc = NULL;
        char *val = NULL;
        char *val_esc = NULL;

        key = ogs_hash_this_key(hi);
        ogs_assert(key);
        val = ogs_hash_this_val(hi);
        ogs_assert(val);

        key_esc = curl_easy_escape(easy, key, 0);
        ogs_assert(key_esc);
        val_esc = curl_easy_escape(easy, val, 0);
        ogs_assert(val_esc);

        if (!has_params) {
            uri = ogs_mstrcatf(uri, "%s%s=%s", fp, key_esc, val_esc);
            ogs_expect(uri);
            has_params = 1;
        } else {
            uri = ogs_mstrcatf(uri, "%s%s=%s", np, key_esc, val_esc);
            ogs_expect(uri);
        }

        curl_free(val_esc);
        curl_free(key_esc);
    }

    return uri;
}

/* User-defined SSL_CTX callback function */
static CURLcode sslctx_callback(CURL *curl, void *sslctx, void *userdata)
{
    SSL_CTX *ctx = (SSL_CTX *)sslctx;
    ogs_sbi_client_t *client = userdata;

    ogs_assert(ctx);
    ogs_assert(userdata);

    /* Ensure app data is set for SSL objects */
    SSL_CTX_set_app_data(ctx, client->sslkeylog);

#if OPENSSL_VERSION_NUMBER >= 0x10101000L
    /* Set the SSL Key Log callback */
    SSL_CTX_set_keylog_callback(ctx, ogs_sbi_keylog_callback);
#endif

    return CURLE_OK;
}

static connection_t *connection_add(
        ogs_sbi_client_t *client, ogs_sbi_client_cb_f client_cb,
        ogs_sbi_request_t *request, void *data)
{
    ogs_hash_index_t *hi;
    int i;
    connection_t *conn = NULL;
    CURLMcode rc;

    ogs_assert(client);
    ogs_assert(client_cb);
    ogs_assert(request);
    ogs_assert(request->h.method);

    ogs_pool_alloc(&connection_pool, &conn);
    if (!conn) {
        ogs_error("ogs_pool_alloc() failed");
        return NULL;
    }
    memset(conn, 0, sizeof(connection_t));

    conn->client = client;
    conn->client_cb = client_cb;
    conn->data = data;

    conn->method = ogs_strdup(request->h.method);
    if (!conn->method) {
        ogs_error("conn->method is NULL");
        connection_free(conn);
        return NULL;
    }

    conn->num_of_header = ogs_hash_count(request->http.headers);
    if (conn->num_of_header) {
        conn->headers = ogs_calloc(conn->num_of_header, sizeof(char *));
        if (!conn->headers) {
            ogs_error("conn->headers is NULL");
            connection_free(conn);
            return NULL;
        }
        for (hi = ogs_hash_first(request->http.headers), i = 0;
                hi && i < conn->num_of_header; hi = ogs_hash_next(hi), i++) {
            const char *key = ogs_hash_this_key(hi);
            char *val = ogs_hash_this_val(hi);

            conn->headers[i] = ogs_msprintf("%s: %s", key, val);
            if (!conn->headers[i]) {
                ogs_error("conn->headers[i=%d] is NULL", i);
                connection_free(conn);
                return NULL;
            }
            conn->header_list = curl_slist_append(
                    conn->header_list, conn->headers[i]);
        }
    }

    conn->timer = ogs_timer_add(
            ogs_app()->timer_mgr, connection_timer_expired, conn);
    if (!conn->timer) {
        ogs_error("conn->timer is NULL");
        connection_free(conn);
        return NULL;
    }

    /* If http response is not received within deadline,
     * Open5GS will discard this request. */
    ogs_timer_start(conn->timer,
            ogs_local_conf()->time.message.sbi.connection_deadline);

    conn->easy = curl_easy_init();
    if (!conn->easy) {
        ogs_error("conn->easy is NULL");
        connection_free(conn);
        return NULL;
    }

    if (ogs_hash_count(request->http.params)) {
        char *uri = add_params_to_uri(conn->easy,
                            request->h.uri, request->http.params);
        if (!uri) {
            ogs_error("add_params_to_uri() failed");
            connection_free(conn);
            return NULL;
        }

        request->h.uri = uri;
    }

    curl_easy_setopt(conn->easy, CURLOPT_BUFFERSIZE, OGS_MAX_SDU_LEN);

    /* HTTPS certificate-related settings */
    if (client->scheme == OpenAPI_uri_scheme_https) {
        if (client->insecure_skip_verify) {
            curl_easy_setopt(conn->easy, CURLOPT_SSL_VERIFYPEER, 0);
            curl_easy_setopt(conn->easy, CURLOPT_SSL_VERIFYHOST, 0);
        } else {
            if (client->cacert)
                curl_easy_setopt(conn->easy, CURLOPT_CAINFO, client->cacert);
        }

        /* Set private key & certificate */
        if (client->private_key && client->cert) {
            curl_easy_setopt(conn->easy, CURLOPT_SSLKEY, client->private_key);
            curl_easy_setopt(conn->easy, CURLOPT_SSLCERT, client->cert);
        }

        if (client->sslkeylog) {
            /* Set SSL_CTX callback */
            curl_easy_setopt(conn->easy, CURLOPT_SSL_CTX_FUNCTION,
                    sslctx_callback);

            /* Optionally set additional user data */
            curl_easy_setopt(conn->easy, CURLOPT_SSL_CTX_DATA, client);
        }
    }

    /* Configure HTTP Method */
    if (strcmp(request->h.method, OGS_SBI_HTTP_METHOD_PUT) == 0 ||
        strcmp(request->h.method, OGS_SBI_HTTP_METHOD_PATCH) == 0 ||
        strcmp(request->h.method, OGS_SBI_HTTP_METHOD_DELETE) == 0 ||
        strcmp(request->h.method, OGS_SBI_HTTP_METHOD_POST) == 0) {

        curl_easy_setopt(conn->easy,
                CURLOPT_CUSTOMREQUEST, request->h.method);
        if (request->http.content) {
            conn->content = ogs_memdup(
                    request->http.content, request->http.content_length);
            if (!conn->content) {
                ogs_error("conn->content is NULL");
                connection_free(conn);
                return NULL;
            }
            curl_easy_setopt(conn->easy,
                    CURLOPT_POSTFIELDS, conn->content);
            curl_easy_setopt(conn->easy,
                CURLOPT_POSTFIELDSIZE, request->http.content_length);
#if 1 /* Disable HTTP/1.1 100 Continue : Use "Expect:" in libcurl */
            conn->header_list = curl_slist_append(
                    conn->header_list, "Expect:");
#else
            curl_easy_setopt(conn->easy, CURLOPT_EXPECT_100_TIMEOUT_MS, 0L);
#endif
            ogs_debug("SENDING...[%d]", (int)request->http.content_length);
            if (request->http.content_length)
                ogs_debug("%s", request->http.content);
            if (request->http.content && request->http.content_length) {
                ogs_debug("%.*s", (int)request->http.content_length, request->http.content);
            }
        }
    }

    curl_easy_setopt(conn->easy, CURLOPT_HTTPHEADER, conn->header_list);

#if 1 /* Use HTTP2 */
    curl_easy_setopt(conn->easy,
            CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2_PRIOR_KNOWLEDGE);
#endif

    ogs_list_add(&client->connection_list, conn);

    curl_easy_setopt(conn->easy, CURLOPT_URL, request->h.uri);

    if (client->resolve) {
        conn->resolve_list = curl_slist_append(NULL, client->resolve);
        curl_easy_setopt(conn->easy, CURLOPT_RESOLVE, conn->resolve_list);
    }

    if (client->local_if) {
        curl_easy_setopt(conn->easy, CURLOPT_INTERFACE, client->local_if);
    }

    curl_easy_setopt(conn->easy, CURLOPT_PRIVATE, conn);
    curl_easy_setopt(conn->easy, CURLOPT_WRITEFUNCTION, write_cb);
    curl_easy_setopt(conn->easy, CURLOPT_WRITEDATA, conn);
    curl_easy_setopt(conn->easy, CURLOPT_HEADERFUNCTION, header_cb);
    curl_easy_setopt(conn->easy, CURLOPT_HEADERDATA, conn);
    curl_easy_setopt(conn->easy, CURLOPT_ERRORBUFFER, conn->error);

    ogs_assert(client->multi);
    rc = curl_multi_add_handle(client->multi, conn->easy);
    mcode_or_die("connection_add: curl_multi_add_handle", rc);

    return conn;
}

static void connection_remove(connection_t *conn)
{
    ogs_sbi_client_t *client = NULL;

    ogs_assert(conn);
    client = conn->client;
    ogs_assert(client);

    ogs_list_remove(&client->connection_list, conn);

    ogs_assert(client->multi);
    curl_multi_remove_handle(client->multi, conn->easy);

    connection_free(conn);
}

static void connection_free(connection_t *conn)
{
    int i;

    ogs_assert(conn);

    if (conn->content)
        ogs_free(conn->content);

    if (conn->location)
        ogs_free(conn->location);
    if (conn->producer_id)
        ogs_free(conn->producer_id);

    if (conn->memory)
        ogs_free(conn->memory);

    if (conn->easy)
        curl_easy_cleanup(conn->easy);

    if (conn->timer)
        ogs_timer_delete(conn->timer);

    if (conn->num_of_header) {
        for (i = 0; i < conn->num_of_header; i++)
            if (conn->headers[i])
                ogs_free(conn->headers[i]);
        ogs_free(conn->headers);
    }
    curl_slist_free_all(conn->header_list);

    curl_slist_free_all(conn->resolve_list);

    if (conn->method)
        ogs_free(conn->method);

    ogs_pool_free(&connection_pool, conn);
}

static void connection_remove_all(ogs_sbi_client_t *client)
{
    connection_t *conn = NULL, *next_conn = NULL;

    ogs_assert(client);

    ogs_list_for_each_safe(&client->connection_list, next_conn, conn)
        connection_remove(conn);
}

static void connection_timer_expired(void *data)
{
    connection_t *conn = NULL;
    CURLcode res;
    char *effective_url = NULL;

    conn = data;
    ogs_assert(conn);

    ogs_error("Connection timer expired [METHOD:%s]", conn->method);

    res = curl_easy_getinfo(conn->easy, CURLINFO_EFFECTIVE_URL, &effective_url);
    if ((res == CURLE_OK) && effective_url)
        ogs_error("Effective URL: %s", effective_url);
    else
        ogs_error("curl_easy_getinfo() failed [%s]", curl_easy_strerror(res));

    ogs_assert(conn->client_cb);
    conn->client_cb(OGS_TIMEUP, NULL, conn->data);

    connection_remove(conn);
}

static void check_multi_info(ogs_sbi_client_t *client)
{
    CURLM *multi = NULL;
    CURLMsg *resource;
    int pending;
    CURL *easy = NULL;
    CURLcode res;
    connection_t *conn = NULL;
    ogs_sbi_response_t *response = NULL;

    ogs_assert(client);
    multi = client->multi;
    ogs_assert(multi);

    while ((resource = curl_multi_info_read(multi, &pending))) {
        char *url;
        char *content_type = NULL;
        long res_status;
        ogs_assert(resource);

        switch (resource->msg) {
        case CURLMSG_DONE:
            easy = resource->easy_handle;
            ogs_assert(easy);

            curl_easy_getinfo(easy, CURLINFO_PRIVATE, &conn);
            ogs_assert(conn);

            curl_easy_getinfo(easy, CURLINFO_EFFECTIVE_URL, &url);
            curl_easy_getinfo(easy, CURLINFO_RESPONSE_CODE, &res_status);
            curl_easy_getinfo(easy, CURLINFO_CONTENT_TYPE, &content_type);

            res = resource->data.result;
            if (res == CURLE_OK) {
                ogs_log_level_e level = OGS_LOG_DEBUG;

                response = ogs_sbi_response_new();
                ogs_assert(response);

                response->status = res_status;

                ogs_assert(conn->method);
                response->h.method = ogs_strdup(conn->method);
                ogs_assert(response->h.method);

                /* remove https://localhost:8000 */
                response->h.uri = ogs_strdup(url);
                ogs_assert(response->h.uri);

                if (content_type)
                    ogs_sbi_header_set(response->http.headers,
                            OGS_SBI_CONTENT_TYPE, content_type);
                if (conn->location)
                    ogs_sbi_header_set(response->http.headers,
                            OGS_SBI_LOCATION, conn->location);
                if (conn->producer_id)
                    ogs_sbi_header_set(response->http.headers,
                            OGS_SBI_CUSTOM_PRODUCER_ID, conn->producer_id);

                if (conn->memory_overflow == true)
                    level = OGS_LOG_ERROR;

                ogs_log_message(level, 0, "[%d:%s] %s",
                        response->status, response->h.method, response->h.uri);

                if (conn->memory) {
                    response->http.content =
                        ogs_memdup(conn->memory, conn->size + 1);
                    ogs_assert(response->http.content);
                    response->http.content_length = conn->size;
                    ogs_assert(response->http.content_length);
                }

                ogs_log_message(level, 0, "RECEIVED[%d]",
                        (int)response->http.content_length);
                if (response->http.content_length && response->http.content)
                    ogs_log_message(level, 0, "%s", response->http.content);

                if (conn->memory_overflow == true) {
                    ogs_sbi_response_free(response);
                    connection_remove(conn);
                    break;
                }

            } else
                ogs_warn("[%d] %s", res, conn->error);

            ogs_assert(conn->client_cb);
            if (res == CURLE_OK)
                conn->client_cb(OGS_OK, response, conn->data);
            else
                conn->client_cb(OGS_ERROR, NULL, conn->data);

            connection_remove(conn);
            break;
        default:
            ogs_error("Unknown CURL resource[%d]", resource->msg);
            break;
        }
    }
}

bool ogs_sbi_client_send_request(
        ogs_sbi_client_t *client, ogs_sbi_client_cb_f client_cb,
        ogs_sbi_request_t *request, void *data)
{
    connection_t *conn = NULL;

    ogs_assert(client);
    ogs_assert(request);
    if (request->h.uri == NULL) {
        request->h.uri = ogs_sbi_client_uri(client, &request->h);
        ogs_assert(request->h.method);
        ogs_assert(request->h.uri);
    }
    ogs_debug("[%s] %s", request->h.method, request->h.uri);

    conn = connection_add(client, client_cb, request, data);
    if (!conn) {
        ogs_error("connection_add() failed");
        return false;
    }

    return true;
}

bool ogs_sbi_client_send_via_scp_or_sepp(
        ogs_sbi_client_t *client, ogs_sbi_client_cb_f client_cb,
        ogs_sbi_request_t *request, void *data)
{
    bool rc;

    ogs_assert(request);
    ogs_assert(client);

    if (request->h.uri) {
        /*
         * In case of the communication using SCP or SEPP,
         * If the full URI is already defined,
         * change full URI to SCP or SEPP as below.
         *
         * OLD: http://127.0.0.5:7777/nnrf-nfm/v1/nf-status-notify
         * SCP: https://scp.open5gs.org/nnrf-nfm/v1/nf-status-notify
         * SEPP: https://sepp.open5gs.org/nnrf-nfm/v1/nf-status-notify
         */
        char *apiroot = NULL;
        char *path = NULL;
        char *old = NULL;

        old = request->h.uri;

        apiroot = ogs_sbi_client_apiroot(client);
        ogs_assert(apiroot);

        rc = ogs_sbi_getpath_from_uri(&path, request->h.uri);
        ogs_assert(path);

        request->h.uri = ogs_msprintf("%s/%s", apiroot, path);
        ogs_assert(request->h.method);
        ogs_assert(request->h.uri);
        ogs_debug("[%s] %s", request->h.method, request->h.uri);

        ogs_free(apiroot);
        ogs_free(path);
        ogs_free(old);
    }

    rc = ogs_sbi_client_send_request(client, client_cb, request, data);
    ogs_expect(rc == true);

    return rc;
}

static size_t write_cb(void *contents, size_t size, size_t nmemb, void *data)
{
    size_t realsize = 0;
    connection_t *conn = NULL;
    char *ptr = NULL;

    conn = data;
    ogs_assert(conn);

    realsize = size * nmemb;
    ptr = ogs_realloc(conn->memory, conn->size + realsize + 1);
    if(!ptr) {
        conn->memory_overflow = true;

        ogs_error("Overflow : conn->size[%d], realsize[%d]",
                    (int)conn->size, (int)realsize);
        ogs_log_hexdump(OGS_LOG_ERROR, contents, realsize);

        return 0;
    }

    conn->memory = ptr;
    memcpy(&(conn->memory[conn->size]), contents, realsize);
    conn->size += realsize;
    conn->memory[conn->size] = 0;

    return realsize;
}

static size_t header_cb(void *ptr, size_t size, size_t nmemb, void *data)
{
    connection_t *conn = NULL;

    conn = data;
    ogs_assert(conn);

    if (ogs_strncasecmp(ptr, OGS_SBI_LOCATION, strlen(OGS_SBI_LOCATION)) == 0) {
    /* ptr : "Location: http://xxx/xxx/xxx\r\n"
       We need to truncate "Location" + ": " + "\r\n" in 'ptr' string */
        int len = strlen(ptr) - strlen(OGS_SBI_LOCATION) - 2 - 2;
        if (len) {
            /* Only copy http://xxx/xxx/xxx" from 'ptr' string */
            conn->location = ogs_memdup(
                    (char *)ptr + strlen(OGS_SBI_LOCATION) + 2,
                    len+1);
            ogs_assert(conn->location);
            conn->location[len] = 0;
        }
    } else if (ogs_strncasecmp(ptr,
                OGS_SBI_CUSTOM_PRODUCER_ID,
                strlen(OGS_SBI_CUSTOM_PRODUCER_ID)) == 0) {
    /* ptr : "3gpp-Sbi-Producer-Id: 0cb58eca-4e84-41ed-aa10-9f892634b770\r\n"
       We need to truncate "3gpp-Sbi-Producer-Id" + ": " + "\r\n"
       in 'ptr' string */
        int len = strlen(ptr) - strlen(OGS_SBI_CUSTOM_PRODUCER_ID) - 2 - 2;
        if (len) {
            /* Only copy  0cb58eca-4e84-41ed-aa10-9f892634b770from 'ptr' string */
            conn->producer_id = ogs_memdup(
                    (char *)ptr + strlen(OGS_SBI_CUSTOM_PRODUCER_ID) + 2,
                    len+1);
            ogs_assert(conn->producer_id);
            conn->producer_id[len] = 0;
        }
    }

    return (nmemb*size);
}

static void event_cb(short when, ogs_socket_t fd, void *data)
{
    sockinfo_t *sockinfo = NULL;
    ogs_sbi_client_t *client = NULL;
    CURLM *multi = NULL;

    CURLMcode rc;
    int action = ((when & OGS_POLLIN) ? CURL_CSELECT_IN : 0) |
                    ((when & OGS_POLLOUT) ? CURL_CSELECT_OUT : 0);

    sockinfo = data;
    ogs_assert(sockinfo);
    client = sockinfo->client;
    ogs_assert(client);
    multi = client->multi;
    ogs_assert(multi);

    rc = curl_multi_socket_action(multi, fd, action, &client->still_running);
    mcode_or_die("event_cb: curl_multi_socket_action", rc);

    check_multi_info(client);
    if (client->still_running <= 0) {
        ogs_timer_t *timer;

        timer = client->t_curl;
        if (timer)
            ogs_timer_stop(timer);
    }
}

/* Assign information to a sockinfo_t structure */
static void sock_set(sockinfo_t *sockinfo, curl_socket_t s,
        CURL *e, int act, ogs_sbi_client_t *client)
{
    int kind = ((act & CURL_POLL_IN) ? OGS_POLLIN : 0) |
                ((act & CURL_POLL_OUT) ? OGS_POLLOUT : 0);

    if (sockinfo->sockfd)
        ogs_pollset_remove(sockinfo->poll);

    sockinfo->sockfd = s;
    sockinfo->action = act;
    sockinfo->easy = e;

    sockinfo->poll = ogs_pollset_add(
            ogs_app()->pollset, kind, s, event_cb, sockinfo);
    ogs_assert(sockinfo->poll);
}

/* Initialize a new sockinfo_t structure */
static void sock_new(curl_socket_t s,
        CURL *easy, int action, ogs_sbi_client_t *client)
{
    sockinfo_t *sockinfo = NULL;
    CURLM *multi = NULL;

    ogs_assert(client);
    multi = client->multi;
    ogs_assert(multi);

    ogs_pool_alloc(&sockinfo_pool, &sockinfo);
    ogs_assert(sockinfo);
    memset(sockinfo, 0, sizeof(sockinfo_t));

    sockinfo->client = client;
    sock_set(sockinfo, s, easy, action, client);
    curl_multi_assign(multi, s, sockinfo);
}

/* Clean up the sockinfo_t structure */
static void sock_free(sockinfo_t *sockinfo, ogs_sbi_client_t *client)
{
    ogs_assert(sockinfo);
    ogs_assert(sockinfo->poll);

    ogs_pollset_remove(sockinfo->poll);
    ogs_pool_free(&sockinfo_pool, sockinfo);
}

/* CURLMOPT_SOCKETFUNCTION */
static int sock_cb(CURL *e, curl_socket_t s, int what, void *cbp, void *sockp)
{
    ogs_sbi_client_t *client = (ogs_sbi_client_t *)cbp;
    sockinfo_t *sockinfo = (sockinfo_t *) sockp;

    if (what == CURL_POLL_REMOVE) {
        sock_free(sockinfo, client);
    } else {
        if (!sockinfo) {
            sock_new(s, e, what, client);
        } else {
            sock_set(sockinfo, s, e, what, client);
        }
    }
    return 0;
}

static void multi_timer_expired(void *data)
{
    CURLMcode rc;
    ogs_sbi_client_t *client = NULL;
    CURLM *multi = NULL;

    client = data;
    ogs_assert(client);
    multi = client->multi;
    ogs_assert(multi);

    rc = curl_multi_socket_action(
            multi, CURL_SOCKET_TIMEOUT, 0, &client->still_running);
    mcode_or_die("multi_timer_expired: curl_multi_socket_action", rc);
    check_multi_info(client);
}

static int multi_timer_cb(CURLM *multi, long timeout_ms, void *cbp)
{
    ogs_sbi_client_t *client = NULL;
    ogs_timer_t *timer = NULL;

    client = cbp;
    ogs_assert(client);
    timer = client->t_curl;
    ogs_assert(timer);

    if (timeout_ms > 0) {
        ogs_timer_start(timer, ogs_time_from_msec(timeout_ms));
    } else if (timeout_ms == 0) {
        /* libcurl wants us to timeout now.
         * The closest we can do is to schedule the timer to fire in 1 us. */
        ogs_timer_start(timer, 1);
    } else {
        ogs_timer_stop(timer);
    }

    return 0;
}
