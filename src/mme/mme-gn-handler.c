/*
 * Copyright (C) 2023 by sysmocom - s.f.m.c. GmbH <info@sysmocom.de>
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
#include "mme-sm.h"
#include "mme-context.h"
#include "mme-timer.h"

#include "mme-gtp-path.h"
#include "mme-fd-path.h"
#include "mme-path.h"

#include "mme-gn-build.h"
#include "mme-gn-handler.h"

void mme_gn_handle_echo_request(
        ogs_gtp_xact_t *xact, ogs_gtp1_echo_request_t *req)
{
    ogs_assert(xact);
    ogs_assert(req);

    ogs_debug("[Gn] Receiving Echo Request");
    /* FIXME : Implementing recovery counter correctly */
    ogs_gtp1_send_echo_response(xact, 0);
}

void mme_gn_handle_echo_response(
        ogs_gtp_xact_t *xact, ogs_gtp1_echo_response_t *req)
{
    /* Not Implemented */
}


void mme_gn_handle_ran_information_relay(
        ogs_gtp_xact_t *xact, ogs_gtp1_ran_information_relay_t *req)
{
    ogs_debug("[Gn] Receiving RAN Information Relay");
    /* Not Implemented */
}
