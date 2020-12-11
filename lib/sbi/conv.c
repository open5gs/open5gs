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

    return ogs_uridup(https, server->node.addr, h);
}

char *ogs_sbi_client_uri(ogs_sbi_client_t *client, ogs_sbi_header_t *h)
{
    bool https = false;

    ogs_assert(client);
    ogs_assert(h);

    if (client->tls.key && client->tls.pem)
        https = true;

    return ogs_uridup(https, client->node.addr, h);
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

#define MAX_TIMESTR_LEN 128

char *ogs_sbi_localtime_string(ogs_time_t timestamp)
{
    struct tm tm;

    char datetime[MAX_TIMESTR_LEN];
    char timezone[MAX_TIMESTR_LEN];

    ogs_localtime(ogs_time_sec(timestamp), &tm);
    ogs_strftime(datetime, sizeof datetime, "%Y-%m-%dT%H:%M:%S", &tm);
    ogs_strftime(timezone, sizeof timezone, "%z", &tm);

    return ogs_msprintf("%s.%06lld%s",
            datetime, (long long)ogs_time_usec(timestamp), timezone);
}

char *ogs_sbi_gmtime_string(ogs_time_t timestamp)
{
    struct tm tm;

    char datetime[MAX_TIMESTR_LEN];

    ogs_gmtime(ogs_time_sec(timestamp), &tm);
    ogs_strftime(datetime, sizeof datetime, "%Y-%m-%dT%H:%M:%S", &tm);

    return ogs_msprintf("%s.%06lldZ",
            datetime, (long long)ogs_time_usec(timestamp));
}

char *ogs_sbi_timezone_string(int tm_gmtoff)
{
    struct tm tm;

    char timezone[MAX_TIMESTR_LEN];

    ogs_localtime(ogs_time_now(), &tm);
    tm.tm_gmtoff = tm_gmtoff;
    ogs_strftime(timezone, sizeof timezone, "%z", &tm);

    return ogs_msprintf("%s", timezone);
}

bool ogs_sbi_time_from_string(ogs_time_t *timestamp, char *str)
{
    int rv, i, j, k;
    struct tm tm;
    bool is_seconds;
    char seconds[MAX_TIMESTR_LEN];
    char subsecs[MAX_TIMESTR_LEN];
    ogs_time_t usecs;

    ogs_assert(str);
    ogs_assert(timestamp);

    memset(seconds, 0, sizeof seconds);
    memset(subsecs, 0, sizeof subsecs);

    is_seconds = true;
    i = 0; j = 0, k = 0;
    while(str[i]) {
        if (is_seconds == true && str[i] == '.')
            is_seconds = false;
        else if (is_seconds == false && (str[i] < '0' || str[i] > '9'))
            is_seconds = true;

        if (is_seconds == true) seconds[j++] = str[i];
        else subsecs[k++] = str[i];

        i++;
    }

    memset(&tm, 0, sizeof(tm));
    ogs_strptime(seconds, "%Y-%m-%dT%H:%M:%S%z", &tm);
    usecs = (ogs_time_t)(atof(subsecs) * 1000000);

    rv = ogs_time_from_gmt(timestamp, &tm, usecs);
    if (rv != OGS_OK) {
        ogs_error("Cannot convert time [%s]", str);
        return false;
    }

    return true;
}

char *ogs_sbi_s_nssai_to_string(ogs_s_nssai_t *s_nssai)
{
    cJSON *item = NULL;
    OpenAPI_snssai_t sNSSAI;

    char *v = NULL;

    ogs_assert(s_nssai);

    sNSSAI.sst = s_nssai->sst;
    sNSSAI.sd = ogs_s_nssai_sd_to_string(s_nssai->sd);

    item = OpenAPI_snssai_convertToJSON(&sNSSAI);
    ogs_assert(item);
    if (sNSSAI.sd) ogs_free(sNSSAI.sd);

    v = cJSON_Print(item);
    ogs_assert(v);
    cJSON_Delete(item);

    return v;
}

bool ogs_sbi_s_nssai_from_string(ogs_s_nssai_t *s_nssai, char *str)
{
    bool rc = false;

    cJSON *item = NULL;
    OpenAPI_snssai_t *sNSSAI = NULL;

    ogs_assert(s_nssai);
    ogs_assert(str);

    item = cJSON_Parse(str);
    if (item) {
        sNSSAI = OpenAPI_snssai_parseFromJSON(item);
        if (sNSSAI) {
            s_nssai->sst = sNSSAI->sst;
            s_nssai->sd = ogs_s_nssai_sd_from_string(sNSSAI->sd);
            OpenAPI_snssai_free(sNSSAI);
            rc = true;
        }
        cJSON_Delete(item);
    }

    return rc;
}

OpenAPI_plmn_id_t *ogs_sbi_build_plmn_id(ogs_plmn_id_t *plmn_id)
{
    OpenAPI_plmn_id_t *PlmnId = NULL;

    ogs_assert(plmn_id);

    PlmnId = ogs_calloc(1, sizeof(*PlmnId));
    ogs_assert(PlmnId);

    PlmnId->mcc = ogs_plmn_id_mcc_string(plmn_id);
    ogs_assert(PlmnId->mcc);
    PlmnId->mnc = ogs_plmn_id_mnc_string(plmn_id);
    ogs_assert(PlmnId->mnc);

    return PlmnId;
}

bool ogs_sbi_parse_plmn_id(
        ogs_plmn_id_t *plmn_id, OpenAPI_plmn_id_t *PlmnId)
{
    ogs_assert(plmn_id);
    ogs_assert(PlmnId);
    ogs_assert(PlmnId->mcc);
    ogs_assert(PlmnId->mnc);

    ogs_plmn_id_build(plmn_id,
            atoi(PlmnId->mcc), atoi(PlmnId->mnc), strlen(PlmnId->mnc));

    return true;
}

void ogs_sbi_free_plmn_id(OpenAPI_plmn_id_t *PlmnId)
{
    ogs_assert(PlmnId);

    if (PlmnId->mcc)
        ogs_free(PlmnId->mcc);
    if (PlmnId->mnc)
        ogs_free(PlmnId->mnc);

    ogs_free(PlmnId);
}

OpenAPI_plmn_id_nid_t *ogs_sbi_build_plmn_id_nid(ogs_plmn_id_t *plmn_id)
{
    OpenAPI_plmn_id_nid_t *PlmnIdNid = NULL;

    ogs_assert(plmn_id);

    PlmnIdNid = ogs_calloc(1, sizeof(*PlmnIdNid));
    ogs_assert(PlmnIdNid);

    PlmnIdNid->mcc = ogs_plmn_id_mcc_string(plmn_id);
    ogs_assert(PlmnIdNid->mcc);
    PlmnIdNid->mnc = ogs_plmn_id_mnc_string(plmn_id);
    ogs_assert(PlmnIdNid->mnc);

    return PlmnIdNid;
}

bool ogs_sbi_parse_plmn_id_nid(
        ogs_plmn_id_t *plmn_id, OpenAPI_plmn_id_nid_t *PlmnIdNid)
{
    ogs_assert(plmn_id);
    ogs_assert(PlmnIdNid);
    ogs_assert(PlmnIdNid->mcc);
    ogs_assert(PlmnIdNid->mnc);

    ogs_plmn_id_build(plmn_id,
            atoi(PlmnIdNid->mcc), atoi(PlmnIdNid->mnc), strlen(PlmnIdNid->mnc));

    return true;
}

void ogs_sbi_free_plmn_id_nid(OpenAPI_plmn_id_nid_t *PlmnIdNid)
{
    ogs_assert(PlmnIdNid);

    if (PlmnIdNid->mcc)
        ogs_free(PlmnIdNid->mcc);
    if (PlmnIdNid->mnc)
        ogs_free(PlmnIdNid->mnc);
    if (PlmnIdNid->nid)
        ogs_free(PlmnIdNid->nid);

    ogs_free(PlmnIdNid);
}

OpenAPI_guami_t *ogs_sbi_build_guami(ogs_guami_t *guami)
{
    OpenAPI_guami_t *Guami = NULL;

    ogs_assert(guami);

    Guami = ogs_calloc(1, sizeof(*Guami));
    ogs_assert(Guami);

    Guami->plmn_id = ogs_sbi_build_plmn_id(&guami->plmn_id);
    ogs_assert(Guami->plmn_id);
    Guami->amf_id = ogs_amf_id_to_string(&guami->amf_id);
    ogs_assert(Guami->amf_id);

    return Guami;
}

bool ogs_sbi_parse_guami(ogs_guami_t *guami, OpenAPI_guami_t *Guami)
{
    ogs_assert(guami);
    ogs_assert(Guami);
    ogs_assert(Guami->amf_id);
    ogs_assert(Guami->plmn_id);

    ogs_amf_id_from_string(&guami->amf_id, Guami->amf_id);
    ogs_sbi_parse_plmn_id(&guami->plmn_id, Guami->plmn_id);

    return true;
}

void ogs_sbi_free_guami(OpenAPI_guami_t *Guami)
{
    ogs_assert(Guami);

    if (Guami->plmn_id)
        ogs_sbi_free_plmn_id(Guami->plmn_id);
    if (Guami->amf_id)
        ogs_free(Guami->amf_id);
    ogs_free(Guami);
}

OpenAPI_nr_location_t *ogs_sbi_build_nr_location(
    ogs_5gs_tai_t *tai, ogs_nr_cgi_t *nr_cgi)
{
    OpenAPI_nr_location_t *NrLocation = NULL;
    OpenAPI_tai_t *Tai = NULL;
    OpenAPI_ncgi_t *Ncgi = NULL;

    ogs_assert(tai);
    ogs_assert(nr_cgi);

    Tai = ogs_calloc(1, sizeof(*Tai));
    ogs_assert(Tai);
    Tai->plmn_id = ogs_sbi_build_plmn_id(&tai->plmn_id);
    Tai->tac = ogs_uint24_to_string(tai->tac);

    Ncgi = ogs_calloc(1, sizeof(*Ncgi));
    ogs_assert(Ncgi);
    Ncgi->plmn_id = ogs_sbi_build_plmn_id(&nr_cgi->plmn_id);
    Ncgi->nr_cell_id = ogs_uint36_to_string(nr_cgi->cell_id);

    NrLocation = ogs_calloc(1, sizeof(*NrLocation));
    ogs_assert(NrLocation);
    NrLocation->tai = Tai;
    NrLocation->ncgi = Ncgi;

    return NrLocation;
}

bool ogs_sbi_parse_nr_location(ogs_5gs_tai_t *tai, ogs_nr_cgi_t *nr_cgi,
        OpenAPI_nr_location_t *NrLocation)
{
    OpenAPI_tai_t *Tai = NULL;
    OpenAPI_ncgi_t *Ncgi = NULL;

    ogs_assert(tai);
    ogs_assert(nr_cgi);
    ogs_assert(NrLocation);

    Tai = NrLocation->tai;
    if (Tai) {
        if (Tai->plmn_id)
            ogs_sbi_parse_plmn_id(&tai->plmn_id, Tai->plmn_id);
        if (Tai->tac)
            tai->tac = ogs_uint24_from_string(Tai->tac);
    }

    Ncgi = NrLocation->ncgi;
    if (Ncgi) {
        if (Ncgi->plmn_id)
            ogs_sbi_parse_plmn_id(&nr_cgi->plmn_id, Ncgi->plmn_id);
        if (Ncgi->nr_cell_id)
            nr_cgi->cell_id = ogs_uint36_from_string(Ncgi->nr_cell_id);

    }

    return true;
}

void ogs_sbi_free_nr_location(OpenAPI_nr_location_t *NrLocation)
{
    OpenAPI_tai_t *Tai = NULL;
    OpenAPI_ncgi_t *Ncgi = NULL;

    ogs_assert(NrLocation);

    Tai = NrLocation->tai;
    if (Tai) {
        if (Tai->plmn_id)
            ogs_sbi_free_plmn_id(Tai->plmn_id);
        if (Tai->tac)
            ogs_free(Tai->tac);
        ogs_free(Tai);
    }

    Ncgi = NrLocation->ncgi;
    if (Ncgi) {
        if (Ncgi->plmn_id)
            ogs_sbi_free_plmn_id(Ncgi->plmn_id);
        if (Ncgi->nr_cell_id)
            ogs_free(Ncgi->nr_cell_id);
        ogs_free(Ncgi);
    }

    ogs_free(NrLocation);
}
