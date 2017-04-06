#define TRACE_MODULE _hss_context

#include "core_debug.h"
#include "core_pool.h"
#include "core_lib.h"

#include "hss_context.h"

#define OP "5F1D289C5D354D0A140C2548F5F3E3BA"
#define OPc "E8ED289DEBA952E4283B54E88E6183CA"
#define AMF "8000"

static hss_context_t self;

pool_declare(hss_ue_pool, hss_ue_t, MAX_NUM_OF_UE);

hss_context_t* hss_self()
{
    return &self;
}

status_t hss_context_init(void)
{
    char buf[HSS_KEY_LEN];
    hss_ue_t *ue;

    pool_init(&hss_ue_pool, MAX_NUM_OF_UE);

    memset(&self, 0, sizeof(hss_context_t));

    memcpy(self.op, CORE_HEX(OP, strlen(OP), buf), HSS_KEY_LEN);
    memcpy(self.amf, CORE_HEX(AMF, strlen(AMF), buf), HSS_AMF_LEN);

    #define K "465B5CE8B199B49FAA5F0A2EE238A6BC"
    #define UE1_IMSI "001010123456800"
    #define UE2_IMSI "001010123456796"

    #define UE3_IMSI "001010123456819"
    #define UE3_RAND "20080c3818183b52 2614162c07601d0d"

    ue = hss_ue_add();
    d_assert(ue, return -1, "UE context add failed");

    strcpy((char*)ue->imsi, UE1_IMSI);
    ue->imsi_len = strlen(UE1_IMSI);
    memcpy(ue->k, CORE_HEX(K, strlen(K), buf), HSS_KEY_LEN);
    core_generate_random_bytes(ue->rand, RAND_LEN);
    ue->sqn = 64;

    ue = hss_ue_add();
    d_assert(ue, return -1, "UE context add failed");

    strcpy((char*)ue->imsi, UE2_IMSI);
    ue->imsi_len = strlen(UE2_IMSI);
    memcpy(ue->k, CORE_HEX(K, strlen(K), buf), HSS_KEY_LEN);
    core_generate_random_bytes(ue->rand, RAND_LEN);
    ue->sqn = 64;

    ue = hss_ue_add();
    d_assert(ue, return -1, "UE context add failed");

    strcpy((char*)ue->imsi, UE3_IMSI);
    ue->imsi_len = strlen(UE3_IMSI);
    memcpy(ue->k, CORE_HEX(K, strlen(K), buf), HSS_KEY_LEN);
    memcpy(ue->rand, CORE_HEX(UE3_RAND, strlen(UE3_RAND), buf), 
            RAND_LEN);
    ue->sqn = 64;

	return CORE_OK;
}

void hss_context_final(void)
{
    hss_ue_remove_all();

    pool_final(&hss_ue_pool);
	
	return;
}

hss_ue_t* hss_ue_add()
{
    hss_ue_t *ue = NULL;

    /* Allocate new eNB context */
    pool_alloc_node(&hss_ue_pool, &ue);
    d_assert(ue, return NULL, "HSS-UE context allocation failed");

    /* Initialize eNB context */
    memset(ue, 0, sizeof(hss_ue_t));

    /* Add new eNB context to list */
    list_append(&self.ue_list, ue);

    memcpy(ue->op, self.op, HSS_KEY_LEN);
    memcpy(ue->amf, self.amf, HSS_AMF_LEN);
    
    return ue;
}

status_t hss_ue_remove(hss_ue_t *ue)
{
    d_assert(ue, return CORE_ERROR, "Null param");

    list_remove(&self.ue_list, ue);
    pool_free_node(&hss_ue_pool, ue);

    return CORE_OK;
}

status_t hss_ue_remove_all()
{
    hss_ue_t *ue = NULL, *next_ue = NULL;
    
    ue = list_first(&self.ue_list);
    while (ue)
    {
        next_ue = list_next(ue);

        hss_ue_remove(ue);

        ue = next_ue;
    }

    return CORE_OK;
}

hss_ue_t* hss_ue_find_by_imsi(c_uint8_t *imsi, c_uint8_t imsi_len)
{
    hss_ue_t *ue = NULL;
    
    ue = list_first(&self.ue_list);
    while (ue)
    {
        if (memcmp(ue->imsi, imsi, imsi_len) == 0)
            break;

        ue = list_next(ue);
    }

    return ue;
}

hss_ue_t* hss_ue_first()
{
    return list_first(&self.ue_list);
}

hss_ue_t* hss_ue_next(hss_ue_t *ue)
{
    return list_next(ue);
}
