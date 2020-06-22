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

#ifndef AMF_PATH_H
#define AMF_PATH_H

#include "sbi-path.h"
#include "nas-path.h"
#include "ngap-path.h"

#ifdef __cplusplus
extern "C" {
#endif

void amf_send_delete_session_or_de_register(amf_ue_t *amf_ue);
void amf_send_delete_session_or_amf_ue_context_release(amf_ue_t *amf_ue);
void amf_send_delete_session_or_ran_ue_context_release(ran_ue_t *ran_ue);

#ifdef __cplusplus
}
#endif

#endif /* AMF_PATH_H */
