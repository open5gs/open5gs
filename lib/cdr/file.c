/*
 * Copyright (C) 2026 by DNL
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

/*
 * CDR file writer per 3GPP TS 32.297 §6.1 (checked against v19.0.0).
 *
 * File header (52 octets):
 *   1..4   file length            5..8   header length
 *   9      high release/version   10     low release/version
 *   11..14 file opening timestamp 15..18 last CDR append timestamp
 *   19..22 number of CDRs         23..26 file sequence number
 *   27     file closure reason    28..47 node IP (4 x 0xFF + IPv6-mapped)
 *   48     lost CDR indicator     49..50 routeing filter length (0)
 *   51     high release id ext    52     low release id ext
 * (private extension length omitted: present only if extension included)
 *
 * CDR header (5 octets): length(2), relid 7|version 2, BER(1)|TS 32.251(7),
 * release id extension 8 (Rel-18, matching the ASN.1 we encode against).
 */

#include "ogs-cdr.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#define FILE_HDR_LEN            52
#define REL_VER_OCTET           0xe2    /* relid 7 << 5 | version 2 */
#define REL_ID_EXTENSION        8       /* Rel-18 */
#define FORMAT_TS_OCTET         0x27    /* BER 1 << 5 | TS 32.251 = 7 */
#define CDR_HDR_LEN             5

struct ogs_cdr_file_s {
    FILE *fp;
    char tmp_path[512];
    char final_path[512];
    unsigned int num_cdrs;
    size_t total_len;
    uint8_t header[FILE_HDR_LEN];
};

static void put32(uint8_t *p, uint32_t v)
{
    p[0] = (uint8_t)(v >> 24);
    p[1] = (uint8_t)(v >> 16);
    p[2] = (uint8_t)(v >> 8);
    p[3] = (uint8_t)v;
}

/*
 * TS 32.297 §6.1.1.5 timestamp:
 * month(4) day(5) hour(5) minute(6) sign(1) tz-hour(5) tz-minute(6)
 */
static uint32_t header_timestamp(time_t t, int tz_offset_min)
{
    time_t local = t + (time_t)tz_offset_min * 60;
    struct tm tm;
    uint32_t v = 0;
    int off = tz_offset_min < 0 ? -tz_offset_min : tz_offset_min;

    gmtime_r(&local, &tm);

    v |= ((uint32_t)(tm.tm_mon + 1) & 0x0f) << 28;
    v |= ((uint32_t)tm.tm_mday & 0x1f) << 23;
    v |= ((uint32_t)tm.tm_hour & 0x1f) << 18;
    v |= ((uint32_t)tm.tm_min & 0x3f) << 12;
    v |= ((uint32_t)(tz_offset_min >= 0 ? 1 : 0)) << 11;
    v |= ((uint32_t)(off / 60) & 0x1f) << 6;
    v |= (uint32_t)(off % 60) & 0x3f;

    return v;
}

ogs_cdr_file_t *ogs_cdr_file_open(const char *final_path,
        uint32_t file_sequence_number, const uint8_t node_ipv4[4],
        time_t now, int tz_offset_min)
{
    ogs_cdr_file_t *file;
    uint8_t *h;

    if (!final_path)
        return NULL;

    file = calloc(1, sizeof(*file));
    if (!file)
        return NULL;

    if (snprintf(file->final_path, sizeof(file->final_path), "%s",
            final_path) >= (int)sizeof(file->final_path) ||
        snprintf(file->tmp_path, sizeof(file->tmp_path), "%s.tmp",
            final_path) >= (int)sizeof(file->tmp_path)) {
        free(file);
        return NULL;
    }

    file->fp = fopen(file->tmp_path, "wb");
    if (!file->fp) {
        free(file);
        return NULL;
    }

    h = file->header;
    put32(h + 4, FILE_HDR_LEN);                 /* header length */
    h[8] = REL_VER_OCTET;                       /* high release/version */
    h[9] = REL_VER_OCTET;                       /* low release/version */
    put32(h + 10, header_timestamp(now, tz_offset_min));
    /* 14..17 last append: 0 until first CDR */
    put32(h + 22, file_sequence_number);
    /* 26 closure reason: set on close */
    memset(h + 27, 0xff, 4);                    /* node IP: 4 insignificant */
    memset(h + 31, 0, 10);                      /* IPv6-mapped IPv4 */
    h[41] = 0xff;
    h[42] = 0xff;
    memcpy(h + 43, node_ipv4, 4);
    /* 47 lost CDR indicator = 0, 48..49 routeing filter length = 0 */
    h[50] = REL_ID_EXTENSION;                   /* high release id ext */
    h[51] = REL_ID_EXTENSION;                   /* low release id ext */

    if (fwrite(file->header, 1, FILE_HDR_LEN, file->fp) != FILE_HDR_LEN) {
        fclose(file->fp);
        unlink(file->tmp_path);
        free(file);
        return NULL;
    }
    file->total_len = FILE_HDR_LEN;

    return file;
}

int ogs_cdr_file_append(ogs_cdr_file_t *file,
        const uint8_t *cdr, size_t cdr_len, time_t now)
{
    uint8_t hdr[CDR_HDR_LEN];

    if (!file || !file->fp || !cdr || !cdr_len || cdr_len > 0xfffe)
        return -1;

    hdr[0] = (uint8_t)(cdr_len >> 8);
    hdr[1] = (uint8_t)cdr_len;
    hdr[2] = REL_VER_OCTET;
    hdr[3] = FORMAT_TS_OCTET;
    hdr[4] = REL_ID_EXTENSION;

    if (fwrite(hdr, 1, sizeof(hdr), file->fp) != sizeof(hdr) ||
        fwrite(cdr, 1, cdr_len, file->fp) != cdr_len)
        return -1;

    file->num_cdrs++;
    file->total_len += sizeof(hdr) + cdr_len;
    /* last CDR append timestamp is in UTC (TS 32.297 §6.1.1.6) */
    put32(file->header + 14, header_timestamp(now, 0));

    return 0;
}

unsigned int ogs_cdr_file_count(const ogs_cdr_file_t *file)
{
    return file ? file->num_cdrs : 0;
}

size_t ogs_cdr_file_size(const ogs_cdr_file_t *file)
{
    return file ? file->total_len : 0;
}

int ogs_cdr_file_close(ogs_cdr_file_t *file, uint8_t closure_reason)
{
    int rv = 0;

    if (!file)
        return -1;

    if (!file->fp) {
        free(file);
        return -1;
    }

    if (file->num_cdrs == 0) {
        /* nothing recorded: drop the file entirely */
        fclose(file->fp);
        unlink(file->tmp_path);
        free(file);
        return 0;
    }

    put32(file->header, (uint32_t)file->total_len);
    put32(file->header + 18, file->num_cdrs);
    file->header[26] = closure_reason;

    if (fflush(file->fp) != 0 ||
        fseek(file->fp, 0, SEEK_SET) != 0 ||
        fwrite(file->header, 1, FILE_HDR_LEN, file->fp) != FILE_HDR_LEN ||
        fflush(file->fp) != 0 ||
        fsync(fileno(file->fp)) != 0)
        rv = -1;

    if (fclose(file->fp) != 0)
        rv = -1;

    if (rv == 0)
        rv = rename(file->tmp_path, file->final_path);
    if (rv != 0)
        unlink(file->tmp_path);

    free(file);
    return rv;
}
