/*
 * Copyright (C) 2019-2025 by Sukchan Lee <acetcom@gmail.com>
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

#include "context.h"
#include "ngap-path.h"
#include "sbc-handler.h"

void sbc_handle_write_replace_warning_request(sbc_pws_data_t *sbc_pws)
{
    ogs_pkbuf_t *ngapbuf = NULL;
    int r;

    /* Build NGAP Write Replace Warning Request message */
    ngapbuf = ngap_build_write_replace_warning_request(sbc_pws);
    if (!ngapbuf) {
        ogs_error("ngap_build_write_replace_warning_request() failed");
        return;
    }

    /* Send to AMF */
    r = pwsiws_ngap_send_to_amf(ngapbuf);
    ogs_info("pwsiws_ngap_send_to_amf() %d", r);
    ogs_expect(r == OGS_OK);
    ogs_assert(r != OGS_ERROR);
}

void sbc_handle_stop_warning_request(sbc_pws_data_t *sbc_pws)
{
    ogs_pkbuf_t *ngapbuf = NULL;
    int r;

    /* Build NGAP Kill request message */
    ngapbuf = ngap_build_kill_request(sbc_pws);
    if (!ngapbuf) {
        ogs_error("ngap_build_kill_request() failed");
        return;
    }

    /* Send to AMF */
    r = pwsiws_ngap_send_to_amf(ngapbuf);
    ogs_expect(r == OGS_OK);
    ogs_assert(r != OGS_ERROR);
} 