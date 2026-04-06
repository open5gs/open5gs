
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "storage_handling_information.h"

OpenAPI_storage_handling_information_t *OpenAPI_storage_handling_information_create(
    bool is_lifetime,
    int lifetime,
    bool is_del_notif_ind,
    int del_notif_ind
)
{
    OpenAPI_storage_handling_information_t *storage_handling_information_local_var = ogs_malloc(sizeof(OpenAPI_storage_handling_information_t));
    ogs_assert(storage_handling_information_local_var);

    storage_handling_information_local_var->is_lifetime = is_lifetime;
    storage_handling_information_local_var->lifetime = lifetime;
    storage_handling_information_local_var->is_del_notif_ind = is_del_notif_ind;
    storage_handling_information_local_var->del_notif_ind = del_notif_ind;

    return storage_handling_information_local_var;
}

void OpenAPI_storage_handling_information_free(OpenAPI_storage_handling_information_t *storage_handling_information)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == storage_handling_information) {
        return;
    }
    ogs_free(storage_handling_information);
}

cJSON *OpenAPI_storage_handling_information_convertToJSON(OpenAPI_storage_handling_information_t *storage_handling_information)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (storage_handling_information == NULL) {
        ogs_error("OpenAPI_storage_handling_information_convertToJSON() failed [StorageHandlingInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (storage_handling_information->is_lifetime) {
    if (cJSON_AddNumberToObject(item, "lifetime", storage_handling_information->lifetime) == NULL) {
        ogs_error("OpenAPI_storage_handling_information_convertToJSON() failed [lifetime]");
        goto end;
    }
    }

    if (storage_handling_information->is_del_notif_ind) {
    if (cJSON_AddBoolToObject(item, "delNotifInd", storage_handling_information->del_notif_ind) == NULL) {
        ogs_error("OpenAPI_storage_handling_information_convertToJSON() failed [del_notif_ind]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_storage_handling_information_t *OpenAPI_storage_handling_information_parseFromJSON(cJSON *storage_handling_informationJSON)
{
    OpenAPI_storage_handling_information_t *storage_handling_information_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *lifetime = NULL;
    cJSON *del_notif_ind = NULL;
    lifetime = cJSON_GetObjectItemCaseSensitive(storage_handling_informationJSON, "lifetime");
    if (lifetime) {
    if (!cJSON_IsNumber(lifetime)) {
        ogs_error("OpenAPI_storage_handling_information_parseFromJSON() failed [lifetime]");
        goto end;
    }
    }

    del_notif_ind = cJSON_GetObjectItemCaseSensitive(storage_handling_informationJSON, "delNotifInd");
    if (del_notif_ind) {
    if (!cJSON_IsBool(del_notif_ind)) {
        ogs_error("OpenAPI_storage_handling_information_parseFromJSON() failed [del_notif_ind]");
        goto end;
    }
    }

    storage_handling_information_local_var = OpenAPI_storage_handling_information_create (
        lifetime ? true : false,
        lifetime ? lifetime->valuedouble : 0,
        del_notif_ind ? true : false,
        del_notif_ind ? del_notif_ind->valueint : 0
    );

    return storage_handling_information_local_var;
end:
    return NULL;
}

OpenAPI_storage_handling_information_t *OpenAPI_storage_handling_information_copy(OpenAPI_storage_handling_information_t *dst, OpenAPI_storage_handling_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_storage_handling_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_storage_handling_information_convertToJSON() failed");
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

    OpenAPI_storage_handling_information_free(dst);
    dst = OpenAPI_storage_handling_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

