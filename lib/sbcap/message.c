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

#include "ogs-sbcap.h"

int __ogs_sbcap_domain;

ogs_pkbuf_t *ogs_sbcap_encode(ogs_sbcap_message_t *message)
{
    ogs_pkbuf_t *pkbuf = NULL;

    ogs_assert(message);

    if (ogs_log_get_domain_level(OGS_LOG_DOMAIN) >= OGS_LOG_TRACE)
        asn_fprint(stdout, &asn_DEF_SBCAP_SBC_AP_PDU, message);

    pkbuf = ogs_asn_encode(&asn_DEF_SBCAP_SBC_AP_PDU, message);
    if (!pkbuf) {
        ogs_error("ogs_asn_encode() failed");
        return NULL;
    }

    return pkbuf;
}

int ogs_sbcap_decode(ogs_sbcap_message_t *message, ogs_pkbuf_t *pkbuf)
{
    int rv;
    ogs_assert(message);
    ogs_assert(pkbuf);
    ogs_assert(pkbuf->data);
    ogs_assert(pkbuf->len);
    ogs_warn("Trying to decode SBCAP-PDU...");
    rv = ogs_asn_decode(&asn_DEF_SBCAP_SBC_AP_PDU,
            message, sizeof(ogs_sbcap_message_t), pkbuf);
    //ogs_warn("Looks like message decoded, checking.... SBCAP-PDU...");
    if (rv != OGS_OK) {
        ogs_warn("Failed to decode SBCAP-PDU");
        return rv;
    }

    if (ogs_log_get_domain_level(OGS_LOG_DOMAIN) >= OGS_LOG_TRACE)
        asn_fprint(stdout, &asn_DEF_SBCAP_SBC_AP_PDU, message);

    return OGS_OK;
}

void ogs_sbcap_free(ogs_sbcap_message_t *message)
{
    ogs_assert(message);
    ogs_asn_free(&asn_DEF_SBCAP_SBC_AP_PDU, message);
}
