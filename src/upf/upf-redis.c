#include <hiredis/hiredis.h>
#include <json-c/json.h>
#include "upf-redis.h"
#include "timer.h"
#include "ogs-core.h"

static upf_redis_context_t self;
static ogs_timer_mgr_t *timer_mgr = NULL;


static void upf_redis_reconnect_timer_expire(void *data);

int upf_redis_init(void)
{
    const char *redis_host;
    const char *redis_port_str;
    int redis_port;

    if (!timer_mgr) {
        timer_mgr = ogs_timer_mgr_create(10);
        if (!timer_mgr) {
            ogs_error("Cannot create timer manager");
            return OGS_ERROR;
        }
    }

    /* Get Redis configuration from environment */
    redis_host = getenv("REDIS_HOST");
    if (!redis_host) redis_host = "localhost";

    redis_port_str = getenv("REDIS_PORT");
    if (!redis_port_str) redis_port = 6379;
    else redis_port = atoi(redis_port_str);

    /* Initialize Redis context */
    memset(&self, 0, sizeof(upf_redis_context_t));

    /* Create socket address for Redis connection */
    self.addr = ogs_calloc(1, sizeof(ogs_sockaddr_t));
    if (!self.addr) {
        ogs_error("ogs_calloc() failed");
        return OGS_ERROR;
    }


    ((struct sockaddr_in *)self.addr)->sin_family = AF_INET;
    ((struct sockaddr_in *)self.addr)->sin_port = htons(redis_port);


    /* Create reconnect timer */
    self.t_reconnect = ogs_timer_add(ogs_app()->timer_mgr,
        upf_redis_reconnect_timer_expire, NULL);
    if (!self.t_reconnect) {
        ogs_error("Cannot create Redis reconnect timer");
        ogs_free(self.addr);
        return OGS_ERROR;
    }


    return upf_redis_connect();
}

void upf_redis_final(void)
{
    upf_redis_disconnect();

    if (self.t_reconnect)
        ogs_timer_delete(self.t_reconnect);

    if (self.addr)
        ogs_freeaddrinfo(self.addr);

    if (timer_mgr)
        ogs_timer_mgr_destroy(timer_mgr);
}

int upf_redis_connect(void)
{
    redisContext *redis_ctx;
    struct timeval timeout = { 1, 500000 }; /* 1.5 seconds */
    char ip_str[OGS_ADDRSTRLEN];
    uint16_t port;

    ogs_assert(self.addr);
    ogs_inet_ntop(&self.addr->sa, ip_str, sizeof(ip_str));
    port = ntohs(((struct sockaddr_in *)self.addr)->sin_port);

    redis_ctx = redisConnectWithTimeout(ip_str, port, timeout);

    if (!redis_ctx || redis_ctx->err) {
        if (redis_ctx) {
            ogs_error("Redis connection error: %s", redis_ctx->errstr);
            redisFree(redis_ctx);
        } else {
            ogs_error("Redis connection error: can't allocate redis context");
        }
        /* Schedule reconnect */

        return OGS_ERROR;
    }

    self.redis = redis_ctx;
    self.is_connected = true;
    ogs_info("Redis connected to %s:%d", ip_str, port);

    return OGS_OK;
}

void upf_redis_disconnect(void)
{
    if (self.redis) {
        redisFree(self.redis);
        self.redis = NULL;
    }
    self.is_connected = false;
}

void upf_redis_reconnect(void)
{
    upf_redis_disconnect();
    if (upf_redis_connect() == OGS_OK) {
        ogs_info("Redis reconnected successfully");
    }
}

static void upf_redis_reconnect_timer_expire(void *data)
{
    if (!self.is_connected) {
        upf_redis_reconnect();
    }
}
