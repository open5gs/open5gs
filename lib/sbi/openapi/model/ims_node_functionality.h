/*
 * ims_node_functionality.h
 *
 * 
 */

#ifndef _OpenAPI_ims_node_functionality_H_
#define _OpenAPI_ims_node_functionality_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_ims_node_functionality_NULL = 0, OpenAPI_ims_node_functionality_S_CSCF, OpenAPI_ims_node_functionality_P_CSCF, OpenAPI_ims_node_functionality_I_CSCF, OpenAPI_ims_node_functionality_MRFC, OpenAPI_ims_node_functionality_MGCF, OpenAPI_ims_node_functionality_BGCF, OpenAPI_ims_node_functionality_AS, OpenAPI_ims_node_functionality_IBCF, OpenAPI_ims_node_functionality_S_GW, OpenAPI_ims_node_functionality_P_GW, OpenAPI_ims_node_functionality_HSGW, OpenAPI_ims_node_functionality_E_CSCF, OpenAPI_ims_node_functionality_MME, OpenAPI_ims_node_functionality_TRF, OpenAPI_ims_node_functionality_TF, OpenAPI_ims_node_functionality_ATCF, OpenAPI_ims_node_functionality_PROXY, OpenAPI_ims_node_functionality_EPDG, OpenAPI_ims_node_functionality_TDF, OpenAPI_ims_node_functionality_TWAG, OpenAPI_ims_node_functionality_SCEF, OpenAPI_ims_node_functionality_IWK_SCEF, OpenAPI_ims_node_functionality_IMS_GWF, OpenAPI_ims_node_functionality_DCSF } OpenAPI_ims_node_functionality_e;

char* OpenAPI_ims_node_functionality_ToString(OpenAPI_ims_node_functionality_e ims_node_functionality);

OpenAPI_ims_node_functionality_e OpenAPI_ims_node_functionality_FromString(char* ims_node_functionality);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ims_node_functionality_H_ */

