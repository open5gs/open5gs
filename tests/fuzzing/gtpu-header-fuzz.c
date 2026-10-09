#include <stdint.h>

#include "fuzzing.h"
#include "ogs-gtp.h"

#define kMinInputLength OGS_GTPV1U_HEADER_LEN
#define kMaxInputLength 2048

extern int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size)
{
    ogs_pkbuf_t *pkbuf;
    ogs_gtp2_header_desc_t header;

    if (Size < kMinInputLength || Size > kMaxInputLength)
        return 0;

    if (!initialized) {
        initialize();
        ogs_log_install_domain(&__ogs_gtp_domain, "gtp", OGS_LOG_NONE);
    }

    pkbuf = ogs_pkbuf_alloc(NULL, Size);
    if (pkbuf == NULL)
        return 0;

    ogs_pkbuf_put_data(pkbuf, Data, Size);
    ogs_gtpu_parse_header(&header, pkbuf);
    ogs_gtpu_parse_header(NULL, pkbuf);
    ogs_pkbuf_free(pkbuf);

    return 0;
}