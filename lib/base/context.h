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

#ifndef __BASE_CONTEXT__
#define __BASE_CONTEXT__

#include "base.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

extern int __base_s1ap_domain;
extern int __base_nas_domain;
extern int __base_gtp_domain;
extern int __base_fd_domain;

typedef struct base_context_s {
    struct {
        struct {
            int pool;
        } node;
        struct {
            int pool;
        } xact;
    } gtp;
} base_context_t;

void base_initialize(void);
void base_finalize(void);
base_context_t *base_self(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __BASE_CONTEXT_H__ */
