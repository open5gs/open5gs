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

#if !defined(OGS_CORE_INSIDE) && !defined(OGS_CORE_COMPILATION)
#error "This header cannot be included directly."
#endif

#ifndef OGS_LOG_H
#define OGS_LOG_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN      1
#endif

#define ogs_fatal(...) ogs_log_message(OGS_LOG_FATAL, 0, __VA_ARGS__)
#define ogs_error(...) ogs_log_message(OGS_LOG_ERROR, 0, __VA_ARGS__)
#define ogs_warn(...) ogs_log_message(OGS_LOG_WARN, 0, __VA_ARGS__)
#define ogs_info(...) ogs_log_message(OGS_LOG_INFO, 0, __VA_ARGS__)
#define ogs_debug(...) ogs_log_message(OGS_LOG_DEBUG, 0, __VA_ARGS__)
#define ogs_trace(...) ogs_log_message(OGS_LOG_TRACE, 0, __VA_ARGS__)

#define ogs_log_message(level, err, ...) \
    ogs_log_printf(level, OGS_LOG_DOMAIN, \
    err, __FILE__, __LINE__, OGS_FUNC,  \
    0, __VA_ARGS__) 

#define ogs_log_print(level, ...) \
    ogs_log_printf(level, OGS_LOG_DOMAIN, \
    0, NULL, 0, NULL,  \
    1, __VA_ARGS__) 

#define ogs_log_hexdump(level, _d, _l) \
    ogs_log_hexdump_func(level, OGS_LOG_DOMAIN, _d, _l)

typedef enum {
    OGS_LOG_NONE,
    OGS_LOG_FATAL,
    OGS_LOG_ERROR,
    OGS_LOG_WARN,
    OGS_LOG_INFO,
    OGS_LOG_DEBUG,
    OGS_LOG_TRACE,
    OGS_LOG_DEFAULT = OGS_LOG_INFO,
    OGS_LOG_FULL = OGS_LOG_TRACE,
} ogs_log_level_e;

typedef struct ogs_log_s ogs_log_t;
typedef struct ogs_log_domain_s ogs_log_domain_t;

void ogs_log_init(void);
void ogs_log_final(void);
void ogs_log_cycle(void);

ogs_log_t *ogs_log_add_stderr(void);
ogs_log_t *ogs_log_add_file(const char *name);
void ogs_log_remove(ogs_log_t *log);

ogs_log_domain_t *ogs_log_add_domain(const char *name, ogs_log_level_e level);
ogs_log_domain_t *ogs_log_find_domain(const char *name);
void ogs_log_remove_domain(ogs_log_domain_t *domain);

void ogs_log_set_domain_level(int id, ogs_log_level_e level);
ogs_log_level_e ogs_log_get_domain_level(int id);

const char *ogs_log_get_domain_name(int id);
int ogs_log_get_domain_id(const char *name);

void ogs_log_install_domain(int *domain_id,
        const char *name, ogs_log_level_e level);
int ogs_log_config_domain(const char *domain, const char *level);

void ogs_log_set_mask_level(const char *mask, ogs_log_level_e level);

void ogs_log_vprintf(ogs_log_level_e level, int id,
    ogs_err_t err, const char *file, int line, const char *func,
    int content_only, const char *format, va_list ap);
void ogs_log_printf(ogs_log_level_e level, int domain_id,
    ogs_err_t err, const char *file, int line, const char *func,
    int content_only, const char *format, ...)
    OGS_GNUC_PRINTF(8, 9);

void ogs_log_hexdump_func(ogs_log_level_e level, int domain_id,
    const unsigned char *data, size_t len);

#define ogs_assert(expr) \
    do { \
        if (ogs_likely(expr)) ; \
        else { \
            ogs_fatal("%s: Assertion `%s' failed.", OGS_FUNC, #expr); \
            ogs_abort(); \
        } \
    } while(0)

#define ogs_assert_if_reached() \
    do { \
        ogs_warn("%s: should not be reached.", OGS_FUNC); \
        ogs_abort(); \
    } while(0)

#define ogs_expect(expr) \
    do { \
        if (ogs_likely(expr)) ; \
        else { \
            ogs_error("%s: Expectation `%s' failed.", OGS_FUNC, #expr); \
        } \
    } while (0)

#define ogs_expect_or_return(expr) \
    do { \
        if (ogs_likely(expr)) ; \
        else { \
            ogs_error("%s: Expectation `%s' failed.", OGS_FUNC, #expr); \
            return; \
        } \
    } while (0)

#define ogs_expect_or_return_val(expr, val) \
    do { \
        if (ogs_likely(expr)) ; \
        else { \
            ogs_error("%s: Expectation `%s' failed.", OGS_FUNC, #expr); \
            return (val); \
        } \
    } while (0)

#ifdef __cplusplus
}
#endif

#endif /* OGS_LOG_H */
