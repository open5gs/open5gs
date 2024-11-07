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

#ifndef OGS_DIAMETER_COMMON_H
#define OGS_DIAMETER_COMMON_H

#include "freeDiameter/freeDiameter-host.h"

#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstrict-prototypes"
#endif

#if defined(__FreeBSD__)
#define HAVE_NTOHLL 1
#endif

#include "freeDiameter/libfdcore.h"

#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

#include "proto/ogs-proto.h"

#define OGS_DIAMETER_INSIDE

#include "diameter/common/message.h"
#include "diameter/common/logger.h"
#include "diameter/common/base.h"
#include "diameter/common/stats.h"

#undef OGS_DIAMETER_INSIDE

#ifdef __cplusplus
extern "C" {
#endif

extern int __ogs_diam_domain;

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __ogs_diam_domain

#ifdef __cplusplus
}
#endif

#endif /* OGS_DIAMETER_COMMON_H */
