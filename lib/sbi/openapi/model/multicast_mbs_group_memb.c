
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "multicast_mbs_group_memb.h"

OpenAPI_multicast_mbs_group_memb_t *OpenAPI_multicast_mbs_group_memb_create(
    OpenAPI_list_t *multicast_group_memb,
    char *af_instance_id,
    char *internal_group_identifier
)
{
    OpenAPI_multicast_mbs_group_memb_t *multicast_mbs_group_memb_local_var = ogs_malloc(sizeof(OpenAPI_multicast_mbs_group_memb_t));
    ogs_assert(multicast_mbs_group_memb_local_var);

    multicast_mbs_group_memb_local_var->multicast_group_memb = multicast_group_memb;
    multicast_mbs_group_memb_local_var->af_instance_id = af_instance_id;
    multicast_mbs_group_memb_local_var->internal_group_identifier = internal_group_identifier;

    return multicast_mbs_group_memb_local_var;
}

void OpenAPI_multicast_mbs_group_memb_free(OpenAPI_multicast_mbs_group_memb_t *multicast_mbs_group_memb)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == multicast_mbs_group_memb) {
        return;
    }
    if (multicast_mbs_group_memb->multicast_group_memb) {
        OpenAPI_list_for_each(multicast_mbs_group_memb->multicast_group_memb, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(multicast_mbs_group_memb->multicast_group_memb);
        multicast_mbs_group_memb->multicast_group_memb = NULL;
    }
    if (multicast_mbs_group_memb->af_instance_id) {
        ogs_free(multicast_mbs_group_memb->af_instance_id);
        multicast_mbs_group_memb->af_instance_id = NULL;
    }
    if (multicast_mbs_group_memb->internal_group_identifier) {
        ogs_free(multicast_mbs_group_memb->internal_group_identifier);
        multicast_mbs_group_memb->internal_group_identifier = NULL;
    }
    ogs_free(multicast_mbs_group_memb);
}

cJSON *OpenAPI_multicast_mbs_group_memb_convertToJSON(OpenAPI_multicast_mbs_group_memb_t *multicast_mbs_group_memb)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (multicast_mbs_group_memb == NULL) {
        ogs_error("OpenAPI_multicast_mbs_group_memb_convertToJSON() failed [MulticastMbsGroupMemb]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!multicast_mbs_group_memb->multicast_group_memb) {
        ogs_error("OpenAPI_multicast_mbs_group_memb_convertToJSON() failed [multicast_group_memb]");
        return NULL;
    }
    cJSON *multicast_group_membList = cJSON_AddArrayToObject(item, "multicastGroupMemb");
    if (multicast_group_membList == NULL) {
        ogs_error("OpenAPI_multicast_mbs_group_memb_convertToJSON() failed [multicast_group_memb]");
        goto end;
    }
    OpenAPI_list_for_each(multicast_mbs_group_memb->multicast_group_memb, node) {
        if (cJSON_AddStringToObject(multicast_group_membList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_multicast_mbs_group_memb_convertToJSON() failed [multicast_group_memb]");
            goto end;
        }
    }

    if (multicast_mbs_group_memb->af_instance_id) {
    if (cJSON_AddStringToObject(item, "afInstanceId", multicast_mbs_group_memb->af_instance_id) == NULL) {
        ogs_error("OpenAPI_multicast_mbs_group_memb_convertToJSON() failed [af_instance_id]");
        goto end;
    }
    }

    if (multicast_mbs_group_memb->internal_group_identifier) {
    if (cJSON_AddStringToObject(item, "internalGroupIdentifier", multicast_mbs_group_memb->internal_group_identifier) == NULL) {
        ogs_error("OpenAPI_multicast_mbs_group_memb_convertToJSON() failed [internal_group_identifier]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_multicast_mbs_group_memb_t *OpenAPI_multicast_mbs_group_memb_parseFromJSON(cJSON *multicast_mbs_group_membJSON)
{
    OpenAPI_multicast_mbs_group_memb_t *multicast_mbs_group_memb_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *multicast_group_memb = NULL;
    OpenAPI_list_t *multicast_group_membList = NULL;
    cJSON *af_instance_id = NULL;
    cJSON *internal_group_identifier = NULL;
    multicast_group_memb = cJSON_GetObjectItemCaseSensitive(multicast_mbs_group_membJSON, "multicastGroupMemb");
    if (!multicast_group_memb) {
        ogs_error("OpenAPI_multicast_mbs_group_memb_parseFromJSON() failed [multicast_group_memb]");
        goto end;
    }
        cJSON *multicast_group_memb_local = NULL;
        if (!cJSON_IsArray(multicast_group_memb)) {
            ogs_error("OpenAPI_multicast_mbs_group_memb_parseFromJSON() failed [multicast_group_memb]");
            goto end;
        }

        multicast_group_membList = OpenAPI_list_create();

        cJSON_ArrayForEach(multicast_group_memb_local, multicast_group_memb) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(multicast_group_memb_local)) {
                ogs_error("OpenAPI_multicast_mbs_group_memb_parseFromJSON() failed [multicast_group_memb]");
                goto end;
            }
            OpenAPI_list_add(multicast_group_membList, ogs_strdup(multicast_group_memb_local->valuestring));
        }

    af_instance_id = cJSON_GetObjectItemCaseSensitive(multicast_mbs_group_membJSON, "afInstanceId");
    if (af_instance_id) {
    if (!cJSON_IsString(af_instance_id) && !cJSON_IsNull(af_instance_id)) {
        ogs_error("OpenAPI_multicast_mbs_group_memb_parseFromJSON() failed [af_instance_id]");
        goto end;
    }
    }

    internal_group_identifier = cJSON_GetObjectItemCaseSensitive(multicast_mbs_group_membJSON, "internalGroupIdentifier");
    if (internal_group_identifier) {
    if (!cJSON_IsString(internal_group_identifier) && !cJSON_IsNull(internal_group_identifier)) {
        ogs_error("OpenAPI_multicast_mbs_group_memb_parseFromJSON() failed [internal_group_identifier]");
        goto end;
    }
    }

    multicast_mbs_group_memb_local_var = OpenAPI_multicast_mbs_group_memb_create (
        multicast_group_membList,
        af_instance_id && !cJSON_IsNull(af_instance_id) ? ogs_strdup(af_instance_id->valuestring) : NULL,
        internal_group_identifier && !cJSON_IsNull(internal_group_identifier) ? ogs_strdup(internal_group_identifier->valuestring) : NULL
    );

    return multicast_mbs_group_memb_local_var;
end:
    if (multicast_group_membList) {
        OpenAPI_list_for_each(multicast_group_membList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(multicast_group_membList);
        multicast_group_membList = NULL;
    }
    return NULL;
}

OpenAPI_multicast_mbs_group_memb_t *OpenAPI_multicast_mbs_group_memb_copy(OpenAPI_multicast_mbs_group_memb_t *dst, OpenAPI_multicast_mbs_group_memb_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_multicast_mbs_group_memb_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_multicast_mbs_group_memb_convertToJSON() failed");
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

    OpenAPI_multicast_mbs_group_memb_free(dst);
    dst = OpenAPI_multicast_mbs_group_memb_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

