type_list["P-TMSI signature"]["decode"] = \
"    *p_tmsi_signature = htonl(*p_tmsi_signature);\n\n"
type_list["P-TMSI signature"]["encode"] = \
"    *p_tmsi_signature = ntohl(*p_tmsi_signature);\n\n"

type_list["Location area identification"]["decode"] = \
"    location_area_identification->lac = ntohs(location_area_identification->lac);\n\n"
type_list["Location area identification"]["encode"] = \
"    target.lac = htons(location_area_identification->lac);\n\n"

type_list["Tracking area identity"]["decode"] = \
"    tracking_area_identity->tac = ntohs(tracking_area_identity->tac);\n\n"
type_list["Tracking area identity"]["encode"] = \
"    target.tac = htons(tracking_area_identity->tac);\n\n"

type_list["Mobile identity"]["decode"] = \
"    if (mobile_identity->tmsi.type == NAS_MOBILE_IDENTITY_TMSI)\n" \
"    {\n" \
"        if (mobile_identity->tmsi.spare != 0xf)\n" \
"            d_warn(\"Spec warning : mobile_identity->tmsi.spare = 0x%x\", mobile_identity->tmsi.spare);\n" \
"        mobile_identity->tmsi.tmsi = ntohl(mobile_identity->tmsi.tmsi);\n" \
"    }\n\n"
type_list["Mobile identity"]["encode"] = \
"    if (mobile_identity->tmsi.type == NAS_MOBILE_IDENTITY_TMSI)\n" \
"    {\n" \
"        target.tmsi.tmsi = htonl(mobile_identity->tmsi.tmsi);\n" \
"        target.tmsi.spare = 0xf;\n" \
"    }\n\n" 

type_list["EPS mobile identity"]["decode"] = \
"    if (eps_mobile_identity->guti.type == NAS_EPS_MOBILE_IDENTITY_GUTI)\n" \
"    {\n" \
"        if (eps_mobile_identity->guti.spare != 0xf)\n" \
"            d_warn(\"Spec warning : eps_mobile_identy->spare = 0x%x\", eps_mobile_identity->guti.spare);\n" \
"        eps_mobile_identity->guti.mme_gid = ntohs(eps_mobile_identity->guti.mme_gid);\n" \
"        eps_mobile_identity->guti.m_tmsi = ntohl(eps_mobile_identity->guti.m_tmsi);\n" \
"    }\n\n"
type_list["EPS mobile identity"]["encode"] = \
"    if (target.guti.type == NAS_EPS_MOBILE_IDENTITY_GUTI)\n" \
"    {\n" \
"        target.guti.spare = 0xf;\n" \
"        target.guti.mme_gid = htons(eps_mobile_identity->guti.mme_gid);\n" \
"        target.guti.m_tmsi = htonl(eps_mobile_identity->guti.m_tmsi);\n" \
"    }\n\n"

type_list["Nonce"]["decode"] = \
"    *nonce = ntohl(*nonce);\n\n"
type_list["Nonce"]["encode"] = \
"    target = htonl(*nonce);\n\n"

type_list["Header compression configuration"]["decode"] = \
"    header_compression_configuration->max_cid = ntohs(header_compression_configuration->max_cid);\n\n"
type_list["Header compression configuration"]["encode"] = \
"    target.max_cid = htons(header_compression_configuration->max_cid);\n\n"

type_list["Short MAC"]["decode"] = \
"    *short_mac = ntohs(*short_mac);\n\n"
type_list["Short MAC"]["encode"] = \
"    target = htons(*short_mac);\n\n"

type_list["Access point name"]["decode"] = \
"    {\n" \
"        c_int8_t apn[MAX_APN_LEN];\n" \
"        access_point_name->length  = apn_parse(apn, access_point_name->apn, access_point_name->length);\n" \
"        core_cpystrn(access_point_name->apn, apn, c_min(access_point_name->length, MAX_APN_LEN) + 1);\n" \
"    }\n\n"

type_list["Access point name"]["encode"] = \
"    target.length = apn_build(target.apn, access_point_name->apn, access_point_name->length);\n" \
"    size = target.length + sizeof(target.length);\n\n"
