/* Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * Copyright (C) 2019 by Sukchan Lee <acetcom@gmail.com>
 *
 * This file is part of Open5GS.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#if !defined(OGS_CORE_INSIDE) && !defined(OGS_CORE_COMPILATION)
#error "This header cannot be included directly."
#endif

#ifndef OGS_SOCKADDR_H
#define OGS_SOCKADDR_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ogs_sockaddr_s ogs_sockaddr_t;
struct ogs_sockaddr_s {
    /* Reserved Area
     *   - Should not add any atrribute in this area.
     *
     *   e.g)
     *   struct sockaddr addr;
     *   ...
     *   sockaddr_len((ogs_sockaddr_t *)&addr);
     */
#define ogs_sa_family sa.sa_family
#define ogs_sin_port sin.sin_port
    union {
        struct sockaddr_storage ss;
        struct sockaddr_in sin;
        struct sockaddr_in6 sin6;
        struct sockaddr sa;
    };

    /*
     * First we created a 'hostname' variable.
     * If there is a name in the configuration file,
     * it is set in the 'hostname' of ogs_sockaddr_t.
     * Then, it immediately call getaddrinfo() to fill addr in ogs_sockaddr_t.
     */
    char *hostname;

    ogs_sockaddr_t *next;
};

typedef struct ogs_ipsubnet_s {
    int family;

    uint32_t sub[4]; /* big enough for IPv4 and IPv6 addresses */
    uint32_t mask[4];
} ogs_ipsubnet_t;

int ogs_getaddrinfo(ogs_sockaddr_t **sa_list,
        int family, const char *hostname, uint16_t port, int flags);
int ogs_freeaddrinfo(ogs_sockaddr_t *sa_list);

int ogs_addaddrinfo(ogs_sockaddr_t **sa_list,
        int family, const char *hostname, uint16_t port, int flags);
int ogs_copyaddrinfo(
        ogs_sockaddr_t **dst, const ogs_sockaddr_t *src);
int ogs_filteraddrinfo(ogs_sockaddr_t **sa_list, int family);
int ogs_sortaddrinfo(ogs_sockaddr_t **sa_list, int family);

void ogs_merge_single_addrinfo(
        ogs_sockaddr_t **dest, const ogs_sockaddr_t *item);
void ogs_merge_addrinfo(ogs_sockaddr_t **dest, const ogs_sockaddr_t *src);

ogs_sockaddr_t *ogs_link_local_addr(const char *dev, const ogs_sockaddr_t *sa);
ogs_sockaddr_t *ogs_link_local_addr_by_dev(const char *dev);
ogs_sockaddr_t *ogs_link_local_addr_by_sa(const ogs_sockaddr_t *sa);
int ogs_filter_ip_version(ogs_sockaddr_t **addr,
        int no_ipv4, int no_ipv6, int prefer_ipv4);

#define OGS_ADDRSTRLEN INET6_ADDRSTRLEN
#define OGS_ADDR(__aDDR, __bUF) \
    ogs_inet_ntop(__aDDR, __bUF, OGS_ADDRSTRLEN)
#define OGS_PORT(__aDDR) \
    be16toh((__aDDR)->ogs_sin_port)
const char *ogs_inet_ntop(void *sa, char *buf, int buflen);
int ogs_inet_pton(int family, const char *src, void *sa);

socklen_t ogs_sockaddr_len(const void *sa);
bool ogs_sockaddr_is_equal(const void *p, const void *q);
bool ogs_sockaddr_is_equal_addr(const void *p, const void *q);

int ogs_ipsubnet(ogs_ipsubnet_t *ipsub,
        const char *ipstr, const char *mask_or_numbits);

char *ogs_gethostname(ogs_sockaddr_t *addr);
char *ogs_ipstrdup(ogs_sockaddr_t *addr);
char *ogs_sockaddr_to_string_static(ogs_sockaddr_t *sa_list);

#ifdef __cplusplus
}
#endif

#endif /* OGS_SOCKADDR_H */
