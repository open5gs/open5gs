/*
 * Copyright (C) 2026 by Sukchan Lee <acetcom@gmail.com>
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

/*
 * S10 GTPv2-C messages and IEs (TS 29.274 clauses 7.3 and 8.38 to 8.51).
 *
 * Reference vectors are derived by hand from the figures of TS 29.274.
 */

#include <ctype.h>

#include "ogs-gtp.h"
#include "core/abts.h"

#define TEST_PLMN(__p) \
    ogs_plmn_id_build((ogs_plmn_id_t *)(__p), 999, 70, 2)

static void hex_equal(abts_case *tc, const char *hex,
        const void *data, int len, int line)
{
    char buf[OGS_HUGE_LEN];
    int expected_len = 0;
    const char *p;

    for (p = hex; *p; p++)
        if (isxdigit((unsigned char)*p)) expected_len++;
    expected_len /= 2;

    abts_int_equal(tc, expected_len, len, line);
    if (expected_len != len)
        return;
    abts_true(tc, memcmp(ogs_hex_from_string(hex, buf, sizeof(buf)),
                data, len) == 0, line);
}
#define HEX_EQUAL(tc, hex, data, len) hex_equal(tc, hex, data, len, __LINE__)

/* 8.38 MM Context : minimal EPS Security Context, golden vector */
static void gtp_s10_test_mm_context_minimal(abts_case *tc, void *data)
{
    ogs_gtp2_mm_context_t mm, decoded;
    ogs_tlv_octet_t octet;
    uint8_t buf[512];
    int16_t size;

    const char *expected =
        /* Security Mode=4 | NHI=0 | DRXI=0 | KSIASME=1 */
        "81"
        /* Quintuplets=0 | Quadruplets=0 | UAMBRI=0 | OSCI=0 */
        "00"
        /* SAMBRI=0 | Used NAS integrity=EIA2 | Used NAS cipher=EEA0 */
        "20"
        /* NAS Downlink Count, NAS Uplink Count */
        "000005" "000003"
        /* KASME */
        "1111111111111111111111111111111111111111111111111111111111111111"
        /* UE Network Capability, MS Network Capability, MEI */
        "02e0e0" "00" "00"
        /* Access restriction data */
        "00"
        /* Voice Domain Preference, UE Radio Capability for Paging */
        "00" "0000"
        /* Extended Access Restriction, UE additional security capability,
         * UE NR security capability */
        "00" "00" "00"
        /* APN Rate Control Statuses, Core Network Restrictions,
         * UE Radio Capability ID */
        "0000" "00" "00"
        /* Octet 'a' : TRIDI=0 | ENSCT=native */
        "01";

    memset(&mm, 0, sizeof(mm));
    mm.ksi_asme = 1;
    mm.nas_integrity_algorithm = 2;
    mm.nas_cipher_algorithm = 0;
    mm.nas_downlink_count = 5;
    mm.nas_uplink_count = 3;
    memset(mm.kasme, 0x11, sizeof(mm.kasme));
    mm.ue_network_capability_len = 2;
    mm.ue_network_capability[0] = 0xe0;
    mm.ue_network_capability[1] = 0xe0;
    mm.ensct = OGS_GTP2_MM_CONTEXT_ENSCT_NATIVE;

    size = ogs_gtp2_build_mm_context(&octet, &mm, buf, sizeof(buf));
    ABTS_INT_EQUAL(tc, 58, size);
    HEX_EQUAL(tc, expected, octet.data, octet.len);

    size = ogs_gtp2_parse_mm_context(&decoded, &octet);
    ABTS_INT_EQUAL(tc, 58, size);
    ABTS_INT_EQUAL(tc,
        OGS_GTP2_MM_CONTEXT_SECURITY_MODE_EPS_SECURITY_CONTEXT_AND_QUADRUPLETS,
        decoded.security_mode);
    ABTS_INT_EQUAL(tc, 1, decoded.ksi_asme);
    ABTS_INT_EQUAL(tc, 2, decoded.nas_integrity_algorithm);
    ABTS_INT_EQUAL(tc, 0, decoded.nas_cipher_algorithm);
    ABTS_INT_EQUAL(tc, 5, decoded.nas_downlink_count);
    ABTS_INT_EQUAL(tc, 3, decoded.nas_uplink_count);
    ABTS_TRUE(tc, memcmp(mm.kasme, decoded.kasme, sizeof(mm.kasme)) == 0);
    ABTS_INT_EQUAL(tc, 0, decoded.nh_presence);
    ABTS_INT_EQUAL(tc, 0, decoded.drx_parameter_presence);
    ABTS_INT_EQUAL(tc, 0, decoded.subscribed_ue_ambr_presence);
    ABTS_INT_EQUAL(tc, 0, decoded.used_ue_ambr_presence);
    ABTS_INT_EQUAL(tc, 2, decoded.ue_network_capability_len);
    ABTS_INT_EQUAL(tc, 0xe0, decoded.ue_network_capability[1]);
    ABTS_INT_EQUAL(tc, 0, decoded.ms_network_capability_len);
    ABTS_INT_EQUAL(tc, 0, decoded.mei_len);
    ABTS_INT_EQUAL(tc, 1, decoded.access_restriction_data_presence);
    ABTS_INT_EQUAL(tc, 0, decoded.old_security_context_presence);
    ABTS_INT_EQUAL(tc, 1, decoded.octet_a_presence);
    ABTS_INT_EQUAL(tc, OGS_GTP2_MM_CONTEXT_ENSCT_NATIVE, decoded.ensct);
}

/* 8.38 MM Context : all optional parts, round trip */
static void gtp_s10_test_mm_context_full(abts_case *tc, void *data)
{
    ogs_gtp2_mm_context_t mm, decoded;
    ogs_tlv_octet_t octet;
    uint8_t buf[1024];
    uint8_t paging[5] = { 1, 2, 3, 4, 5 };
    uint8_t radio_cap_id[3] = { 0xa, 0xb, 0xc };
    int16_t size;
    int i;

    memset(&mm, 0, sizeof(mm));
    mm.ksi_asme = 6;
    mm.nas_integrity_algorithm = 1;
    mm.nas_cipher_algorithm = 3;
    mm.nas_downlink_count = 0x123456;
    mm.nas_uplink_count = 0xabcdef;
    for (i = 0; i < OGS_GTP2_KASME_LEN; i++)
        mm.kasme[i] = i;

    mm.num_of_quadruplets = 2;
    for (i = 0; i < 2; i++) {
        memset(mm.quadruplet[i].rand, 0x20 + i, OGS_GTP2_RAND_LEN);
        mm.quadruplet[i].xres_len = 8;
        memset(mm.quadruplet[i].xres, 0x30 + i, 8);
        mm.quadruplet[i].autn_len = OGS_GTP2_AUTN_LEN;
        memset(mm.quadruplet[i].autn, 0x40 + i, OGS_GTP2_AUTN_LEN);
        memset(mm.quadruplet[i].kasme, 0x50 + i, OGS_GTP2_KASME_LEN);
    }

    mm.drx_parameter_presence = true;
    mm.drx_parameter[0] = 0x0a;
    mm.drx_parameter[1] = 0x00;

    mm.nh_presence = true;
    memset(mm.nh, 0x77, OGS_GTP2_NH_LEN);
    mm.ncc = 5;

    mm.subscribed_ue_ambr_presence = true;
    mm.subscribed_ue_ambr.uplink = 100000;
    mm.subscribed_ue_ambr.downlink = 200000;
    mm.used_ue_ambr_presence = true;
    mm.used_ue_ambr.uplink = 50000;
    mm.used_ue_ambr.downlink = 60000;

    mm.ue_network_capability_len = 4;
    memcpy(mm.ue_network_capability, "\xf0\xf0\xc0\x40", 4);
    mm.ms_network_capability_len = 3;
    memcpy(mm.ms_network_capability, "\xe5\xe0\x34", 3);
    mm.mei_len = 8;
    memcpy(mm.mei, "\x53\x61\x20\x00\x91\x78\x84\x00", 8);

    mm.access_restriction_data.una = 1;
    mm.access_restriction_data.nbna = 1;

    mm.old_security_context_presence = true;
    mm.rlos = true;
    mm.old_ksi_asme = 2;
    mm.old_ncc = 7;
    memset(mm.old_kasme, 0x88, OGS_GTP2_KASME_LEN);
    mm.old_nh_presence = true;
    memset(mm.old_nh, 0x99, OGS_GTP2_NH_LEN);

    mm.voice_domain_preference_len = 1;
    mm.voice_domain_preference[0] = 0x03;
    mm.ue_radio_capability_for_paging_len = sizeof(paging);
    mm.ue_radio_capability_for_paging = paging;
    mm.extended_access_restriction_data_len = 1;
    mm.extended_access_restriction_data = 0x01;
    mm.ue_additional_security_capability_len = 4;
    memcpy(mm.ue_additional_security_capability, "\x80\x00\x80\x00", 4);
    mm.ue_nr_security_capability_len = 2;
    memcpy(mm.ue_nr_security_capability, "\xe0\xe0", 2);
    mm.core_network_restrictions_len = 4;
    memcpy(mm.core_network_restrictions, "\x00\x00\x00\x01", 4);
    mm.ue_radio_capability_id_len = sizeof(radio_cap_id);
    mm.ue_radio_capability_id = radio_cap_id;
    mm.tridi = true;
    mm.ensct = OGS_GTP2_MM_CONTEXT_ENSCT_MAPPED;

    size = ogs_gtp2_build_mm_context(&octet, &mm, buf, sizeof(buf));
    ABTS_TRUE(tc, size > 0);

    /* Octets 5 to 7 : fixed header bits */
    ABTS_INT_EQUAL(tc, (4 << 5) | (1 << 4) | (1 << 3) | 6, buf[0]);
    ABTS_INT_EQUAL(tc, (0 << 5) | (2 << 2) | (1 << 1) | 1, buf[1]);
    ABTS_INT_EQUAL(tc, (1 << 7) | (1 << 4) | 3, buf[2]);

    size = ogs_gtp2_parse_mm_context(&decoded, &octet);
    ABTS_INT_EQUAL(tc, octet.len, size);

    ABTS_INT_EQUAL(tc, 6, decoded.ksi_asme);
    ABTS_INT_EQUAL(tc, 1, decoded.nas_integrity_algorithm);
    ABTS_INT_EQUAL(tc, 3, decoded.nas_cipher_algorithm);
    ABTS_INT_EQUAL(tc, 0x123456, decoded.nas_downlink_count);
    ABTS_INT_EQUAL(tc, 0xabcdef, decoded.nas_uplink_count);
    ABTS_TRUE(tc, memcmp(mm.kasme, decoded.kasme, OGS_GTP2_KASME_LEN) == 0);

    ABTS_INT_EQUAL(tc, 2, decoded.num_of_quadruplets);
    ABTS_INT_EQUAL(tc, 0, decoded.num_of_quintuplets);
    for (i = 0; i < 2; i++) {
        ABTS_TRUE(tc, memcmp(&mm.quadruplet[i], &decoded.quadruplet[i],
                    sizeof(ogs_gtp2_auth_quadruplet_t)) == 0);
    }

    ABTS_INT_EQUAL(tc, 1, decoded.drx_parameter_presence);
    ABTS_INT_EQUAL(tc, 0x0a, decoded.drx_parameter[0]);
    ABTS_INT_EQUAL(tc, 1, decoded.nh_presence);
    ABTS_TRUE(tc, memcmp(mm.nh, decoded.nh, OGS_GTP2_NH_LEN) == 0);
    ABTS_INT_EQUAL(tc, 5, decoded.ncc);

    ABTS_INT_EQUAL(tc, 1, decoded.subscribed_ue_ambr_presence);
    ABTS_INT_EQUAL(tc, 100000, decoded.subscribed_ue_ambr.uplink);
    ABTS_INT_EQUAL(tc, 200000, decoded.subscribed_ue_ambr.downlink);
    ABTS_INT_EQUAL(tc, 1, decoded.used_ue_ambr_presence);
    ABTS_INT_EQUAL(tc, 50000, decoded.used_ue_ambr.uplink);
    ABTS_INT_EQUAL(tc, 60000, decoded.used_ue_ambr.downlink);

    ABTS_INT_EQUAL(tc, 4, decoded.ue_network_capability_len);
    ABTS_TRUE(tc, memcmp(mm.ue_network_capability,
                decoded.ue_network_capability, 4) == 0);
    ABTS_INT_EQUAL(tc, 3, decoded.ms_network_capability_len);
    ABTS_INT_EQUAL(tc, 8, decoded.mei_len);
    ABTS_TRUE(tc, memcmp(mm.mei, decoded.mei, 8) == 0);

    ABTS_INT_EQUAL(tc, 1, decoded.access_restriction_data_presence);
    ABTS_INT_EQUAL(tc, 1, decoded.access_restriction_data.una);
    ABTS_INT_EQUAL(tc, 1, decoded.access_restriction_data.nbna);
    ABTS_INT_EQUAL(tc, 0, decoded.access_restriction_data.ena);

    ABTS_INT_EQUAL(tc, 1, decoded.old_security_context_presence);
    ABTS_INT_EQUAL(tc, 1, decoded.rlos);
    ABTS_INT_EQUAL(tc, 2, decoded.old_ksi_asme);
    ABTS_INT_EQUAL(tc, 7, decoded.old_ncc);
    ABTS_TRUE(tc, memcmp(mm.old_kasme, decoded.old_kasme,
                OGS_GTP2_KASME_LEN) == 0);
    ABTS_INT_EQUAL(tc, 1, decoded.old_nh_presence);
    ABTS_TRUE(tc, memcmp(mm.old_nh, decoded.old_nh, OGS_GTP2_NH_LEN) == 0);

    ABTS_INT_EQUAL(tc, 1, decoded.voice_domain_preference_len);
    ABTS_INT_EQUAL(tc, 3, decoded.voice_domain_preference[0]);
    ABTS_INT_EQUAL(tc, sizeof(paging),
            decoded.ue_radio_capability_for_paging_len);
    ABTS_TRUE(tc, memcmp(paging, decoded.ue_radio_capability_for_paging,
                sizeof(paging)) == 0);
    ABTS_INT_EQUAL(tc, 1, decoded.extended_access_restriction_data_len);
    ABTS_INT_EQUAL(tc, 1, decoded.extended_access_restriction_data);
    ABTS_INT_EQUAL(tc, 4, decoded.ue_additional_security_capability_len);
    ABTS_INT_EQUAL(tc, 2, decoded.ue_nr_security_capability_len);
    ABTS_INT_EQUAL(tc, 0, decoded.apn_rate_control_statuses_len);
    ABTS_INT_EQUAL(tc, 4, decoded.core_network_restrictions_len);
    ABTS_INT_EQUAL(tc, 1, decoded.core_network_restrictions[3]);
    ABTS_INT_EQUAL(tc, 3, decoded.ue_radio_capability_id_len);
    ABTS_TRUE(tc, memcmp(radio_cap_id, decoded.ue_radio_capability_id,
                sizeof(radio_cap_id)) == 0);
    ABTS_INT_EQUAL(tc, 1, decoded.octet_a_presence);
    ABTS_INT_EQUAL(tc, 1, decoded.tridi);
    ABTS_INT_EQUAL(tc, OGS_GTP2_MM_CONTEXT_ENSCT_MAPPED, decoded.ensct);
}

/* 8.38 MM Context : older releases, malformed values */
static void gtp_s10_test_mm_context_robustness(abts_case *tc, void *data)
{
    ogs_gtp2_mm_context_t decoded;
    ogs_tlv_octet_t octet;
    char buf[OGS_HUGE_LEN];
    int16_t size;

    /* Release 8 style : the IE ends after the MEI */
    const char *rel8 =
        "81" "00" "20" "000005" "000003"
        "1111111111111111111111111111111111111111111111111111111111111111"
        "02e0e0" "00" "00";

    /* OSCI is set but the Old EPS Security Context is missing */
    const char *osci_missing =
        "81" "01" "20" "000005" "000003"
        "1111111111111111111111111111111111111111111111111111111111111111"
        "02e0e0" "00" "00" "00";

    /* Security Mode 3 (UMTS Key and Quintuplets) is not supported */
    const char *umts = "61" "00" "00";

    /* Truncated in the middle of KASME */
    const char *truncated = "81" "00" "20" "000005" "000003" "1111";

    /* Trailing octets after octet 'a' are ignored */
    const char *extended =
        "81" "00" "20" "000005" "000003"
        "1111111111111111111111111111111111111111111111111111111111111111"
        "02e0e0" "00" "00" "00" "00" "0000" "00" "00" "00" "0000" "00" "00"
        "02" "deadbeef";

    octet.data = ogs_hex_from_string(rel8, buf, sizeof(buf));
    octet.len = 46;
    size = ogs_gtp2_parse_mm_context(&decoded, &octet);
    ABTS_INT_EQUAL(tc, 46, size);
    ABTS_INT_EQUAL(tc, 2, decoded.ue_network_capability_len);
    ABTS_INT_EQUAL(tc, 0, decoded.access_restriction_data_presence);
    ABTS_INT_EQUAL(tc, 0, decoded.octet_a_presence);

    octet.data = ogs_hex_from_string(osci_missing, buf, sizeof(buf));
    octet.len = 47;
    size = ogs_gtp2_parse_mm_context(&decoded, &octet);
    ABTS_INT_EQUAL(tc, 0, size);

    octet.data = ogs_hex_from_string(umts, buf, sizeof(buf));
    octet.len = 3;
    size = ogs_gtp2_parse_mm_context(&decoded, &octet);
    ABTS_INT_EQUAL(tc, 0, size);

    octet.data = ogs_hex_from_string(truncated, buf, sizeof(buf));
    octet.len = 11;
    size = ogs_gtp2_parse_mm_context(&decoded, &octet);
    ABTS_INT_EQUAL(tc, 0, size);

    octet.data = ogs_hex_from_string(extended, buf, sizeof(buf));
    octet.len = 62;
    size = ogs_gtp2_parse_mm_context(&decoded, &octet);
    ABTS_INT_EQUAL(tc, 62, size);
    ABTS_INT_EQUAL(tc, 1, decoded.octet_a_presence);
    ABTS_INT_EQUAL(tc, OGS_GTP2_MM_CONTEXT_ENSCT_MAPPED, decoded.ensct);
}

/* 8.46, 8.47, 8.48, 8.49, 8.51 : golden vectors */
static void gtp_s10_test_simple_ies(abts_case *tc, void *data)
{
    ogs_tlv_octet_t octet;
    uint8_t buf[64];
    char hexbuf[OGS_HUGE_LEN];
    int16_t size;

    ogs_gtp2_guti_t guti, guti2;
    ogs_gtp2_complete_request_message_t crm, crm2;
    ogs_gtp2_f_container_t fc, fc2;
    ogs_gtp2_f_cause_t cause, cause2;
    ogs_gtp2_target_identification_t target, target2;

    /* 8.47 GUTI */
    memset(&guti, 0, sizeof(guti));
    TEST_PLMN(&guti.nas_plmn_id);
    guti.mme_gid = 2;
    guti.mme_code = 1;
    guti.m_tmsi = 0xc0000001;
    size = ogs_gtp2_build_guti(&octet, &guti, buf, sizeof(buf));
    ABTS_INT_EQUAL(tc, OGS_GTP2_GUTI_LEN, size);
    HEX_EQUAL(tc, "99f907 0002 01 c0000001", octet.data, octet.len);
    size = ogs_gtp2_parse_guti(&guti2, &octet);
    ABTS_INT_EQUAL(tc, OGS_GTP2_GUTI_LEN, size);
    ABTS_TRUE(tc, memcmp(&guti, &guti2, sizeof(guti)) == 0);

    octet.data = ogs_hex_from_string("99f907 0002 01 c000", hexbuf,
            sizeof(hexbuf));
    octet.len = 8;
    ABTS_INT_EQUAL(tc, 0, ogs_gtp2_parse_guti(&guti2, &octet));

    /* 8.46 Complete Request Message */
    memset(&crm, 0, sizeof(crm));
    crm.type = OGS_GTP2_COMPLETE_REQUEST_MESSAGE_TYPE_TAU_REQUEST;
    crm.data = (uint8_t *)"\x17\x01\x02\x03";
    crm.len = 4;
    size = ogs_gtp2_build_complete_request_message(
            &octet, &crm, buf, sizeof(buf));
    ABTS_INT_EQUAL(tc, 5, size);
    HEX_EQUAL(tc, "01 17010203", octet.data, octet.len);
    size = ogs_gtp2_parse_complete_request_message(&crm2, &octet);
    ABTS_INT_EQUAL(tc, 5, size);
    ABTS_INT_EQUAL(tc, 1, crm2.type);
    ABTS_INT_EQUAL(tc, 4, crm2.len);
    ABTS_TRUE(tc, memcmp(crm.data, crm2.data, 4) == 0);

    /* 8.48 F-Container */
    memset(&fc, 0, sizeof(fc));
    fc.container_type = OGS_GTP2_F_CONTAINER_TYPE_E_UTRAN_TRANSPARENT_CONTAINER;
    fc.data = (uint8_t *)"\x01\x02";
    fc.len = 2;
    size = ogs_gtp2_build_f_container(&octet, &fc, buf, sizeof(buf));
    ABTS_INT_EQUAL(tc, 3, size);
    HEX_EQUAL(tc, "03 0102", octet.data, octet.len);
    ((uint8_t *)octet.data)[0] |= 0xf0; /* spare bits ignored */
    size = ogs_gtp2_parse_f_container(&fc2, &octet);
    ABTS_INT_EQUAL(tc, 3, size);
    ABTS_INT_EQUAL(tc, 3, fc2.container_type);
    ABTS_INT_EQUAL(tc, 2, fc2.len);
    ABTS_TRUE(tc, memcmp(fc.data, fc2.data, 2) == 0);

    /* 8.49 F-Cause : S1-AP cause (one octet) */
    memset(&cause, 0, sizeof(cause));
    cause.cause_type = OGS_GTP2_F_CAUSE_TYPE_RADIO_NETWORK_LAYER;
    cause.value = 16;
    size = ogs_gtp2_build_f_cause(&octet, &cause, buf, sizeof(buf));
    ABTS_INT_EQUAL(tc, 2, size);
    HEX_EQUAL(tc, "00 10", octet.data, octet.len);
    size = ogs_gtp2_parse_f_cause(&cause2, &octet);
    ABTS_INT_EQUAL(tc, 2, size);
    ABTS_INT_EQUAL(tc, 0, cause2.cause_type);
    ABTS_INT_EQUAL(tc, 1, cause2.value_len);
    ABTS_INT_EQUAL(tc, 16, cause2.value);

    /* 8.49 F-Cause : RANAP cause (two octets) */
    memset(&cause, 0, sizeof(cause));
    cause.value_len = 2;
    cause.value = 400;
    size = ogs_gtp2_build_f_cause(&octet, &cause, buf, sizeof(buf));
    ABTS_INT_EQUAL(tc, 3, size);
    HEX_EQUAL(tc, "00 0190", octet.data, octet.len);
    size = ogs_gtp2_parse_f_cause(&cause2, &octet);
    ABTS_INT_EQUAL(tc, 3, size);
    ABTS_INT_EQUAL(tc, 400, cause2.value);

    /* 8.51.3 Target Identification : Macro eNodeB ID */
    memset(&target, 0, sizeof(target));
    target.target_type = OGS_GTP2_TARGET_TYPE_MACRO_ENODEB_ID;
    TEST_PLMN(&target.nas_plmn_id);
    target.enodeb_id = 0x12345;
    target.tac = 1;
    size = ogs_gtp2_build_target_identification(
            &octet, &target, buf, sizeof(buf));
    ABTS_INT_EQUAL(tc, 9, size);
    HEX_EQUAL(tc, "01 99f907 012345 0001", octet.data, octet.len);
    size = ogs_gtp2_parse_target_identification(&target2, &octet);
    ABTS_INT_EQUAL(tc, 9, size);
    ABTS_INT_EQUAL(tc, 1, target2.target_type);
    ABTS_INT_EQUAL(tc, 0x12345, target2.enodeb_id);
    ABTS_INT_EQUAL(tc, 1, target2.tac);
    ABTS_TRUE(tc, memcmp(&target.nas_plmn_id, &target2.nas_plmn_id,
                OGS_PLMN_ID_LEN) == 0);

    /* Macro eNodeB ID is 20 bits */
    target.enodeb_id = 0x100000;
    ABTS_INT_EQUAL(tc, 0, ogs_gtp2_build_target_identification(
            &octet, &target, buf, sizeof(buf)));

    /* 8.51.4 Home eNodeB ID */
    memset(&target, 0, sizeof(target));
    target.target_type = OGS_GTP2_TARGET_TYPE_HOME_ENODEB_ID;
    TEST_PLMN(&target.nas_plmn_id);
    target.enodeb_id = 0xabcdef1;
    target.tac = 0x1234;
    size = ogs_gtp2_build_target_identification(
            &octet, &target, buf, sizeof(buf));
    ABTS_INT_EQUAL(tc, 10, size);
    HEX_EQUAL(tc, "03 99f907 0abcdef1 1234", octet.data, octet.len);
    size = ogs_gtp2_parse_target_identification(&target2, &octet);
    ABTS_INT_EQUAL(tc, 10, size);
    ABTS_INT_EQUAL(tc, 0xabcdef1, target2.enodeb_id);
    ABTS_INT_EQUAL(tc, 0x1234, target2.tac);

    /* 8.51.5 Extended Macro eNodeB ID : long and short */
    memset(&target, 0, sizeof(target));
    target.target_type = OGS_GTP2_TARGET_TYPE_EXTENDED_MACRO_ENODEB_ID;
    TEST_PLMN(&target.nas_plmn_id);
    target.enodeb_id = 0x1fffff;
    target.tac = 7;
    size = ogs_gtp2_build_target_identification(
            &octet, &target, buf, sizeof(buf));
    ABTS_INT_EQUAL(tc, 9, size);
    HEX_EQUAL(tc, "04 99f907 1fffff 0007", octet.data, octet.len);
    size = ogs_gtp2_parse_target_identification(&target2, &octet);
    ABTS_INT_EQUAL(tc, 0, target2.smenb);
    ABTS_INT_EQUAL(tc, 0x1fffff, target2.enodeb_id);

    target.smenb = true;
    target.enodeb_id = 0x3ffff;
    size = ogs_gtp2_build_target_identification(
            &octet, &target, buf, sizeof(buf));
    HEX_EQUAL(tc, "04 99f907 83ffff 0007", octet.data, octet.len);
    size = ogs_gtp2_parse_target_identification(&target2, &octet);
    ABTS_INT_EQUAL(tc, 1, target2.smenb);
    ABTS_INT_EQUAL(tc, 0x3ffff, target2.enodeb_id);

    /* Non E-UTRAN target type : only the type is decoded */
    octet.data = ogs_hex_from_string("05 99f907 18 00001234 000001",
            hexbuf, sizeof(hexbuf));
    octet.len = 12;
    size = ogs_gtp2_parse_target_identification(&target2, &octet);
    ABTS_INT_EQUAL(tc, 12, size);
    ABTS_INT_EQUAL(tc, OGS_GTP2_TARGET_TYPE_GNODEB_ID, target2.target_type);
}

/* 7.3.5 Context Request : build, compare, parse */
static void gtp_s10_test_context_request(abts_case *tc, void *data)
{
    static ogs_gtp2_context_request_t req;
    ogs_gtp2_guti_t guti;
    ogs_gtp2_f_teid_t f_teid;
    ogs_gtp2_complete_request_message_t crm;
    uint8_t guti_buf[OGS_GTP2_GUTI_LEN];
    uint8_t crm_buf[16];
    ogs_pkbuf_t *pkbuf = NULL;
    int rv;

    const char *expected =
        /* GUTI : type 117, length 10 */
        "75000a00" "99f907000201c0000001"
        /* Complete Request Message : type 116, length 5 */
        "74000500" "0117010203"
        /* F-TEID S10 MME GTP-C, 127.0.0.2, TEID 1 */
        "57000900" "8c00000001" "7f000002"
        /* RAT Type E-UTRAN */
        "52000100" "06";

    memset(&req, 0, sizeof(req));

    memset(&guti, 0, sizeof(guti));
    TEST_PLMN(&guti.nas_plmn_id);
    guti.mme_gid = 2;
    guti.mme_code = 1;
    guti.m_tmsi = 0xc0000001;
    req.guti.presence = 1;
    ogs_gtp2_build_guti(&req.guti, &guti, guti_buf, sizeof(guti_buf));

    memset(&crm, 0, sizeof(crm));
    crm.type = OGS_GTP2_COMPLETE_REQUEST_MESSAGE_TYPE_TAU_REQUEST;
    crm.data = (uint8_t *)"\x17\x01\x02\x03";
    crm.len = 4;
    req.complete_tau_request_message.presence = 1;
    ogs_gtp2_build_complete_request_message(
            &req.complete_tau_request_message, &crm, crm_buf, sizeof(crm_buf));

    memset(&f_teid, 0, sizeof(f_teid));
    f_teid.ipv4 = 1;
    f_teid.interface_type = OGS_GTP2_F_TEID_S10_MME_GTP_C;
    f_teid.teid = htobe32(1);
    f_teid.addr = htobe32(0x7f000002);
    req.s3_s16_s10_n26_address_and_teid_for_control_plane.presence = 1;
    req.s3_s16_s10_n26_address_and_teid_for_control_plane.data = &f_teid;
    req.s3_s16_s10_n26_address_and_teid_for_control_plane.len =
        OGS_GTP2_F_TEID_IPV4_LEN;

    req.rat_type.presence = 1;
    req.rat_type.u8 = OGS_GTP2_RAT_TYPE_EUTRAN;

    pkbuf = ogs_tlv_build_msg(&ogs_gtp2_tlv_desc_context_request,
            &req, OGS_TLV_MODE_T1_L2_I1);
    ABTS_PTR_NOTNULL(tc, pkbuf);
    HEX_EQUAL(tc, expected, pkbuf->data, pkbuf->len);

    memset(&req, 0, sizeof(req));
    rv = ogs_tlv_parse_msg(&req, &ogs_gtp2_tlv_desc_context_request,
            pkbuf, OGS_TLV_MODE_T1_L2_I1);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    ABTS_INT_EQUAL(tc, 0, req.imsi.presence);
    ABTS_INT_EQUAL(tc, 1, req.guti.presence);
    memset(&guti, 0, sizeof(guti));
    ABTS_INT_EQUAL(tc, OGS_GTP2_GUTI_LEN,
            ogs_gtp2_parse_guti(&guti, &req.guti));
    ABTS_INT_EQUAL(tc, 2, guti.mme_gid);
    ABTS_INT_EQUAL(tc, 1, guti.mme_code);
    ABTS_INT_EQUAL(tc, 0xc0000001, guti.m_tmsi);

    ABTS_INT_EQUAL(tc, 1, req.complete_tau_request_message.presence);
    ABTS_INT_EQUAL(tc, 5, ogs_gtp2_parse_complete_request_message(
                &crm, &req.complete_tau_request_message));
    ABTS_INT_EQUAL(tc, 1, crm.type);

    ABTS_INT_EQUAL(tc, 1,
        req.s3_s16_s10_n26_address_and_teid_for_control_plane.presence);
    ABTS_INT_EQUAL(tc, 1, req.rat_type.presence);
    ABTS_INT_EQUAL(tc, OGS_GTP2_RAT_TYPE_EUTRAN, req.rat_type.u8);
    ABTS_INT_EQUAL(tc, 0, req.indication.presence);

    ogs_pkbuf_free(pkbuf);
}

/*
 * 7.3.6 Context Response : several PDN Connections, each with several
 * Bearer Contexts (repeated IEs inside a grouped IE).
 */
static void gtp_s10_test_context_response(abts_case *tc, void *data)
{
    static ogs_gtp2_context_response_t rsp;
    ogs_gtp2_mm_context_t mm;
    ogs_gtp2_cause_t cause;
    uint8_t mm_buf[256];
    uint8_t ebi[2][2] = { { 5, 6 }, { 7, 8 } };
    const char *apn[2] = { "\x08internet", "\x03ims" };
    ogs_pkbuf_t *pkbuf = NULL;
    int rv, i, j;

    memset(&rsp, 0, sizeof(rsp));

    memset(&cause, 0, sizeof(cause));
    cause.value = OGS_GTP2_CAUSE_REQUEST_ACCEPTED;
    rsp.cause.presence = 1;
    rsp.cause.data = &cause;
    rsp.cause.len = sizeof(cause);

    rsp.imsi.presence = 1;
    rsp.imsi.data = (uint8_t *)"\x99\x09\x07\x00\x00\x00\x00\xf1";
    rsp.imsi.len = 8;

    memset(&mm, 0, sizeof(mm));
    mm.ksi_asme = 1;
    memset(mm.kasme, 0x11, sizeof(mm.kasme));
    rsp.mme_sgsn_amf_ue_mm_context.presence = 1;
    ABTS_TRUE(tc, ogs_gtp2_build_mm_context(
            &rsp.mme_sgsn_amf_ue_mm_context, &mm,
            mm_buf, sizeof(mm_buf)) > 0);

    for (i = 0; i < 2; i++) {
        ogs_gtp2_tlv_pdn_connection_t *pdn =
            &rsp.mme_sgsn_amf_ue_eps_pdn_connections[i];
        pdn->presence = 1;
        pdn->apn.presence = 1;
        pdn->apn.data = (uint8_t *)apn[i];
        pdn->apn.len = strlen(apn[i]);
        pdn->linked_eps_bearer_id.presence = 1;
        pdn->linked_eps_bearer_id.u8 = ebi[i][0];
        for (j = 0; j < 2; j++) {
            pdn->bearer_contexts[j].presence = 1;
            pdn->bearer_contexts[j].eps_bearer_id.presence = 1;
            pdn->bearer_contexts[j].eps_bearer_id.u8 = ebi[i][j];
        }
    }

    pkbuf = ogs_tlv_build_msg(&ogs_gtp2_tlv_desc_context_response,
            &rsp, OGS_TLV_MODE_T1_L2_I1);
    ABTS_PTR_NOTNULL(tc, pkbuf);

    /* The MM Context IE uses the type 107 */
    {
        uint8_t *p = pkbuf->data;
        int pos = 0, found = 0;
        while (pos + 4 <= pkbuf->len) {
            int len = (p[pos+1] << 8) | p[pos+2];
            if (p[pos] == OGS_GTP2_MM_CONTEXT_TYPE) found++;
            pos += 4 + len;
        }
        ABTS_INT_EQUAL(tc, 1, found);
        ABTS_INT_EQUAL(tc, 107, OGS_GTP2_MM_CONTEXT_TYPE);
    }

    memset(&rsp, 0, sizeof(rsp));
    rv = ogs_tlv_parse_msg(&rsp, &ogs_gtp2_tlv_desc_context_response,
            pkbuf, OGS_TLV_MODE_T1_L2_I1);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    ABTS_INT_EQUAL(tc, 1, rsp.cause.presence);
    ABTS_INT_EQUAL(tc, 1, rsp.imsi.presence);
    ABTS_INT_EQUAL(tc, 1, rsp.mme_sgsn_amf_ue_mm_context.presence);
    memset(&mm, 0, sizeof(mm));
    ABTS_TRUE(tc, ogs_gtp2_parse_mm_context(
                &mm, &rsp.mme_sgsn_amf_ue_mm_context) > 0);
    ABTS_INT_EQUAL(tc, 1, mm.ksi_asme);

    for (i = 0; i < 2; i++) {
        ogs_gtp2_tlv_pdn_connection_t *pdn =
            &rsp.mme_sgsn_amf_ue_eps_pdn_connections[i];
        ABTS_INT_EQUAL(tc, 1, pdn->presence);
        ABTS_INT_EQUAL(tc, strlen(apn[i]), pdn->apn.len);
        ABTS_TRUE(tc, memcmp(apn[i], pdn->apn.data, pdn->apn.len) == 0);
        ABTS_INT_EQUAL(tc, ebi[i][0], pdn->linked_eps_bearer_id.u8);
        for (j = 0; j < 2; j++) {
            ABTS_INT_EQUAL(tc, 1, pdn->bearer_contexts[j].presence);
            ABTS_INT_EQUAL(tc, ebi[i][j],
                    pdn->bearer_contexts[j].eps_bearer_id.u8);
        }
        ABTS_INT_EQUAL(tc, 0, pdn->bearer_contexts[2].presence);
    }
    ABTS_INT_EQUAL(tc, 0, rsp.mme_sgsn_amf_ue_eps_pdn_connections[2].presence);

    ogs_pkbuf_free(pkbuf);
}

/* 7.3.1 / 7.3.2 Forward Relocation Request and Response */
static void gtp_s10_test_forward_relocation(abts_case *tc, void *data)
{
    static ogs_gtp2_forward_relocation_request_t req;
    static ogs_gtp2_forward_relocation_response_t rsp;
    ogs_gtp2_f_container_t fc;
    ogs_gtp2_f_cause_t f_cause;
    ogs_gtp2_target_identification_t target;
    ogs_gtp2_cause_t cause;
    uint8_t fc_buf[16], cause_buf[4], target_buf[16];
    ogs_pkbuf_t *pkbuf = NULL;
    int rv;

    memset(&req, 0, sizeof(req));

    memset(&fc, 0, sizeof(fc));
    fc.container_type = OGS_GTP2_F_CONTAINER_TYPE_E_UTRAN_TRANSPARENT_CONTAINER;
    fc.data = (uint8_t *)"\xde\xad\xbe\xef";
    fc.len = 4;
    req.e_utran_transparent_container.presence = 1;
    ogs_gtp2_build_f_container(&req.e_utran_transparent_container,
            &fc, fc_buf, sizeof(fc_buf));

    memset(&f_cause, 0, sizeof(f_cause));
    f_cause.cause_type = OGS_GTP2_F_CAUSE_TYPE_RADIO_NETWORK_LAYER;
    f_cause.value = 16;
    req.s1_ap_cause.presence = 1;
    ogs_gtp2_build_f_cause(&req.s1_ap_cause,
            &f_cause, cause_buf, sizeof(cause_buf));

    memset(&target, 0, sizeof(target));
    target.target_type = OGS_GTP2_TARGET_TYPE_MACRO_ENODEB_ID;
    TEST_PLMN(&target.nas_plmn_id);
    target.enodeb_id = 0x12345;
    target.tac = 2;
    req.target_identification.presence = 1;
    ogs_gtp2_build_target_identification(&req.target_identification,
            &target, target_buf, sizeof(target_buf));

    req.mme_sgsn_amf_ue_eps_pdn_connections[0].presence = 1;
    req.mme_sgsn_amf_ue_eps_pdn_connections[0].
        linked_eps_bearer_id.presence = 1;
    req.mme_sgsn_amf_ue_eps_pdn_connections[0].linked_eps_bearer_id.u8 = 5;

    pkbuf = ogs_tlv_build_msg(&ogs_gtp2_tlv_desc_forward_relocation_request,
            &req, OGS_TLV_MODE_T1_L2_I1);
    ABTS_PTR_NOTNULL(tc, pkbuf);

    memset(&req, 0, sizeof(req));
    rv = ogs_tlv_parse_msg(&req, &ogs_gtp2_tlv_desc_forward_relocation_request,
            pkbuf, OGS_TLV_MODE_T1_L2_I1);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    ABTS_INT_EQUAL(tc, 1, req.e_utran_transparent_container.presence);
    ABTS_INT_EQUAL(tc, 0, req.utran_transparent_container.presence);
    ABTS_INT_EQUAL(tc, 5, ogs_gtp2_parse_f_container(
                &fc, &req.e_utran_transparent_container));
    ABTS_INT_EQUAL(tc, 3, fc.container_type);
    ABTS_TRUE(tc, memcmp("\xde\xad\xbe\xef", fc.data, 4) == 0);

    ABTS_INT_EQUAL(tc, 1, req.s1_ap_cause.presence);
    ABTS_INT_EQUAL(tc, 0, req.ranap_cause.presence);
    ABTS_INT_EQUAL(tc, 2, ogs_gtp2_parse_f_cause(&f_cause, &req.s1_ap_cause));
    ABTS_INT_EQUAL(tc, 16, f_cause.value);

    ABTS_INT_EQUAL(tc, 1, req.target_identification.presence);
    ABTS_INT_EQUAL(tc, 9, ogs_gtp2_parse_target_identification(
                &target, &req.target_identification));
    ABTS_INT_EQUAL(tc, 0x12345, target.enodeb_id);
    ABTS_INT_EQUAL(tc, 2, target.tac);

    ABTS_INT_EQUAL(tc, 1, req.mme_sgsn_amf_ue_eps_pdn_connections[0].presence);
    ABTS_INT_EQUAL(tc, 0, req.mme_sgsn_amf_ue_eps_pdn_connections[1].presence);

    ogs_pkbuf_free(pkbuf);

    /* Forward Relocation Response with two set-up bearers */
    memset(&rsp, 0, sizeof(rsp));
    memset(&cause, 0, sizeof(cause));
    cause.value = OGS_GTP2_CAUSE_REQUEST_ACCEPTED;
    rsp.cause.presence = 1;
    rsp.cause.data = &cause;
    rsp.cause.len = sizeof(cause);
    rsp.list_of_set_up_bearers[0].presence = 1;
    rsp.list_of_set_up_bearers[0].eps_bearer_id.presence = 1;
    rsp.list_of_set_up_bearers[0].eps_bearer_id.u8 = 5;
    rsp.list_of_set_up_bearers[1].presence = 1;
    rsp.list_of_set_up_bearers[1].eps_bearer_id.presence = 1;
    rsp.list_of_set_up_bearers[1].eps_bearer_id.u8 = 6;

    pkbuf = ogs_tlv_build_msg(&ogs_gtp2_tlv_desc_forward_relocation_response,
            &rsp, OGS_TLV_MODE_T1_L2_I1);
    ABTS_PTR_NOTNULL(tc, pkbuf);

    memset(&rsp, 0, sizeof(rsp));
    rv = ogs_tlv_parse_msg(&rsp,
            &ogs_gtp2_tlv_desc_forward_relocation_response,
            pkbuf, OGS_TLV_MODE_T1_L2_I1);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    ABTS_INT_EQUAL(tc, 1, rsp.list_of_set_up_bearers[0].presence);
    ABTS_INT_EQUAL(tc, 5, rsp.list_of_set_up_bearers[0].eps_bearer_id.u8);
    ABTS_INT_EQUAL(tc, 1, rsp.list_of_set_up_bearers[1].presence);
    ABTS_INT_EQUAL(tc, 6, rsp.list_of_set_up_bearers[1].eps_bearer_id.u8);
    ABTS_INT_EQUAL(tc, 0, rsp.list_of_set_up_bearers[2].presence);

    ogs_pkbuf_free(pkbuf);
}

/*
 * 7.3.10 Forward Access Context Notification : two F-Containers with
 * the same name and different instances.
 * 7.3.18 Configuration Transfer Tunnel.
 */
static void gtp_s10_test_containers(abts_case *tc, void *data)
{
    static ogs_gtp2_forward_access_context_notification_t noti;
    static ogs_gtp2_configuration_transfer_tunnel_t tunnel;
    ogs_gtp2_f_container_t fc;
    ogs_gtp2_target_identification_t target;
    uint8_t buf0[8], buf1[8], target_buf[16];
    ogs_pkbuf_t *pkbuf = NULL;
    int rv;

    memset(&noti, 0, sizeof(noti));
    memset(&fc, 0, sizeof(fc));
    fc.container_type = OGS_GTP2_F_CONTAINER_TYPE_E_UTRAN_TRANSPARENT_CONTAINER;
    fc.data = (uint8_t *)"\x00";
    fc.len = 1;
    noti.e_utran_transparent_container.presence = 1;
    ogs_gtp2_build_f_container(&noti.e_utran_transparent_container,
            &fc, buf0, sizeof(buf0));
    fc.data = (uint8_t *)"\x01";
    noti.e_utran_early_status_transparent_container.presence = 1;
    ogs_gtp2_build_f_container(
            &noti.e_utran_early_status_transparent_container,
            &fc, buf1, sizeof(buf1));

    pkbuf = ogs_tlv_build_msg(
            &ogs_gtp2_tlv_desc_forward_access_context_notification,
            &noti, OGS_TLV_MODE_T1_L2_I1);
    ABTS_PTR_NOTNULL(tc, pkbuf);
    /* F-Container type 118, instance 0 then instance 1 */
    HEX_EQUAL(tc, "76000200 0300" "76000201 0301", pkbuf->data, pkbuf->len);

    memset(&noti, 0, sizeof(noti));
    rv = ogs_tlv_parse_msg(&noti,
            &ogs_gtp2_tlv_desc_forward_access_context_notification,
            pkbuf, OGS_TLV_MODE_T1_L2_I1);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    ABTS_INT_EQUAL(tc, 1, noti.e_utran_transparent_container.presence);
    ABTS_INT_EQUAL(tc, 1,
            noti.e_utran_early_status_transparent_container.presence);
    ABTS_INT_EQUAL(tc, 0,
            ((uint8_t *)noti.e_utran_transparent_container.data)[1]);
    ABTS_INT_EQUAL(tc, 1, ((uint8_t *)
            noti.e_utran_early_status_transparent_container.data)[1]);
    ogs_pkbuf_free(pkbuf);

    memset(&tunnel, 0, sizeof(tunnel));
    fc.data = (uint8_t *)"\xaa\xbb";
    fc.len = 2;
    tunnel.e_utran_transparent_container.presence = 1;
    ogs_gtp2_build_f_container(&tunnel.e_utran_transparent_container,
            &fc, buf0, sizeof(buf0));
    memset(&target, 0, sizeof(target));
    target.target_type = OGS_GTP2_TARGET_TYPE_MACRO_ENODEB_ID;
    TEST_PLMN(&target.nas_plmn_id);
    target.enodeb_id = 1;
    target.tac = 3;
    tunnel.target_enodeb_id.presence = 1;
    ogs_gtp2_build_target_identification(&tunnel.target_enodeb_id,
            &target, target_buf, sizeof(target_buf));

    pkbuf = ogs_tlv_build_msg(&ogs_gtp2_tlv_desc_configuration_transfer_tunnel,
            &tunnel, OGS_TLV_MODE_T1_L2_I1);
    ABTS_PTR_NOTNULL(tc, pkbuf);
    HEX_EQUAL(tc, "76000300 03aabb" "79000900 01 99f907 000001 0003",
            pkbuf->data, pkbuf->len);

    memset(&tunnel, 0, sizeof(tunnel));
    rv = ogs_tlv_parse_msg(&tunnel,
            &ogs_gtp2_tlv_desc_configuration_transfer_tunnel,
            pkbuf, OGS_TLV_MODE_T1_L2_I1);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    ABTS_INT_EQUAL(tc, 1, tunnel.e_utran_transparent_container.presence);
    ABTS_INT_EQUAL(tc, 1, tunnel.target_enodeb_id.presence);
    ABTS_INT_EQUAL(tc, 0, tunnel.connected_target_enodeb_id.presence);
    ogs_pkbuf_free(pkbuf);
}

/* Message type values (TS 29.274 Table 6.1-1) and full header encoding */
static void gtp_s10_test_message_types(abts_case *tc, void *data)
{
    static ogs_gtp2_message_t message;
    ogs_gtp2_cause_t cause;
    ogs_pkbuf_t *pkbuf = NULL;

    ABTS_INT_EQUAL(tc, 128, OGS_GTP2_IDENTIFICATION_REQUEST_TYPE);
    ABTS_INT_EQUAL(tc, 129, OGS_GTP2_IDENTIFICATION_RESPONSE_TYPE);
    ABTS_INT_EQUAL(tc, 130, OGS_GTP2_CONTEXT_REQUEST_TYPE);
    ABTS_INT_EQUAL(tc, 131, OGS_GTP2_CONTEXT_RESPONSE_TYPE);
    ABTS_INT_EQUAL(tc, 132, OGS_GTP2_CONTEXT_ACKNOWLEDGE_TYPE);
    ABTS_INT_EQUAL(tc, 133, OGS_GTP2_FORWARD_RELOCATION_REQUEST_TYPE);
    ABTS_INT_EQUAL(tc, 134, OGS_GTP2_FORWARD_RELOCATION_RESPONSE_TYPE);
    ABTS_INT_EQUAL(tc, 135,
            OGS_GTP2_FORWARD_RELOCATION_COMPLETE_NOTIFICATION_TYPE);
    ABTS_INT_EQUAL(tc, 136,
            OGS_GTP2_FORWARD_RELOCATION_COMPLETE_ACKNOWLEDGE_TYPE);
    ABTS_INT_EQUAL(tc, 137, OGS_GTP2_FORWARD_ACCESS_CONTEXT_NOTIFICATION_TYPE);
    ABTS_INT_EQUAL(tc, 138, OGS_GTP2_FORWARD_ACCESS_CONTEXT_ACKNOWLEDGE_TYPE);
    ABTS_INT_EQUAL(tc, 139, OGS_GTP2_RELOCATION_CANCEL_REQUEST_TYPE);
    ABTS_INT_EQUAL(tc, 140, OGS_GTP2_RELOCATION_CANCEL_RESPONSE_TYPE);
    ABTS_INT_EQUAL(tc, 141, OGS_GTP2_CONFIGURATION_TRANSFER_TUNNEL_TYPE);

    /* Generic build and parse go through the S10 message switch */
    memset(&message, 0, sizeof(message));
    message.h.type = OGS_GTP2_RELOCATION_CANCEL_RESPONSE_TYPE;
    memset(&cause, 0, sizeof(cause));
    cause.value = OGS_GTP2_CAUSE_REQUEST_ACCEPTED;
    message.relocation_cancel_response.cause.presence = 1;
    message.relocation_cancel_response.cause.data = &cause;
    message.relocation_cancel_response.cause.len = sizeof(cause);

    pkbuf = ogs_gtp2_build_msg(&message);
    ABTS_PTR_NOTNULL(tc, pkbuf);
    HEX_EQUAL(tc, "02000200 1000", pkbuf->data, pkbuf->len);
    ogs_pkbuf_free(pkbuf);
}

abts_suite *test_gtp_s10(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, gtp_s10_test_mm_context_minimal, NULL);
    abts_run_test(suite, gtp_s10_test_mm_context_full, NULL);
    abts_run_test(suite, gtp_s10_test_mm_context_robustness, NULL);
    abts_run_test(suite, gtp_s10_test_simple_ies, NULL);
    abts_run_test(suite, gtp_s10_test_context_request, NULL);
    abts_run_test(suite, gtp_s10_test_context_response, NULL);
    abts_run_test(suite, gtp_s10_test_forward_relocation, NULL);
    abts_run_test(suite, gtp_s10_test_containers, NULL);
    abts_run_test(suite, gtp_s10_test_message_types, NULL);

    return suite;
}
