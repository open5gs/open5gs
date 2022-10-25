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

#include "nbsf-build.h"

ogs_sbi_request_t *af_nbsf_management_build_discover(
        af_sess_t *sess, void *data)
{
    ogs_sbi_message_t message;
    ogs_sbi_request_t *request = NULL;

    ogs_assert(sess);

    memset(&message, 0, sizeof(message));
    message.h.method = (char *)OGS_SBI_HTTP_METHOD_GET;
    message.h.service.name = (char *)OGS_SBI_SERVICE_NAME_NBSF_MANAGEMENT;
    message.h.api.version = (char *)OGS_SBI_API_V1;
    message.h.resource.component[0] =
        (char *)OGS_SBI_RESOURCE_NAME_PCF_BINDINGS;

    message.param.ipv4addr = sess->ipv4addr;
    message.param.ipv6prefix = sess->ipv6prefix;

    request = ogs_sbi_build_request(&message);
    ogs_expect(request);
    
    return request;
}
