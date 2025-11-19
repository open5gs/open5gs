/*
 * Copyright (C) 2019-2023 by Sukchan Lee <acetcom@gmail.com>
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

#if !defined(OGS_APP_INSIDE) && !defined(OGS_APP_COMPILATION)
#error "This header cannot be included directly."
#endif

#ifndef OGS_APP_CONTEXT_H
#define OGS_APP_CONTEXT_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ogs_app_context_s {
    const char *version;

    const char *file;
    void *document;

    const char *db_uri;

    struct {
        ogs_log_ts_e timestamp;
    } logger_default;

    struct {
        const char *file;
        const char *level;
        const char *domain;
        ogs_log_ts_e timestamp;
    } logger;

    ogs_queue_t *queue;
    ogs_timer_mgr_t *timer_mgr;
    ogs_pollset_t *pollset;

    struct {
        int udp_port;
    } usrsctp;

    struct {
        uint64_t packet;

        uint64_t sess;
        uint64_t bearer;
        uint64_t tunnel;

        uint64_t nf_service;

        uint64_t timer;
        uint64_t message;
        uint64_t event;
        uint64_t socket;
        uint64_t subscription;
        uint64_t xact;
        uint64_t stream;

        uint64_t nf;
        uint64_t gtp_node;

        uint64_t csmap;

        uint64_t impi;
        uint64_t impu;
    } pool;

    struct metrics {
        uint64_t max_specs;
    } metrics;

    int config_section_id;

} ogs_app_context_t;

int ogs_app_context_init(void);
void ogs_app_context_final(void);

ogs_app_context_t *ogs_app(void);

#ifdef __cplusplus
}
#endif

#endif /* OGS_APP_CONTEXT_H */
