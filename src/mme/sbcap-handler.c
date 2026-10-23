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

#include "mme-event.h"
#include "mme-timer.h"

#include "sbcap-path.h"
#include "mme-fd-path.h"

//#include "s1ap-build.h"
#include "sbc-handler.h"

#include "stdio.h"

#include "sbcap-handler.h"

#include "mme-path.h"
#include "mme-sm.h"



void sbcap_handle_write_replace_warning_response(
        mme_sbcap_t *sbc, ogs_sbcap_message_t *message)
{
    char buf[OGS_ADDRSTRLEN];

    SBCAP_SuccessfulOutcome_t *successfulOutcome = NULL;
    SBCAP_Write_Replace_Warning_Response_t *SbcapWriteReplaceWarningResponse = NULL;

    ogs_assert(sbc);
    ogs_assert(sbc->sctp.sock);

    ogs_assert(message);
    successfulOutcome = message->choice.successfulOutcome;
    ogs_assert(successfulOutcome);
    SbcapWriteReplaceWarningResponse =
        &successfulOutcome->value.choice.Write_Replace_Warning_Response;
    ogs_assert(SbcapWriteReplaceWarningResponse);

    ogs_debug("SbcapWriteReplaceWarningResponse");

    ogs_debug("    IP[%s]",
            OGS_ADDR(sbc->sctp.addr, buf));

}

void sbcap_handle_write_replace_warning_request(
        mme_sbcap_t *sbc, ogs_sbcap_message_t *message)
{
    char buf[OGS_ADDRSTRLEN];
    int i;

    SBCAP_InitiatingMessage_t *initiatingMessage = NULL;
    SBCAP_Write_Replace_Warning_Request_t *SbcapWriteReplaceWarningRequest = NULL;
    SBCAP_Write_Replace_Warning_Request_IEs_t *ie = NULL;
    SBCAP_Message_Identifier_t *message_identifier = NULL;
    SBCAP_Serial_Number_t *serial_number                      = NULL;
    SBCAP_Repetition_Period_t *repetition_period    = NULL;
    SBCAP_NumberOfBroadcasts_t *broadcast_requested    = NULL;
    SBCAP_Data_Coding_Scheme_t *data_coding_scheme    = NULL;
    SBCAP_Warning_Message_Content_t *warning_message_content    = NULL;
    //sbc_handle_write_replace_warning_request(sbc_pws_data_t *sbc_pws)
    sbc_pws_data_t *sbc_pws = NULL; 
    //ogs_debug("Test3");
    //memset(sbc_pws, 0, sizeof(sbc_pws_data_t)); 
    sbc_pws = (sbc_pws_data_t*)malloc(sizeof(sbc_pws_data_t)); 
    //ogs_debug("Test1");
    ogs_assert(sbc_pws);
    //ogs_debug("Test2");


    ogs_assert(sbc);
    ogs_assert(sbc->sctp.sock);

    ogs_assert(message);
    initiatingMessage = message->choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    SbcapWriteReplaceWarningRequest =
        &initiatingMessage->value.choice.Write_Replace_Warning_Request;
    ogs_assert(SbcapWriteReplaceWarningRequest);

    ogs_debug("SbcapWriteReplaceWarningRequest");

    for (i=0; i < SbcapWriteReplaceWarningRequest->protocolIEs.list.count; i++ ){
            ie = SbcapWriteReplaceWarningRequest->protocolIEs.list.array[i];
            switch(ie->id){
                case SBCAP_ProtocolIE_ID_id_Message_Identifier :
                   message_identifier = &ie->value.choice.Message_Identifier;
                   int message = 0;
                   memcpy(&message,message_identifier->buf, sizeof(uint16_t));  //Obrce redosled Bytova

                   ogs_debug("message_identifier: %ld",message_identifier->size);
                   ogs_info("message_identifier: %d",message);
                   sbc_pws ->message_id = message;
                   ogs_log_hexdump(OGS_LOG_INFO,message_identifier->buf, sizeof(message_identifier->buf));

                break;
                case SBCAP_ProtocolIE_ID_id_Serial_Number :
                   serial_number = &ie->value.choice.Serial_Number;
                   ogs_debug("serial_number: %s",serial_number->buf);
                   uint16_t serial_number_i = 0;
                   memcpy(&serial_number_i,serial_number->buf, sizeof(uint16_t));  //Obrce redosled Bytova
                   sbc_pws ->serial_number = serial_number_i;

                break;
                case SBCAP_ProtocolIE_ID_id_Repetition_Period :
                   repetition_period = &ie->value.choice.Repetition_Period;
                   ogs_info("repetition_period: %ld",*repetition_period);
                   sbc_pws -> repetition_period = *repetition_period;

                break;
                case SBCAP_ProtocolIE_ID_id_Number_Of_Broadcast_Requested :
                   broadcast_requested = &ie->value.choice.Number_of_Broadcasts_Requested;
                   ogs_info("broadcast_requested: %ld",*broadcast_requested);
                   sbc_pws -> number_of_broadcast = *broadcast_requested;
                break;
                case SBCAP_ProtocolIE_ID_id_Data_Coding_Scheme :
                   data_coding_scheme = &ie->value.choice.Data_Coding_Scheme;
                   ogs_debug("data_coding_scheme: %s",data_coding_scheme->buf);
                   sbc_pws->data_coding_scheme = 0xFF & 0xFF;
                break;
               case SBCAP_ProtocolIE_ID_id_Warning_Message_Content :
                   warning_message_content = &ie->value.choice.Warning_Message_Content;
                   ogs_info("warning_message_content: %s",warning_message_content->buf);
                   ogs_log_hexdump(OGS_LOG_INFO,warning_message_content->buf, sizeof(warning_message_content->buf));
                   uint8_t warn_message[1024];
                   memcpy(&warn_message,warning_message_content->buf, warning_message_content->size);
                   memcpy(sbc_pws->message_contents,warning_message_content->buf, warning_message_content->size);                   
                   sbc_pws -> message_length = warning_message_content->size;
                   //sbc_pws -> no_of_tai = 1;
                   //sbc_pws -> tai[0] = 0x0007;
                break;
            }
        
    }

    mme_enb_t *enb = NULL;
    
    ogs_list_for_each(&mme_self()->enb_list, enb) {
      for (i = 0; i < enb->num_of_supported_ta_list; i++) {
         ogs_debug("eNB Tac =  %d",enb->supported_ta_list[i].tac);
         sbc_pws -> tai[i] = enb->supported_ta_list[i];
         sbc_pws -> no_of_tai = i + 1;
         ogs_debug("SBcAP Tac =  %d",sbc_pws -> tai[i].tac);
         ogs_debug("SBcAP no_of_tai =  %d",sbc_pws -> no_of_tai);
         if(i>15){
            break;
         }
      }
    }
    sbc_handle_write_replace_warning_request(sbc_pws);
    sbc_handle_stop_warning_request(sbc_pws);
    ogs_debug("    IP[%s]",
            OGS_ADDR(sbc->sctp.addr, buf));

}

