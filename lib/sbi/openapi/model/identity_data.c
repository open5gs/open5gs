
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "identity_data.h"

OpenAPI_identity_data_t *OpenAPI_identity_data_create(
    OpenAPI_list_t *supi_list,
    OpenAPI_list_t *gpsi_list,
    OpenAPI_list_t *allowed_af_ids,
    OpenAPI_list_t* application_port_ids
)
{
    OpenAPI_identity_data_t *identity_data_local_var = ogs_malloc(sizeof(OpenAPI_identity_data_t));
    ogs_assert(identity_data_local_var);

    identity_data_local_var->supi_list = supi_list;
    identity_data_local_var->gpsi_list = gpsi_list;
    identity_data_local_var->allowed_af_ids = allowed_af_ids;
    identity_data_local_var->application_port_ids = application_port_ids;

    return identity_data_local_var;
}

void OpenAPI_identity_data_free(OpenAPI_identity_data_t *identity_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == identity_data) {
        return;
    }
    if (identity_data->supi_list) {
        OpenAPI_list_for_each(identity_data->supi_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(identity_data->supi_list);
        identity_data->supi_list = NULL;
    }
    if (identity_data->gpsi_list) {
        OpenAPI_list_for_each(identity_data->gpsi_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(identity_data->gpsi_list);
        identity_data->gpsi_list = NULL;
    }
    if (identity_data->allowed_af_ids) {
        OpenAPI_list_for_each(identity_data->allowed_af_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(identity_data->allowed_af_ids);
        identity_data->allowed_af_ids = NULL;
    }
    if (identity_data->application_port_ids) {
        OpenAPI_list_for_each(identity_data->application_port_ids, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            ogs_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(identity_data->application_port_ids);
        identity_data->application_port_ids = NULL;
    }
    ogs_free(identity_data);
}

cJSON *OpenAPI_identity_data_convertToJSON(OpenAPI_identity_data_t *identity_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (identity_data == NULL) {
        ogs_error("OpenAPI_identity_data_convertToJSON() failed [IdentityData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (identity_data->supi_list) {
    cJSON *supi_listList = cJSON_AddArrayToObject(item, "supiList");
    if (supi_listList == NULL) {
        ogs_error("OpenAPI_identity_data_convertToJSON() failed [supi_list]");
        goto end;
    }
    OpenAPI_list_for_each(identity_data->supi_list, node) {
        if (cJSON_AddStringToObject(supi_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_identity_data_convertToJSON() failed [supi_list]");
            goto end;
        }
    }
    }

    if (identity_data->gpsi_list) {
    cJSON *gpsi_listList = cJSON_AddArrayToObject(item, "gpsiList");
    if (gpsi_listList == NULL) {
        ogs_error("OpenAPI_identity_data_convertToJSON() failed [gpsi_list]");
        goto end;
    }
    OpenAPI_list_for_each(identity_data->gpsi_list, node) {
        if (cJSON_AddStringToObject(gpsi_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_identity_data_convertToJSON() failed [gpsi_list]");
            goto end;
        }
    }
    }

    if (identity_data->allowed_af_ids) {
    cJSON *allowed_af_idsList = cJSON_AddArrayToObject(item, "allowedAfIds");
    if (allowed_af_idsList == NULL) {
        ogs_error("OpenAPI_identity_data_convertToJSON() failed [allowed_af_ids]");
        goto end;
    }
    OpenAPI_list_for_each(identity_data->allowed_af_ids, node) {
        if (cJSON_AddStringToObject(allowed_af_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_identity_data_convertToJSON() failed [allowed_af_ids]");
            goto end;
        }
    }
    }

    if (identity_data->application_port_ids) {
    cJSON *application_port_ids = cJSON_AddObjectToObject(item, "applicationPortIds");
    if (application_port_ids == NULL) {
        ogs_error("OpenAPI_identity_data_convertToJSON() failed [application_port_ids]");
        goto end;
    }
    cJSON *localMapObject = application_port_ids;
    if (identity_data->application_port_ids) {
        OpenAPI_list_for_each(identity_data->application_port_ids, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_identity_data_convertToJSON() failed [application_port_ids]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_identity_data_convertToJSON() failed [application_port_ids]");
                goto end;
            }
            if (cJSON_AddStringToObject(localMapObject, localKeyValue->key, (char*)localKeyValue->value) == NULL) {
                ogs_error("OpenAPI_identity_data_convertToJSON() failed [inner]");
                goto end;
            }
        }
    }
    }

end:
    return item;
}

OpenAPI_identity_data_t *OpenAPI_identity_data_parseFromJSON(cJSON *identity_dataJSON)
{
    OpenAPI_identity_data_t *identity_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *supi_list = NULL;
    OpenAPI_list_t *supi_listList = NULL;
    cJSON *gpsi_list = NULL;
    OpenAPI_list_t *gpsi_listList = NULL;
    cJSON *allowed_af_ids = NULL;
    OpenAPI_list_t *allowed_af_idsList = NULL;
    cJSON *application_port_ids = NULL;
    OpenAPI_list_t *application_port_idsList = NULL;
    supi_list = cJSON_GetObjectItemCaseSensitive(identity_dataJSON, "supiList");
    if (supi_list) {
        cJSON *supi_list_local = NULL;
        if (!cJSON_IsArray(supi_list)) {
            ogs_error("OpenAPI_identity_data_parseFromJSON() failed [supi_list]");
            goto end;
        }

        supi_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(supi_list_local, supi_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(supi_list_local)) {
                ogs_error("OpenAPI_identity_data_parseFromJSON() failed [supi_list]");
                goto end;
            }
            OpenAPI_list_add(supi_listList, ogs_strdup(supi_list_local->valuestring));
        }
    }

    gpsi_list = cJSON_GetObjectItemCaseSensitive(identity_dataJSON, "gpsiList");
    if (gpsi_list) {
        cJSON *gpsi_list_local = NULL;
        if (!cJSON_IsArray(gpsi_list)) {
            ogs_error("OpenAPI_identity_data_parseFromJSON() failed [gpsi_list]");
            goto end;
        }

        gpsi_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(gpsi_list_local, gpsi_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(gpsi_list_local)) {
                ogs_error("OpenAPI_identity_data_parseFromJSON() failed [gpsi_list]");
                goto end;
            }
            OpenAPI_list_add(gpsi_listList, ogs_strdup(gpsi_list_local->valuestring));
        }
    }

    allowed_af_ids = cJSON_GetObjectItemCaseSensitive(identity_dataJSON, "allowedAfIds");
    if (allowed_af_ids) {
        cJSON *allowed_af_ids_local = NULL;
        if (!cJSON_IsArray(allowed_af_ids)) {
            ogs_error("OpenAPI_identity_data_parseFromJSON() failed [allowed_af_ids]");
            goto end;
        }

        allowed_af_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(allowed_af_ids_local, allowed_af_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(allowed_af_ids_local)) {
                ogs_error("OpenAPI_identity_data_parseFromJSON() failed [allowed_af_ids]");
                goto end;
            }
            OpenAPI_list_add(allowed_af_idsList, ogs_strdup(allowed_af_ids_local->valuestring));
        }
    }

    application_port_ids = cJSON_GetObjectItemCaseSensitive(identity_dataJSON, "applicationPortIds");
    if (application_port_ids) {
        cJSON *application_port_ids_local_map = NULL;
        if (!cJSON_IsObject(application_port_ids) && !cJSON_IsNull(application_port_ids)) {
            ogs_error("OpenAPI_identity_data_parseFromJSON() failed [application_port_ids]");
            goto end;
        }
        if (cJSON_IsObject(application_port_ids)) {
            application_port_idsList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(application_port_ids_local_map, application_port_ids) {
                cJSON *localMapObject = application_port_ids_local_map;
                double *localDouble = NULL;
                int *localInt = NULL;
                if (!cJSON_IsString(localMapObject)) {
                    ogs_error("OpenAPI_identity_data_parseFromJSON() failed [inner]");
                    goto end;
                }
                localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), ogs_strdup(localMapObject->valuestring));
                OpenAPI_list_add(application_port_idsList, localMapKeyPair);
            }
        }
    }

    identity_data_local_var = OpenAPI_identity_data_create (
        supi_list ? supi_listList : NULL,
        gpsi_list ? gpsi_listList : NULL,
        allowed_af_ids ? allowed_af_idsList : NULL,
        application_port_ids ? application_port_idsList : NULL
    );

    return identity_data_local_var;
end:
    if (supi_listList) {
        OpenAPI_list_for_each(supi_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(supi_listList);
        supi_listList = NULL;
    }
    if (gpsi_listList) {
        OpenAPI_list_for_each(gpsi_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(gpsi_listList);
        gpsi_listList = NULL;
    }
    if (allowed_af_idsList) {
        OpenAPI_list_for_each(allowed_af_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(allowed_af_idsList);
        allowed_af_idsList = NULL;
    }
    if (application_port_idsList) {
        OpenAPI_list_for_each(application_port_idsList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            ogs_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(application_port_idsList);
        application_port_idsList = NULL;
    }
    return NULL;
}

OpenAPI_identity_data_t *OpenAPI_identity_data_copy(OpenAPI_identity_data_t *dst, OpenAPI_identity_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_identity_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_identity_data_convertToJSON() failed");
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

    OpenAPI_identity_data_free(dst);
    dst = OpenAPI_identity_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

