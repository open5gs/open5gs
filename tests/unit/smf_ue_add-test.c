#include "core/abts.h"
#include "ogs-pool.h"
#include "ogs-core.h"
#include "smf/context.h"

void test_smf_ue_add_success(abts_case *tc, void *value) {
    smf_context_init(); 

    smf_ue_t *ue = smf_ue_add();
    ABTS_PTR_NOTNULL(tc, ue);
    if (ue) {
        ogs_list_remove(&self.smf_ue_list, ue);
        ogs_pool_free(&smf_ue_pool, ue);
    }

    smf_context_final(); 
}

void test_smf_ue_add_initialization(abts_case *tc, void *value) {
    smf_context_init(); 

    smf_ue_t *ue = smf_ue_add();
    ABTS_PTR_NOTNULL(tc, ue);
    if (ue) {
        ABTS_TRUE(tc,ogs_list_empty(&ue->sess_list));
        ogs_list_remove(&self.smf_ue_list, ue);
        ogs_pool_free(&smf_ue_pool, ue);
    }

    smf_context_final(); 
}

void test_smf_ue_add_pool_limit(abts_case *tc, void *value) {

    smf_context_init(); 

    ogs_global_conf()->max.ue = 1; 
    smf_ue_t *ue = smf_ue_add();
    ABTS_PTR_NOTNULL(tc, ue);
    smf_ue_t *ue_overflow = smf_ue_add();
    ABTS_PTR_EQUAL(tc, NULL, ue_overflow);
    if (ue) {
        ogs_list_remove(&self.smf_ue_list, ue);
        ogs_pool_free(&smf_ue_pool, ue);
    }

    smf_context_final();
}

void test_smf_ue_add_different_pool_sizes(abts_case *tc, void *value) {

    smf_context_init();

    int pool_sizes[] = {1, 5, 10};
    for (int i = 0; i < sizeof(pool_sizes)/sizeof(pool_sizes[0]); i++) {
        ogs_global_conf()->max.ue = pool_sizes[i];
        smf_ue_t *ue = NULL;
        for (int j = 0; j < pool_sizes[i]; j++) {
            ue = smf_ue_add();
            ABTS_PTR_NOTNULL(tc, ue);
            if (ue) {
                ogs_list_remove(&self.smf_ue_list, ue);
                ogs_pool_free(&smf_ue_pool, ue);
            }
        }
        smf_ue_t *ue_overflow = smf_ue_add();
        ABTS_PTR_EQUAL(tc, NULL, ue_overflow);
    }

    smf_context_final(); 
}

void test_smf_ue_add_multiple(abts_case *tc, void *value) {
    smf_context_init();

    ogs_global_conf()->max.ue = 3;
    smf_ue_t *ue1 = smf_ue_add();
    ABTS_PTR_NOTNULL(tc, ue1);
    smf_ue_t *ue2 = smf_ue_add();
    ABTS_PTR_NOTNULL(tc, ue2);
    smf_ue_t *ue3 = smf_ue_add();
    ABTS_PTR_NOTNULL(tc, ue3);

    if (ue1) {
        ogs_list_remove(&self.smf_ue_list, ue1);
        ogs_pool_free(&smf_ue_pool, ue1);
    }
    if (ue2) {
        ogs_list_remove(&self.smf_ue_list, ue2);
        ogs_pool_free(&smf_ue_pool, ue2);
    }
    if (ue3) {
        ogs_list_remove(&self.smf_ue_list, ue3);
        ogs_pool_free(&smf_ue_pool, ue3);
    }

    smf_context_final();
}

abts_suite *test_smf_ue_add(abts_suite *suite) {

    suite = ADD_SUITE(suite);

    abts_run_test(suite, test_smf_ue_add_success, NULL);
    abts_run_test(suite, test_smf_ue_add_initialization, NULL);
    abts_run_test(suite, test_smf_ue_add_pool_limit, NULL);
    abts_run_test(suite, test_smf_ue_add_different_pool_sizes, NULL);
    abts_run_test(suite, test_smf_ue_add_multiple, NULL); 

    return suite;
}



