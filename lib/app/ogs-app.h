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

#ifndef OGS_APP_H
#define OGS_APP_H

#include "crypt/ogs-crypt.h"

#define OGS_APP_INSIDE

extern int __ogs_app_domain;

#include "app/ogs-yaml.h"
#include "app/ogs-context.h"
#include "app/ogs-config.h"
#include "app/ogs-init.h"

#undef OGS_APP_INSIDE

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __ogs_app_domain

#ifdef __cplusplus
extern "C" {
#endif

int app_initialize(const char *const argv[]);
void app_terminate(void);

int mme_initialize(void);
void mme_terminate(void);

int hss_initialize(void);
void hss_terminate(void);

int sgw_initialize(void);
void sgw_terminate(void);

int sgwc_initialize(void);
void sgwc_terminate(void);

int sgwu_initialize(void);
void sgwu_terminate(void);

int pgw_initialize(void);
void pgw_terminate(void);

int pcrf_initialize(void);
void pcrf_terminate(void);

int nrf_initialize(void);
void nrf_terminate(void);

int udr_initialize(void);
void udr_terminate(void);

int udm_initialize(void);
void udm_terminate(void);

int ausf_initialize(void);
void ausf_terminate(void);

int pcf_initialize(void);
void pcf_terminate(void);

int nssf_initialize(void);
void nssf_terminate(void);

int bsf_initialize(void);
void bsf_terminate(void);

int upf_initialize(void);
void upf_terminate(void);

int smf_initialize(void);
void smf_terminate(void);

int amf_initialize(void);
void amf_terminate(void);

int scp_initialize(void);
void scp_terminate(void);

int sepp_initialize(void);
void sepp_terminate(void);

#ifdef __cplusplus
}
#endif

#endif /* OGS_APP_H */
