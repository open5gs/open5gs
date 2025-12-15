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

//#include "ogs-gtp.h"
//#include "ogs-nas-eps.h"

//#include "s1ap-build.h"
#include "sbcap-handler.h"
#include "sbcap-path.h"

#include "mme-event.h"
#include "mme-timer.h"
#include "mme-sm.h"

void sbcap_state_initial(ogs_fsm_t *s, mme_event_t *e)
{
    ogs_assert(s);

    mme_sm_debug(e);

    OGS_FSM_TRAN(s, &sbcap_state_operational);
}

void sbcap_state_final(ogs_fsm_t *s, mme_event_t *e)
{
    ogs_assert(s);

    mme_sm_debug(e);
}

void sbcap_state_operational(ogs_fsm_t *s, mme_event_t *e)
{
    mme_sbcap_t *sbc = NULL;
    //ogs_pkbuf_t *pkbuf = NULL;

    SBCAP_SBC_AP_PDU_t *pdu = NULL;
    SBCAP_SuccessfulOutcome_t *successfulOutcome = NULL;
    SBCAP_InitiatingMessage_t *initiatingMessage = NULL;


    ogs_assert(s);
    ogs_assert(e);

    mme_sm_debug(e);

    sbc = e->sbc;
    ogs_assert(sbc);

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        break;
    case OGS_FSM_EXIT_SIG:
        break;
    case MME_EVENT_SBCAP_MESSAGE:
        ///ogs_info(" %s", mme_event_get_name(e));
        pdu = e->sbcap_message;
        ogs_assert(pdu);
        switch (pdu->present) {

        case SBCAP_SBC_AP_PDU_PR_successfulOutcome :
            successfulOutcome = pdu->choice.successfulOutcome;
            ogs_assert(successfulOutcome);

            switch (successfulOutcome->procedureCode) {
            case SBCAP_id_Write_Replace_Warning:
                sbcap_handle_write_replace_warning_response(sbc,pdu);
                ogs_info("Write_Replace_Warning_Response");
                break;
            default:
                ogs_error("Not implemented(choice:%d, proc:%d)",
                        pdu->present, (int)successfulOutcome->procedureCode);
                break;
            }
            break;
        case SBCAP_SBC_AP_PDU_PR_initiatingMessage :
            initiatingMessage = pdu->choice.initiatingMessage;
            ogs_assert(initiatingMessage);

            switch (initiatingMessage->procedureCode) {
            case SBCAP_id_Write_Replace_Warning:
                sbcap_handle_write_replace_warning_request(sbc,pdu);
                ogs_info("Write_Replace_Warning_Request");
                break;

            case SBCAP_id_Stop_Warning:
                sbcap_handle_stop_warning_request(sbc,pdu);
                ogs_info("Stop_Warning_Request");
                break;
            default:
                ogs_error("Not implemented(choice:%d, proc:%d)",
                        pdu->present, (int)initiatingMessage->procedureCode);
                break;
            }
            break;

        default:
            ogs_error("Not implemented(choice:%d)", pdu->present);
            break;
        }

        break;
        //ogs_info("MME_EVENT_SBCAP_MESSAGE %s", mme_event_get_name(e));

    default:
        ogs_error("Unknown event %s", mme_event_get_name(e));
        break;
    }
}

void sbcap_state_exception(ogs_fsm_t *s, mme_event_t *e)
{
    ogs_assert(s);
    ogs_assert(e);

    mme_sm_debug(e);

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        break;
    case OGS_FSM_EXIT_SIG:
        break;
    default:
        ogs_error("Unknown event %s", mme_event_get_name(e));
        break;
    }
}

