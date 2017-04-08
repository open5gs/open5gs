#define TRACE_MODULE _3gpp_common

#include "core_debug.h"
#include "3gpp_common.h"

#define PLMN_ID_DIGIT1(x) (((x) / 100) % 10)
#define PLMN_ID_DIGIT2(x) (((x) / 10) % 10)
#define PLMN_ID_DIGIT3(x) ((x) % 10)

c_uint16_t plmn_id_mcc(plmn_id_t *plmn_id)
{
    return plmn_id->mcc1 * 100 + plmn_id->mcc2 * 10 + plmn_id->mcc3;
}
c_uint16_t plmn_id_mnc(plmn_id_t *plmn_id)
{
    return plmn_id->mnc1 == 0xf ? plmn_id->mnc2 * 10 + plmn_id->mnc3 :
        plmn_id->mnc1 * 100 + plmn_id->mnc2 * 10 + plmn_id->mnc3;
}
c_uint16_t plmn_id_mnc_len(plmn_id_t *plmn_id)
{
    return plmn_id->mnc1 == 0xf ? 2 : 3;
}

void *plmn_id_build(plmn_id_t *plmn_id, 
        c_uint16_t mcc, c_uint16_t mnc, c_uint16_t mnc_len)
{
    plmn_id->mcc1 = PLMN_ID_DIGIT1(mcc);
    plmn_id->mcc2 = PLMN_ID_DIGIT2(mcc);
    plmn_id->mcc3 = PLMN_ID_DIGIT3(mcc);

    if (mnc_len == 2)
        plmn_id->mnc1 = 0xf;
    else
        plmn_id->mnc1 = PLMN_ID_DIGIT1(mnc);

    plmn_id->mnc2 = PLMN_ID_DIGIT2(mnc);
    plmn_id->mnc3 = PLMN_ID_DIGIT3(mnc);

    return plmn_id;
}

/* 8.13 Protocol Configuration Options (PCO) 
 * 10.5.6.3 Protocol configuration options in 3GPP TS 24.008 */
c_int16_t pco_parse(pco_t *pco, void *data, int data_len)
{
    pco_t *source = (pco_t *)data;
    c_int16_t size = 0;
    int i = 0;

    d_assert(pco, return -1, "Null param");
    d_assert(data, return -1, "Null param");
    d_assert(data_len, return -1, "Null param");

    memset(pco, 0, sizeof(pco_t));

    pco->ext = source->ext;
    pco->configuration_protocol = source->configuration_protocol;
    size++;

    while(size < data_len && i < MAX_NUM_OF_PROTOCOL_OR_CONTAINER_ID)
    {
        protocol_or_container_id_t *id = &pco->ids[i];
        d_assert(size + sizeof(id->id) <= data_len, 
                return -1, "decode error");
        memcpy(&id->id, data + size, sizeof(id->id));
        id->id = ntohs(id->id);
        size += sizeof(id->id);

        d_assert(size + sizeof(id->length) <= data_len, 
                return -1, "decode error");
        memcpy(&id->length, data + size, sizeof(id->length));
        size += sizeof(id->length);

        id->contents = data + size;
        size += id->length;

        i++;
    }
    pco->num_of_id = i;
    d_assert(size == data_len, return -1, "decode error");
    
    return size;
}
c_int16_t pco_build(void *data, int data_len, pco_t *pco)
{
    pco_t target;
    c_int16_t size = 0;
    int i = 0;

    d_assert(pco, return -1, "Null param");
    d_assert(data, return -1, "Null param");
    d_assert(data_len, return -1, "Null param");

    memcpy(&target, pco, sizeof(pco_t));

    d_assert(size + 1 <= data_len, return -1, "encode error");
    memcpy(data + size, &target, 1);
    size += 1;

    d_assert(target.num_of_id <= MAX_NUM_OF_PROTOCOL_OR_CONTAINER_ID, 
            return -1, "encode error");
    for (i = 0; i < target.num_of_id; i++)
    {
        protocol_or_container_id_t *id = &target.ids[i];

        d_assert(size + sizeof(id->id) <= data_len, 
                return -1, "encode error");
        id->id = htons(id->id);
        memcpy(data + size, &id->id, sizeof(id->id));
        size += sizeof(id->id);

        d_assert(size + sizeof(id->length) <= data_len, 
                return -1, "encode error");
        memcpy(data + size, &id->length, sizeof(id->length));
        size += sizeof(id->length);

        d_assert(size + id->length <= data_len, return -1, "encode error");
        memcpy(data + size, id->contents, id->length);
        size += id->length;
    }

    return size;
}

