/*
 * Copyright (C) 2019,2020 by Sukchan Lee <acetcom@gmail.com>
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

#ifndef PWSIWS_NGAP_BUILD_H
#define PWSIWS_NGAP_BUILD_H

#include "context.h"

#ifdef __cplusplus
extern "C" {
#endif

/* NGAP Warning Message Transfer Building Functions */
ogs_pkbuf_t *ngap_build_warning_message_broadcast_request_transfer(
        pwsiws_warning_t *warning);

ogs_pkbuf_t *ngap_build_warning_message_stop_request_transfer(
        pwsiws_warning_t *warning);

ogs_pkbuf_t *ngap_build_warning_message_cancel_request_transfer(
        pwsiws_warning_t *warning);

#ifdef __cplusplus
}
#endif

#endif /* PWSIWS_NGAP_BUILD_H */ 