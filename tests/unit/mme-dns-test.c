/*
 * Copyright (C) 2024 by sysmocom - s.f.m.c. GmbH <info@sysmocom.de>
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
 * Unit tests for the TS 29.303 S-NAPTR PGW discovery helpers in the MME
 * (src/mme/mme-dns.c). These exercise the pure, resolver-independent logic:
 *   - mme_dns_service_matches(): Service-field filtering by S5/S8 interface
 *   - mme_dns_parse_naptr():     NAPTR rdata parsing, filtering and ordering
 *   - mme_dns_apn_fqdn():        TS 23.003 APN-FQDN construction
 *
 * The parser is fed hand-built raw DNS NAPTR response messages so the test
 * runs without any network or live resolver.
 */

#include "ogs-core.h"
#include "core/abts.h"

#include "mme/mme-context.h"
#include "mme/mme-dns.h"

#include <arpa/nameser.h>

/* ----------------------------------------------- minimal DNS message builder */

typedef struct dnsbuf_s {
    unsigned char   buf[2048];
    int             len;
} dnsbuf_t;

static void put16(dnsbuf_t *b, uint16_t v)
{
    b->buf[b->len++] = (v >> 8) & 0xff;
    b->buf[b->len++] = v & 0xff;
}

static void put32(dnsbuf_t *b, uint32_t v)
{
    b->buf[b->len++] = (v >> 24) & 0xff;
    b->buf[b->len++] = (v >> 16) & 0xff;
    b->buf[b->len++] = (v >> 8) & 0xff;
    b->buf[b->len++] = v & 0xff;
}

static void put_charstr(dnsbuf_t *b, const char *s)
{
    size_t n = strlen(s);
    b->buf[b->len++] = (unsigned char)n;
    memcpy(&b->buf[b->len], s, n);
    b->len += (int)n;
}

/* Encode a dotted name as a sequence of length-prefixed labels + root. */
static void put_name(dnsbuf_t *b, const char *name)
{
    const char *p = name;

    while (*p) {
        const char *dot = strchr(p, '.');
        size_t n = dot ? (size_t)(dot - p) : strlen(p);
        b->buf[b->len++] = (unsigned char)n;
        memcpy(&b->buf[b->len], p, n);
        b->len += (int)n;
        p += n;
        if (*p == '.')
            p++;
    }
    b->buf[b->len++] = 0;
}

static void dns_begin(dnsbuf_t *b, int ancount)
{
    memset(b, 0, sizeof(*b));
    put16(b, 0x1234);   /* transaction id */
    put16(b, 0x8180);   /* flags: response + recursion available */
    put16(b, 1);        /* qdcount */
    put16(b, ancount);  /* ancount */
    put16(b, 0);        /* nscount */
    put16(b, 0);        /* arcount */

    put_name(b, "ims.apn.epc.mnc001.mcc001.3gppnetwork.org");
    put16(b, ns_t_naptr);
    put16(b, ns_c_in);
}

static void dns_add_naptr(dnsbuf_t *b, uint16_t order, uint16_t pref,
        const char *flags, const char *services, const char *replacement,
        uint32_t ttl)
{
    int rdlen_pos, rdstart, rdlen;

    /* Owner name: compression pointer to the question name at offset 12. */
    b->buf[b->len++] = 0xc0;
    b->buf[b->len++] = 0x0c;
    put16(b, ns_t_naptr);
    put16(b, ns_c_in);
    put32(b, ttl);

    rdlen_pos = b->len;
    put16(b, 0);            /* RDLENGTH placeholder */
    rdstart = b->len;

    put16(b, order);
    put16(b, pref);
    put_charstr(b, flags);
    put_charstr(b, services);
    put_charstr(b, "");     /* regexp: empty (replacement form) */
    put_name(b, replacement);

    rdlen = b->len - rdstart;
    b->buf[rdlen_pos] = (rdlen >> 8) & 0xff;
    b->buf[rdlen_pos + 1] = rdlen & 0xff;
}

/* ------------------------------------------------------------------- tests */

static void service_matches_test(abts_case *tc, void *data)
{
    /* S8 home-routed selection. */
    ABTS_TRUE(tc, mme_dns_service_matches(
            "x-3gpp-pgw:x-s8-gtp", MME_PGW_DISCOVERY_IFACE_S8));
    ABTS_TRUE(tc, !mme_dns_service_matches(
            "x-3gpp-pgw:x-s8-gtp", MME_PGW_DISCOVERY_IFACE_S5));

    /* S5 selection. */
    ABTS_TRUE(tc, mme_dns_service_matches(
            "x-3gpp-pgw:x-s5-gtp", MME_PGW_DISCOVERY_IFACE_S5));
    ABTS_TRUE(tc, !mme_dns_service_matches(
            "x-3gpp-pgw:x-s5-gtp", MME_PGW_DISCOVERY_IFACE_S8));

    /* BOTH accepts either protocol tag. */
    ABTS_TRUE(tc, mme_dns_service_matches(
            "x-3gpp-pgw:x-s8-gtp", MME_PGW_DISCOVERY_IFACE_BOTH));
    ABTS_TRUE(tc, mme_dns_service_matches(
            "x-3gpp-pgw:x-s5-gtp", MME_PGW_DISCOVERY_IFACE_BOTH));

    /* Must be a PGW node; an SGW record must never match. */
    ABTS_TRUE(tc, !mme_dns_service_matches(
            "x-3gpp-sgw:x-s8-gtp", MME_PGW_DISCOVERY_IFACE_S8));

    /* Matching is case-insensitive (RFC 3958). */
    ABTS_TRUE(tc, mme_dns_service_matches(
            "X-3GPP-PGW:X-S8-GTP", MME_PGW_DISCOVERY_IFACE_S8));

    /* NULL is handled safely. */
    ABTS_TRUE(tc, !mme_dns_service_matches(NULL, MME_PGW_DISCOVERY_IFACE_S8));
}

static void parse_naptr_filter_and_sort_test(abts_case *tc, void *data)
{
    dnsbuf_t b;
    mme_dns_naptr_t out[8];
    int n;

    /*
     * Three records: two PGW/S8 with different Order, plus one SGW record that
     * must be filtered out. Lower Order must sort first.
     */
    dns_begin(&b, 3);
    dns_add_naptr(&b, 100, 10, "a", "x-3gpp-pgw:x-s8-gtp",
            "high-order.pgw.example", 3600);
    dns_add_naptr(&b, 50, 20, "a", "x-3gpp-pgw:x-s8-gtp",
            "low-order.pgw.example", 3600);
    dns_add_naptr(&b, 100, 5, "a", "x-3gpp-sgw:x-s8-gtp",
            "sgw.example", 3600);

    n = mme_dns_parse_naptr(b.buf, b.len, MME_PGW_DISCOVERY_IFACE_S8, out, 8);
    ABTS_INT_EQUAL(tc, 2, n);
    /* Sorted by Order: the Order=50 record comes first. */
    ABTS_INT_EQUAL(tc, 50, out[0].order);
    ABTS_STR_EQUAL(tc, "low-order.pgw.example", out[0].replacement);
    ABTS_INT_EQUAL(tc, 100, out[1].order);
    ABTS_STR_EQUAL(tc, "high-order.pgw.example", out[1].replacement);

    /* Same answer, but no record matches S5: expect nothing. */
    n = mme_dns_parse_naptr(b.buf, b.len, MME_PGW_DISCOVERY_IFACE_S5, out, 8);
    ABTS_INT_EQUAL(tc, 0, n);
}

static void parse_naptr_s8_over_s5_test(abts_case *tc, void *data)
{
    dnsbuf_t b;
    mme_dns_naptr_t out[8];
    int n;

    /*
     * Same Order: an S5 record (lower Preference) and an S8 record. In S8
     * mode only the S8 record is selected even though S5 has the better
     * Preference; in BOTH mode both are returned, Preference-ordered.
     */
    dns_begin(&b, 2);
    dns_add_naptr(&b, 100, 100, "a", "x-3gpp-pgw:x-s8-gtp",
            "s8.pgw.example", 3600);
    dns_add_naptr(&b, 100, 10, "a", "x-3gpp-pgw:x-s5-gtp",
            "s5.pgw.example", 3600);

    n = mme_dns_parse_naptr(b.buf, b.len, MME_PGW_DISCOVERY_IFACE_S8, out, 8);
    ABTS_INT_EQUAL(tc, 1, n);
    ABTS_STR_EQUAL(tc, "s8.pgw.example", out[0].replacement);

    n = mme_dns_parse_naptr(b.buf, b.len, MME_PGW_DISCOVERY_IFACE_BOTH, out, 8);
    ABTS_INT_EQUAL(tc, 2, n);
    /* Lower Preference (S5, 10) sorts ahead of S8 (100). */
    ABTS_STR_EQUAL(tc, "s5.pgw.example", out[0].replacement);
    ABTS_STR_EQUAL(tc, "s8.pgw.example", out[1].replacement);
}

static void parse_naptr_flags_test(abts_case *tc, void *data)
{
    dnsbuf_t b;
    mme_dns_naptr_t out[4];
    int n;

    /* The terminal flag ("a"/"s") is preserved for the resolver to act on. */
    dns_begin(&b, 1);
    dns_add_naptr(&b, 100, 10, "s", "x-3gpp-pgw:x-s8-gtp",
            "_gtp._udp.pgw.example", 1800);

    n = mme_dns_parse_naptr(b.buf, b.len, MME_PGW_DISCOVERY_IFACE_S8, out, 4);
    ABTS_INT_EQUAL(tc, 1, n);
    ABTS_STR_EQUAL(tc, "s", out[0].flags);
    ABTS_STR_EQUAL(tc, "_gtp._udp.pgw.example", out[0].replacement);
    ABTS_INT_EQUAL(tc, 1800, (int)out[0].ttl);

    /* Defensive: empty/garbage input yields zero records, not a crash. */
    ABTS_INT_EQUAL(tc, 0,
            mme_dns_parse_naptr(NULL, 0, MME_PGW_DISCOVERY_IFACE_S8, out, 4));
    ABTS_INT_EQUAL(tc, 0,
            mme_dns_parse_naptr(b.buf, 3, MME_PGW_DISCOVERY_IFACE_S8, out, 4));
}

static void apn_fqdn_test(abts_case *tc, void *data)
{
    ogs_plmn_id_t plmn;
    char fqdn[OGS_MAX_FQDN_LEN + 1];

    /* 3-digit MNC: MCC=262, MNC=011 -> mnc011.mcc262 */
    ogs_plmn_id_build(&plmn, 262, 11, 3);
    ABTS_INT_EQUAL(tc, OGS_OK,
            mme_dns_apn_fqdn("ims", &plmn, fqdn, sizeof(fqdn)));
    ABTS_STR_EQUAL(tc,
            "ims.apn.epc.mnc011.mcc262.3gppnetwork.org", fqdn);

    /* 2-digit MNC: MCC=001, MNC=01 -> zero-padded to mnc001 (TS 23.003). */
    ogs_plmn_id_build(&plmn, 1, 1, 2);
    ABTS_INT_EQUAL(tc, OGS_OK,
            mme_dns_apn_fqdn("internet", &plmn, fqdn, sizeof(fqdn)));
    ABTS_STR_EQUAL(tc,
            "internet.apn.epc.mnc001.mcc001.3gppnetwork.org", fqdn);

    /* NULL arguments are rejected. */
    ABTS_INT_EQUAL(tc, OGS_ERROR,
            mme_dns_apn_fqdn(NULL, &plmn, fqdn, sizeof(fqdn)));
    ABTS_INT_EQUAL(tc, OGS_ERROR,
            mme_dns_apn_fqdn("ims", NULL, fqdn, sizeof(fqdn)));
}

abts_suite *test_mme_dns(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, service_matches_test, NULL);
    abts_run_test(suite, parse_naptr_filter_and_sort_test, NULL);
    abts_run_test(suite, parse_naptr_s8_over_s5_test, NULL);
    abts_run_test(suite, parse_naptr_flags_test, NULL);
    abts_run_test(suite, apn_fqdn_test, NULL);

    return suite;
}

int main(int argc, const char *const argv[])
{
    abts_suite *suite = NULL;
    int rv;

    ogs_core_initialize();

    suite = test_mme_dns(suite);
    rv = abts_report(suite);

    ogs_core_terminate();

    return rv;
}
