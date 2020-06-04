
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
    OpenAPI_list_for_each(dnn_route_selection_descriptor->ssc_modes, node) {
        OpenAPI_ssc_mode_free(node->data);
    }
    OpenAPI_list_free(dnn_route_selection_descriptor->ssc_modes);
    OpenAPI_list_for_each(dnn_route_selection_descriptor->pdu_sess_types, node) {
        OpenAPI_pdu_session_type_free(node->data);
    }
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
        cJSON *ssc_modesList = cJSON_AddArrayToObject(item, "sscModes");
        if (ssc_modesList == NULL) {
            ogs_error("OpenAPI_dnn_route_selection_descriptor_convertToJSON() failed [ssc_modes]");
            goto end;
        }

        OpenAPI_lnode_t *ssc_modes_node;
        if (dnn_route_selection_descriptor->ssc_modes) {
            OpenAPI_list_for_each(dnn_route_selection_descriptor->ssc_modes, ssc_modes_node) {
                cJSON *itemLocal = OpenAPI_ssc_mode_convertToJSON(ssc_modes_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_dnn_route_selection_descriptor_convertToJSON() failed [ssc_modes]");
                    goto end;
                }
                cJSON_AddItemToArray(ssc_modesList, itemLocal);
            }
        }
    }

    if (dnn_route_selection_descriptor->pdu_sess_types) {
        cJSON *pdu_sess_typesList = cJSON_AddArrayToObject(item, "pduSessTypes");
        if (pdu_sess_typesList == NULL) {
            ogs_error("OpenAPI_dnn_route_selection_descriptor_convertToJSON() failed [pdu_sess_types]");
            goto end;
        }

        OpenAPI_lnode_t *pdu_sess_types_node;
        if (dnn_route_selection_descriptor->pdu_sess_types) {
            OpenAPI_list_for_each(dnn_route_selection_descriptor->pdu_sess_types, pdu_sess_types_node) {
                cJSON *itemLocal = OpenAPI_pdu_session_type_convertToJSON(pdu_sess_types_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_dnn_route_selection_descriptor_convertToJSON() failed [pdu_sess_types]");
                    goto end;
                }
                cJSON_AddItemToArray(pdu_sess_typesList, itemLocal);
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
            if (!cJSON_IsObject(ssc_modes_local_nonprimitive)) {
                ogs_error("OpenAPI_dnn_route_selection_descriptor_parseFromJSON() failed [ssc_modes]");
                goto end;
            }
            OpenAPI_ssc_mode_t *ssc_modesItem = OpenAPI_ssc_mode_parseFromJSON(ssc_modes_local_nonprimitive);

            OpenAPI_list_add(ssc_modesList, ssc_modesItem);
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
            if (!cJSON_IsObject(pdu_sess_types_local_nonprimitive)) {
                ogs_error("OpenAPI_dnn_route_selection_descriptor_parseFromJSON() failed [pdu_sess_types]");
                goto end;
            }
            OpenAPI_pdu_session_type_t *pdu_sess_typesItem = OpenAPI_pdu_session_type_parseFromJSON(pdu_sess_types_local_nonprimitive);

            OpenAPI_list_add(pdu_sess_typesList, pdu_sess_typesItem);
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

