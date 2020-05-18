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

#ifndef OGS_STRINGS_H
#define OGS_STRINGS_H

#ifdef __cplusplus
extern "C" {
#endif

#define OGS_HUGE_LEN        8192

#if defined(_WIN32)
#define ogs_strtok_r strtok_s
#define ogs_strcasecmp _stricmp
#define ogs_strncasecmp _strnicmp
#else
#define ogs_strtok_r strtok_r
#define ogs_strcasecmp strcasecmp
#define ogs_strncasecmp strncasecmp
#endif

int ogs_vsnprintf(char *str, size_t size, const char *format, va_list ap)
    OGS_GNUC_PRINTF (3, 0);
int ogs_snprintf(char *str, size_t size, const char *format, ...)
    OGS_GNUC_PRINTF(3, 4);
char *ogs_vslprintf(char *str, char *last, const char *format, va_list ap)
    OGS_GNUC_PRINTF (3, 0);
char *ogs_slprintf(char *str, char *last, const char *format, ...)
    OGS_GNUC_PRINTF(3, 4);

char *ogs_strdup(const char *s);
char *ogs_strndup(const char *s, size_t n);
void *ogs_memdup(const void *m, size_t n);

char *ogs_cpystrn(char *dst, const char *src, size_t dst_size);

char *ogs_msprintf(const char *message, ...)
    OGS_GNUC_PRINTF(1, 2);
char *ogs_mstrcatf(char *source, const char *message, ...)
    OGS_GNUC_PRINTF(2, 3);

#ifdef __cplusplus
}
#endif

#endif /* OGS_STRINGS_H */
