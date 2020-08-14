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

#ifndef BUILD_MME_METRICS_H
#define BUILD_MME_METRICS_H

#include "prom.h"

prom_gauge_t *mme_up_gauge;
prom_gauge_t *mme_ue_gauge;
prom_gauge_t *mme_enb_gauge;
prom_gauge_t *mme_sessions_gauge;
prom_counter_t *mme_sessions_counter;
prom_counter_t *mme_messages_counter;

void mme_metrics_initialize(void);

#endif //BUILD_MME_METRICS_H
