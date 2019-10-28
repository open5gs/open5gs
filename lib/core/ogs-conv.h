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

#define OGS_HEX(I, I_LEN, O) ogs_ascii_to_hex((char*)I, I_LEN, O, sizeof(O))
void *ogs_ascii_to_hex(char *in, int in_len, void *out, int out_len);
void *ogs_hex_to_ascii(void *in, int in_len, void *out, int out_len);
void *ogs_uint64_to_buffer(uint64_t num, int size, void *buffer);
uint64_t ogs_buffer_to_uint64(void *buffer, int size);
void *ogs_bcd_to_buffer(const char *in, void *out, int *out_len);
void *ogs_buffer_to_bcd(uint8_t *in, int in_len, void *out);

#ifdef __cplusplus
}
#endif

#endif /* OGS_CONV_H */
