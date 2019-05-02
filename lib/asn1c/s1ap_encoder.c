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

#include "s1ap_message.h"

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __base_s1ap_domain

int s1ap_encode_pdu(ogs_pkbuf_t **pkbuf, s1ap_message_t *message)
{
    asn_enc_rval_t enc_ret = {0};

    ogs_assert(message);

    if (ogs_log_get_domain_level(OGS_LOG_DOMAIN) >= OGS_LOG_TRACE)
        asn_fprint(stdout, &asn_DEF_S1AP_S1AP_PDU, message);

    *pkbuf = ogs_pkbuf_alloc(NULL, MAX_SDU_LEN);
    ogs_pkbuf_put(*pkbuf, MAX_SDU_LEN);

    enc_ret = aper_encode_to_buffer(&asn_DEF_S1AP_S1AP_PDU, NULL,
                    message, (*pkbuf)->data, MAX_SDU_LEN);
    if (enc_ret.encoded < 0)
    {
        ogs_error("Failed to encode S1AP-PDU[%d]", (int)enc_ret.encoded);
        ogs_pkbuf_free(*pkbuf);
        return OGS_ERROR;
    }

    ogs_pkbuf_trim(*pkbuf, (enc_ret.encoded >> 3));

    return OGS_OK;
}
