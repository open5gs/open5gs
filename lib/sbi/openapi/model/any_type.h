#ifndef OGS_SBI_ANY_TYPE_H
#define OGS_SBI_ANY_TYPE_H

#include "../external/cJSON.h"
#include "ogs-core.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OpenAPI_Invalid (0)
#define OpenAPI_False  (1 << 0)
#define OpenAPI_True   (1 << 1)
#define OpenAPI_NULL   (1 << 2)
#define OpenAPI_Number (1 << 3)
#define OpenAPI_String (1 << 4)
#define OpenAPI_Array  (1 << 5)
#define OpenAPI_Object (1 << 6)
#define OpenAPI_Raw    (1 << 7) /* raw json */

#define OpenAPI_IsReference 256
#define OpenAPI_StringIsConst 512

typedef struct OpenAPI_any_type_s OpenAPI_any_type_t;
typedef struct OpenAPI_any_type_s {
    cJSON *json;
} OpenAPI_any_type_t;

bool OpenAPI_IsInvalid(const OpenAPI_any_type_t * const item);
bool OpenAPI_IsFalse(const OpenAPI_any_type_t * const item);
bool OpenAPI_IsTrue(const OpenAPI_any_type_t * const item);
bool OpenAPI_IsBool(const OpenAPI_any_type_t * const item);
bool OpenAPI_IsNull(const OpenAPI_any_type_t * const item);
bool OpenAPI_IsNumber(const OpenAPI_any_type_t * const item);
bool OpenAPI_IsString(const OpenAPI_any_type_t * const item);
bool OpenAPI_IsArray(const OpenAPI_any_type_t * const item);
bool OpenAPI_IsObject(const OpenAPI_any_type_t * const item);
bool OpenAPI_IsRaw(const OpenAPI_any_type_t * const item);

OpenAPI_any_type_t *OpenAPI_any_type_create(cJSON *json);
OpenAPI_any_type_t *OpenAPI_any_type_parseFromJSON(cJSON *json);
cJSON *OpenAPI_any_type_convertToJSON(OpenAPI_any_type_t *any_type);

OpenAPI_any_type_t *OpenAPI_any_type_create_true(void);
OpenAPI_any_type_t *OpenAPI_any_type_create_false(void);
OpenAPI_any_type_t *OpenAPI_any_type_create_bool(bool boolean);
OpenAPI_any_type_t *OpenAPI_any_type_create_number(double num);
OpenAPI_any_type_t *OpenAPI_any_type_create_string(const char *string);
void OpenAPI_any_type_free(OpenAPI_any_type_t *any_type);

#ifdef __cplusplus
}
#endif

#endif /* OGS_SBI_ANY_TYPE_H */
