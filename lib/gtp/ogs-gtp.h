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

#ifndef OGS_GTP_H
#define OGS_GTP_H

#include "ogs-core.h"
#include "ipfw/ogs-ipfw.h"
#include "ogs-app.h"
#include "ogs-nas-common.h"

#define OGS_GTPV1_U_UDP_PORT            2152
#define OGS_GTPV2_C_UDP_PORT            2123

#define OGS_GTP_INSIDE

#include "gtp/v1/message.h"
#include "gtp/v2/message.h"
#include "gtp/v1/types.h"
#include "gtp/v2/types.h"
#include "gtp/v1/conv.h"
#include "gtp/v2/conv.h"
#include "gtp/context.h"
#include "gtp/v1/build.h"
#include "gtp/v2/build.h"
#include "gtp/v1/path.h"
#include "gtp/v2/path.h"
#include "gtp/path.h"
#include "gtp/xact.h"
#include "gtp/util.h"

#ifdef __cplusplus
extern "C" {
#endif

#undef OGS_GTP_INSIDE

extern int __ogs_gtp_domain;

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __ogs_gtp_domain

#ifdef __cplusplus
}
#endif

#endif /* OGS_GTP_H */
