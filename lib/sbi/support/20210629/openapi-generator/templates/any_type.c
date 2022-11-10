#include "any_type.h"

bool OpenAPI_IsInvalid(const OpenAPI_any_type_t * const item)
{
    if (item == NULL) return false;

    return (item->type & 0xFF) == OpenAPI_Invalid;
}
bool OpenAPI_IsFalse(const OpenAPI_any_type_t * const item)
{
    if (item == NULL) return false;

    return (item->type & 0xFF) == OpenAPI_False;
}
bool OpenAPI_IsTrue(const OpenAPI_any_type_t * const item)
{
    if (item == NULL) return false;

    return (item->type & 0xff) == OpenAPI_True;
}
bool OpenAPI_IsBool(const OpenAPI_any_type_t * const item)
{
    if (item == NULL) return false;

    return (item->type & (OpenAPI_True | OpenAPI_False)) != 0;
}
bool OpenAPI_IsNull(const OpenAPI_any_type_t * const item)
{
    if (item == NULL) return false;

    return (item->type & 0xFF) == OpenAPI_NULL;
}
bool OpenAPI_IsNumber(const OpenAPI_any_type_t * const item)
{
    if (item == NULL) return false;

    return (item->type & 0xFF) == OpenAPI_Number;
}
bool OpenAPI_IsString(const OpenAPI_any_type_t * const item)
{
    if (item == NULL) return false;

    return (item->type & 0xFF) == OpenAPI_String;
}
bool OpenAPI_IsArray(const OpenAPI_any_type_t * const item)
{
    if (item == NULL) return false;

    return (item->type & 0xFF) == OpenAPI_Array;
}
bool OpenAPI_IsObject(const OpenAPI_any_type_t * const item)
{
    if (item == NULL) return false;

    return (item->type & 0xFF) == OpenAPI_Object;
}
bool OpenAPI_IsRaw(const OpenAPI_any_type_t * const item)
{
    if (item == NULL) return false;

    return (item->type & 0xFF) == OpenAPI_Raw;
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
    OpenAPI_any_type_t *any_type = NULL;

    any_type = ogs_calloc(1, sizeof(*any_type));
    if (any_type) {
        any_type->type = boolean ? OpenAPI_True : OpenAPI_False;
    }

    return any_type;
}

OpenAPI_any_type_t *OpenAPI_any_type_create_number(double num)
{
    OpenAPI_any_type_t *any_type = NULL;

    any_type = ogs_calloc(1, sizeof(*any_type));
    if (any_type) {
        any_type->type = OpenAPI_Number;
        any_type->valuedouble = num;
    }

    return any_type;
}

OpenAPI_any_type_t *OpenAPI_any_type_create_string(const char *string)
{
    OpenAPI_any_type_t *any_type = NULL;

    any_type = ogs_calloc(1, sizeof(*any_type));
    if (any_type) {
        any_type->type = OpenAPI_String;
        any_type->valuestring = ogs_strdup(string);
    }

    return any_type;
}

void OpenAPI_any_type_free(OpenAPI_any_type_t *any_type)
{
    if (any_type) {
        if (any_type->type == OpenAPI_String) {
            ogs_free(any_type->valuestring);
        }
        ogs_free(any_type);
    }
}
