/*
 * Copyright (C) 2019,2020 by Sukchan Lee <acetcom@gmail.com>
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

#include "ogs-ngap.h"

int __ogs_ngap_domain;

ogs_pkbuf_t *ogs_ngap_encode(ogs_ngap_message_t *message)
{
    ogs_pkbuf_t *pkbuf = NULL;

    ogs_assert(message);

    if (ogs_log_get_domain_level(OGS_LOG_DOMAIN) >= OGS_LOG_TRACE)
        asn_fprint(stdout, &asn_DEF_NGAP_NGAP_PDU, message);

    pkbuf = ogs_asn_encode(&asn_DEF_NGAP_NGAP_PDU, message);
    if (!pkbuf) {
        ogs_error("ogs_asn_encode() failed");
        return NULL;
    }

    return pkbuf;
}

int ogs_ngap_decode(ogs_ngap_message_t *message, ogs_pkbuf_t *pkbuf)
{
    int rv;
    ogs_assert(message);
    ogs_assert(pkbuf);
    ogs_assert(pkbuf->data);
    ogs_assert(pkbuf->len);

    rv = ogs_asn_decode(&asn_DEF_NGAP_NGAP_PDU,
            message, sizeof(ogs_ngap_message_t), pkbuf);
    if (rv != OGS_OK) {
        ogs_warn("Failed to decode NGAP-PDU");
        return rv;
    }

    if (ogs_log_get_domain_level(OGS_LOG_DOMAIN) >= OGS_LOG_TRACE)
        asn_fprint(stdout, &asn_DEF_NGAP_NGAP_PDU, message);

    return OGS_OK;
}

void ogs_ngap_free(ogs_ngap_message_t *message)
{
    ogs_assert(message);
    ogs_asn_free(&asn_DEF_NGAP_NGAP_PDU, message);
}
