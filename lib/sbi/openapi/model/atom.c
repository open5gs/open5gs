
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "atom.h"

OpenAPI_atom_t *OpenAPI_atom_create(
    char *attr,
    bool is_value_null,
    OpenAPI_any_type_t *value,
    bool is_negative,
    int negative
)
{
    OpenAPI_atom_t *atom_local_var = ogs_malloc(sizeof(OpenAPI_atom_t));
    ogs_assert(atom_local_var);

    atom_local_var->attr = attr;
    atom_local_var->is_value_null = is_value_null;
    atom_local_var->value = value;
    atom_local_var->is_negative = is_negative;
    atom_local_var->negative = negative;

    return atom_local_var;
}

void OpenAPI_atom_free(OpenAPI_atom_t *atom)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == atom) {
        return;
    }
    if (atom->attr) {
        ogs_free(atom->attr);
        atom->attr = NULL;
    }
    if (atom->value) {
        OpenAPI_any_type_free(atom->value);
        atom->value = NULL;
    }
    ogs_free(atom);
}

cJSON *OpenAPI_atom_convertToJSON(OpenAPI_atom_t *atom)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (atom == NULL) {
        ogs_error("OpenAPI_atom_convertToJSON() failed [Atom]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!atom->attr) {
        ogs_error("OpenAPI_atom_convertToJSON() failed [attr]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "attr", atom->attr) == NULL) {
        ogs_error("OpenAPI_atom_convertToJSON() failed [attr]");
        goto end;
    }

    if (!atom->value) {
        ogs_error("OpenAPI_atom_convertToJSON() failed [value]");
        return NULL;
    }
    cJSON *value_object = OpenAPI_any_type_convertToJSON(atom->value);
    if (value_object == NULL) {
        ogs_error("OpenAPI_atom_convertToJSON() failed [value]");
        goto end;
    }
    cJSON_AddItemToObject(item, "value", value_object);
    if (item->child == NULL) {
        ogs_error("OpenAPI_atom_convertToJSON() failed [value]");
        goto end;
    }

    if (atom->is_negative) {
    if (cJSON_AddBoolToObject(item, "negative", atom->negative) == NULL) {
        ogs_error("OpenAPI_atom_convertToJSON() failed [negative]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_atom_t *OpenAPI_atom_parseFromJSON(cJSON *atomJSON)
{
    OpenAPI_atom_t *atom_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *attr = NULL;
    cJSON *value = NULL;
    OpenAPI_any_type_t *value_local_object = NULL;
    cJSON *negative = NULL;
    attr = cJSON_GetObjectItemCaseSensitive(atomJSON, "attr");
    if (!attr) {
        ogs_error("OpenAPI_atom_parseFromJSON() failed [attr]");
        goto end;
    }
    if (!cJSON_IsString(attr)) {
        ogs_error("OpenAPI_atom_parseFromJSON() failed [attr]");
        goto end;
    }

    value = cJSON_GetObjectItemCaseSensitive(atomJSON, "value");
    if (!value) {
        ogs_error("OpenAPI_atom_parseFromJSON() failed [value]");
        goto end;
    }
    value_local_object = OpenAPI_any_type_parseFromJSON(value);

    negative = cJSON_GetObjectItemCaseSensitive(atomJSON, "negative");
    if (negative) {
    if (!cJSON_IsBool(negative)) {
        ogs_error("OpenAPI_atom_parseFromJSON() failed [negative]");
        goto end;
    }
    }

    atom_local_var = OpenAPI_atom_create (
        ogs_strdup(attr->valuestring),
        value && cJSON_IsNull(value) ? true : false,
        value_local_object,
        negative ? true : false,
        negative ? negative->valueint : 0
    );

    return atom_local_var;
end:
    if (value_local_object) {
        OpenAPI_any_type_free(value_local_object);
        value_local_object = NULL;
    }
    return NULL;
}

OpenAPI_atom_t *OpenAPI_atom_copy(OpenAPI_atom_t *dst, OpenAPI_atom_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_atom_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_atom_convertToJSON() failed");
        return NULL;
    }

    content = cJSON_Print(item);
    cJSON_Delete(item);

    if (!content) {
        ogs_error("cJSON_Print() failed");
        return NULL;
    }

    item = cJSON_Parse(content);
    ogs_free(content);
    if (!item) {
        ogs_error("cJSON_Parse() failed");
        return NULL;
    }

    OpenAPI_atom_free(dst);
    dst = OpenAPI_atom_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

