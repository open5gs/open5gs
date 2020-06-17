
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ec_restriction.h"

OpenAPI_ec_restriction_t *OpenAPI_ec_restriction_create(
    char *af_instance_id,
    int reference_id,
    OpenAPI_list_t *plmn_ec_infos
    )
{
    OpenAPI_ec_restriction_t *ec_restriction_local_var = OpenAPI_malloc(sizeof(OpenAPI_ec_restriction_t));
    if (!ec_restriction_local_var) {
        return NULL;
    }
    ec_restriction_local_var->af_instance_id = af_instance_id;
    ec_restriction_local_var->reference_id = reference_id;
    ec_restriction_local_var->plmn_ec_infos = plmn_ec_infos;

    return ec_restriction_local_var;
}

void OpenAPI_ec_restriction_free(OpenAPI_ec_restriction_t *ec_restriction)
{
    if (NULL == ec_restriction) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(ec_restriction->af_instance_id);
    OpenAPI_list_for_each(ec_restriction->plmn_ec_infos, node) {
        OpenAPI_plmn_ec_info_free(node->data);
    }
    OpenAPI_list_free(ec_restriction->plmn_ec_infos);
    ogs_free(ec_restriction);
}

cJSON *OpenAPI_ec_restriction_convertToJSON(OpenAPI_ec_restriction_t *ec_restriction)
{
    cJSON *item = NULL;

    if (ec_restriction == NULL) {
        ogs_error("OpenAPI_ec_restriction_convertToJSON() failed [EcRestriction]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!ec_restriction->af_instance_id) {
        ogs_error("OpenAPI_ec_restriction_convertToJSON() failed [af_instance_id]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "afInstanceId", ec_restriction->af_instance_id) == NULL) {
        ogs_error("OpenAPI_ec_restriction_convertToJSON() failed [af_instance_id]");
        goto end;
    }

    if (!ec_restriction->reference_id) {
        ogs_error("OpenAPI_ec_restriction_convertToJSON() failed [reference_id]");
        goto end;
    }
    if (cJSON_AddNumberToObject(item, "referenceId", ec_restriction->reference_id) == NULL) {
        ogs_error("OpenAPI_ec_restriction_convertToJSON() failed [reference_id]");
        goto end;
    }

    if (ec_restriction->plmn_ec_infos) {
        cJSON *plmn_ec_infosList = cJSON_AddArrayToObject(item, "plmnEcInfos");
        if (plmn_ec_infosList == NULL) {
            ogs_error("OpenAPI_ec_restriction_convertToJSON() failed [plmn_ec_infos]");
            goto end;
        }

        OpenAPI_lnode_t *plmn_ec_infos_node;
        if (ec_restriction->plmn_ec_infos) {
            OpenAPI_list_for_each(ec_restriction->plmn_ec_infos, plmn_ec_infos_node) {
                cJSON *itemLocal = OpenAPI_plmn_ec_info_convertToJSON(plmn_ec_infos_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_ec_restriction_convertToJSON() failed [plmn_ec_infos]");
                    goto end;
                }
                cJSON_AddItemToArray(plmn_ec_infosList, itemLocal);
            }
        }
    }

end:
    return item;
}

OpenAPI_ec_restriction_t *OpenAPI_ec_restriction_parseFromJSON(cJSON *ec_restrictionJSON)
{
    OpenAPI_ec_restriction_t *ec_restriction_local_var = NULL;
    cJSON *af_instance_id = cJSON_GetObjectItemCaseSensitive(ec_restrictionJSON, "afInstanceId");
    if (!af_instance_id) {
        ogs_error("OpenAPI_ec_restriction_parseFromJSON() failed [af_instance_id]");
        goto end;
    }


    if (!cJSON_IsString(af_instance_id)) {
        ogs_error("OpenAPI_ec_restriction_parseFromJSON() failed [af_instance_id]");
        goto end;
    }

    cJSON *reference_id = cJSON_GetObjectItemCaseSensitive(ec_restrictionJSON, "referenceId");
    if (!reference_id) {
        ogs_error("OpenAPI_ec_restriction_parseFromJSON() failed [reference_id]");
        goto end;
    }


    if (!cJSON_IsNumber(reference_id)) {
        ogs_error("OpenAPI_ec_restriction_parseFromJSON() failed [reference_id]");
        goto end;
    }

    cJSON *plmn_ec_infos = cJSON_GetObjectItemCaseSensitive(ec_restrictionJSON, "plmnEcInfos");

    OpenAPI_list_t *plmn_ec_infosList;
    if (plmn_ec_infos) {
        cJSON *plmn_ec_infos_local_nonprimitive;
        if (!cJSON_IsArray(plmn_ec_infos)) {
            ogs_error("OpenAPI_ec_restriction_parseFromJSON() failed [plmn_ec_infos]");
            goto end;
        }

        plmn_ec_infosList = OpenAPI_list_create();

        cJSON_ArrayForEach(plmn_ec_infos_local_nonprimitive, plmn_ec_infos ) {
            if (!cJSON_IsObject(plmn_ec_infos_local_nonprimitive)) {
                ogs_error("OpenAPI_ec_restriction_parseFromJSON() failed [plmn_ec_infos]");
                goto end;
            }
            OpenAPI_plmn_ec_info_t *plmn_ec_infosItem = OpenAPI_plmn_ec_info_parseFromJSON(plmn_ec_infos_local_nonprimitive);

            OpenAPI_list_add(plmn_ec_infosList, plmn_ec_infosItem);
        }
    }

    ec_restriction_local_var = OpenAPI_ec_restriction_create (
        ogs_strdup(af_instance_id->valuestring),
        reference_id->valuedouble,
        plmn_ec_infos ? plmn_ec_infosList : NULL
        );

    return ec_restriction_local_var;
end:
    return NULL;
}

OpenAPI_ec_restriction_t *OpenAPI_ec_restriction_copy(OpenAPI_ec_restriction_t *dst, OpenAPI_ec_restriction_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ec_restriction_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ec_restriction_convertToJSON() failed");
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

    OpenAPI_ec_restriction_free(dst);
    dst = OpenAPI_ec_restriction_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

