/*
 * inline_object.h
 *
 *
 */

#ifndef _OpenAPI_inline_object_H_
#define _OpenAPI_inline_object_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "ue_context_create_data.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_inline_object_s OpenAPI_inline_object_t;
typedef struct OpenAPI_inline_object_s {
    struct OpenAPI_ue_context_create_data_s *json_data;
    OpenAPI_binary_t* binary_data_n2_information;
    OpenAPI_binary_t* binary_data_n2_information_ext1;
    OpenAPI_binary_t* binary_data_n2_information_ext2;
    OpenAPI_binary_t* binary_data_n2_information_ext3;
    OpenAPI_binary_t* binary_data_n2_information_ext4;
    OpenAPI_binary_t* binary_data_n2_information_ext5;
    OpenAPI_binary_t* binary_data_n2_information_ext6;
    OpenAPI_binary_t* binary_data_n2_information_ext7;
    OpenAPI_binary_t* binary_data_n2_information_ext8;
    OpenAPI_binary_t* binary_data_n2_information_ext9;
    OpenAPI_binary_t* binary_data_n2_information_ext10;
    OpenAPI_binary_t* binary_data_n2_information_ext11;
    OpenAPI_binary_t* binary_data_n2_information_ext12;
    OpenAPI_binary_t* binary_data_n2_information_ext13;
    OpenAPI_binary_t* binary_data_n2_information_ext14;
    OpenAPI_binary_t* binary_data_n2_information_ext15;
    OpenAPI_binary_t* binary_data_n2_information_ext16;
} OpenAPI_inline_object_t;

OpenAPI_inline_object_t *OpenAPI_inline_object_create(
    OpenAPI_ue_context_create_data_t *json_data,
    OpenAPI_binary_t* binary_data_n2_information,
    OpenAPI_binary_t* binary_data_n2_information_ext1,
    OpenAPI_binary_t* binary_data_n2_information_ext2,
    OpenAPI_binary_t* binary_data_n2_information_ext3,
    OpenAPI_binary_t* binary_data_n2_information_ext4,
    OpenAPI_binary_t* binary_data_n2_information_ext5,
    OpenAPI_binary_t* binary_data_n2_information_ext6,
    OpenAPI_binary_t* binary_data_n2_information_ext7,
    OpenAPI_binary_t* binary_data_n2_information_ext8,
    OpenAPI_binary_t* binary_data_n2_information_ext9,
    OpenAPI_binary_t* binary_data_n2_information_ext10,
    OpenAPI_binary_t* binary_data_n2_information_ext11,
    OpenAPI_binary_t* binary_data_n2_information_ext12,
    OpenAPI_binary_t* binary_data_n2_information_ext13,
    OpenAPI_binary_t* binary_data_n2_information_ext14,
    OpenAPI_binary_t* binary_data_n2_information_ext15,
    OpenAPI_binary_t* binary_data_n2_information_ext16
    );
void OpenAPI_inline_object_free(OpenAPI_inline_object_t *inline_object);
OpenAPI_inline_object_t *OpenAPI_inline_object_parseFromJSON(cJSON *inline_objectJSON);
cJSON *OpenAPI_inline_object_convertToJSON(OpenAPI_inline_object_t *inline_object);
OpenAPI_inline_object_t *OpenAPI_inline_object_copy(OpenAPI_inline_object_t *dst, OpenAPI_inline_object_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_inline_object_H_ */

