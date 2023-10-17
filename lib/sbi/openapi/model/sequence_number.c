
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sequence_number.h"

OpenAPI_sequence_number_t *OpenAPI_sequence_number_create(
    OpenAPI_sqn_scheme_e sqn_scheme,
    char *sqn,
    OpenAPI_list_t* last_indexes,
    bool is_ind_length,
    int ind_length,
    OpenAPI_sign_e dif_sign
)
{
    OpenAPI_sequence_number_t *sequence_number_local_var = ogs_malloc(sizeof(OpenAPI_sequence_number_t));
    ogs_assert(sequence_number_local_var);

    sequence_number_local_var->sqn_scheme = sqn_scheme;
    sequence_number_local_var->sqn = sqn;
    sequence_number_local_var->last_indexes = last_indexes;
    sequence_number_local_var->is_ind_length = is_ind_length;
    sequence_number_local_var->ind_length = ind_length;
    sequence_number_local_var->dif_sign = dif_sign;

    return sequence_number_local_var;
}

void OpenAPI_sequence_number_free(OpenAPI_sequence_number_t *sequence_number)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == sequence_number) {
        return;
    }
    if (sequence_number->sqn) {
        ogs_free(sequence_number->sqn);
        sequence_number->sqn = NULL;
    }
    if (sequence_number->last_indexes) {
        OpenAPI_list_for_each(sequence_number->last_indexes, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            ogs_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(sequence_number->last_indexes);
        sequence_number->last_indexes = NULL;
    }
    ogs_free(sequence_number);
}

cJSON *OpenAPI_sequence_number_convertToJSON(OpenAPI_sequence_number_t *sequence_number)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (sequence_number == NULL) {
        ogs_error("OpenAPI_sequence_number_convertToJSON() failed [SequenceNumber]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (sequence_number->sqn_scheme != OpenAPI_sqn_scheme_NULL) {
    if (cJSON_AddStringToObject(item, "sqnScheme", OpenAPI_sqn_scheme_ToString(sequence_number->sqn_scheme)) == NULL) {
        ogs_error("OpenAPI_sequence_number_convertToJSON() failed [sqn_scheme]");
        goto end;
    }
    }

    if (sequence_number->sqn) {
    if (cJSON_AddStringToObject(item, "sqn", sequence_number->sqn) == NULL) {
        ogs_error("OpenAPI_sequence_number_convertToJSON() failed [sqn]");
        goto end;
    }
    }

    if (sequence_number->last_indexes) {
    cJSON *last_indexes = cJSON_AddObjectToObject(item, "lastIndexes");
    if (last_indexes == NULL) {
        ogs_error("OpenAPI_sequence_number_convertToJSON() failed [last_indexes]");
        goto end;
    }
    cJSON *localMapObject = last_indexes;
    if (sequence_number->last_indexes) {
        OpenAPI_list_for_each(sequence_number->last_indexes, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_sequence_number_convertToJSON() failed [last_indexes]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_sequence_number_convertToJSON() failed [last_indexes]");
                goto end;
            }
            if (localKeyValue->value == NULL) {
                ogs_error("OpenAPI_sequence_number_convertToJSON() failed [inner]");
                goto end;
            }
            if (cJSON_AddNumberToObject(localMapObject, localKeyValue->key, *(double *)localKeyValue->value) == NULL) {
                ogs_error("OpenAPI_sequence_number_convertToJSON() failed [inner]");
                goto end;
            }
        }
    }
    }

    if (sequence_number->is_ind_length) {
    if (cJSON_AddNumberToObject(item, "indLength", sequence_number->ind_length) == NULL) {
        ogs_error("OpenAPI_sequence_number_convertToJSON() failed [ind_length]");
        goto end;
    }
    }

    if (sequence_number->dif_sign != OpenAPI_sign_NULL) {
    if (cJSON_AddStringToObject(item, "difSign", OpenAPI_sign_ToString(sequence_number->dif_sign)) == NULL) {
        ogs_error("OpenAPI_sequence_number_convertToJSON() failed [dif_sign]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_sequence_number_t *OpenAPI_sequence_number_parseFromJSON(cJSON *sequence_numberJSON)
{
    OpenAPI_sequence_number_t *sequence_number_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *sqn_scheme = NULL;
    OpenAPI_sqn_scheme_e sqn_schemeVariable = 0;
    cJSON *sqn = NULL;
    cJSON *last_indexes = NULL;
    OpenAPI_list_t *last_indexesList = NULL;
    cJSON *ind_length = NULL;
    cJSON *dif_sign = NULL;
    OpenAPI_sign_e dif_signVariable = 0;
    sqn_scheme = cJSON_GetObjectItemCaseSensitive(sequence_numberJSON, "sqnScheme");
    if (sqn_scheme) {
    if (!cJSON_IsString(sqn_scheme)) {
        ogs_error("OpenAPI_sequence_number_parseFromJSON() failed [sqn_scheme]");
        goto end;
    }
    sqn_schemeVariable = OpenAPI_sqn_scheme_FromString(sqn_scheme->valuestring);
    }

    sqn = cJSON_GetObjectItemCaseSensitive(sequence_numberJSON, "sqn");
    if (sqn) {
    if (!cJSON_IsString(sqn) && !cJSON_IsNull(sqn)) {
        ogs_error("OpenAPI_sequence_number_parseFromJSON() failed [sqn]");
        goto end;
    }
    }

    last_indexes = cJSON_GetObjectItemCaseSensitive(sequence_numberJSON, "lastIndexes");
    if (last_indexes) {
        cJSON *last_indexes_local_map = NULL;
        if (!cJSON_IsObject(last_indexes) && !cJSON_IsNull(last_indexes)) {
            ogs_error("OpenAPI_sequence_number_parseFromJSON() failed [last_indexes]");
            goto end;
        }
        if (cJSON_IsObject(last_indexes)) {
            last_indexesList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(last_indexes_local_map, last_indexes) {
                cJSON *localMapObject = last_indexes_local_map;
                double *localDouble = NULL;
                int *localInt = NULL;
                if (!cJSON_IsNumber(localMapObject)) {
                    ogs_error("OpenAPI_sequence_number_parseFromJSON() failed [inner]");
                    goto end;
                }
                localDouble = (double *)ogs_calloc(1, sizeof(double));
                if (!localDouble) {
                    ogs_error("OpenAPI_sequence_number_parseFromJSON() failed [inner]");
                    goto end;
                }
                *localDouble = localMapObject->valuedouble;
                localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), localDouble);
                OpenAPI_list_add(last_indexesList, localMapKeyPair);
            }
        }
    }

    ind_length = cJSON_GetObjectItemCaseSensitive(sequence_numberJSON, "indLength");
    if (ind_length) {
    if (!cJSON_IsNumber(ind_length)) {
        ogs_error("OpenAPI_sequence_number_parseFromJSON() failed [ind_length]");
        goto end;
    }
    }

    dif_sign = cJSON_GetObjectItemCaseSensitive(sequence_numberJSON, "difSign");
    if (dif_sign) {
    if (!cJSON_IsString(dif_sign)) {
        ogs_error("OpenAPI_sequence_number_parseFromJSON() failed [dif_sign]");
        goto end;
    }
    dif_signVariable = OpenAPI_sign_FromString(dif_sign->valuestring);
    }

    sequence_number_local_var = OpenAPI_sequence_number_create (
        sqn_scheme ? sqn_schemeVariable : 0,
        sqn && !cJSON_IsNull(sqn) ? ogs_strdup(sqn->valuestring) : NULL,
        last_indexes ? last_indexesList : NULL,
        ind_length ? true : false,
        ind_length ? ind_length->valuedouble : 0,
        dif_sign ? dif_signVariable : 0
    );

    return sequence_number_local_var;
end:
    if (last_indexesList) {
        OpenAPI_list_for_each(last_indexesList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            ogs_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(last_indexesList);
        last_indexesList = NULL;
    }
    return NULL;
}

OpenAPI_sequence_number_t *OpenAPI_sequence_number_copy(OpenAPI_sequence_number_t *dst, OpenAPI_sequence_number_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_sequence_number_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_sequence_number_convertToJSON() failed");
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

    OpenAPI_sequence_number_free(dst);
    dst = OpenAPI_sequence_number_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

