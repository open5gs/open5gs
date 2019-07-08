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

#include "ogs-sctp.h"

#include "mme-event.h"
#include "mme-sm.h"

#include "sgsap-types.h"
#include "sgsap-build.h"
#include "sgsap-path.h"

int sgsap_open()
{
    mme_vlr_t *vlr = NULL;

    ogs_list_for_each(&mme_self()->vlr_list, vlr) {
        mme_event_t e;
        e.vlr = vlr;

        ogs_fsm_create(&vlr->sm, sgsap_state_initial, sgsap_state_final);
        ogs_fsm_init(&vlr->sm, &e);
    }

    return OGS_OK;
}

void sgsap_close()
{
    mme_vlr_t *vlr = NULL;

    ogs_list_for_each(&mme_self()->vlr_list, vlr) {
        mme_event_t e;
        e.vlr = vlr;

        ogs_fsm_fini(&vlr->sm, &e);
        ogs_fsm_delete(&vlr->sm);
    }
}

int sgsap_send(ogs_sock_t *sock, ogs_pkbuf_t *pkbuf,
        ogs_sockaddr_t *addr, uint16_t stream_no)
{
    int sent;

    ogs_assert(sock);
    ogs_assert(pkbuf);

    sent = ogs_sctp_sendmsg(sock, pkbuf->data, pkbuf->len,
            addr, SCTP_SGSAP_PPID, stream_no);
    if (sent < 0 || sent != pkbuf->len) {
        ogs_error("ogs_sctp_sendmsg error (%d:%s)", errno, strerror(errno));
        return OGS_ERROR;
    }
    ogs_pkbuf_free(pkbuf);

    return OGS_OK;
}

int sgsap_send_to_vlr_with_sid(
        mme_vlr_t *vlr, ogs_pkbuf_t *pkbuf, uint16_t stream_no)
{
    char buf[OGS_ADDRSTRLEN];
    ogs_socknode_t *node = NULL;;
    ogs_sock_t *sock = NULL;;
    int rv;

    ogs_assert(vlr);
    ogs_assert(pkbuf);
    node = vlr->node;
    ogs_assert(node);
    sock = node->sock;
    ogs_assert(sock);

    ogs_debug("    VLR-IP[%s]", OGS_ADDR(node->addr, buf));
    ogs_debug("    TAI[PLMN_ID:%06x,TAC:%d]",
                plmn_id_hexdump(&vlr->tai.nas_plmn_id), vlr->tai.tac);
    ogs_debug("    LAI[PLMN_ID:%06x,LAC:%d]",
                plmn_id_hexdump(&vlr->lai.nas_plmn_id), vlr->lai.lac);

    rv = sgsap_send(sock, pkbuf, node->addr, stream_no);
    if (rv != OGS_OK) {
        ogs_error("sgsap_send() failed");

        ogs_pkbuf_free(pkbuf);
        sgsap_event_push(MME_EVT_SGSAP_LO_CONNREFUSED,
                node->sock, node->addr, NULL, 0, 0);
    }

    return OGS_OK;
}

int sgsap_send_to_vlr(mme_ue_t *mme_ue, ogs_pkbuf_t *pkbuf)
{
    mme_vlr_t *vlr = NULL;
    ogs_assert(mme_ue);
    ogs_assert(pkbuf);
    vlr = mme_ue->vlr;
    ogs_assert(vlr);

    return sgsap_send_to_vlr_with_sid(vlr, pkbuf, mme_ue->vlr_ostream_id);
}

int sgsap_send_location_update_request(mme_ue_t *mme_ue)
{
    int rv;
    ogs_pkbuf_t *pkbuf = NULL;
    ogs_assert(mme_ue);

    ogs_debug("[SGSAP] LOCATION-UPDATE-REQUEST");
    ogs_debug("    IMSI[%s]", mme_ue->imsi_bcd);

    pkbuf = sgsap_build_location_update_request(mme_ue);
    rv = sgsap_send_to_vlr(mme_ue, pkbuf);
    ogs_assert(rv == OGS_OK);

    return OGS_OK;
}

int sgsap_send_tmsi_reallocation_complete(mme_ue_t *mme_ue)
{
    int rv;
    ogs_pkbuf_t *pkbuf = NULL;
    ogs_assert(mme_ue);

    ogs_debug("[SGSAP] TMSI-REALLOCATION-COMPLETE");
    ogs_debug("    IMSI[%s]", mme_ue->imsi_bcd);

    pkbuf = sgsap_build_tmsi_reallocation_complete(mme_ue);
    rv = sgsap_send_to_vlr(mme_ue, pkbuf);
    ogs_assert(rv == OGS_OK);

    return OGS_OK;
}

int sgsap_send_detach_indication(mme_ue_t *mme_ue)
{
    int rv;
    ogs_pkbuf_t *pkbuf = NULL;
    ogs_assert(mme_ue);

    pkbuf = sgsap_build_detach_indication(mme_ue);
    rv = sgsap_send_to_vlr(mme_ue, pkbuf);
    ogs_assert(rv == OGS_OK);

    return OGS_OK;
}

int sgsap_send_mo_csfb_indication(mme_ue_t *mme_ue)
{
    int rv;
    ogs_pkbuf_t *pkbuf = NULL;
    ogs_assert(mme_ue);

    ogs_debug("[SGSAP] MO-CSFB-INDICATION");
    ogs_debug("    IMSI[%s]", mme_ue->imsi_bcd);

    pkbuf = sgsap_build_mo_csfb_indication(mme_ue);
    rv = sgsap_send_to_vlr(mme_ue, pkbuf);
    ogs_assert(rv == OGS_OK);

    return OGS_OK;
}

int sgsap_send_service_request(mme_ue_t *mme_ue)
{
    int rv;
    ogs_pkbuf_t *pkbuf = NULL;
    ogs_assert(mme_ue);

    ogs_debug("[SGSAP] MO-CSFB-INDICATION");
    ogs_debug("    IMSI[%s]", mme_ue->imsi_bcd);
    ogs_debug("    SERVICE_INDICATOR[%d]", mme_ue->service_indicator);

    pkbuf = sgsap_build_service_request(mme_ue);
    rv = sgsap_send_to_vlr(mme_ue, pkbuf);
    ogs_assert(rv == OGS_OK);

    return OGS_OK;
}

int sgsap_send_reset_ack(mme_vlr_t *vlr)
{
    int rv;
    ogs_pkbuf_t *pkbuf = NULL;
    ogs_assert(vlr);

    ogs_debug("[SGSAP] RESET-ACK");

    pkbuf = sgsap_build_reset_ack(vlr);
    rv =  sgsap_send_to_vlr_with_sid(vlr, pkbuf, 0);
    ogs_assert(rv == OGS_OK);

    return OGS_OK;
}
