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
            context_self()->db_name)
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
