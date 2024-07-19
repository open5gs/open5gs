#include "ogs-core.h"
#include "core/abts.h"
#include "smf/event.h"
#include "smf/context.h"
#include "smf/context.c"

#define SESS_POOL_SIZE 10
#define BEARER_POOL_SIZE 10
#define METRICS_MAX_SPECS 512

smf_sess_t *sess = NULL;
smf_bearer_t *bearer[BEARER_POOL_SIZE];

void ogs_pfcp_pdr_remove(ogs_pfcp_pdr_t *pdr){}
void ogs_pfcp_far_remove(ogs_pfcp_far_t *far){}
void ogs_pfcp_urr_remove(ogs_pfcp_urr_t *urr){}
void ogs_pfcp_qer_remove(ogs_pfcp_qer_t *qer){}
int ogs_freeaddrinfo(ogs_sockaddr_t *sa_list){return OGS_OK;}
int ogs_talloc_free(void *ptr, const char *location){ return 0;}

void init(void)
{
    ogs_app()->metrics.max_specs = METRICS_MAX_SPECS;
    smf_metrics_init();

    ogs_pool_init(&smf_sess_pool, SESS_POOL_SIZE);
    ogs_pool_init(&smf_bearer_pool, BEARER_POOL_SIZE);

    ogs_pool_id_calloc(&smf_sess_pool, &sess);
}

void deinit(void)
{
    ogs_pool_id_free(&smf_sess_pool, sess);

    for(int idx = 0; idx < BEARER_POOL_SIZE; idx++)
    {
        ogs_pool_id_free(&smf_bearer_pool, bearer[idx]);
    }
    
    ogs_pool_final(&smf_sess_pool);
    ogs_pool_final(&smf_bearer_pool);
}

smf_bearer_t *create_bearer(void)
{
    smf_bearer_t *bearer = NULL;
    ogs_pool_id_calloc(&smf_bearer_pool, &bearer);

    smf_pf_identifier_pool_init(bearer);

    bearer->dl_pdr = (ogs_pfcp_pdr_t*) malloc (sizeof(ogs_pfcp_pdr_t));
    bearer->ul_pdr = (ogs_pfcp_pdr_t*) malloc (sizeof(ogs_pfcp_pdr_t));
    bearer->dl_far = (ogs_pfcp_far_t*) malloc (sizeof(ogs_pfcp_far_t));
    bearer->ul_far = (ogs_pfcp_far_t*) malloc (sizeof(ogs_pfcp_far_t));
    bearer->pcc_rule.name = (char *) malloc(sizeof(char));
    bearer->pcc_rule.id = (char *) malloc(sizeof(char));
    bearer->pgw_s5u_addr = (ogs_sockaddr_t*) malloc (sizeof(ogs_sockaddr_t));
    bearer->pgw_s5u_addr6 = (ogs_sockaddr_t*) malloc (sizeof(ogs_sockaddr_t));
    bearer->qfi_node = 0;
    ogs_list_init(&bearer->pf_list);

    bearer->sess_id = sess->id;
    ogs_list_add(&sess->bearer_list, bearer);

    return bearer;
}

void _test_smf_bearer_remove(abts_case *tc, void *data) {
    smf_bearer_t* bearer = create_bearer();
    int rv = smf_bearer_remove(bearer);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
}

void _test_smf_bearer_remove_all(abts_case *tc, void *data) 
{
    for(int idx = 0; idx < BEARER_POOL_SIZE; idx++)
    {
        bearer[idx] = create_bearer();
    }

    smf_bearer_remove_all(sess);
}

abts_suite *test_smf_bearer_remove(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    init();

    abts_run_test(suite, _test_smf_bearer_remove, NULL);
    abts_run_test(suite, _test_smf_bearer_remove_all, NULL);

    deinit();

    return suite;
}