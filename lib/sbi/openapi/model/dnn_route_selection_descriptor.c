
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "dnn_route_selection_descriptor.h"

OpenAPI_dnn_route_selection_descriptor_t *OpenAPI_dnn_route_selection_descriptor_create(
    char *dnn,
    OpenAPI_list_t *ssc_modes,
    OpenAPI_list_t *pdu_sess_types
    )
{
    OpenAPI_dnn_route_selection_descriptor_t *dnn_route_selection_descriptor_local_var = OpenAPI_malloc(sizeof(OpenAPI_dnn_route_selection_descriptor_t));
    if (!dnn_route_selection_descriptor_local_var) {
        return NULL;
    }
    dnn_route_selection_descriptor_local_var->dnn = dnn;
    dnn_route_selection_descriptor_local_var->ssc_modes = ssc_modes;
    dnn_route_selection_descriptor_local_var->pdu_sess_types = pdu_sess_types;

    return dnn_route_selection_descriptor_local_var;
}

void OpenAPI_dnn_route_selection_descriptor_free(OpenAPI_dnn_route_selection_descriptor_t *dnn_route_selection_descriptor)
{
    if (NULL == dnn_route_selection_descriptor) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(dnn_route_selection_descriptor->dnn);
    OpenAPI_list_free(dnn_route_selection_descriptor->ssc_modes);
    OpenAPI_list_free(dnn_route_selection_descriptor->pdu_sess_types);
    ogs_free(dnn_route_selection_descriptor);
}

cJSON *OpenAPI_dnn_route_selection_descriptor_convertToJSON(OpenAPI_dnn_route_selection_descriptor_t *dnn_route_selection_descriptor)
{
    cJSON *item = NULL;

    if (dnn_route_selection_descriptor == NULL) {
        ogs_error("OpenAPI_dnn_route_selection_descriptor_convertToJSON() failed [DnnRouteSelectionDescriptor]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!dnn_route_selection_descriptor->dnn) {
        ogs_error("OpenAPI_dnn_route_selection_descriptor_convertToJSON() failed [dnn]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "dnn", dnn_route_selection_descriptor->dnn) == NULL) {
        ogs_error("OpenAPI_dnn_route_selection_descriptor_convertToJSON() failed [dnn]");
        goto end;
    }

    if (dnn_route_selection_descriptor->ssc_modes) {
        cJSON *ssc_modes = cJSON_AddArrayToObject(item, "sscModes");
        if (ssc_modes == NULL) {
            ogs_error("OpenAPI_dnn_route_selection_descriptor_convertToJSON() failed [ssc_modes]");
            goto end;
        }
        OpenAPI_lnode_t *ssc_modes_node;
        OpenAPI_list_for_each(dnn_route_selection_descriptor->ssc_modes, ssc_modes_node) {
            if (cJSON_AddStringToObject(ssc_modes, "", OpenAPI_ssc_mode_ToString((OpenAPI_ssc_mode_e)ssc_modes_node->data)) == NULL) {
                ogs_error("OpenAPI_dnn_route_selection_descriptor_convertToJSON() failed [ssc_modes]");
                goto end;
            }
        }
    }

    if (dnn_route_selection_descriptor->pdu_sess_types) {
        cJSON *pdu_sess_types = cJSON_AddArrayToObject(item, "pduSessTypes");
        if (pdu_sess_types == NULL) {
            ogs_error("OpenAPI_dnn_route_selection_descriptor_convertToJSON() failed [pdu_sess_types]");
            goto end;
        }
        OpenAPI_lnode_t *pdu_sess_types_node;
        OpenAPI_list_for_each(dnn_route_selection_descriptor->pdu_sess_types, pdu_sess_types_node) {
            if (cJSON_AddStringToObject(pdu_sess_types, "", OpenAPI_pdu_session_type_ToString((OpenAPI_pdu_session_type_e)pdu_sess_types_node->data)) == NULL) {
                ogs_error("OpenAPI_dnn_route_selection_descriptor_convertToJSON() failed [pdu_sess_types]");
                goto end;
            }
        }
    }

end:
    return item;
}

OpenAPI_dnn_route_selection_descriptor_t *OpenAPI_dnn_route_selection_descriptor_parseFromJSON(cJSON *dnn_route_selection_descriptorJSON)
{
    OpenAPI_dnn_route_selection_descriptor_t *dnn_route_selection_descriptor_local_var = NULL;
    cJSON *dnn = cJSON_GetObjectItemCaseSensitive(dnn_route_selection_descriptorJSON, "dnn");
    if (!dnn) {
        ogs_error("OpenAPI_dnn_route_selection_descriptor_parseFromJSON() failed [dnn]");
        goto end;
    }


    if (!cJSON_IsString(dnn)) {
        ogs_error("OpenAPI_dnn_route_selection_descriptor_parseFromJSON() failed [dnn]");
        goto end;
    }

    cJSON *ssc_modes = cJSON_GetObjectItemCaseSensitive(dnn_route_selection_descriptorJSON, "sscModes");

    OpenAPI_list_t *ssc_modesList;
    if (ssc_modes) {
        cJSON *ssc_modes_local_nonprimitive;
        if (!cJSON_IsArray(ssc_modes)) {
            ogs_error("OpenAPI_dnn_route_selection_descriptor_parseFromJSON() failed [ssc_modes]");
            goto end;
        }

        ssc_modesList = OpenAPI_list_create();

        cJSON_ArrayForEach(ssc_modes_local_nonprimitive, ssc_modes ) {
            if (!cJSON_IsString(ssc_modes_local_nonprimitive)) {
                ogs_error("OpenAPI_dnn_route_selection_descriptor_parseFromJSON() failed [ssc_modes]");
                goto end;
            }

            OpenAPI_list_add(ssc_modesList, (void *)OpenAPI_ssc_mode_FromString(ssc_modes_local_nonprimitive->valuestring));
        }
    }

    cJSON *pdu_sess_types = cJSON_GetObjectItemCaseSensitive(dnn_route_selection_descriptorJSON, "pduSessTypes");

    OpenAPI_list_t *pdu_sess_typesList;
    if (pdu_sess_types) {
        cJSON *pdu_sess_types_local_nonprimitive;
        if (!cJSON_IsArray(pdu_sess_types)) {
            ogs_error("OpenAPI_dnn_route_selection_descriptor_parseFromJSON() failed [pdu_sess_types]");
            goto end;
        }

        pdu_sess_typesList = OpenAPI_list_create();

        cJSON_ArrayForEach(pdu_sess_types_local_nonprimitive, pdu_sess_types ) {
            if (!cJSON_IsString(pdu_sess_types_local_nonprimitive)) {
                ogs_error("OpenAPI_dnn_route_selection_descriptor_parseFromJSON() failed [pdu_sess_types]");
                goto end;
            }

            OpenAPI_list_add(pdu_sess_typesList, (void *)OpenAPI_pdu_session_type_FromString(pdu_sess_types_local_nonprimitive->valuestring));
        }
    }

    dnn_route_selection_descriptor_local_var = OpenAPI_dnn_route_selection_descriptor_create (
        ogs_strdup(dnn->valuestring),
        ssc_modes ? ssc_modesList : NULL,
        pdu_sess_types ? pdu_sess_typesList : NULL
        );

    return dnn_route_selection_descriptor_local_var;
end:
    return NULL;
}

OpenAPI_dnn_route_selection_descriptor_t *OpenAPI_dnn_route_selection_descriptor_copy(OpenAPI_dnn_route_selection_descriptor_t *dst, OpenAPI_dnn_route_selection_descriptor_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_dnn_route_selection_descriptor_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_dnn_route_selection_descriptor_convertToJSON() failed");
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

    OpenAPI_dnn_route_selection_descriptor_free(dst);
    dst = OpenAPI_dnn_route_selection_descriptor_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

