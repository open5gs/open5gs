
# Copyright (C) 2019 by Sukchan Lee <acetcom@gmail.com>

# This file is part of Open5GS.

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

type_list["5GS tracking area identity"]["decode"] = \
"    tracking_area_identity->tac = ogs_be24toh(tracking_area_identity->tac);\n\n"
type_list["5GS tracking area identity"]["encode"] = \
"    target.tac = ogs_htobe24(tracking_area_identity->tac);\n\n"

type_list["Header compression configuration"]["decode"] = \
"    header_compression_configuration->max_cid = be16toh(header_compression_configuration->max_cid);\n\n"
type_list["Header compression configuration"]["encode"] = \
"    target.max_cid = htobe16(header_compression_configuration->max_cid);\n\n"

type_list["DNN"]["decode"] = \
"    {\n" \
"        char data_network_name[OGS_MAX_DNN_LEN+1];\n" \
"        dnn->length = ogs_fqdn_parse(data_network_name, dnn->value, ogs_min(dnn->length, OGS_MAX_DNN_LEN));\n" \
"        if (dnn->length > 0) {\n" \
"            ogs_cpystrn(dnn->value, data_network_name, ogs_min(dnn->length, OGS_MAX_DNN_LEN)+1);\n" \
"        } else {\n" \
"            ogs_error(\"UE not APN setting\");\n" \
"        }\n" \
"    }\n\n"

type_list["DNN"]["encode"] = \
"    target.length = ogs_fqdn_build(target.value, dnn->value, dnn->length);\n" \
"    size = target.length + sizeof(target.length);\n\n"

type_list["Session-AMBR"]["decode"] = \
"    session_ambr->downlink.value = be16toh(source->downlink.value);\n" \
"    session_ambr->uplink.value = be16toh(source->uplink.value);\n\n"

type_list["Session-AMBR"]["encode"] = \
"    target.downlink.value = htobe16(session_ambr->downlink.value);\n" \
"    target.uplink.value = htobe16(session_ambr->uplink.value);\n\n"

type_list["Uplink data status"]["decode"] = \
"    uplink_data_status->psi = be16toh(uplink_data_status->psi);\n\n"
type_list["Uplink data status"]["encode"] = \
"    target.psi = htobe16(uplink_data_status->psi);\n\n"
type_list["Allowed PDU session status"]["decode"] = \
"    allowed_pdu_session_status->psi = be16toh(allowed_pdu_session_status->psi);\n\n"
type_list["Allowed PDU session status"]["encode"] = \
"    target.psi = htobe16(allowed_pdu_session_status->psi);\n\n"
type_list["PDU session status"]["decode"] = \
"    pdu_session_status->psi = be16toh(pdu_session_status->psi);\n\n"
type_list["PDU session status"]["encode"] = \
"    target.psi = htobe16(pdu_session_status->psi);\n\n"
type_list["PDU session reactivation result"]["decode"] = \
"    pdu_session_reactivation_result->psi = be16toh(pdu_session_reactivation_result->psi);\n\n"
type_list["PDU session reactivation result"]["encode"] = \
"    target.psi = htobe16(pdu_session_reactivation_result->psi);\n\n"
