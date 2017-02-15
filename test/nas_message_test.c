#include "core.h"
#include "core_errno.h"
#include "core_general.h"
#include "core_debug.h"
#include "core_pkbuf.h"

#include "testutil.h"

#include "nas_message.h"

static void nas_message_test1(abts_case *tc, void *data)
{
    /* Attach Request */
    char *payload[] = {
        "\x17\xdf\x67\x5a\xa8\x05\x07\x41\x02\x0b\xf6\x00\xf1\x10\x00\x02\x01\x03\x00\x03\xe6\x05\xf0\x70\x00\x00\x10\x00\x05\x02\x15\xd0\x11\xd1\x52\x00\xf1\x10\x30\x39\x5c\x0a\x00\x31\x03\xe5\xe0\x34\x90\x11\x03\x57\x58\xa6\x5d\x01\x00\xe0\xc1"
    };

    int i = 0; 

    nas_message_t message;
    pkbuf_t *pkbuf;
    status_t rv;

    pkbuf = pkbuf_alloc(0, NAS_SDU_SIZE);
    ABTS_PTR_NOTNULL(tc, pkbuf);
    pkbuf->len = 59;
    memcpy(pkbuf->payload, payload[0], pkbuf->len);

    rv = nas_decode_pdu(&message, pkbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    pkbuf_free(pkbuf);
}

abts_suite *test_nas_message(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, nas_message_test1, NULL);

    return suite;
}
