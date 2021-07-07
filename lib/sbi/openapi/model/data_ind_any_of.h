/*
 * data_ind_any_of.h
 *
 * 
 */

#ifndef _OpenAPI_data_ind_any_of_H_
#define _OpenAPI_data_ind_any_of_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_data_ind_any_of_NULL = 0, OpenAPI_data_ind_any_of_PFD, OpenAPI_data_ind_any_of_IPTV, OpenAPI_data_ind_any_of_BDT, OpenAPI_data_ind_any_of_SVC_PARAM } OpenAPI_data_ind_any_of_e;

char* OpenAPI_data_ind_any_of_ToString(OpenAPI_data_ind_any_of_e data_ind_any_of);

OpenAPI_data_ind_any_of_e OpenAPI_data_ind_any_of_FromString(char* data_ind_any_of);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_data_ind_any_of_H_ */

