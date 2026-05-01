/*
 * Copyright (C) 2019-2024 by Sukchan Lee <acetcom@gmail.com>
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

/*
 * 3GPP TS 28.552 Section 5.21.3
 * Performance measurements for Authentication Server Function (AUSF)
 *
 * Metrics implemented:
 *   fivegs_ausffunction_auth_authreq   — initiated auth requests
 *   fivegs_ausffunction_auth_authsucc  — successful authentications
 *   fivegs_ausffunction_auth_authfail  — failed authentications
 */

#ifndef AUSF_METRICS_H
#define AUSF_METRICS_H

#include "ogs-metrics.h"

#ifdef __cplusplus
extern "C" {
#endif

/* TS 28.552 §5.21.3 — global AUSF counters */
typedef enum ausf_metric_type_global_s {
    AUSF_METR_GLOB_CTR_AUTH_REQ = 0,  /* auth requests initiated   */
    AUSF_METR_GLOB_CTR_AUTH_SUCC,     /* auth confirmations passed */
    AUSF_METR_GLOB_CTR_AUTH_FAIL,     /* auth confirmations failed */
    _AUSF_METR_GLOB_MAX,
} ausf_metric_type_global_t;

extern ogs_metrics_inst_t *ausf_metrics_inst_global[_AUSF_METR_GLOB_MAX];

int ausf_metrics_init_inst_global(void);
int ausf_metrics_free_inst_global(void);

static inline void ausf_metrics_inst_global_inc(
        ausf_metric_type_global_t t)
{
    ogs_metrics_inst_inc(ausf_metrics_inst_global[t]);
}

void ausf_metrics_init(void);
void ausf_metrics_final(void);

#ifdef __cplusplus
}
#endif

#endif /* AUSF_METRICS_H */
