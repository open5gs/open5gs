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

#include "ogs-nas-eps.h"

int ogs_nas_tai_list_build(ogs_nas_tracking_area_identity_list_t *target,
        ogs_eps_tai0_list_t *source0, ogs_eps_tai2_list_t *source2)
{
    int i = 0, j = 0, size = 0;

    ogs_eps_tai0_list_t target0;
    ogs_eps_tai2_list_t target2;
    ogs_nas_plmn_id_t ogs_nas_plmn_id;

    ogs_assert(target);
    ogs_assert(source0);
    ogs_assert(source2);

    memset(target, 0, sizeof(ogs_nas_tracking_area_identity_list_t));
    memset(&target0, 0, sizeof(ogs_eps_tai0_list_t));
    memset(&target2, 0, sizeof(ogs_eps_tai2_list_t));

    for (i = 0; source0->tai[i].num; i++) {
        ogs_assert(source0->tai[i].type == OGS_TAI0_TYPE);
        target0.tai[i].type = source0->tai[i].type;

        /* <Spec> target->num = source->num - 1 */
        ogs_assert(source0->tai[i].num < OGS_MAX_NUM_OF_TAI);
        target0.tai[i].num = source0->tai[i].num - 1;
        memcpy(&target0.tai[i].plmn_id,
            ogs_nas_from_plmn_id(&ogs_nas_plmn_id, &source0->tai[i].plmn_id),
            OGS_PLMN_ID_LEN);

        for (j = 0; j < source0->tai[i].num; j++) {
            target0.tai[i].tac[j] = htobe16(source0->tai[i].tac[j]);
        }

        size = (1 + 3 + 2 * source0->tai[i].num);
        if ((target->length + size) > OGS_NAS_EPS_MAX_TAI_LIST_LEN) {
            ogs_error("Overflow: Ignore remained TAI LIST(length:%d, size:%d)",
                    target->length, size);
            return OGS_ERROR;
        }
        memcpy(target->buffer + target->length, &target0.tai[i], size);
        target->length += size;
    }

    if (source2->num) {
        memset(&target2, 0, sizeof(target2));

        ogs_assert(source2->type == OGS_TAI1_TYPE ||
                    source2->type == OGS_TAI2_TYPE);
        target2.type = source2->type;

        /* <Spec> target->num = source->num - 1 */
        ogs_assert(source2->num < OGS_MAX_NUM_OF_TAI);
        target2.num = source2->num - 1;

        size = (1 + (3 + 2) * source2->num);
        if ((target->length + size) > OGS_NAS_EPS_MAX_TAI_LIST_LEN) {
            ogs_error("Overflow: Ignore remained TAI LIST(length:%d, size:%d)",
                    target->length, size);
            return OGS_ERROR;
        }
        for (i = 0; i < source2->num; i++) {
            memcpy(&target2.tai[i].plmn_id,
                    ogs_nas_from_plmn_id(&ogs_nas_plmn_id,
                        &source2->tai[i].plmn_id),
                    OGS_PLMN_ID_LEN);
            target2.tai[i].tac = htobe16(source2->tai[i].tac);
        }
        memcpy(target->buffer + target->length, &target2, size);
        target->length += size;
    }

    return OGS_OK;
}
