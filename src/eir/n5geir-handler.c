/*
 * Copyright (C) 2026 by Erol Yağız Aydın <ygzaydns@gmail.com>
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

#include "sbi-path.h"
#include "n5geir-handler.h"

static bool pei_is_valid(const char *pei)
{
    char *type;
    bool rc = false;

    type = ogs_id_get_type(pei);
    if (!type)
        return false;

    if (!strcmp(type, "imei") || !strcmp(type, "imeisv"))
        rc = true;

    ogs_free(type);
    return rc;
}

static bool supi_is_valid(const char *supi)
{
    char *type;
    bool rc = false;

    type = ogs_id_get_type(supi);
    if (!type)
        return false;

    if (!strcmp(type, "imsi"))
        rc = true;

    ogs_free(type);
    return rc;
}

static OpenAPI_equipment_status_e equipment_status_from_dbi(
        ogs_dbi_eir_status_t status)
{
    switch (status) {
    case OGS_DBI_EIR_STATUS_WHITELISTED:
        return OpenAPI_equipment_status_WHITELISTED;
    case OGS_DBI_EIR_STATUS_BLACKLISTED:
        return OpenAPI_equipment_status_BLACKLISTED;
    case OGS_DBI_EIR_STATUS_GREYLISTED:
        return OpenAPI_equipment_status_GREYLISTED;
    default:
        return OpenAPI_equipment_status_NULL;
    }
}

bool eir_n5geir_eic_handle_equipment_status(
        ogs_sbi_stream_t *stream, ogs_sbi_message_t *recvmsg)
{
    int rv;
    ogs_dbi_eir_record_t record;
    ogs_sbi_message_t sendmsg;
    ogs_sbi_response_t *response = NULL;
    OpenAPI_eir_response_data_t EirResponseData;

    ogs_assert(stream);
    ogs_assert(recvmsg);

    if (!recvmsg->param.pei) {
        ogs_error("No PEI in equipment-status request");
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST, recvmsg,
                "No PEI", NULL, NULL));
        return false;
    }

    if (!pei_is_valid(recvmsg->param.pei)) {
        ogs_error("Invalid PEI in equipment-status request");
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST, recvmsg,
                "Invalid PEI", NULL, NULL));
        return false;
    }

    if (recvmsg->param.supi && !supi_is_valid(recvmsg->param.supi)) {
        ogs_error("Invalid SUPI in equipment-status request");
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST, recvmsg,
                "Invalid SUPI", NULL, NULL));
        return false;
    }

    rv = ogs_dbi_eir_check_equipment(
            recvmsg->param.supi, recvmsg->param.pei, &record);
    switch (rv) {
    case OGS_OK:
        memset(&EirResponseData, 0, sizeof(EirResponseData));
        EirResponseData.status = equipment_status_from_dbi(record.status);
        ogs_dbi_eir_record_free(&record);

        memset(&sendmsg, 0, sizeof(sendmsg));
        sendmsg.EirResponseData = &EirResponseData;

        response = ogs_sbi_build_response(&sendmsg, OGS_SBI_HTTP_STATUS_OK);
        ogs_assert(response);
        ogs_assert(true == ogs_sbi_server_send_response(stream, response));
        return true;

    case OGS_NOTFOUND:
        ogs_info("Unknown equipment [supi_presence:%s]",
                recvmsg->param.supi ? "true" : "false");
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream,
                OGS_SBI_HTTP_STATUS_NOT_FOUND, recvmsg,
                "PEI not found", NULL, "ERROR_EQUIPMENT_UNKNOWN"));
        return false;

    default:
        ogs_error("EIR database error");
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream,
                OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR, recvmsg,
                "EIR database error", NULL, NULL));
        return false;
    }
}
