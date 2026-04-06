#include "any_type.h"

bool OpenAPI_IsInvalid(const OpenAPI_any_type_t * const item)
{
    return item && cJSON_IsInvalid(item->json);
}
bool OpenAPI_IsFalse(const OpenAPI_any_type_t * const item)
{
    return item && cJSON_IsFalse(item->json);
}
bool OpenAPI_IsTrue(const OpenAPI_any_type_t * const item)
{
    return item && cJSON_IsTrue(item->json);
}
bool OpenAPI_IsBool(const OpenAPI_any_type_t * const item)
{
    return item && cJSON_IsBool(item->json);
}
bool OpenAPI_IsNull(const OpenAPI_any_type_t * const item)
{
    return item && cJSON_IsNull(item->json);
}
bool OpenAPI_IsNumber(const OpenAPI_any_type_t * const item)
{
    return item && cJSON_IsNumber(item->json);
}
bool OpenAPI_IsString(const OpenAPI_any_type_t * const item)
{
    return item && cJSON_IsString(item->json);
}
bool OpenAPI_IsArray(const OpenAPI_any_type_t * const item)
{
    return item && cJSON_IsArray(item->json);
}
bool OpenAPI_IsObject(const OpenAPI_any_type_t * const item)
{
    return item && cJSON_IsObject(item->json);
}
bool OpenAPI_IsRaw(const OpenAPI_any_type_t * const item)
{
    return item && cJSON_IsRaw(item->json);
}

static OpenAPI_any_type_t *any_create(cJSON *json) {
    OpenAPI_any_type_t *any_type = NULL;

    if (!json) {
        return NULL;
    }

    any_type = ogs_calloc(1, sizeof(*any_type));
    if (!any_type) {
        cJSON_Delete(json);
    }
    any_type->json = json;

    return any_type;
}

OpenAPI_any_type_t *OpenAPI_any_type_create(cJSON *json) {
    OpenAPI_any_type_t *any_type_local_var =
        any_create(cJSON_Duplicate(json, true));
    ogs_assert(any_type_local_var);

    return any_type_local_var;
}

OpenAPI_any_type_t *OpenAPI_any_type_parseFromJSON(cJSON *json) {
    return OpenAPI_any_type_create(json);
}

cJSON *OpenAPI_any_type_convertToJSON(OpenAPI_any_type_t *any_type) {
    cJSON *item = NULL;

    if (any_type == NULL) {
        ogs_error("OpenAPI_any_type_convertToJSON() failed [AnyType]");
        return NULL;
    }

    item = cJSON_Duplicate(any_type->json, true);
    if (item == NULL) {
        ogs_error("OpenAPI_any_type_convertToJSON() failed [AnyType]");
        return NULL;
    }

    return item;
}

OpenAPI_any_type_t *OpenAPI_any_type_create_true(void)
{
    return OpenAPI_any_type_create_bool(true);
}

OpenAPI_any_type_t *OpenAPI_any_type_create_false(void)
{
    return OpenAPI_any_type_create_bool(false);
}

OpenAPI_any_type_t *OpenAPI_any_type_create_bool(bool boolean)
{
    return any_create(boolean ? cJSON_CreateTrue() : cJSON_CreateFalse());
}

OpenAPI_any_type_t *OpenAPI_any_type_create_number(double num)
{
    return any_create(cJSON_CreateNumber(num));
}


OpenAPI_any_type_t *OpenAPI_any_type_create_string(const char *string)
{
    return any_create(cJSON_CreateString(string));
}

void OpenAPI_any_type_free(OpenAPI_any_type_t *any_type)
{
    if (any_type) {
        cJSON_Delete(any_type->json);
        ogs_free(any_type);
    }
}
