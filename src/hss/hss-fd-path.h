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

#ifndef HSS_FD_PATH_H
#define HSS_FD_PATH_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct hss_diam_stats_cx_s {
    unsigned long long rx_unknown;
    unsigned long long rx_mar;
    unsigned long long rx_mar_error;
    unsigned long long rx_sar;
    unsigned long long rx_sar_error;
    unsigned long long rx_uar;
    unsigned long long rx_uar_error;
    unsigned long long rx_lir;
    unsigned long long rx_lir_error;
    unsigned long long tx_maa;
    unsigned long long tx_saa;
    unsigned long long tx_uaa;
    unsigned long long tx_lia;
} hss_diam_stats_cx_t;

typedef struct hss_diam_stats_s6a_s {
    unsigned long long rx_unknown;
    unsigned long long rx_air;
    unsigned long long rx_air_error;
    unsigned long long rx_cla;
    unsigned long long rx_cla_error;
    unsigned long long rx_ida;
    unsigned long long rx_ida_error;
    unsigned long long rx_pur;
    unsigned long long rx_pur_error;
    unsigned long long rx_ulr;
    unsigned long long rx_ulr_error;
    unsigned long long tx_aia;
    unsigned long long tx_clr;
    unsigned long long tx_idr;
    unsigned long long tx_pua;
    unsigned long long tx_ula;
} hss_diam_stats_s6a_t;

typedef struct hss_diam_stats_swx_s {
    unsigned long long rx_unknown;
    unsigned long long rx_mar;
    unsigned long long rx_mar_error;
    unsigned long long rx_sar;
    unsigned long long rx_sar_error;
    unsigned long long tx_maa;
    unsigned long long tx_saa;
} hss_diam_stats_swx_t;

typedef struct hss_diam_stats_s {
    hss_diam_stats_cx_t cx;
    hss_diam_stats_s6a_t s6a;
    hss_diam_stats_swx_t swx;
} hss_diam_stats_t;

#define HSS_DIAM_PRIV_STATS_ADD(field, val) ((hss_diam_stats_t *)ogs_diam_stats_self()->priv_stats)->field += val
#define HSS_DIAM_PRIV_STATS_INC(field) HSS_DIAM_PRIV_STATS_ADD(field, 1)

int hss_fd_init(void);
void hss_fd_final(void);

int hss_s6a_init(void);
void hss_s6a_final(void);
int hss_cx_init(void);
void hss_cx_final(void);
int hss_swx_init(void);
void hss_swx_final(void);

#ifdef __cplusplus
}
#endif

#endif /* HSS_FD_PATH_H */
