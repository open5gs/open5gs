/* SPDX-License-Identifier: AGPL-3.0-or-later */
/* Add a second, independently addressed APN without adding policy rules. */
static bson_t *s10_two_apns(bson_t *original)
{
    bson_iter_t iter, child;
    const uint8_t *bytes;
    uint32_t len;
    bson_t first_slice, first_session, extra, slice, sessions, slices;
    bson_t *result = bson_new();
    ogs_assert(bson_iter_init(&iter, original));
    ogs_assert(bson_iter_find_descendant(&iter, "slice.0", &child));
    bson_iter_document(&child, &len, &bytes);
    ogs_assert(bson_init_static(&first_slice, bytes, len));
    ogs_assert(bson_iter_init(&iter, &first_slice));
    ogs_assert(bson_iter_find_descendant(&iter, "session.0", &child));
    bson_iter_document(&child, &len, &bytes);
    ogs_assert(bson_init_static(&first_session, bytes, len));
    bson_init(&extra);
    bson_copy_to_excluding_noinit(&first_session, &extra, "name", "pcc_rule", NULL);
    BSON_APPEND_UTF8(&extra, "name", "extra");
    bson_copy_to_excluding_noinit(original, result, "slice", NULL);
    BSON_APPEND_ARRAY_BEGIN(result, "slice", &slices);
    BSON_APPEND_DOCUMENT_BEGIN(&slices, "0", &slice);
    bson_iter_init(&iter, &first_slice);
    while (bson_iter_next(&iter))
        if (strcmp(bson_iter_key(&iter), "session"))
            bson_append_value(&slice, bson_iter_key(&iter), -1, bson_iter_value(&iter));
    BSON_APPEND_ARRAY_BEGIN(&slice, "session", &sessions);
    BSON_APPEND_DOCUMENT(&sessions, "0", &first_session);
    BSON_APPEND_DOCUMENT(&sessions, "1", &extra);
    bson_append_array_end(&slice, &sessions);
    bson_append_document_end(&slices, &slice);
    bson_append_array_end(result, &slices);
    bson_destroy(&extra);
    bson_destroy(original);
    return result;
}

static test_bearer_t *s10_add_pdn(abts_case *tc, test_ue_t *ue,
        ogs_socknode_t *s1ap)
{
    test_sess_t *sess = test_sess_add_by_apn(ue, "extra", OGS_GTP2_RAT_TYPE_EUTRAN);
    test_bearer_t *bearer;
    ogs_pkbuf_t *p, *nas;
    ogs_assert(sess);
    sess->pti = 5;
    sess->pdn_connectivity_param.apn = 1;
    sess->pdn_connectivity_param.pco = 1;
    sess->pdn_connectivity_param.request_type = OGS_NAS_EPS_REQUEST_TYPE_INITIAL;
    nas = testesm_build_pdn_connectivity_request(sess, true, OGS_NAS_EPS_PDN_TYPE_IPV4V6);
    p = test_s1ap_build_uplink_nas_transport(ue, nas);
    ABTS_INT_EQUAL(tc, OGS_OK, testenb_s1ap_send(s1ap, p));
    p = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, p);
    tests1ap_recv(ue, p);
    ABTS_INT_EQUAL(tc, S1AP_ProcedureCode_id_E_RABSetup, ue->s1ap_procedure_code);
    bearer = ogs_list_first(&sess->bearer_list);
    ogs_assert(bearer);
    p = test_s1ap_build_e_rab_setup_response(bearer);
    ABTS_INT_EQUAL(tc, OGS_OK, testenb_s1ap_send(s1ap, p));
    nas = testesm_build_activate_default_eps_bearer_context_accept(bearer, true);
    p = test_s1ap_build_uplink_nas_transport(ue, nas);
    ABTS_INT_EQUAL(tc, OGS_OK, testenb_s1ap_send(s1ap, p));
    ogs_msleep(100);
    return bearer;
}
