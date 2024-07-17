#include "core/abts.h"
#include "gtp/context.h"
#include "ogs-core.h"

static void smf_gtp_node_new_test1(abts_case *tc, void *data) {
    smf_gtp_node_t *smf_gnode = NULL;
    ogs_gtp_node_t gnode;
    ogs_sockaddr_t sa_list[1];
    gnode.sa_list = sa_list;
    gnode.data_ptr = NULL;
    smf_gtp_node_pool.avail = 1;
    smf_gtp_node_pool.free[smf_gtp_node_pool.head] = malloc(sizeof(smf_gtp_node_t));

    smf_gtp_node_t *node = smf_gtp_node_new(&gnode);
    ABTS_PTR_NOTNULL(tc, node);
    ABTS_PTR_EQUAL(tc, node->gnode, &gnode);
    ABTS_PTR_EQUAL(tc, gnode.data_ptr, node);

    free(smf_gtp_node_pool.free[smf_gtp_node_pool.head]);
}

static void smf_gtp_node_new_test2(abts_case *tc, void *data) {
    ogs_gtp_node_t gnode;
    smf_gtp_node_pool.avail = 0;

    smf_gtp_node_t *node = smf_gtp_node_new(&gnode);
    ABTS_PTR_NULL(tc, node);
}

static void smf_gtp_node_new_test3(abts_case *tc, void *data) {
    ogs_gtp_node_t gnode;
    ogs_sockaddr_t sa_list[1];
    gnode.sa_list = sa_list; 
    gnode.data_ptr = NULL; 
    smf_gtp_node_pool.avail = 1;
    smf_gtp_node_pool.free[smf_gtp_node_pool.head] = malloc(sizeof(smf_gtp_node_t));
    smf_metrics_init_inst_gtp_node = [](ogs_metrics_inst_t **metrics, const char *addr) {
        return OGS_ERROR; 
    };

    smf_gtp_node_t *node = smf_gtp_node_new(&gnode);
    ABTS_PTR_NULL(tc, node); 

    free(smf_gtp_node_pool.free[smf_gtp_node_pool.head]);
}

abts_suite *test_smf_gtp_node_new(abts_suite *suite) {
    suite = ADD_SUITE(suite);

    abts_run_test(suite, smf_gtp_node_new_test1, NULL);
    abts_run_test(suite, smf_gtp_node_new_test2, NULL);
    abts_run_test(suite, smf_gtp_node_new_test3, NULL);

    return suite;
}