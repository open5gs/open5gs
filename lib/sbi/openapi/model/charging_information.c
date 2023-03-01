
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "charging_information.h"

OpenAPI_charging_information_t *OpenAPI_charging_information_create(
    char *primary_chf_address,
    char *secondary_chf_address,
    char *primary_chf_set_id,
    char *primary_chf_instance_id,
    char *secondary_chf_set_id,
    char *secondary_chf_instance_id
)
{
    OpenAPI_charging_information_t *charging_information_local_var = ogs_malloc(sizeof(OpenAPI_charging_information_t));
    ogs_assert(charging_information_local_var);

    charging_information_local_var->primary_chf_address = primary_chf_address;
    charging_information_local_var->secondary_chf_address = secondary_chf_address;
    charging_information_local_var->primary_chf_set_id = primary_chf_set_id;
    charging_information_local_var->primary_chf_instance_id = primary_chf_instance_id;
    charging_information_local_var->secondary_chf_set_id = secondary_chf_set_id;
    charging_information_local_var->secondary_chf_instance_id = secondary_chf_instance_id;

    return charging_information_local_var;
}

void OpenAPI_charging_information_free(OpenAPI_charging_information_t *charging_information)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == charging_information) {
        return;
    }
    if (charging_information->primary_chf_address) {
        ogs_free(charging_information->primary_chf_address);
        charging_information->primary_chf_address = NULL;
    }
    if (charging_information->secondary_chf_address) {
        ogs_free(charging_information->secondary_chf_address);
        charging_information->secondary_chf_address = NULL;
    }
    if (charging_information->primary_chf_set_id) {
        ogs_free(charging_information->primary_chf_set_id);
        charging_information->primary_chf_set_id = NULL;
    }
    if (charging_information->primary_chf_instance_id) {
        ogs_free(charging_information->primary_chf_instance_id);
        charging_information->primary_chf_instance_id = NULL;
    }
    if (charging_information->secondary_chf_set_id) {
        ogs_free(charging_information->secondary_chf_set_id);
        charging_information->secondary_chf_set_id = NULL;
    }
    if (charging_information->secondary_chf_instance_id) {
        ogs_free(charging_information->secondary_chf_instance_id);
        charging_information->secondary_chf_instance_id = NULL;
    }
    ogs_free(charging_information);
}

cJSON *OpenAPI_charging_information_convertToJSON(OpenAPI_charging_information_t *charging_information)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (charging_information == NULL) {
        ogs_error("OpenAPI_charging_information_convertToJSON() failed [ChargingInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!charging_information->primary_chf_address) {
        ogs_error("OpenAPI_charging_information_convertToJSON() failed [primary_chf_address]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "primaryChfAddress", charging_information->primary_chf_address) == NULL) {
        ogs_error("OpenAPI_charging_information_convertToJSON() failed [primary_chf_address]");
        goto end;
    }

    if (charging_information->secondary_chf_address) {
    if (cJSON_AddStringToObject(item, "secondaryChfAddress", charging_information->secondary_chf_address) == NULL) {
        ogs_error("OpenAPI_charging_information_convertToJSON() failed [secondary_chf_address]");
        goto end;
    }
    }

    if (charging_information->primary_chf_set_id) {
    if (cJSON_AddStringToObject(item, "primaryChfSetId", charging_information->primary_chf_set_id) == NULL) {
        ogs_error("OpenAPI_charging_information_convertToJSON() failed [primary_chf_set_id]");
        goto end;
    }
    }

    if (charging_information->primary_chf_instance_id) {
    if (cJSON_AddStringToObject(item, "primaryChfInstanceId", charging_information->primary_chf_instance_id) == NULL) {
        ogs_error("OpenAPI_charging_information_convertToJSON() failed [primary_chf_instance_id]");
        goto end;
    }
    }

    if (charging_information->secondary_chf_set_id) {
    if (cJSON_AddStringToObject(item, "secondaryChfSetId", charging_information->secondary_chf_set_id) == NULL) {
        ogs_error("OpenAPI_charging_information_convertToJSON() failed [secondary_chf_set_id]");
        goto end;
    }
    }

    if (charging_information->secondary_chf_instance_id) {
    if (cJSON_AddStringToObject(item, "secondaryChfInstanceId", charging_information->secondary_chf_instance_id) == NULL) {
        ogs_error("OpenAPI_charging_information_convertToJSON() failed [secondary_chf_instance_id]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_charging_information_t *OpenAPI_charging_information_parseFromJSON(cJSON *charging_informationJSON)
{
    OpenAPI_charging_information_t *charging_information_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *primary_chf_address = NULL;
    cJSON *secondary_chf_address = NULL;
    cJSON *primary_chf_set_id = NULL;
    cJSON *primary_chf_instance_id = NULL;
    cJSON *secondary_chf_set_id = NULL;
    cJSON *secondary_chf_instance_id = NULL;
    primary_chf_address = cJSON_GetObjectItemCaseSensitive(charging_informationJSON, "primaryChfAddress");
    if (!primary_chf_address) {
        ogs_error("OpenAPI_charging_information_parseFromJSON() failed [primary_chf_address]");
        goto end;
    }
    if (!cJSON_IsString(primary_chf_address)) {
        ogs_error("OpenAPI_charging_information_parseFromJSON() failed [primary_chf_address]");
        goto end;
    }

    secondary_chf_address = cJSON_GetObjectItemCaseSensitive(charging_informationJSON, "secondaryChfAddress");
    if (secondary_chf_address) {
    if (!cJSON_IsString(secondary_chf_address) && !cJSON_IsNull(secondary_chf_address)) {
        ogs_error("OpenAPI_charging_information_parseFromJSON() failed [secondary_chf_address]");
        goto end;
    }
    }

    primary_chf_set_id = cJSON_GetObjectItemCaseSensitive(charging_informationJSON, "primaryChfSetId");
    if (primary_chf_set_id) {
    if (!cJSON_IsString(primary_chf_set_id) && !cJSON_IsNull(primary_chf_set_id)) {
        ogs_error("OpenAPI_charging_information_parseFromJSON() failed [primary_chf_set_id]");
        goto end;
    }
    }

    primary_chf_instance_id = cJSON_GetObjectItemCaseSensitive(charging_informationJSON, "primaryChfInstanceId");
    if (primary_chf_instance_id) {
    if (!cJSON_IsString(primary_chf_instance_id) && !cJSON_IsNull(primary_chf_instance_id)) {
        ogs_error("OpenAPI_charging_information_parseFromJSON() failed [primary_chf_instance_id]");
        goto end;
    }
    }

    secondary_chf_set_id = cJSON_GetObjectItemCaseSensitive(charging_informationJSON, "secondaryChfSetId");
    if (secondary_chf_set_id) {
    if (!cJSON_IsString(secondary_chf_set_id) && !cJSON_IsNull(secondary_chf_set_id)) {
        ogs_error("OpenAPI_charging_information_parseFromJSON() failed [secondary_chf_set_id]");
        goto end;
    }
    }

    secondary_chf_instance_id = cJSON_GetObjectItemCaseSensitive(charging_informationJSON, "secondaryChfInstanceId");
    if (secondary_chf_instance_id) {
    if (!cJSON_IsString(secondary_chf_instance_id) && !cJSON_IsNull(secondary_chf_instance_id)) {
        ogs_error("OpenAPI_charging_information_parseFromJSON() failed [secondary_chf_instance_id]");
        goto end;
    }
    }

    charging_information_local_var = OpenAPI_charging_information_create (
        ogs_strdup(primary_chf_address->valuestring),
        secondary_chf_address && !cJSON_IsNull(secondary_chf_address) ? ogs_strdup(secondary_chf_address->valuestring) : NULL,
        primary_chf_set_id && !cJSON_IsNull(primary_chf_set_id) ? ogs_strdup(primary_chf_set_id->valuestring) : NULL,
        primary_chf_instance_id && !cJSON_IsNull(primary_chf_instance_id) ? ogs_strdup(primary_chf_instance_id->valuestring) : NULL,
        secondary_chf_set_id && !cJSON_IsNull(secondary_chf_set_id) ? ogs_strdup(secondary_chf_set_id->valuestring) : NULL,
        secondary_chf_instance_id && !cJSON_IsNull(secondary_chf_instance_id) ? ogs_strdup(secondary_chf_instance_id->valuestring) : NULL
    );

    return charging_information_local_var;
end:
    return NULL;
}

OpenAPI_charging_information_t *OpenAPI_charging_information_copy(OpenAPI_charging_information_t *dst, OpenAPI_charging_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_charging_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_charging_information_convertToJSON() failed");
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

    OpenAPI_charging_information_free(dst);
    dst = OpenAPI_charging_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

