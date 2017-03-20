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
"    if (mobile_identity->tmsi.type_of_identity == NAS_MOBILE_IDENTITY_TMSI)\n" \
"    {\n" \
"        if (mobile_identity->tmsi.spare != 0xf)\n" \
"            d_warn(\"Spec warning : mobile_identity->tmsi.spare = 0x%x\", mobile_identity->tmsi.spare);\n" \
"        mobile_identity->tmsi.tmsi = ntohl(mobile_identity->tmsi.tmsi);\n" \
"    }\n\n"
type_list["Mobile identity"]["encode"] = \
"    if (mobile_identity->tmsi.type_of_identity == NAS_MOBILE_IDENTITY_TMSI)\n" \
"    {\n" \
"        target.tmsi.tmsi = htonl(mobile_identity->tmsi.tmsi);\n" \
"        target.tmsi.spare = 0xf;\n" \
"    }\n\n" 

type_list["EPS mobile identity"]["decode"] = \
"    if (eps_mobile_identity->guti.type_of_identity == NAS_EPS_MOBILE_IDENTITY_GUTI)\n" \
"    {\n" \
"        if (eps_mobile_identity->guti.spare != 0xf)\n" \
"            d_warn(\"Spec warning : eps_mobile_identy->spare = 0x%x\", eps_mobile_identity->guti.spare);\n" \
"        eps_mobile_identity->guti.mme_group_id = ntohs(eps_mobile_identity->guti.mme_group_id);\n" \
"        eps_mobile_identity->guti.m_tmsi = ntohl(eps_mobile_identity->guti.m_tmsi);\n" \
"    }\n\n"
type_list["EPS mobile identity"]["encode"] = \
"    if (target.guti.type_of_identity == NAS_EPS_MOBILE_IDENTITY_GUTI)\n" \
"    {\n" \
"        target.guti.spare = 0xf;\n" \
"        target.guti.mme_group_id = htons(eps_mobile_identity->guti.mme_group_id);\n" \
"        target.guti.m_tmsi = htonl(eps_mobile_identity->guti.m_tmsi);\n" \
"    }\n\n"

