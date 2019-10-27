#ifndef TEST_APP_H
#define TEST_APP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ogs-diameter-common.h"
#include "ogs-sctp.h"
#include "ogs-dbi.h"
#include "ogs-app.h"
#include "mme/mme-context.h"
#include "core/abts.h"

#include "test-epc.h"
#include "test-packet.h"

void test_app_run(int argc, const char *const argv[],
        const char *name, void (*init)(const char * const argv[]));

void test_app_init(void);
#define test_app_final ogs_mongoc_final

#ifdef __cplusplus
}
#endif

#endif /* TEST_APP_H */
