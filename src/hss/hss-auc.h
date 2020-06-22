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

#ifndef HSS_KDF_H
#define HSS_KDF_H

#include "ogs-core.h"

#ifdef __cplusplus
extern "C" {
#endif

void hss_auc_kasme(const uint8_t *ck, const uint8_t *ik, 
        const uint8_t plmn_id[3], const uint8_t *sqn,  const uint8_t *ak,
        uint8_t *kasme);

#ifdef __cplusplus
}
#endif

#endif /* HSS_KDF_H */
