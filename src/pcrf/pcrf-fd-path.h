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

#ifndef PCRF_FD_PATH_H
#define PCRF_FD_PATH_H

#ifdef __cplusplus
extern "C" {
#endif

struct sess_state;
typedef struct ogs_diam_rx_message_s ogs_diam_rx_message_t;

typedef struct pcrf_diam_stats_gx_s {
    unsigned long long rx_unknown;
    unsigned long long rx_ccr;
    unsigned long long rx_ccr_error;
    unsigned long long rx_raa;
    unsigned long long tx_cca;
    unsigned long long tx_rar;
    unsigned long long tx_rar_error;
} pcrf_diam_stats_gx_t;

typedef struct pcrf_diam_stats_rx_s {
    unsigned long long rx_unknown;
    unsigned long long rx_aar;
    unsigned long long rx_aar_error;
    unsigned long long rx_asa;
    unsigned long long rx_str;
    unsigned long long rx_str_error;
    unsigned long long tx_aaa;
    unsigned long long tx_sar;
    unsigned long long tx_sta;
} pcrf_diam_stats_rx_t;

typedef struct pcrf_diam_stats_s {
    pcrf_diam_stats_gx_t gx;
    pcrf_diam_stats_rx_t rx;
} pcrf_diam_stats_t;

#define PCRF_DIAM_PRIV_STATS_ADD(field, val) ((pcrf_diam_stats_t *)ogs_diam_stats_self()->priv_stats)->field += val
#define PCRF_DIAM_PRIV_STATS_INC(field) PCRF_DIAM_PRIV_STATS_ADD(field, 1)

int pcrf_fd_init(void);
void pcrf_fd_final(void);

int pcrf_gx_init(void);
void pcrf_gx_final(void);
int pcrf_rx_init(void);
void pcrf_rx_final(void);

int pcrf_gx_send_rar(
        uint8_t *gx_sid, uint8_t *rx_sid, ogs_diam_rx_message_t *rx_message);
int pcrf_rx_send_asr(
        uint8_t *rx_sid, uint32_t abort_cause);

#ifdef __cplusplus
}
#endif

#endif /* PCRF_FD_PATH_H */

