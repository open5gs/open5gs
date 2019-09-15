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

#include "diameter/ogs-rx.h"

void ogs_diam_rx_message_free(ogs_diam_rx_message_t *rx_message)
{
    int i, j, k;

    ogs_assert(rx_message);

    for (i = 0; i < rx_message->num_of_media_component; i++) {
        ogs_diam_rx_media_component_t *media_component =
            &rx_message->media_component[i];

        for (j = 0; j < media_component->num_of_sub; j++) {
            ogs_diam_rx_media_sub_component_t *sub = &media_component->sub[j];

            for (k = 0; k < sub->num_of_flow; k++) {
                ogs_flow_t *flow = &sub->flow[k];

                if (flow->description) {
                    ogs_free(flow->description);
                }
                else
                    ogs_assert_if_reached();
            }
        }
    }
}
