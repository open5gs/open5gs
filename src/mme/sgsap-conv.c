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

#include "sgsap-conv.h"

char *sgsap_to_mme_name(char *buf, size_t size,
        uint32_t mme_code, uint16_t mme_gid, plmn_id_t *plmn_id)
{
    ogs_assert(buf);
    ogs_assert(plmn_id);
    if (ogs_snprintf(buf, size,
        "mmec%02d.mmegi%04d.mme.epc.mnc%03d.mcc%03d.3gppnetwork.org",
        mme_code, mme_gid, plmn_id_mnc(plmn_id), plmn_id_mcc(plmn_id) < 0)) {
        ogs_error("Cannot make MME name");
        return NULL;
    }

    return buf;
}
