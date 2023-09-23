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

#include "event.h"
#include "context.h"

/******************************************************************
 * Functionality: Create a new NRF event with a specified ID.
 * This function is used to create a new NRF event with a specified event ID.
 * Events are typically used to represent actions, signals, or messages that trigger specific behaviors or processing within the NRF system.
 * 
 * Input: id (Event ID)
 * Input Type: int (Integer representing the event ID)
 * Output: Pointer to the new NRF event
 * Output Type: nrf_event_t* (Pointer to the NRF event structure)
 ********************************************************************/


nrf_event_t *nrf_event_new(int id)
{
    nrf_event_t *e = NULL;
    // Allocate memory for the NRF event based on its ID and size.
    e = ogs_event_size(id, sizeof(nrf_event_t));
    ogs_assert(e);
    // Set the event ID in the NRF event structure.
    e->h.id = id;

    return e;
}


/******************************************************************
 * Functionality: Get the name associated with an NRF event.
 * Input: Pointer to an NRF event (e)
 * Input Type: nrf_event_t* (Pointer to an NRF event structure)
 * Output: Event name (const char*)
 * Output Type: const char* (Pointer to a constant character string)
 ********************************************************************/


const char *nrf_event_get_name(nrf_event_t *e)
{
    if (e == NULL)
        return OGS_FSM_NAME_INIT_SIG;

    switch (e->h.id) {
    case OGS_FSM_ENTRY_SIG: 
        return OGS_FSM_NAME_ENTRY_SIG;
    case OGS_FSM_EXIT_SIG: 
        return OGS_FSM_NAME_EXIT_SIG;

    case OGS_EVENT_SBI_SERVER:
        return OGS_EVENT_NAME_SBI_SERVER;
    case OGS_EVENT_SBI_CLIENT:
        return OGS_EVENT_NAME_SBI_CLIENT;
    case OGS_EVENT_SBI_TIMER:s
        return OGS_EVENT_NAME_SBI_TIMER;

    default: 
       break;
    }

    return "UNKNOWN_EVENT";
}
