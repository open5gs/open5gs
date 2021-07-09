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

#if !defined(OGS_DIAMETER_INSIDE) && !defined(OGS_DIAMETER_COMPILATION)
#error "This header cannot be included directly."
#endif

#ifndef OGS_DIAM_S6B_MESSAGE_H
#define OGS_DIAM_S6B_MESSAGE_H

#ifdef __cplusplus
extern "C" {
#endif

#define OGS_DIAM_S6B_APPLICATION_ID 16777272

extern struct dict_object *ogs_diam_s6b_application;

extern struct dict_object *ogs_diam_s6b_mip6_feature_vector;

int ogs_diam_s6b_init(void);

#ifdef __cplusplus
}
#endif

#endif /* OGS_DIAM_OGS_DIAM_S6B_MESSAGE_H */
