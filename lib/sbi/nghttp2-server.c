/*
 * Copyright (C) 2019,2024 by Sukchan Lee <acetcom@gmail.com>
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
#include "yuarel.h"

#include <netinet/tcp.h>
#include <nghttp2/nghttp2.h>

#define USE_SEND_DATA_WITH_NO_COPY 1

static void server_init(int num_of_session_pool, int num_of_stream_pool);
static void server_final(void);

static int server_start(ogs_sbi_server_t *server,
        int (*cb)(ogs_sbi_request_t *request, void *data));
static void server_stop(ogs_sbi_server_t *server);

static bool server_send_rspmem_persistent(
        ogs_sbi_stream_t *stream, ogs_sbi_response_t *response);
static bool server_send_response(
        ogs_sbi_stream_t *stream, ogs_sbi_response_t *response);

static ogs_sbi_server_t *server_from_stream(ogs_sbi_stream_t *stream);

static ogs_pool_id_t id_from_stream(ogs_sbi_stream_t *stream);
static void *stream_find_by_id(ogs_pool_id_t id);

const ogs_sbi_server_actions_t ogs_nghttp2_server_actions = {
    server_init,
    server_final,

    server_start,
    server_stop,

    server_send_rspmem_persistent,
    server_send_response,

    server_from_stream,

    id_from_stream,
    stream_find_by_id,
};

struct h2_settings {
    uint32_t max_concurrent_streams;
    bool enable_push;
};

typedef struct ogs_sbi_session_s {
    ogs_lnode_t             lnode;

    ogs_sock_t              *sock;
    ogs_sockaddr_t          *addr;
    struct {
        ogs_poll_t          *read;
        ogs_poll_t          *write;
    } poll;

    nghttp2_session         *session;
    ogs_list_t              write_queue;

    ogs_sbi_server_t        *server;
    ogs_list_t              stream_list;
    int32_t                 last_stream_id;

    struct h2_settings      settings;
    SSL*                    ssl;
} ogs_sbi_session_t;

typedef struct ogs_sbi_stream_s {
    ogs_lnode_t             lnode;

    ogs_pool_id_t           id;

    int32_t                 stream_id;
    ogs_sbi_request_t       *request;
    bool                    memory_overflow;

    ogs_sbi_session_t       *session;
} ogs_sbi_stream_t;

static void session_remove(ogs_sbi_session_t *sbi_sess);
static void session_remove_all(ogs_sbi_server_t *server);

static void stream_remove(ogs_sbi_stream_t *stream);

static void accept_handler(short when, ogs_socket_t fd, void *data);
static void recv_handler(short when, ogs_socket_t fd, void *data);

static int session_set_callbacks(ogs_sbi_session_t *sbi_sess);
static int session_send_preface(ogs_sbi_session_t *sbi_sess);
static int session_send(ogs_sbi_session_t *sbi_sess);
static void session_write_to_buffer(
        ogs_sbi_session_t *sbi_sess, ogs_pkbuf_t *pkbuf);

static OGS_POOL(session_pool, ogs_sbi_session_t);
static OGS_POOL(stream_pool, ogs_sbi_stream_t);

static void server_init(int num_of_session_pool, int num_of_stream_pool)
{
    ogs_pool_init(&session_pool, num_of_session_pool);
    ogs_pool_init(&stream_pool, num_of_stream_pool);
}

static void server_final(void)
{
    ogs_pool_final(&stream_pool);
    ogs_pool_final(&session_pool);
}

#ifndef OPENSSL_NO_NEXTPROTONEG
static int next_proto_cb(SSL *ssl, const unsigned char **data,
                         unsigned int *len, void *arg)
{
    static unsigned char next_proto_list[256];
    (void)ssl;
    (void)arg;

    next_proto_list[0] = NGHTTP2_PROTO_VERSION_ID_LEN;
    memcpy(&next_proto_list[1], NGHTTP2_PROTO_VERSION_ID, NGHTTP2_PROTO_VERSION_ID_LEN);

    *data = next_proto_list;
    *len = 1 + NGHTTP2_PROTO_VERSION_ID_LEN;
    return SSL_TLSEXT_ERR_OK;
}
#endif

#if OPENSSL_VERSION_NUMBER >= 0x10002000L
static int alpn_select_proto_cb(SSL *ssl, const unsigned char **out,
                                unsigned char *outlen, const unsigned char *in,
                                unsigned int inlen, void *arg)
{
    int rv;
    (void)ssl;
    (void)arg;

    rv = nghttp2_select_next_protocol((unsigned char **)out, outlen, in, inlen);
    if (rv != 1) {
        return SSL_TLSEXT_ERR_NOACK;
    }

    return SSL_TLSEXT_ERR_OK;
}
#endif /* OPENSSL_VERSION_NUMBER >= 0x10002000L */

static int ssl_ctx_set_proto_versions(SSL_CTX *ssl_ctx, int min, int max)
{
#if OPENSSL_VERSION_NUMBER >= 0x1010000fL
  if (SSL_CTX_set_min_proto_version(ssl_ctx, min) != 1 ||
      SSL_CTX_set_max_proto_version(ssl_ctx, max) != 1) {
    return -1;
  }
  return 0;
#else /* !(OPENSSL_VERSION_NUMBER >= 0x1010000fL) */
  long int opts = 0;

  // TODO We depends on the ordering of protocol version macro in
  // OpenSSL.
  if (min > TLS1_VERSION) {
    opts |= SSL_OP_NO_TLSv1;
  }
  if (min > TLS1_1_VERSION) {
    opts |= SSL_OP_NO_TLSv1_1;
  }
  if (min > TLS1_2_VERSION) {
    opts |= SSL_OP_NO_TLSv1_2;
  }

  if (max < TLS1_2_VERSION) {
    opts |= SSL_OP_NO_TLSv1_2;
  }
  if (max < TLS1_1_VERSION) {
    opts |= SSL_OP_NO_TLSv1_1;
  }

  SSL_CTX_set_options(ssl_ctx, opts);

  return 0;
#endif /* OPENSSL_VERSION_NUMBER >= 0x1010000fL */
}

static SSL_CTX *create_ssl_ctx(
        const char *key_file, const char *cert_file,
        const char *sslkeylog_file)
{
    SSL_CTX *ssl_ctx;
    uint64_t ssl_opts;

    ogs_assert(key_file);
    ogs_assert(cert_file);

    ssl_ctx = SSL_CTX_new(TLS_server_method());
    if (!ssl_ctx) {
        ogs_error("Could not create SSL/TLS context: %s", ERR_error_string(ERR_get_error(), NULL));
        return NULL;
    }

    /* Set key log files for each SSL_CTX */
    if (sslkeylog_file) {
        /* Ensure app data is set for SSL objects */
        SSL_CTX_set_app_data(ssl_ctx, sslkeylog_file);
        /* Set the SSL Key Log callback */
        SSL_CTX_set_keylog_callback(ssl_ctx, ogs_sbi_keylog_callback);
    }

    ssl_opts = (SSL_OP_ALL & ~SSL_OP_DONT_INSERT_EMPTY_FRAGMENTS) |
                  SSL_OP_NO_SSLv2 | SSL_OP_NO_SSLv3 | SSL_OP_NO_COMPRESSION |
                  SSL_OP_NO_SESSION_RESUMPTION_ON_RENEGOTIATION |
                  SSL_OP_SINGLE_ECDH_USE | SSL_OP_SINGLE_DH_USE |
                  SSL_OP_CIPHER_SERVER_PREFERENCE
#if OPENSSL_VERSION_NUMBER >= 0x10101000L
                  // The reason for disabling built-in anti-replay in
                  // OpenSSL is that it only works if client gets back
                  // to the same server.  The freshness check
                  // described in
                  // https://tools.ietf.org/html/rfc8446#section-8.3
                  // is still performed.
                  | SSL_OP_NO_ANTI_REPLAY
#endif /* OPENSSL_VERSION_NUMBER >= 0x10101000L */
        ;


    SSL_CTX_set_options(ssl_ctx, ssl_opts);

#if OPENSSL_VERSION_NUMBER >= 0x30000000L
    if (SSL_CTX_set1_curves_list(ssl_ctx, "P-256") != 1) {
        ogs_error("SSL_CTX_set1_curves_list failed: %s", ERR_error_string(ERR_get_error(), NULL));
        return NULL;
    }
#endif /* !(OPENSSL_VERSION_NUMBER >= 0x30000000L) */

    SSL_CTX_set_mode(ssl_ctx, SSL_MODE_AUTO_RETRY);
    SSL_CTX_set_mode(ssl_ctx, SSL_MODE_RELEASE_BUFFERS);

    if (SSL_CTX_set_default_verify_paths(ssl_ctx) != 1) {
        ogs_warn("Could not load system trusted ca certificates: %s",
                ERR_error_string(ERR_get_error(), NULL));
    }

#define OGS_TLS_MIN_VERSION TLS1_VERSION
#ifdef TLS1_3_VERSION
#define OGS_TLS_MAX_VERSION TLS1_3_VERSION
#else  /* !TLS1_3_VERSION */
#define OGS_TLS_MAX_VERSION TLS1_2_VERSION
#endif /* TLS1_3_VERSION */
    if (ssl_ctx_set_proto_versions(
                ssl_ctx, OGS_TLS_MIN_VERSION, OGS_TLS_MAX_VERSION) != 0) {
        ogs_error("Could not set TLS versions [%d:%d]",
                    OGS_TLS_MIN_VERSION, OGS_TLS_MAX_VERSION);
        return NULL;
    }

#define DEFAULT_CIPHER_LIST \
    "ECDHE-ECDSA-AES128-GCM-SHA256:ECDHE-RSA-AES128-GCM-SHA256:ECDHE-ECDSA-" \
    "AES256-GCM-SHA384:ECDHE-RSA-AES256-GCM-SHA384:ECDHE-ECDSA-CHACHA20-" \
    "POLY1305:ECDHE-RSA-CHACHA20-POLY1305:DHE-RSA-AES128-GCM-SHA256:DHE-RSA-" \
    "AES256-GCM-SHA384"
    if (SSL_CTX_set_cipher_list(ssl_ctx, DEFAULT_CIPHER_LIST) == 0) {
        ogs_error("%s", ERR_error_string(ERR_get_error(), NULL));
        return NULL;
    }

    if (SSL_CTX_use_PrivateKey_file(ssl_ctx, key_file, SSL_FILETYPE_PEM) != 1) {
        ogs_error("Could not read private key file - key_file=%s", key_file);
        return NULL;
    }
    if (SSL_CTX_use_certificate_chain_file(ssl_ctx, cert_file) != 1) {
        ogs_error("Could not read certificate file - cert_file=%s ", cert_file);
        return NULL;
    }
    if (SSL_CTX_check_private_key(ssl_ctx) != 1) {
        ogs_error("SSL_CTX_check_private_key failed: %s",
                ERR_error_string(ERR_get_error(), NULL));
        return NULL;
    }

#ifndef OPENSSL_NO_NEXTPROTONEG
    SSL_CTX_set_next_protos_advertised_cb(ssl_ctx, next_proto_cb, NULL);
#endif /* !OPENSSL_NO_NEXTPROTONEG */

#if OPENSSL_VERSION_NUMBER >= 0x10002000L
    SSL_CTX_set_alpn_select_cb(ssl_ctx, alpn_select_proto_cb, NULL);
#endif /* OPENSSL_VERSION_NUMBER >= 0x10002000L */

    return ssl_ctx;
}

static int verify_callback(int preverify_ok, X509_STORE_CTX *ctx)
{
    if (!preverify_ok) {
        int err = X509_STORE_CTX_get_error(ctx);
        int depth = X509_STORE_CTX_get_error_depth(ctx);
        if (err == X509_V_ERR_CERT_HAS_EXPIRED && depth == 0) {
            ogs_error("The client certificate has expired, but is accepted by "
                        "configuration");
            return 1;
        }
        ogs_error("client certificate verify error:num=%d:%s:depth=%d",
                err, X509_verify_cert_error_string(err), depth);
    }
    return preverify_ok;
}

static int server_start(ogs_sbi_server_t *server,
        int (*cb)(ogs_sbi_request_t *request, void *data))
{
    char buf[OGS_ADDRSTRLEN];
    ogs_sock_t *sock = NULL;
    ogs_sockaddr_t *addr = NULL;
    char *hostname = NULL;

    addr = server->node.addr;
    ogs_assert(addr);

    /* Create SSL CTX */
    if (server->scheme == OpenAPI_uri_scheme_https) {

        server->ssl_ctx = create_ssl_ctx(
                server->private_key, server->cert, server->sslkeylog);
        if (!server->ssl_ctx) {
            ogs_error("Cannot create SSL CTX");
            return OGS_ERROR;
        }

        if (server->verify_client_cacert) {
            char *context = NULL;
            STACK_OF(X509_NAME) *cert_names = NULL;

            if (SSL_CTX_load_verify_locations(
                        server->ssl_ctx,
                        server->verify_client_cacert, NULL) != 1) {
                ogs_error("Could not load trusted ca certificates from %s:%s",
                        server->verify_client_cacert,
                        ERR_error_string(ERR_get_error(), NULL));

                SSL_CTX_free(server->ssl_ctx);

                return OGS_ERROR;
            }

            /*
             * It is heard that SSL_CTX_load_verify_locations() may leave
             * error even though it returns success. See
             * http://forum.nginx.org/read.php?29,242540
             */
            cert_names = SSL_load_client_CA_file(server->verify_client_cacert);
            if (!cert_names) {
                ogs_error("Could not load ca certificates from %s:%s",
                    server->verify_client_cacert,
                    ERR_error_string(ERR_get_error(), NULL));

                SSL_CTX_free(server->ssl_ctx);

                return OGS_ERROR;
            }
            SSL_CTX_set_client_CA_list(server->ssl_ctx, cert_names);

            if (server->verify_client)
                SSL_CTX_set_verify(
                        server->ssl_ctx,
                        SSL_VERIFY_PEER | SSL_VERIFY_CLIENT_ONCE |
                        SSL_VERIFY_FAIL_IF_NO_PEER_CERT,
                        verify_callback);

            ogs_assert(server->id >= OGS_MIN_POOL_ID &&
                    server->id <= OGS_MAX_POOL_ID);
            context = ogs_msprintf("%d", server->id);
            if (!context) {
                ogs_error("ogs_sbi_server_id_context() failed");

                SSL_CTX_free(server->ssl_ctx);

                return OGS_ERROR;
            }

            if (!SSL_CTX_set_session_id_context(
                        server->ssl_ctx,
                        (unsigned char *)context, strlen(context))) {
                ogs_error("SSL_CTX_set_session_id_context() failed");

                ogs_free(context);
                SSL_CTX_free(server->ssl_ctx);

                return OGS_ERROR;
            }

            ogs_free(context);
        }
    }

    sock = ogs_tcp_server(addr, server->node.option);
    if (!sock) {
        ogs_error("Cannot start SBI server");

        if (server->ssl_ctx)
            SSL_CTX_free(server->ssl_ctx);

        return OGS_ERROR;
    }

    server->node.sock = sock;

    /* Setup callback function */
    server->cb = cb;

    /* Setup poll for server listening socket */
    server->node.poll = ogs_pollset_add(ogs_app()->pollset,
            OGS_POLLIN, sock->fd, accept_handler, server);
    ogs_assert(server->node.poll);

    hostname = ogs_gethostname(addr);
    if (hostname)
        ogs_info("nghttp2_server(%s) [%s://%s]:%d",
                server->interface ? server->interface : "",
                server->ssl_ctx ? "https" : "http",
                hostname, OGS_PORT(addr));
    else
        ogs_info("nghttp2_server(%s) [%s://%s]:%d",
                server->interface ? server->interface : "",
                server->ssl_ctx ? "https" : "http",
                OGS_ADDR(addr, buf), OGS_PORT(addr));

    return OGS_OK;
}

static void server_stop(ogs_sbi_server_t *server)
{
    ogs_assert(server);

    /* Free SSL CTX */
    if (server->ssl_ctx)
        SSL_CTX_free(server->ssl_ctx);

    if (server->node.poll)
        ogs_pollset_remove(server->node.poll);

    if (server->node.sock)
        ogs_sock_destroy(server->node.sock);

    session_remove_all(server);
}

static void add_header(nghttp2_nv *nv, const char *key, const char *value)
{
    nv->name = (uint8_t *)key;
    nv->namelen = strlen(key);
    nv->value = (uint8_t *)value;
    nv->valuelen = strlen(value);
    nv->flags = NGHTTP2_NV_FLAG_NONE;
}

static char status_string[600][4] = {
 "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
 "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
 "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
 "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
 "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
 "100", "101", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
 "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
 "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
 "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
 "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
 "200", "201", "202", "203", "204", "205", "206", "", "", "", "", "", "", "", "", "", "", "", "", "",
 "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
 "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
 "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
 "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
 "300", "301", "302", "303", "304", "305", "306", "307", "308", "", "", "", "", "", "", "", "", "", "", "",
 "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
 "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
 "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
 "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
 "400", "401", "402", "403", "404", "405", "406", "407", "408", "409",
 "410", "411", "412", "413", "414", "415", "416", "417", "", "",
 "", "421", "", "", "", "", "426", "", "428", "429", "", "431", "", "", "", "", "", "", "", "",
 "", "", "", "", "", "", "", "", "", "", "", "451", "", "", "", "", "", "", "", "",
 "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
 "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
 "500", "501", "502", "503", "504", "505", "", "", "", "", "", "511", "", "", "", "", "", "", "", "",
 "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
 "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
 "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
 "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""
};

#define DATE_STRLEN 128
static char *get_date_string(char *date)
{
    static const char *const days[] = {
        "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
    };
    static const char *const mons[] = {
        "Jan", "Feb", "Mar", "Apr", "May", "Jun",
        "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
    };

    ogs_assert(date);

    struct tm tm;
    ogs_gmtime(ogs_time_sec(ogs_time_now()), &tm);

    ogs_snprintf(date, DATE_STRLEN, "%3s, %02u %3s %04u %02u:%02u:%02u GMT",
            days[tm.tm_wday % 7],
            (unsigned int)tm.tm_mday,
            mons[tm.tm_mon % 12],
            (unsigned int)(1900 + tm.tm_year),
            (unsigned int)tm.tm_hour,
            (unsigned int)tm.tm_min,
            (unsigned int)tm.tm_sec);

    return date;
}

static ssize_t response_read_callback(nghttp2_session *session,
                                      int32_t stream_id,
                                      uint8_t *buf, size_t length,
                                      uint32_t *data_flags,
                                      nghttp2_data_source *source,
                                      void *user_data)
{
#if USE_SEND_DATA_WITH_NO_COPY
    int rv;
#endif

    ogs_sbi_response_t *response = NULL;
    ogs_sbi_stream_t *stream = NULL;

    ogs_assert(session);

    stream = nghttp2_session_get_stream_user_data(session, stream_id);
    if (!stream) {
        ogs_error("no stream [%d]", stream_id);
        return NGHTTP2_ERR_TEMPORAL_CALLBACK_FAILURE;
    }

    ogs_assert(source);
    response = source->ptr;
    ogs_assert(response);

    ogs_assert(response->http.content);
    ogs_assert(response->http.content_length);

#if USE_SEND_DATA_WITH_NO_COPY
    *data_flags |= NGHTTP2_DATA_FLAG_NO_COPY;
#else
    memcpy(buf, response->http.content, response->http.content_length);
#endif

    *data_flags |= NGHTTP2_DATA_FLAG_EOF;

#if USE_SEND_DATA_WITH_NO_COPY
    rv = nghttp2_session_get_stream_remote_close(session, stream_id);
    if (rv == 0) {
        ogs_warn("nghttp2_session_get_stream_remote_close() failed");
        nghttp2_submit_rst_stream(
                session, NGHTTP2_FLAG_NONE, stream_id, NGHTTP2_NO_ERROR);
    } else if (rv != 1) {
        ogs_error("nghttp2_session_get_stream_remote_close() failed[%d]", rv);
    }
#endif

    return response->http.content_length;
}

static bool server_send_rspmem_persistent(
        ogs_sbi_stream_t *stream, ogs_sbi_response_t *response)
{
    ogs_sbi_session_t *sbi_sess = NULL;
    ogs_sock_t *sock = NULL;
    ogs_socket_t fd = INVALID_SOCKET;

    ogs_hash_index_t *hi;
    nghttp2_nv *nva;
    size_t nvlen;
    int i, rv;
    char datebuf[DATE_STRLEN];
    char srv_version[128];
    char clen[128];

    ogs_assert(response);

    if (response->status >= 600) {
        ogs_error("Invalid response status [%d]", response->status);
        return false;
    }

    ogs_assert(stream);
    sbi_sess = stream->session;
    ogs_assert(sbi_sess);
    ogs_assert(sbi_sess->session);

    sock = sbi_sess->sock;
    ogs_assert(sock);
    fd = sock->fd;
    ogs_assert(fd != INVALID_SOCKET); /* Check if session is removed */

    nvlen = 3; /* :status && server && date */

    for (hi = ogs_hash_first(response->http.headers);
            hi; hi = ogs_hash_next(hi))
        nvlen++;

    if (response->http.content && response->http.content_length)
        nvlen++;

    nva = ogs_calloc(nvlen, sizeof(nghttp2_nv));
    if (!nva) {
        ogs_error("ogs_calloc() failed");
        return false;
    }

    i = 0;

    if (strlen(status_string[response->status]) != 3) {
        ogs_fatal("response status [%d]", response->status);
        ogs_fatal("status string [%s]", status_string[response->status]);
        ogs_assert_if_reached();
        return false;
    }

    add_header(&nva[i++], ":status", status_string[response->status]);

    ogs_snprintf(srv_version, sizeof(srv_version),
            "Open5GS %s", ogs_app()->version ? ogs_app()->version : "TEST");
    add_header(&nva[i++], "server", srv_version);
    add_header(&nva[i++], "date", get_date_string(datebuf));

    if (response->http.content && response->http.content_length) {
        ogs_snprintf(clen, sizeof(clen),
                "%d", (int)response->http.content_length);
        add_header(&nva[i++], "content-length", clen);
    }

    for (hi = ogs_hash_first(response->http.headers);
            hi; hi = ogs_hash_next(hi)) {
        add_header(&nva[i++], ogs_hash_this_key(hi), ogs_hash_this_val(hi));
    }

    ogs_debug("STATUS [%d]", response->status);

    if (response->http.content && response->http.content_length) {
        nghttp2_data_provider data_prd;

        data_prd.source.ptr = response;
        data_prd.read_callback = response_read_callback;

        ogs_debug("SENDING...: %d", (int)response->http.content_length);
        ogs_debug("%s", response->http.content);

        rv = nghttp2_submit_response(sbi_sess->session,
                stream->stream_id, nva, nvlen, &data_prd);
    } else {
        rv = nghttp2_submit_response(sbi_sess->session,
                stream->stream_id, nva, nvlen, NULL);
    }

    if (rv != OGS_OK) {
        ogs_error("nghttp2_submit_response(%d) failed (%d:%s)",
                    (int)response->http.content_length,
                    rv, nghttp2_strerror(rv));
        nghttp2_submit_rst_stream(
                sbi_sess->session, NGHTTP2_FLAG_NONE, stream->stream_id, rv);
    }

    if (session_send(sbi_sess) != OGS_OK) {
        ogs_error("session_send() failed");
        session_remove(sbi_sess);
    }

    ogs_free(nva);

    return true;
}

static bool server_send_response(
        ogs_sbi_stream_t *stream, ogs_sbi_response_t *response)
{
    bool rc;

    ogs_assert(response);

    rc = server_send_rspmem_persistent(stream, response);

    ogs_sbi_response_free(response);

    return rc;
}

static ogs_sbi_server_t *server_from_stream(ogs_sbi_stream_t *stream)
{
    ogs_sbi_session_t *sbi_sess = NULL;

    ogs_assert(stream);
    sbi_sess = stream->session;
    ogs_assert(sbi_sess);
    ogs_assert(sbi_sess->server);

    return sbi_sess->server;
}

static ogs_sbi_stream_t *stream_add(
        ogs_sbi_session_t *sbi_sess, int32_t stream_id)
{
    ogs_sbi_stream_t *stream = NULL;

    ogs_assert(sbi_sess);

    ogs_pool_id_calloc(&stream_pool, &stream);
    if (!stream) {
        ogs_error("ogs_pool_id_calloc() failed");
        return NULL;
    }

    stream->request = ogs_sbi_request_new();
    if (!stream->request) {
        ogs_error("ogs_sbi_request_new() failed");
        ogs_pool_id_free(&stream_pool, stream);
        return NULL;
    }

    stream->stream_id = stream_id;
    sbi_sess->last_stream_id = stream_id;

    stream->session = sbi_sess;

    ogs_list_add(&sbi_sess->stream_list, stream);

    return stream;
}

static void stream_remove(ogs_sbi_stream_t *stream)
{
    ogs_sbi_session_t *sbi_sess = NULL;

    ogs_assert(stream);
    sbi_sess = stream->session;
    ogs_assert(sbi_sess);

    ogs_list_remove(&sbi_sess->stream_list, stream);

    ogs_assert(stream->request);
    ogs_sbi_request_free(stream->request);

    ogs_pool_id_free(&stream_pool, stream);
}

static void stream_remove_all(ogs_sbi_session_t *sbi_sess)
{
    ogs_sbi_stream_t *stream = NULL, *next_stream = NULL;

    ogs_assert(sbi_sess);

    ogs_list_for_each_safe(&sbi_sess->stream_list, next_stream, stream)
        stream_remove(stream);
}

static ogs_pool_id_t id_from_stream(ogs_sbi_stream_t *stream)
{
    ogs_assert(stream);
    return stream->id;
}

static void *stream_find_by_id(ogs_pool_id_t id)
{
    return ogs_pool_find_by_id(&stream_pool, id);
}

static ogs_sbi_session_t *session_add(
        ogs_sbi_server_t *server, ogs_sock_t *sock)
{
    ogs_sbi_session_t *sbi_sess = NULL;

    ogs_assert(server);
    ogs_assert(sock);

    ogs_pool_alloc(&session_pool, &sbi_sess);
    if (!sbi_sess) {
        ogs_error("ogs_pool_alloc() failed");
        return NULL;
    }
    memset(sbi_sess, 0, sizeof(ogs_sbi_session_t));

    sbi_sess->server = server;
    sbi_sess->sock = sock;

    sbi_sess->addr = ogs_calloc(1, sizeof(ogs_sockaddr_t));
    if (!sbi_sess->addr) {
        ogs_error("ogs_calloc() failed");
        ogs_pool_free(&session_pool, sbi_sess);
        return NULL;
    }
    memcpy(sbi_sess->addr, &sock->remote_addr, sizeof(ogs_sockaddr_t));

    if (server->ssl_ctx) {
        char *context = NULL;

        sbi_sess->ssl = SSL_new(server->ssl_ctx);
        if (!sbi_sess->ssl) {
            ogs_error("SSL_new() failed");
            ogs_free(sbi_sess->addr);
            ogs_pool_free(&session_pool, sbi_sess);
            return NULL;
        }

        context = ogs_msprintf("%d",
                (int)ogs_pool_index(&session_pool, sbi_sess));
        if (!context) {
            ogs_error("No memory for session id context");
            SSL_free(sbi_sess->ssl);
            ogs_free(sbi_sess->addr);
            ogs_pool_free(&session_pool, sbi_sess);
            return NULL;
        }

        if (!SSL_set_session_id_context(
                    sbi_sess->ssl, (unsigned char *)context, strlen(context))) {
            ogs_error("SSL_set_session_id_context() failed");
            ogs_free(context);
            ogs_free(sbi_sess->addr);
            SSL_free(sbi_sess->ssl);
            ogs_pool_free(&session_pool, sbi_sess);
            return NULL;
        }

        ogs_free(context);
    }

    ogs_list_add(&server->session_list, sbi_sess);

    return sbi_sess;
}

static void session_remove(ogs_sbi_session_t *sbi_sess)
{
    ogs_sbi_server_t *server = NULL;
    ogs_pkbuf_t *pkbuf = NULL, *next_pkbuf = NULL;

    ogs_assert(sbi_sess);
    server = sbi_sess->server;
    ogs_assert(server);

    ogs_list_remove(&server->session_list, sbi_sess);

    if (sbi_sess->ssl)
        SSL_free(sbi_sess->ssl);

    stream_remove_all(sbi_sess);
    nghttp2_session_del(sbi_sess->session);

    if (sbi_sess->poll.read)
        ogs_pollset_remove(sbi_sess->poll.read);

    if (sbi_sess->poll.write)
        ogs_pollset_remove(sbi_sess->poll.write);

    ogs_list_for_each_safe(&sbi_sess->write_queue, next_pkbuf, pkbuf) {
        ogs_list_remove(&sbi_sess->write_queue, pkbuf);
        ogs_pkbuf_free(pkbuf);
    }

    ogs_assert(sbi_sess->addr);
    ogs_free(sbi_sess->addr);

    ogs_assert(sbi_sess->sock);
    ogs_sock_destroy(sbi_sess->sock);

    ogs_pool_free(&session_pool, sbi_sess);
}

static void session_remove_all(ogs_sbi_server_t *server)
{
    ogs_sbi_session_t *sbi_sess = NULL, *next_sbi_sess = NULL;

    ogs_assert(server);

    ogs_list_for_each_safe(&server->session_list, next_sbi_sess, sbi_sess)
        session_remove(sbi_sess);
}

static void accept_handler(short when, ogs_socket_t fd, void *data)
{
    ogs_sbi_server_t *server = data;
    ogs_sbi_session_t *sbi_sess = NULL;
    ogs_sock_t *sock = NULL;
    ogs_sock_t *new = NULL;

    int on;

    ogs_assert(data);
    ogs_assert(fd != INVALID_SOCKET);

    sock = server->node.sock;

    new = ogs_sock_accept(sock);
    if (!new) {
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno, "accept() failed");
        return;
    }
    ogs_assert(new->fd != INVALID_SOCKET);

    on = 1;
    if (setsockopt(new->fd, IPPROTO_TCP, TCP_NODELAY, &on, sizeof(on)) != 0) {
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
                "setsockopt() for SCTP_NODELAY failed");
        ogs_sock_destroy(new);
        return;
    }

    sbi_sess = session_add(server, new);
    ogs_assert(sbi_sess);

    if (sbi_sess->ssl) {
        int err;
        SSL_set_fd(sbi_sess->ssl, new->fd);
        SSL_set_accept_state(sbi_sess->ssl);
        err = SSL_accept(sbi_sess->ssl);
        if (err <= 0) {
            ogs_error("SSL_accept failed [%s]", ERR_error_string(ERR_get_error(), NULL));
            session_remove(sbi_sess);
            return;
        }
    }

    sbi_sess->poll.read = ogs_pollset_add(ogs_app()->pollset,
        OGS_POLLIN, new->fd, recv_handler, sbi_sess);
    ogs_assert(sbi_sess->poll.read);

    if (session_set_callbacks(sbi_sess) != OGS_OK ||
        session_send_preface(sbi_sess) != OGS_OK) {
        ogs_error("session_add() failed");
        session_remove(sbi_sess);
    }
}

static void recv_handler(short when, ogs_socket_t fd, void *data)
{
    char buf[OGS_ADDRSTRLEN];
    ogs_sockaddr_t *addr = NULL;

    ogs_sbi_session_t *sbi_sess = data;
    ogs_pkbuf_t *pkbuf = NULL;
    ssize_t readlen;
    int n;

    ogs_assert(sbi_sess);
    ogs_assert(fd != INVALID_SOCKET);
    addr = sbi_sess->addr;
    ogs_assert(addr);

    pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_assert(pkbuf);

    if (sbi_sess->ssl)
        n = SSL_read(sbi_sess->ssl, pkbuf->data, OGS_MAX_SDU_LEN);
    else
        n = ogs_recv(fd, pkbuf->data, OGS_MAX_SDU_LEN, 0);

    if (n > 0) {
        ogs_pkbuf_put(pkbuf, n);

        ogs_assert(sbi_sess->session);
        readlen = nghttp2_session_mem_recv(
                sbi_sess->session, pkbuf->data, pkbuf->len);
        if (readlen < 0) {
            ogs_error("nghttp2_session_mem_recv() failed (%d:%s)",
                        (int)readlen, nghttp2_strerror((int)readlen));
            session_remove(sbi_sess);
        } else {
            /*
             * Issues #2385
             *
             * Nokia AMF is sending GOAWAY because it didn't get
             * ACK SETTINGS packet for the SETTINGS it set,
             * this is according to http2 RFC, all settings must be
             * ACK or connection will be dropped.
             *
             * Open5GS is not ACKing pure settings packets,
             * looks like it is waiting for a header
             * like POST/GET first to trigger
             * sending settings ACK and then headers reply.
             */

            /*
             * [SOLVED]
             *
             * Whether or not to send a Setting ACK is determined
             * by the nghttp2 library. Therefore, when nghttp2 informs us
             * that it want to send an SETTING frame with ACK
             * by nghttp2_session_want_write(), we need to call session_send()
             * directly to send it.
             */
            if (nghttp2_session_want_write(sbi_sess->session))
                session_send(sbi_sess);
        }
    } else {
        if (n < 0) {
            if (errno != OGS_ECONNRESET)
                ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
                                "lost connection [%s]:%d",
                                OGS_ADDR(addr, buf), OGS_PORT(addr));
        } else if (n == 0) {
            ogs_debug("connection closed [%s]:%d",
                        OGS_ADDR(addr, buf), OGS_PORT(addr));
        }

        session_remove(sbi_sess);
    }

    ogs_pkbuf_free(pkbuf);
}

static int on_frame_recv(nghttp2_session *session,
                         const nghttp2_frame *frame, void *user_data);
static int on_stream_close(nghttp2_session *session, int32_t stream_id,
                           uint32_t error_code, void *user_data);
static int on_header(nghttp2_session *session,
                     const nghttp2_frame *frame,
                     nghttp2_rcbuf *name, nghttp2_rcbuf *value,
                     uint8_t flags, void *user_data);
static int on_data_chunk_recv(nghttp2_session *session, uint8_t flags,
                              int32_t stream_id, const uint8_t *data,
                              size_t len, void *user_data);
static int error_callback(nghttp2_session *session,
                          const char *msg, size_t len, void *user_data);
static int on_invalid_frame_recv(nghttp2_session *session,
                                 const nghttp2_frame *frame,
                                 int error_code, void *user_data);
static int on_invalid_header(nghttp2_session *session,
                             const nghttp2_frame *frame,
                             const uint8_t *name, size_t namelen,
                             const uint8_t *value, size_t valuelen,
                             uint8_t flags, void *user_data);
static int on_begin_frame(nghttp2_session *session,
                          const nghttp2_frame_hd *hd, void *user_data);
static int on_begin_headers(nghttp2_session *session,
                            const nghttp2_frame *frame,
                                     void *user_data);
#if USE_SEND_DATA_WITH_NO_COPY
static int on_send_data(nghttp2_session *session, nghttp2_frame *frame,
                        const uint8_t *framehd, size_t length,
                        nghttp2_data_source *source, void *user_data);
#else
static ssize_t send_callback(nghttp2_session *session, const uint8_t *data,
                             size_t length, int flags, void *user_data);
#endif

static int session_set_callbacks(ogs_sbi_session_t *sbi_sess)
{
    int rv;
    nghttp2_session_callbacks *callbacks = NULL;

    ogs_assert(sbi_sess);

    rv = nghttp2_session_callbacks_new(&callbacks);
    if (rv != 0) {
        ogs_error("nghttp2_session_callbacks_new() failed (%d:%s)",
                    rv, nghttp2_strerror(rv));
        return OGS_ERROR;
    }

    nghttp2_session_callbacks_set_on_frame_recv_callback(
            callbacks, on_frame_recv);

    nghttp2_session_callbacks_set_on_stream_close_callback(
            callbacks, on_stream_close);

    nghttp2_session_callbacks_set_on_header_callback2(callbacks, on_header);

    nghttp2_session_callbacks_set_on_data_chunk_recv_callback(
            callbacks, on_data_chunk_recv);

    nghttp2_session_callbacks_set_error_callback(callbacks, error_callback);

    nghttp2_session_callbacks_set_on_invalid_frame_recv_callback(
            callbacks, on_invalid_frame_recv);

    nghttp2_session_callbacks_set_on_invalid_header_callback(
            callbacks, on_invalid_header);

    nghttp2_session_callbacks_set_on_begin_frame_callback(
            callbacks, on_begin_frame);

    nghttp2_session_callbacks_set_on_begin_headers_callback(
            callbacks, on_begin_headers);

#if USE_SEND_DATA_WITH_NO_COPY
    nghttp2_session_callbacks_set_send_data_callback(callbacks, on_send_data);
#else
    nghttp2_session_callbacks_set_send_callback(callbacks, send_callback);
#endif

    rv = nghttp2_session_server_new(&sbi_sess->session, callbacks, sbi_sess);
    if (rv != 0) {
        ogs_error("nghttp2_session_callbacks_new() failed (%d:%s)",
                    rv, nghttp2_strerror(rv));
        return OGS_ERROR;
    }

    nghttp2_session_callbacks_del(callbacks);

    return OGS_OK;
}

static int on_frame_recv(nghttp2_session *session,
                         const nghttp2_frame *frame, void *user_data)
{
    int rv;
    ogs_sbi_session_t *sbi_sess = user_data;

    ogs_sbi_server_t *server = NULL;
    ogs_sbi_stream_t *stream = NULL;
    ogs_sbi_request_t *request = NULL;

    ogs_assert(sbi_sess);
    server = sbi_sess->server;
    ogs_assert(server);
    ogs_assert(server->cb);

    ogs_assert(session);
    ogs_assert(frame);

    switch (frame->hd.type) {
    case NGHTTP2_HEADERS:
        stream = nghttp2_session_get_stream_user_data(
                session, frame->hd.stream_id);
        if (!stream) return 0;

        request = stream->request;
        ogs_assert(request);

        if (frame->headers.cat == NGHTTP2_HCAT_REQUEST) {
            const char *expect100 =
                ogs_sbi_header_get(request->http.headers, OGS_SBI_EXPECT);
            if (expect100 && ogs_strcasecmp(expect100, "100-continue") == 0) {
                nghttp2_nv nva;

                add_header(&nva, ":status", status_string[100]);
                rv = nghttp2_submit_headers(session, NGHTTP2_FLAG_NONE,
                           stream->stream_id, NULL, &nva, 1, NULL);
                if (rv != 0) {
                    ogs_error("nghttp2_submit_headers() failed (%d:%s)",
                            rv, nghttp2_strerror(rv));
                    nghttp2_submit_rst_stream(
                            session, NGHTTP2_FLAG_NONE, stream->stream_id, rv);
                    return 0;
                }
            }
        }
        /* fallthrough */
        OGS_GNUC_FALLTHROUGH;

    case NGHTTP2_DATA:
        stream = nghttp2_session_get_stream_user_data(
                session, frame->hd.stream_id);
        if (!stream) return 0;

        request = stream->request;
        ogs_assert(request);

        /* HEADERS or DATA frame with +END_STREAM flag */
        if (frame->hd.flags & NGHTTP2_FLAG_END_STREAM) {
            ogs_log_level_e level = OGS_LOG_DEBUG;

            if (stream->memory_overflow == true)
                level = OGS_LOG_ERROR;

            ogs_log_message(level, 0,
                    "[%s] %s", request->h.method, request->h.uri);

            if (request->http.content_length && request->http.content) {
                ogs_log_message(level, 0,
                        "RECEIVED: %d", (int)request->http.content_length);
                ogs_log_message(level, 0, "%s", request->http.content);
            }

            if (stream->memory_overflow == true) {
                ogs_error("[DROP] Overflow");
                break;
            }

            if (server->cb(request,
                        OGS_UINT_TO_POINTER(stream->id)) != OGS_OK) {
                ogs_warn("server callback error");
                ogs_assert(true ==
                    ogs_sbi_server_send_error(stream,
                        OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR, NULL,
                        "server callback error", NULL, NULL));

                return 0;
            }
        } else {
            /* TODO : Need to implement the timeouf of reading STREAM */
        }
        break;
    case NGHTTP2_SETTINGS:
        ogs_debug("FLAGS(0x%x) [%s]",
                frame->hd.flags,
                frame->hd.flags & NGHTTP2_FLAG_ACK ? "ACK" : "NO-ACK");

        if ((frame->hd.flags & NGHTTP2_FLAG_ACK) == 0) {
            sbi_sess->settings.max_concurrent_streams =
                nghttp2_session_get_remote_settings(
                    session, NGHTTP2_SETTINGS_MAX_CONCURRENT_STREAMS);
            sbi_sess->settings.enable_push =
                nghttp2_session_get_remote_settings(
                    session, NGHTTP2_SETTINGS_ENABLE_PUSH);
            ogs_debug("MAX_CONCURRENT_STREAMS = %d",
                sbi_sess->settings.max_concurrent_streams);
            ogs_debug("ENABLE_PUSH = %s",
                sbi_sess->settings.enable_push ? "TRUE" : "false");

            return 0;
        }

        /*
         * TODO:
         *
         * if Setting ACK received, need to stop Timer.
         * Otherwise, we need to send NGHTTP2_GOAWAY
         */
        break;
    case NGHTTP2_GOAWAY:
#if 0 /* It is not neeed in other nghttp2 example */
        rv = nghttp2_submit_goaway(
             session, NGHTTP2_FLAG_NONE, sbi_sess->last_stream_id,
             NGHTTP2_NO_ERROR, NULL, 0);
        if (rv != 0) {
            ogs_error("nghttp2_submit_goaway() failed (%d:%s)",
                        rv, nghttp2_strerror(rv));
            return OGS_ERROR;
        }

        session_send(sbi_sess);
#endif
        ogs_info("GOAWAY received: last-stream-id=%d",
                frame->goaway.last_stream_id);
        ogs_info("error_code=%d", frame->goaway.error_code);
        break;
    case NGHTTP2_RST_STREAM:
        ogs_info("RST_STREAM received: stream_id=%d", frame->hd.stream_id);
        break;
    case NGHTTP2_PING:
        if (frame->hd.flags & NGHTTP2_FLAG_ACK)
            ogs_info("PING ACK received");
        break;
    case NGHTTP2_PUSH_PROMISE:
        ogs_info("PUSH_PROMISE recieved: stream_id=%d", frame->hd.stream_id);
        ogs_info("promised_stream_id=%d",
                frame->push_promise.promised_stream_id);
        break;
    default:
        break;
    }

    return 0;
}

static int on_stream_close(nghttp2_session *session, int32_t stream_id,
                           uint32_t error_code, void *user_data)
{
    ogs_sbi_stream_t *stream = NULL;

    ogs_assert(session);

    stream = nghttp2_session_get_stream_user_data(session, stream_id);
    if (!stream) {
        ogs_error("no stream [%d]", stream_id);
        return NGHTTP2_ERR_TEMPORAL_CALLBACK_FAILURE;
    }

    if (error_code) {
        ogs_error("on_stream_close_callback() failed (%d:%s)",
                    error_code, nghttp2_http2_strerror(error_code));
        nghttp2_submit_rst_stream(
                session, NGHTTP2_FLAG_NONE, stream_id, error_code);
    }

    ogs_debug("STREAM closed [%d]", stream_id);
    stream_remove(stream);
    return 0;
}

static int on_header(nghttp2_session *session, const nghttp2_frame *frame,
                     nghttp2_rcbuf *name, nghttp2_rcbuf *value,
                     uint8_t flags, void *user_data)
{
    ogs_sbi_session_t *sbi_sess = user_data;
    ogs_sbi_stream_t *stream = NULL;
    ogs_sbi_request_t *request = NULL;

    const char PATH[] = ":path";
    const char METHOD[] = ":method";

    nghttp2_vec namebuf, valuebuf;
    char *namestr = NULL, *valuestr = NULL;

    ogs_assert(session);
    ogs_assert(frame);

    if (frame->hd.type != NGHTTP2_HEADERS ||
        frame->headers.cat != NGHTTP2_HCAT_REQUEST) {
        return 0;
    }

    stream = nghttp2_session_get_stream_user_data(session, frame->hd.stream_id);
    if (!stream) {
        ogs_error("no stream [%d]", frame->hd.stream_id);
        return NGHTTP2_ERR_TEMPORAL_CALLBACK_FAILURE;
    }

    ogs_assert(sbi_sess);

    request = stream->request;
    ogs_assert(request);

    ogs_assert(name);
    namebuf = nghttp2_rcbuf_get_buf(name);
    ogs_assert(namebuf.base);
    ogs_assert(namebuf.len);

    ogs_assert(value);
    valuebuf = nghttp2_rcbuf_get_buf(value);
    ogs_assert(valuebuf.base);

    if (valuebuf.len == 0) return 0;

    namestr = ogs_strndup((const char *)namebuf.base, namebuf.len);
    ogs_assert(namestr);

    valuestr = ogs_strndup((const char *)valuebuf.base, valuebuf.len);
    ogs_assert(valuestr);

    if (namebuf.len == sizeof(PATH) - 1 &&
            memcmp(PATH, namebuf.base, namebuf.len) == 0) {
        char *saveptr = NULL, *query;
#define MAX_NUM_OF_PARAM_IN_QUERY 16
        struct yuarel_param params[MAX_NUM_OF_PARAM_IN_QUERY+2];
        int j;

        ogs_assert(request->h.uri == NULL);
        request->h.uri = ogs_sbi_parse_uri(valuestr, "?", &saveptr);
        ogs_assert(request->h.uri);

        memset(params, 0, sizeof(params));

        query = ogs_sbi_parse_uri(NULL, "?", &saveptr);
        if (query && *query && strlen(query))
            yuarel_parse_query(query, '&', params, MAX_NUM_OF_PARAM_IN_QUERY+1);

        j = 0;
        while(params[j].key && params[j].val) {
            if (strlen(params[j].key))
                ogs_sbi_header_set(request->http.params,
                        params[j].key, params[j].val);
            else
                ogs_warn("No KEY in Query-Parms");

            j++;
        }

        if (j >= MAX_NUM_OF_PARAM_IN_QUERY+1) {
            ogs_fatal("Maximum number(%d) of query params reached",
                    MAX_NUM_OF_PARAM_IN_QUERY);
            ogs_assert_if_reached();
        }

        ogs_free(query);

    } else if (namebuf.len == sizeof(METHOD) - 1 &&
            memcmp(METHOD, namebuf.base, namebuf.len) == 0) {

        ogs_assert(request->h.method == NULL);
        request->h.method = ogs_strdup(valuestr);
        ogs_assert(request->h.method);

    } else {

        ogs_sbi_header_set(request->http.headers, namestr, valuestr);

    }

    ogs_free(namestr);
    ogs_free(valuestr);

    return 0;
}

static int on_data_chunk_recv(nghttp2_session *session, uint8_t flags,
                              int32_t stream_id, const uint8_t *data,
                              size_t len, void *user_data)
{
    ogs_sbi_stream_t *stream = NULL;
    ogs_sbi_request_t *request = NULL;

    size_t offset = 0;

    ogs_assert(session);

    stream = nghttp2_session_get_stream_user_data(session, stream_id);
    if (!stream) {
        ogs_error("no stream [%d]", stream_id);
        return 0;
    }

    request = stream->request;
    ogs_assert(request);

    ogs_assert(data);
    ogs_assert(len);

    if (request->http.content == NULL) {
        ogs_assert(request->http.content_length == 0);
        ogs_assert(offset == 0);

        request->http.content = (char*)ogs_malloc(len + 1);
    } else {
        ogs_assert(request->http.content_length != 0);

        request->http.content = (char*)ogs_realloc(
                request->http.content, request->http.content_length + len + 1);
    }

    if (!request->http.content) {
        stream->memory_overflow = true;

        ogs_error("Overflow : Content-Length[%d], len[%d]",
                    (int)request->http.content_length, (int)len);
        ogs_log_hexdump(OGS_LOG_ERROR, data, len);

        return 0;
    }

    offset = request->http.content_length;
    request->http.content_length += len;

    memcpy(request->http.content + offset, data, len);
    request->http.content[request->http.content_length] = '\0';

    return 0;
}

static int error_callback(nghttp2_session *session,
                          const char *msg, size_t len, void *user_data)
{
    char buf[OGS_ADDRSTRLEN];
    ogs_sockaddr_t *addr = NULL;
    ogs_sbi_session_t *sbi_sess = user_data;

    ogs_assert(sbi_sess);
    addr = sbi_sess->addr;
    ogs_assert(addr);

    ogs_assert(msg);

    ogs_error("[%s]:%d http2 error: %.*s",
            OGS_ADDR(addr, buf), OGS_PORT(addr), (int)len, msg);

    return 0;
}

static int on_invalid_frame_recv(nghttp2_session *session,
                                 const nghttp2_frame *frame,
                                 int error_code, void *user_data)
{
    char buf[OGS_ADDRSTRLEN];
    ogs_sockaddr_t *addr = NULL;

    ogs_sbi_session_t *sbi_sess = user_data;

    ogs_assert(sbi_sess);
    addr = sbi_sess->addr;
    ogs_assert(addr);

    ogs_error("[%s]:%d invalid frame (%d:%s)",
            OGS_ADDR(addr, buf), OGS_PORT(addr),
            error_code, nghttp2_strerror(error_code));
    return 0;
}

static int on_invalid_header(nghttp2_session *session,
                             const nghttp2_frame *frame,
                             const uint8_t *name, size_t namelen,
                             const uint8_t *value, size_t valuelen,
                             uint8_t flags, void *user_data)
{
    char buf[OGS_ADDRSTRLEN];
    ogs_sockaddr_t *addr = NULL;
    char *namestr = NULL, *valuestr = NULL;

    ogs_sbi_session_t *sbi_sess = user_data;

    ogs_assert(sbi_sess);
    addr = sbi_sess->addr;
    ogs_assert(addr);

    namestr = ogs_strndup((const char *)name, namelen);
    ogs_assert(namestr);

    valuestr = ogs_strndup((const char *)value, valuelen);
    ogs_assert(valuestr);

    ogs_error("[%s]:%d invalid header (%s:%s)",
            OGS_ADDR(addr, buf), OGS_PORT(addr), namestr, valuestr);

    ogs_free(namestr);
    ogs_free(valuestr);

    return 0;
}

static int on_begin_frame(nghttp2_session *session, const nghttp2_frame_hd *hd,
                          void *user_data)
{
    char buf[OGS_ADDRSTRLEN];
    ogs_sockaddr_t *addr = NULL;
    ogs_sbi_session_t *sbi_sess = user_data;

    ogs_assert(sbi_sess);
    addr = sbi_sess->addr;
    ogs_assert(addr);

    ogs_assert(hd);

    if ((hd->type == NGHTTP2_HEADERS) &&
        (hd->stream_id < sbi_sess->last_stream_id)) {
        ogs_error("[%s]:%d invalid stream id(%d) >= last stream id(%d)",
                OGS_ADDR(addr, buf), OGS_PORT(addr),
                hd->stream_id, sbi_sess->last_stream_id);
        return NGHTTP2_ERR_CALLBACK_FAILURE;
    }

    return 0;
}

static int on_begin_headers(nghttp2_session *session,
                            const nghttp2_frame *frame, void *user_data)
{
    ogs_sbi_session_t *sbi_sess = user_data;
    ogs_sbi_stream_t *stream = NULL;

    ogs_assert(sbi_sess);
    ogs_assert(session);
    ogs_assert(frame);

    if (frame->hd.type != NGHTTP2_HEADERS ||
        frame->headers.cat != NGHTTP2_HCAT_REQUEST) {
        return 0;
    }

    stream = stream_add(sbi_sess, frame->hd.stream_id);
    ogs_assert(stream);
    ogs_debug("STREAM added [%d]", frame->hd.stream_id);

    nghttp2_session_set_stream_user_data(session, frame->hd.stream_id, stream);

    return 0;
}

static int session_send_preface(ogs_sbi_session_t *sbi_sess)
{
    int rv;
    nghttp2_settings_entry iv[1] = {
        { NGHTTP2_SETTINGS_MAX_CONCURRENT_STREAMS, ogs_app()->pool.stream }
    };

    ogs_assert(sbi_sess);
    ogs_assert(sbi_sess->session);

    rv = nghttp2_submit_settings(
            sbi_sess->session, NGHTTP2_FLAG_NONE, iv, OGS_ARRAY_SIZE(iv));
    if (rv != 0) {
        ogs_error("nghttp2_submit_settings() failed (%d:%s)",
                    rv, nghttp2_strerror(rv));
        return OGS_ERROR;
    }

    return session_send(sbi_sess);
}

#if USE_SEND_DATA_WITH_NO_COPY
static int on_send_data(nghttp2_session *session, nghttp2_frame *frame,
                        const uint8_t *framehd, size_t length,
                        nghttp2_data_source *source, void *user_data)
{
    ogs_sbi_session_t *sbi_sess = user_data;

    ogs_sbi_response_t *response = NULL;
    ogs_sbi_stream_t *stream = NULL;
    ogs_pkbuf_t *pkbuf = NULL;
    size_t padlen = 0;

    ogs_assert(session);
    ogs_assert(frame);

    stream = nghttp2_session_get_stream_user_data(session, frame->hd.stream_id);
    if (!stream) {
        ogs_error("no stream [%d]", frame->hd.stream_id);
        return NGHTTP2_ERR_TEMPORAL_CALLBACK_FAILURE;
    }

    ogs_assert(sbi_sess);

    ogs_assert(source);
    response = source->ptr;
    ogs_assert(response);

    ogs_assert(response->http.content);
    ogs_assert(response->http.content_length);

    ogs_assert(framehd);
    ogs_assert(length);

    pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_assert(pkbuf);
    ogs_pkbuf_put_data(pkbuf, framehd, 9);

    padlen = frame->data.padlen;

    if (padlen > 0) {
        ogs_pkbuf_put_u8(pkbuf, padlen-1);
    }

    ogs_pkbuf_put_data(pkbuf,
            response->http.content, response->http.content_length);

    if (padlen > 0) {
        memset(pkbuf->tail, 0, padlen-1);
        ogs_pkbuf_put(pkbuf, padlen-1);
    }

    session_write_to_buffer(sbi_sess, pkbuf);

    return 0;
}
#else
static ssize_t send_callback(nghttp2_session *session, const uint8_t *data,
                             size_t length, int flags, void *user_data)
{
    ogs_sbi_session_t *sbi_sess = user_data;
    ogs_sock_t *sock = NULL;
    ogs_socket_t fd = INVALID_SOCKET;

    ogs_pkbuf_t *pkbuf = NULL;

    ogs_assert(sbi_sess);
    sock = sbi_sess->sock;
    ogs_assert(sock);
    fd = sock->fd;
    ogs_assert(fd != INVALID_SOCKET);

    ogs_assert(data);
    ogs_assert(length);

    pkbuf = ogs_pkbuf_alloc(NULL, length);
    ogs_assert(pkbuf);
    ogs_pkbuf_put_data(pkbuf, data, length);

    session_write_to_buffer(sbi_sess, pkbuf);

    return length;
}
#endif

static int session_send(ogs_sbi_session_t *sbi_sess)
{
#if USE_SEND_DATA_WITH_NO_COPY
    ogs_pkbuf_t *pkbuf = NULL;
#else
    int rv;
#endif

    ogs_assert(sbi_sess);
    ogs_assert(sbi_sess->session);

#if USE_SEND_DATA_WITH_NO_COPY
    for (;;) {
        const uint8_t *data = NULL;
        ssize_t data_len;

        data_len = nghttp2_session_mem_send(sbi_sess->session, &data);
        if (data_len < 0) {
            ogs_error("nghttp2_session_mem_send() failed (%d:%s)",
                        (int)data_len, nghttp2_strerror((int)data_len));
            return OGS_ERROR;
        }

        if (data_len == 0) {
            break;
        }

        pkbuf = ogs_pkbuf_alloc(NULL, data_len);
        ogs_assert(pkbuf);
        ogs_pkbuf_put_data(pkbuf, data, data_len);

        session_write_to_buffer(sbi_sess, pkbuf);
    }
#else
    rv = nghttp2_session_send(sbi_sess->session);
    if (rv != 0) {
        ogs_error("nghttp_session_send() failed (%d:%s)",
                    rv, nghttp2_strerror(rv));
        return OGS_ERROR;
    }
#endif

    return OGS_OK;
}

static void session_write_callback(short when, ogs_socket_t fd, void *data)
{
    ogs_sbi_session_t *sbi_sess = data;
    ogs_pkbuf_t *pkbuf = NULL;

    ogs_assert(sbi_sess);

    if (ogs_list_empty(&sbi_sess->write_queue) == true) {
        ogs_assert(sbi_sess->poll.write);
        ogs_pollset_remove(sbi_sess->poll.write);
        sbi_sess->poll.write = NULL;
        return;
    }

    pkbuf = ogs_list_first(&sbi_sess->write_queue);
    ogs_assert(pkbuf);
    ogs_list_remove(&sbi_sess->write_queue, pkbuf);

    if (sbi_sess->ssl)
        SSL_write(sbi_sess->ssl, pkbuf->data, pkbuf->len);
    else
        ogs_send(fd, pkbuf->data, pkbuf->len, 0);

    ogs_log_hexdump(OGS_LOG_DEBUG, pkbuf->data, pkbuf->len);

    ogs_pkbuf_free(pkbuf);
}

static void session_write_to_buffer(
        ogs_sbi_session_t *sbi_sess, ogs_pkbuf_t *pkbuf)
{
    ogs_sock_t *sock = NULL;
    ogs_socket_t fd = INVALID_SOCKET;

    ogs_assert(pkbuf);

    ogs_assert(sbi_sess);
    sock = sbi_sess->sock;
    ogs_assert(sock);
    fd = sock->fd;
    ogs_assert(fd != INVALID_SOCKET);

    ogs_list_add(&sbi_sess->write_queue, pkbuf);

    if (!sbi_sess->poll.write) {
        sbi_sess->poll.write = ogs_pollset_add(ogs_app()->pollset,
            OGS_POLLOUT, fd, session_write_callback, sbi_sess);
        ogs_assert(sbi_sess->poll.write);
    }
}
