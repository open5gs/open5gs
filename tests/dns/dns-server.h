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

/* Minimal programmable stub DNS server (UDP, A/SRV/NAPTR) for tests */

#ifndef TEST_DNS_SERVER_H
#define TEST_DNS_SERVER_H

#ifdef __cplusplus
extern "C" {
#endif

#define TEST_DNS_T_A        1
#define TEST_DNS_T_SRV      33
#define TEST_DNS_T_NAPTR    35

#define TEST_DNS_MODE_ANSWER    0   /* answer from the record table */
#define TEST_DNS_MODE_EMPTY     1   /* always NOERROR with 0 answers */
#define TEST_DNS_MODE_DROP      2   /* never answer */

typedef struct test_dns_record_s {
    const char *qname;      /* owner name the query must match */
    int qtype;              /* TEST_DNS_T_* */

    /* NAPTR */
    int order;
    int preference;
    const char *flags;
    const char *service;
    const char *replacement;

    /* SRV */
    int priority;
    int weight;
    int port;
    const char *target;

    /* A */
    const char *ipv4;
} test_dns_record_t;

int test_dns_server_start(const char *address, int port);
void test_dns_server_stop(void);

void test_dns_server_set_records(const test_dns_record_t *records, int num);
void test_dns_server_set_mode(int mode);

/* Number of queries received whose qname contains the given substring
 * (NULL = all queries) */
int test_dns_server_num_queries(const char *qname_substr);
void test_dns_server_reset_counters(void);

#ifdef __cplusplus
}
#endif

#endif /* TEST_DNS_SERVER_H */
