
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "roaming_qbc_information.h"

OpenAPI_roaming_qbc_information_t *OpenAPI_roaming_qbc_information_create(
    OpenAPI_list_t *multiple_qf_icontainer,
    char *u_pfid,
    OpenAPI_roaming_charging_profile_t *roaming_charging_profile
)
{
    OpenAPI_roaming_qbc_information_t *roaming_qbc_information_local_var = ogs_malloc(sizeof(OpenAPI_roaming_qbc_information_t));
    ogs_assert(roaming_qbc_information_local_var);

    roaming_qbc_information_local_var->multiple_qf_icontainer = multiple_qf_icontainer;
    roaming_qbc_information_local_var->u_pfid = u_pfid;
    roaming_qbc_information_local_var->roaming_charging_profile = roaming_charging_profile;

    return roaming_qbc_information_local_var;
}

void OpenAPI_roaming_qbc_information_free(OpenAPI_roaming_qbc_information_t *roaming_qbc_information)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == roaming_qbc_information) {
        return;
    }
    if (roaming_qbc_information->multiple_qf_icontainer) {
        OpenAPI_list_for_each(roaming_qbc_information->multiple_qf_icontainer, node) {
            OpenAPI_multiple_qf_icontainer_free(node->data);
        }
        OpenAPI_list_free(roaming_qbc_information->multiple_qf_icontainer);
        roaming_qbc_information->multiple_qf_icontainer = NULL;
    }
    if (roaming_qbc_information->u_pfid) {
        ogs_free(roaming_qbc_information->u_pfid);
        roaming_qbc_information->u_pfid = NULL;
    }
    if (roaming_qbc_information->roaming_charging_profile) {
        OpenAPI_roaming_charging_profile_free(roaming_qbc_information->roaming_charging_profile);
        roaming_qbc_information->roaming_charging_profile = NULL;
    }
    ogs_free(roaming_qbc_information);
}

cJSON *OpenAPI_roaming_qbc_information_convertToJSON(OpenAPI_roaming_qbc_information_t *roaming_qbc_information)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (roaming_qbc_information == NULL) {
        ogs_error("OpenAPI_roaming_qbc_information_convertToJSON() failed [RoamingQBCInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (roaming_qbc_information->multiple_qf_icontainer) {
    cJSON *multiple_qf_icontainerList = cJSON_AddArrayToObject(item, "multipleQFIcontainer");
    if (multiple_qf_icontainerList == NULL) {
        ogs_error("OpenAPI_roaming_qbc_information_convertToJSON() failed [multiple_qf_icontainer]");
        goto end;
    }
    OpenAPI_list_for_each(roaming_qbc_information->multiple_qf_icontainer, node) {
        cJSON *itemLocal = OpenAPI_multiple_qf_icontainer_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_roaming_qbc_information_convertToJSON() failed [multiple_qf_icontainer]");
            goto end;
        }
        cJSON_AddItemToArray(multiple_qf_icontainerList, itemLocal);
    }
    }

    if (roaming_qbc_information->u_pfid) {
    if (cJSON_AddStringToObject(item, "uPFID", roaming_qbc_information->u_pfid) == NULL) {
        ogs_error("OpenAPI_roaming_qbc_information_convertToJSON() failed [u_pfid]");
        goto end;
    }
    }

    if (roaming_qbc_information->roaming_charging_profile) {
    cJSON *roaming_charging_profile_local_JSON = OpenAPI_roaming_charging_profile_convertToJSON(roaming_qbc_information->roaming_charging_profile);
    if (roaming_charging_profile_local_JSON == NULL) {
        ogs_error("OpenAPI_roaming_qbc_information_convertToJSON() failed [roaming_charging_profile]");
        goto end;
    }
    cJSON_AddItemToObject(item, "roamingChargingProfile", roaming_charging_profile_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_roaming_qbc_information_convertToJSON() failed [roaming_charging_profile]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_roaming_qbc_information_t *OpenAPI_roaming_qbc_information_parseFromJSON(cJSON *roaming_qbc_informationJSON)
{
    OpenAPI_roaming_qbc_information_t *roaming_qbc_information_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *multiple_qf_icontainer = NULL;
    OpenAPI_list_t *multiple_qf_icontainerList = NULL;
    cJSON *u_pfid = NULL;
    cJSON *roaming_charging_profile = NULL;
    OpenAPI_roaming_charging_profile_t *roaming_charging_profile_local_nonprim = NULL;
    multiple_qf_icontainer = cJSON_GetObjectItemCaseSensitive(roaming_qbc_informationJSON, "multipleQFIcontainer");
    if (multiple_qf_icontainer) {
        cJSON *multiple_qf_icontainer_local = NULL;
        if (!cJSON_IsArray(multiple_qf_icontainer)) {
            ogs_error("OpenAPI_roaming_qbc_information_parseFromJSON() failed [multiple_qf_icontainer]");
            goto end;
        }

        multiple_qf_icontainerList = OpenAPI_list_create();

        cJSON_ArrayForEach(multiple_qf_icontainer_local, multiple_qf_icontainer) {
            if (!cJSON_IsObject(multiple_qf_icontainer_local)) {
                ogs_error("OpenAPI_roaming_qbc_information_parseFromJSON() failed [multiple_qf_icontainer]");
                goto end;
            }
            OpenAPI_multiple_qf_icontainer_t *multiple_qf_icontainerItem = OpenAPI_multiple_qf_icontainer_parseFromJSON(multiple_qf_icontainer_local);
            if (!multiple_qf_icontainerItem) {
                ogs_error("No multiple_qf_icontainerItem");
                goto end;
            }
            OpenAPI_list_add(multiple_qf_icontainerList, multiple_qf_icontainerItem);
        }
    }

    u_pfid = cJSON_GetObjectItemCaseSensitive(roaming_qbc_informationJSON, "uPFID");
    if (u_pfid) {
    if (!cJSON_IsString(u_pfid) && !cJSON_IsNull(u_pfid)) {
        ogs_error("OpenAPI_roaming_qbc_information_parseFromJSON() failed [u_pfid]");
        goto end;
    }
    }

    roaming_charging_profile = cJSON_GetObjectItemCaseSensitive(roaming_qbc_informationJSON, "roamingChargingProfile");
    if (roaming_charging_profile) {
    roaming_charging_profile_local_nonprim = OpenAPI_roaming_charging_profile_parseFromJSON(roaming_charging_profile);
    if (!roaming_charging_profile_local_nonprim) {
        ogs_error("OpenAPI_roaming_charging_profile_parseFromJSON failed [roaming_charging_profile]");
        goto end;
    }
    }

    roaming_qbc_information_local_var = OpenAPI_roaming_qbc_information_create (
        multiple_qf_icontainer ? multiple_qf_icontainerList : NULL,
        u_pfid && !cJSON_IsNull(u_pfid) ? ogs_strdup(u_pfid->valuestring) : NULL,
        roaming_charging_profile ? roaming_charging_profile_local_nonprim : NULL
    );

    return roaming_qbc_information_local_var;
end:
    if (multiple_qf_icontainerList) {
        OpenAPI_list_for_each(multiple_qf_icontainerList, node) {
            OpenAPI_multiple_qf_icontainer_free(node->data);
        }
        OpenAPI_list_free(multiple_qf_icontainerList);
        multiple_qf_icontainerList = NULL;
    }
    if (roaming_charging_profile_local_nonprim) {
        OpenAPI_roaming_charging_profile_free(roaming_charging_profile_local_nonprim);
        roaming_charging_profile_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_roaming_qbc_information_t *OpenAPI_roaming_qbc_information_copy(OpenAPI_roaming_qbc_information_t *dst, OpenAPI_roaming_qbc_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_roaming_qbc_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_roaming_qbc_information_convertToJSON() failed");
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

    OpenAPI_roaming_qbc_information_free(dst);
    dst = OpenAPI_roaming_qbc_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

