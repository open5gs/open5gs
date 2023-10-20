
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "dnn_route_selection_descriptor.h"

OpenAPI_dnn_route_selection_descriptor_t *OpenAPI_dnn_route_selection_descriptor_create(
    char *dnn,
    OpenAPI_list_t *ssc_modes,
    OpenAPI_list_t *pdu_sess_types,
    bool is_atsss_info,
    int atsss_info
)
{
    OpenAPI_dnn_route_selection_descriptor_t *dnn_route_selection_descriptor_local_var = ogs_malloc(sizeof(OpenAPI_dnn_route_selection_descriptor_t));
    ogs_assert(dnn_route_selection_descriptor_local_var);

    dnn_route_selection_descriptor_local_var->dnn = dnn;
    dnn_route_selection_descriptor_local_var->ssc_modes = ssc_modes;
    dnn_route_selection_descriptor_local_var->pdu_sess_types = pdu_sess_types;
    dnn_route_selection_descriptor_local_var->is_atsss_info = is_atsss_info;
    dnn_route_selection_descriptor_local_var->atsss_info = atsss_info;

    return dnn_route_selection_descriptor_local_var;
}

void OpenAPI_dnn_route_selection_descriptor_free(OpenAPI_dnn_route_selection_descriptor_t *dnn_route_selection_descriptor)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == dnn_route_selection_descriptor) {
        return;
    }
    if (dnn_route_selection_descriptor->dnn) {
        ogs_free(dnn_route_selection_descriptor->dnn);
        dnn_route_selection_descriptor->dnn = NULL;
    }
    if (dnn_route_selection_descriptor->ssc_modes) {
        OpenAPI_list_free(dnn_route_selection_descriptor->ssc_modes);
        dnn_route_selection_descriptor->ssc_modes = NULL;
    }
    if (dnn_route_selection_descriptor->pdu_sess_types) {
        OpenAPI_list_free(dnn_route_selection_descriptor->pdu_sess_types);
        dnn_route_selection_descriptor->pdu_sess_types = NULL;
    }
    ogs_free(dnn_route_selection_descriptor);
}

cJSON *OpenAPI_dnn_route_selection_descriptor_convertToJSON(OpenAPI_dnn_route_selection_descriptor_t *dnn_route_selection_descriptor)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (dnn_route_selection_descriptor == NULL) {
        ogs_error("OpenAPI_dnn_route_selection_descriptor_convertToJSON() failed [DnnRouteSelectionDescriptor]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!dnn_route_selection_descriptor->dnn) {
        ogs_error("OpenAPI_dnn_route_selection_descriptor_convertToJSON() failed [dnn]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "dnn", dnn_route_selection_descriptor->dnn) == NULL) {
        ogs_error("OpenAPI_dnn_route_selection_descriptor_convertToJSON() failed [dnn]");
        goto end;
    }

    if (dnn_route_selection_descriptor->ssc_modes != OpenAPI_ssc_mode_NULL) {
    cJSON *ssc_modesList = cJSON_AddArrayToObject(item, "sscModes");
    if (ssc_modesList == NULL) {
        ogs_error("OpenAPI_dnn_route_selection_descriptor_convertToJSON() failed [ssc_modes]");
        goto end;
    }
    OpenAPI_list_for_each(dnn_route_selection_descriptor->ssc_modes, node) {
        if (cJSON_AddStringToObject(ssc_modesList, "", OpenAPI_ssc_mode_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_dnn_route_selection_descriptor_convertToJSON() failed [ssc_modes]");
            goto end;
        }
    }
    }

    if (dnn_route_selection_descriptor->pdu_sess_types != OpenAPI_pdu_session_type_NULL) {
    cJSON *pdu_sess_typesList = cJSON_AddArrayToObject(item, "pduSessTypes");
    if (pdu_sess_typesList == NULL) {
        ogs_error("OpenAPI_dnn_route_selection_descriptor_convertToJSON() failed [pdu_sess_types]");
        goto end;
    }
    OpenAPI_list_for_each(dnn_route_selection_descriptor->pdu_sess_types, node) {
        if (cJSON_AddStringToObject(pdu_sess_typesList, "", OpenAPI_pdu_session_type_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_dnn_route_selection_descriptor_convertToJSON() failed [pdu_sess_types]");
            goto end;
        }
    }
    }

    if (dnn_route_selection_descriptor->is_atsss_info) {
    if (cJSON_AddBoolToObject(item, "atsssInfo", dnn_route_selection_descriptor->atsss_info) == NULL) {
        ogs_error("OpenAPI_dnn_route_selection_descriptor_convertToJSON() failed [atsss_info]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_dnn_route_selection_descriptor_t *OpenAPI_dnn_route_selection_descriptor_parseFromJSON(cJSON *dnn_route_selection_descriptorJSON)
{
    OpenAPI_dnn_route_selection_descriptor_t *dnn_route_selection_descriptor_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *dnn = NULL;
    cJSON *ssc_modes = NULL;
    OpenAPI_list_t *ssc_modesList = NULL;
    cJSON *pdu_sess_types = NULL;
    OpenAPI_list_t *pdu_sess_typesList = NULL;
    cJSON *atsss_info = NULL;
    dnn = cJSON_GetObjectItemCaseSensitive(dnn_route_selection_descriptorJSON, "dnn");
    if (!dnn) {
        ogs_error("OpenAPI_dnn_route_selection_descriptor_parseFromJSON() failed [dnn]");
        goto end;
    }
    if (!cJSON_IsString(dnn)) {
        ogs_error("OpenAPI_dnn_route_selection_descriptor_parseFromJSON() failed [dnn]");
        goto end;
    }

    ssc_modes = cJSON_GetObjectItemCaseSensitive(dnn_route_selection_descriptorJSON, "sscModes");
    if (ssc_modes) {
        cJSON *ssc_modes_local = NULL;
        if (!cJSON_IsArray(ssc_modes)) {
            ogs_error("OpenAPI_dnn_route_selection_descriptor_parseFromJSON() failed [ssc_modes]");
            goto end;
        }

        ssc_modesList = OpenAPI_list_create();

        cJSON_ArrayForEach(ssc_modes_local, ssc_modes) {
            OpenAPI_ssc_mode_e localEnum = OpenAPI_ssc_mode_NULL;
            if (!cJSON_IsString(ssc_modes_local)) {
                ogs_error("OpenAPI_dnn_route_selection_descriptor_parseFromJSON() failed [ssc_modes]");
                goto end;
            }
            localEnum = OpenAPI_ssc_mode_FromString(ssc_modes_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"ssc_modes\" is not supported. Ignoring it ...",
                         ssc_modes_local->valuestring);
            } else {
                OpenAPI_list_add(ssc_modesList, (void *)localEnum);
            }
        }
        if (ssc_modesList->count == 0) {
            ogs_error("OpenAPI_dnn_route_selection_descriptor_parseFromJSON() failed: Expected ssc_modesList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    pdu_sess_types = cJSON_GetObjectItemCaseSensitive(dnn_route_selection_descriptorJSON, "pduSessTypes");
    if (pdu_sess_types) {
        cJSON *pdu_sess_types_local = NULL;
        if (!cJSON_IsArray(pdu_sess_types)) {
            ogs_error("OpenAPI_dnn_route_selection_descriptor_parseFromJSON() failed [pdu_sess_types]");
            goto end;
        }

        pdu_sess_typesList = OpenAPI_list_create();

        cJSON_ArrayForEach(pdu_sess_types_local, pdu_sess_types) {
            OpenAPI_pdu_session_type_e localEnum = OpenAPI_pdu_session_type_NULL;
            if (!cJSON_IsString(pdu_sess_types_local)) {
                ogs_error("OpenAPI_dnn_route_selection_descriptor_parseFromJSON() failed [pdu_sess_types]");
                goto end;
            }
            localEnum = OpenAPI_pdu_session_type_FromString(pdu_sess_types_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"pdu_sess_types\" is not supported. Ignoring it ...",
                         pdu_sess_types_local->valuestring);
            } else {
                OpenAPI_list_add(pdu_sess_typesList, (void *)localEnum);
            }
        }
        if (pdu_sess_typesList->count == 0) {
            ogs_error("OpenAPI_dnn_route_selection_descriptor_parseFromJSON() failed: Expected pdu_sess_typesList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    atsss_info = cJSON_GetObjectItemCaseSensitive(dnn_route_selection_descriptorJSON, "atsssInfo");
    if (atsss_info) {
    if (!cJSON_IsBool(atsss_info)) {
        ogs_error("OpenAPI_dnn_route_selection_descriptor_parseFromJSON() failed [atsss_info]");
        goto end;
    }
    }

    dnn_route_selection_descriptor_local_var = OpenAPI_dnn_route_selection_descriptor_create (
        ogs_strdup(dnn->valuestring),
        ssc_modes ? ssc_modesList : NULL,
        pdu_sess_types ? pdu_sess_typesList : NULL,
        atsss_info ? true : false,
        atsss_info ? atsss_info->valueint : 0
    );

    return dnn_route_selection_descriptor_local_var;
end:
    if (ssc_modesList) {
        OpenAPI_list_free(ssc_modesList);
        ssc_modesList = NULL;
    }
    if (pdu_sess_typesList) {
        OpenAPI_list_free(pdu_sess_typesList);
        pdu_sess_typesList = NULL;
    }
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

