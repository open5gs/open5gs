
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "network_area_info_2.h"

OpenAPI_network_area_info_2_t *OpenAPI_network_area_info_2_create(
    OpenAPI_list_t *ecgis,
    OpenAPI_list_t *ncgis,
    OpenAPI_list_t *g_ran_node_ids,
    OpenAPI_list_t *tais
)
{
    OpenAPI_network_area_info_2_t *network_area_info_2_local_var = ogs_malloc(sizeof(OpenAPI_network_area_info_2_t));
    ogs_assert(network_area_info_2_local_var);

    network_area_info_2_local_var->ecgis = ecgis;
    network_area_info_2_local_var->ncgis = ncgis;
    network_area_info_2_local_var->g_ran_node_ids = g_ran_node_ids;
    network_area_info_2_local_var->tais = tais;

    return network_area_info_2_local_var;
}

void OpenAPI_network_area_info_2_free(OpenAPI_network_area_info_2_t *network_area_info_2)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == network_area_info_2) {
        return;
    }
    if (network_area_info_2->ecgis) {
        OpenAPI_list_for_each(network_area_info_2->ecgis, node) {
            OpenAPI_ecgi_1_free(node->data);
        }
        OpenAPI_list_free(network_area_info_2->ecgis);
        network_area_info_2->ecgis = NULL;
    }
    if (network_area_info_2->ncgis) {
        OpenAPI_list_for_each(network_area_info_2->ncgis, node) {
            OpenAPI_ncgi_1_free(node->data);
        }
        OpenAPI_list_free(network_area_info_2->ncgis);
        network_area_info_2->ncgis = NULL;
    }
    if (network_area_info_2->g_ran_node_ids) {
        OpenAPI_list_for_each(network_area_info_2->g_ran_node_ids, node) {
            OpenAPI_global_ran_node_id_1_free(node->data);
        }
        OpenAPI_list_free(network_area_info_2->g_ran_node_ids);
        network_area_info_2->g_ran_node_ids = NULL;
    }
    if (network_area_info_2->tais) {
        OpenAPI_list_for_each(network_area_info_2->tais, node) {
            OpenAPI_tai_1_free(node->data);
        }
        OpenAPI_list_free(network_area_info_2->tais);
        network_area_info_2->tais = NULL;
    }
    ogs_free(network_area_info_2);
}

cJSON *OpenAPI_network_area_info_2_convertToJSON(OpenAPI_network_area_info_2_t *network_area_info_2)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (network_area_info_2 == NULL) {
        ogs_error("OpenAPI_network_area_info_2_convertToJSON() failed [NetworkAreaInfo_2]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (network_area_info_2->ecgis) {
    cJSON *ecgisList = cJSON_AddArrayToObject(item, "ecgis");
    if (ecgisList == NULL) {
        ogs_error("OpenAPI_network_area_info_2_convertToJSON() failed [ecgis]");
        goto end;
    }
    OpenAPI_list_for_each(network_area_info_2->ecgis, node) {
        cJSON *itemLocal = OpenAPI_ecgi_1_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_network_area_info_2_convertToJSON() failed [ecgis]");
            goto end;
        }
        cJSON_AddItemToArray(ecgisList, itemLocal);
    }
    }

    if (network_area_info_2->ncgis) {
    cJSON *ncgisList = cJSON_AddArrayToObject(item, "ncgis");
    if (ncgisList == NULL) {
        ogs_error("OpenAPI_network_area_info_2_convertToJSON() failed [ncgis]");
        goto end;
    }
    OpenAPI_list_for_each(network_area_info_2->ncgis, node) {
        cJSON *itemLocal = OpenAPI_ncgi_1_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_network_area_info_2_convertToJSON() failed [ncgis]");
            goto end;
        }
        cJSON_AddItemToArray(ncgisList, itemLocal);
    }
    }

    if (network_area_info_2->g_ran_node_ids) {
    cJSON *g_ran_node_idsList = cJSON_AddArrayToObject(item, "gRanNodeIds");
    if (g_ran_node_idsList == NULL) {
        ogs_error("OpenAPI_network_area_info_2_convertToJSON() failed [g_ran_node_ids]");
        goto end;
    }
    OpenAPI_list_for_each(network_area_info_2->g_ran_node_ids, node) {
        cJSON *itemLocal = OpenAPI_global_ran_node_id_1_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_network_area_info_2_convertToJSON() failed [g_ran_node_ids]");
            goto end;
        }
        cJSON_AddItemToArray(g_ran_node_idsList, itemLocal);
    }
    }

    if (network_area_info_2->tais) {
    cJSON *taisList = cJSON_AddArrayToObject(item, "tais");
    if (taisList == NULL) {
        ogs_error("OpenAPI_network_area_info_2_convertToJSON() failed [tais]");
        goto end;
    }
    OpenAPI_list_for_each(network_area_info_2->tais, node) {
        cJSON *itemLocal = OpenAPI_tai_1_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_network_area_info_2_convertToJSON() failed [tais]");
            goto end;
        }
        cJSON_AddItemToArray(taisList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_network_area_info_2_t *OpenAPI_network_area_info_2_parseFromJSON(cJSON *network_area_info_2JSON)
{
    OpenAPI_network_area_info_2_t *network_area_info_2_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ecgis = NULL;
    OpenAPI_list_t *ecgisList = NULL;
    cJSON *ncgis = NULL;
    OpenAPI_list_t *ncgisList = NULL;
    cJSON *g_ran_node_ids = NULL;
    OpenAPI_list_t *g_ran_node_idsList = NULL;
    cJSON *tais = NULL;
    OpenAPI_list_t *taisList = NULL;
    ecgis = cJSON_GetObjectItemCaseSensitive(network_area_info_2JSON, "ecgis");
    if (ecgis) {
        cJSON *ecgis_local = NULL;
        if (!cJSON_IsArray(ecgis)) {
            ogs_error("OpenAPI_network_area_info_2_parseFromJSON() failed [ecgis]");
            goto end;
        }

        ecgisList = OpenAPI_list_create();

        cJSON_ArrayForEach(ecgis_local, ecgis) {
            if (!cJSON_IsObject(ecgis_local)) {
                ogs_error("OpenAPI_network_area_info_2_parseFromJSON() failed [ecgis]");
                goto end;
            }
            OpenAPI_ecgi_1_t *ecgisItem = OpenAPI_ecgi_1_parseFromJSON(ecgis_local);
            if (!ecgisItem) {
                ogs_error("No ecgisItem");
                goto end;
            }
            OpenAPI_list_add(ecgisList, ecgisItem);
        }
    }

    ncgis = cJSON_GetObjectItemCaseSensitive(network_area_info_2JSON, "ncgis");
    if (ncgis) {
        cJSON *ncgis_local = NULL;
        if (!cJSON_IsArray(ncgis)) {
            ogs_error("OpenAPI_network_area_info_2_parseFromJSON() failed [ncgis]");
            goto end;
        }

        ncgisList = OpenAPI_list_create();

        cJSON_ArrayForEach(ncgis_local, ncgis) {
            if (!cJSON_IsObject(ncgis_local)) {
                ogs_error("OpenAPI_network_area_info_2_parseFromJSON() failed [ncgis]");
                goto end;
            }
            OpenAPI_ncgi_1_t *ncgisItem = OpenAPI_ncgi_1_parseFromJSON(ncgis_local);
            if (!ncgisItem) {
                ogs_error("No ncgisItem");
                goto end;
            }
            OpenAPI_list_add(ncgisList, ncgisItem);
        }
    }

    g_ran_node_ids = cJSON_GetObjectItemCaseSensitive(network_area_info_2JSON, "gRanNodeIds");
    if (g_ran_node_ids) {
        cJSON *g_ran_node_ids_local = NULL;
        if (!cJSON_IsArray(g_ran_node_ids)) {
            ogs_error("OpenAPI_network_area_info_2_parseFromJSON() failed [g_ran_node_ids]");
            goto end;
        }

        g_ran_node_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(g_ran_node_ids_local, g_ran_node_ids) {
            if (!cJSON_IsObject(g_ran_node_ids_local)) {
                ogs_error("OpenAPI_network_area_info_2_parseFromJSON() failed [g_ran_node_ids]");
                goto end;
            }
            OpenAPI_global_ran_node_id_1_t *g_ran_node_idsItem = OpenAPI_global_ran_node_id_1_parseFromJSON(g_ran_node_ids_local);
            if (!g_ran_node_idsItem) {
                ogs_error("No g_ran_node_idsItem");
                goto end;
            }
            OpenAPI_list_add(g_ran_node_idsList, g_ran_node_idsItem);
        }
    }

    tais = cJSON_GetObjectItemCaseSensitive(network_area_info_2JSON, "tais");
    if (tais) {
        cJSON *tais_local = NULL;
        if (!cJSON_IsArray(tais)) {
            ogs_error("OpenAPI_network_area_info_2_parseFromJSON() failed [tais]");
            goto end;
        }

        taisList = OpenAPI_list_create();

        cJSON_ArrayForEach(tais_local, tais) {
            if (!cJSON_IsObject(tais_local)) {
                ogs_error("OpenAPI_network_area_info_2_parseFromJSON() failed [tais]");
                goto end;
            }
            OpenAPI_tai_1_t *taisItem = OpenAPI_tai_1_parseFromJSON(tais_local);
            if (!taisItem) {
                ogs_error("No taisItem");
                goto end;
            }
            OpenAPI_list_add(taisList, taisItem);
        }
    }

    network_area_info_2_local_var = OpenAPI_network_area_info_2_create (
        ecgis ? ecgisList : NULL,
        ncgis ? ncgisList : NULL,
        g_ran_node_ids ? g_ran_node_idsList : NULL,
        tais ? taisList : NULL
    );

    return network_area_info_2_local_var;
end:
    if (ecgisList) {
        OpenAPI_list_for_each(ecgisList, node) {
            OpenAPI_ecgi_1_free(node->data);
        }
        OpenAPI_list_free(ecgisList);
        ecgisList = NULL;
    }
    if (ncgisList) {
        OpenAPI_list_for_each(ncgisList, node) {
            OpenAPI_ncgi_1_free(node->data);
        }
        OpenAPI_list_free(ncgisList);
        ncgisList = NULL;
    }
    if (g_ran_node_idsList) {
        OpenAPI_list_for_each(g_ran_node_idsList, node) {
            OpenAPI_global_ran_node_id_1_free(node->data);
        }
        OpenAPI_list_free(g_ran_node_idsList);
        g_ran_node_idsList = NULL;
    }
    if (taisList) {
        OpenAPI_list_for_each(taisList, node) {
            OpenAPI_tai_1_free(node->data);
        }
        OpenAPI_list_free(taisList);
        taisList = NULL;
    }
    return NULL;
}

OpenAPI_network_area_info_2_t *OpenAPI_network_area_info_2_copy(OpenAPI_network_area_info_2_t *dst, OpenAPI_network_area_info_2_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_network_area_info_2_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_network_area_info_2_convertToJSON() failed");
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

    OpenAPI_network_area_info_2_free(dst);
    dst = OpenAPI_network_area_info_2_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

