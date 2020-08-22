/*
 * Copyright (C) 2019 by Sukchan Lee <acetcom@gmail.com>
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

#include "core-config-private.h"

#if HAVE_CTYPE_H
#include <ctype.h>
#endif

#if HAVE_STDARG_H
#include <stdarg.h>
#endif

#include "ogs-core.h"

#define TA_NOR              "\033[0m"       /* all off */

#define TA_FGC_BLACK        "\033[30m"      /* Black */
#define TA_FGC_RED          "\033[31m"      /* Red */
#define TA_FGC_BOLD_RED     "\033[1;31m"    /* Bold Red */
#define TA_FGC_GREEN        "\033[32m"      /* Green */
#define TA_FGC_BOLD_GREEN   "\033[1;32m"    /* Bold Green */
#define TA_FGC_YELLOW       "\033[33m"      /* Yellow */
#define TA_FGC_BOLD_YELLOW  "\033[1;33m"    /* Bold Yellow */
#define TA_FGC_BOLD_BLUE    "\033[1;34m"    /* Bold Blue */
#define TA_FGC_BOLD_MAGENTA "\033[1;35m"    /* Bold Magenta */
#define TA_FGC_BOLD_CYAN    "\033[1;36m"    /* Bold Cyan */
#define TA_FGC_WHITE        "\033[37m"      /* White  */
#define TA_FGC_BOLD_WHITE   "\033[1;37m"    /* Bold White  */
#define TA_FGC_DEFAULT      "\033[39m"      /* default */

typedef enum {
    OGS_LOG_STDERR_TYPE,
    OGS_LOG_FILE_TYPE,
} ogs_log_type_e;

typedef struct ogs_log_s {
    ogs_lnode_t node;

    ogs_log_type_e type;

    union {
        struct {
            FILE *out;
            const char *name;
        } file;
    };

    struct {
    ED7(uint8_t color:1;,
        uint8_t timestamp:1;,
        uint8_t domain:1;,
        uint8_t level:1;,
        uint8_t fileline:1;,
        uint8_t function:1;,
        uint8_t linefeed:1;)
    } print;

    void (*writer)(ogs_log_t *log, ogs_log_level_e level, const char *string);

} ogs_log_t;

typedef struct ogs_log_domain_s {
    ogs_lnode_t node;

    int id;
    ogs_log_level_e level;
    const char *name;
} ogs_log_domain_t;

const char *level_strings[] = {
    NULL,
    "FATAL", "ERROR", "WARNING", "INFO", "DEBUG", "TRACE",
};

static OGS_POOL(log_pool, ogs_log_t);
static OGS_LIST(log_list);

static OGS_POOL(domain_pool, ogs_log_domain_t);
static OGS_LIST(domain_list);

static ogs_log_t *add_log(ogs_log_type_e type);
static int file_cycle(ogs_log_t *log);

static char *log_timestamp(char *buf, char *last,
        int use_color);
static char *log_domain(char *buf, char *last,
        const char *name, int use_color);
static char *log_content(char *buf, char *last,
        const char *format, va_list ap);
static char *log_level(char *buf, char *last,
        ogs_log_level_e level, int use_color);
static char *log_linefeed(char *buf, char *last);

static void file_writer(
        ogs_log_t *log, ogs_log_level_e level, const char *string);

void ogs_log_init(void)
{
    ogs_pool_init(&log_pool, ogs_core()->log.pool);
    ogs_pool_init(&domain_pool, ogs_core()->log.domain_pool);

    ogs_log_add_domain("core", ogs_core()->log.level);
    ogs_log_add_stderr();
}

void ogs_log_final(void)
{
    ogs_log_t *log, *saved_log;
    ogs_log_domain_t *domain, *saved_domain;

    ogs_list_for_each_safe(&log_list, saved_log, log)
        ogs_log_remove(log);
    ogs_pool_final(&log_pool);

    ogs_list_for_each_safe(&domain_list, saved_domain, domain)
        ogs_log_remove_domain(domain);
    ogs_pool_final(&domain_pool);
}

void ogs_log_cycle(void)
{
    ogs_log_t *log = NULL;

    ogs_list_for_each(&log_list, log) {
        switch(log->type) {
        case OGS_LOG_FILE_TYPE:
            file_cycle(log);
        default:
            break;
        }
    }
}

ogs_log_t *ogs_log_add_stderr(void)
{
    ogs_log_t *log = NULL;
    
    log = add_log(OGS_LOG_STDERR_TYPE);
    ogs_assert(log);

    log->file.out = stderr;
    log->writer = file_writer;

#if !defined(_WIN32)
    log->print.color = 1;
#endif

    return log;
}

ogs_log_t *ogs_log_add_file(const char *name)
{
    FILE *out = NULL;
    ogs_log_t *log = NULL;

    out = fopen(name, "a");
    if (!out) 
        return NULL;
    
    log = add_log(OGS_LOG_FILE_TYPE);
    ogs_assert(log);

    log->file.name = name;
    log->file.out = out;

    log->writer = file_writer;

    return log;
}

void ogs_log_remove(ogs_log_t *log)
{
    ogs_assert(log);

    ogs_list_remove(&log_list, log);

    if (log->type == OGS_LOG_FILE_TYPE) {
        ogs_assert(log->file.out);
        fclose(log->file.out);
        log->file.out = NULL;
    }

    ogs_pool_free(&log_pool, log);
}

ogs_log_domain_t *ogs_log_add_domain(const char *name, ogs_log_level_e level)
{
    ogs_log_domain_t *domain = NULL;

    ogs_assert(name);

    ogs_pool_alloc(&domain_pool, &domain);
    ogs_assert(domain);

    domain->name = name;
    domain->id = ogs_pool_index(&domain_pool, domain);
    domain->level = level;

    ogs_list_add(&domain_list, domain);

    return domain;
}

ogs_log_domain_t *ogs_log_find_domain(const char *name)
{
    ogs_log_domain_t *domain = NULL;

    ogs_assert(name);

    ogs_list_for_each(&domain_list, domain)
        if (!ogs_strcasecmp(domain->name, name))
            return domain;

    return NULL;
}

void ogs_log_remove_domain(ogs_log_domain_t *domain)
{
    ogs_assert(domain);

    ogs_list_remove(&domain_list, domain);
    ogs_pool_free(&domain_pool, domain);
}

void ogs_log_set_domain_level(int id, ogs_log_level_e level)
{
    ogs_log_domain_t *domain = NULL;

    ogs_assert(id > 0 && id <= ogs_core()->log.domain_pool);

    domain = ogs_pool_find(&domain_pool, id);
    ogs_assert(domain);

    domain->level = level;
}

ogs_log_level_e ogs_log_get_domain_level(int id)
{
    ogs_log_domain_t *domain = NULL;

    ogs_assert(id > 0 && id <= ogs_core()->log.domain_pool);

    domain = ogs_pool_find(&domain_pool, id);
    ogs_assert(domain);

    return domain->level;
}

const char *ogs_log_get_domain_name(int id)
{
    ogs_log_domain_t *domain = NULL;

    ogs_assert(id > 0 && id <= ogs_core()->log.domain_pool);

    domain = ogs_pool_find(&domain_pool, id);
    ogs_assert(domain);

    return domain->name;
}

int ogs_log_get_domain_id(const char *name)
{
    ogs_log_domain_t *domain = NULL;

    ogs_assert(name);
    
    domain = ogs_log_find_domain(name);
    ogs_assert(domain);

    return domain->id;
}

void ogs_log_install_domain(int *domain_id,
        const char *name, ogs_log_level_e level)
{
    ogs_log_domain_t *domain = NULL;

    ogs_assert(domain_id);
    ogs_assert(name);

    domain = ogs_log_find_domain(name);
    if (domain) {
        ogs_warn("`%s` log-domain duplicated", name);
        if (level != domain->level) {
            ogs_warn("[%s]->[%s] log-level changed",
                    level_strings[domain->level], level_strings[level]);
            ogs_log_set_domain_level(domain->id, level);
        }
    } else {
        domain = ogs_log_add_domain(name, level);
        ogs_assert(domain);
    }

    *domain_id = domain->id;
}

void ogs_log_set_mask_level(const char *_mask, ogs_log_level_e level)
{
    ogs_log_domain_t *domain = NULL;

    if (_mask) {
        const char *delim = " \t\n,:";
        char *mask = NULL;
        char *saveptr;
        char *name;

        mask = ogs_strdup(_mask);
        ogs_assert(mask);

        for (name = ogs_strtok_r(mask, delim, &saveptr);
            name != NULL;
            name = ogs_strtok_r(NULL, delim, &saveptr)) {

            domain = ogs_log_find_domain(name);
            if (domain)
                domain->level = level;
        }

        ogs_free(mask);
    } else {
        ogs_list_for_each(&domain_list, domain)
            domain->level = level;
    }
}

static ogs_log_level_e ogs_log_level_from_string(const char *string)
{
    ogs_log_level_e level = OGS_ERROR;

    if (!strcasecmp(string, "none")) level = OGS_LOG_NONE;
    else if (!strcasecmp(string, "fatal")) level = OGS_LOG_FATAL;
    else if (!strcasecmp(string, "error")) level = OGS_LOG_ERROR;
    else if (!strcasecmp(string, "warn")) level = OGS_LOG_WARN;
    else if (!strcasecmp(string, "info")) level = OGS_LOG_INFO;
    else if (!strcasecmp(string, "debug")) level = OGS_LOG_DEBUG;
    else if (!strcasecmp(string, "trace")) level = OGS_LOG_TRACE;

    return level;
}

int ogs_log_config_domain(const char *domain, const char *level)
{
    if (domain || level) {
        int l = ogs_core()->log.level;

        if (level) {
            l = ogs_log_level_from_string(level);
            if (l == OGS_ERROR) {
                ogs_error("Invalid LOG-LEVEL "
                        "[none:fatal|error|warn|info|debug|trace]: %s\n",
                        level);
                return OGS_ERROR;
            }
        }

        ogs_log_set_mask_level(domain, l);
    }

    return OGS_OK;
}

void ogs_log_vprintf(ogs_log_level_e level, int id,
    ogs_err_t err, const char *file, int line, const char *func,
    int content_only, const char *format, va_list ap)
{
    ogs_log_t *log = NULL;
    ogs_log_domain_t *domain = NULL;

    char logstr[OGS_HUGE_LEN];
    char *p, *last;

    int wrote_stderr = 0;

    ogs_list_for_each(&log_list, log) {
        domain = ogs_pool_find(&domain_pool, id);
        if (!domain) {
            fprintf(stderr, "No LogDomain[id:%d] in %s:%d", id, file, line);
            ogs_assert_if_reached();
        }
        if (domain->level < level)
            return;

        p = logstr;
        last = logstr + OGS_HUGE_LEN;

        if (!content_only) {
            if (log->print.timestamp)
                p = log_timestamp(p, last, log->print.color);
            if (log->print.domain)
                p = log_domain(p, last, domain->name, log->print.color);
            if (log->print.level)
                p = log_level(p, last, level, log->print.color);
        }

        p = log_content(p, last, format, ap);

        if (err) {
            char errbuf[OGS_HUGE_LEN];
            p = ogs_slprintf(p, last, " (%d:%s)",
                    (int)err, ogs_strerror(err, errbuf, OGS_HUGE_LEN));
        }

        if (!content_only) {
            if (log->print.fileline)
                p = ogs_slprintf(p, last, " (%s:%d)", file, line);
            if (log->print.function)
                p = ogs_slprintf(p, last, " %s()", func);
            if (log->print.linefeed) 
                p = log_linefeed(p, last);
        }

        log->writer(log, level, logstr);
        
        if (log->type == OGS_LOG_STDERR_TYPE)
            wrote_stderr = 1;
    }

    if (!wrote_stderr)
    {
        int use_color = 0;
#if !defined(_WIN32)
        use_color = 1;
#endif

        p = logstr;
        last = logstr + OGS_HUGE_LEN;

        if (!content_only) {
            p = log_timestamp(p, last, use_color);
            p = log_level(p, last, level, use_color);
        }
        p = log_content(p, last, format, ap);
        if (!content_only) {
            p = ogs_slprintf(p, last, " (%s:%d)", file, line);
            p = ogs_slprintf(p, last, " %s()", func);
            p = log_linefeed(p, last);
        }

        fprintf(stderr, "%s", logstr);
        fflush(stderr);
    }
}

void ogs_log_printf(ogs_log_level_e level, int id,
    ogs_err_t err, const char *file, int line, const char *func,
    int content_only, const char *format, ...)
{
    va_list args;

    va_start(args, format);
    ogs_log_vprintf(level, id,
            err, file, line, func, content_only, format, args);
    va_end(args);
}

void ogs_log_hexdump_func(ogs_log_level_e level, int id,
        const unsigned char *data, size_t len)
{
    size_t n, m;
    char dumpstr[OGS_HUGE_LEN];
    char *p, *last;

    last = dumpstr + OGS_HUGE_LEN;
    p = dumpstr;

    for (n = 0; n < len; n += 16) {
        p = ogs_slprintf(p, last, "%04x: ", (int)n);
        
        for (m = n; m < n + 16; m++) {
            if (m > n && (m % 4) == 0)
                p = ogs_slprintf(p, last, " ");
            if (m < len)
                p = ogs_slprintf(p, last, "%02x", data[m]);
            else
                p = ogs_slprintf(p, last, "  ");
        }

        p = ogs_slprintf(p, last, "   ");

        for (m = n; m < len && m < n + 16; m++)
            p = ogs_slprintf(p, last, "%c", isprint(data[m]) ? data[m] : '.');

        p = ogs_slprintf(p, last, "\n");
    }

    ogs_log_print(level, "%s", dumpstr);
}

static ogs_log_t *add_log(ogs_log_type_e type)
{
    ogs_log_t *log = NULL;

    ogs_pool_alloc(&log_pool, &log);
    ogs_assert(log);
    memset(log, 0, sizeof *log);

    log->type = type;

    log->print.timestamp = 1;
    log->print.domain = 1;
    log->print.level = 1;
    log->print.fileline = 1;
    log->print.linefeed = 1;

    ogs_list_add(&log_list, log);

    return log;
}

static int file_cycle(ogs_log_t *log)
{
    ogs_assert(log);
    ogs_assert(log->file.out);
    ogs_assert(log->file.name);

    fclose(log->file.out);
    log->file.out = fopen(log->file.name, "a");
    ogs_assert(log->file.out);

    return 0;
}

static char *log_timestamp(char *buf, char *last,
        int use_color)
{
    struct timeval tv;
    struct tm tm;
    char nowstr[32];

    ogs_gettimeofday(&tv);
    ogs_localtime(tv.tv_sec, &tm);
    strftime(nowstr, sizeof nowstr, "%m/%d %H:%M:%S", &tm);

    buf = ogs_slprintf(buf, last, "%s%s.%03d%s: ",
            use_color ? TA_FGC_GREEN : "",
            nowstr, (int)(tv.tv_usec/1000),
            use_color ? TA_NOR : "");

    return buf;
}

static char *log_domain(char *buf, char *last,
        const char *name, int use_color)
{
    buf = ogs_slprintf(buf, last, "[%s%s%s] ",
            use_color ? TA_FGC_YELLOW : "",
            name,
            use_color ? TA_NOR : "");

    return buf;
}

static char *log_level(char *buf, char *last,
        ogs_log_level_e level, int use_color)
{
    const char *colors[] = {
        TA_NOR,
        TA_FGC_BOLD_RED, TA_FGC_BOLD_YELLOW, TA_FGC_BOLD_CYAN,
        TA_FGC_BOLD_GREEN, TA_FGC_BOLD_WHITE, TA_FGC_WHITE,
    };

    buf = ogs_slprintf(buf, last, "%s%s%s: ",
            use_color ? colors[level] : "",
            level_strings[level],
            use_color ? TA_NOR : "");

    return buf;
}

static char *log_content(char *buf, char *last,
        const char *format, va_list ap)
{
    va_list bp;

    va_copy(bp, ap);
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-nonliteral"
    buf = ogs_vslprintf(buf, last, format, bp);
#pragma GCC diagnostic pop
    va_end(bp);

    return buf;
}

static char *log_linefeed(char *buf, char *last)
{
#if defined(_WIN32)
    if (buf > last - 3)
        buf = last - 3;

    buf = ogs_slprintf(buf, last, "\r\n");
#else
    if (buf > last - 2)
        buf = last - 2;

    buf = ogs_slprintf(buf, last, "\n");
#endif

    return buf;
}

static void file_writer(
        ogs_log_t *log, ogs_log_level_e level, const char *string)
{
    fprintf(log->file.out, "%s", string);
    fflush(log->file.out);
}

