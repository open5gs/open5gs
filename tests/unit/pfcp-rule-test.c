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
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "ogs-pfcp.h"
#include "core/abts.h"

static void set_pdr_teid(ogs_pfcp_pdr_t *pdr, uint32_t teid)
{
    pdr->f_teid.teid = teid;
    pdr->f_teid_len = sizeof(pdr->f_teid);
}

static void test_clear_since_mark(abts_case *tc, void *data)
{
    ogs_pfcp_sess_t sess;
    ogs_pfcp_sess_mark_t mark;
    ogs_pfcp_pdr_t *pdr = NULL;
    ogs_pfcp_far_t *far = NULL;
    ogs_pfcp_urr_t *urr = NULL;
    ogs_pfcp_qer_t *qer = NULL;

    memset(&sess, 0, sizeof(sess));
    ogs_pfcp_pool_init(&sess);

    pdr = ogs_pfcp_pdr_add(&sess);
    far = ogs_pfcp_far_add(&sess);
    urr = ogs_pfcp_urr_add(&sess);
    qer = ogs_pfcp_qer_add(&sess);
    ogs_pfcp_pdr_associate_far(pdr, far);
    ogs_pfcp_pdr_associate_urr(pdr, urr);
    ogs_pfcp_pdr_associate_qer(pdr, qer);
    ogs_pfcp_sess_mark(&sess, &mark);

    ogs_pfcp_pdr_add(&sess);
    ogs_pfcp_far_add(&sess);
    ogs_pfcp_urr_add(&sess);
    ogs_pfcp_qer_add(&sess);
    ogs_pfcp_bar_new(&sess);

    ogs_pfcp_sess_clear_since_mark(&sess, &mark);

    ABTS_INT_EQUAL(tc, 1, ogs_list_count(&sess.pdr_list));
    ABTS_INT_EQUAL(tc, 1, ogs_list_count(&sess.far_list));
    ABTS_INT_EQUAL(tc, 1, ogs_list_count(&sess.urr_list));
    ABTS_INT_EQUAL(tc, 1, ogs_list_count(&sess.qer_list));
    ABTS_PTR_EQUAL(tc, pdr, ogs_list_first(&sess.pdr_list));
    ABTS_PTR_EQUAL(tc, far, ogs_list_first(&sess.far_list));
    ABTS_PTR_EQUAL(tc, urr, ogs_list_first(&sess.urr_list));
    ABTS_PTR_EQUAL(tc, qer, ogs_list_first(&sess.qer_list));
    ABTS_PTR_EQUAL(tc, NULL, sess.bar);

    ogs_pfcp_sess_clear(&sess);
    ogs_pfcp_pool_final(&sess);
}

/* A rejected request must not leave an End Marker for the next request. */
static void test_clear_since_mark_far_flags(abts_case *tc, void *data)
{
    ogs_pfcp_sess_t sess;
    ogs_pfcp_sess_mark_t mark;
    ogs_pfcp_pdr_t *pdr = NULL;
    ogs_pfcp_far_t *far = NULL, *new_far = NULL;
    ogs_pfcp_tlv_update_far_t update_far;
    ogs_pfcp_smreq_flags_t flags;
    uint32_t new_far_id;
    uint8_t cause = OGS_PFCP_CAUSE_REQUEST_ACCEPTED;
    uint8_t offending_ie = 0;

    memset(&sess, 0, sizeof(sess));
    ogs_pfcp_pool_init(&sess);
    pdr = ogs_pfcp_pdr_add(&sess);
    far = ogs_pfcp_far_add(&sess);
    ogs_pfcp_pdr_associate_far(pdr, far);
    far->apply_action = OGS_PFCP_APPLY_ACTION_FORW;
    far->outer_header_creation.teid = 0x1234;
    ogs_pfcp_sess_mark(&sess, &mark);

    new_far = ogs_pfcp_far_add(&sess);
    new_far_id = new_far->id;

    memset(&flags, 0, sizeof(flags));
    flags.send_end_marker_packets = 1;
    memset(&update_far, 0, sizeof(update_far));
    update_far.presence = update_far.far_id.presence = 1;
    update_far.far_id.u32 = far->id;
    update_far.update_forwarding_parameters.presence = 1;
    update_far.update_forwarding_parameters.pfcpsmreq_flags.presence = 1;
    update_far.update_forwarding_parameters.pfcpsmreq_flags.u8 = flags.value;
    ABTS_PTR_EQUAL(tc, far, ogs_pfcp_handle_update_far_flags(
                &sess, &update_far, &cause, &offending_ie));
    ABTS_INT_EQUAL(tc, 1, far->smreq_flags.send_end_marker_packets);

    /* A later Update FAR fails before the normal End Marker/clear pass. */
    update_far.far_id.u32 = 0x100;
    ABTS_PTR_EQUAL(tc, NULL, ogs_pfcp_handle_update_far_flags(
                &sess, &update_far, &cause, &offending_ie));
    ABTS_INT_EQUAL(tc, OGS_PFCP_CAUSE_MANDATORY_IE_INCORRECT, cause);
    ABTS_INT_EQUAL(tc, OGS_PFCP_FAR_ID_TYPE, offending_ie);

    ogs_pfcp_sess_clear_since_mark(&sess, &mark);

    ABTS_INT_EQUAL(tc, 1, ogs_list_count(&sess.pdr_list));
    ABTS_INT_EQUAL(tc, 1, ogs_list_count(&sess.far_list));
    ABTS_PTR_EQUAL(tc, pdr, ogs_list_first(&sess.pdr_list));
    ABTS_PTR_EQUAL(tc, far, pdr->far);
    ABTS_PTR_EQUAL(tc, NULL, ogs_pfcp_far_find(&sess, new_far_id));
    ABTS_INT_EQUAL(tc, OGS_PFCP_APPLY_ACTION_FORW, far->apply_action);
    ABTS_INT_EQUAL(tc, 0x1234, far->outer_header_creation.teid);
    ABTS_INT_EQUAL(tc, 0, far->smreq_flags.value);

    ogs_pfcp_sess_clear(&sess);
    ogs_pfcp_pool_final(&sess);
}

static void test_clear_failed_create(abts_case *tc, void *data)
{
    ogs_pfcp_sess_t sess;
    ogs_pfcp_sess_mark_t mark;
    ogs_pfcp_tlv_create_far_t create_far;
    uint8_t cause = OGS_PFCP_CAUSE_REQUEST_ACCEPTED;
    uint8_t offending_ie = 0;

    memset(&sess, 0, sizeof(sess));
    ogs_pfcp_pool_init(&sess);
    ogs_pfcp_sess_mark(&sess, &mark);

    memset(&create_far, 0, sizeof(create_far));
    create_far.presence = 1;
    create_far.far_id.presence = 1;
    create_far.far_id.u32 = 0x100;

    ABTS_PTR_EQUAL(tc, NULL,
            ogs_pfcp_handle_create_far(
                &sess, &create_far, &cause, &offending_ie));
    ABTS_PTR_NOTNULL(tc, ogs_pfcp_far_find(&sess, create_far.far_id.u32));

    ogs_pfcp_sess_clear_since_mark(&sess, &mark);
    ABTS_PTR_EQUAL(tc, NULL,
            ogs_pfcp_far_find(&sess, create_far.far_id.u32));

    ogs_pfcp_sess_clear(&sess);
    ogs_pfcp_pool_final(&sess);
}

static void test_validate_create_rules(abts_case *tc, void *data)
{
    ogs_pfcp_sess_t sess;
    ogs_pfcp_pdr_t *pdr = NULL;
    ogs_pfcp_far_t *far = NULL;
    ogs_pfcp_urr_t *urr = NULL;
    ogs_pfcp_qer_t *qer = NULL;
    ogs_pfcp_session_modification_request_t req;
    uint8_t rule_type = 0;
    uint32_t rule_id = 0;

    memset(&sess, 0, sizeof(sess));
    ogs_pfcp_pool_init(&sess);
    pdr = ogs_pfcp_pdr_add(&sess);
    far = ogs_pfcp_far_add(&sess);
    urr = ogs_pfcp_urr_add(&sess);
    qer = ogs_pfcp_qer_add(&sess);

    memset(&req, 0, sizeof(req));
    ABTS_TRUE(tc, ogs_pfcp_validate_create_rules(
                &sess, &req, &rule_type, &rule_id));

    req.create_pdr[0].presence = 1;
    req.create_pdr[0].pdr_id.presence = 1;
    req.create_pdr[0].pdr_id.u16 = pdr->id;
    ABTS_TRUE(tc, !ogs_pfcp_validate_create_rules(
                &sess, &req, &rule_type, &rule_id));
    ABTS_INT_EQUAL(tc, OGS_PFCP_RULE_TYPE_PDR, rule_type);
    ABTS_INT_EQUAL(tc, pdr->id, rule_id);

    req.create_pdr[0].pdr_id.u16 = 0x1234;
    ABTS_TRUE(tc, ogs_pfcp_validate_create_rules(
                &sess, &req, &rule_type, &rule_id));
    req.create_pdr[1] = req.create_pdr[0];
    ABTS_TRUE(tc, !ogs_pfcp_validate_create_rules(
                &sess, &req, &rule_type, &rule_id));
    ABTS_INT_EQUAL(tc, OGS_PFCP_RULE_TYPE_PDR, rule_type);
    ABTS_INT_EQUAL(tc, 0x1234, rule_id);

    memset(&req, 0, sizeof(req));
    req.create_far[0].presence = 1;
    req.create_far[0].far_id.presence = 1;
    req.create_far[0].far_id.u32 = far->id;
    ABTS_TRUE(tc, !ogs_pfcp_validate_create_rules(
                &sess, &req, &rule_type, &rule_id));
    ABTS_INT_EQUAL(tc, OGS_PFCP_RULE_TYPE_FAR, rule_type);
    ABTS_INT_EQUAL(tc, far->id, rule_id);

    req.create_far[0].far_id.u32 = 0x12345678;
    ABTS_TRUE(tc, ogs_pfcp_validate_create_rules(
                &sess, &req, &rule_type, &rule_id));
    req.create_far[1] = req.create_far[0];
    ABTS_TRUE(tc, !ogs_pfcp_validate_create_rules(
                &sess, &req, &rule_type, &rule_id));
    ABTS_INT_EQUAL(tc, OGS_PFCP_RULE_TYPE_FAR, rule_type);
    ABTS_INT_EQUAL(tc, 0x12345678, rule_id);

    memset(&req, 0, sizeof(req));
    req.create_urr[0].presence = 1;
    req.create_urr[0].urr_id.presence = 1;
    req.create_urr[0].urr_id.u32 = urr->id;
    ABTS_TRUE(tc, !ogs_pfcp_validate_create_rules(
                &sess, &req, &rule_type, &rule_id));
    ABTS_INT_EQUAL(tc, OGS_PFCP_RULE_TYPE_URR, rule_type);
    ABTS_INT_EQUAL(tc, urr->id, rule_id);

    req.create_urr[0].urr_id.u32 = 0x12345678;
    ABTS_TRUE(tc, ogs_pfcp_validate_create_rules(
                &sess, &req, &rule_type, &rule_id));
    req.create_urr[1] = req.create_urr[0];
    ABTS_TRUE(tc, !ogs_pfcp_validate_create_rules(
                &sess, &req, &rule_type, &rule_id));
    ABTS_INT_EQUAL(tc, OGS_PFCP_RULE_TYPE_URR, rule_type);
    ABTS_INT_EQUAL(tc, 0x12345678, rule_id);

    memset(&req, 0, sizeof(req));
    req.create_qer[0].presence = 1;
    req.create_qer[0].qer_id.presence = 1;
    req.create_qer[0].qer_id.u32 = qer->id;
    ABTS_TRUE(tc, !ogs_pfcp_validate_create_rules(
                &sess, &req, &rule_type, &rule_id));
    ABTS_INT_EQUAL(tc, OGS_PFCP_RULE_TYPE_QER, rule_type);
    ABTS_INT_EQUAL(tc, qer->id, rule_id);

    req.create_qer[0].qer_id.u32 = 0x12345678;
    ABTS_TRUE(tc, ogs_pfcp_validate_create_rules(
                &sess, &req, &rule_type, &rule_id));
    req.create_qer[1] = req.create_qer[0];
    ABTS_TRUE(tc, !ogs_pfcp_validate_create_rules(
                &sess, &req, &rule_type, &rule_id));
    ABTS_INT_EQUAL(tc, OGS_PFCP_RULE_TYPE_QER, rule_type);
    ABTS_INT_EQUAL(tc, 0x12345678, rule_id);

    /* A missing Rule ID is left to the ordinary Create IE validation. */
    req.create_qer[1].qer_id.presence = 0;
    ABTS_TRUE(tc, ogs_pfcp_validate_create_rules(
                &sess, &req, &rule_type, &rule_id));

    ABTS_INT_EQUAL(tc, 1, ogs_list_count(&sess.pdr_list));
    ABTS_INT_EQUAL(tc, 1, ogs_list_count(&sess.far_list));
    ABTS_INT_EQUAL(tc, 1, ogs_list_count(&sess.urr_list));
    ABTS_INT_EQUAL(tc, 1, ogs_list_count(&sess.qer_list));

    ogs_pfcp_sess_clear(&sess);
    ogs_pfcp_pool_final(&sess);
}

static void test_pdr_teid_owner(abts_case *tc, void *data)
{
    ogs_pfcp_sess_t sess, other_sess;
    ogs_pfcp_pdr_t *first = NULL, *second = NULL, *other = NULL;
    uint32_t teid = 0x1234;

    memset(&sess, 0, sizeof(sess));
    memset(&other_sess, 0, sizeof(other_sess));
    ogs_pfcp_pool_init(&sess);
    ogs_pfcp_pool_init(&other_sess);

    first = ogs_pfcp_pdr_add(&sess);
    second = ogs_pfcp_pdr_add(&sess);
    other = ogs_pfcp_pdr_add(&other_sess);
    set_pdr_teid(first, teid);
    set_pdr_teid(second, teid);
    set_pdr_teid(other, teid);

    ABTS_INT_EQUAL(tc, OGS_PFCP_CAUSE_REQUEST_ACCEPTED,
            ogs_pfcp_object_teid_hash_set(
                OGS_PFCP_OBJ_PDR_TYPE, first));
    ABTS_INT_EQUAL(tc, OGS_PFCP_CAUSE_REQUEST_ACCEPTED,
            ogs_pfcp_object_teid_hash_set(
                OGS_PFCP_OBJ_PDR_TYPE, second));
    ABTS_TRUE(tc, first->hash.teid.registered);
    ABTS_TRUE(tc, !second->hash.teid.registered);
    ABTS_PTR_EQUAL(tc, first, ogs_pfcp_object_find_by_teid(teid));

    ABTS_INT_EQUAL(tc, OGS_PFCP_CAUSE_REQUEST_REJECTED,
            ogs_pfcp_object_teid_hash_set(
                OGS_PFCP_OBJ_PDR_TYPE, other));
    ABTS_TRUE(tc, !other->hash.teid.registered);

    ogs_pfcp_pdr_remove(first);
    ABTS_TRUE(tc, second->hash.teid.registered);
    ABTS_PTR_EQUAL(tc, second, ogs_pfcp_object_find_by_teid(teid));

    set_pdr_teid(second, teid + 1);
    ABTS_INT_EQUAL(tc, OGS_PFCP_CAUSE_REQUEST_ACCEPTED,
            ogs_pfcp_object_teid_hash_set(
                OGS_PFCP_OBJ_PDR_TYPE, second));
    ABTS_PTR_EQUAL(tc, NULL, ogs_pfcp_object_find_by_teid(teid));
    ABTS_PTR_EQUAL(tc, second, ogs_pfcp_object_find_by_teid(teid + 1));

    ogs_pfcp_sess_clear(&other_sess);
    ogs_pfcp_sess_clear(&sess);
    ABTS_PTR_EQUAL(tc, NULL, ogs_pfcp_object_find_by_teid(teid));
    ABTS_PTR_EQUAL(tc, NULL, ogs_pfcp_object_find_by_teid(teid + 1));
    ogs_pfcp_pool_final(&other_sess);
    ogs_pfcp_pool_final(&sess);
}

static void test_removed_pdr_owner(abts_case *tc, void *data)
{
    ogs_pfcp_sess_t sess;
    ogs_pfcp_sess_mark_t mark;
    ogs_pfcp_pdr_t *pdr = NULL, *new_pdr = NULL;
    ogs_pfcp_object_type_e type[] = {
        OGS_PFCP_OBJ_SESS_TYPE,
        OGS_PFCP_OBJ_PDR_TYPE,
    };
    uint32_t teid = 0x2345;
    void *expected = NULL;
    int i;

    for (i = 0; i < OGS_ARRAY_SIZE(type); i++) {
        memset(&sess, 0, sizeof(sess));
        ogs_pfcp_pool_init(&sess);
        pdr = ogs_pfcp_pdr_add(&sess);
        pdr->id = 1;
        set_pdr_teid(pdr, teid + i);
        ABTS_INT_EQUAL(tc, OGS_PFCP_CAUSE_REQUEST_ACCEPTED,
                ogs_pfcp_object_teid_hash_set(type[i], pdr));
        ogs_pfcp_sess_mark(&sess, &mark);

        new_pdr = ogs_pfcp_pdr_add(&sess);
        new_pdr->id = 2;
        set_pdr_teid(new_pdr, teid + i);
        ogs_pfcp_pdr_remove(pdr);
        ABTS_TRUE(tc, new_pdr->hash.teid.registered);
        expected = type[i] == OGS_PFCP_OBJ_PDR_TYPE ?
            (void *)new_pdr : (void *)&sess;
        ABTS_PTR_EQUAL(tc, expected,
                ogs_pfcp_object_find_by_teid(teid + i));

        ogs_pfcp_sess_clear_since_mark(&sess, &mark);
        ABTS_PTR_EQUAL(tc, NULL,
                ogs_pfcp_object_find_by_teid(teid + i));
        ogs_pfcp_pool_final(&sess);
    }
}

static void test_far_f_teid_owner(abts_case *tc, void *data)
{
    ogs_pfcp_sess_t sess;
    ogs_pfcp_sess_mark_t mark;
    ogs_pfcp_far_t *first = NULL, *second = NULL;
    ogs_pfcp_far_hash_f_teid_t key;
    ogs_gtp_node_t gnode;
    int len;

    memset(&sess, 0, sizeof(sess));
    memset(&gnode, 0, sizeof(gnode));
    ogs_pfcp_pool_init(&sess);
    gnode.addr.sin.sin_family = AF_INET;
    gnode.addr.sin.sin_addr.s_addr = htobe32(0x7f000001);

    first = ogs_pfcp_far_add(&sess);
    first->id = 1;
    first->gnode = &gnode;
    first->outer_header_creation.teid = 0x3456;
    ogs_pfcp_far_f_teid_hash_set(first);
    key = first->hash.f_teid.key;
    len = first->hash.f_teid.len;
    ABTS_TRUE(tc, len != 0);
    ogs_pfcp_sess_mark(&sess, &mark);

    second = ogs_pfcp_far_add(&sess);
    second->id = 2;
    second->gnode = &gnode;
    second->outer_header_creation.teid = first->outer_header_creation.teid;
    ogs_pfcp_far_f_teid_hash_set(second);
    ABTS_INT_EQUAL(tc, 0, second->hash.f_teid.len);

    /* Rejection must not erase the marked FAR's Error Indication entry. */
    second->gnode = NULL;
    ogs_pfcp_sess_clear_since_mark(&sess, &mark);
    ABTS_PTR_EQUAL(tc, first,
            ogs_hash_get(ogs_pfcp_self()->far_f_teid_hash, &key, len));

    second = ogs_pfcp_far_add(&sess);
    second->gnode = &gnode;
    second->outer_header_creation.teid = first->outer_header_creation.teid;
    ogs_pfcp_far_f_teid_hash_set(second);

    /* Removing the registered FAR ends the mapping; there is no handoff. */
    first->gnode = NULL;
    ogs_pfcp_far_remove(first);
    ABTS_INT_EQUAL(tc, 0, second->hash.f_teid.len);
    ABTS_PTR_EQUAL(tc, NULL,
            ogs_hash_get(ogs_pfcp_self()->far_f_teid_hash, &key, len));

    second->gnode = NULL;
    ogs_pfcp_far_remove(second);
    ogs_pfcp_pool_final(&sess);
}

static void test_far_rekey_collision(abts_case *tc, void *data)
{
    ogs_pfcp_sess_t sess, other_sess;
    ogs_pfcp_far_t *far = NULL, *other = NULL;
    ogs_pfcp_far_hash_f_teid_t old_key, new_key;
    ogs_gtp_node_t gnode;
    int len;

    memset(&sess, 0, sizeof(sess));
    memset(&other_sess, 0, sizeof(other_sess));
    memset(&gnode, 0, sizeof(gnode));
    ogs_pfcp_pool_init(&sess);
    ogs_pfcp_pool_init(&other_sess);
    gnode.addr.sin.sin_family = AF_INET;
    gnode.addr.sin.sin_addr.s_addr = htobe32(0x7f000001);

    far = ogs_pfcp_far_add(&sess);
    other = ogs_pfcp_far_add(&other_sess);
    far->gnode = other->gnode = &gnode;
    far->outer_header_creation.teid = 0x4567;
    other->outer_header_creation.teid = 0x4568;
    ogs_pfcp_far_f_teid_hash_set(far);
    ogs_pfcp_far_f_teid_hash_set(other);
    old_key = far->hash.f_teid.key;
    new_key = other->hash.f_teid.key;
    len = far->hash.f_teid.len;

    /* A collision at the new tunnel must still release the old mapping. */
    far->outer_header_creation.teid = other->outer_header_creation.teid;
    ogs_pfcp_far_f_teid_hash_set(far);
    ABTS_INT_EQUAL(tc, 0, far->hash.f_teid.len);
    ABTS_PTR_EQUAL(tc, NULL,
            ogs_hash_get(ogs_pfcp_self()->far_f_teid_hash, &old_key, len));
    ABTS_PTR_EQUAL(tc, other,
            ogs_hash_get(ogs_pfcp_self()->far_f_teid_hash, &new_key, len));

    far->gnode = NULL;
    ogs_pfcp_far_remove(far);
    ABTS_PTR_EQUAL(tc, other,
            ogs_hash_get(ogs_pfcp_self()->far_f_teid_hash, &new_key, len));
    other->gnode = NULL;
    ogs_pfcp_far_remove(other);
    ABTS_PTR_EQUAL(tc, NULL,
            ogs_hash_get(ogs_pfcp_self()->far_f_teid_hash, &new_key, len));
    ogs_pfcp_pool_final(&sess);
    ogs_pfcp_pool_final(&other_sess);
}

static void test_remove_referenced_rules(abts_case *tc, void *data)
{
    ogs_pfcp_sess_t sess;
    ogs_pfcp_pdr_t *pdr = NULL;
    ogs_pfcp_far_t *far = NULL;
    ogs_pfcp_urr_t *urr = NULL;
    ogs_pfcp_qer_t *qer = NULL;
    ogs_pfcp_tlv_remove_far_t remove_far;
    ogs_pfcp_tlv_remove_urr_t remove_urr;
    ogs_pfcp_tlv_remove_qer_t remove_qer;
    uint8_t cause = OGS_PFCP_CAUSE_REQUEST_ACCEPTED;
    uint8_t offending_ie = 0;

    memset(&sess, 0, sizeof(sess));
    ogs_pfcp_pool_init(&sess);
    pdr = ogs_pfcp_pdr_add(&sess);
    far = ogs_pfcp_far_add(&sess);
    urr = ogs_pfcp_urr_add(&sess);
    qer = ogs_pfcp_qer_add(&sess);
    ogs_pfcp_pdr_associate_far(pdr, far);
    ogs_pfcp_pdr_associate_urr(pdr, urr);
    ogs_pfcp_pdr_associate_qer(pdr, qer);

    memset(&remove_far, 0, sizeof(remove_far));
    remove_far.presence = remove_far.far_id.presence = 1;
    remove_far.far_id.u32 = far->id;
    ABTS_TRUE(tc, !ogs_pfcp_handle_remove_far(
                &sess, &remove_far, &cause, &offending_ie));
    ABTS_INT_EQUAL(tc, OGS_PFCP_CAUSE_REQUEST_REJECTED, cause);
    ABTS_INT_EQUAL(tc, 0, offending_ie);

    cause = OGS_PFCP_CAUSE_REQUEST_ACCEPTED;
    memset(&remove_urr, 0, sizeof(remove_urr));
    remove_urr.presence = remove_urr.urr_id.presence = 1;
    remove_urr.urr_id.u32 = urr->id;
    ABTS_TRUE(tc, !ogs_pfcp_handle_remove_urr(
                &sess, &remove_urr, &cause, &offending_ie));
    ABTS_INT_EQUAL(tc, OGS_PFCP_CAUSE_REQUEST_REJECTED, cause);
    ABTS_INT_EQUAL(tc, 0, offending_ie);

    cause = OGS_PFCP_CAUSE_REQUEST_ACCEPTED;
    memset(&remove_qer, 0, sizeof(remove_qer));
    remove_qer.presence = remove_qer.qer_id.presence = 1;
    remove_qer.qer_id.u32 = qer->id;
    ABTS_TRUE(tc, !ogs_pfcp_handle_remove_qer(
                &sess, &remove_qer, &cause, &offending_ie));
    ABTS_INT_EQUAL(tc, OGS_PFCP_CAUSE_REQUEST_REJECTED, cause);
    ABTS_INT_EQUAL(tc, 0, offending_ie);

    ogs_pfcp_pdr_remove(pdr);
    cause = OGS_PFCP_CAUSE_REQUEST_ACCEPTED;
    ABTS_TRUE(tc, ogs_pfcp_handle_remove_far(
                &sess, &remove_far, &cause, &offending_ie));
    ABTS_TRUE(tc, ogs_pfcp_handle_remove_urr(
                &sess, &remove_urr, &cause, &offending_ie));
    ABTS_TRUE(tc, ogs_pfcp_handle_remove_qer(
                &sess, &remove_qer, &cause, &offending_ie));

    ogs_pfcp_sess_clear(&sess);
    ogs_pfcp_pool_final(&sess);
}

abts_suite *test_pfcp_rule(abts_suite *suite)
{
    int id;
    ogs_log_level_e level;

    ogs_app_context_init();
    ogs_app()->pool.nf = 8;
    ogs_app()->pool.sess = 8;
    ogs_pfcp_context_init();

    suite = ADD_SUITE(suite)

    id = ogs_log_get_domain_id("pfcp");
    level = ogs_log_get_domain_level(id);
    ogs_log_set_domain_level(id, OGS_LOG_NONE);

    abts_run_test(suite, test_clear_since_mark, NULL);
    abts_run_test(suite, test_clear_since_mark_far_flags, NULL);
    abts_run_test(suite, test_clear_failed_create, NULL);
    abts_run_test(suite, test_validate_create_rules, NULL);
    abts_run_test(suite, test_pdr_teid_owner, NULL);
    abts_run_test(suite, test_removed_pdr_owner, NULL);
    abts_run_test(suite, test_far_f_teid_owner, NULL);
    abts_run_test(suite, test_far_rekey_collision, NULL);
    abts_run_test(suite, test_remove_referenced_rules, NULL);

    ogs_log_set_domain_level(id, level);
    ogs_pfcp_context_final();
    ogs_app_context_final();

    return suite;
}
