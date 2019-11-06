#ifndef TEST_EPC_H
#define TEST_EPC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ogs-app.h"

void test_child_terminate(void);
ogs_thread_t *test_child_create(const char *name, const char *const argv[]);

#ifdef __cplusplus
}
#endif

#endif /* TEST_EPC_H */
