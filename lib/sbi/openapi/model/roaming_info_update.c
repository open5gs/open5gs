
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "roaming_info_update.h"

OpenAPI_roaming_info_update_t *OpenAPI_roaming_info_update_create(
    bool is_roaming,
    int roaming,
    OpenAPI_plmn_id_t *serving_plmn
)
{
    OpenAPI_roaming_info_update_t *roaming_info_update_local_var = ogs_malloc(sizeof(OpenAPI_roaming_info_update_t));
    ogs_assert(roaming_info_update_local_var);

    roaming_info_update_local_var->is_roaming = is_roaming;
    roaming_info_update_local_var->roaming = roaming;
    roaming_info_update_local_var->serving_plmn = serving_plmn;

    return roaming_info_update_local_var;
}

void OpenAPI_roaming_info_update_free(OpenAPI_roaming_info_update_t *roaming_info_update)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == roaming_info_update) {
        return;
    }
    if (roaming_info_update->serving_plmn) {
        OpenAPI_plmn_id_free(roaming_info_update->serving_plmn);
        roaming_info_update->serving_plmn = NULL;
    }
    ogs_free(roaming_info_update);
}

cJSON *OpenAPI_roaming_info_update_convertToJSON(OpenAPI_roaming_info_update_t *roaming_info_update)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (roaming_info_update == NULL) {
        ogs_error("OpenAPI_roaming_info_update_convertToJSON() failed [RoamingInfoUpdate]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (roaming_info_update->is_roaming) {
    if (cJSON_AddBoolToObject(item, "roaming", roaming_info_update->roaming) == NULL) {
        ogs_error("OpenAPI_roaming_info_update_convertToJSON() failed [roaming]");
        goto end;
    }
    }

    if (!roaming_info_update->serving_plmn) {
        ogs_error("OpenAPI_roaming_info_update_convertToJSON() failed [serving_plmn]");
        return NULL;
    }
    cJSON *serving_plmn_local_JSON = OpenAPI_plmn_id_convertToJSON(roaming_info_update->serving_plmn);
    if (serving_plmn_local_JSON == NULL) {
        ogs_error("OpenAPI_roaming_info_update_convertToJSON() failed [serving_plmn]");
        goto end;
    }
    cJSON_AddItemToObject(item, "servingPlmn", serving_plmn_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_roaming_info_update_convertToJSON() failed [serving_plmn]");
        goto end;
    }

end:
    return item;
}

OpenAPI_roaming_info_update_t *OpenAPI_roaming_info_update_parseFromJSON(cJSON *roaming_info_updateJSON)
{
    OpenAPI_roaming_info_update_t *roaming_info_update_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *roaming = NULL;
    cJSON *serving_plmn = NULL;
    OpenAPI_plmn_id_t *serving_plmn_local_nonprim = NULL;
    roaming = cJSON_GetObjectItemCaseSensitive(roaming_info_updateJSON, "roaming");
    if (roaming) {
    if (!cJSON_IsBool(roaming)) {
        ogs_error("OpenAPI_roaming_info_update_parseFromJSON() failed [roaming]");
        goto end;
    }
    }

    serving_plmn = cJSON_GetObjectItemCaseSensitive(roaming_info_updateJSON, "servingPlmn");
    if (!serving_plmn) {
        ogs_error("OpenAPI_roaming_info_update_parseFromJSON() failed [serving_plmn]");
        goto end;
    }
    serving_plmn_local_nonprim = OpenAPI_plmn_id_parseFromJSON(serving_plmn);
    if (!serving_plmn_local_nonprim) {
        ogs_error("OpenAPI_plmn_id_parseFromJSON failed [serving_plmn]");
        goto end;
    }

    roaming_info_update_local_var = OpenAPI_roaming_info_update_create (
        roaming ? true : false,
        roaming ? roaming->valueint : 0,
        serving_plmn_local_nonprim
    );

    return roaming_info_update_local_var;
end:
    if (serving_plmn_local_nonprim) {
        OpenAPI_plmn_id_free(serving_plmn_local_nonprim);
        serving_plmn_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_roaming_info_update_t *OpenAPI_roaming_info_update_copy(OpenAPI_roaming_info_update_t *dst, OpenAPI_roaming_info_update_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_roaming_info_update_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_roaming_info_update_convertToJSON() failed");
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

    OpenAPI_roaming_info_update_free(dst);
    dst = OpenAPI_roaming_info_update_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

