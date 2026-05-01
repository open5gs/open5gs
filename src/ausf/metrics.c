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
 * Performance measurements for Authentication Server Function (AUSF):
 *   - fivegs_ausffunction_auth_authreq   (initiated authentication requests)
 *   - fivegs_ausffunction_auth_authsucc  (successful authentications)
 *   - fivegs_ausffunction_auth_authfail  (failed authentications)
 */

#include "ogs-app.h"
#include "context.h"

#include "metrics.h"

typedef struct ausf_metrics_spec_def_s {
    unsigned int    type;
    const char     *name;
    const char     *description;
    int             initial_val;
    unsigned int    num_labels;
    const char    **labels;
} ausf_metrics_spec_def_t;

/* Generic helpers (mirror of AMF pattern) */
static int ausf_metrics_init_inst(
        ogs_metrics_inst_t **inst, ogs_metrics_spec_t **specs,
        unsigned int len,
        unsigned int num_labels, const char **labels)
{
    unsigned int i;
    for (i = 0; i < len; i++)
        inst[i] = ogs_metrics_inst_new(specs[i], num_labels, labels);
    return OGS_OK;
}

static int ausf_metrics_free_inst(
        ogs_metrics_inst_t **inst, unsigned int len)
{
    unsigned int i;
    for (i = 0; i < len; i++)
        ogs_metrics_inst_free(inst[i]);
    memset(inst, 0, sizeof(inst[0]) * len);
    return OGS_OK;
}

static int ausf_metrics_init_spec(
        ogs_metrics_context_t *ctx,
        ogs_metrics_spec_t **dst,
        ausf_metrics_spec_def_t *src, unsigned int len)
{
    unsigned int i;
    for (i = 0; i < len; i++) {
        dst[i] = ogs_metrics_spec_new(ctx, src[i].type,
                src[i].name, src[i].description,
                src[i].initial_val,
                src[i].num_labels, src[i].labels,
                NULL);
    }
    return OGS_OK;
}

/* GLOBAL */
ogs_metrics_spec_t *ausf_metrics_spec_global[_AUSF_METR_GLOB_MAX];
ogs_metrics_inst_t *ausf_metrics_inst_global[_AUSF_METR_GLOB_MAX];

/*
 * TS 28.552 §5.21.3 Table 5.21.3-1
 * Authentication-related performance counters for AUSF.
 */
ausf_metrics_spec_def_t ausf_metrics_spec_def_global[_AUSF_METR_GLOB_MAX] = {

[AUSF_METR_GLOB_CTR_AUTH_REQ] = {
    .type = OGS_METRICS_METRIC_TYPE_COUNTER,
    /* TS 28.552 §5.21.3 — 5GS.Auth.AuthenticationsInitiated */
    .name = "fivegs_ausffunction_auth_authreq",
    .description =
        "Number of authentication requests received by the AUSF",
},

[AUSF_METR_GLOB_CTR_AUTH_SUCC] = {
    .type = OGS_METRICS_METRIC_TYPE_COUNTER,
    /* TS 28.552 §5.21.3 — 5GS.Auth.AuthenticationsSucceeded */
    .name = "fivegs_ausffunction_auth_authsucc",
    .description =
        "Number of successful authentication confirmations at the AUSF",
},

[AUSF_METR_GLOB_CTR_AUTH_FAIL] = {
    .type = OGS_METRICS_METRIC_TYPE_COUNTER,
    /* TS 28.552 §5.21.3 — 5GS.Auth.AuthenticationsFailed */
    .name = "fivegs_ausffunction_auth_authfail",
    .description =
        "Number of failed authentication confirmations at the AUSF",
},

}; /* ausf_metrics_spec_def_global */

int ausf_metrics_init_inst_global(void)
{
    return ausf_metrics_init_inst(
            ausf_metrics_inst_global,
            ausf_metrics_spec_global,
            _AUSF_METR_GLOB_MAX, 0, NULL);
}

int ausf_metrics_free_inst_global(void)
{
    return ausf_metrics_free_inst(
            ausf_metrics_inst_global, _AUSF_METR_GLOB_MAX);
}

void ausf_metrics_init(void)
{
    ogs_metrics_context_t *ctx = ogs_metrics_self();
    ogs_metrics_context_init();

    ausf_metrics_init_spec(ctx,
            ausf_metrics_spec_global,
            ausf_metrics_spec_def_global,
            _AUSF_METR_GLOB_MAX);

    ausf_metrics_init_inst_global();
}

void ausf_metrics_final(void)
{
    ogs_metrics_context_final();
}
