#define TRACE_MODULE _mme_context

#include "core_debug.h"
#include "core_pool.h"
#include "core_lib.h"
#include <mongoc.h>

#include "gtp_path.h"
#include "s1ap_message.h"
#include "fd_lib.h"

#include "context.h"
#include "nas_conv.h"
#include "mme_context.h"
#include "mme_event.h"
#include "s1ap_path.h"

#define MAX_CELL_PER_ENB            8

static mme_context_t self;

pool_declare(mme_s1ap_pool, mme_s1ap_t, MAX_NUM_OF_S1AP_SERVER);
pool_declare(mme_sgw_pool, mme_sgw_t, MAX_NUM_OF_GTP_CLIENT);

index_declare(mme_enb_pool, mme_enb_t, MAX_NUM_OF_ENB);
index_declare(mme_ue_pool, mme_ue_t, MAX_POOL_OF_UE);
index_declare(enb_ue_pool, enb_ue_t, MAX_POOL_OF_UE);
index_declare(mme_sess_pool, mme_sess_t, MAX_POOL_OF_SESS);
index_declare(mme_bearer_pool, mme_bearer_t, MAX_POOL_OF_BEARER);

static int context_initialized = 0;

status_t mme_context_init()
{
    d_assert(context_initialized == 0, return CORE_ERROR,
            "MME context already has been context_initialized");

    /* Initialize MME context */
    memset(&self, 0, sizeof(mme_context_t));

    pool_init(&mme_s1ap_pool, MAX_NUM_OF_S1AP_SERVER);
    list_init(&self.s1ap_list);
    pool_init(&mme_sgw_pool, MAX_NUM_OF_GTP_CLIENT);
    list_init(&self.sgw_list);

    index_init(&mme_enb_pool, MAX_NUM_OF_ENB);
    index_init(&mme_ue_pool, MAX_POOL_OF_UE);
    index_init(&enb_ue_pool, MAX_POOL_OF_UE);
    index_init(&mme_sess_pool, MAX_POOL_OF_SESS);
    index_init(&mme_bearer_pool, MAX_POOL_OF_BEARER);

    self.enb_sock_hash = hash_make();
    self.enb_addr_hash = hash_make();
    self.enb_id_hash = hash_make();
    self.mme_ue_s1ap_id_hash = hash_make();
    self.imsi_ue_hash = hash_make();
    self.guti_ue_hash = hash_make();

    /* Timer value */
    self.t3413_value = 2; /* Paging retry Timer: 2 secs */

    context_initialized = 1;

    return CORE_OK;
}

status_t mme_context_final()
{
    d_assert(context_initialized == 1, return CORE_ERROR,
            "MME context already has been finalized");

    mme_s1ap_remove_all();
    
    mme_sgw_remove_all();
    mme_enb_remove_all();

    mme_ue_remove_all();

    d_assert(self.enb_sock_hash, , "Null param");
    hash_destroy(self.enb_sock_hash);
    d_assert(self.enb_addr_hash, , "Null param");
    hash_destroy(self.enb_addr_hash);
    d_assert(self.enb_id_hash, , "Null param");
    hash_destroy(self.enb_id_hash);

    d_assert(self.mme_ue_s1ap_id_hash, , "Null param");
    hash_destroy(self.mme_ue_s1ap_id_hash);
    d_assert(self.imsi_ue_hash, , "Null param");
    hash_destroy(self.imsi_ue_hash);
    d_assert(self.guti_ue_hash, , "Null param");
    hash_destroy(self.guti_ue_hash);

    index_final(&mme_bearer_pool);
    index_final(&mme_sess_pool);
    index_final(&mme_ue_pool);
    index_final(&enb_ue_pool);

    index_final(&mme_enb_pool);
    pool_final(&mme_sgw_pool);

    pool_final(&mme_s1ap_pool);

    context_initialized = 0;

    return CORE_OK;
}

mme_context_t* mme_self()
{
    return &self;
}

static status_t mme_context_prepare()
{
    self.relative_capacity = 0xff;

    self.gtpc_port = GTPV2_C_UDP_PORT;
    self.s5c_port = GTPV2_C_UDP_PORT;

    return CORE_OK;
}

static status_t mme_context_validation()
{
    if (self.fd_conf_path == NULL)
    {
        d_error("No MME.FD_CONF_PATH in '%s'",
                context_self()->config.path);
        return CORE_ERROR;
    }
    if (mme_s1ap_first() == NULL)
    {
        d_error("No MME.S1AP in '%s'",
                context_self()->config.path);
        return CORE_ERROR;
    }
    if (self.gtpc_addr == 0)
    {
        d_error("No MME.NEWORK.GTPC_IPV4 in '%s'",
                context_self()->config.path);
        return CORE_ERROR;
    }

    mme_sgw_t *sgw = mme_sgw_first();
    if (sgw == NULL)
    {
        d_error("No SGW.NEWORK in '%s'",
                context_self()->config.path);
        return CORE_ERROR;
    }
#if 0 /* ADDR */
    while(sgw)
    {
        if (sgw->addr == 0)
        {
            d_error("No SGW.NEWORK.GTPC_IPV4 in '%s'",
                    context_self()->config.path);
            return CORE_ERROR;
        }
        sgw = mme_sgw_next(sgw);
    }
#endif
    self.sgw = mme_sgw_first();

    if (self.max_num_of_served_gummei == 0)
    {
        d_error("No MME.GUMMEI in '%s'",
                context_self()->config.path);
        return CORE_ERROR;
    }

    if (self.served_gummei[0].num_of_plmn_id == 0)
    {
        d_error("No MME.GUMMEI.PLMN_ID in '%s'",
                context_self()->config.path);
        return CORE_ERROR;
    }

    if (self.served_gummei[0].num_of_mme_gid == 0)
    {
        d_error("No MME.GUMMEI.MME_GID in '%s'",
                context_self()->config.path);
        return CORE_ERROR;
    }

    if (self.served_gummei[0].num_of_mme_code == 0)
    {
        d_error("No MME.GUMMEI.MME_CODE in '%s'",
                context_self()->config.path);
        return CORE_ERROR;
    }

    if (self.max_num_of_served_tai == 0)
    {
        d_error("No MME.TAI(PLMN_ID.MCC.MNC|TAC) in '%s'",
                context_self()->config.path);
        return CORE_ERROR;
    }

    if (self.num_of_integrity_order == 0)
    {
        d_error("No MME.SECURITY.INTEGRITY_ORDER in '%s'",
                context_self()->config.path);
        return CORE_ERROR;
    }
    if (self.num_of_ciphering_order == 0)
    {
        d_error("No MME.SECURITY.CIPHERING_ORDER in '%s'",
                context_self()->config.path);
        return CORE_ERROR;
    }

    return CORE_OK;
}

status_t mme_context_parse_config()
{
    status_t rv;
    config_t *config = &context_self()->config;
    bson_iter_t iter;
    c_uint32_t length = 0;

    d_assert(config, return CORE_ERROR, );

    rv = mme_context_prepare();
    if (rv != CORE_OK) return rv;

    if (!bson_iter_init(&iter, config->bson))
    {
        d_error("bson_iter_init failed in this document");
        return CORE_ERROR;
    }

    while(bson_iter_next(&iter))
    {
        const char *key = bson_iter_key(&iter);
        if (!strcmp(key, "MME") && BSON_ITER_HOLDS_DOCUMENT(&iter))
        {
            bson_iter_t mme_iter;
            bson_iter_recurse(&iter, &mme_iter);
            while(bson_iter_next(&mme_iter))
            {
                const char *mme_key = bson_iter_key(&mme_iter);
                if (!strcmp(mme_key, "RELATIVE_CAPACITY") &&
                    BSON_ITER_HOLDS_INT32(&mme_iter))
                {
                    self.relative_capacity = bson_iter_int32(&mme_iter);
                }
                else if (!strcmp(mme_key, "FD_CONF_PATH") &&
                    BSON_ITER_HOLDS_UTF8(&mme_iter))
                {
                    self.fd_conf_path = bson_iter_utf8(&mme_iter, &length);
                }
                else if (!strcmp(mme_key, "S1AP"))
                {
                    int s1ap_index = 0;
                    bson_iter_t s1ap_array;

                    if (BSON_ITER_HOLDS_ARRAY(&mme_iter))
                    {
                        bson_iter_recurse(&mme_iter, &s1ap_array);
                        d_assert(bson_iter_next(&s1ap_array),
                                return CORE_ERROR,);
                    }
                    else if (BSON_ITER_HOLDS_DOCUMENT(&mme_iter))
                    {
                        memcpy(&s1ap_array, &mme_iter,
                                sizeof(s1ap_array));
                    }
                    else
                        d_assert(0, return CORE_ERROR,);

                    do 
                    {
                        bson_iter_t s1ap_iter;
                        const char *s1ap_index_key =
                            bson_iter_key(&s1ap_array);

                        int family = AF_UNSPEC;
                        const char *hostname = NULL;
                        c_uint16_t port = S1AP_SCTP_PORT;
                        mme_s1ap_t *s1ap = NULL;

                        d_assert(s1ap_index_key, return CORE_ERROR,);
                        if (BSON_ITER_HOLDS_ARRAY(&mme_iter))
                            s1ap_index = atoi(s1ap_index_key);
                        d_assert(s1ap_index < MAX_NUM_OF_S1AP_SERVER,
                                return CORE_ERROR,
                                "GTP NODE Overflow : %d", s1ap_index);

                        bson_iter_recurse(&s1ap_array, &s1ap_iter);
                        while(bson_iter_next(&s1ap_iter))
                        {
                            const char *s1ap_key =
                                bson_iter_key(&s1ap_iter);

                            if (!strcmp(s1ap_key, "FAMILY") &&
                                    BSON_ITER_HOLDS_UTF8(&s1ap_iter))
                            {
                                const char *v =
                                    bson_iter_utf8(&s1ap_iter, &length);
                                if (v)
                                {
                                    if (!strcmp(v, "AF_INET") ||
                                        !strcmp(v, "PF_INET"))
                                    {
                                        family = AF_INET;
                                    }
                                    else if (!strcmp(v, "AF_INET6") ||
                                        !strcmp(v, "PF_INET6"))
                                    {
                                        family = AF_INET6;
                                    }
                                    else
                                    {
                                        d_warn("Unknown family(%s)", v);
                                    }
                                }
                            }
                            else if (!strcmp(s1ap_key, "HOSTNAME") &&
                                    BSON_ITER_HOLDS_UTF8(&s1ap_iter))
                            {
                                hostname = bson_iter_utf8(&s1ap_iter, &length);
                            }
                            else if (!strcmp(s1ap_key, "PORT") &&
                                    BSON_ITER_HOLDS_INT32(&s1ap_iter))
                            {
                                port = bson_iter_int32(&s1ap_iter);
                            }
                        }

                        s1ap = mme_s1ap_add(family, hostname, port);
                        d_assert(s1ap, return CORE_ERROR,);

                    } while(
                        BSON_ITER_HOLDS_ARRAY(&mme_iter) &&
                        bson_iter_next(&s1ap_array));
                }
                else if (!strcmp(mme_key, "NETWORK"))
                {
                    bson_iter_t network_iter;

                    if (BSON_ITER_HOLDS_ARRAY(&mme_iter))
                    {
                        bson_iter_t array_iter;
                        bson_iter_recurse(&mme_iter, &array_iter);
                        if (bson_iter_next(&array_iter))
                            bson_iter_recurse(&array_iter, &network_iter);
                    }
                    else if (BSON_ITER_HOLDS_DOCUMENT(&mme_iter))
                    {
                        bson_iter_recurse(&mme_iter, &network_iter);
                    }
                    else
                        d_assert(0, return CORE_ERROR,);

                    while(bson_iter_next(&network_iter))
                    {
                        const char *network_key = bson_iter_key(&network_iter);
                        if (!strcmp(network_key, "GTPC_IPV4") &&
                            BSON_ITER_HOLDS_UTF8(&network_iter))
                        {
                            const char *v = 
                                    bson_iter_utf8(&network_iter, &length);
                            if (v) self.gtpc_addr = inet_addr(v);
                        }
                        else if (!strcmp(network_key, "GTPC_PORT") &&
                            BSON_ITER_HOLDS_INT32(&network_iter))
                        {
                            self.gtpc_port = bson_iter_int32(&network_iter);
                        }
                    }
                }
                else if (!strcmp(mme_key, "GUMMEI"))
                {
                    int gummei_index = 0;
                    bson_iter_t gummei_array;

                    if (BSON_ITER_HOLDS_ARRAY(&mme_iter))
                    {
                        bson_iter_recurse(&mme_iter, &gummei_array);
                        d_assert(bson_iter_next(&gummei_array),
                                return CORE_ERROR,);
                    }
                    else if (BSON_ITER_HOLDS_DOCUMENT(&mme_iter))
                    {
                        memcpy(&gummei_array, &mme_iter, sizeof(gummei_array));
                    }
                    else
                        d_assert(0, return CORE_ERROR,);

                    do 
                    {
                        served_gummei_t *gummei = NULL;
                        bson_iter_t gummei_iter;
                        const char *gummei_index_key =
                            bson_iter_key(&gummei_array);

                        d_assert(gummei_index_key, return CORE_ERROR,);
                        if (BSON_ITER_HOLDS_ARRAY(&mme_iter))
                            gummei_index = atoi(gummei_index_key);
                        d_assert(gummei_index < MAX_NUM_OF_SERVED_GUMMEI,
                                return CORE_ERROR,
                                "GUMMEI Overflow : %d", gummei_index);
                        gummei = &self.served_gummei[gummei_index];

                        bson_iter_recurse(&gummei_array, &gummei_iter);
                        while(bson_iter_next(&gummei_iter))
                        {
                            const char *gummei_key =
                                bson_iter_key(&gummei_iter);
                            if (!strcmp(gummei_key, "PLMN_ID"))
                            {
                                int plmn_id_index = 0;
                                bson_iter_t plmn_id_array;

                                if (BSON_ITER_HOLDS_ARRAY(&gummei_iter))
                                {
                                    bson_iter_recurse(&gummei_iter,
                                            &plmn_id_array);
                                    d_assert(bson_iter_next(&plmn_id_array),
                                            return CORE_ERROR,);
                                }
                                else if (BSON_ITER_HOLDS_DOCUMENT(&gummei_iter))
                                {
                                    memcpy(&plmn_id_array,
                                        &gummei_iter, sizeof(plmn_id_array));
                                }
                                else
                                    d_assert(0, return CORE_ERROR,);

                                do 
                                {
                                    bson_iter_t plmn_id_iter;
                                    const char *mcc = NULL, *mnc = NULL;
                                    const char *plmn_id_index_key =
                                        bson_iter_key(&plmn_id_array);

                                    d_assert(plmn_id_index_key,
                                            return CORE_ERROR, );
                                    if (BSON_ITER_HOLDS_ARRAY(&gummei_iter))
                                        plmn_id_index = atoi(plmn_id_index_key);
                                    d_assert(plmn_id_index < MAX_PLMN_ID,
                                        return CORE_ERROR,
                                        "PLMN_ID Overflow : %d", plmn_id_index);

                                    bson_iter_recurse(
                                            &plmn_id_array, &plmn_id_iter);
                                    while(bson_iter_next(&plmn_id_iter))
                                    {
                                        const char *plmn_id_key =
                                            bson_iter_key(&plmn_id_iter);

                                        if (!strcmp(plmn_id_key, "MCC") &&
                                            BSON_ITER_HOLDS_UTF8(&plmn_id_iter))
                                        {
                                            mcc = bson_iter_utf8(
                                                    &plmn_id_iter, &length);
                                        } else if (
                                            !strcmp(plmn_id_key, "MNC") &&
                                            BSON_ITER_HOLDS_UTF8(&plmn_id_iter))
                                        {
                                            mnc = bson_iter_utf8(
                                                    &plmn_id_iter, &length);
                                        }
                                    }
                                    if (mcc && mnc)
                                    {
                                        plmn_id_build(&gummei->
                                            plmn_id[gummei->num_of_plmn_id],
                                            atoi(mcc), 
                                            atoi(mnc), strlen(mnc));
                                        gummei->num_of_plmn_id++;
                                    }
                                } while(
                                    BSON_ITER_HOLDS_ARRAY(&gummei_iter) &&
                                    bson_iter_next(&plmn_id_array));
                            }
                            else if (!strcmp(gummei_key, "MME_GID"))
                            {
                                int mme_gid_index = 0;
                                bson_iter_t mme_gid_array;

                                if (BSON_ITER_HOLDS_ARRAY(&gummei_iter))
                                {
                                    bson_iter_recurse(&gummei_iter,
                                            &mme_gid_array);
                                    d_assert(bson_iter_next(&mme_gid_array),
                                            return CORE_ERROR,);
                                }
                                else if (BSON_ITER_HOLDS_INT32(&gummei_iter))
                                {
                                    memcpy(&mme_gid_array, &gummei_iter,
                                            sizeof(mme_gid_array));
                                }
                                else
                                    d_assert(0, return CORE_ERROR,);

                                do
                                {
                                    const char *mme_gid_index_key =
                                        bson_iter_key(&mme_gid_array);

                                    d_assert(mme_gid_index_key,
                                            return CORE_ERROR,);
                                    if (BSON_ITER_HOLDS_ARRAY(&gummei_iter))
                                        mme_gid_index = atoi(mme_gid_index_key);
                                    d_assert(mme_gid_index < GRP_PER_MME,
                                            return CORE_ERROR,
                                            "MME_GID Overflow : %d",
                                                mme_gid_index);
                                    gummei->mme_gid[mme_gid_index] = 
                                        bson_iter_int32(&mme_gid_array);
                                    gummei->num_of_mme_gid++;
                                } while(
                                    BSON_ITER_HOLDS_ARRAY(&gummei_iter) &&
                                    bson_iter_next(&mme_gid_array));
                            }
                            else if (!strcmp(gummei_key, "MME_CODE"))
                            {
                                int mme_code_index = 0;
                                bson_iter_t mme_code_array;

                                if (BSON_ITER_HOLDS_ARRAY(&gummei_iter))
                                {
                                    bson_iter_recurse(&gummei_iter,
                                            &mme_code_array);
                                    d_assert(bson_iter_next(&mme_code_array),
                                            return CORE_ERROR,);
                                }
                                else if (BSON_ITER_HOLDS_INT32(&gummei_iter))
                                {
                                    memcpy(&mme_code_array, &gummei_iter,
                                            sizeof(mme_code_array));
                                }
                                else
                                    d_assert(0, return CORE_ERROR,);

                                do
                                {
                                    const char *mme_code_index_key =
                                        bson_iter_key(&mme_code_array);

                                    d_assert(mme_code_index_key,
                                            return CORE_ERROR,);
                                    if (BSON_ITER_HOLDS_ARRAY(&gummei_iter))
                                        mme_code_index = 
                                            atoi(mme_code_index_key);
                                    d_assert(mme_code_index < CODE_PER_MME,
                                            return CORE_ERROR,
                                            "MME_CODE Overflow : %d",
                                                mme_code_index);
                                    gummei->mme_code[mme_code_index] = 
                                        bson_iter_int32(&mme_code_array);
                                    gummei->num_of_mme_code++;
                                } while(
                                    BSON_ITER_HOLDS_ARRAY(&gummei_iter) &&
                                    bson_iter_next(&mme_code_array));
                            }
                        }
                        self.max_num_of_served_gummei++;
                    } while(
                        BSON_ITER_HOLDS_ARRAY(&mme_iter) &&
                        bson_iter_next(&gummei_array));
                }
                else if (!strcmp(mme_key, "TAI"))
                {
                    int tai_index = 0;
                    bson_iter_t tai_array;

                    if (BSON_ITER_HOLDS_ARRAY(&mme_iter))
                    {
                        bson_iter_recurse(&mme_iter, &tai_array);
                        d_assert(bson_iter_next(&tai_array),
                                return CORE_ERROR,);
                    }
                    else if (BSON_ITER_HOLDS_DOCUMENT(&mme_iter))
                    {
                        memcpy(&tai_array, &mme_iter, sizeof(tai_array));
                    }
                    else
                        d_assert(0, return CORE_ERROR,);

                    do 
                    {
                        const char *mcc = NULL, *mnc = NULL;
                        c_uint16_t tac = 0;

                        bson_iter_t tai_iter;
                        const char *tai_index_key =
                            bson_iter_key(&tai_array);

                        d_assert(tai_index_key, return CORE_ERROR,);
                        if (BSON_ITER_HOLDS_ARRAY(&mme_iter))
                            tai_index = atoi(tai_index_key);
                        d_assert(tai_index < MAX_NUM_OF_SERVED_TAI,
                                return CORE_ERROR,
                                "TAI Overflow : %d", tai_index);

                        bson_iter_recurse(&tai_array, &tai_iter);
                        while(bson_iter_next(&tai_iter))
                        {
                            const char *tai_key =
                                bson_iter_key(&tai_iter);

                            if (!strcmp(tai_key, "PLMN_ID") &&
                                BSON_ITER_HOLDS_DOCUMENT(&tai_iter))
                            {
                                bson_iter_t plmn_id_iter;
                                bson_iter_recurse(&tai_iter, &plmn_id_iter);
                                while(bson_iter_next(&plmn_id_iter))
                                {
                                    const char *plmn_id_key =
                                        bson_iter_key(&plmn_id_iter);
                                    if (!strcmp(plmn_id_key, "MCC") &&
                                        BSON_ITER_HOLDS_UTF8(&plmn_id_iter))
                                    {
                                        mcc = bson_iter_utf8(
                                                &plmn_id_iter, &length);
                                    }
                                    else if (!strcmp(plmn_id_key, "MNC") &&
                                        BSON_ITER_HOLDS_UTF8(&plmn_id_iter))
                                    {
                                        mnc = bson_iter_utf8(
                                                &plmn_id_iter, &length);
                                    }
                                }
                            }
                            else if (!strcmp(tai_key, "TAC") &&
                                    BSON_ITER_HOLDS_INT32(&tai_iter))
                            {
                                tac = bson_iter_int32(&tai_iter);
                            }
                        }
                        if (mcc && mnc && tac)
                        {
                            tai_t *tai = &self.served_tai[
                                self.max_num_of_served_tai];
                           
                            plmn_id_build(&tai->plmn_id,
                                atoi(mcc), atoi(mnc), strlen(mnc));
                            tai->tac = tac;

                            self.max_num_of_served_tai++;
                        }
                    } while(
                        BSON_ITER_HOLDS_ARRAY(&mme_iter) &&
                        bson_iter_next(&tai_array));
                }
                else if (!strcmp(mme_key, "SECURITY") &&
                        BSON_ITER_HOLDS_DOCUMENT(&mme_iter))
                {
                    bson_iter_t security_iter;

                    bson_iter_recurse(&mme_iter, &security_iter);
                    while(bson_iter_next(&security_iter))
                    {
                        const char *security_key =
                            bson_iter_key(&security_iter);
                        if (!strcmp(security_key, "INTEGRITY_ORDER"))
                        {
                            int integrity_index = 0;
                            bson_iter_t integrity_array;

                            if (BSON_ITER_HOLDS_ARRAY(&security_iter))
                            {
                                bson_iter_recurse(&security_iter,
                                        &integrity_array);
                                d_assert(bson_iter_next(&integrity_array),
                                        return CORE_ERROR,);
                            }
                            else if (BSON_ITER_HOLDS_UTF8(&security_iter))
                            {
                                memcpy(&integrity_array, &security_iter,
                                        sizeof(integrity_array));
                            }
                            else
                                d_assert(0, return CORE_ERROR,);

                            do 
                            {
                                const char *integrity_index_key =
                                    bson_iter_key(&integrity_array);
                                const char *v =
                                    bson_iter_utf8(&integrity_array, &length);

                                d_assert(integrity_index_key,
                                        return CORE_ERROR,);
                                if (BSON_ITER_HOLDS_ARRAY(&security_iter))
                                    integrity_index = atoi(integrity_index_key);
                                d_assert(integrity_index < MAX_NUM_OF_ALGORITHM,
                                        return CORE_ERROR,
                                        "Integrity Overflow : %d",
                                            integrity_index);
                                if (v)
                                {
                                    if (strcmp(v, "EIA0") == 0)
                                    {
                                        self.integrity_order[integrity_index] = 
                                            NAS_SECURITY_ALGORITHMS_EIA0;
                                        self.num_of_integrity_order++;
                                    }
                                    else if (strcmp(v, "EIA1") == 0)
                                    {
                                        self.integrity_order[integrity_index] = 
                                            NAS_SECURITY_ALGORITHMS_128_EIA1;
                                        self.num_of_integrity_order++;
                                    }
                                    else if (strcmp(v, "EIA2") == 0)
                                    {
                                        self.integrity_order[integrity_index] = 
                                            NAS_SECURITY_ALGORITHMS_128_EIA2;
                                        self.num_of_integrity_order++;
                                    }
                                    else if (strcmp(v, "EIA3") == 0)
                                    {
                                        self.integrity_order[integrity_index] = 
                                            NAS_SECURITY_ALGORITHMS_128_EIA3;
                                        self.num_of_integrity_order++;
                                    }
                                }
                            } while(
                                BSON_ITER_HOLDS_ARRAY(&security_iter) &&
                                bson_iter_next(&integrity_array));
                        }
                        else if (!strcmp(security_key, "CIPHERING_ORDER"))
                        {
                            int ciphering_index = 0;
                            bson_iter_t ciphering_array;

                            if (BSON_ITER_HOLDS_ARRAY(&security_iter))
                            {
                                bson_iter_recurse(&security_iter,
                                        &ciphering_array);
                                d_assert(bson_iter_next(&ciphering_array),
                                        return CORE_ERROR,);
                            }
                            else if (BSON_ITER_HOLDS_UTF8(&security_iter))
                            {
                                memcpy(&ciphering_array, &security_iter,
                                        sizeof(ciphering_array));
                            }
                            else
                                d_assert(0, return CORE_ERROR,);

                            do 
                            {
                                const char *ciphering_index_key =
                                    bson_iter_key(&ciphering_array);
                                const char *v =
                                    bson_iter_utf8(&ciphering_array, &length);

                                d_assert(ciphering_index_key,
                                        return CORE_ERROR,);
                                if (BSON_ITER_HOLDS_ARRAY(&security_iter))
                                    ciphering_index = atoi(ciphering_index_key);
                                d_assert(ciphering_index < MAX_NUM_OF_ALGORITHM,
                                        return CORE_ERROR,
                                        "Ciphering Overflow : %d",
                                            ciphering_index);
                                if (v)
                                {
                                    if (strcmp(v, "EEA0") == 0)
                                    {
                                        self.ciphering_order[ciphering_index] = 
                                            NAS_SECURITY_ALGORITHMS_EEA0;
                                        self.num_of_ciphering_order++;
                                    }
                                    else if (strcmp(v, "EEA1") == 0)
                                    {
                                        self.ciphering_order[ciphering_index] = 
                                            NAS_SECURITY_ALGORITHMS_128_EEA1;
                                        self.num_of_ciphering_order++;
                                    }
                                    else if (strcmp(v, "EEA2") == 0)
                                    {
                                        self.ciphering_order[ciphering_index] = 
                                            NAS_SECURITY_ALGORITHMS_128_EEA2;
                                        self.num_of_ciphering_order++;
                                    }
                                    else if (strcmp(v, "EEA3") == 0)
                                    {
                                        self.ciphering_order[ciphering_index] = 
                                            NAS_SECURITY_ALGORITHMS_128_EEA3;
                                        self.num_of_ciphering_order++;
                                    }
                                }
                            } while(
                                BSON_ITER_HOLDS_ARRAY(&security_iter) &&
                                bson_iter_next(&ciphering_array));
                        }
                    }
                }
            }
        }
        else if (!strcmp(key, "SGW") && BSON_ITER_HOLDS_DOCUMENT(&iter))
        {
            bson_iter_t sgw_iter;
            bson_iter_recurse(&iter, &sgw_iter);
            while(bson_iter_next(&sgw_iter))
            {
                const char *sgw_key = bson_iter_key(&sgw_iter);
                if (!strcmp(sgw_key, "NETWORK"))
                {
                    int network_index = 0;
                    bson_iter_t network_array;

                    if (BSON_ITER_HOLDS_ARRAY(&sgw_iter))
                    {
                        bson_iter_recurse(&sgw_iter, &network_array);
                        d_assert(bson_iter_next(&network_array),
                                return CORE_ERROR,);
                    }
                    else if (BSON_ITER_HOLDS_DOCUMENT(&sgw_iter))
                    {
                        memcpy(&network_array, &sgw_iter,
                                sizeof(network_array));
                    }
                    else
                        d_assert(0, return CORE_ERROR,);

                    do 
                    {
                        bson_iter_t network_iter;
                        const char *network_index_key =
                            bson_iter_key(&network_array);
                        const char *addr = NULL;
                        c_uint16_t port = GTPV2_C_UDP_PORT;

                        d_assert(network_index_key, return CORE_ERROR,);
                        if (BSON_ITER_HOLDS_ARRAY(&sgw_iter))
                            network_index = atoi(network_index_key);
                        d_assert(network_index < MAX_NUM_OF_GTP_CLIENT,
                                return CORE_ERROR,
                                "GTP NODE Overflow : %d", network_index);

                        bson_iter_recurse(&network_array, &network_iter);
                        while(bson_iter_next(&network_iter))
                        {
                            const char *network_key =
                                bson_iter_key(&network_iter);

                            if (!strcmp(network_key, "GTPC_IPV4") &&
                                    BSON_ITER_HOLDS_UTF8(&network_iter))
                            {
                                addr = bson_iter_utf8(&network_iter, &length);
                            }
                            else if (!strcmp(network_key, "GTPC_PORT") &&
                                    BSON_ITER_HOLDS_INT32(&network_iter))
                            {
                                port = bson_iter_int32(&network_iter);
                            }
                        }

                        if (addr && port)
                        {
                            mme_sgw_t *sgw = mme_sgw_add();
                            d_assert(sgw, return CORE_ERROR,);

                            core_inet_pton(AF_INET, addr, &sgw->addr);
                            sgw->addr.c_sa_port = htons(port);
                        }
                    } while(
                        BSON_ITER_HOLDS_ARRAY(&sgw_iter) &&
                        bson_iter_next(&network_array));
                }
            }
        }
        else if (!strcmp(key, "PGW") && BSON_ITER_HOLDS_DOCUMENT(&iter))
        {
            bson_iter_t pgw_iter;
            bson_iter_recurse(&iter, &pgw_iter);
            while(bson_iter_next(&pgw_iter))
            {
                const char *pgw_key = bson_iter_key(&pgw_iter);
                if (!strcmp(pgw_key, "NETWORK"))
                {
                    bson_iter_t network_iter;

                    if (BSON_ITER_HOLDS_ARRAY(&pgw_iter))
                    {
                        bson_iter_t array_iter;
                        bson_iter_recurse(&pgw_iter, &array_iter);
                        if (bson_iter_next(&array_iter))
                            bson_iter_recurse(&array_iter, &network_iter);
                    }
                    else if (BSON_ITER_HOLDS_DOCUMENT(&pgw_iter))
                    {
                        bson_iter_recurse(&pgw_iter, &network_iter);
                    }
                    else
                        d_assert(0, return CORE_ERROR,);

                    while(bson_iter_next(&network_iter))
                    {
                        const char *network_key = bson_iter_key(&network_iter);
                        if (!strcmp(network_key, "GTPC_IPV4") &&
                            BSON_ITER_HOLDS_UTF8(&network_iter))
                        {
                            const char *v = 
                                    bson_iter_utf8(&network_iter, &length);
                            if (v) self.s5c_addr = inet_addr(v);
                        }
                        else if (!strcmp(network_key, "GTPC_PORT") &&
                            BSON_ITER_HOLDS_INT32(&network_iter))
                        {
                            self.s5c_port = bson_iter_int32(&network_iter);
                        }
                    }
                }
            }
        }
    }

    rv = mme_context_validation();
    if (rv != CORE_OK) return rv;

    return CORE_OK;
}

status_t mme_context_setup_trace_module()
{
    int s1ap = context_self()->trace_level.s1ap;
    int nas = context_self()->trace_level.nas;
    int fd = context_self()->trace_level.fd;
    int gtp = context_self()->trace_level.gtp;
    int others = context_self()->trace_level.others;

    if (s1ap)
    {
        extern int _s1ap_sm;
        d_trace_level(&_s1ap_sm, s1ap);
        extern int _s1ap_build;
        d_trace_level(&_s1ap_build, s1ap);
        extern int _s1ap_handler;
        d_trace_level(&_s1ap_handler, s1ap);
        extern int _s1ap_sctp;
        d_trace_level(&_s1ap_sctp, s1ap);
        extern int _s1ap_path;
        d_trace_level(&_s1ap_path, s1ap);
        extern int _s1ap_recv;
        d_trace_level(&_s1ap_recv, s1ap);
        extern int _s1ap_send;
        d_trace_level(&_s1ap_send, s1ap);
        extern int _s1ap_ies_decoder;
        d_trace_level(&_s1ap_ies_decoder, s1ap);
        extern int _s1ap_ies_encoder;
        d_trace_level(&_s1ap_ies_encoder, s1ap);
    }

    if (nas)
    {
        extern int _emm_sm;
        d_trace_level(&_emm_sm, nas);
        extern int _esm_sm;
        d_trace_level(&_esm_sm, nas);
        extern int _emm_handler;
        d_trace_level(&_emm_handler, nas);
        extern int _esm_handler;
        d_trace_level(&_esm_handler, nas);
        extern int _nas_decoder;
        d_trace_level(&_nas_decoder, nas);
        extern int _nas_encoder;
        d_trace_level(&_nas_encoder, nas);
        extern int _nas_ies;
        d_trace_level(&_nas_ies, nas);
    }

    if (fd)
    {
        if (fd <= 1) fd_g_debug_lvl = FD_LOG_ERROR;
        else if (fd <= 3) fd_g_debug_lvl = FD_LOG_NOTICE;
        else if (fd <= 5) fd_g_debug_lvl = FD_LOG_DEBUG;
        else fd_g_debug_lvl = FD_LOG_ANNOYING;

        extern int _mme_fd_path;
        d_trace_level(&_mme_fd_path, fd);
        extern int _fd_init;
        d_trace_level(&_fd_init, fd);
        extern int _fd_logger;
        d_trace_level(&_fd_logger, fd);
    }

    if (gtp)
    {
        extern int _mme_s11_handler;
        d_trace_level(&_mme_s11_handler, gtp);
        extern int _gtp_path;
        d_trace_level(&_gtp_path, gtp);
        extern int _mme_s11_path;
        d_trace_level(&_mme_s11_path, gtp);
        extern int _tlv_msg;
        d_trace_level(&_tlv_msg, gtp);
        extern int _gtp_xact;
        d_trace_level(&_gtp_xact, gtp);
    }

    if (others)
    {
        extern int _mutex;
        d_trace_level(&_mutex, others);
        extern int _pkbuf;
        d_trace_level(&_pkbuf, others);

        extern int _context;
        d_trace_level(&_context, others);
        extern int _mme_context;
        d_trace_level(&_mme_context, others);
        extern int _mme_sm;
        d_trace_level(&_mme_sm, others);
    }

    return CORE_OK;
}

mme_s1ap_t* mme_s1ap_add(
        int family, const char *hostname, c_uint16_t port)
{
    mme_s1ap_t *s1ap = NULL;

    pool_alloc_node(&mme_s1ap_pool, &s1ap);
    d_assert(s1ap, return NULL, "Null param");
    memset(s1ap, 0, sizeof(mme_s1ap_t));

    s1ap->family = family;
    s1ap->hostname = hostname;
    s1ap->port = port;

    list_append(&self.s1ap_list, s1ap);
    
    return s1ap;
}

status_t mme_s1ap_remove(mme_s1ap_t *s1ap)
{
    d_assert(s1ap, return CORE_ERROR, "Null param");

    list_remove(&self.s1ap_list, s1ap);

    pool_free_node(&mme_s1ap_pool, s1ap);

    return CORE_OK;
}

status_t mme_s1ap_remove_all()
{
    mme_s1ap_t *s1ap = NULL, *next_s1ap = NULL;
    
    s1ap = mme_s1ap_first();
    while (s1ap)
    {
        next_s1ap = mme_s1ap_next(s1ap);

        mme_s1ap_remove(s1ap);

        s1ap = next_s1ap;
    }

    return CORE_OK;
}

mme_s1ap_t* mme_s1ap_first()
{
    return list_first(&self.s1ap_list);
}

mme_s1ap_t* mme_s1ap_next(mme_s1ap_t *s1ap)
{
    return list_next(s1ap);
}

mme_sgw_t* mme_sgw_add()
{
    mme_sgw_t *sgw = NULL;

    pool_alloc_node(&mme_sgw_pool, &sgw);
    d_assert(sgw, return NULL, "Null param");

    memset(sgw, 0, sizeof(mme_sgw_t));

    list_init(&sgw->local_list);
    list_init(&sgw->remote_list);

    list_append(&self.sgw_list, sgw);
    
    return sgw;
}

status_t mme_sgw_remove(mme_sgw_t *sgw)
{
    d_assert(sgw, return CORE_ERROR, "Null param");

    list_remove(&self.sgw_list, sgw);

    gtp_xact_delete_all(sgw);
    pool_free_node(&mme_sgw_pool, sgw);

    return CORE_OK;
}

status_t mme_sgw_remove_all()
{
    mme_sgw_t *sgw = NULL, *next_sgw = NULL;
    
    sgw = mme_sgw_first();
    while (sgw)
    {
        next_sgw = mme_sgw_next(sgw);

        mme_sgw_remove(sgw);

        sgw = next_sgw;
    }

    return CORE_OK;
}

mme_sgw_t* mme_sgw_first()
{
    return list_first(&self.sgw_list);
}

mme_sgw_t* mme_sgw_next(mme_sgw_t *sgw)
{
    return list_next(sgw);
}

mme_enb_t* mme_enb_add(sock_id sock, c_sockaddr_t *addr)
{
    mme_enb_t *enb = NULL;
    event_t e;

    d_assert(sock, return NULL,);
    d_assert(addr, return NULL,);

    index_alloc(&mme_enb_pool, &enb);
    d_assert(enb, return NULL, "Null param");

    enb->sock = sock;
    enb->addr = addr;
    enb->sock_type = mme_enb_sock_type(enb->sock);

    list_init(&enb->enb_ue_list);

    hash_set(self.enb_sock_hash, &enb->sock, sizeof(enb->sock), enb);
    hash_set(self.enb_addr_hash, enb->addr, sizeof(c_sockaddr_t), enb);
    
    event_set_param1(&e, (c_uintptr_t)enb->index);
    fsm_create(&enb->sm, s1ap_state_initial, s1ap_state_final);
    fsm_init(&enb->sm, &e);

    return enb;
}

status_t mme_enb_remove(mme_enb_t *enb)
{
    event_t e;

    d_assert(enb, return CORE_ERROR, "Null param");
    d_assert(enb->sock, return CORE_ERROR, "Null param");

    event_set_param1(&e, (c_uintptr_t)enb->index);
    fsm_final(&enb->sm, &e);
    fsm_clear(&enb->sm);

    hash_set(self.enb_sock_hash, &enb->sock, sizeof(enb->sock), NULL);
    hash_set(self.enb_addr_hash, enb->addr, sizeof(c_sockaddr_t), NULL);
    if (enb->enb_id)
        hash_set(self.enb_id_hash, &enb->enb_id, sizeof(enb->enb_id), NULL);

    enb_ue_remove_in_enb(enb);

    if (enb->sock_type == SOCK_STREAM)
        s1ap_delete(enb->sock);
    core_free(enb->addr);

    index_free(&mme_enb_pool, enb);

    return CORE_OK;
}

status_t mme_enb_remove_all()
{
    hash_index_t *hi = NULL;
    mme_enb_t *enb = NULL;

    for (hi = mme_enb_first(); hi; hi = mme_enb_next(hi))
    {
        enb = mme_enb_this(hi);
        mme_enb_remove(enb);
    }

    return CORE_OK;
}

mme_enb_t* mme_enb_find(index_t index)
{
    d_assert(index, return NULL, "Invalid Index");
    return index_find(&mme_enb_pool, index);
}

mme_enb_t* mme_enb_find_by_sock(sock_id sock)
{
    d_assert(sock, return NULL,"Invalid param");
    return (mme_enb_t *)hash_get(self.enb_sock_hash, &sock, sizeof(sock));

    return NULL;
}

mme_enb_t* mme_enb_find_by_addr(c_sockaddr_t *addr)
{
    d_assert(addr, return NULL,"Invalid param");
    return (mme_enb_t *)hash_get(self.enb_addr_hash,
            addr, sizeof(c_sockaddr_t));

    return NULL;
}

mme_enb_t* mme_enb_find_by_enb_id(c_uint32_t enb_id)
{
    d_assert(enb_id, return NULL,"Invalid param");
    return (mme_enb_t *)hash_get(self.enb_id_hash, &enb_id, sizeof(enb_id));
}

status_t mme_enb_set_enb_id(mme_enb_t *enb, c_uint32_t enb_id)
{
    d_assert(enb, return CORE_ERROR, "Invalid param");
    d_assert(enb_id, return CORE_ERROR, "Invalid param");

    enb->enb_id = enb_id;
    hash_set(self.enb_id_hash, &enb->enb_id, sizeof(enb->enb_id), enb);

    return CORE_OK;
}

hash_index_t* mme_enb_first()
{
    d_assert(self.enb_sock_hash, return NULL, "Null param");
    return hash_first(self.enb_sock_hash);
}

hash_index_t* mme_enb_next(hash_index_t *hi)
{
    return hash_next(hi);
}

mme_enb_t *mme_enb_this(hash_index_t *hi)
{
    d_assert(hi, return NULL, "Null param");
    return hash_this_val(hi);
}

int mme_enb_sock_type(sock_id sock)
{
    mme_s1ap_t *s1ap = NULL;

    d_assert(sock, return 0,);

    for (s1ap = mme_s1ap_first(); s1ap; s1ap = mme_s1ap_next(s1ap))
    {
        if (s1ap->sock == sock) return SOCK_SEQPACKET;
    }

    return SOCK_STREAM;
}


/** enb_ue_context handling function */
enb_ue_t* enb_ue_add(mme_enb_t *enb)
{
    enb_ue_t *enb_ue = NULL;

    d_assert(self.mme_ue_s1ap_id_hash, return NULL, "Null param");
    d_assert(enb, return NULL, "Null param");

    index_alloc(&enb_ue_pool, &enb_ue);
    d_assert(enb_ue, return NULL, "Null param");

    enb_ue->mme_ue_s1ap_id = NEXT_ID(self.mme_ue_s1ap_id, 1, 0xffffffff);
    enb_ue->enb = enb;

    hash_set(self.mme_ue_s1ap_id_hash, &enb_ue->mme_ue_s1ap_id, 
            sizeof(enb_ue->mme_ue_s1ap_id), enb_ue);
    list_append(&enb->enb_ue_list, enb_ue);

    return enb_ue;

}

unsigned int enb_ue_count()
{
    d_assert(self.mme_ue_s1ap_id_hash, return 0, "Null param");
    return hash_count(self.mme_ue_s1ap_id_hash);
}

status_t enb_ue_remove(enb_ue_t *enb_ue)
{
    status_t rv;
    d_assert(self.mme_ue_s1ap_id_hash, return CORE_ERROR, "Null param");
    d_assert(enb_ue, return CORE_ERROR, "Null param");
    d_assert(enb_ue->enb, return CORE_ERROR, "Null param");

    list_remove(&enb_ue->enb->enb_ue_list, enb_ue);
    hash_set(self.mme_ue_s1ap_id_hash, &enb_ue->mme_ue_s1ap_id, 
            sizeof(enb_ue->mme_ue_s1ap_id), NULL);

    rv = source_ue_deassociate_target_ue(enb_ue);
    d_assert(rv == CORE_OK, return CORE_ERROR, "Null param");

    index_free(&enb_ue_pool, enb_ue);

    return CORE_OK;
}

status_t enb_ue_remove_in_enb(mme_enb_t *enb)
{
    enb_ue_t *enb_ue = NULL, *next_enb_ue = NULL;
    
    enb_ue = enb_ue_first_in_enb(enb);
    while (enb_ue)
    {
        next_enb_ue = enb_ue_next_in_enb(enb_ue);

        enb_ue_remove(enb_ue);

        enb_ue = next_enb_ue;
    }

    return CORE_OK;
}

status_t enb_ue_switch_to_enb(enb_ue_t *enb_ue, mme_enb_t *new_enb)
{
    d_assert(enb_ue, return CORE_ERROR, "Null param");
    d_assert(enb_ue->enb, return CORE_ERROR, "Null param");
    d_assert(new_enb, return CORE_ERROR, "Null param");

    /* Remove from the old enb */
    list_remove(&enb_ue->enb->enb_ue_list, enb_ue);

    /* Add to the new enb */
    list_append(&new_enb->enb_ue_list, enb_ue);

    /* Switch to enb */
    enb_ue->enb = new_enb;

    return CORE_OK;
}

enb_ue_t* enb_ue_find(index_t index)
{
    d_assert(index, return NULL, "Invalid Index");
    return index_find(&enb_ue_pool, index);
}

enb_ue_t* enb_ue_find_by_enb_ue_s1ap_id(
        mme_enb_t *enb, c_uint32_t enb_ue_s1ap_id)
{
    enb_ue_t *enb_ue = NULL;
    
    enb_ue = enb_ue_first_in_enb(enb);
    while (enb_ue)
    {
        if (enb_ue_s1ap_id == enb_ue->enb_ue_s1ap_id)
            break;

        enb_ue = enb_ue_next_in_enb(enb_ue);
    }

    return enb_ue;
}

enb_ue_t* enb_ue_find_by_mme_ue_s1ap_id(c_uint32_t mme_ue_s1ap_id)
{
    d_assert(self.mme_ue_s1ap_id_hash, return NULL, "Null param");
    return hash_get(self.mme_ue_s1ap_id_hash, 
            &mme_ue_s1ap_id, sizeof(mme_ue_s1ap_id));
}

enb_ue_t* enb_ue_first_in_enb(mme_enb_t *enb)
{
    return list_first(&enb->enb_ue_list);
}

enb_ue_t* enb_ue_next_in_enb(enb_ue_t *enb_ue)
{
    return list_next(enb_ue);
}

mme_ue_t* mme_ue_add(enb_ue_t *enb_ue)
{
    mme_ue_t *mme_ue = NULL;
    event_t e;

    d_assert(enb_ue, return NULL, "Null param");

    index_alloc(&mme_ue_pool, &mme_ue);
    d_assert(mme_ue, return NULL, "Null param");

    list_init(&mme_ue->sess_list);

    mme_ue->mme_s11_teid = mme_ue->index;
    mme_ue->mme_s11_addr = mme_self()->gtpc_addr;

    /* Create t3413 timer */
    mme_ue->t3413 = timer_create(&self.tm_service, MME_EVT_EMM_T3413,
            self.t3413_value * 1000);
    d_assert(mme_ue->t3413, return NULL, "Null param");
    timer_set_param1(mme_ue->t3413, mme_ue->index);

    event_set_param1(&e, (c_uintptr_t)mme_ue->index);
    fsm_create(&mme_ue->sm, emm_state_initial, emm_state_final);
    fsm_init(&mme_ue->sm, &e);

    CONNECT_SGW_GTP_NODE(mme_ue);
    
    return mme_ue;
}

status_t mme_ue_remove(mme_ue_t *mme_ue)
{
    event_t e;

    d_assert(mme_ue, return CORE_ERROR, "Null param");

    event_set_param1(&e, (c_uintptr_t)mme_ue->index);
    fsm_final(&mme_ue->sm, &e);
    fsm_clear(&mme_ue->sm);

    /* Clear hash table */
    if (mme_ue->guti.m_tmsi != 0)
        hash_set(self.guti_ue_hash, &mme_ue->guti, sizeof(guti_t), NULL);
    if (mme_ue->imsi_len != 0)
        hash_set(self.imsi_ue_hash, mme_ue->imsi, mme_ue->imsi_len, NULL);
    
    /* Clear the saved PDN Connectivity Request */
    NAS_CLEAR_DATA(&mme_ue->pdn_connectivity_request);

    /* Clear Paging info : t3413, last_paing_msg */
    CLEAR_PAGING_INFO(mme_ue);

    /* Free UeRadioCapability */
    if (mme_ue->radio_capa)
    {
        S1ap_UERadioCapability_t *radio_capa = 
            (S1ap_UERadioCapability_t *)mme_ue->radio_capa;

        if (radio_capa->buf)
            core_free(radio_capa->buf);
        core_free(mme_ue->radio_capa);
    }

    /* Clear Transparent Container */
    S1AP_CLEAR_DATA(&mme_ue->container);

    mme_sess_remove_all(mme_ue);
    mme_pdn_remove_all(mme_ue);

    mme_ue_deassociate_enb_ue(mme_ue->enb_ue);

    index_free(&mme_ue_pool, mme_ue);

    return CORE_OK;
}

status_t mme_ue_remove_all()
{
    hash_index_t *hi = NULL;
    mme_ue_t *mme_ue = NULL;

    for (hi = mme_ue_first(); hi; hi = mme_ue_next(hi))
    {
        mme_ue = mme_ue_this(hi);
        mme_ue_remove(mme_ue);
    }

    return CORE_OK;
}

mme_ue_t* mme_ue_find(index_t index)
{
    d_assert(index, return NULL, "Invalid Index");
    return index_find(&mme_ue_pool, index);
}

mme_ue_t* mme_ue_find_by_imsi_bcd(c_int8_t *imsi_bcd)
{
    c_uint8_t imsi[MAX_IMSI_LEN];
    int imsi_len = 0;

    d_assert(imsi_bcd, return NULL,"Invalid param");

    core_bcd_to_buffer(imsi_bcd, imsi, &imsi_len);

    return mme_ue_find_by_imsi(imsi, imsi_len);
}

mme_ue_t* mme_ue_find_by_imsi(c_uint8_t *imsi, int imsi_len)
{
    d_assert(imsi && imsi_len, return NULL,"Invalid param");

    return (mme_ue_t *)hash_get(self.imsi_ue_hash, imsi, imsi_len);
}

mme_ue_t* mme_ue_find_by_guti(guti_t *guti)
{
    d_assert(guti, return NULL,"Invalid param");

    return (mme_ue_t *)hash_get(self.guti_ue_hash, guti, sizeof(guti_t));
}

mme_ue_t* mme_ue_find_by_teid(c_uint32_t teid)
{
    return mme_ue_find(teid);
}

hash_index_t *mme_ue_first()
{
    d_assert(self.imsi_ue_hash, return NULL, "Null param");
    return hash_first(self.imsi_ue_hash);
}

hash_index_t *mme_ue_next(hash_index_t *hi)
{
    return hash_next(hi);
}

mme_ue_t *mme_ue_this(hash_index_t *hi)
{
    d_assert(hi, return NULL, "Null param");
    return hash_this_val(hi);
}

mme_ue_t* mme_ue_find_by_message(nas_message_t *message)
{
    mme_ue_t *mme_ue = NULL;

    switch(message->emm.h.message_type)
    {
        case NAS_ATTACH_REQUEST:
        {
            nas_attach_request_t *attach_request =
                &message->emm.attach_request;

            nas_eps_mobile_identity_t *eps_mobile_identity =
                            &attach_request->eps_mobile_identity;

            switch(eps_mobile_identity->imsi.type)
            {
                case NAS_EPS_MOBILE_IDENTITY_IMSI:
                {
                    c_int8_t imsi_bcd[MAX_IMSI_BCD_LEN+1];

                    nas_imsi_to_bcd(
                        &eps_mobile_identity->imsi, eps_mobile_identity->length,
                        imsi_bcd);


                    mme_ue = mme_ue_find_by_imsi_bcd(imsi_bcd);
                    if (mme_ue)
                    {
                        d_trace(3,"known UE by IMSI[%s]\n", imsi_bcd);
                    }
                    else
                    {
                        d_trace(3,"Unknown UE by IMSI[%s]\n", imsi_bcd);
                    }
                    break;
                }
                case NAS_EPS_MOBILE_IDENTITY_GUTI:
                {
                    nas_eps_mobile_identity_guti_t *nas_guti = NULL;
                    nas_guti = &eps_mobile_identity->guti;
                    guti_t guti;

                    guti.plmn_id = nas_guti->plmn_id;
                    guti.mme_gid = nas_guti->mme_gid;
                    guti.mme_code = nas_guti->mme_code;
                    guti.m_tmsi = nas_guti->m_tmsi;

                    mme_ue = mme_ue_find_by_guti(&guti);
                    if (mme_ue)
                    {
                        d_trace(3, "Known UE by GUTI[G:%d,C:%d,M_TMSI:0x%x]\n",
                                guti.mme_gid,
                                guti.mme_code,
                                guti.m_tmsi);
                    }
                    else
                    {
                        d_warn("Unknown UE by GUTI[G:%d,C:%d,M_TMSI:0x%x]",
                                guti.mme_gid,
                                guti.mme_code,
                                guti.m_tmsi);
                    }
                    break;
                }
                default:
                {
                    d_error("Uknown message imsi type =%d\n",
                            eps_mobile_identity->imsi.type);
                    break;
                }
            }
            break;
        }
        case NAS_DETACH_REQUEST:
        {
            /* TODO */
            break;
        }
        case NAS_TRACKING_AREA_UPDATE_REQUEST:
        {
            nas_tracking_area_update_request_t *tau_request =
                &message->emm.tracking_area_update_request;

            nas_eps_mobile_identity_t *eps_mobile_identity =
                            &tau_request->old_guti;

            switch(eps_mobile_identity->imsi.type)
            {
                case NAS_EPS_MOBILE_IDENTITY_GUTI:
                {
                    nas_eps_mobile_identity_guti_t *nas_guti = NULL;
                    nas_guti = &eps_mobile_identity->guti;
                    guti_t guti;

                    guti.plmn_id = nas_guti->plmn_id;
                    guti.mme_gid = nas_guti->mme_gid;
                    guti.mme_code = nas_guti->mme_code;
                    guti.m_tmsi = nas_guti->m_tmsi;

                    mme_ue = mme_ue_find_by_guti(&guti);
                    if (mme_ue)
                    {
                        d_trace(3, "Known UE by GUTI[G:%d,C:%d,M_TMSI:0x%x]\n",
                                guti.mme_gid,
                                guti.mme_code,
                                guti.m_tmsi);
                    }
                    else
                    {
                        d_warn("Unknown UE by GUTI[G:%d,C:%d,M_TMSI:0x%x]",
                                guti.mme_gid,
                                guti.mme_code,
                                guti.m_tmsi);
                    }
                    break;
                }
                default:
                {
                    d_error("Uknown message imsi type =%d\n",
                            eps_mobile_identity->imsi.type);
                    break;
                }
            }
            break;
        }
        default:
        {
            break;
        }
    }

    return mme_ue;
}

/* At this point, I'm not sure whether this function is exported or not */
static status_t mme_ue_new_guti(mme_ue_t *mme_ue)
{
    served_gummei_t *served_gummei = NULL;

    d_assert(mme_ue, return CORE_ERROR, "Invalid param");
    d_assert(mme_self()->max_num_of_served_gummei > 0,
            return CORE_ERROR, "Invalid param");

    served_gummei = &mme_self()->served_gummei[0];

    d_assert(served_gummei->num_of_plmn_id > 0,
            return CORE_ERROR, "Invalid param");
    d_assert(served_gummei->num_of_mme_gid > 0,
            return CORE_ERROR, "Invalid param");
    d_assert(served_gummei->num_of_mme_code > 0,
            return CORE_ERROR, "Invalid param");

    if (mme_ue->guti.m_tmsi != 0)
    {
        /* MME has a VALID GUIT
         * As such, we need to remove previous GUTI in hash table */
        hash_set(self.guti_ue_hash, &mme_ue->guti, sizeof(guti_t), NULL);
    }

    memset(&mme_ue->guti, 0, sizeof(guti_t));

    /* FIXME : How to generate GUTI?
     * At this point, I'll use first(0-index) Served GUMMEI in MME_CONTEXT */
    memcpy(&mme_ue->guti.plmn_id, &served_gummei->plmn_id[0], PLMN_ID_LEN);
    mme_ue->guti.mme_gid = served_gummei->mme_gid[0];
    mme_ue->guti.mme_code = served_gummei->mme_code[0];

    mme_ue->guti.m_tmsi = NEXT_ID(self.m_tmsi, 1, 0xffffffff);

    hash_set(self.guti_ue_hash, &mme_ue->guti, sizeof(guti_t), mme_ue);

    return CORE_OK;
}

status_t mme_ue_set_imsi(mme_ue_t *mme_ue, c_int8_t *imsi_bcd)
{
    d_assert(mme_ue && imsi_bcd, return CORE_ERROR, "Invalid param");

    core_cpystrn(mme_ue->imsi_bcd, imsi_bcd, MAX_IMSI_BCD_LEN+1);
    core_bcd_to_buffer(mme_ue->imsi_bcd, mme_ue->imsi, &mme_ue->imsi_len);

    hash_set(self.imsi_ue_hash, mme_ue->imsi, mme_ue->imsi_len, mme_ue);

    mme_ue_new_guti(mme_ue);

    return CORE_OK;
}

/* 
 * S1AP Initial UE-Message : S-TMSI
 * NAS ATTACH_REQUEST : IMSI, GUTI
 * NAS TAU_REQUEST : GUTI
 * S1AP Handover Notification
 */
status_t mme_ue_associate_enb_ue(mme_ue_t *mme_ue, enb_ue_t *enb_ue)
{
    d_assert(mme_ue, return CORE_ERROR, "Null param");
    d_assert(enb_ue, return CORE_ERROR, "Null param");

    mme_ue->enb_ue = enb_ue;
    enb_ue->mme_ue = mme_ue;

    return CORE_OK;
}

/* 
 * mme_ue_remove()
 *
 * Note : should not call in enb_ue_remove()
 *
 * When mme_ue is removed, enb_ue->mme_ue must be NULL.
 * However, when enb_ue is removed, mme_ue->enb_ue need not be NULL.
 * mme_ue->enb_ue will be updated again when enb_ue is added.
 */
status_t mme_ue_deassociate_enb_ue(enb_ue_t *enb_ue)
{
    mme_ue_t *mme_ue = NULL;

    d_assert(enb_ue, return CORE_ERROR, "Null param");
    mme_ue = enb_ue->mme_ue;
    d_assert(mme_ue, return CORE_ERROR, "Null param");

    mme_ue->enb_ue = NULL;
    enb_ue->mme_ue = NULL;

    return CORE_OK;
}

/* 
 * S1AP Handover Required
 */
status_t source_ue_associate_target_ue(
        enb_ue_t *source_ue, enb_ue_t *target_ue)
{
    mme_ue_t *mme_ue = NULL;

    d_assert(source_ue, return CORE_ERROR, "Null param");
    d_assert(target_ue, return CORE_ERROR, "Null param");
    mme_ue = source_ue->mme_ue;
    d_assert(mme_ue, return CORE_ERROR, "Null param");

    target_ue->mme_ue = mme_ue;
    target_ue->source_ue = source_ue;
    source_ue->target_ue = target_ue;

    return CORE_OK;
}

/* 
 * enb_ue_remove()
 *
 * enb_ue->mme_ue->enb_ue should not be set to NULL.
 * This is because enb_ue is not known as source_ue or target_ue.
 * Therefore, when enb_ue is removed, leave enb_ue->mme_ue->enb_ue as is.
 */
status_t source_ue_deassociate_target_ue(enb_ue_t *enb_ue)
{
    enb_ue_t *source_ue = NULL;
    enb_ue_t *target_ue = NULL;
    d_assert(enb_ue, return CORE_ERROR,);

    if (enb_ue->target_ue)
    {
        source_ue = enb_ue;
        target_ue = enb_ue->target_ue;

        d_assert(source_ue->target_ue, return CORE_ERROR,);
        d_assert(target_ue->source_ue, return CORE_ERROR,);
        source_ue->target_ue = NULL;
        target_ue->source_ue = NULL;
    }
    else if (enb_ue->source_ue)
    {
        target_ue = enb_ue;
        source_ue = enb_ue->source_ue;

        d_assert(source_ue->target_ue, return CORE_ERROR,);
        d_assert(target_ue->source_ue, return CORE_ERROR,);
        source_ue->target_ue = NULL;
        target_ue->source_ue = NULL;
    }

    return CORE_OK;
}

mme_sess_t *mme_sess_add(mme_ue_t *mme_ue, c_uint8_t pti)
{
    mme_sess_t *sess = NULL;
    mme_bearer_t *bearer = NULL;

    d_assert(mme_ue, return NULL, "Null param");
    d_assert(pti != NAS_PROCEDURE_TRANSACTION_IDENTITY_UNASSIGNED,
            return NULL, "Invalid PTI(%d)", pti);

    index_alloc(&mme_sess_pool, &sess);
    d_assert(sess, return NULL, "Null param");

    list_init(&sess->bearer_list);

    sess->mme_ue = mme_ue;
    sess->pti = pti;

    bearer = mme_bearer_add(sess);
    d_assert(bearer, mme_sess_remove(sess); return NULL, 
            "Can't add default bearer context");

    list_append(&mme_ue->sess_list, sess);

    return sess;
}

status_t mme_sess_remove(mme_sess_t *sess)
{
    d_assert(sess, return CORE_ERROR, "Null param");
    d_assert(sess->mme_ue, return CORE_ERROR, "Null param");
    
    list_remove(&sess->mme_ue->sess_list, sess);

    mme_bearer_remove_all(sess);

    NAS_CLEAR_DATA(&sess->ue_pco);
    TLV_CLEAR_DATA(&sess->pgw_pco);

    index_free(&mme_sess_pool, sess);

    return CORE_OK;
}

status_t mme_sess_remove_all(mme_ue_t *mme_ue)
{
    mme_sess_t *sess = NULL, *next_sess = NULL;
    
    sess = mme_sess_first(mme_ue);
    while (sess)
    {
        next_sess = mme_sess_next(sess);

        mme_sess_remove(sess);

        sess = next_sess;
    }

    return CORE_OK;
}

mme_sess_t* mme_sess_find(index_t index)
{
    d_assert(index, return NULL, "Invalid Index");
    return index_find(&mme_sess_pool, index);
}

mme_sess_t* mme_sess_find_by_pti(mme_ue_t *mme_ue, c_uint8_t pti)
{
    mme_sess_t *sess = NULL;

    sess = mme_sess_first(mme_ue);
    while(sess)
    {
        if (pti == sess->pti)
            return sess;

        sess = mme_sess_next(sess);
    }

    return NULL;
}

mme_sess_t* mme_sess_find_by_ebi(mme_ue_t *mme_ue, c_uint8_t ebi)
{
    mme_bearer_t *bearer = NULL;

    bearer = mme_bearer_find_by_ue_ebi(mme_ue, ebi);
    if (bearer)
        return bearer->sess;

    return NULL;
}

mme_sess_t* mme_sess_find_by_apn(mme_ue_t *mme_ue, c_int8_t *apn)
{
    mme_sess_t *sess = NULL;

    sess = mme_sess_first(mme_ue);
    while(sess)
    {
        if (sess->pdn && strcmp(sess->pdn->apn, apn) == 0)
            return sess;

        sess = mme_sess_next(sess);
    }

    return NULL;
}

mme_sess_t* mme_sess_first(mme_ue_t *mme_ue)
{
    return list_first(&mme_ue->sess_list);
}

mme_sess_t* mme_sess_next(mme_sess_t *sess)
{
    return list_next(sess);
}

mme_bearer_t* mme_bearer_add(mme_sess_t *sess)
{
    event_t e;

    mme_bearer_t *bearer = NULL;
    mme_ue_t *mme_ue = NULL;

    d_assert(sess, return NULL, "Null param");
    mme_ue = sess->mme_ue;
    d_assert(mme_ue, return NULL, "Null param");

    index_alloc(&mme_bearer_pool, &bearer);
    d_assert(bearer, return NULL, "Null param");

    bearer->ebi = NEXT_ID(mme_ue->ebi, MIN_EPS_BEARER_ID, MAX_EPS_BEARER_ID);

    bearer->mme_ue = mme_ue;
    bearer->sess = sess;

    list_append(&sess->bearer_list, bearer);
    
    event_set_param1(&e, (c_uintptr_t)bearer->index);
    fsm_create(&bearer->sm, esm_state_initial, esm_state_final);
    fsm_init(&bearer->sm, &e);

    return bearer;
}

status_t mme_bearer_remove(mme_bearer_t *bearer)
{
    event_t e;

    d_assert(bearer, return CORE_ERROR, "Null param");
    d_assert(bearer->sess, return CORE_ERROR, "Null param");

    event_set_param1(&e, (c_uintptr_t)bearer->index);
    fsm_final(&bearer->sm, &e);
    fsm_clear(&bearer->sm);

    list_remove(&bearer->sess->bearer_list, bearer);

    TLV_CLEAR_DATA(&bearer->tft);
    
    index_free(&mme_bearer_pool, bearer);

    return CORE_OK;
}

status_t mme_bearer_remove_all(mme_sess_t *sess)
{
    mme_bearer_t *bearer = NULL, *next_bearer = NULL;

    d_assert(sess, return CORE_ERROR, "Null param");
    
    bearer = mme_bearer_first(sess);
    while (bearer)
    {
        next_bearer = mme_bearer_next(bearer);

        mme_bearer_remove(bearer);

        bearer = next_bearer;
    }

    return CORE_OK;
}

mme_bearer_t* mme_bearer_find(index_t index)
{
    d_assert(index, return NULL, "Invalid Index");
    return index_find(&mme_bearer_pool, index);
}

mme_bearer_t* mme_bearer_find_by_sess_ebi(mme_sess_t *sess, c_uint8_t ebi)
{
    mme_bearer_t *bearer = NULL;

    bearer = mme_bearer_first(sess);
    while(bearer)
    {
        if (ebi == bearer->ebi)
            return bearer;

        bearer = mme_bearer_next(bearer);
    }

    return NULL;
}

mme_bearer_t* mme_bearer_find_by_ue_ebi(mme_ue_t *mme_ue, c_uint8_t ebi)
{
    mme_sess_t *sess = NULL;
    mme_bearer_t *bearer = NULL;
    
    sess = mme_sess_first(mme_ue);
    while (sess)
    {
        bearer = mme_bearer_find_by_sess_ebi(sess, ebi);
        if (bearer)
        {
            return bearer;
        }

        sess = mme_sess_next(sess);
    }

    return NULL;
}

mme_bearer_t* mme_bearer_find_or_add_by_message(
        mme_ue_t *mme_ue, nas_message_t *message)
{
    c_uint8_t pti = NAS_PROCEDURE_TRANSACTION_IDENTITY_UNASSIGNED;
    c_uint8_t ebi = NAS_EPS_BEARER_IDENTITY_UNASSIGNED;

    mme_bearer_t *bearer = NULL;
    mme_sess_t *sess = NULL;

    d_assert(mme_ue, return NULL,);
    d_assert(message, return NULL,);

    if (message->esm.h.message_type == NAS_PDN_DISCONNECT_REQUEST)
    {
        nas_pdn_disconnect_request_t *pdn_disconnect_request = 
            &message->esm.pdn_disconnect_request;
        nas_linked_eps_bearer_identity_t *linked_eps_bearer_identity =
            &pdn_disconnect_request->linked_eps_bearer_identity;

        bearer = mme_bearer_find_by_ue_ebi(mme_ue,
                linked_eps_bearer_identity->eps_bearer_identity);
        d_assert(bearer, return NULL,
                "Invalid pti(%d) and ebi(%d)\n", pti, ebi);
        sess = bearer->sess;
        d_assert(sess, return NULL, "Null param");
        sess->pti = pti;

        return bearer;
    }

    pti = message->esm.h.procedure_transaction_identity;
    ebi = message->esm.h.eps_bearer_identity;

    if (ebi != NAS_EPS_BEARER_IDENTITY_UNASSIGNED)
    {
        bearer = mme_bearer_find_by_ue_ebi(mme_ue, ebi);
        d_assert(bearer, return NULL,);
        return bearer;
    }
        
    d_assert(pti != NAS_PROCEDURE_TRANSACTION_IDENTITY_UNASSIGNED,
            return NULL, );

    if (message->esm.h.message_type == NAS_PDN_CONNECTIVITY_REQUEST)
    {
        nas_pdn_connectivity_request_t *pdn_connectivity_request =
            &message->esm.pdn_connectivity_request;
        if (pdn_connectivity_request->presencemask &
                NAS_PDN_CONNECTIVITY_REQUEST_ACCESS_POINT_NAME_PRESENT)
            sess = mme_sess_find_by_apn(mme_ue,
                    pdn_connectivity_request->access_point_name.apn);
        else
            sess = mme_sess_first(mme_ue);

        if (!sess)
            sess = mme_sess_add(mme_ue, pti);
        else
            sess->pti = pti;

        d_assert(sess, return NULL,);
    }
    else
    {
        sess = mme_sess_find_by_pti(mme_ue, pti);
        d_assert(sess, return NULL,);
    }

    bearer = mme_default_bearer_in_sess(sess);
    d_assert(bearer, return NULL,);
    return bearer;
}

mme_bearer_t* mme_default_bearer_in_sess(mme_sess_t *sess)
{
    return mme_bearer_first(sess);
}

mme_bearer_t* mme_linked_bearer(mme_bearer_t *bearer)
{
    mme_sess_t *sess = NULL;

    d_assert(bearer, return NULL, "Null param");
    sess = bearer->sess;
    d_assert(sess, return NULL, "Null param");

    return mme_default_bearer_in_sess(sess);
}

mme_bearer_t* mme_bearer_first(mme_sess_t *sess)
{
    d_assert(sess, return NULL, "Null param");

    return list_first(&sess->bearer_list);
}

mme_bearer_t* mme_bearer_next(mme_bearer_t *bearer)
{
    return list_next(bearer);
}

status_t mme_pdn_remove_all(mme_ue_t *mme_ue)
{
    s6a_subscription_data_t *subscription_data = NULL;

    d_assert(mme_ue, return CORE_ERROR, "Null param");
    subscription_data = &mme_ue->subscription_data;
    d_assert(subscription_data, return CORE_ERROR, "Null param");

    subscription_data->num_of_pdn = 0;
    
    return CORE_OK;
}

pdn_t* mme_pdn_find_by_apn(mme_ue_t *mme_ue, c_int8_t *apn)
{
    s6a_subscription_data_t *subscription_data = NULL;
    pdn_t *pdn = NULL;
    int i = 0;
    
    d_assert(mme_ue, return NULL, "Null param");
    subscription_data = &mme_ue->subscription_data;
    d_assert(subscription_data, return NULL, "Null param");

    for (i = 0; i < subscription_data->num_of_pdn; i++)
    {
        pdn = &subscription_data->pdn[i];
        if (strcmp(pdn->apn, apn) == 0)
            return pdn;
    }

    return NULL;
}
