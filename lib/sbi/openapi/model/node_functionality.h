/*
 * node_functionality.h
 *
 * 
 */

#ifndef _OpenAPI_node_functionality_H_
#define _OpenAPI_node_functionality_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_node_functionality_NULL = 0, OpenAPI_node_functionality_AMF, OpenAPI_node_functionality_SMF, OpenAPI_node_functionality_SMS, OpenAPI_node_functionality_SMSF, OpenAPI_node_functionality_PGW_C_SMF, OpenAPI_node_functionality_NEFF, OpenAPI_node_functionality_SGW, OpenAPI_node_functionality_I_SMF, OpenAPI_node_functionality_ePDG, OpenAPI_node_functionality_CEF, OpenAPI_node_functionality_NEF, OpenAPI_node_functionality_MnS_Producer, OpenAPI_node_functionality_SGSN, OpenAPI_node_functionality_V_SMF, OpenAPI_node_functionality__5G_DDNMF, OpenAPI_node_functionality_IMS_Node, OpenAPI_node_functionality_MMS_Node, OpenAPI_node_functionality_EES, OpenAPI_node_functionality_PCF, OpenAPI_node_functionality_UDM, OpenAPI_node_functionality_UPF, OpenAPI_node_functionality_TSN_AF, OpenAPI_node_functionality_TSCTSF, OpenAPI_node_functionality_MB_SMF, OpenAPI_node_functionality_CHF, OpenAPI_node_functionality_GMLC, OpenAPI_node_functionality_AIOTF, OpenAPI_node_functionality_CCF, OpenAPI_node_functionality_NWDAF } OpenAPI_node_functionality_e;

char* OpenAPI_node_functionality_ToString(OpenAPI_node_functionality_e node_functionality);

OpenAPI_node_functionality_e OpenAPI_node_functionality_FromString(char* node_functionality);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_node_functionality_H_ */

