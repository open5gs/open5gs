/*
 * enhanced_coverage_restriction_data.h
 *
 * 
 */

#ifndef _OpenAPI_enhanced_coverage_restriction_data_H_
#define _OpenAPI_enhanced_coverage_restriction_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "plmn_ec_info.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_enhanced_coverage_restriction_data_s OpenAPI_enhanced_coverage_restriction_data_t;
typedef struct OpenAPI_enhanced_coverage_restriction_data_s {
    OpenAPI_list_t *plmn_ec_info_list;
} OpenAPI_enhanced_coverage_restriction_data_t;

OpenAPI_enhanced_coverage_restriction_data_t *OpenAPI_enhanced_coverage_restriction_data_create(
    OpenAPI_list_t *plmn_ec_info_list
);
void OpenAPI_enhanced_coverage_restriction_data_free(OpenAPI_enhanced_coverage_restriction_data_t *enhanced_coverage_restriction_data);
OpenAPI_enhanced_coverage_restriction_data_t *OpenAPI_enhanced_coverage_restriction_data_parseFromJSON(cJSON *enhanced_coverage_restriction_dataJSON);
cJSON *OpenAPI_enhanced_coverage_restriction_data_convertToJSON(OpenAPI_enhanced_coverage_restriction_data_t *enhanced_coverage_restriction_data);
OpenAPI_enhanced_coverage_restriction_data_t *OpenAPI_enhanced_coverage_restriction_data_copy(OpenAPI_enhanced_coverage_restriction_data_t *dst, OpenAPI_enhanced_coverage_restriction_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_enhanced_coverage_restriction_data_H_ */

