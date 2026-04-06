/*
 * pru_existence_info.h
 *
 * PRU Existence Information
 */

#ifndef _OpenAPI_pru_existence_info_H_
#define _OpenAPI_pru_existence_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_pru_existence_info_s OpenAPI_pru_existence_info_t;
#include "tai.h"
#include "tai_range.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_pru_existence_info_s {
    OpenAPI_list_t *tai_list;
    OpenAPI_list_t *tai_range_list;
};

OpenAPI_pru_existence_info_t *OpenAPI_pru_existence_info_create(
    OpenAPI_list_t *tai_list,
    OpenAPI_list_t *tai_range_list
);
void OpenAPI_pru_existence_info_free(OpenAPI_pru_existence_info_t *pru_existence_info);
OpenAPI_pru_existence_info_t *OpenAPI_pru_existence_info_parseFromJSON(cJSON *pru_existence_infoJSON);
cJSON *OpenAPI_pru_existence_info_convertToJSON(OpenAPI_pru_existence_info_t *pru_existence_info);
OpenAPI_pru_existence_info_t *OpenAPI_pru_existence_info_copy(OpenAPI_pru_existence_info_t *dst, OpenAPI_pru_existence_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pru_existence_info_H_ */

