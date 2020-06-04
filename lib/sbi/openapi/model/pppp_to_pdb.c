
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pppp_to_pdb.h"

OpenAPI_pppp_to_pdb_t *OpenAPI_pppp_to_pdb_create(
    int pppp,
    int pdb
    )
{
    OpenAPI_pppp_to_pdb_t *pppp_to_pdb_local_var = OpenAPI_malloc(sizeof(OpenAPI_pppp_to_pdb_t));
    if (!pppp_to_pdb_local_var) {
        return NULL;
    }
    pppp_to_pdb_local_var->pppp = pppp;
    pppp_to_pdb_local_var->pdb = pdb;

    return pppp_to_pdb_local_var;
}

void OpenAPI_pppp_to_pdb_free(OpenAPI_pppp_to_pdb_t *pppp_to_pdb)
{
    if (NULL == pppp_to_pdb) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(pppp_to_pdb);
}

cJSON *OpenAPI_pppp_to_pdb_convertToJSON(OpenAPI_pppp_to_pdb_t *pppp_to_pdb)
{
    cJSON *item = NULL;

    if (pppp_to_pdb == NULL) {
        ogs_error("OpenAPI_pppp_to_pdb_convertToJSON() failed [PpppToPdb]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!pppp_to_pdb->pppp) {
        ogs_error("OpenAPI_pppp_to_pdb_convertToJSON() failed [pppp]");
        goto end;
    }
    if (cJSON_AddNumberToObject(item, "pppp", pppp_to_pdb->pppp) == NULL) {
        ogs_error("OpenAPI_pppp_to_pdb_convertToJSON() failed [pppp]");
        goto end;
    }

    if (!pppp_to_pdb->pdb) {
        ogs_error("OpenAPI_pppp_to_pdb_convertToJSON() failed [pdb]");
        goto end;
    }
    if (cJSON_AddNumberToObject(item, "pdb", pppp_to_pdb->pdb) == NULL) {
        ogs_error("OpenAPI_pppp_to_pdb_convertToJSON() failed [pdb]");
        goto end;
    }

end:
    return item;
}

OpenAPI_pppp_to_pdb_t *OpenAPI_pppp_to_pdb_parseFromJSON(cJSON *pppp_to_pdbJSON)
{
    OpenAPI_pppp_to_pdb_t *pppp_to_pdb_local_var = NULL;
    cJSON *pppp = cJSON_GetObjectItemCaseSensitive(pppp_to_pdbJSON, "pppp");
    if (!pppp) {
        ogs_error("OpenAPI_pppp_to_pdb_parseFromJSON() failed [pppp]");
        goto end;
    }


    if (!cJSON_IsNumber(pppp)) {
        ogs_error("OpenAPI_pppp_to_pdb_parseFromJSON() failed [pppp]");
        goto end;
    }

    cJSON *pdb = cJSON_GetObjectItemCaseSensitive(pppp_to_pdbJSON, "pdb");
    if (!pdb) {
        ogs_error("OpenAPI_pppp_to_pdb_parseFromJSON() failed [pdb]");
        goto end;
    }


    if (!cJSON_IsNumber(pdb)) {
        ogs_error("OpenAPI_pppp_to_pdb_parseFromJSON() failed [pdb]");
        goto end;
    }

    pppp_to_pdb_local_var = OpenAPI_pppp_to_pdb_create (
        pppp->valuedouble,
        pdb->valuedouble
        );

    return pppp_to_pdb_local_var;
end:
    return NULL;
}

