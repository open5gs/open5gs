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

#include "pcrf-context.h"
#include "pcrf-fd-path.h"

static pcrf_diam_stats_t prev_st;

static void pcrf_diam_stats_update_cb(const ogs_diam_stats_t *stats, const void *priv_stats)
{
    const pcrf_diam_stats_t *st = (const pcrf_diam_stats_t *)priv_stats;

    ogs_debug("%s(): Update PCRF diameter metrics", __func__);

    #define METRIC_ADD(metric_name, field) \
        { \
        int diff = st->field - prev_st.field; \
        if (diff > 0) pcrf_metrics_inst_global_add(metric_name, diff); \
        }

    METRIC_ADD(PCRF_METR_GLOB_CTR_Gx_RX_UNKNOWN,     gx.rx_unknown);
    METRIC_ADD(PCRF_METR_GLOB_CTR_Gx_RX_CCR,         gx.rx_ccr);
    METRIC_ADD(PCRF_METR_GLOB_CTR_Gx_RX_CCR_ERROR,   gx.rx_ccr_error);
    METRIC_ADD(PCRF_METR_GLOB_CTR_Gx_RX_RAA,         gx.rx_raa);
    METRIC_ADD(PCRF_METR_GLOB_CTR_Gx_TX_CCA,         gx.tx_cca);
    METRIC_ADD(PCRF_METR_GLOB_CTR_Gx_TX_RAR,         gx.tx_rar);
    METRIC_ADD(PCRF_METR_GLOB_CTR_Gx_TX_RAR_ERROR,   gx.tx_rar_error);

    METRIC_ADD(PCRF_METR_GLOB_CTR_Rx_RX_UNKNOWN,     rx.rx_unknown);
    METRIC_ADD(PCRF_METR_GLOB_CTR_Rx_RX_AAR,         rx.rx_aar);
    METRIC_ADD(PCRF_METR_GLOB_CTR_Rx_RX_AAR_ERROR,   rx.rx_aar_error);
    METRIC_ADD(PCRF_METR_GLOB_CTR_Rx_RX_ASA,         rx.rx_asa);
    METRIC_ADD(PCRF_METR_GLOB_CTR_Rx_RX_STR,         rx.rx_str);
    METRIC_ADD(PCRF_METR_GLOB_CTR_Rx_RX_STR_ERROR,   rx.rx_str_error);
    METRIC_ADD(PCRF_METR_GLOB_CTR_Rx_TX_AAA,         rx.tx_aaa);
    METRIC_ADD(PCRF_METR_GLOB_CTR_Rx_TX_SAR,         rx.tx_sar);
    METRIC_ADD(PCRF_METR_GLOB_CTR_Rx_TX_STA,         rx.tx_sta);

    memcpy(&prev_st, st, sizeof(*st));
}

int pcrf_fd_init(void)
{
    int rv;

    rv = ogs_diam_init(FD_MODE_CLIENT|FD_MODE_SERVER,
                pcrf_self()->diam_conf_path, pcrf_self()->diam_config);
    ogs_assert(rv == 0);

    rv = pcrf_gx_init();
    ogs_assert(rv == OGS_OK);
    rv = pcrf_rx_init();
    ogs_assert(rv == OGS_OK);

    ogs_diam_stats_update_cb_register(pcrf_diam_stats_update_cb);

    rv = ogs_diam_start();
    ogs_assert(rv == 0);

    return OGS_OK;
}

void pcrf_fd_final(void)
{
    pcrf_gx_final();
    pcrf_rx_final();

    ogs_diam_final();
}
