/*
 * Copyright (C) 2026 by Erol Yağız Aydın <ygzaydns@gmail.com>
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

#ifndef EIR_CONTEXT_H
#define EIR_CONTEXT_H

#include "ogs-app.h"
#include "ogs-dbi.h"
#include "ogs-sbi.h"

#include "eir-sm.h"

#ifdef __cplusplus
extern "C" {
#endif

extern int __eir_log_domain;

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __eir_log_domain

typedef struct eir_context_s {
} eir_context_t;

void eir_context_init(void);
void eir_context_final(void);
eir_context_t *eir_self(void);

int eir_context_parse_config(void);

#ifdef __cplusplus
}
#endif

#endif /* EIR_CONTEXT_H */
