#include <hiredis/hiredis.h>
#include <json-c/json.h>
#include "mme-redis.h"
#include "mme-timer.h"
#include "ogs-core.h"

static mme_redis_context_t self;
static ogs_timer_mgr_t *timer_mgr = NULL;


static void mme_redis_reconnect_timer_expire(void *data);

int mme_redis_init(void)
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
    /* Initialize Redis context */
    memset(&self, 0, sizeof(mme_redis_context_t));

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
        mme_redis_reconnect_timer_expire, NULL);
    if (!self.t_reconnect) {
        ogs_error("Cannot create Redis reconnect timer");
        ogs_free(self.addr);
        return OGS_ERROR;
    }


    return mme_redis_connect();
}

void mme_redis_final(void)
{
    mme_redis_disconnect();

    if (self.t_reconnect)
        ogs_timer_delete(self.t_reconnect);

    if (self.addr)
        ogs_freeaddrinfo(self.addr);

    if (timer_mgr)
        ogs_timer_mgr_destroy(timer_mgr);
}

int mme_redis_connect(void)
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

void mme_redis_disconnect(void)
{
    if (self.redis) {
        redisFree(self.redis);
        self.redis = NULL;
    }
    self.is_connected = false;
}

void mme_redis_reconnect(void)
{
    mme_redis_disconnect();
    if (mme_redis_connect() == OGS_OK) {
        ogs_info("Redis reconnected successfully");
    }
}

static void mme_redis_reconnect_timer_expire(void *data)
{
    if (!self.is_connected) {
        mme_redis_reconnect();
    }
}

int mme_redis_update_ue(mme_ue_t *mme_ue)
{
    redisReply *reply;
    struct json_object *j_obj;
    char *json_str;
    char key[128];
    int ret = OGS_OK;

    ogs_assert(mme_ue);
    if (!self.is_connected || !self.redis) {
        ogs_error("Redis not connected");
        return OGS_ERROR;
    }

    /* Create JSON object with UE info */
    j_obj = json_object_new_object();
    json_object_object_add(j_obj, "tac", 
        json_object_new_int(mme_ue->tai.tac));
    json_object_object_add(j_obj, "mme_id", 
        json_object_new_string(mme_self()->mme_name));
    json_object_object_add(j_obj, "mme_host", 
        json_object_new_string(ogs_gethostname(self.addr)));
    json_object_object_add(j_obj, "timestamp", 
        json_object_new_int64(ogs_time_now()));

    json_str = (char*)json_object_to_json_string(j_obj);
    snprintf(key, sizeof(key), "mme_%s", mme_ue->imsi_bcd);

    reply = redisCommand(self.redis, "SET %s %s", key, json_str);
    if (!reply) {
        ogs_error("Redis SET command failed");
        ret = OGS_ERROR;
        goto cleanup;
    }
    
    if (reply->type == REDIS_REPLY_ERROR) {
        ogs_error("Redis error: %s", reply->str);
        ret = OGS_ERROR;
    }

    freeReplyObject(reply);

cleanup:
    json_object_put(j_obj);
    return ret;
}

int mme_redis_remove_ue(mme_ue_t *mme_ue)
{
    redisReply *reply;
    char key[128];
    int ret = OGS_OK;

    ogs_assert(mme_ue);
    if (!self.is_connected || !self.redis) {
        ogs_error("Redis not connected");
        return OGS_ERROR;
    }

    snprintf(key, sizeof(key), "mme_%s", mme_ue->imsi_bcd);
    
    reply = redisCommand(self.redis, "DEL %s", key);
    if (!reply) {
        ogs_error("Redis DEL command failed");
        return OGS_ERROR;
    }

    if (reply->type == REDIS_REPLY_ERROR) {
        ogs_error("Redis error: %s", reply->str);
        ret = OGS_ERROR;
    }

    freeReplyObject(reply);
    return ret;
} 