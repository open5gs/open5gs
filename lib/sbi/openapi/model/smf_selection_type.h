/*
 * smf_selection_type.h
 *
 * 
 */

#ifndef _OpenAPI_smf_selection_type_H_
#define _OpenAPI_smf_selection_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_smf_selection_type_NULL = 0, OpenAPI_smf_selection_type_CURRENT_PDU_SESSION, OpenAPI_smf_selection_type_NEXT_PDU_SESSION } OpenAPI_smf_selection_type_e;

char* OpenAPI_smf_selection_type_ToString(OpenAPI_smf_selection_type_e smf_selection_type);

OpenAPI_smf_selection_type_e OpenAPI_smf_selection_type_FromString(char* smf_selection_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_smf_selection_type_H_ */

