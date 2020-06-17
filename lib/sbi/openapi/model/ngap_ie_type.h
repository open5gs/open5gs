/*
 * ngap_ie_type.h
 *
 *
 */

#ifndef _OpenAPI_ngap_ie_type_H_
#define _OpenAPI_ngap_ie_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_ngap_ie_type_NULL = 0, OpenAPI_ngap_ie_type_PDU_RES_SETUP_REQ, OpenAPI_ngap_ie_type_PDU_RES_REL_CMD, OpenAPI_ngap_ie_type_PDU_RES_MOD_REQ, OpenAPI_ngap_ie_type_HANDOVER_CMD, OpenAPI_ngap_ie_type_HANDOVER_REQUIRED, OpenAPI_ngap_ie_type_HANDOVER_PREP_FAIL, OpenAPI_ngap_ie_type_SRC_TO_TAR_CONTAINER, OpenAPI_ngap_ie_type_TAR_TO_SRC_CONTAINER, OpenAPI_ngap_ie_type_RAN_STATUS_TRANS_CONTAINER, OpenAPI_ngap_ie_type_SON_CONFIG_TRANSFER, OpenAPI_ngap_ie_type_NRPPA_PDU, OpenAPI_ngap_ie_type_UE_RADIO_CAPABILITY, OpenAPI_ngap_ie_type_RIM_INFO_TRANSFER, OpenAPI_ngap_ie_type_SECONDARY_RAT_USAGE } OpenAPI_ngap_ie_type_e;

char* OpenAPI_ngap_ie_type_ToString(OpenAPI_ngap_ie_type_e ngap_ie_type);

OpenAPI_ngap_ie_type_e OpenAPI_ngap_ie_type_FromString(char* ngap_ie_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ngap_ie_type_H_ */

