/* SPDX-License-Identifier: AGPL-3.0-or-later */
/* End Markers are valid during relocation; an arbitrary UDP packet is not
 * evidence that the new user-plane path works. Require an IPv4 Echo Reply. */
static ogs_pkbuf_t *s10_read_echo(abts_case *tc, ogs_socknode_t *node,
        test_bearer_t *bearer)
{
    ogs_pkbuf_t *p = NULL;
    ogs_gtp2_header_desc_t h = {0};
    int n, len = 0;
    uint8_t *ip;
    uint32_t destination;
    for (n = 0; n <= OGS_BEARER_PER_UE; n++) {
        p = test_gtpu_read(node);
        ABTS_PTR_NOTNULL(tc, p);
        if (!p) return NULL;
        len = ogs_gtpu_parse_header(&h, p);
        if (len <= 0 || h.type != OGS_GTPU_MSGTYPE_END_MARKER) break;
        ogs_pkbuf_free(p);
        p = NULL;
    }
    ABTS_PTR_NOTNULL(tc, p);
    if (!p) return NULL;
    ABTS_INT_EQUAL(tc, OGS_GTPU_MSGTYPE_GPDU, h.type);
    ABTS_TRUE(tc, len >= OGS_GTPV1U_HEADER_LEN && p->len >= len + 28);
    if (len >= OGS_GTPV1U_HEADER_LEN && p->len >= len + 28) {
        ip = p->data + len;
        ABTS_INT_EQUAL(tc, 4, ip[0] >> 4);
        ABTS_INT_EQUAL(tc, 1, ip[9]);
        memcpy(&destination, ip + 16, sizeof(destination));
        ABTS_INT_EQUAL(tc, bearer->sess->ue_ip.addr, destination);
        ABTS_TRUE(tc, p->len >= len + (ip[0] & 15) * 4 + 8);
        if (p->len >= len + (ip[0] & 15) * 4 + 8)
            ABTS_INT_EQUAL(tc, 0, ip[(ip[0] & 15) * 4]);
    }
    return p;
}
