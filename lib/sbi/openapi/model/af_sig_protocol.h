/*
 * af_sig_protocol.h
 *
 * Possible values are - NO_INFORMATION: Indicate that no information about the AF signalling protocol is being provided.  - SIP: Indicate that the signalling protocol is Session Initiation Protocol.
 */

#ifndef _OpenAPI_af_sig_protocol_H_
#define _OpenAPI_af_sig_protocol_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "null_value.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_af_sig_protocol_s OpenAPI_af_sig_protocol_t;
typedef struct OpenAPI_af_sig_protocol_s {
} OpenAPI_af_sig_protocol_t;

OpenAPI_af_sig_protocol_t *OpenAPI_af_sig_protocol_create(
    );
void OpenAPI_af_sig_protocol_free(OpenAPI_af_sig_protocol_t *af_sig_protocol);
OpenAPI_af_sig_protocol_t *OpenAPI_af_sig_protocol_parseFromJSON(cJSON *af_sig_protocolJSON);
cJSON *OpenAPI_af_sig_protocol_convertToJSON(OpenAPI_af_sig_protocol_t *af_sig_protocol);
OpenAPI_af_sig_protocol_t *OpenAPI_af_sig_protocol_copy(OpenAPI_af_sig_protocol_t *dst, OpenAPI_af_sig_protocol_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_af_sig_protocol_H_ */

