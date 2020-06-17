
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "civic_address.h"

OpenAPI_civic_address_t *OpenAPI_civic_address_create(
    char *country,
    char *a1,
    char *a2,
    char *a3,
    char *a4,
    char *a5,
    char *a6,
    char *prd,
    char *pod,
    char *sts,
    char *hno,
    char *hns,
    char *lmk,
    char *loc,
    char *nam,
    char *pc,
    char *bld,
    char *unit,
    char *flr,
    char *room,
    char *plc,
    char *pcn,
    char *pobox,
    char *addcode,
    char *seat,
    char *rd,
    char *rdsec,
    char *rdbr,
    char *rdsubbr,
    char *prm,
    char *pom
    )
{
    OpenAPI_civic_address_t *civic_address_local_var = OpenAPI_malloc(sizeof(OpenAPI_civic_address_t));
    if (!civic_address_local_var) {
        return NULL;
    }
    civic_address_local_var->country = country;
    civic_address_local_var->a1 = a1;
    civic_address_local_var->a2 = a2;
    civic_address_local_var->a3 = a3;
    civic_address_local_var->a4 = a4;
    civic_address_local_var->a5 = a5;
    civic_address_local_var->a6 = a6;
    civic_address_local_var->prd = prd;
    civic_address_local_var->pod = pod;
    civic_address_local_var->sts = sts;
    civic_address_local_var->hno = hno;
    civic_address_local_var->hns = hns;
    civic_address_local_var->lmk = lmk;
    civic_address_local_var->loc = loc;
    civic_address_local_var->nam = nam;
    civic_address_local_var->pc = pc;
    civic_address_local_var->bld = bld;
    civic_address_local_var->unit = unit;
    civic_address_local_var->flr = flr;
    civic_address_local_var->room = room;
    civic_address_local_var->plc = plc;
    civic_address_local_var->pcn = pcn;
    civic_address_local_var->pobox = pobox;
    civic_address_local_var->addcode = addcode;
    civic_address_local_var->seat = seat;
    civic_address_local_var->rd = rd;
    civic_address_local_var->rdsec = rdsec;
    civic_address_local_var->rdbr = rdbr;
    civic_address_local_var->rdsubbr = rdsubbr;
    civic_address_local_var->prm = prm;
    civic_address_local_var->pom = pom;

    return civic_address_local_var;
}

void OpenAPI_civic_address_free(OpenAPI_civic_address_t *civic_address)
{
    if (NULL == civic_address) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(civic_address->country);
    ogs_free(civic_address->a1);
    ogs_free(civic_address->a2);
    ogs_free(civic_address->a3);
    ogs_free(civic_address->a4);
    ogs_free(civic_address->a5);
    ogs_free(civic_address->a6);
    ogs_free(civic_address->prd);
    ogs_free(civic_address->pod);
    ogs_free(civic_address->sts);
    ogs_free(civic_address->hno);
    ogs_free(civic_address->hns);
    ogs_free(civic_address->lmk);
    ogs_free(civic_address->loc);
    ogs_free(civic_address->nam);
    ogs_free(civic_address->pc);
    ogs_free(civic_address->bld);
    ogs_free(civic_address->unit);
    ogs_free(civic_address->flr);
    ogs_free(civic_address->room);
    ogs_free(civic_address->plc);
    ogs_free(civic_address->pcn);
    ogs_free(civic_address->pobox);
    ogs_free(civic_address->addcode);
    ogs_free(civic_address->seat);
    ogs_free(civic_address->rd);
    ogs_free(civic_address->rdsec);
    ogs_free(civic_address->rdbr);
    ogs_free(civic_address->rdsubbr);
    ogs_free(civic_address->prm);
    ogs_free(civic_address->pom);
    ogs_free(civic_address);
}

cJSON *OpenAPI_civic_address_convertToJSON(OpenAPI_civic_address_t *civic_address)
{
    cJSON *item = NULL;

    if (civic_address == NULL) {
        ogs_error("OpenAPI_civic_address_convertToJSON() failed [CivicAddress]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (civic_address->country) {
        if (cJSON_AddStringToObject(item, "country", civic_address->country) == NULL) {
            ogs_error("OpenAPI_civic_address_convertToJSON() failed [country]");
            goto end;
        }
    }

    if (civic_address->a1) {
        if (cJSON_AddStringToObject(item, "A1", civic_address->a1) == NULL) {
            ogs_error("OpenAPI_civic_address_convertToJSON() failed [a1]");
            goto end;
        }
    }

    if (civic_address->a2) {
        if (cJSON_AddStringToObject(item, "A2", civic_address->a2) == NULL) {
            ogs_error("OpenAPI_civic_address_convertToJSON() failed [a2]");
            goto end;
        }
    }

    if (civic_address->a3) {
        if (cJSON_AddStringToObject(item, "A3", civic_address->a3) == NULL) {
            ogs_error("OpenAPI_civic_address_convertToJSON() failed [a3]");
            goto end;
        }
    }

    if (civic_address->a4) {
        if (cJSON_AddStringToObject(item, "A4", civic_address->a4) == NULL) {
            ogs_error("OpenAPI_civic_address_convertToJSON() failed [a4]");
            goto end;
        }
    }

    if (civic_address->a5) {
        if (cJSON_AddStringToObject(item, "A5", civic_address->a5) == NULL) {
            ogs_error("OpenAPI_civic_address_convertToJSON() failed [a5]");
            goto end;
        }
    }

    if (civic_address->a6) {
        if (cJSON_AddStringToObject(item, "A6", civic_address->a6) == NULL) {
            ogs_error("OpenAPI_civic_address_convertToJSON() failed [a6]");
            goto end;
        }
    }

    if (civic_address->prd) {
        if (cJSON_AddStringToObject(item, "PRD", civic_address->prd) == NULL) {
            ogs_error("OpenAPI_civic_address_convertToJSON() failed [prd]");
            goto end;
        }
    }

    if (civic_address->pod) {
        if (cJSON_AddStringToObject(item, "POD", civic_address->pod) == NULL) {
            ogs_error("OpenAPI_civic_address_convertToJSON() failed [pod]");
            goto end;
        }
    }

    if (civic_address->sts) {
        if (cJSON_AddStringToObject(item, "STS", civic_address->sts) == NULL) {
            ogs_error("OpenAPI_civic_address_convertToJSON() failed [sts]");
            goto end;
        }
    }

    if (civic_address->hno) {
        if (cJSON_AddStringToObject(item, "HNO", civic_address->hno) == NULL) {
            ogs_error("OpenAPI_civic_address_convertToJSON() failed [hno]");
            goto end;
        }
    }

    if (civic_address->hns) {
        if (cJSON_AddStringToObject(item, "HNS", civic_address->hns) == NULL) {
            ogs_error("OpenAPI_civic_address_convertToJSON() failed [hns]");
            goto end;
        }
    }

    if (civic_address->lmk) {
        if (cJSON_AddStringToObject(item, "LMK", civic_address->lmk) == NULL) {
            ogs_error("OpenAPI_civic_address_convertToJSON() failed [lmk]");
            goto end;
        }
    }

    if (civic_address->loc) {
        if (cJSON_AddStringToObject(item, "LOC", civic_address->loc) == NULL) {
            ogs_error("OpenAPI_civic_address_convertToJSON() failed [loc]");
            goto end;
        }
    }

    if (civic_address->nam) {
        if (cJSON_AddStringToObject(item, "NAM", civic_address->nam) == NULL) {
            ogs_error("OpenAPI_civic_address_convertToJSON() failed [nam]");
            goto end;
        }
    }

    if (civic_address->pc) {
        if (cJSON_AddStringToObject(item, "PC", civic_address->pc) == NULL) {
            ogs_error("OpenAPI_civic_address_convertToJSON() failed [pc]");
            goto end;
        }
    }

    if (civic_address->bld) {
        if (cJSON_AddStringToObject(item, "BLD", civic_address->bld) == NULL) {
            ogs_error("OpenAPI_civic_address_convertToJSON() failed [bld]");
            goto end;
        }
    }

    if (civic_address->unit) {
        if (cJSON_AddStringToObject(item, "UNIT", civic_address->unit) == NULL) {
            ogs_error("OpenAPI_civic_address_convertToJSON() failed [unit]");
            goto end;
        }
    }

    if (civic_address->flr) {
        if (cJSON_AddStringToObject(item, "FLR", civic_address->flr) == NULL) {
            ogs_error("OpenAPI_civic_address_convertToJSON() failed [flr]");
            goto end;
        }
    }

    if (civic_address->room) {
        if (cJSON_AddStringToObject(item, "ROOM", civic_address->room) == NULL) {
            ogs_error("OpenAPI_civic_address_convertToJSON() failed [room]");
            goto end;
        }
    }

    if (civic_address->plc) {
        if (cJSON_AddStringToObject(item, "PLC", civic_address->plc) == NULL) {
            ogs_error("OpenAPI_civic_address_convertToJSON() failed [plc]");
            goto end;
        }
    }

    if (civic_address->pcn) {
        if (cJSON_AddStringToObject(item, "PCN", civic_address->pcn) == NULL) {
            ogs_error("OpenAPI_civic_address_convertToJSON() failed [pcn]");
            goto end;
        }
    }

    if (civic_address->pobox) {
        if (cJSON_AddStringToObject(item, "POBOX", civic_address->pobox) == NULL) {
            ogs_error("OpenAPI_civic_address_convertToJSON() failed [pobox]");
            goto end;
        }
    }

    if (civic_address->addcode) {
        if (cJSON_AddStringToObject(item, "ADDCODE", civic_address->addcode) == NULL) {
            ogs_error("OpenAPI_civic_address_convertToJSON() failed [addcode]");
            goto end;
        }
    }

    if (civic_address->seat) {
        if (cJSON_AddStringToObject(item, "SEAT", civic_address->seat) == NULL) {
            ogs_error("OpenAPI_civic_address_convertToJSON() failed [seat]");
            goto end;
        }
    }

    if (civic_address->rd) {
        if (cJSON_AddStringToObject(item, "RD", civic_address->rd) == NULL) {
            ogs_error("OpenAPI_civic_address_convertToJSON() failed [rd]");
            goto end;
        }
    }

    if (civic_address->rdsec) {
        if (cJSON_AddStringToObject(item, "RDSEC", civic_address->rdsec) == NULL) {
            ogs_error("OpenAPI_civic_address_convertToJSON() failed [rdsec]");
            goto end;
        }
    }

    if (civic_address->rdbr) {
        if (cJSON_AddStringToObject(item, "RDBR", civic_address->rdbr) == NULL) {
            ogs_error("OpenAPI_civic_address_convertToJSON() failed [rdbr]");
            goto end;
        }
    }

    if (civic_address->rdsubbr) {
        if (cJSON_AddStringToObject(item, "RDSUBBR", civic_address->rdsubbr) == NULL) {
            ogs_error("OpenAPI_civic_address_convertToJSON() failed [rdsubbr]");
            goto end;
        }
    }

    if (civic_address->prm) {
        if (cJSON_AddStringToObject(item, "PRM", civic_address->prm) == NULL) {
            ogs_error("OpenAPI_civic_address_convertToJSON() failed [prm]");
            goto end;
        }
    }

    if (civic_address->pom) {
        if (cJSON_AddStringToObject(item, "POM", civic_address->pom) == NULL) {
            ogs_error("OpenAPI_civic_address_convertToJSON() failed [pom]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_civic_address_t *OpenAPI_civic_address_parseFromJSON(cJSON *civic_addressJSON)
{
    OpenAPI_civic_address_t *civic_address_local_var = NULL;
    cJSON *country = cJSON_GetObjectItemCaseSensitive(civic_addressJSON, "country");

    if (country) {
        if (!cJSON_IsString(country)) {
            ogs_error("OpenAPI_civic_address_parseFromJSON() failed [country]");
            goto end;
        }
    }

    cJSON *a1 = cJSON_GetObjectItemCaseSensitive(civic_addressJSON, "A1");

    if (a1) {
        if (!cJSON_IsString(a1)) {
            ogs_error("OpenAPI_civic_address_parseFromJSON() failed [a1]");
            goto end;
        }
    }

    cJSON *a2 = cJSON_GetObjectItemCaseSensitive(civic_addressJSON, "A2");

    if (a2) {
        if (!cJSON_IsString(a2)) {
            ogs_error("OpenAPI_civic_address_parseFromJSON() failed [a2]");
            goto end;
        }
    }

    cJSON *a3 = cJSON_GetObjectItemCaseSensitive(civic_addressJSON, "A3");

    if (a3) {
        if (!cJSON_IsString(a3)) {
            ogs_error("OpenAPI_civic_address_parseFromJSON() failed [a3]");
            goto end;
        }
    }

    cJSON *a4 = cJSON_GetObjectItemCaseSensitive(civic_addressJSON, "A4");

    if (a4) {
        if (!cJSON_IsString(a4)) {
            ogs_error("OpenAPI_civic_address_parseFromJSON() failed [a4]");
            goto end;
        }
    }

    cJSON *a5 = cJSON_GetObjectItemCaseSensitive(civic_addressJSON, "A5");

    if (a5) {
        if (!cJSON_IsString(a5)) {
            ogs_error("OpenAPI_civic_address_parseFromJSON() failed [a5]");
            goto end;
        }
    }

    cJSON *a6 = cJSON_GetObjectItemCaseSensitive(civic_addressJSON, "A6");

    if (a6) {
        if (!cJSON_IsString(a6)) {
            ogs_error("OpenAPI_civic_address_parseFromJSON() failed [a6]");
            goto end;
        }
    }

    cJSON *prd = cJSON_GetObjectItemCaseSensitive(civic_addressJSON, "PRD");

    if (prd) {
        if (!cJSON_IsString(prd)) {
            ogs_error("OpenAPI_civic_address_parseFromJSON() failed [prd]");
            goto end;
        }
    }

    cJSON *pod = cJSON_GetObjectItemCaseSensitive(civic_addressJSON, "POD");

    if (pod) {
        if (!cJSON_IsString(pod)) {
            ogs_error("OpenAPI_civic_address_parseFromJSON() failed [pod]");
            goto end;
        }
    }

    cJSON *sts = cJSON_GetObjectItemCaseSensitive(civic_addressJSON, "STS");

    if (sts) {
        if (!cJSON_IsString(sts)) {
            ogs_error("OpenAPI_civic_address_parseFromJSON() failed [sts]");
            goto end;
        }
    }

    cJSON *hno = cJSON_GetObjectItemCaseSensitive(civic_addressJSON, "HNO");

    if (hno) {
        if (!cJSON_IsString(hno)) {
            ogs_error("OpenAPI_civic_address_parseFromJSON() failed [hno]");
            goto end;
        }
    }

    cJSON *hns = cJSON_GetObjectItemCaseSensitive(civic_addressJSON, "HNS");

    if (hns) {
        if (!cJSON_IsString(hns)) {
            ogs_error("OpenAPI_civic_address_parseFromJSON() failed [hns]");
            goto end;
        }
    }

    cJSON *lmk = cJSON_GetObjectItemCaseSensitive(civic_addressJSON, "LMK");

    if (lmk) {
        if (!cJSON_IsString(lmk)) {
            ogs_error("OpenAPI_civic_address_parseFromJSON() failed [lmk]");
            goto end;
        }
    }

    cJSON *loc = cJSON_GetObjectItemCaseSensitive(civic_addressJSON, "LOC");

    if (loc) {
        if (!cJSON_IsString(loc)) {
            ogs_error("OpenAPI_civic_address_parseFromJSON() failed [loc]");
            goto end;
        }
    }

    cJSON *nam = cJSON_GetObjectItemCaseSensitive(civic_addressJSON, "NAM");

    if (nam) {
        if (!cJSON_IsString(nam)) {
            ogs_error("OpenAPI_civic_address_parseFromJSON() failed [nam]");
            goto end;
        }
    }

    cJSON *pc = cJSON_GetObjectItemCaseSensitive(civic_addressJSON, "PC");

    if (pc) {
        if (!cJSON_IsString(pc)) {
            ogs_error("OpenAPI_civic_address_parseFromJSON() failed [pc]");
            goto end;
        }
    }

    cJSON *bld = cJSON_GetObjectItemCaseSensitive(civic_addressJSON, "BLD");

    if (bld) {
        if (!cJSON_IsString(bld)) {
            ogs_error("OpenAPI_civic_address_parseFromJSON() failed [bld]");
            goto end;
        }
    }

    cJSON *unit = cJSON_GetObjectItemCaseSensitive(civic_addressJSON, "UNIT");

    if (unit) {
        if (!cJSON_IsString(unit)) {
            ogs_error("OpenAPI_civic_address_parseFromJSON() failed [unit]");
            goto end;
        }
    }

    cJSON *flr = cJSON_GetObjectItemCaseSensitive(civic_addressJSON, "FLR");

    if (flr) {
        if (!cJSON_IsString(flr)) {
            ogs_error("OpenAPI_civic_address_parseFromJSON() failed [flr]");
            goto end;
        }
    }

    cJSON *room = cJSON_GetObjectItemCaseSensitive(civic_addressJSON, "ROOM");

    if (room) {
        if (!cJSON_IsString(room)) {
            ogs_error("OpenAPI_civic_address_parseFromJSON() failed [room]");
            goto end;
        }
    }

    cJSON *plc = cJSON_GetObjectItemCaseSensitive(civic_addressJSON, "PLC");

    if (plc) {
        if (!cJSON_IsString(plc)) {
            ogs_error("OpenAPI_civic_address_parseFromJSON() failed [plc]");
            goto end;
        }
    }

    cJSON *pcn = cJSON_GetObjectItemCaseSensitive(civic_addressJSON, "PCN");

    if (pcn) {
        if (!cJSON_IsString(pcn)) {
            ogs_error("OpenAPI_civic_address_parseFromJSON() failed [pcn]");
            goto end;
        }
    }

    cJSON *pobox = cJSON_GetObjectItemCaseSensitive(civic_addressJSON, "POBOX");

    if (pobox) {
        if (!cJSON_IsString(pobox)) {
            ogs_error("OpenAPI_civic_address_parseFromJSON() failed [pobox]");
            goto end;
        }
    }

    cJSON *addcode = cJSON_GetObjectItemCaseSensitive(civic_addressJSON, "ADDCODE");

    if (addcode) {
        if (!cJSON_IsString(addcode)) {
            ogs_error("OpenAPI_civic_address_parseFromJSON() failed [addcode]");
            goto end;
        }
    }

    cJSON *seat = cJSON_GetObjectItemCaseSensitive(civic_addressJSON, "SEAT");

    if (seat) {
        if (!cJSON_IsString(seat)) {
            ogs_error("OpenAPI_civic_address_parseFromJSON() failed [seat]");
            goto end;
        }
    }

    cJSON *rd = cJSON_GetObjectItemCaseSensitive(civic_addressJSON, "RD");

    if (rd) {
        if (!cJSON_IsString(rd)) {
            ogs_error("OpenAPI_civic_address_parseFromJSON() failed [rd]");
            goto end;
        }
    }

    cJSON *rdsec = cJSON_GetObjectItemCaseSensitive(civic_addressJSON, "RDSEC");

    if (rdsec) {
        if (!cJSON_IsString(rdsec)) {
            ogs_error("OpenAPI_civic_address_parseFromJSON() failed [rdsec]");
            goto end;
        }
    }

    cJSON *rdbr = cJSON_GetObjectItemCaseSensitive(civic_addressJSON, "RDBR");

    if (rdbr) {
        if (!cJSON_IsString(rdbr)) {
            ogs_error("OpenAPI_civic_address_parseFromJSON() failed [rdbr]");
            goto end;
        }
    }

    cJSON *rdsubbr = cJSON_GetObjectItemCaseSensitive(civic_addressJSON, "RDSUBBR");

    if (rdsubbr) {
        if (!cJSON_IsString(rdsubbr)) {
            ogs_error("OpenAPI_civic_address_parseFromJSON() failed [rdsubbr]");
            goto end;
        }
    }

    cJSON *prm = cJSON_GetObjectItemCaseSensitive(civic_addressJSON, "PRM");

    if (prm) {
        if (!cJSON_IsString(prm)) {
            ogs_error("OpenAPI_civic_address_parseFromJSON() failed [prm]");
            goto end;
        }
    }

    cJSON *pom = cJSON_GetObjectItemCaseSensitive(civic_addressJSON, "POM");

    if (pom) {
        if (!cJSON_IsString(pom)) {
            ogs_error("OpenAPI_civic_address_parseFromJSON() failed [pom]");
            goto end;
        }
    }

    civic_address_local_var = OpenAPI_civic_address_create (
        country ? ogs_strdup(country->valuestring) : NULL,
        a1 ? ogs_strdup(a1->valuestring) : NULL,
        a2 ? ogs_strdup(a2->valuestring) : NULL,
        a3 ? ogs_strdup(a3->valuestring) : NULL,
        a4 ? ogs_strdup(a4->valuestring) : NULL,
        a5 ? ogs_strdup(a5->valuestring) : NULL,
        a6 ? ogs_strdup(a6->valuestring) : NULL,
        prd ? ogs_strdup(prd->valuestring) : NULL,
        pod ? ogs_strdup(pod->valuestring) : NULL,
        sts ? ogs_strdup(sts->valuestring) : NULL,
        hno ? ogs_strdup(hno->valuestring) : NULL,
        hns ? ogs_strdup(hns->valuestring) : NULL,
        lmk ? ogs_strdup(lmk->valuestring) : NULL,
        loc ? ogs_strdup(loc->valuestring) : NULL,
        nam ? ogs_strdup(nam->valuestring) : NULL,
        pc ? ogs_strdup(pc->valuestring) : NULL,
        bld ? ogs_strdup(bld->valuestring) : NULL,
        unit ? ogs_strdup(unit->valuestring) : NULL,
        flr ? ogs_strdup(flr->valuestring) : NULL,
        room ? ogs_strdup(room->valuestring) : NULL,
        plc ? ogs_strdup(plc->valuestring) : NULL,
        pcn ? ogs_strdup(pcn->valuestring) : NULL,
        pobox ? ogs_strdup(pobox->valuestring) : NULL,
        addcode ? ogs_strdup(addcode->valuestring) : NULL,
        seat ? ogs_strdup(seat->valuestring) : NULL,
        rd ? ogs_strdup(rd->valuestring) : NULL,
        rdsec ? ogs_strdup(rdsec->valuestring) : NULL,
        rdbr ? ogs_strdup(rdbr->valuestring) : NULL,
        rdsubbr ? ogs_strdup(rdsubbr->valuestring) : NULL,
        prm ? ogs_strdup(prm->valuestring) : NULL,
        pom ? ogs_strdup(pom->valuestring) : NULL
        );

    return civic_address_local_var;
end:
    return NULL;
}

OpenAPI_civic_address_t *OpenAPI_civic_address_copy(OpenAPI_civic_address_t *dst, OpenAPI_civic_address_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_civic_address_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_civic_address_convertToJSON() failed");
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

    OpenAPI_civic_address_free(dst);
    dst = OpenAPI_civic_address_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

