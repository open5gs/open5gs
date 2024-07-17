/*Nie byłem w stanie poprawnie zlinkować structów + funkcji z context.h*/
#define ASSERT_BACKPOINTER(ptr) \
    if ((ptr) != NULL) { \
        fprintf(stderr, "Assertion failed: %s is NULL (file %s, line %d)\n", #ptr, __FILE__, __LINE__); \
        exit(EXIT_FAILURE); \
    }

#define ASSERT_METRICS_FREE(ptr) \
    if ((ptr) != 0) { \
        fprintf(stderr, "Assertion failed: %s is not 0 (file %s, line %d)\n", #ptr, __FILE__, __LINE__); \
        exit(EXIT_FAILURE); \
    }

#include "core/abts.h"

typedef enum smf_metric_type_gtp_node_s {
    SMF_METR_GTP_NODE_CTR_GN_RX_PARSE_FAILED = 0,
    SMF_METR_GTP_NODE_CTR_GN_RX_CREATEPDPCTXREQ,
    SMF_METR_GTP_NODE_CTR_GN_RX_DELETEPDPCTXREQ,
    SMF_METR_GTP_NODE_CTR_S5C_RX_PARSE_FAILED,
    SMF_METR_GTP_NODE_CTR_S5C_RX_CREATESESSIONREQ,
    SMF_METR_GTP_NODE_CTR_S5C_RX_DELETESESSIONREQ,
    _SMF_METR_GTP_NODE_MAX,
} smf_metric_type_gtp_node_t;

typedef struct ogs_gtp_node_s {
            /* A node of list_t */
    void            *data_ptr;      /* Can be used by app */
} ogs_gtp_node_t;

typedef struct ogs_metrics_inst_s {
    int unused;
} ogs_metrics_inst_t;

typedef struct smf_gtp_node_s {
    ogs_gtp_node_t *gnode;
    ogs_metrics_inst_t *metrics[_SMF_METR_GTP_NODE_MAX];
} smf_gtp_node_t;


void smf_gtp_node_free(smf_gtp_node_t *smf_gnode){

};

static void test1_smf_gtp_node_free(){
    smf_gtp_node_t test_node;
    ogs_gtp_node_t gnode;

    test_node.gnode = &gnode;
    smf_gtp_node_free(&test_node);
    ASSERT_BACKPOINTER(test_node.gnode->data_ptr);

}

static void test2_smf_gtp_node_free(){
    int i;
    smf_gtp_node_t test_node;
    ogs_metrics_inst_t *metrics[_SMF_METR_GTP_NODE_MAX];
   
    for (i = 0; i < _SMF_METR_GTP_NODE_MAX; i++) {
        metrics[i] = (ogs_metrics_inst_t *) malloc(sizeof(ogs_metrics_inst_t));
        if (metrics[i] == NULL) {
            exit(EXIT_FAILURE);
        }
    }

    for (i = 0; i < _SMF_METR_GTP_NODE_MAX; i++) {
        metrics[i]->unused = 0; 
    }

    for (i = 0; i<_SMF_METR_GTP_NODE_MAX;i++){
         test_node.metrics[i] = metrics[i];
    }
   
    smf_gtp_node_free(&test_node);

    for (i = 0; i<_SMF_METR_GTP_NODE_MAX;i++){
         ASSERT_METRICS_FREE(test_node.metrics[i]->unused);
    }
    
}
abts_suite *test_smf_gtp_node_free(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, test1_smf_gtp_node_free, NULL);
    abts_run_test(suite, test2_smf_gtp_node_free, NULL);

    return suite;
}


