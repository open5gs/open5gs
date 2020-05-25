
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

type_list["5GS mobile identity"]["decode"] = \
"    if (mobile_identity->guti.type == OGS_NAS_5GS_MOBILE_IDENTITY_GUTI) {\n" \
"        mobile_identity->guti.m_tmsi = be32toh(mobile_identity->guti.m_tmsi);\n" \
"    } else if (mobile_identity->s_tmsi.type == OGS_NAS_5GS_MOBILE_IDENTITY_S_TMSI) {\n" \
"        mobile_identity->s_tmsi.m_tmsi = be32toh(mobile_identity->s_tmsi.m_tmsi);\n" \
"    }\n\n"
type_list["5GS mobile identity"]["encode"] = \
"    if (mobile_identity->guti.type == OGS_NAS_5GS_MOBILE_IDENTITY_GUTI) {\n" \
"        target.guti.m_tmsi = htobe32(mobile_identity->guti.m_tmsi);\n" \
"        target.guti._0xf = 0xf;\n" \
"    } else if (mobile_identity->s_tmsi.type == OGS_NAS_5GS_MOBILE_IDENTITY_S_TMSI) {\n" \
"        target.s_tmsi.m_tmsi = htobe32(mobile_identity->s_tmsi.m_tmsi);\n" \
"        target.s_tmsi._0xf = 0xf;\n" \
"    }\n\n"

type_list["Header compression configuration"]["decode"] = \
"    header_compression_configuration->max_cid = be16toh(header_compression_configuration->max_cid);\n\n"
type_list["Header compression configuration"]["encode"] = \
"    target.max_cid = htobe16(header_compression_configuration->max_cid);\n\n"

type_list["DNN"]["decode"] = \
"    {\n" \
"        char data_network_name[OGS_MAX_DNN_LEN];\n" \
"        dnn->length  = ogs_fqdn_parse(data_network_name, dnn->value, dnn->length);\n" \
"        ogs_cpystrn(dnn->value, data_network_name, ogs_min(dnn->length, OGS_MAX_DNN_LEN) + 1);\n" \
"    }\n\n"

type_list["DNN"]["encode"] = \
"    target.length = ogs_fqdn_build(target.value, dnn->value, dnn->length);\n" \
"    size = target.length + sizeof(target.length);\n\n"
