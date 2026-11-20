
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "radio_parameter_set_info.h"

OpenAPI_radio_parameter_set_info_t *OpenAPI_radio_parameter_set_info_create(
    OpenAPI_list_t *radio_parameter_set_values,
    char *change_timestamp
)
{
    OpenAPI_radio_parameter_set_info_t *radio_parameter_set_info_local_var = ogs_malloc(sizeof(OpenAPI_radio_parameter_set_info_t));
    ogs_assert(radio_parameter_set_info_local_var);

    radio_parameter_set_info_local_var->radio_parameter_set_values = radio_parameter_set_values;
    radio_parameter_set_info_local_var->change_timestamp = change_timestamp;

    return radio_parameter_set_info_local_var;
}

void OpenAPI_radio_parameter_set_info_free(OpenAPI_radio_parameter_set_info_t *radio_parameter_set_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == radio_parameter_set_info) {
        return;
    }
    if (radio_parameter_set_info->radio_parameter_set_values) {
        OpenAPI_list_for_each(radio_parameter_set_info->radio_parameter_set_values, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(radio_parameter_set_info->radio_parameter_set_values);
        radio_parameter_set_info->radio_parameter_set_values = NULL;
    }
    if (radio_parameter_set_info->change_timestamp) {
        ogs_free(radio_parameter_set_info->change_timestamp);
        radio_parameter_set_info->change_timestamp = NULL;
    }
    ogs_free(radio_parameter_set_info);
}

cJSON *OpenAPI_radio_parameter_set_info_convertToJSON(OpenAPI_radio_parameter_set_info_t *radio_parameter_set_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (radio_parameter_set_info == NULL) {
        ogs_error("OpenAPI_radio_parameter_set_info_convertToJSON() failed [RadioParameterSetInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (radio_parameter_set_info->radio_parameter_set_values) {
    cJSON *radio_parameter_set_valuesList = cJSON_AddArrayToObject(item, "radioParameterSetValues");
    if (radio_parameter_set_valuesList == NULL) {
        ogs_error("OpenAPI_radio_parameter_set_info_convertToJSON() failed [radio_parameter_set_values]");
        goto end;
    }
    OpenAPI_list_for_each(radio_parameter_set_info->radio_parameter_set_values, node) {
        if (cJSON_AddStringToObject(radio_parameter_set_valuesList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_radio_parameter_set_info_convertToJSON() failed [radio_parameter_set_values]");
            goto end;
        }
    }
    }

    if (radio_parameter_set_info->change_timestamp) {
    if (cJSON_AddStringToObject(item, "changeTimestamp", radio_parameter_set_info->change_timestamp) == NULL) {
        ogs_error("OpenAPI_radio_parameter_set_info_convertToJSON() failed [change_timestamp]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_radio_parameter_set_info_t *OpenAPI_radio_parameter_set_info_parseFromJSON(cJSON *radio_parameter_set_infoJSON)
{
    OpenAPI_radio_parameter_set_info_t *radio_parameter_set_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *radio_parameter_set_values = NULL;
    OpenAPI_list_t *radio_parameter_set_valuesList = NULL;
    cJSON *change_timestamp = NULL;
    radio_parameter_set_values = cJSON_GetObjectItemCaseSensitive(radio_parameter_set_infoJSON, "radioParameterSetValues");
    if (radio_parameter_set_values) {
        cJSON *radio_parameter_set_values_local = NULL;
        if (!cJSON_IsArray(radio_parameter_set_values)) {
            ogs_error("OpenAPI_radio_parameter_set_info_parseFromJSON() failed [radio_parameter_set_values]");
            goto end;
        }

        radio_parameter_set_valuesList = OpenAPI_list_create();

        cJSON_ArrayForEach(radio_parameter_set_values_local, radio_parameter_set_values) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(radio_parameter_set_values_local)) {
                ogs_error("OpenAPI_radio_parameter_set_info_parseFromJSON() failed [radio_parameter_set_values]");
                goto end;
            }
            OpenAPI_list_add(radio_parameter_set_valuesList, ogs_strdup(radio_parameter_set_values_local->valuestring));
        }
    }

    change_timestamp = cJSON_GetObjectItemCaseSensitive(radio_parameter_set_infoJSON, "changeTimestamp");
    if (change_timestamp) {
    if (!cJSON_IsString(change_timestamp) && !cJSON_IsNull(change_timestamp)) {
        ogs_error("OpenAPI_radio_parameter_set_info_parseFromJSON() failed [change_timestamp]");
        goto end;
    }
    }

    radio_parameter_set_info_local_var = OpenAPI_radio_parameter_set_info_create (
        radio_parameter_set_values ? radio_parameter_set_valuesList : NULL,
        change_timestamp && !cJSON_IsNull(change_timestamp) ? ogs_strdup(change_timestamp->valuestring) : NULL
    );

    return radio_parameter_set_info_local_var;
end:
    if (radio_parameter_set_valuesList) {
        OpenAPI_list_for_each(radio_parameter_set_valuesList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(radio_parameter_set_valuesList);
        radio_parameter_set_valuesList = NULL;
    }
    return NULL;
}

OpenAPI_radio_parameter_set_info_t *OpenAPI_radio_parameter_set_info_copy(OpenAPI_radio_parameter_set_info_t *dst, OpenAPI_radio_parameter_set_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_radio_parameter_set_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_radio_parameter_set_info_convertToJSON() failed");
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

    OpenAPI_radio_parameter_set_info_free(dst);
    dst = OpenAPI_radio_parameter_set_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

