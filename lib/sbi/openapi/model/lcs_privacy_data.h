/*
 * lcs_privacy_data.h
 *
 *
 */

#ifndef _OpenAPI_lcs_privacy_data_H_
#define _OpenAPI_lcs_privacy_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "lpi.h"
#include "plmn_operator_class.h"
#include "unrelated_class.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_lcs_privacy_data_s OpenAPI_lcs_privacy_data_t;
typedef struct OpenAPI_lcs_privacy_data_s {
    struct OpenAPI_lpi_s *lpi;
    OpenAPI_list_t *unrelated_classes;
    OpenAPI_list_t *plmn_operator_classes;
} OpenAPI_lcs_privacy_data_t;

OpenAPI_lcs_privacy_data_t *OpenAPI_lcs_privacy_data_create(
    OpenAPI_lpi_t *lpi,
    OpenAPI_list_t *unrelated_classes,
    OpenAPI_list_t *plmn_operator_classes
    );
void OpenAPI_lcs_privacy_data_free(OpenAPI_lcs_privacy_data_t *lcs_privacy_data);
OpenAPI_lcs_privacy_data_t *OpenAPI_lcs_privacy_data_parseFromJSON(cJSON *lcs_privacy_dataJSON);
cJSON *OpenAPI_lcs_privacy_data_convertToJSON(OpenAPI_lcs_privacy_data_t *lcs_privacy_data);
OpenAPI_lcs_privacy_data_t *OpenAPI_lcs_privacy_data_copy(OpenAPI_lcs_privacy_data_t *dst, OpenAPI_lcs_privacy_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_lcs_privacy_data_H_ */

