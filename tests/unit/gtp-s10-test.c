/*
 * Copyright (C) 2026 by Open5GS contributors
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#include "ogs-gtp.h"
#include "core/abts.h"

static void eps_known_encoding(abts_case *tc, void *data)
{
    /* Independently encoded TS 29.274 Figure 8.38-5 vector: mode 4,
     * KSI 3, EIA2/EEA1, no optional fields and no unused vectors. */
    static const uint8_t wire[] = {
        0x83, 0x00, 0x21, 0x01, 0x02, 0x03, 0xfe, 0xfd, 0x01,
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
        16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
        0, 0, 0, 0
    };
    ogs_gtp2_eps_mm_context_t c = {0};
    ogs_tlv_octet_t ie = {0}, built = {0};
    uint8_t out[sizeof(wire)];
    ie.data = (void *)wire;
    ie.len = sizeof(wire);
    ABTS_INT_EQUAL(tc, OGS_OK, ogs_gtp2_parse_eps_mm_context(&c, &ie));
    ABTS_INT_EQUAL(tc, 3, c.ksi);
    ABTS_INT_EQUAL(tc, 2, c.integrity_algorithm);
    ABTS_INT_EQUAL(tc, 1, c.cipher_algorithm);
    ABTS_INT_EQUAL(tc, 0x010203, c.dl_count);
    ABTS_INT_EQUAL(tc, 0xfefd01, c.ul_count);
    ABTS_INT_EQUAL(tc, 31, c.kasme[31]);
    ABTS_INT_EQUAL(tc, OGS_OK,
            ogs_gtp2_build_eps_mm_context(&built, &c, out, sizeof(out)));
    ABTS_INT_EQUAL(tc, sizeof(wire), built.len);
    ABTS_TRUE(tc, memcmp(wire, out, sizeof(wire)) == 0);
}

static void eps_optional_and_truncated(abts_case *tc, void *data)
{
    ogs_gtp2_eps_mm_context_t c = {0}, parsed, unchanged;
    ogs_tlv_octet_t ie = {0}, short_ie;
    uint8_t out[1024], again[1024], caps[] = {0xf0, 0xf0, 0x80};
    size_t i;
    c.ksi = 6;
    c.integrity_algorithm = 2;
    c.cipher_algorithm = 2;
    c.dl_count = 0xffffff;
    c.ul_count = 0xfffeff;
    memset(c.kasme, 0x11, sizeof(c.kasme));
    c.num_vectors = 1;
    c.vector[0].xres_len = 8;
    c.vector[0].autn_len = 16;
    memset(c.vector[0].rand, 0x22, 16);
    memset(c.vector[0].xres, 0x33, 8);
    memset(c.vector[0].autn, 0x44, 16);
    memset(c.vector[0].kasme, 0x55, 32);
    c.drx_present = true;
    c.drx[0] = 0x80;
    c.drx[1] = 0x12;
    c.nh_present = true;
    memset(c.nh, 0x66, 32);
    c.ncc = 7;
    c.subscribed_ambr_present = c.used_ambr_present = true;
    c.subscribed_ambr_ul = 0x01020304;
    c.subscribed_ambr_dl = 0xfedcba98;
    c.used_ambr_ul = 10000;
    c.used_ambr_dl = 20000;
    c.ue_network_capability.data = caps;
    c.ue_network_capability.len = sizeof(caps);
    c.ms_network_capability.data = caps;
    c.ms_network_capability.len = 1;
    c.mei.data = (void *)"12345678";
    c.mei.len = 8;
    c.access_restriction = 0x81;
    c.old_context_present = c.old_nh_present = true;
    c.old_ksi = 5;
    c.old_ncc = 4;
    memset(c.old_kasme, 0x77, 32);
    memset(c.old_nh, 0x88, 32);
    ABTS_INT_EQUAL(tc, OGS_OK,
            ogs_gtp2_build_eps_mm_context(&ie, &c, out, sizeof(out)));
    memset(&unchanged, 0xa5, sizeof(unchanged));
    for (i = 0; i < ie.len; i++) {
        parsed = unchanged;
        short_ie = ie;
        short_ie.len = i;
        ABTS_INT_EQUAL(tc, OGS_ERROR,
                ogs_gtp2_parse_eps_mm_context(&parsed, &short_ie));
        ABTS_TRUE(tc, memcmp(&unchanged, &parsed, sizeof(parsed)) == 0);
    }
    ABTS_INT_EQUAL(tc, OGS_OK, ogs_gtp2_parse_eps_mm_context(&parsed, &ie));
    ABTS_INT_EQUAL(tc, c.ul_count, parsed.ul_count);
    ABTS_INT_EQUAL(tc, c.subscribed_ambr_dl, parsed.subscribed_ambr_dl);
    ABTS_INT_EQUAL(tc, 4, parsed.old_ncc);
    ABTS_TRUE(tc, memcmp(c.vector[0].kasme, parsed.vector[0].kasme, 32) == 0);
    ABTS_INT_EQUAL(tc, OGS_OK,
            ogs_gtp2_build_eps_mm_context(&short_ie, &parsed, again, sizeof(again)));
    ABTS_INT_EQUAL(tc, ie.len, short_ie.len);
    ABTS_TRUE(tc, memcmp(out, again, ie.len) == 0);
    memset(again, 0xa5, sizeof(again));
    ABTS_INT_EQUAL(tc, OGS_ERROR,
            ogs_gtp2_build_eps_mm_context(&short_ie, &parsed, again, ie.len - 1));
    for (i = 0; i < sizeof(again); i++) ABTS_INT_EQUAL(tc, 0xa5, again[i]);
}

static void eps_invalid_and_extensions(abts_case *tc, void *data)
{
    ogs_gtp2_eps_mm_context_t c = {0}, parsed;
    ogs_tlv_octet_t ie = {0};
    uint8_t out[256], ext[] = {0, 0, 2, 0xab, 0xcd};
    c.extensions.data = ext;
    c.extensions.len = sizeof(ext);
    ABTS_INT_EQUAL(tc, OGS_OK,
            ogs_gtp2_build_eps_mm_context(&ie, &c, out, sizeof(out)));
    ABTS_INT_EQUAL(tc, OGS_OK, ogs_gtp2_parse_eps_mm_context(&parsed, &ie));
    ABTS_INT_EQUAL(tc, sizeof(ext), parsed.extensions.len);
    ABTS_TRUE(tc, memcmp(ext, parsed.extensions.data, sizeof(ext)) == 0);
    /* A paging capability length of two followed by only one octet. */
    ie.len--;
    ABTS_INT_EQUAL(tc, OGS_ERROR, ogs_gtp2_parse_eps_mm_context(&parsed, &ie));
    ie.len++;
    out[0] = 0x60; /* UMTS context is not an EPS context. */
    ABTS_INT_EQUAL(tc, OGS_ERROR, ogs_gtp2_parse_eps_mm_context(&parsed, &ie));
    out[0] = 0x80;
    out[1] = 0x20; /* Quintuplets must not cross S10. */
    ABTS_INT_EQUAL(tc, OGS_ERROR, ogs_gtp2_parse_eps_mm_context(&parsed, &ie));
    out[1] = 6 << 2;
    ABTS_INT_EQUAL(tc, OGS_ERROR, ogs_gtp2_parse_eps_mm_context(&parsed, &ie));
    c.dl_count = 0x1000000;
    ABTS_INT_EQUAL(tc, OGS_ERROR,
            ogs_gtp2_build_eps_mm_context(&ie, &c, out, sizeof(out)));
    c.dl_count = 0;
    c.ue_network_capability.len = 256;
    ABTS_INT_EQUAL(tc, OGS_ERROR,
            ogs_gtp2_build_eps_mm_context(&ie, &c, out, sizeof(out)));
    c.ue_network_capability.len = 0;
    c.extensions.len = UINT32_MAX;
    ABTS_INT_EQUAL(tc, OGS_ERROR,
            ogs_gtp2_build_eps_mm_context(&ie, &c, out, sizeof(out)));
    ie.len = UINT32_MAX;
    ABTS_INT_EQUAL(tc, OGS_ERROR, ogs_gtp2_parse_eps_mm_context(&parsed, &ie));
}

static void identification_wire(abts_case *tc, void *data)
{
    /* TS 29.274 7.3.8: GUTI type 117, 10-octet value, instance 0;
     * complete request type 116, Attach discriminator 0. */
    uint8_t wire[] = {117,0,10,0, 0x00,0xf1,0x10,0,2,1,0x12,0x34,0x56,0x78,
                     116,0,3,0, 0,0x07,0x41};
    ogs_gtp2_identification_request_t req = {0};
    ogs_pkbuf_t *p = ogs_pkbuf_alloc(NULL, sizeof(wire)), *built;
    ogs_assert(p);
    ogs_pkbuf_put_data(p, wire, sizeof(wire));
    ABTS_INT_EQUAL(tc, OGS_OK,
            ogs_tlv_parse_msg(&req, &ogs_gtp2_tlv_desc_identification_request,
                p, OGS_TLV_MODE_T1_L2_I1));
    ABTS_INT_EQUAL(tc, 10, req.guti.len);
    ABTS_INT_EQUAL(tc, 3, req.complete_attach_request_message.len);
    built = ogs_tlv_build_msg(&ogs_gtp2_tlv_desc_identification_request,
            &req, OGS_TLV_MODE_T1_L2_I1);
    ABTS_PTR_NOTNULL(tc, built);
    if (built) {
        ABTS_INT_EQUAL(tc, sizeof(wire), built->len);
        ABTS_TRUE(tc, memcmp(wire, built->data, sizeof(wire)) == 0);
        ogs_pkbuf_free(built);
    }
    ogs_pkbuf_free(p);
}

static void empty_relocation_complete(abts_case *tc, void *data)
{
    ogs_gtp2_message_t *msg = ogs_calloc(1, sizeof(*msg));
    ogs_pkbuf_t *p;
    ogs_assert(msg);
    msg->h.type = OGS_GTP2_FORWARD_RELOCATION_COMPLETE_NOTIFICATION_TYPE;
    p = ogs_gtp2_build_msg(msg);
    ABTS_PTR_NOTNULL(tc, p);
    if (p) {
        ABTS_INT_EQUAL(tc, 0, p->len);
        ABTS_TRUE(tc, ogs_pkbuf_headroom(p) >= OGS_GTPV2C_HEADER_LEN);
        ogs_pkbuf_free(p);
    }
    ogs_free(msg);
}

static void relocation_forwarding_wire(abts_case *tc, void *data)
{
    /* Independent TS 29.274 table 7.3.2-2 vector: EBI plus all six
     * forwarding F-TEID instances. These are not Create Session instances. */
    uint8_t wire[] = {
        93, 0, 83, 0, 73, 0, 1, 0, 5,
        87, 0, 9, 0, 0x80, 0,0,0,1, 192,0,2,1,
        87, 0, 9, 1, 0x80, 0,0,0,2, 192,0,2,2,
        87, 0, 9, 2, 0x80, 0,0,0,3, 192,0,2,3,
        87, 0, 9, 3, 0x80, 0,0,0,4, 192,0,2,4,
        87, 0, 9, 4, 0x80, 0,0,0,5, 192,0,2,5,
        87, 0, 9, 5, 0x80, 0,0,0,6, 192,0,2,6
    };
    ogs_gtp2_forward_relocation_response_t *rsp = ogs_calloc(1, sizeof(*rsp));
    ogs_gtp2_tlv_relocation_bearer_context_t *bc = &rsp->list_of_set_up_bearers[0];
    ogs_pkbuf_t *p = ogs_pkbuf_alloc(NULL, sizeof(wire)), *built;
    ogs_tlv_octet_t *endpoints[] = {&bc->enb_dl_forwarding_f_teid,
        &bc->enb_ul_forwarding_f_teid, &bc->sgw_dl_forwarding_f_teid,
        &bc->rnc_dl_forwarding_f_teid, &bc->sgsn_dl_forwarding_f_teid,
        &bc->sgw_ul_forwarding_f_teid};
    size_t i;
    ogs_assert(rsp && p);
    ogs_pkbuf_put_data(p, wire, sizeof(wire));
    ABTS_INT_EQUAL(tc, OGS_OK, ogs_tlv_parse_msg(rsp,
                &ogs_gtp2_tlv_desc_forward_relocation_response,
                p, OGS_TLV_MODE_T1_L2_I1));
    ABTS_INT_EQUAL(tc, 5, bc->eps_bearer_id.u8);
    for (i = 0; i < OGS_ARRAY_SIZE(endpoints); i++) {
        ABTS_TRUE(tc, endpoints[i]->presence);
        ABTS_INT_EQUAL(tc, 9, endpoints[i]->len);
        if (endpoints[i]->data)
            ABTS_INT_EQUAL(tc, i + 1, ((uint8_t *)endpoints[i]->data)[4]);
    }
    built = ogs_tlv_build_msg(&ogs_gtp2_tlv_desc_forward_relocation_response,
            rsp, OGS_TLV_MODE_T1_L2_I1);
    ABTS_PTR_NOTNULL(tc, built);
    if (built) {
        ABTS_INT_EQUAL(tc, sizeof(wire), built->len);
        ABTS_TRUE(tc, !memcmp(wire, built->data, sizeof(wire)));
        ogs_pkbuf_free(built);
    }
    ogs_pkbuf_free(p);
    ogs_free(rsp);
}

static void multiple_pdn_and_bearer(abts_case *tc, void *data)
{
    ogs_gtp2_context_response_t *src = ogs_calloc(1, sizeof(*src));
    ogs_gtp2_context_response_t *dst = ogs_calloc(1, sizeof(*dst));
    ogs_pkbuf_t *p;
    unsigned int i, j;
    ogs_assert(src && dst);
    for (i = 0; i < OGS_MAX_NUM_OF_SESS; i++) {
        ogs_gtp2_tlv_pdn_connection_t *pdn = &src->mme_sgsn_amf_ue_eps_pdn_connections[i];
        pdn->presence = 1;
        pdn->apn.presence = 1;
        pdn->apn.data = (void *)(i ? "\3ims" : "\10internet");
        pdn->apn.len = i ? 4 : 9;
        for (j = 0; j < 2; j++) {
            pdn->bearer_contexts[j].presence = 1;
            pdn->bearer_contexts[j].eps_bearer_id.presence = 1;
            pdn->bearer_contexts[j].eps_bearer_id.u8 = 5 + i * 2 + j;
        }
    }
    p = ogs_tlv_build_msg(&ogs_gtp2_tlv_desc_context_response,
            src, OGS_TLV_MODE_T1_L2_I1);
    ABTS_PTR_NOTNULL(tc, p);
    if (p) {
        ABTS_INT_EQUAL(tc, OGS_OK,
                ogs_tlv_parse_msg(dst, &ogs_gtp2_tlv_desc_context_response,
                    p, OGS_TLV_MODE_T1_L2_I1));
        for (i = 0; i < OGS_MAX_NUM_OF_SESS; i++) {
            ogs_gtp2_tlv_pdn_connection_t *pdn = &dst->mme_sgsn_amf_ue_eps_pdn_connections[i];
            ABTS_TRUE(tc, pdn->presence);
            for (j = 0; j < 2; j++)
                ABTS_INT_EQUAL(tc, 5 + i * 2 + j,
                        pdn->bearer_contexts[j].eps_bearer_id.u8);
            ABTS_TRUE(tc, !pdn->bearer_contexts[2].presence);
        }
        ogs_pkbuf_free(p);
    }
    ogs_free(src);
    ogs_free(dst);
}

static void pdn_bearer_wire(abts_case *tc, void *data)
{
    /* TS 29.274 7.3.6: SGW S1-U is F-TEID instance 0, PGW S5/S8-U
     * is instance 1. Create Session uses instance 3 for the latter. */
    uint8_t wire[] = {
        93,0,31,0,
        73,0,1,0,5,
        87,0,9,0,0x81,0,0,0,1,127,0,0,4,
        87,0,9,1,0x85,0,0,0,2,127,0,0,5
    };
    ogs_gtp2_tlv_eps_pdn_bearer_context_t bearer = {0};
    ogs_tlv_desc_t root = {
        OGS_TLV_MESSAGE, "PDN bearer vector", 0, 0, 0, sizeof(bearer),
        {&ogs_gtp2_tlv_desc_eps_pdn_bearer_context_0, NULL}
    };
    ogs_pkbuf_t *p = ogs_pkbuf_alloc(NULL, sizeof(wire)), *built;
    ogs_pkbuf_put_data(p, wire, sizeof(wire));
    ABTS_INT_EQUAL(tc, OGS_OK, ogs_tlv_parse_msg(&bearer,
                &root, p,
                OGS_TLV_MODE_T1_L2_I1));
    ABTS_TRUE(tc, bearer.s1_u_sgw_f_teid.presence);
    ABTS_TRUE(tc, bearer.s5_s8_u_pgw_f_teid.presence);
    ABTS_INT_EQUAL(tc, 9, bearer.s5_s8_u_pgw_f_teid.len);
    if (bearer.s5_s8_u_pgw_f_teid.data)
        ABTS_INT_EQUAL(tc, 0x85,
                ((uint8_t *)bearer.s5_s8_u_pgw_f_teid.data)[0]);
    built = ogs_tlv_build_msg(&root,
            &bearer, OGS_TLV_MODE_T1_L2_I1);
    ABTS_PTR_NOTNULL(tc, built);
    if (built) {
        ABTS_INT_EQUAL(tc, sizeof(wire), built->len);
        if (built->len == sizeof(wire))
            ABTS_TRUE(tc, !memcmp(wire, built->data, sizeof(wire)));
        ogs_pkbuf_free(built);
    }
    ogs_pkbuf_free(p);
}

static void s10_message_dispatch(abts_case *tc, void *data)
{
    /* Message numbers and the first IE are taken from TS 29.274 6.1 and
     * 7.3, independently of the generated message constants/descriptors. */
    static const struct {
        uint8_t type;
        uint8_t body[14];
        size_t len;
    } vectors[] = {
        {128, {117,0,10,0, 0,0xf1,0x10,0,2,1,0x12,0x34,0x56,0x78}, 14},
        {129, {2,0,2,0,16,0}, 6},
        {130, {117,0,10,0, 0,0xf1,0x10,0,2,1,0x12,0x34,0x56,0x78}, 14},
        {131, {2,0,2,0,16,0}, 6},
        {132, {2,0,2,0,16,0}, 6},
        {133, {1,0,8,0,0,0x01,0x21,0x43,0x65,0x87,0x09,0xf1}, 12},
        {134, {2,0,2,0,16,0}, 6},
        {135, {77,0,1,0,0}, 5},
        {136, {2,0,2,0,16,0}, 6},
        {137, {118,0,2,0,3,0xab}, 6},
        {138, {2,0,2,0,16,0}, 6},
        {139, {1,0,8,0,0,0x01,0x21,0x43,0x65,0x87,0x09,0xf1}, 12},
        {140, {2,0,2,0,16,0}, 6},
        {141, {118,0,2,0,3,0xab}, 6}
    };
    ogs_gtp2_message_t *message = ogs_calloc(1, sizeof(*message));
    unsigned int i;
    ogs_assert(message);
    for (i = 0; i < OGS_ARRAY_SIZE(vectors); i++) {
        uint8_t h[12] = {0x48, 0, 0, 0, 0x12,0x34,0x56,0x78, 0,0,1,0};
        ogs_pkbuf_t *p = ogs_pkbuf_alloc(NULL, sizeof(h) + vectors[i].len);
        ogs_pkbuf_t *built;
        ogs_assert(p);
        h[1] = vectors[i].type;
        h[3] = 8 + vectors[i].len;
        ogs_pkbuf_put_data(p, h, sizeof(h));
        ogs_pkbuf_put_data(p, vectors[i].body, vectors[i].len);
        ABTS_INT_EQUAL(tc, OGS_OK, ogs_gtp2_parse_msg(message, p));
        ABTS_INT_EQUAL(tc, vectors[i].type, message->h.type);
        ABTS_INT_EQUAL(tc, 0x12345678, message->h.teid);
        built = ogs_gtp2_build_msg(message);
        ABTS_PTR_NOTNULL(tc, built);
        if (built) {
            ABTS_INT_EQUAL(tc, vectors[i].len, built->len);
            ABTS_TRUE(tc, memcmp(vectors[i].body, built->data, built->len) == 0);
            ogs_pkbuf_free(built);
        }
        ogs_pkbuf_free(p);
    }
    ogs_free(message);
}

static ogs_pkbuf_t *transaction_payload(void)
{
    ogs_pkbuf_t *p = ogs_pkbuf_alloc(NULL, 64);
    ogs_assert(p);
    ogs_pkbuf_reserve(p, OGS_GTPV2C_HEADER_LEN);
    ogs_pkbuf_put_u8(p, 0);
    return p;
}

static void s10_transaction_stages(abts_case *tc, void *data)
{
    static const uint8_t pairs[][2] = {
        {128,129}, {133,134}, {135,136}, {137,138}, {139,140}
    };
    ogs_gtp_node_t node = {0}, old_mme = {0};
    ogs_gtp2_header_t header = {0};
    ogs_gtp_xact_t *xact, *received, *remote;
    ogs_timer_mgr_t *saved_timers = ogs_app()->timer_mgr;
    uint64_t saved_pool = ogs_app()->pool.xact;
    ogs_time_t saved_holding = ogs_local_conf()->time.message.gtp.t3_holding_duration;
    ogs_time_t saved_response = ogs_local_conf()->time.message.gtp.t3_response_duration;
    int saved_retries = ogs_local_conf()->time.message.gtp.n3_holding_rcount;
    ogs_pool_id_t id;
    unsigned int i;

    ogs_app()->pool.xact = 8;
    ogs_app()->timer_mgr = ogs_timer_mgr_create(32);
    ogs_assert(ogs_app()->timer_mgr);
    node.addr.sin.sin_family = old_mme.addr.sin.sin_family = AF_INET;
    ogs_local_conf()->time.message.gtp.t3_holding_duration = ogs_time_from_sec(60);
    ogs_local_conf()->time.message.gtp.t3_response_duration = ogs_time_from_sec(5);
    ogs_local_conf()->time.message.gtp.n3_holding_rcount = 1;
    ABTS_INT_EQUAL(tc, OGS_OK, ogs_gtp_xact_init());
    for (i = 0; i < OGS_ARRAY_SIZE(pairs); i++) {
        header.type = pairs[i][0];
        xact = ogs_gtp_xact_local_create(&node, &header,
                transaction_payload(), NULL, NULL);
        ABTS_PTR_NOTNULL(tc, xact);
        ogs_assert(xact);
        id = xact->id;
        header.type = pairs[i][1];
        header.teid_presence = 1;
        header.sqn = OGS_GTP2_XID_TO_SQN(xact->xid);
        ABTS_INT_EQUAL(tc, OGS_OK,
                ogs_gtp_xact_receive(&node, &header, &received));
        ABTS_TRUE(tc, received == xact);
        ABTS_INT_EQUAL(tc, OGS_OK, ogs_gtp_xact_commit(xact));
        ABTS_TRUE(tc, !ogs_gtp_xact_find_by_id(id));
    }
    /* The Context Request / Response / Acknowledge procedure is three legs.
     * The ACK must find the old MME's remote list, not allocate a new xact. */
    header.type = 130;
    xact = ogs_gtp_xact_local_create(&node, &header,
            transaction_payload(), NULL, NULL);
    ogs_assert(xact);
    header.sqn = OGS_GTP2_XID_TO_SQN(xact->xid);
    ABTS_INT_EQUAL(tc, OGS_OK,
            ogs_gtp_xact_receive(&old_mme, &header, &remote));
    ogs_assert(remote);
    id = remote->id;
    header.type = 131;
    ABTS_INT_EQUAL(tc, OGS_OK,
            ogs_gtp_xact_update_tx(remote, &header, transaction_payload()));
    ABTS_INT_EQUAL(tc, OGS_OK,
            ogs_gtp_xact_receive(&node, &header, &received));
    ABTS_TRUE(tc, received == xact);
    ABTS_INT_EQUAL(tc, OGS_OK, ogs_gtp_xact_commit(xact));
    ABTS_INT_EQUAL(tc, 2, xact->step);
    header.type = 132;
    ABTS_INT_EQUAL(tc, OGS_OK,
            ogs_gtp_xact_update_tx(xact, &header, transaction_payload()));
    ABTS_INT_EQUAL(tc, OGS_OK,
            ogs_gtp_xact_receive(&old_mme, &header, &received));
    ABTS_TRUE(tc, received == remote);
    ABTS_INT_EQUAL(tc, 3, remote->step);
    ABTS_INT_EQUAL(tc, OGS_OK, ogs_gtp_xact_commit(remote));
    ABTS_TRUE(tc, !ogs_gtp_xact_find_by_id(id));
    ogs_gtp_xact_delete_all(&node);
    /* A rejected Context Response is a two-leg exchange. Preserve its
     * duplicate cache, without starting a timer waiting for a forbidden ACK. */
    {
        ogs_sockaddr_t bind_addr = {0};
        bind_addr.sin.sin_family = AF_INET;
        bind_addr.sin.sin_addr.s_addr = htobe32(INADDR_LOOPBACK);
        old_mme.addr = bind_addr;
        old_mme.addr.sin.sin_port = htobe16(9);
        old_mme.sock = ogs_udp_server(&bind_addr, NULL);
        ABTS_PTR_NOTNULL(tc, old_mme.sock);
        header.type = OGS_GTP2_CONTEXT_REQUEST_TYPE;
        xact = ogs_gtp_xact_local_create(&node, &header,
                transaction_payload(), NULL, NULL);
        ogs_assert(xact);
        header.sqn = OGS_GTP2_XID_TO_SQN(xact->xid);
        ABTS_INT_EQUAL(tc, OGS_OK, ogs_gtp_xact_receive(&old_mme, &header, &remote));
        ogs_assert(remote);
        remote->response_final = true;
        header.type = OGS_GTP2_CONTEXT_RESPONSE_TYPE;
        ABTS_INT_EQUAL(tc, OGS_OK,
                ogs_gtp_xact_update_tx(remote, &header, transaction_payload()));
        ABTS_INT_EQUAL(tc, OGS_OK, ogs_gtp_xact_commit(remote));
        ABTS_INT_EQUAL(tc, OGS_OK, ogs_gtp_xact_receive(&node, &header, &received));
        ABTS_TRUE(tc, received == xact);
        id = xact->id;
        xact->response_final = true;
        ABTS_INT_EQUAL(tc, OGS_OK, ogs_gtp_xact_commit(xact));
        ABTS_TRUE(tc, !ogs_gtp_xact_find_by_id(id));
        ABTS_TRUE(tc, ogs_timer_mgr_next(ogs_app()->timer_mgr) > ogs_time_from_sec(50));
        header.type = OGS_GTP2_CONTEXT_REQUEST_TYPE;
        ABTS_INT_EQUAL(tc, OGS_RETRY,
                ogs_gtp_xact_receive(&old_mme, &header, &received));
        ogs_gtp_xact_delete_all(&old_mme);
        ogs_sock_destroy(old_mme.sock);
        old_mme.sock = NULL;
    }
    /* Cancelling an abandoned mobility procedure releases its pool slot
     * and removes it from peer lookup without a synthetic response. */
    header.type = OGS_GTP2_CONTEXT_REQUEST_TYPE;
    xact = ogs_gtp_xact_local_create(&node, &header,
            transaction_payload(), NULL, NULL);
    ABTS_PTR_NOTNULL(tc, xact);
    if (xact) {
        id = xact->id;
        ogs_gtp_xact_cancel(xact);
        ABTS_TRUE(tc, ogs_gtp_xact_find_by_id(id) == NULL);
        ABTS_TRUE(tc, ogs_list_empty(&node.local_list));
    }
    ogs_gtp_xact_delete_all(&old_mme);
    ogs_gtp_xact_final();
    ogs_timer_mgr_destroy(ogs_app()->timer_mgr);
    ogs_app()->timer_mgr = saved_timers;
    ogs_app()->pool.xact = saved_pool;
    ogs_local_conf()->time.message.gtp.t3_holding_duration = saved_holding;
    ogs_local_conf()->time.message.gtp.t3_response_duration = saved_response;
    ogs_local_conf()->time.message.gtp.n3_holding_rcount = saved_retries;
}

abts_suite *test_gtp_s10(abts_suite *suite)
{
    suite = ADD_SUITE(suite);
    abts_run_test(suite, eps_known_encoding, NULL);
    abts_run_test(suite, eps_optional_and_truncated, NULL);
    abts_run_test(suite, eps_invalid_and_extensions, NULL);
    abts_run_test(suite, identification_wire, NULL);
    abts_run_test(suite, pdn_bearer_wire, NULL);
    abts_run_test(suite, relocation_forwarding_wire, NULL);
    abts_run_test(suite, empty_relocation_complete, NULL);
    abts_run_test(suite, multiple_pdn_and_bearer, NULL);
    abts_run_test(suite, s10_message_dispatch, NULL);
    abts_run_test(suite, s10_transaction_stages, NULL);
    return suite;
}
