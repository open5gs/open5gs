#include "smf/context.h"
#include "core/abts.h"
#include "ogs-core.h"

// i have troubles with context_init 
// no matter what i do, i always get "Maximum number of smf_ue reached" message when adding ue
// even though no ue were previously added
// pushing it to branch to show i tried to do the assignment, maybe i'll come up with an idea how to do init correctly later

static void test_simple_delete(abts_case *tc, void *data){
    smf_context_init();
    ogs_global_conf()->max.ue = 1;
    smf_context_t *self = smf_self();
    smf_ue_t *ue = smf_ue_add();

    smf_ue_remove(ue);
    ABTS_PTR_NOTNULL(tc, ue);

    smf_context_final();
}

static void test_simple_delete_multiple(abts_case *tc, void *data){
    smf_context_init();
    ogs_global_conf()->max.ue = 3;
    smf_ue_t *ue1 = smf_ue_add();
    smf_ue_t *ue2 = smf_ue_add();
    smf_ue_t *ue3 = smf_ue_add();

    smf_ue_remove(ue1);
    ABTS_PTR_NOTNULL(tc, ue1);
    smf_ue_remove(ue2);
    ABTS_PTR_NOTNULL(tc, ue2);
    smf_ue_remove(ue3);
    ABTS_PTR_NOTNULL(tc, ue3);

    smf_context_final();
}


static void test_remove_all(abts_case *tc, void *data){
    smf_context_init();
    ogs_global_conf()->max.ue = 3;
    smf_ue_t *ue1 = smf_ue_add();
    smf_ue_t *ue2 = smf_ue_add();
    smf_ue_t *ue3 = smf_ue_add();

    smf_ue_remove_all();

    ABTS_PTR_NOTNULL(tc, ue1);
    ABTS_PTR_NOTNULL(tc, ue2);
    ABTS_PTR_NOTNULL(tc, ue3);

    smf_context_final();
}

// this test shouldnt work, will throw an error
static void remove_impossible(abts_case *tc, void *data){
    smf_context_init();
    ogs_global_conf()->max.ue = 1;
    smf_ue_t *ue = smf_ue_add();

    smf_ue_remove(ue);
    smf_ue_remove(ue); // impossible 
    ABTS_PTR_NOTNULL(tc, ue);

    smf_ue_remove(NULL); // impossible

    smf_context_final();
}

abts_suite *test_smf_ue_delete(abts_suite *suite){
    suite = ADD_SUITE(suite)

    abts_run_test(suite, test_simple_delete, NULL);
    abts_run_test(suite, test_simple_delete_multiple, NULL);
    abts_run_test(suite, remove_impossible, NULL);
    abts_run_test(suite, test_remove_all, NULL);

    return suite;
}