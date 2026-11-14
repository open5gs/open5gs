/*
 * Copyright (C) 2026 by Daniel Brune <daniel.brune.so@outlook.com>
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
 * TS 29.244 6.2.6.2: PFCP Session Retention Information in the PFCP
 * Association Setup Request of the CP function, decided from the sessions
 * bound to the node (node->num_of_sessions) and the sessions kept when
 * the node failed (node->num_of_sessions_kept).
 */

#include "ogs-pfcp.h"
#include "core/abts.h"

typedef struct test_sess_s {
    ogs_pfcp_node_t *pfcp_node;
} test_sess_t;

static ogs_timer_mgr_t *timer_mgr;
static ogs_pfcp_node_t node;

static void test_bind(test_sess_t *sess, ogs_pfcp_node_t *pfcp_node)
{
    OGS_SETUP_PFCP_NODE(sess, pfcp_node);
}

static void test_unbind(test_sess_t *sess)
{
    OGS_CLEAR_PFCP_NODE(sess);
}

static void node_init(void)
{
    memset(&node, 0, sizeof(node));

    ogs_assert(OGS_OK == ogs_getaddrinfo(
                &node.addr_list, AF_INET, "127.0.0.7", 8805, 0));
    node.t_release_after_peer_failure =
        ogs_timer_add(timer_mgr, NULL, NULL);
    ogs_assert(node.t_release_after_peer_failure);
}

static void node_final(void)
{
    ogs_timer_delete(node.t_release_after_peer_failure);
    ogs_freeaddrinfo(node.addr_list);
}

static void test_setup(abts_case *tc, void *data)
{
    timer_mgr = ogs_timer_mgr_create(4);
    ogs_assert(timer_mgr);

    /* pfcp.release_after_peer_failure: 30 */
    ogs_pfcp_self()->release_after_peer_failure = ogs_time_from_sec(30);
}

static void test_teardown(abts_case *tc, void *data)
{
    ogs_pfcp_self()->release_after_peer_failure = 0;

    ogs_timer_mgr_destroy(timer_mgr);
}

/* Binding and removal of sessions are counted once per node */
static void test_num_of_sessions(abts_case *tc, void *data)
{
    ogs_pfcp_node_t other;
    test_sess_t sess[2];

    node_init();
    memset(&other, 0, sizeof(other));
    memset(sess, 0, sizeof(sess));

    test_bind(&sess[0], &node);
    test_bind(&sess[0], &node);
    test_bind(&sess[1], &node);
    ABTS_INT_EQUAL(tc, 2, node.num_of_sessions);

    test_bind(&sess[1], &other);
    ABTS_INT_EQUAL(tc, 1, node.num_of_sessions);
    ABTS_INT_EQUAL(tc, 1, other.num_of_sessions);

    test_unbind(&sess[0]);
    test_unbind(&sess[0]);
    test_unbind(&sess[1]);
    ABTS_INT_EQUAL(tc, 0, node.num_of_sessions);
    ABTS_INT_EQUAL(tc, 0, other.num_of_sessions);
    ABTS_TRUE(tc, sess[0].pfcp_node == NULL);
    ABTS_TRUE(tc, sess[1].pfcp_node == NULL);

    node_final();
}

/* All kept sessions still there: request to retain them */
static void test_all_kept(abts_case *tc, void *data)
{
    test_sess_t sess[2];

    node_init();
    memset(sess, 0, sizeof(sess));

    test_bind(&sess[0], &node);
    test_bind(&sess[1], &node);

    ogs_pfcp_node_start_release_after_peer_failure(&node);
    ABTS_INT_EQUAL(tc, 2, node.num_of_sessions_kept);
    ABTS_TRUE(tc, node.sessions_kept == true);
    ABTS_TRUE(tc, ogs_pfcp_node_retain_sessions(&node) == true);

    /* Also after an association set up by the UP function */
    ABTS_TRUE(tc, ogs_pfcp_node_retention_pending(&node) == true);

    test_unbind(&sess[0]);
    test_unbind(&sess[1]);
    node_final();
}

/*
 * A kept session removed during the failure: do not request retention,
 * the UP function would retain the removed one as well
 */
static void test_one_removed(abts_case *tc, void *data)
{
    test_sess_t sess[2];

    node_init();
    memset(sess, 0, sizeof(sess));

    test_bind(&sess[0], &node);
    test_bind(&sess[1], &node);

    ogs_pfcp_node_start_release_after_peer_failure(&node);
    test_unbind(&sess[1]);

    ABTS_INT_EQUAL(tc, 1, node.num_of_sessions);
    ABTS_INT_EQUAL(tc, 2, node.num_of_sessions_kept);
    ABTS_TRUE(tc, node.sessions_kept == true);
    ABTS_TRUE(tc, ogs_pfcp_node_retain_sessions(&node) == false);

    /*
     * The UP function sets up the association first: the decision is
     * still pending, so the CP function follows with its own request
     * (without the IE), and the node is not selectable meanwhile
     */
    ABTS_TRUE(tc, ogs_pfcp_node_retention_pending(&node) == true);

    /*
     * The node fails again before the CP and the UP function agreed on
     * the sessions: the removal still counts
     */
    ogs_pfcp_node_start_release_after_peer_failure(&node);
    ABTS_INT_EQUAL(tc, 2, node.num_of_sessions_kept);
    ABTS_TRUE(tc, ogs_pfcp_node_retain_sessions(&node) == false);

    test_unbind(&sess[0]);
    node_final();
}

/*
 * The timer expired and the sessions are being released (not to be
 * restored), then the node fails again before the CP and the UP function
 * agreed: they stay released
 */
static void test_released_then_failed_again(abts_case *tc, void *data)
{
    test_sess_t sess[2];

    node_init();
    memset(sess, 0, sizeof(sess));

    test_bind(&sess[0], &node);
    test_bind(&sess[1], &node);

    ogs_pfcp_node_start_release_after_peer_failure(&node);

    /* Expiry (see the SMF), one session is still waiting for its release */
    node.sessions_kept = false;
    test_unbind(&sess[1]);

    ogs_pfcp_node_start_release_after_peer_failure(&node);
    ABTS_INT_EQUAL(tc, 2, node.num_of_sessions_kept);
    ABTS_TRUE(tc, node.sessions_kept == false);
    ABTS_TRUE(tc, ogs_pfcp_node_retain_sessions(&node) == false);
    ABTS_TRUE(tc, ogs_pfcp_node_retention_pending(&node) == true);

    test_unbind(&sess[0]);
    node_final();
}

/* No session when the node failed: nothing to retain */
static void test_no_sessions(abts_case *tc, void *data)
{
    node_init();

    ogs_pfcp_node_start_release_after_peer_failure(&node);
    ABTS_INT_EQUAL(tc, 0, node.num_of_sessions_kept);
    ABTS_TRUE(tc, node.sessions_kept == false);
    ABTS_TRUE(tc, ogs_pfcp_node_retain_sessions(&node) == false);

    /* Nothing to agree on: an association set up by the UP function is
     * enough, and the node is selectable */
    ABTS_TRUE(tc, ogs_pfcp_node_retention_pending(&node) == false);

    node_final();
}

abts_suite *test_pfcp_retention(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, test_setup, NULL);
    abts_run_test(suite, test_num_of_sessions, NULL);
    abts_run_test(suite, test_all_kept, NULL);
    abts_run_test(suite, test_one_removed, NULL);
    abts_run_test(suite, test_released_then_failed_again, NULL);
    abts_run_test(suite, test_no_sessions, NULL);
    abts_run_test(suite, test_teardown, NULL);

    return suite;
}
