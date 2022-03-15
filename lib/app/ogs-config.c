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

#include "ogs-app.h"

int ogs_app_config_parse_sockopt(
        ogs_yaml_iter_t *parent, ogs_sockopt_t *option)
{
    ogs_yaml_iter_t sockopt_iter;

    ogs_assert(parent);
    ogs_assert(option);

    ogs_sockopt_init(option);

    ogs_yaml_iter_recurse(parent, &sockopt_iter);
    while (ogs_yaml_iter_next(&sockopt_iter)) {
        const char *sockopt_key = ogs_yaml_iter_key(&sockopt_iter);
        ogs_assert(sockopt_key);

        if (!strcmp(sockopt_key, "sctp")) {
            ogs_yaml_iter_t sctp_iter;
            ogs_yaml_iter_recurse(&sockopt_iter, &sctp_iter);

            while (ogs_yaml_iter_next(&sctp_iter)) {
                const char *sctp_key = ogs_yaml_iter_key(&sctp_iter);
                ogs_assert(sctp_key);
                if (!strcmp(sctp_key, "spp_hbinterval")) {
                    const char *v = ogs_yaml_iter_value(&sctp_iter);
                    if (v) option->sctp.spp_hbinterval = atoi(v);
                } else if (!strcmp(sctp_key, "spp_sackdelay")) {
                    const char *v = ogs_yaml_iter_value(&sctp_iter);
                    if (v) option->sctp.spp_sackdelay = atoi(v);
                } else if (!strcmp(sctp_key, "srto_initial")) {
                    const char *v = ogs_yaml_iter_value(&sctp_iter);
                    if (v) option->sctp.srto_initial = atoi(v);
                } else if (!strcmp(sctp_key, "srto_min")) {
                    const char *v = ogs_yaml_iter_value(&sctp_iter);
                    if (v) option->sctp.srto_min = atoi(v);
                } else if (!strcmp(sctp_key, "srto_max")) {
                    const char *v = ogs_yaml_iter_value(&sctp_iter);
                    if (v) option->sctp.srto_max = atoi(v);
                } else if (!strcmp(sctp_key, "sinit_num_ostreams")) {
                    const char *v = ogs_yaml_iter_value(&sctp_iter);
                    if (v) option->sctp.sinit_num_ostreams = atoi(v);
                } else if (!strcmp(sctp_key, "sinit_max_instreams")) {
                    const char *v = ogs_yaml_iter_value(&sctp_iter);
                    if (v) option->sctp.sinit_max_instreams = atoi(v);
                } else if (!strcmp(sctp_key, "sinit_max_attempts")) {
                    const char *v = ogs_yaml_iter_value(&sctp_iter);
                    if (v) option->sctp.sinit_max_attempts = atoi(v);
                } else if (!strcmp(sctp_key, "sinit_max_init_timeo")) {
                    const char *v = ogs_yaml_iter_value(&sctp_iter);
                    if (v) option->sctp.sinit_max_init_timeo = atoi(v);
                } else {
                    ogs_error("unknown key `%s`", sctp_key);
                    return OGS_ERROR;
                }
            }
        } else if (!strcmp(sockopt_key, "sctp_nodelay")) {
            option->sctp_nodelay = ogs_yaml_iter_bool(&sockopt_iter);
        } else if (!strcmp(sockopt_key, "tcp_nodelay")) {
            option->tcp_nodelay = ogs_yaml_iter_bool(&sockopt_iter);

        } else if (!strcmp(sockopt_key, "so_linger")) {
            ogs_yaml_iter_t so_linger_iter;
            ogs_yaml_iter_recurse(&sockopt_iter, &so_linger_iter);

            while (ogs_yaml_iter_next(&so_linger_iter)) {
                const char *so_linger_key = ogs_yaml_iter_key(&so_linger_iter);
                ogs_assert(so_linger_key);
                if (!strcmp(so_linger_key, "l_onoff")) {
                    option->so_linger.l_onoff =
                        ogs_yaml_iter_bool(&so_linger_iter);
                } else if (!strcmp(so_linger_key, "l_linger")) {
                    const char *v = ogs_yaml_iter_value(&so_linger_iter);
                    if (v) option->so_linger.l_linger = atoi(v);
                } else {
                    ogs_error("unknown key `%s`", so_linger_key);
                    return OGS_ERROR;
                }
            }

        } else if (!strcmp(sockopt_key, "so_bindtodevice")) {
            option->so_bindtodevice = ogs_yaml_iter_value(&sockopt_iter);

        } else {
            ogs_error("unknown key `%s`", sockopt_key);
            return OGS_ERROR;
        }
    }

    return OGS_OK;
}
