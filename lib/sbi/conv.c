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

#include "ogs-sbi.h"
#include "yuarel.h"

static char *ogs_uridup(bool https, ogs_sockaddr_t *addr, ogs_sbi_header_t *h)
{
    char buf[OGS_ADDRSTRLEN];
    char uri[OGS_HUGE_LEN];
    char *p, *last;
    int i;

    ogs_assert(addr);
    ogs_assert(h);

    p = uri;
    last = uri + OGS_HUGE_LEN;

    /* HTTP scheme is selected based on TLS information */
    if (https == true)
        p = ogs_slprintf(p, last, "https://");
    else
        p = ogs_slprintf(p, last, "http://");

    /* IP address */
    if (addr->ogs_sa_family == AF_INET6)
        p = ogs_slprintf(p, last, "[%s]", OGS_ADDR(addr, buf));
    else
        p = ogs_slprintf(p, last, "%s", OGS_ADDR(addr, buf));

    /* Port number */
    if (OGS_PORT(addr) != OGS_SBI_HTTP_PORT) {
        p = ogs_slprintf(p, last, ":%d", OGS_PORT(addr));
    }

    /* API */
    ogs_assert(h->service.name);
    p = ogs_slprintf(p, last, "/%s", h->service.name);
    ogs_assert(h->api.version);
    p = ogs_slprintf(p, last, "/%s", h->api.version);

    /* Resource */
    ogs_assert(h->resource.component[0]);
    for (i = 0; i < OGS_SBI_MAX_NUM_OF_RESOURCE_COMPONENT &&
                        h->resource.component[i]; i++)
        p = ogs_slprintf(p, last, "/%s", h->resource.component[i]);

    return ogs_strdup(uri);
}

char *ogs_sbi_server_uri(ogs_sbi_server_t *server, ogs_sbi_header_t *h)
{
    bool https = false;

    ogs_assert(server);
    ogs_assert(h);

    if (server->tls.key && server->tls.pem)
        https = true;

    return ogs_uridup(https, server->addr, h);
}

char *ogs_sbi_client_uri(ogs_sbi_client_t *client, ogs_sbi_header_t *h)
{
    bool https = false;

    ogs_assert(client);
    ogs_assert(h);

    if (client->tls.key && client->tls.pem)
        https = true;

    return ogs_uridup(https, client->addr, h);
}

/**
 * Returns a url-decoded version of str
 * IMPORTANT: be sure to free() the returned string after use
 * Thanks Geek Hideout!
 * http://www.geekhideout.com/urlcode.shtml
 */
static char *url_decode(const char *str)
{
    if (str != NULL) {
        char *pstr = (char*)str;
        char *buf = ogs_malloc(strlen(str) + 1);
        char *pbuf = buf;
        while (*pstr) {
            if (*pstr == '%') {
                if (pstr[1] && pstr[2]) {
                    *pbuf++ = ogs_from_hex(pstr[1]) << 4 |
                                ogs_from_hex(pstr[2]);
                    pstr += 2;
                }
            } else if (*pstr == '+') {
                *pbuf++ = ' ';
            } else {
                *pbuf++ = * pstr;
            }
            pstr++;
        }
        *pbuf = '\0';
        return buf;
    } else {
        return NULL;
    }
}

char *ogs_sbi_parse_uri(char *uri, const char *delim, char **saveptr)
{
    char *item = NULL;

    item = url_decode(strtok_r(uri, delim, saveptr));
    if (!item) {
        return NULL;
    }

    return item;
}

ogs_sockaddr_t *ogs_sbi_getaddr_from_uri(char *uri)
{
    int rv;
    struct yuarel yuarel;
    char *p = NULL;
    int port;

    ogs_sockaddr_t *addr = NULL;

    p = ogs_strdup(uri);

    rv = yuarel_parse(&yuarel, p);
    if (rv != OGS_OK) {
        ogs_free(p);
        ogs_error("yuarel_parse() failed [%s]", uri);
        return NULL;
    }

    if (!yuarel.scheme) {
        ogs_error("No http.scheme found [%s]", uri);
        ogs_free(p);
        return NULL;
    }

    if (strcmp(yuarel.scheme, "https") == 0) {
        port = OGS_SBI_HTTPS_PORT;
    } else if (strcmp(yuarel.scheme, "http") == 0) {
        port = OGS_SBI_HTTP_PORT;
    } else {
        ogs_error("Invalid http.scheme [%s:%s]", yuarel.scheme, uri);
        ogs_free(p);
        return NULL;
    }

    if (!yuarel.host) {
        ogs_error("No http.host found [%s]", uri);
        ogs_free(p);
        return NULL;
    }

    if (yuarel.port) port = yuarel.port;

    rv = ogs_getaddrinfo(&addr, AF_UNSPEC, yuarel.host, port, 0);
    if (rv != OGS_OK) {
        ogs_error("ogs_getaddrinfo() failed [%s]", uri);
        ogs_free(p);
        return NULL;
    }

    ogs_free(p);
    return addr;
}

char *ogs_sbi_bitrate_to_string(uint64_t bitrate, int unit)
{
    if (unit == OGS_SBI_BITRATE_KBPS) {
        return ogs_msprintf("%lld Kbps",
                (long long)bitrate / 1024);
    } else if (unit == OGS_SBI_BITRATE_MBPS) {
        return ogs_msprintf("%lld Mbps",
                (long long)bitrate / 1024 / 1024);
    } else if (unit == OGS_SBI_BITRATE_GBPS) {
        return ogs_msprintf("%lld Gbps",
                (long long)bitrate / 1024 / 1024 / 1024);
    } else if (unit == OGS_SBI_BITRATE_TBPS) {
        return ogs_msprintf("%lld Tbps",
                (long long)bitrate / 1024 / 1024 / 1024 / 1024);
    }

    return ogs_msprintf("%lld bps", (long long)bitrate);
}

uint64_t ogs_sbi_bitrate_from_string(char *str)
{
    char *unit = NULL;
    uint64_t bitrate = 0;
    ogs_assert(str);

    unit = strrchr(str, ' ');
    bitrate = atoll(str);

    SWITCH(unit+1)
    CASE("Kbps")
        return bitrate * 1024;
    CASE("Mbps")
        return bitrate * 1024 * 1024;
    CASE("Gbps")
        return bitrate * 1024 * 1024 * 1024;
    CASE("Tbps")
        return bitrate * 1024 * 1024 * 1024 * 1024;
    DEFAULT
    END
    return bitrate;
}
