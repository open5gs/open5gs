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

#include "ogs-crypt.h"

#include "hss-context.h"
#include "hss-fd-path.h"

static hss_diam_stats_t prev_st;

static void hsss_diam_stats_update_cb(const ogs_diam_stats_t *stats, const void *priv_stats)
{
    const hss_diam_stats_t *st = (const hss_diam_stats_t *)priv_stats;

     ogs_debug("%s(): Update HSS diameter metrics", __func__);

    #define METRIC_ADD(metric_name, field) \
        { \
        int diff = st->field - prev_st.field; \
        if (diff > 0) hss_metrics_inst_global_add(metric_name, diff); \
        }

    METRIC_ADD(HSS_METR_GLOB_CTR_CX_RX_UNKNOWN,     cx.rx_unknown);
    METRIC_ADD(HSS_METR_GLOB_CTR_CX_RX_MAR,         cx.rx_mar);
    METRIC_ADD(HSS_METR_GLOB_CTR_CX_RX_MAR_ERROR,   cx.rx_mar_error);
    METRIC_ADD(HSS_METR_GLOB_CTR_CX_RX_SAR,         cx.rx_sar);
    METRIC_ADD(HSS_METR_GLOB_CTR_CX_RX_SAR_ERROR,   cx.rx_sar_error);
    METRIC_ADD(HSS_METR_GLOB_CTR_CX_RX_UAR,         cx.rx_uar);
    METRIC_ADD(HSS_METR_GLOB_CTR_CX_RX_UAR_ERROR,   cx.rx_uar_error);
    METRIC_ADD(HSS_METR_GLOB_CTR_CX_RX_LIR,         cx.rx_lir);
    METRIC_ADD(HSS_METR_GLOB_CTR_CX_RX_LIR_ERROR,   cx.rx_lir_error);
    METRIC_ADD(HSS_METR_GLOB_CTR_CX_TX_MAA,         cx.tx_maa);
    METRIC_ADD(HSS_METR_GLOB_CTR_CX_TX_SAA,         cx.tx_saa);
    METRIC_ADD(HSS_METR_GLOB_CTR_CX_TX_UAA,         cx.tx_uaa);
    METRIC_ADD(HSS_METR_GLOB_CTR_CX_TX_LIA,         cx.tx_lia);

    METRIC_ADD(HSS_METR_GLOB_CTR_S6a_RX_UNKNOWN,      s6a.rx_unknown);
    METRIC_ADD(HSS_METR_GLOB_CTR_S6a_RX_AIR,          s6a.rx_air);
    METRIC_ADD(HSS_METR_GLOB_CTR_S6a_RX_AIR_ERROR,    s6a.rx_air_error);
    METRIC_ADD(HSS_METR_GLOB_CTR_S6a_RX_CLA,          s6a.rx_cla);
    METRIC_ADD(HSS_METR_GLOB_CTR_S6a_RX_CLA_ERROR,    s6a.rx_cla_error);
    METRIC_ADD(HSS_METR_GLOB_CTR_S6a_RX_IDA,          s6a.rx_ida);
    METRIC_ADD(HSS_METR_GLOB_CTR_S6a_RX_IDA_ERROR,    s6a.rx_ida_error);
    METRIC_ADD(HSS_METR_GLOB_CTR_S6a_RX_PUR,          s6a.rx_pur);
    METRIC_ADD(HSS_METR_GLOB_CTR_S6a_RX_PUR_ERROR,    s6a.rx_pur_error);
    METRIC_ADD(HSS_METR_GLOB_CTR_S6a_RX_ULR,          s6a.rx_ulr);
    METRIC_ADD(HSS_METR_GLOB_CTR_S6a_RX_ULR_ERROR,    s6a.rx_ulr_error);
    METRIC_ADD(HSS_METR_GLOB_CTR_S6a_TX_AIA,          s6a.tx_aia);
    METRIC_ADD(HSS_METR_GLOB_CTR_S6a_TX_CLR,          s6a.tx_clr);
    METRIC_ADD(HSS_METR_GLOB_CTR_S6a_TX_IDR,          s6a.tx_idr);
    METRIC_ADD(HSS_METR_GLOB_CTR_S6a_TX_PUA,          s6a.tx_pua);
    METRIC_ADD(HSS_METR_GLOB_CTR_S6a_TX_ULA,          s6a.tx_ula);

    METRIC_ADD(HSS_METR_GLOB_CTR_SWx_RX_UNKNOWN,     swx.rx_unknown);
    METRIC_ADD(HSS_METR_GLOB_CTR_SWx_RX_MAR,         swx.rx_mar);
    METRIC_ADD(HSS_METR_GLOB_CTR_SWx_RX_MAR_ERROR,   swx.rx_mar_error);
    METRIC_ADD(HSS_METR_GLOB_CTR_SWx_RX_SAR,         swx.rx_sar);
    METRIC_ADD(HSS_METR_GLOB_CTR_SWx_RX_SAR_ERROR,   swx.rx_sar_error);
    METRIC_ADD(HSS_METR_GLOB_CTR_SWx_TX_MAA,         swx.tx_maa);
    METRIC_ADD(HSS_METR_GLOB_CTR_SWx_TX_SAA,         swx.tx_saa);

    memcpy(&prev_st, st, sizeof(*st));
}

int hss_fd_init(void)
{
    int rv;

    rv = ogs_diam_init(FD_MODE_SERVER,
                hss_self()->diam_conf_path, hss_self()->diam_config);
    ogs_assert(rv == 0);

    rv = hss_s6a_init();
    ogs_assert(rv == OGS_OK);
    rv = hss_cx_init();
    ogs_assert(rv == OGS_OK);
    rv = hss_swx_init();
    ogs_assert(rv == OGS_OK);

    ogs_diam_stats_update_cb_register(hsss_diam_stats_update_cb);

    rv = ogs_diam_start();
    ogs_assert(rv == 0);

    return OGS_OK;
}

void hss_fd_final(void)
{
    hss_s6a_final();
    hss_cx_final();
    hss_swx_final();

    ogs_diam_final();
}
