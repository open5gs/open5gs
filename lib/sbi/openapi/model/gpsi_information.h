/*
 * gpsi_information.h
 *
 * List of Gpsis.
 */

#ifndef _OpenAPI_gpsi_information_H_
#define _OpenAPI_gpsi_information_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_gpsi_information_s OpenAPI_gpsi_information_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_gpsi_information_s {
    OpenAPI_list_t *gpsi_list;
};

OpenAPI_gpsi_information_t *OpenAPI_gpsi_information_create(
    OpenAPI_list_t *gpsi_list
);
void OpenAPI_gpsi_information_free(OpenAPI_gpsi_information_t *gpsi_information);
OpenAPI_gpsi_information_t *OpenAPI_gpsi_information_parseFromJSON(cJSON *gpsi_informationJSON);
cJSON *OpenAPI_gpsi_information_convertToJSON(OpenAPI_gpsi_information_t *gpsi_information);
OpenAPI_gpsi_information_t *OpenAPI_gpsi_information_copy(OpenAPI_gpsi_information_t *dst, OpenAPI_gpsi_information_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_gpsi_information_H_ */

