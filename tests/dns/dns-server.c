/*
 * Copyright (C) 2026 by Evgenii Grigorev <tothe8c@gmail.com>
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

#include "ogs-core.h"
#include "dns-server.h"

#include <pthread.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX_RECORDS     32
#define MAX_QUERIES     256
#define MAX_NAME        256
#define MAX_MSG         1024

static struct {
    int sock;
    pthread_t thread;
    bool running;

    pthread_mutex_t lock;
    int mode;
    int num_records;
    test_dns_record_t records[MAX_RECORDS];

    int num_queries;
    char queries[MAX_QUERIES][MAX_NAME];
} g;

/* Parse the question name at msg+12 into a dotted string.
 * Returns the offset just past the name, or -1. */
static int parse_qname(const uint8_t *msg, int len, char *name, size_t sz)
{
    int off = 12;
    size_t used = 0;

    while (off < len) {
        uint8_t l = msg[off];
        if (l == 0) {
            off++;
            break;
        }
        if (l >= 0xc0)  /* no compression expected in a question */
            return -1;
        if (off + 1 + l > len || used + l + 2 > sz)
            return -1;
        if (used) name[used++] = '.';
        memcpy(name + used, msg + off + 1, l);
        used += l;
        off += 1 + l;
    }
    name[used] = 0;
    return off;
}

static int put_name(uint8_t *buf, int off, const char *name)
{
    const char *p = name;

    /* Root name "." encodes as a single null label (RFC 1035 3.1);
     * needed to serve SRV records with target "." = service not
     * available (RFC 2782) */
    if (name[0] == '.' && name[1] == 0) {
        buf[off++] = 0;
        return off;
    }

    while (*p) {
        const char *dot = strchr(p, '.');
        size_t l = dot ? (size_t)(dot - p) : strlen(p);
        ogs_assert(l > 0 && l < 64);
        buf[off++] = l;
        memcpy(buf + off, p, l);
        off += l;
        p += l;
        if (*p == '.') p++;
    }
    buf[off++] = 0;
    return off;
}

static int put_char_string(uint8_t *buf, int off, const char *s)
{
    size_t l = s ? strlen(s) : 0;
    ogs_assert(l < 256);
    buf[off++] = l;
    if (l) memcpy(buf + off, s, l);
    return off + l;
}

static int put_u16(uint8_t *buf, int off, uint16_t v)
{
    buf[off++] = v >> 8;
    buf[off++] = v & 0xff;
    return off;
}

static int put_u32(uint8_t *buf, int off, uint32_t v)
{
    buf[off++] = v >> 24;
    buf[off++] = (v >> 16) & 0xff;
    buf[off++] = (v >> 8) & 0xff;
    buf[off++] = v & 0xff;
    return off;
}

/* Append one answer RR (NAME = pointer to the question at offset 12) */
static int put_answer(uint8_t *buf, int off, const test_dns_record_t *r)
{
    int rdlen_off, rdata_start;

    off = put_u16(buf, off, 0xc00c);        /* name pointer */
    off = put_u16(buf, off, r->qtype);
    off = put_u16(buf, off, 1);             /* IN */
    off = put_u32(buf, off, 60);            /* TTL */
    rdlen_off = off;
    off = put_u16(buf, off, 0);             /* rdlength placeholder */
    rdata_start = off;

    switch (r->qtype) {
    case TEST_DNS_T_A: {
        struct in_addr a;
        ogs_assert(inet_pton(AF_INET, r->ipv4, &a) == 1);
        memcpy(buf + off, &a, 4);
        off += 4;
        break;
    }
    case TEST_DNS_T_SRV:
        off = put_u16(buf, off, r->priority);
        off = put_u16(buf, off, r->weight);
        off = put_u16(buf, off, r->port);
        off = put_name(buf, off, r->target);
        break;
    case TEST_DNS_T_NAPTR:
        off = put_u16(buf, off, r->order);
        off = put_u16(buf, off, r->preference);
        off = put_char_string(buf, off, r->flags);
        off = put_char_string(buf, off, r->service);
        off = put_char_string(buf, off, "");    /* regexp */
        off = put_name(buf, off, r->replacement);
        break;
    default:
        ogs_assert_if_reached();
    }

    put_u16(buf, rdlen_off, off - rdata_start);
    return off;
}

static void *server_main(void *data)
{
    uint8_t msg[MAX_MSG], out[MAX_MSG];
    struct sockaddr_in from;
    socklen_t fromlen;
    struct timeval tv;

    tv.tv_sec = 0;
    tv.tv_usec = 100000;
    setsockopt(g.sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    while (g.running) {
        int len, qend, i, out_len, ancount = 0;
        char qname[MAX_NAME];
        uint16_t qtype;

        fromlen = sizeof(from);
        len = recvfrom(g.sock, msg, sizeof(msg), 0,
                (struct sockaddr *)&from, &fromlen);
        if (len < 12 + 5)
            continue;

        qend = parse_qname(msg, len, qname, sizeof(qname));
        if (qend < 0 || qend + 4 > len)
            continue;
        qtype = (msg[qend] << 8) | msg[qend+1];

        pthread_mutex_lock(&g.lock);

        if (g.num_queries < MAX_QUERIES) {
            ogs_cpystrn(g.queries[g.num_queries], qname, MAX_NAME);
            g.num_queries++;
        }

        if (g.mode == TEST_DNS_MODE_DROP) {
            pthread_mutex_unlock(&g.lock);
            continue;
        }

        /* header + question echo */
        memcpy(out, msg, qend + 4);
        out_len = qend + 4;
        out[2] = 0x84;  /* QR|AA */
        out[3] = 0x80;  /* RA, RCODE=0 */
        put_u16(out, 4, 1);     /* QDCOUNT */
        put_u16(out, 8, 0);     /* NSCOUNT */
        put_u16(out, 10, 0);    /* ARCOUNT */

        if (g.mode == TEST_DNS_MODE_ANSWER) {
            for (i = 0; i < g.num_records; i++) {
                const test_dns_record_t *r = &g.records[i];
                if (r->qtype != qtype)
                    continue;
                if (strcasecmp(r->qname, qname) != 0)
                    continue;
                out_len = put_answer(out, out_len, r);
                ancount++;
            }
        }
        put_u16(out, 6, ancount);   /* ANCOUNT */

        pthread_mutex_unlock(&g.lock);

        (void)!sendto(g.sock, out, out_len, 0,
                (struct sockaddr *)&from, fromlen);
    }

    return NULL;
}

int test_dns_server_start(const char *address, int port)
{
    struct sockaddr_in sin;

    memset(&g, 0, sizeof(g));
    pthread_mutex_init(&g.lock, NULL);

    g.sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (g.sock < 0)
        return OGS_ERROR;

    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);
    if (inet_pton(AF_INET, address, &sin.sin_addr) != 1) {
        close(g.sock);
        return OGS_ERROR;
    }
    if (bind(g.sock, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
        close(g.sock);
        return OGS_ERROR;
    }

    g.running = true;
    if (pthread_create(&g.thread, NULL, server_main, NULL) != 0) {
        close(g.sock);
        return OGS_ERROR;
    }

    return OGS_OK;
}

void test_dns_server_stop(void)
{
    if (!g.running)
        return;
    g.running = false;
    pthread_join(g.thread, NULL);
    close(g.sock);
    pthread_mutex_destroy(&g.lock);
}

void test_dns_server_set_records(const test_dns_record_t *records, int num)
{
    ogs_assert(num <= MAX_RECORDS);
    pthread_mutex_lock(&g.lock);
    memcpy(g.records, records, sizeof(*records) * num);
    g.num_records = num;
    g.mode = TEST_DNS_MODE_ANSWER;
    pthread_mutex_unlock(&g.lock);
}

void test_dns_server_set_mode(int mode)
{
    pthread_mutex_lock(&g.lock);
    g.mode = mode;
    pthread_mutex_unlock(&g.lock);
}

int test_dns_server_num_queries(const char *qname_substr)
{
    int i, count = 0;

    pthread_mutex_lock(&g.lock);
    for (i = 0; i < g.num_queries; i++) {
        if (!qname_substr || strstr(g.queries[i], qname_substr))
            count++;
    }
    pthread_mutex_unlock(&g.lock);

    return count;
}

void test_dns_server_reset_counters(void)
{
    pthread_mutex_lock(&g.lock);
    g.num_queries = 0;
    pthread_mutex_unlock(&g.lock);
}
