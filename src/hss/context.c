#define TRACE_MODULE _hss_ctx

#include "core_debug.h"
#include "core_pool.h"
#include "core_lib.h"

#include "context.h"

#define OP "5F1D289C5D354D0A140C2548F5F3E3BA"
#define OPc "E8ED289DEBA952E4283B54E88E6183CA"
#define AMF "8000"

static hss_ctx_t self;

pool_declare(ue_pool, ue_ctx_t, SIZE_OF_UE_POOL);

static list_t g_ue_list;

hss_ctx_t* hss_self()
{
    return &self;
}

status_t hss_ctx_init(void)
{
    char buf[HSS_KEY_LEN];
    ue_ctx_t *ue;

    pool_init(&ue_pool, SIZE_OF_UE_POOL);

    memset(&self, 0, sizeof(hss_ctx_t));

    memcpy(self.op, core_ascii_to_hex(
                OP, strlen(OP), buf, HSS_KEY_LEN), HSS_KEY_LEN);
    memcpy(self.amf, core_ascii_to_hex(
                AMF, strlen(AMF), buf, HSS_AMF_LEN), HSS_AMF_LEN);

    #define K "465B5CE8B199B49FAA5F0A2EE238A6BC"
    #define UE1_IMSI "001010123456800"
    #define UE2_IMSI "001010123456796"

    #define UE3_IMSI "001010123456819"
    #define UE3_RAND "20080c3818183b52 2614162c07601d0d"

    ue = hss_ctx_ue_add();
    d_assert(ue, return -1, "UE context add failed");

    strcpy((char*)ue->imsi, UE1_IMSI);
    ue->imsi_len = strlen(UE1_IMSI);
    memcpy(ue->k, core_ascii_to_hex(
                K, strlen(K), buf, HSS_KEY_LEN), HSS_KEY_LEN);
    core_generate_random_bytes(ue->rand, RAND_LEN);
    ue->sqn = 64;

    ue = hss_ctx_ue_add();
    d_assert(ue, return -1, "UE context add failed");

    strcpy((char*)ue->imsi, UE2_IMSI);
    ue->imsi_len = strlen(UE2_IMSI);
    memcpy(ue->k, core_ascii_to_hex(
                K, strlen(K), buf, HSS_KEY_LEN), HSS_KEY_LEN);
    core_generate_random_bytes(ue->rand, RAND_LEN);
    ue->sqn = 64;

    ue = hss_ctx_ue_add();
    d_assert(ue, return -1, "UE context add failed");

    strcpy((char*)ue->imsi, UE3_IMSI);
    ue->imsi_len = strlen(UE3_IMSI);
    memcpy(ue->k, core_ascii_to_hex(
                K, strlen(K), buf, HSS_KEY_LEN), HSS_KEY_LEN);
    memcpy(ue->rand, core_ascii_to_hex(
                UE3_RAND, strlen(UE3_RAND), buf, RAND_LEN), RAND_LEN);
    ue->sqn = 64;

	return CORE_OK;
}

void hss_ctx_final(void)
{
    hss_ctx_ue_remove_all();

    pool_final(&ue_pool);
	
	return;
}

ue_ctx_t* hss_ctx_ue_add()
{
    ue_ctx_t *ue = NULL;

    /* Allocate new eNB context */
    pool_alloc_node(&ue_pool, &ue);
    d_assert(ue, return NULL, "HSS-UE context allocation failed");

    /* Initialize eNB context */
    memset(ue, 0, sizeof(ue_ctx_t));

    /* Add new eNB context to list */
    list_append(&g_ue_list, ue);

    memcpy(ue->op, self.op, HSS_KEY_LEN);
    memcpy(ue->amf, self.amf, HSS_AMF_LEN);
    
    return ue;
}

status_t hss_ctx_ue_remove(ue_ctx_t *ue)
{
    d_assert(ue, return CORE_ERROR, "Null param");

    list_remove(&g_ue_list, ue);
    pool_free_node(&ue_pool, ue);

    return CORE_OK;
}

status_t hss_ctx_ue_remove_all()
{
    ue_ctx_t *ue = NULL, *next_ue = NULL;
    
    ue = list_first(&g_ue_list);
    while (ue)
    {
        next_ue = list_next(ue);

        hss_ctx_ue_remove(ue);

        ue = next_ue;
    }

    return CORE_OK;
}

ue_ctx_t* hss_ctx_ue_find_by_imsi(c_uint8_t *imsi, c_uint8_t imsi_len)
{
    ue_ctx_t *ue = NULL;
    
    ue = list_first(&g_ue_list);
    while (ue)
    {
        if (memcmp(ue->imsi, imsi, imsi_len) == 0)
            break;

        ue = list_next(ue);
    }

    return ue;
}

ue_ctx_t* hss_ctx_ue_first()
{
    return list_first(&g_ue_list);
}

ue_ctx_t* hss_ctx_ue_next(ue_ctx_t *ue)
{
    return list_next(ue);
}
