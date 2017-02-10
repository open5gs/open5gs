#ifndef _S1AP_MESSAGE_H__
#define _S1AP_MESSAGE_H__

#include "s1ap_codecs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define S1AP_SDU_SIZE 2048

CORE_DECLARE(status_t) s1ap_build_setup_rsp(pkbuf_t **pkbuf);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

