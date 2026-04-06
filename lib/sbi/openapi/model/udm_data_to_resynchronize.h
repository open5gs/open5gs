/*
 * udm_data_to_resynchronize.h
 *
 * data to be resynchronized
 */

#ifndef _OpenAPI_udm_data_to_resynchronize_H_
#define _OpenAPI_udm_data_to_resynchronize_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_udm_data_to_resynchronize_NULL = 0, OpenAPI_udm_data_to_resynchronize_UDM_UECM_REGISTRATION, OpenAPI_udm_data_to_resynchronize_UDM_SDM_SUBSCRIBE, OpenAPI_udm_data_to_resynchronize_UDM_EE_SUBSCRIBE } OpenAPI_udm_data_to_resynchronize_e;

char* OpenAPI_udm_data_to_resynchronize_ToString(OpenAPI_udm_data_to_resynchronize_e udm_data_to_resynchronize);

OpenAPI_udm_data_to_resynchronize_e OpenAPI_udm_data_to_resynchronize_FromString(char* udm_data_to_resynchronize);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_udm_data_to_resynchronize_H_ */

