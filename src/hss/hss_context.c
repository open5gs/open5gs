#define TRACE_MODULE _hss_context

#include "core_debug.h"
#include "core_pool.h"
#include "core_lib.h"

#include "hss_context.h"

#define HSS_MAX_NUM_OF_PROFILE  8

static hss_context_t self;

pool_declare(hss_profile_pool, hss_profile_t, HSS_MAX_NUM_OF_PROFILE);
pool_declare(hss_pdn_pool, pdn_t, MAX_NUM_OF_PDN);
pool_declare(hss_ue_pool, hss_ue_t, MAX_NUM_OF_UE);

hss_context_t* hss_self()
{
    return &self;
}

status_t hss_context_init(void)
{
    char buf[HSS_KEY_LEN];

    hss_profile_id_t profile_id = 1;
    hss_profile_t *profile;
    pdn_t *pdn;
    hss_ue_t *ue;
    c_int8_t apn[MAX_APN_LEN];

    memset(&self, 0, sizeof(hss_context_t));

    pool_init(&hss_profile_pool, HSS_MAX_NUM_OF_PROFILE);
    pool_init(&hss_pdn_pool, MAX_NUM_OF_PDN);
    pool_init(&hss_ue_pool, MAX_NUM_OF_UE);

    list_init(&self.profile_list);
    list_init(&self.pdn_list);
    list_init(&self.ue_list);

    /***********************************************
     * Profile DB */
    profile = hss_profile_add(profile_id);
    d_assert(profile, return -1, "Profile context add failed");

#define OP "5F1D289C5D354D0A140C2548F5F3E3BA"
#define AMF "8000"
#define OPc "E8ED289DEBA952E4283B54E88E6183CA"
#define K "465B5CE8B199B49FAA5F0A2EE238A6BC"
    memcpy(profile->op, CORE_HEX(OP, strlen(OP), buf), HSS_KEY_LEN);
    memcpy(profile->amf, CORE_HEX(AMF, strlen(AMF), buf), HSS_AMF_LEN);
    memcpy(profile->k, CORE_HEX(K, strlen(K), buf), HSS_KEY_LEN);
    profile->sqn = 64;

    profile->access_restriction_data = 0;
    profile->subscriber_status = HSS_SUBSCRIBER_STATUS_SERVICE_GRANTED;
    profile->network_access_mode = HSS_NETWORK_ACCESS_MODE_ONLY_PACKET;
    profile->max_bandwidth_ul = 102400; /* Kbps */
    profile->max_bandwidth_dl = 102400; /* Kbps */

    profile->subscribed_rau_tau_timer = 12; /* minutes */

    /***********************************************
     * PDN DB */
    apn[0] = 0x08;
    strcpy(apn+1, "internet");

    pdn = hss_pdn_add(apn);
    d_assert(pdn, return -1, "PDN context add failed");

    pdn->s6a_type = S6A_PDN_TYPE_IPV4;

    pdn->qci = PDN_QCI_9;
    pdn->priority_level = 8;

    pdn->pre_emption_capability = PDN_PRE_EMPTION_CAPABILITY_DISABLED;
    pdn->pre_emption_vulnerability = PDN_PRE_EMPTION_VULNERABILITY_DISABLED;

    pdn->max_bandwidth_ul = 102400; /* Kbps */
    pdn->max_bandwidth_dl = 102400; /* Kbps */

    /***********************************************
     * UE DB */
    #define UE1_IMSI "001010123456800"
    #define UE2_IMSI "001010123456796"

    #define UE3_IMSI "001010123456819"
    #define UE3_RAND "20080c3818183b52 2614162c07601d0d"

    ue = hss_ue_add(profile_id, UE1_IMSI);
    d_assert(ue, return -1, "UE context add failed");
    ue->pdn[0] = pdn;
    ue->num_of_pdn = 1;

    ue = hss_ue_add(profile_id, UE2_IMSI);
    d_assert(ue, return -1, "UE context add failed");
    ue->pdn[0] = pdn;
    ue->num_of_pdn = 1;

    ue = hss_ue_add(profile_id, UE3_IMSI);
    d_assert(ue, return -1, "UE context add failed");
    ue->pdn[0] = pdn;
    ue->num_of_pdn = 1;

    memcpy(ue->rand, CORE_HEX(UE3_RAND, strlen(UE3_RAND), buf), 
            RAND_LEN);

	return CORE_OK;
}

void hss_context_final(void)
{
    hss_ue_remove_all();
    hss_pdn_remove_all();
    hss_profile_remove_all();

    pool_final(&hss_ue_pool);
    pool_final(&hss_pdn_pool);
    pool_final(&hss_profile_pool);
	
	return;
}

pdn_t* hss_pdn_add(c_int8_t *apn)
{
    pdn_t *pdn = NULL;

    pool_alloc_node(&hss_pdn_pool, &pdn);
    d_assert(pdn, return NULL, "HSS-UE context allocation failed");

    memset(pdn, 0, sizeof(pdn_t));

    strcpy(pdn->apn, apn);
    pdn->id = NEXT_ID(self.pdn_id, 1, 0xffffffff);
    
    list_append(&self.pdn_list, pdn);

    return pdn;
}

status_t hss_pdn_remove(pdn_t *pdn)
{
    d_assert(pdn, return CORE_ERROR, "Null param");

    list_remove(&self.pdn_list, pdn);
    pool_free_node(&hss_pdn_pool, pdn);

    return CORE_OK;
}

status_t hss_pdn_remove_all()
{
    pdn_t *pdn = NULL, *next_pdn = NULL;
    
    pdn = list_first(&self.pdn_list);
    while (pdn)
    {
        next_pdn = list_next(pdn);

        hss_pdn_remove(pdn);

        pdn = next_pdn;
    }

    return CORE_OK;
}

pdn_t* hss_pdn_find_by_apn(c_int8_t *apn)
{
    pdn_t *pdn = NULL;
    
    pdn = list_first(&self.pdn_list);
    while (pdn)
    {
        if (strcmp(pdn->apn, apn) == 0)
            break;

        pdn = list_next(pdn);
    }

    return pdn;
}

pdn_t* hss_pdn_first()
{
    return list_first(&self.pdn_list);
}

pdn_t* hss_pdn_next(pdn_t *pdn)
{
    return list_next(pdn);
}

hss_profile_t* hss_profile_add(hss_profile_id_t id)
{
    hss_profile_t *profile = NULL;

    pool_alloc_node(&hss_profile_pool, &profile);
    d_assert(profile, return NULL, "HSS-UE context allocation failed");

    memset(profile, 0, sizeof(hss_profile_t));

    profile->id = id;
    
    list_append(&self.profile_list, profile);

    return profile;
}

status_t hss_profile_remove(hss_profile_t *profile)
{
    d_assert(profile, return CORE_ERROR, "Null param");

    list_remove(&self.profile_list, profile);
    pool_free_node(&hss_profile_pool, profile);

    return CORE_OK;
}

status_t hss_profile_remove_all()
{
    hss_profile_t *profile = NULL, *next_profile = NULL;
    
    profile = list_first(&self.profile_list);
    while (profile)
    {
        next_profile = list_next(profile);

        hss_profile_remove(profile);

        profile = next_profile;
    }

    return CORE_OK;
}

hss_profile_t* hss_profile_find_by_id(hss_profile_id_t id)
{
    hss_profile_t *profile = NULL;
    
    profile = list_first(&self.profile_list);
    while (profile)
    {
        if (profile->id == id)
            break;

        profile = list_next(profile);
    }

    return profile;
}

hss_profile_t* hss_profile_find_by_name(c_int8_t *name)
{
    hss_profile_t *profile = NULL;
    
    profile = list_first(&self.profile_list);
    while (profile)
    {
        if (strcmp(profile->name, name) == 0)
            break;

        profile = list_next(profile);
    }

    return profile;
}

hss_profile_t* hss_profile_first()
{
    return list_first(&self.profile_list);
}

hss_profile_t* hss_profile_next(hss_profile_t *profile)
{
    return list_next(profile);
}

hss_ue_t* hss_ue_add(hss_profile_id_t id, c_int8_t *imsi_bcd)
{
    hss_profile_t *profile = NULL;
    hss_ue_t *ue = NULL;

    profile = hss_profile_find_by_id(id);
    d_assert(profile, return NULL, "Can't find Profile = %d", id);

    pool_alloc_node(&hss_ue_pool, &ue);
    d_assert(ue, return NULL, "HSS-UE context allocation failed");

    memset(ue, 0, sizeof(hss_ue_t));

    memcpy(ue->k, profile->k, HSS_KEY_LEN);
    memcpy(ue->op, profile->op, HSS_KEY_LEN);
    memcpy(ue->amf, profile->amf, HSS_AMF_LEN);

    strcpy(ue->imsi_bcd, imsi_bcd);
    core_bcd_to_buffer(ue->imsi_bcd, ue->msisdn, &ue->msisdn_len);

    core_generate_random_bytes(ue->rand, RAND_LEN);
    ue->sqn = profile->sqn;

    ue->access_restriction_data = profile->access_restriction_data;
    ue->subscriber_status = profile->subscriber_status;
    ue->network_access_mode = profile->network_access_mode;
    ue->max_bandwidth_ul = profile->max_bandwidth_ul;
    ue->max_bandwidth_dl = profile->max_bandwidth_dl;

    ue->subscribed_rau_tau_timer = profile->subscribed_rau_tau_timer;
    
    list_append(&self.ue_list, ue);

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

hss_ue_t* hss_ue_find_by_imsi_bcd(c_int8_t *imsi_bcd)
{
    hss_ue_t *ue = NULL;
    
    ue = list_first(&self.ue_list);
    while (ue)
    {
        if (strcmp(ue->imsi_bcd, imsi_bcd) == 0)
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
