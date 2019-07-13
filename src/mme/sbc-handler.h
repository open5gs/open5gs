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

#ifndef SBC_HANDLER_H
#define SBC_HANDLER_H

#include "sbc-message.h"

/* SBc-AP handles */

#ifdef __cplusplus
extern "C" {
#endif

void sbc_handle_write_replace_warning_request(sbc_pws_data_t *sbc_pws);
void sbc_handle_stop_warning_request(sbc_pws_data_t *sbc_pws);

#ifdef __cplusplus
}
#endif

#endif /* SBC_HANDLER_H */
