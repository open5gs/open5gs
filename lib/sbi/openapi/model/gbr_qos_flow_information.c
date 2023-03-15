
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "gbr_qos_flow_information.h"

OpenAPI_gbr_qos_flow_information_t *OpenAPI_gbr_qos_flow_information_create(
    char *max_fbr_dl,
    char *max_fbr_ul,
    char *gua_fbr_dl,
    char *gua_fbr_ul,
    OpenAPI_notification_control_e notif_control,
    bool is_max_packet_loss_rate_dl,
    int max_packet_loss_rate_dl,
    bool is_max_packet_loss_rate_ul,
    int max_packet_loss_rate_ul,
    OpenAPI_list_t *alternative_qos_profile_list
)
{
    OpenAPI_gbr_qos_flow_information_t *gbr_qos_flow_information_local_var = ogs_malloc(sizeof(OpenAPI_gbr_qos_flow_information_t));
    ogs_assert(gbr_qos_flow_information_local_var);

    gbr_qos_flow_information_local_var->max_fbr_dl = max_fbr_dl;
    gbr_qos_flow_information_local_var->max_fbr_ul = max_fbr_ul;
    gbr_qos_flow_information_local_var->gua_fbr_dl = gua_fbr_dl;
    gbr_qos_flow_information_local_var->gua_fbr_ul = gua_fbr_ul;
    gbr_qos_flow_information_local_var->notif_control = notif_control;
    gbr_qos_flow_information_local_var->is_max_packet_loss_rate_dl = is_max_packet_loss_rate_dl;
    gbr_qos_flow_information_local_var->max_packet_loss_rate_dl = max_packet_loss_rate_dl;
    gbr_qos_flow_information_local_var->is_max_packet_loss_rate_ul = is_max_packet_loss_rate_ul;
    gbr_qos_flow_information_local_var->max_packet_loss_rate_ul = max_packet_loss_rate_ul;
    gbr_qos_flow_information_local_var->alternative_qos_profile_list = alternative_qos_profile_list;

    return gbr_qos_flow_information_local_var;
}

void OpenAPI_gbr_qos_flow_information_free(OpenAPI_gbr_qos_flow_information_t *gbr_qos_flow_information)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == gbr_qos_flow_information) {
        return;
    }
    if (gbr_qos_flow_information->max_fbr_dl) {
        ogs_free(gbr_qos_flow_information->max_fbr_dl);
        gbr_qos_flow_information->max_fbr_dl = NULL;
    }
    if (gbr_qos_flow_information->max_fbr_ul) {
        ogs_free(gbr_qos_flow_information->max_fbr_ul);
        gbr_qos_flow_information->max_fbr_ul = NULL;
    }
    if (gbr_qos_flow_information->gua_fbr_dl) {
        ogs_free(gbr_qos_flow_information->gua_fbr_dl);
        gbr_qos_flow_information->gua_fbr_dl = NULL;
    }
    if (gbr_qos_flow_information->gua_fbr_ul) {
        ogs_free(gbr_qos_flow_information->gua_fbr_ul);
        gbr_qos_flow_information->gua_fbr_ul = NULL;
    }
    if (gbr_qos_flow_information->alternative_qos_profile_list) {
        OpenAPI_list_for_each(gbr_qos_flow_information->alternative_qos_profile_list, node) {
            OpenAPI_alternative_qos_profile_free(node->data);
        }
        OpenAPI_list_free(gbr_qos_flow_information->alternative_qos_profile_list);
        gbr_qos_flow_information->alternative_qos_profile_list = NULL;
    }
    ogs_free(gbr_qos_flow_information);
}

cJSON *OpenAPI_gbr_qos_flow_information_convertToJSON(OpenAPI_gbr_qos_flow_information_t *gbr_qos_flow_information)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (gbr_qos_flow_information == NULL) {
        ogs_error("OpenAPI_gbr_qos_flow_information_convertToJSON() failed [GbrQosFlowInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!gbr_qos_flow_information->max_fbr_dl) {
        ogs_error("OpenAPI_gbr_qos_flow_information_convertToJSON() failed [max_fbr_dl]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "maxFbrDl", gbr_qos_flow_information->max_fbr_dl) == NULL) {
        ogs_error("OpenAPI_gbr_qos_flow_information_convertToJSON() failed [max_fbr_dl]");
        goto end;
    }

    if (!gbr_qos_flow_information->max_fbr_ul) {
        ogs_error("OpenAPI_gbr_qos_flow_information_convertToJSON() failed [max_fbr_ul]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "maxFbrUl", gbr_qos_flow_information->max_fbr_ul) == NULL) {
        ogs_error("OpenAPI_gbr_qos_flow_information_convertToJSON() failed [max_fbr_ul]");
        goto end;
    }

    if (!gbr_qos_flow_information->gua_fbr_dl) {
        ogs_error("OpenAPI_gbr_qos_flow_information_convertToJSON() failed [gua_fbr_dl]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "guaFbrDl", gbr_qos_flow_information->gua_fbr_dl) == NULL) {
        ogs_error("OpenAPI_gbr_qos_flow_information_convertToJSON() failed [gua_fbr_dl]");
        goto end;
    }

    if (!gbr_qos_flow_information->gua_fbr_ul) {
        ogs_error("OpenAPI_gbr_qos_flow_information_convertToJSON() failed [gua_fbr_ul]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "guaFbrUl", gbr_qos_flow_information->gua_fbr_ul) == NULL) {
        ogs_error("OpenAPI_gbr_qos_flow_information_convertToJSON() failed [gua_fbr_ul]");
        goto end;
    }

    if (gbr_qos_flow_information->notif_control != OpenAPI_notification_control_NULL) {
    if (cJSON_AddStringToObject(item, "notifControl", OpenAPI_notification_control_ToString(gbr_qos_flow_information->notif_control)) == NULL) {
        ogs_error("OpenAPI_gbr_qos_flow_information_convertToJSON() failed [notif_control]");
        goto end;
    }
    }

    if (gbr_qos_flow_information->is_max_packet_loss_rate_dl) {
    if (cJSON_AddNumberToObject(item, "maxPacketLossRateDl", gbr_qos_flow_information->max_packet_loss_rate_dl) == NULL) {
        ogs_error("OpenAPI_gbr_qos_flow_information_convertToJSON() failed [max_packet_loss_rate_dl]");
        goto end;
    }
    }

    if (gbr_qos_flow_information->is_max_packet_loss_rate_ul) {
    if (cJSON_AddNumberToObject(item, "maxPacketLossRateUl", gbr_qos_flow_information->max_packet_loss_rate_ul) == NULL) {
        ogs_error("OpenAPI_gbr_qos_flow_information_convertToJSON() failed [max_packet_loss_rate_ul]");
        goto end;
    }
    }

    if (gbr_qos_flow_information->alternative_qos_profile_list) {
    cJSON *alternative_qos_profile_listList = cJSON_AddArrayToObject(item, "alternativeQosProfileList");
    if (alternative_qos_profile_listList == NULL) {
        ogs_error("OpenAPI_gbr_qos_flow_information_convertToJSON() failed [alternative_qos_profile_list]");
        goto end;
    }
    OpenAPI_list_for_each(gbr_qos_flow_information->alternative_qos_profile_list, node) {
        cJSON *itemLocal = OpenAPI_alternative_qos_profile_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_gbr_qos_flow_information_convertToJSON() failed [alternative_qos_profile_list]");
            goto end;
        }
        cJSON_AddItemToArray(alternative_qos_profile_listList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_gbr_qos_flow_information_t *OpenAPI_gbr_qos_flow_information_parseFromJSON(cJSON *gbr_qos_flow_informationJSON)
{
    OpenAPI_gbr_qos_flow_information_t *gbr_qos_flow_information_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *max_fbr_dl = NULL;
    cJSON *max_fbr_ul = NULL;
    cJSON *gua_fbr_dl = NULL;
    cJSON *gua_fbr_ul = NULL;
    cJSON *notif_control = NULL;
    OpenAPI_notification_control_e notif_controlVariable = 0;
    cJSON *max_packet_loss_rate_dl = NULL;
    cJSON *max_packet_loss_rate_ul = NULL;
    cJSON *alternative_qos_profile_list = NULL;
    OpenAPI_list_t *alternative_qos_profile_listList = NULL;
    max_fbr_dl = cJSON_GetObjectItemCaseSensitive(gbr_qos_flow_informationJSON, "maxFbrDl");
    if (!max_fbr_dl) {
        ogs_error("OpenAPI_gbr_qos_flow_information_parseFromJSON() failed [max_fbr_dl]");
        goto end;
    }
    if (!cJSON_IsString(max_fbr_dl)) {
        ogs_error("OpenAPI_gbr_qos_flow_information_parseFromJSON() failed [max_fbr_dl]");
        goto end;
    }

    max_fbr_ul = cJSON_GetObjectItemCaseSensitive(gbr_qos_flow_informationJSON, "maxFbrUl");
    if (!max_fbr_ul) {
        ogs_error("OpenAPI_gbr_qos_flow_information_parseFromJSON() failed [max_fbr_ul]");
        goto end;
    }
    if (!cJSON_IsString(max_fbr_ul)) {
        ogs_error("OpenAPI_gbr_qos_flow_information_parseFromJSON() failed [max_fbr_ul]");
        goto end;
    }

    gua_fbr_dl = cJSON_GetObjectItemCaseSensitive(gbr_qos_flow_informationJSON, "guaFbrDl");
    if (!gua_fbr_dl) {
        ogs_error("OpenAPI_gbr_qos_flow_information_parseFromJSON() failed [gua_fbr_dl]");
        goto end;
    }
    if (!cJSON_IsString(gua_fbr_dl)) {
        ogs_error("OpenAPI_gbr_qos_flow_information_parseFromJSON() failed [gua_fbr_dl]");
        goto end;
    }

    gua_fbr_ul = cJSON_GetObjectItemCaseSensitive(gbr_qos_flow_informationJSON, "guaFbrUl");
    if (!gua_fbr_ul) {
        ogs_error("OpenAPI_gbr_qos_flow_information_parseFromJSON() failed [gua_fbr_ul]");
        goto end;
    }
    if (!cJSON_IsString(gua_fbr_ul)) {
        ogs_error("OpenAPI_gbr_qos_flow_information_parseFromJSON() failed [gua_fbr_ul]");
        goto end;
    }

    notif_control = cJSON_GetObjectItemCaseSensitive(gbr_qos_flow_informationJSON, "notifControl");
    if (notif_control) {
    if (!cJSON_IsString(notif_control)) {
        ogs_error("OpenAPI_gbr_qos_flow_information_parseFromJSON() failed [notif_control]");
        goto end;
    }
    notif_controlVariable = OpenAPI_notification_control_FromString(notif_control->valuestring);
    }

    max_packet_loss_rate_dl = cJSON_GetObjectItemCaseSensitive(gbr_qos_flow_informationJSON, "maxPacketLossRateDl");
    if (max_packet_loss_rate_dl) {
    if (!cJSON_IsNumber(max_packet_loss_rate_dl)) {
        ogs_error("OpenAPI_gbr_qos_flow_information_parseFromJSON() failed [max_packet_loss_rate_dl]");
        goto end;
    }
    }

    max_packet_loss_rate_ul = cJSON_GetObjectItemCaseSensitive(gbr_qos_flow_informationJSON, "maxPacketLossRateUl");
    if (max_packet_loss_rate_ul) {
    if (!cJSON_IsNumber(max_packet_loss_rate_ul)) {
        ogs_error("OpenAPI_gbr_qos_flow_information_parseFromJSON() failed [max_packet_loss_rate_ul]");
        goto end;
    }
    }

    alternative_qos_profile_list = cJSON_GetObjectItemCaseSensitive(gbr_qos_flow_informationJSON, "alternativeQosProfileList");
    if (alternative_qos_profile_list) {
        cJSON *alternative_qos_profile_list_local = NULL;
        if (!cJSON_IsArray(alternative_qos_profile_list)) {
            ogs_error("OpenAPI_gbr_qos_flow_information_parseFromJSON() failed [alternative_qos_profile_list]");
            goto end;
        }

        alternative_qos_profile_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(alternative_qos_profile_list_local, alternative_qos_profile_list) {
            if (!cJSON_IsObject(alternative_qos_profile_list_local)) {
                ogs_error("OpenAPI_gbr_qos_flow_information_parseFromJSON() failed [alternative_qos_profile_list]");
                goto end;
            }
            OpenAPI_alternative_qos_profile_t *alternative_qos_profile_listItem = OpenAPI_alternative_qos_profile_parseFromJSON(alternative_qos_profile_list_local);
            if (!alternative_qos_profile_listItem) {
                ogs_error("No alternative_qos_profile_listItem");
                goto end;
            }
            OpenAPI_list_add(alternative_qos_profile_listList, alternative_qos_profile_listItem);
        }
    }

    gbr_qos_flow_information_local_var = OpenAPI_gbr_qos_flow_information_create (
        ogs_strdup(max_fbr_dl->valuestring),
        ogs_strdup(max_fbr_ul->valuestring),
        ogs_strdup(gua_fbr_dl->valuestring),
        ogs_strdup(gua_fbr_ul->valuestring),
        notif_control ? notif_controlVariable : 0,
        max_packet_loss_rate_dl ? true : false,
        max_packet_loss_rate_dl ? max_packet_loss_rate_dl->valuedouble : 0,
        max_packet_loss_rate_ul ? true : false,
        max_packet_loss_rate_ul ? max_packet_loss_rate_ul->valuedouble : 0,
        alternative_qos_profile_list ? alternative_qos_profile_listList : NULL
    );

    return gbr_qos_flow_information_local_var;
end:
    if (alternative_qos_profile_listList) {
        OpenAPI_list_for_each(alternative_qos_profile_listList, node) {
            OpenAPI_alternative_qos_profile_free(node->data);
        }
        OpenAPI_list_free(alternative_qos_profile_listList);
        alternative_qos_profile_listList = NULL;
    }
    return NULL;
}

OpenAPI_gbr_qos_flow_information_t *OpenAPI_gbr_qos_flow_information_copy(OpenAPI_gbr_qos_flow_information_t *dst, OpenAPI_gbr_qos_flow_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_gbr_qos_flow_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_gbr_qos_flow_information_convertToJSON() failed");
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

    OpenAPI_gbr_qos_flow_information_free(dst);
    dst = OpenAPI_gbr_qos_flow_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

