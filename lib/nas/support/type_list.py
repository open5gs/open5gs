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

type_list["Tracking area identity list"]["decode"] = \
"    int i = 0;\n" \
"    {\n" \
"        if (tracking_area_identity_list->type == NAS_TRACKING_AREA_IDENTITY_LIST_ONE_PLMN_NON_CONSECUTIVE_TACS)\n" \
"            for (i = 0; i < tracking_area_identity_list->num + 1 && i < NAS_MAX_TRACKING_AREA_IDENTITY; i++)\n" \
"                tracking_area_identity_list->type0.tac[i] = ntohs(tracking_area_identity_list->type0.tac[i]);\n" \
"        else if (tracking_area_identity_list->type == NAS_TRACKING_AREA_IDENTITY_LIST_ONE_PLMN_CONSECUTIVE_TACS)\n" \
"            tracking_area_identity_list->type1.tac = ntohs(tracking_area_identity_list->type1.tac);\n" \
"        else if (tracking_area_identity_list->type == NAS_TRACKING_AREA_IDENTITY_LIST_MANY_PLMNS)\n" \
"            for (i = 0; i < tracking_area_identity_list->num + 1 && i < NAS_MAX_TRACKING_AREA_IDENTITY; i++)\n" \
"                tracking_area_identity_list->type2.tai[i].tac = ntohs(tracking_area_identity_list->type2.tai[i].tac);\n" \
"        else\n" \
"            return -1;\n" \
"    }\n\n"
type_list["Tracking area identity list"]["encode"] = \
"    int i = 0;\n" \
"    {\n" \
"        if (tracking_area_identity_list->type == NAS_TRACKING_AREA_IDENTITY_LIST_ONE_PLMN_NON_CONSECUTIVE_TACS)\n" \
"            for (i = 0; i < tracking_area_identity_list->num + 1 && i < NAS_MAX_TRACKING_AREA_IDENTITY; i++)\n" \
"                target.type0.tac[i] = htons(tracking_area_identity_list->type0.tac[i]);\n" \
"        else if (tracking_area_identity_list->type == NAS_TRACKING_AREA_IDENTITY_LIST_ONE_PLMN_CONSECUTIVE_TACS)\n" \
"            target.type1.tac = htons(tracking_area_identity_list->type1.tac);\n" \
"        else if (tracking_area_identity_list->type == NAS_TRACKING_AREA_IDENTITY_LIST_MANY_PLMNS)\n" \
"            for (i = 0; i < tracking_area_identity_list->num + 1 && i < NAS_MAX_TRACKING_AREA_IDENTITY; i++)\n" \
"                target.type2.tai[i].tac = htons(tracking_area_identity_list->type2.tai[i].tac);\n" \
"        else\n" \
"            return -1;\n" \
"    }\n\n"

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
