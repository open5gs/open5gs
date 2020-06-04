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

#ifndef OGS_DBI_H
#define OGS_DBI_H

#include "ogs-core.h"
#include "ogs-crypt.h"

#define OGS_DBI_INSIDE

#include "dbi/ogs-mongoc.h"
#include "dbi/subscription.h"

#undef OGS_DBI_INSIDE

#ifdef __cplusplus
extern "C" {
#endif

extern int __ogs_dbi_domain;

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __ogs_dbi_domain

#ifdef __cplusplus
}
#endif

#endif /* OGS_DBI_H */
