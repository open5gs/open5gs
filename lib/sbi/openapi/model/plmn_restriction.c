
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "plmn_restriction.h"

OpenAPI_plmn_restriction_t *OpenAPI_plmn_restriction_create(
    OpenAPI_set_t *rat_restrictions,
    OpenAPI_list_t *forbidden_areas,
    OpenAPI_service_area_restriction_t *service_area_restriction,
    OpenAPI_list_t *core_network_type_restrictions,
    OpenAPI_set_t *primary_rat_restrictions,
    OpenAPI_set_t *secondary_rat_restrictions
)
{
    OpenAPI_plmn_restriction_t *plmn_restriction_local_var = ogs_malloc(sizeof(OpenAPI_plmn_restriction_t));
    ogs_assert(plmn_restriction_local_var);

    plmn_restriction_local_var->rat_restrictions = rat_restrictions;
    plmn_restriction_local_var->forbidden_areas = forbidden_areas;
    plmn_restriction_local_var->service_area_restriction = service_area_restriction;
    plmn_restriction_local_var->core_network_type_restrictions = core_network_type_restrictions;
    plmn_restriction_local_var->primary_rat_restrictions = primary_rat_restrictions;
    plmn_restriction_local_var->secondary_rat_restrictions = secondary_rat_restrictions;

    return plmn_restriction_local_var;
}

void OpenAPI_plmn_restriction_free(OpenAPI_plmn_restriction_t *plmn_restriction)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == plmn_restriction) {
        return;
    }
    if (plmn_restriction->rat_restrictions) {
        OpenAPI_list_free(plmn_restriction->rat_restrictions);
        plmn_restriction->rat_restrictions = NULL;
    }
    if (plmn_restriction->forbidden_areas) {
        OpenAPI_list_for_each(plmn_restriction->forbidden_areas, node) {
            OpenAPI_area_free(node->data);
        }
        OpenAPI_list_free(plmn_restriction->forbidden_areas);
        plmn_restriction->forbidden_areas = NULL;
    }
    if (plmn_restriction->service_area_restriction) {
        OpenAPI_service_area_restriction_free(plmn_restriction->service_area_restriction);
        plmn_restriction->service_area_restriction = NULL;
    }
    if (plmn_restriction->core_network_type_restrictions) {
        OpenAPI_list_free(plmn_restriction->core_network_type_restrictions);
        plmn_restriction->core_network_type_restrictions = NULL;
    }
    if (plmn_restriction->primary_rat_restrictions) {
        OpenAPI_list_free(plmn_restriction->primary_rat_restrictions);
        plmn_restriction->primary_rat_restrictions = NULL;
    }
    if (plmn_restriction->secondary_rat_restrictions) {
        OpenAPI_list_free(plmn_restriction->secondary_rat_restrictions);
        plmn_restriction->secondary_rat_restrictions = NULL;
    }
    ogs_free(plmn_restriction);
}

cJSON *OpenAPI_plmn_restriction_convertToJSON(OpenAPI_plmn_restriction_t *plmn_restriction)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (plmn_restriction == NULL) {
        ogs_error("OpenAPI_plmn_restriction_convertToJSON() failed [PlmnRestriction]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (plmn_restriction->rat_restrictions != OpenAPI_rat_type_NULL) {
    cJSON *rat_restrictionsList = cJSON_AddArrayToObject(item, "ratRestrictions");
    if (rat_restrictionsList == NULL) {
        ogs_error("OpenAPI_plmn_restriction_convertToJSON() failed [rat_restrictions]");
        goto end;
    }
    OpenAPI_list_for_each(plmn_restriction->rat_restrictions, node) {
        if (cJSON_AddStringToObject(rat_restrictionsList, "", OpenAPI_rat_type_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_plmn_restriction_convertToJSON() failed [rat_restrictions]");
            goto end;
        }
    }
    }

    if (plmn_restriction->forbidden_areas) {
    cJSON *forbidden_areasList = cJSON_AddArrayToObject(item, "forbiddenAreas");
    if (forbidden_areasList == NULL) {
        ogs_error("OpenAPI_plmn_restriction_convertToJSON() failed [forbidden_areas]");
        goto end;
    }
    OpenAPI_list_for_each(plmn_restriction->forbidden_areas, node) {
        cJSON *itemLocal = OpenAPI_area_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_plmn_restriction_convertToJSON() failed [forbidden_areas]");
            goto end;
        }
        cJSON_AddItemToArray(forbidden_areasList, itemLocal);
    }
    }

    if (plmn_restriction->service_area_restriction) {
    cJSON *service_area_restriction_local_JSON = OpenAPI_service_area_restriction_convertToJSON(plmn_restriction->service_area_restriction);
    if (service_area_restriction_local_JSON == NULL) {
        ogs_error("OpenAPI_plmn_restriction_convertToJSON() failed [service_area_restriction]");
        goto end;
    }
    cJSON_AddItemToObject(item, "serviceAreaRestriction", service_area_restriction_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_plmn_restriction_convertToJSON() failed [service_area_restriction]");
        goto end;
    }
    }

    if (plmn_restriction->core_network_type_restrictions != OpenAPI_core_network_type_NULL) {
    cJSON *core_network_type_restrictionsList = cJSON_AddArrayToObject(item, "coreNetworkTypeRestrictions");
    if (core_network_type_restrictionsList == NULL) {
        ogs_error("OpenAPI_plmn_restriction_convertToJSON() failed [core_network_type_restrictions]");
        goto end;
    }
    OpenAPI_list_for_each(plmn_restriction->core_network_type_restrictions, node) {
        if (cJSON_AddStringToObject(core_network_type_restrictionsList, "", OpenAPI_core_network_type_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_plmn_restriction_convertToJSON() failed [core_network_type_restrictions]");
            goto end;
        }
    }
    }

    if (plmn_restriction->primary_rat_restrictions != OpenAPI_rat_type_NULL) {
    cJSON *primary_rat_restrictionsList = cJSON_AddArrayToObject(item, "primaryRatRestrictions");
    if (primary_rat_restrictionsList == NULL) {
        ogs_error("OpenAPI_plmn_restriction_convertToJSON() failed [primary_rat_restrictions]");
        goto end;
    }
    OpenAPI_list_for_each(plmn_restriction->primary_rat_restrictions, node) {
        if (cJSON_AddStringToObject(primary_rat_restrictionsList, "", OpenAPI_rat_type_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_plmn_restriction_convertToJSON() failed [primary_rat_restrictions]");
            goto end;
        }
    }
    }

    if (plmn_restriction->secondary_rat_restrictions != OpenAPI_rat_type_NULL) {
    cJSON *secondary_rat_restrictionsList = cJSON_AddArrayToObject(item, "secondaryRatRestrictions");
    if (secondary_rat_restrictionsList == NULL) {
        ogs_error("OpenAPI_plmn_restriction_convertToJSON() failed [secondary_rat_restrictions]");
        goto end;
    }
    OpenAPI_list_for_each(plmn_restriction->secondary_rat_restrictions, node) {
        if (cJSON_AddStringToObject(secondary_rat_restrictionsList, "", OpenAPI_rat_type_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_plmn_restriction_convertToJSON() failed [secondary_rat_restrictions]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_plmn_restriction_t *OpenAPI_plmn_restriction_parseFromJSON(cJSON *plmn_restrictionJSON)
{
    OpenAPI_plmn_restriction_t *plmn_restriction_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *rat_restrictions = NULL;
    OpenAPI_list_t *rat_restrictionsList = NULL;
    cJSON *forbidden_areas = NULL;
    OpenAPI_list_t *forbidden_areasList = NULL;
    cJSON *service_area_restriction = NULL;
    OpenAPI_service_area_restriction_t *service_area_restriction_local_nonprim = NULL;
    cJSON *core_network_type_restrictions = NULL;
    OpenAPI_list_t *core_network_type_restrictionsList = NULL;
    cJSON *primary_rat_restrictions = NULL;
    OpenAPI_list_t *primary_rat_restrictionsList = NULL;
    cJSON *secondary_rat_restrictions = NULL;
    OpenAPI_list_t *secondary_rat_restrictionsList = NULL;
    rat_restrictions = cJSON_GetObjectItemCaseSensitive(plmn_restrictionJSON, "ratRestrictions");
    if (rat_restrictions) {
        cJSON *rat_restrictions_local = NULL;
        if (!cJSON_IsArray(rat_restrictions)) {
            ogs_error("OpenAPI_plmn_restriction_parseFromJSON() failed [rat_restrictions]");
            goto end;
        }

        rat_restrictionsList = OpenAPI_list_create();

        cJSON_ArrayForEach(rat_restrictions_local, rat_restrictions) {
            OpenAPI_rat_type_e localEnum = OpenAPI_rat_type_NULL;
            if (!cJSON_IsString(rat_restrictions_local)) {
                ogs_error("OpenAPI_plmn_restriction_parseFromJSON() failed [rat_restrictions]");
                goto end;
            }
            localEnum = OpenAPI_rat_type_FromString(rat_restrictions_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"rat_restrictions\" is not supported. Ignoring it ...",
                         rat_restrictions_local->valuestring);
            } else {
                OpenAPI_list_add(rat_restrictionsList, (void *)localEnum);
            }
        }
        if (rat_restrictionsList->count == 0) {
            ogs_error("OpenAPI_plmn_restriction_parseFromJSON() failed: Expected rat_restrictionsList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    forbidden_areas = cJSON_GetObjectItemCaseSensitive(plmn_restrictionJSON, "forbiddenAreas");
    if (forbidden_areas) {
        cJSON *forbidden_areas_local = NULL;
        if (!cJSON_IsArray(forbidden_areas)) {
            ogs_error("OpenAPI_plmn_restriction_parseFromJSON() failed [forbidden_areas]");
            goto end;
        }

        forbidden_areasList = OpenAPI_list_create();

        cJSON_ArrayForEach(forbidden_areas_local, forbidden_areas) {
            if (!cJSON_IsObject(forbidden_areas_local)) {
                ogs_error("OpenAPI_plmn_restriction_parseFromJSON() failed [forbidden_areas]");
                goto end;
            }
            OpenAPI_area_t *forbidden_areasItem = OpenAPI_area_parseFromJSON(forbidden_areas_local);
            if (!forbidden_areasItem) {
                ogs_error("No forbidden_areasItem");
                goto end;
            }
            OpenAPI_list_add(forbidden_areasList, forbidden_areasItem);
        }
    }

    service_area_restriction = cJSON_GetObjectItemCaseSensitive(plmn_restrictionJSON, "serviceAreaRestriction");
    if (service_area_restriction) {
    service_area_restriction_local_nonprim = OpenAPI_service_area_restriction_parseFromJSON(service_area_restriction);
    if (!service_area_restriction_local_nonprim) {
        ogs_error("OpenAPI_service_area_restriction_parseFromJSON failed [service_area_restriction]");
        goto end;
    }
    }

    core_network_type_restrictions = cJSON_GetObjectItemCaseSensitive(plmn_restrictionJSON, "coreNetworkTypeRestrictions");
    if (core_network_type_restrictions) {
        cJSON *core_network_type_restrictions_local = NULL;
        if (!cJSON_IsArray(core_network_type_restrictions)) {
            ogs_error("OpenAPI_plmn_restriction_parseFromJSON() failed [core_network_type_restrictions]");
            goto end;
        }

        core_network_type_restrictionsList = OpenAPI_list_create();

        cJSON_ArrayForEach(core_network_type_restrictions_local, core_network_type_restrictions) {
            OpenAPI_core_network_type_e localEnum = OpenAPI_core_network_type_NULL;
            if (!cJSON_IsString(core_network_type_restrictions_local)) {
                ogs_error("OpenAPI_plmn_restriction_parseFromJSON() failed [core_network_type_restrictions]");
                goto end;
            }
            localEnum = OpenAPI_core_network_type_FromString(core_network_type_restrictions_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"core_network_type_restrictions\" is not supported. Ignoring it ...",
                         core_network_type_restrictions_local->valuestring);
            } else {
                OpenAPI_list_add(core_network_type_restrictionsList, (void *)localEnum);
            }
        }
        if (core_network_type_restrictionsList->count == 0) {
            ogs_error("OpenAPI_plmn_restriction_parseFromJSON() failed: Expected core_network_type_restrictionsList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    primary_rat_restrictions = cJSON_GetObjectItemCaseSensitive(plmn_restrictionJSON, "primaryRatRestrictions");
    if (primary_rat_restrictions) {
        cJSON *primary_rat_restrictions_local = NULL;
        if (!cJSON_IsArray(primary_rat_restrictions)) {
            ogs_error("OpenAPI_plmn_restriction_parseFromJSON() failed [primary_rat_restrictions]");
            goto end;
        }

        primary_rat_restrictionsList = OpenAPI_list_create();

        cJSON_ArrayForEach(primary_rat_restrictions_local, primary_rat_restrictions) {
            OpenAPI_rat_type_e localEnum = OpenAPI_rat_type_NULL;
            if (!cJSON_IsString(primary_rat_restrictions_local)) {
                ogs_error("OpenAPI_plmn_restriction_parseFromJSON() failed [primary_rat_restrictions]");
                goto end;
            }
            localEnum = OpenAPI_rat_type_FromString(primary_rat_restrictions_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"primary_rat_restrictions\" is not supported. Ignoring it ...",
                         primary_rat_restrictions_local->valuestring);
            } else {
                OpenAPI_list_add(primary_rat_restrictionsList, (void *)localEnum);
            }
        }
        if (primary_rat_restrictionsList->count == 0) {
            ogs_error("OpenAPI_plmn_restriction_parseFromJSON() failed: Expected primary_rat_restrictionsList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    secondary_rat_restrictions = cJSON_GetObjectItemCaseSensitive(plmn_restrictionJSON, "secondaryRatRestrictions");
    if (secondary_rat_restrictions) {
        cJSON *secondary_rat_restrictions_local = NULL;
        if (!cJSON_IsArray(secondary_rat_restrictions)) {
            ogs_error("OpenAPI_plmn_restriction_parseFromJSON() failed [secondary_rat_restrictions]");
            goto end;
        }

        secondary_rat_restrictionsList = OpenAPI_list_create();

        cJSON_ArrayForEach(secondary_rat_restrictions_local, secondary_rat_restrictions) {
            OpenAPI_rat_type_e localEnum = OpenAPI_rat_type_NULL;
            if (!cJSON_IsString(secondary_rat_restrictions_local)) {
                ogs_error("OpenAPI_plmn_restriction_parseFromJSON() failed [secondary_rat_restrictions]");
                goto end;
            }
            localEnum = OpenAPI_rat_type_FromString(secondary_rat_restrictions_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"secondary_rat_restrictions\" is not supported. Ignoring it ...",
                         secondary_rat_restrictions_local->valuestring);
            } else {
                OpenAPI_list_add(secondary_rat_restrictionsList, (void *)localEnum);
            }
        }
        if (secondary_rat_restrictionsList->count == 0) {
            ogs_error("OpenAPI_plmn_restriction_parseFromJSON() failed: Expected secondary_rat_restrictionsList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    plmn_restriction_local_var = OpenAPI_plmn_restriction_create (
        rat_restrictions ? rat_restrictionsList : NULL,
        forbidden_areas ? forbidden_areasList : NULL,
        service_area_restriction ? service_area_restriction_local_nonprim : NULL,
        core_network_type_restrictions ? core_network_type_restrictionsList : NULL,
        primary_rat_restrictions ? primary_rat_restrictionsList : NULL,
        secondary_rat_restrictions ? secondary_rat_restrictionsList : NULL
    );

    return plmn_restriction_local_var;
end:
    if (rat_restrictionsList) {
        OpenAPI_list_free(rat_restrictionsList);
        rat_restrictionsList = NULL;
    }
    if (forbidden_areasList) {
        OpenAPI_list_for_each(forbidden_areasList, node) {
            OpenAPI_area_free(node->data);
        }
        OpenAPI_list_free(forbidden_areasList);
        forbidden_areasList = NULL;
    }
    if (service_area_restriction_local_nonprim) {
        OpenAPI_service_area_restriction_free(service_area_restriction_local_nonprim);
        service_area_restriction_local_nonprim = NULL;
    }
    if (core_network_type_restrictionsList) {
        OpenAPI_list_free(core_network_type_restrictionsList);
        core_network_type_restrictionsList = NULL;
    }
    if (primary_rat_restrictionsList) {
        OpenAPI_list_free(primary_rat_restrictionsList);
        primary_rat_restrictionsList = NULL;
    }
    if (secondary_rat_restrictionsList) {
        OpenAPI_list_free(secondary_rat_restrictionsList);
        secondary_rat_restrictionsList = NULL;
    }
    return NULL;
}

OpenAPI_plmn_restriction_t *OpenAPI_plmn_restriction_copy(OpenAPI_plmn_restriction_t *dst, OpenAPI_plmn_restriction_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_plmn_restriction_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_plmn_restriction_convertToJSON() failed");
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

    OpenAPI_plmn_restriction_free(dst);
    dst = OpenAPI_plmn_restriction_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

