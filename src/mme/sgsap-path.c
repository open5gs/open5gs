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

#include "sgsap-path.h"

int sgsap_open()
{
    return OGS_OK;
}

void sgsap_close()
{
}

void sgsap_connect_timeout(void *data)
{
#if 0
    mme_vlr_t *vlr = data;
    ogs_assert(vlr);

    if (mme_ue->max_paging_retry >= MAX_NUM_OF_PAGING) {
        /* Paging failed */
        ogs_warn("[EMM] Paging to IMSI[%s] failed. Stop paging",
                mme_ue->imsi_bcd);
        if (mme_ue->last_paging_msg) {
            ogs_pkbuf_free(mme_ue->last_paging_msg);
            mme_ue->last_paging_msg = NULL;
        }
    } else {
        mme_ue->max_paging_retry++;
        s1ap_handle_paging(mme_ue);
        /* Start T3413 */
        ogs_timer_start(mme_ue->t3413, mme_self()->t3413_value);
    }
#endif
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

int sgsap_recv(ogs_sock_t *sock, ogs_pkbuf_t *pkbuf)
{
    int size;

    ogs_assert(sock);
    ogs_assert(pkbuf);

    size = ogs_sctp_recvdata(sock, pkbuf->data, MAX_SDU_LEN, NULL, NULL);
    if (size <= 0) {
        ogs_error("sgsap_recv() failed");
        return OGS_ERROR;
    }

    ogs_pkbuf_trim(pkbuf, size);
    return OGS_OK;;
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

    ogs_debug("    IP[%s] TAI[MCC:%d%d%d,MNC:%d%d%d,TAC:%d]",
        OGS_ADDR(node->addr, buf),
        vlr->tai.plmn_id.mcc1, vlr->tai.plmn_id.mcc2, vlr->tai.plmn_id.mcc3,
        vlr->tai.plmn_id.mnc1, vlr->tai.plmn_id.mnc2, vlr->tai.plmn_id.mnc3,
        vlr->tai.tac);
    ogs_debug("           LAI[MCC:%d%d%d,MNC:%d%d%d,TAC:%d]",
        vlr->lai.plmn_id.mcc1, vlr->lai.plmn_id.mcc2, vlr->lai.plmn_id.mcc3,
        vlr->lai.plmn_id.mnc1, vlr->lai.plmn_id.mnc2, vlr->lai.plmn_id.mnc3,
        vlr->lai.lac);

    rv = sgsap_send(sock, pkbuf, NULL, stream_no);
    if (rv != OGS_OK) {
        ogs_error("sgsap_send() failed");
        ogs_pkbuf_free(pkbuf);
    }

    return OGS_OK;;
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
