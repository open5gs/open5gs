/*
 * software_image_info.h
 *
 * 
 */

#ifndef _OpenAPI_software_image_info_H_
#define _OpenAPI_software_image_info_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_software_image_info_s OpenAPI_software_image_info_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_software_image_info_s {
    bool is_minimum_disk;
    int minimum_disk;
    bool is_minimum_ram;
    int minimum_ram;
    char *disc_format;
    char *operating_system;
    char *sw_image_ref;
};

OpenAPI_software_image_info_t *OpenAPI_software_image_info_create(
    bool is_minimum_disk,
    int minimum_disk,
    bool is_minimum_ram,
    int minimum_ram,
    char *disc_format,
    char *operating_system,
    char *sw_image_ref
);
void OpenAPI_software_image_info_free(OpenAPI_software_image_info_t *software_image_info);
OpenAPI_software_image_info_t *OpenAPI_software_image_info_parseFromJSON(cJSON *software_image_infoJSON);
cJSON *OpenAPI_software_image_info_convertToJSON(OpenAPI_software_image_info_t *software_image_info);
OpenAPI_software_image_info_t *OpenAPI_software_image_info_copy(OpenAPI_software_image_info_t *dst, OpenAPI_software_image_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_software_image_info_H_ */

