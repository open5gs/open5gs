/*
 * Copyright (C) 2022 by sysmocom - s.f.m.c. GmbH <info@sysmocom.de>
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

#ifndef OGS_METRICS_H
#define OGS_METRICS_H

/* MUST come first to satisfy core headers like ogs-list.h */
#include "core/ogs-core.h"

/* App layer (logging domain, etc.) */
#include "app/ogs-app.h"

/* Expose internal metrics structures to metrics library users */
#define OGS_METRICS_INSIDE
#include "metrics/context.h"
#undef OGS_METRICS_INSIDE


#ifdef __cplusplus
extern "C" {
#endif

extern size_t (*ogs_metrics_pdu_info_dumper)(char *buf, size_t buflen);
void ogs_metrics_register_pdu_info(size_t (*fn)(char *buf, size_t buflen));

extern size_t (*ogs_metrics_ue_info_dumper)(char *buf, size_t buflen);
void ogs_metrics_register_ue_info(size_t (*fn)(char *buf, size_t buflen));

extern size_t (*ogs_metrics_gnb_info_dumper)(char *buf, size_t buflen);
void ogs_metrics_register_gnb_info(size_t (*fn)(char *buf, size_t buflen));

extern size_t (*ogs_metrics_enb_info_dumper)(char *buf, size_t buflen);
void ogs_metrics_register_enb_info(size_t (*fn)(char *buf, size_t buflen));


#ifdef __cplusplus
}
#endif

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __ogs_metrics_domain

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* OGS_METRICS_H */

