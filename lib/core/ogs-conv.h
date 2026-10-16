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

#ifndef OGS_CONV_H
#define OGS_CONV_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Skip whitespace and convert up to out_len bytes without validating hex.
 * Return the number of complete bytes. Short/odd input may leave partial
 * output; excess input is ignored. out_len is capacity, not an exact size.
 * Use ogs_ascii_to_hex_checked() for received, fixed-size hex fields.
 */
int ogs_ascii_to_hex(char *in, int in_len, void *out, int out_len);
/*
 * Require exactly out_len * 2 hex digits (0-9, a-f, A-F), without whitespace.
 * Return OGS_OK, or log an error and return OGS_ERROR without changing out.
 * Check the result before using the output; out_len is the exact decoded size.
 *
 * Added for AUSF vector validation after issue #4734
 * (opened 2026-08-08): https://github.com/open5gs/open5gs/issues/4734
 */
int ogs_ascii_to_hex_checked(
        const char *in, int in_len, void *out, int out_len);
/*
 * Permissive string wrapper; returns out without reporting invalid input.
 * Use ogs_hex_from_string_checked() for received, fixed-size hex fields.
 */
static ogs_inline void *ogs_hex_from_string(
        const char *str, void *out, int out_len)
{
    ogs_ascii_to_hex((char*)str, strlen(str), out, out_len);
    return out;
}
/*
 * String wrapper for ogs_ascii_to_hex_checked(); check the return value.
 */
static ogs_inline int ogs_hex_from_string_checked(
        const char *str, void *out, int out_len)
{
    return ogs_ascii_to_hex_checked(
            str, str ? strlen(str) : 0, out, out_len);
}

void *ogs_hex_to_ascii(const void *in, int in_len, void *out, int out_len);
void *ogs_uint64_to_buffer(uint64_t num, int size, void *buffer);
uint64_t ogs_buffer_to_uint64(void *buffer, int size);
void *ogs_bcd_to_buffer(const char *in, void *out, int *out_len);
void *ogs_bcd_to_buffer_reverse_order(const char *in, void *out, int *out_len);
void *ogs_buffer_to_bcd(const uint8_t *in, int in_len, void *out,
        int out_len);

char ogs_from_hex(char ch);
char ogs_to_hex(char ch);

char *ogs_uint24_to_0string(ogs_uint24_t x);
char *ogs_uint28_to_0string(uint32_t x);
char *ogs_uint32_to_0string(uint32_t x);
char *ogs_uint36_to_0string(uint64_t x);
char *ogs_uint64_to_0string(uint64_t x);
char *ogs_uint64_to_string(uint64_t x);

#define ogs_uint24_from_string_decimal(str) \
    ogs_uint24_from_string((str), 10)
#define ogs_uint24_from_string_hexadecimal(str) \
    ogs_uint24_from_string((str), 16)
ogs_uint24_t ogs_uint24_from_string(char *str, int base);

#define ogs_uint64_from_string_decimal(str) \
    ogs_uint64_from_string((str), 10)
#define ogs_uint64_from_string_hexadecimal(str) \
    ogs_uint64_from_string((str), 16)
uint64_t ogs_uint64_from_string(char *str, int base);

double *ogs_alloc_double(double value);

#ifdef __cplusplus
}
#endif

#endif /* OGS_CONV_H */
