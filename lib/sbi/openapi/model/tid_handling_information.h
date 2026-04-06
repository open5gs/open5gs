/*
 * tid_handling_information.h
 *
 * Contains the T-ID handling information
 */

#ifndef _OpenAPI_tid_handling_information_H_
#define _OpenAPI_tid_handling_information_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_tid_handling_information_s OpenAPI_tid_handling_information_t;
#include "tid_type.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_tid_handling_information_s {
    OpenAPI_tid_type_e tid_type;
    bool is_tid_handling_information;
    int tid_handling_information;
};

OpenAPI_tid_handling_information_t *OpenAPI_tid_handling_information_create(
    OpenAPI_tid_type_e tid_type,
    bool is_tid_handling_information,
    int tid_handling_information
);
void OpenAPI_tid_handling_information_free(OpenAPI_tid_handling_information_t *tid_handling_information);
OpenAPI_tid_handling_information_t *OpenAPI_tid_handling_information_parseFromJSON(cJSON *tid_handling_informationJSON);
cJSON *OpenAPI_tid_handling_information_convertToJSON(OpenAPI_tid_handling_information_t *tid_handling_information);
OpenAPI_tid_handling_information_t *OpenAPI_tid_handling_information_copy(OpenAPI_tid_handling_information_t *dst, OpenAPI_tid_handling_information_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_tid_handling_information_H_ */

