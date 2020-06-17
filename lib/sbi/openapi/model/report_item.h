/*
 * report_item.h
 *
 *
 */

#ifndef _OpenAPI_report_item_H_
#define _OpenAPI_report_item_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_report_item_s OpenAPI_report_item_t;
typedef struct OpenAPI_report_item_s {
    char *path;
} OpenAPI_report_item_t;

OpenAPI_report_item_t *OpenAPI_report_item_create(
    char *path
    );
void OpenAPI_report_item_free(OpenAPI_report_item_t *report_item);
OpenAPI_report_item_t *OpenAPI_report_item_parseFromJSON(cJSON *report_itemJSON);
cJSON *OpenAPI_report_item_convertToJSON(OpenAPI_report_item_t *report_item);
OpenAPI_report_item_t *OpenAPI_report_item_copy(OpenAPI_report_item_t *dst, OpenAPI_report_item_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_report_item_H_ */

