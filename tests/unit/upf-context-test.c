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

#include "upf/context.h"
#include "core/abts.h"

/* How many of the index's stored keys are the klen bytes at key. */
static int count_key(ogs_hash_t *hash, const void *key, int klen)
{
    ogs_hash_index_t *hi;
    int n = 0;

    for (hi = ogs_hash_first(hash); hi; hi = ogs_hash_next(hi))
        if (ogs_hash_this_key_len(hi) == klen &&
            !memcmp(ogs_hash_this_key(hi), key, klen))
            n++;

    return n;
}

static void set_ue_ipv4(abts_case *tc, upf_sess_t *sess,
        ogs_pfcp_pdr_t *pdr, uint32_t addr)
{
    memset(&pdr->ue_ip_addr, 0, sizeof(pdr->ue_ip_addr));
    pdr->ue_ip_addr.ipv4 = 1;
    pdr->ue_ip_addr.addr = addr;
    pdr->ue_ip_addr_len = OGS_IPV4_LEN + 1;
    ABTS_INT_EQUAL(tc, OGS_PFCP_CAUSE_REQUEST_ACCEPTED,
            upf_sess_set_ue_ip(sess, OGS_PDU_SESSION_TYPE_IPV4, pdr));
}

/*
 * A session of the same PFCP node may take over a UE address. When the
 * previous session moves away, the address must still find the new owner.
 */
static void test_ue_ip_takeover(abts_case *tc, void *data)
{
    ogs_pfcp_node_t node;
    upf_sess_t *a = NULL, *b = NULL;
    ogs_pfcp_pdr_t *pdr_a = NULL, *pdr_b = NULL;
    uint32_t x = htobe32(0x0a2d0010), y = htobe32(0x0a2d0020);

    memset(&node, 0, sizeof(node));
    a = ogs_calloc(1, sizeof(*a));
    b = ogs_calloc(1, sizeof(*b));
    ogs_assert(a && b);
    ogs_pfcp_pool_init(&a->pfcp);
    ogs_pfcp_pool_init(&b->pfcp);
    a->pfcp_node = b->pfcp_node = &node;
    pdr_a = ogs_pfcp_pdr_add(&a->pfcp);
    pdr_b = ogs_pfcp_pdr_add(&b->pfcp);

    set_ue_ipv4(tc, a, pdr_a, x);
    set_ue_ipv4(tc, b, pdr_b, x);
    ABTS_PTR_EQUAL(tc, b, upf_sess_find_by_ipv4(x));

    /* The index holds the address itself: its contents and length. */
    ABTS_INT_EQUAL(tc, 1, count_key(upf_self()->ipv4_hash, &x, OGS_IPV4_LEN));

    /* a moves to another address and frees the one it held. */
    set_ue_ipv4(tc, a, pdr_a, y);
    ABTS_PTR_EQUAL(tc, a, upf_sess_find_by_ipv4(y));
    ABTS_PTR_EQUAL(tc, b, upf_sess_find_by_ipv4(x));
    ABTS_INT_EQUAL(tc, 1, count_key(upf_self()->ipv4_hash, &x, OGS_IPV4_LEN));
    ABTS_INT_EQUAL(tc, 1, count_key(upf_self()->ipv4_hash, &y, OGS_IPV4_LEN));

    ogs_hash_set(upf_self()->ipv4_hash, &x, OGS_IPV4_LEN, NULL);
    ogs_hash_set(upf_self()->ipv4_hash, &y, OGS_IPV4_LEN, NULL);
    ogs_pfcp_ue_ip_free(a->ipv4);
    ogs_pfcp_ue_ip_free(b->ipv4);

    ogs_pfcp_sess_clear(&a->pfcp);
    ogs_pfcp_sess_clear(&b->pfcp);
    ogs_pfcp_pool_final(&a->pfcp);
    ogs_pfcp_pool_final(&b->pfcp);
    ogs_free(a);
    ogs_free(b);
}

abts_suite *test_upf_context(abts_suite *suite)
{
    ogs_log_level_e level = ogs_log_get_domain_level(__ogs_pfcp_domain);

    ogs_app_context_init();
    ogs_app()->pool.nf = 8;
    ogs_app()->pool.sess = 8;
    ogs_app()->timer_mgr = ogs_timer_mgr_create(16);
    ogs_pfcp_context_init();
    upf_context_init();
    ogs_assert(ogs_pfcp_subnet_add("10.45.0.1", "16", NULL, NULL, "ogstun"));
    ogs_assert(OGS_OK == ogs_pfcp_ue_pool_generate());
    ogs_log_set_domain_level(__ogs_pfcp_domain, level);

    suite = ADD_SUITE(suite)

    abts_run_test(suite, test_ue_ip_takeover, NULL);

    upf_context_final();
    ogs_pfcp_context_final();
    ogs_timer_mgr_destroy(ogs_app()->timer_mgr);
    ogs_app()->timer_mgr = NULL;
    ogs_app_context_final();

    return suite;
}
