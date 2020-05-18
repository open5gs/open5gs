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

#include "sbi-private.h"

static char *ogs_uridup(bool https, ogs_sockaddr_t *addr,
    const char *service_name, const char *api_version,
    const char *resource_name, const char *resource_id)
{
    char buf[OGS_ADDRSTRLEN];
    char url[OGS_HUGE_LEN];
    char *p, *last;

    ogs_assert(addr);
    ogs_assert(service_name);
    ogs_assert(api_version);
    ogs_assert(resource_name);

    p = url;
    last = url + OGS_HUGE_LEN;

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
    ogs_assert(service_name);
    p = ogs_slprintf(p, last, "/%s", service_name);
    ogs_assert(api_version);
    p = ogs_slprintf(p, last, "/%s", api_version);

    /* Resource */
    ogs_assert(resource_name);
    p = ogs_slprintf(p, last, "/%s", resource_name);
    if (resource_id)
        p = ogs_slprintf(p, last, "/%s", resource_id);

    return ogs_strdup(url);
}

char *ogs_sbi_server_uri(ogs_sbi_server_t *server,
    const char *service_name, const char *api_version,
    const char *resource_name, const char *resource_id)
{
    bool https = false;
    if (server->tls.key && server->tls.pem)
        https = true;

    return ogs_uridup(https, server->addr, service_name, api_version,
            resource_name, resource_id);
}

char *ogs_sbi_client_uri(ogs_sbi_client_t *client,
    const char *service_name, const char *api_version,
    const char *resource_name, const char *resource_id)
{
    bool https = false;
    if (client->tls.key && client->tls.pem)
        https = true;

    return ogs_uridup(https, client->addr, service_name, api_version,
            resource_name, resource_id);
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

char *ogs_sbi_parse_url(char *url, const char *delim, char **saveptr)
{
    char *item = NULL;

    item = url_decode(strtok_r(url, delim, saveptr));
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
