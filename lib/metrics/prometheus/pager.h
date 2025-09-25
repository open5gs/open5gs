/*
 * Copyright (C) 2025 by Juraj Elias <juraj.elias@gmail.com>
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

#ifndef OGS_METRICS_PROM_PAGER_H
#define OGS_METRICS_PROM_PAGER_H

#include <stddef.h>

/* NF-specific optional pager setter signatures.
 * NFs may assign these at init time. If left NULL, paging is ignored for that NF.
 */

typedef void (*ogs_metrics_pager_fn)(size_t page, size_t page_size);

/* Connected PDUs (SMF) */
extern ogs_metrics_pager_fn ogs_metrics_pdu_info_set_pager;
extern ogs_metrics_pager_fn ogs_metrics_ue_info_set_pager;
extern ogs_metrics_pager_fn ogs_metrics_gnb_info_set_pager;
extern ogs_metrics_pager_fn ogs_metrics_enb_info_set_pager;

#endif /* OGS_METRICS_PROM_PAGER_H */

