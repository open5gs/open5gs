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

#include "mme-context.h"
#include "s1ap-path.h"
#include "s1ap-build.h"
#include "sbc-handler.h"

void sbc_handle_write_replace_warning_request(sbc_pws_data_t *sbc_pws)
{
    ogs_pkbuf_t *s1apbuf = NULL;
    mme_enb_t *enb = NULL;
    int i, j, flag;

    /* Find enB with matched TAI */
    ogs_list_for_each(&mme_self()->enb_list, enb) {
        flag = 0;
        if (sbc_pws->no_of_tai > 0) {
            for (i = 0, flag = 0; i < enb->num_of_supported_ta_list; i++) {
                for (j = 0; j < sbc_pws->no_of_tai; j++) {
                    if (!memcmp(&enb->supported_ta_list[i],
                                &sbc_pws->tai[j], sizeof(ogs_eps_tai_t)))
                        flag = 1;

                    if (flag) break;
                }
                if (flag) break;
            }
        } else
            flag = 1;

        if (flag) {
            /* Build S1AP Write Replace Warning Request message */
            s1apbuf = s1ap_build_write_replace_warning_request(sbc_pws);
            ogs_expect_or_return(s1apbuf);

            /* Send to enb */
            ogs_expect(s1ap_send_to_enb(
                    enb, s1apbuf, S1AP_NON_UE_SIGNALLING) == OGS_OK);
        }
    }
}

void sbc_handle_stop_warning_request(sbc_pws_data_t *sbc_pws)
{
    ogs_pkbuf_t *s1apbuf = NULL;
    mme_enb_t *enb = NULL;
    int i, j, flag;

    /* Find enB with matched TAI */
    ogs_list_for_each(&mme_self()->enb_list, enb) {
        flag = 0;
        if (sbc_pws->no_of_tai > 0) {
            for (i = 0, flag = 0; i < enb->num_of_supported_ta_list; i++) {
                for (j = 0; j < sbc_pws->no_of_tai; j++) {
                    if (!memcmp(&enb->supported_ta_list[i],
                                &sbc_pws->tai[j], sizeof(ogs_eps_tai_t)))
                        flag = 1;

                    if (flag) break;
                }
                if (flag) break;
            }
        } else
            flag = 1;

        if (flag) {
            /* Build S1AP Kill request message */
            s1apbuf = s1ap_build_kill_request(sbc_pws);
            ogs_expect_or_return(s1apbuf);

            /* Send to enb */
            ogs_expect(s1ap_send_to_enb(
                    enb, s1apbuf, S1AP_NON_UE_SIGNALLING) == OGS_OK);
        }
    }
}
