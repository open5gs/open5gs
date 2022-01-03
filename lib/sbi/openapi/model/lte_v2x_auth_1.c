
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "lte_v2x_auth_1.h"

OpenAPI_lte_v2x_auth_1_t *OpenAPI_lte_v2x_auth_1_create(
    OpenAPI_ue_auth_e vehicle_ue_auth,
    OpenAPI_ue_auth_e pedestrian_ue_auth
)
{
    OpenAPI_lte_v2x_auth_1_t *lte_v2x_auth_1_local_var = ogs_malloc(sizeof(OpenAPI_lte_v2x_auth_1_t));
    ogs_assert(lte_v2x_auth_1_local_var);

    lte_v2x_auth_1_local_var->vehicle_ue_auth = vehicle_ue_auth;
    lte_v2x_auth_1_local_var->pedestrian_ue_auth = pedestrian_ue_auth;

    return lte_v2x_auth_1_local_var;
}

void OpenAPI_lte_v2x_auth_1_free(OpenAPI_lte_v2x_auth_1_t *lte_v2x_auth_1)
{
    if (NULL == lte_v2x_auth_1) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(lte_v2x_auth_1);
}

cJSON *OpenAPI_lte_v2x_auth_1_convertToJSON(OpenAPI_lte_v2x_auth_1_t *lte_v2x_auth_1)
{
    cJSON *item = NULL;

    if (lte_v2x_auth_1 == NULL) {
        ogs_error("OpenAPI_lte_v2x_auth_1_convertToJSON() failed [LteV2xAuth_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (lte_v2x_auth_1->vehicle_ue_auth) {
    if (cJSON_AddStringToObject(item, "vehicleUeAuth", OpenAPI_ue_auth_ToString(lte_v2x_auth_1->vehicle_ue_auth)) == NULL) {
        ogs_error("OpenAPI_lte_v2x_auth_1_convertToJSON() failed [vehicle_ue_auth]");
        goto end;
    }
    }

    if (lte_v2x_auth_1->pedestrian_ue_auth) {
    if (cJSON_AddStringToObject(item, "pedestrianUeAuth", OpenAPI_ue_auth_ToString(lte_v2x_auth_1->pedestrian_ue_auth)) == NULL) {
        ogs_error("OpenAPI_lte_v2x_auth_1_convertToJSON() failed [pedestrian_ue_auth]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_lte_v2x_auth_1_t *OpenAPI_lte_v2x_auth_1_parseFromJSON(cJSON *lte_v2x_auth_1JSON)
{
    OpenAPI_lte_v2x_auth_1_t *lte_v2x_auth_1_local_var = NULL;
    cJSON *vehicle_ue_auth = cJSON_GetObjectItemCaseSensitive(lte_v2x_auth_1JSON, "vehicleUeAuth");

    OpenAPI_ue_auth_e vehicle_ue_authVariable;
    if (vehicle_ue_auth) {
    if (!cJSON_IsString(vehicle_ue_auth)) {
        ogs_error("OpenAPI_lte_v2x_auth_1_parseFromJSON() failed [vehicle_ue_auth]");
        goto end;
    }
    vehicle_ue_authVariable = OpenAPI_ue_auth_FromString(vehicle_ue_auth->valuestring);
    }

    cJSON *pedestrian_ue_auth = cJSON_GetObjectItemCaseSensitive(lte_v2x_auth_1JSON, "pedestrianUeAuth");

    OpenAPI_ue_auth_e pedestrian_ue_authVariable;
    if (pedestrian_ue_auth) {
    if (!cJSON_IsString(pedestrian_ue_auth)) {
        ogs_error("OpenAPI_lte_v2x_auth_1_parseFromJSON() failed [pedestrian_ue_auth]");
        goto end;
    }
    pedestrian_ue_authVariable = OpenAPI_ue_auth_FromString(pedestrian_ue_auth->valuestring);
    }

    lte_v2x_auth_1_local_var = OpenAPI_lte_v2x_auth_1_create (
        vehicle_ue_auth ? vehicle_ue_authVariable : 0,
        pedestrian_ue_auth ? pedestrian_ue_authVariable : 0
    );

    return lte_v2x_auth_1_local_var;
end:
    return NULL;
}

OpenAPI_lte_v2x_auth_1_t *OpenAPI_lte_v2x_auth_1_copy(OpenAPI_lte_v2x_auth_1_t *dst, OpenAPI_lte_v2x_auth_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_lte_v2x_auth_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_lte_v2x_auth_1_convertToJSON() failed");
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

    OpenAPI_lte_v2x_auth_1_free(dst);
    dst = OpenAPI_lte_v2x_auth_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

