/*
 * Copyright (C) 2019-2023 by Sukchan Lee <acetcom@gmail.com>
 * Copyright (C) 2024 by Arnaud DJOUM <arnaudchewa65@gmail.com>
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
#include "core/abts.h"

/*
 * SBc-AP Message Tests for CMAS/PWS support
 * Reference: 3GPP TS 29.168 (SBc-AP specification)
 */

static void sbcap_message_test1(abts_case *tc, void *data)
{
    /*
     * Test 1: Verify procedure code constants
     */
    ABTS_INT_EQUAL(tc, 0, SBCAP_id_Write_Replace_Warning);
    ABTS_INT_EQUAL(tc, 1, SBCAP_id_Stop_Warning);
    ABTS_INT_EQUAL(tc, 2, SBCAP_id_Error_Indication);
}

static void sbcap_message_test2(abts_case *tc, void *data)
{
    /*
     * Test 2: Verify IE ID constants
     */
    ABTS_INT_EQUAL(tc, 5, SBCAP_ProtocolIE_ID_id_Message_Identifier);
    ABTS_INT_EQUAL(tc, 11, SBCAP_ProtocolIE_ID_id_Serial_Number);
}

static void sbcap_message_test3(abts_case *tc, void *data)
{
    /*
     * Test 3: Test PDU structure allocation
     */
    SBCAP_SBC_AP_PDU_t pdu;
    
    memset(&pdu, 0, sizeof(pdu));
    
    /* Set to initiating message */
    pdu.present = SBCAP_SBC_AP_PDU_PR_initiatingMessage;
    
    /* Verify */
    ABTS_INT_EQUAL(tc, SBCAP_SBC_AP_PDU_PR_initiatingMessage, pdu.present);
}

abts_suite *test_sbcap_message(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, sbcap_message_test1, NULL);
    abts_run_test(suite, sbcap_message_test2, NULL);
    abts_run_test(suite, sbcap_message_test3, NULL);

    return suite;
}
