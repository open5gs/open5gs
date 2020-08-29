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

#ifndef OGS_PFCP_H
#define OGS_PFCP_H

#include "ogs-core.h"
#include "ipfw/ogs-ipfw.h"
#include "ogs-app.h"
#include "ogs-gtp.h"

#define OGS_PFCP_UDP_PORT               8805

#define OGS_MAX_NUM_OF_PDR              16
#define OGS_MAX_NUM_OF_FAR              16
#define OGS_MAX_NUM_OF_URR              2
#define OGS_MAX_NUM_OF_QER              4
#define OGS_MAX_NUM_OF_BAR              1
#define OGS_MAX_NUM_OF_GTPU_RESOURCE    4

#define OGS_PFCP_INSIDE

#include "pfcp/message.h"
#include "pfcp/types.h"
#include "pfcp/conv.h"
#include "pfcp/context.h"
#include "pfcp/build.h"
#include "pfcp/path.h"
#include "pfcp/xact.h"
#include "pfcp/handler.h"

#ifdef __cplusplus
extern "C" {
#endif

#undef OGS_PFCP_INSIDE

extern int __ogs_pfcp_domain;

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __ogs_pfcp_domain

#ifdef __cplusplus
}
#endif

#endif /* OGS_PFCP_H */
