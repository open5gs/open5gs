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

#ifndef OGS_DIAM_STATS_H
#define OGS_DIAM_STATS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/time.h>

typedef struct ogs_diam_stats_s {
    unsigned long long nb_echoed; /* server */
    unsigned long long nb_sent;   /* client */
    unsigned long long nb_recv;   /* client */
    unsigned long long nb_errs;   /* client */
    unsigned long shortest;  /* fastest answer, in microseconds */
    unsigned long longest;   /* slowest answer, in microseconds */
    unsigned long avg;       /* average answer time, in microseconds */
} ogs_diam_stats_t;

typedef void (*ogs_diam_stats_update_cb)(const ogs_diam_stats_t *stats, const void *priv_stats);

typedef struct ogs_diam_stats_ctx_s {

#define FD_MODE_SERVER   0x1
#define FD_MODE_CLIENT   0x2
    int mode;        /* default FD_MODE_SERVER | FD_MODE_CLIENT */

    struct poll {
        ogs_timer_t *timer;
        ogs_time_t t_start; /* in usecs */
        ogs_time_t t_prev; /* in usecs */
        ogs_time_t t_interval; /* in usecs */
    } poll;
    ogs_diam_stats_t stats;
    void *priv_stats; /* if !NULL, allocated struct of size "priv_stats_size" */
    size_t priv_stats_size;
    void *priv_stats_copy; /* buffer where priv_state are copied and passed to update_cb */
    ogs_diam_stats_update_cb update_cb;

    pthread_mutex_t stats_lock;
} ogs_diam_stats_ctx_t;

int ogs_diam_stats_init(int mode, const ogs_diam_config_stats_t *config);
void ogs_diam_stats_final(void);

ogs_diam_stats_ctx_t* ogs_diam_stats_self(void);
#define OGS_DIAM_STATS_ADD(field, val) ogs_diam_stats_self()->stats.field += val
#define OGS_DIAM_STATS_INC(field) OGS_DIAM_STATS_ADD(field, 1)

#define OGS_DIAM_STATS_MTX(code) \
    ogs_assert(pthread_mutex_lock(&ogs_diam_stats_self()->stats_lock) == 0); \
    { code } \
    ogs_assert(pthread_mutex_unlock(&ogs_diam_stats_self()->stats_lock) == 0);


int ogs_diam_stats_start(void);

void ogs_diam_stats_update_cb_register(ogs_diam_stats_update_cb cb);
void ogs_diam_stats_update_cb_unregister(void);

#ifdef __cplusplus
}
#endif

#endif /* OGS_DIAM_STATS_H */
