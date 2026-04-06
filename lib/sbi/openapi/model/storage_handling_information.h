/*
 * storage_handling_information.h
 *
 * Contains storage handling information about data or analytics.
 */

#ifndef _OpenAPI_storage_handling_information_H_
#define _OpenAPI_storage_handling_information_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_storage_handling_information_s OpenAPI_storage_handling_information_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_storage_handling_information_s {
    bool is_lifetime;
    int lifetime;
    bool is_del_notif_ind;
    int del_notif_ind;
};

OpenAPI_storage_handling_information_t *OpenAPI_storage_handling_information_create(
    bool is_lifetime,
    int lifetime,
    bool is_del_notif_ind,
    int del_notif_ind
);
void OpenAPI_storage_handling_information_free(OpenAPI_storage_handling_information_t *storage_handling_information);
OpenAPI_storage_handling_information_t *OpenAPI_storage_handling_information_parseFromJSON(cJSON *storage_handling_informationJSON);
cJSON *OpenAPI_storage_handling_information_convertToJSON(OpenAPI_storage_handling_information_t *storage_handling_information);
OpenAPI_storage_handling_information_t *OpenAPI_storage_handling_information_copy(OpenAPI_storage_handling_information_t *dst, OpenAPI_storage_handling_information_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_storage_handling_information_H_ */

