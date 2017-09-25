#include "core_tlv.h"
#include "core_tlv_msg.h"
#include "core_lib.h"

#include "core_debug.h"

#include "testutil.h"

#define TLV_0_LEN 10
#define TLV_1_LEN 20
#define TLV_2_LEN 100
#define TLV_3_LEN 200
#define TLV_4_LEN 2
#define TLV_5_LEN 2000
#define TLV_0_INSTANCE 0
#define TLV_1_INSTANCE 1
#define TLV_2_INSTANCE 2
#define TLV_3_INSTANCE 3
#define TLV_4_INSTANCE 4
#define TLV_5_INSTANCE 5
#define TLV_VALUE_ARRAY_SIZE    3000

#define EMBED_TLV_TYPE      20
#define EMBED_TLV_INSTANCE  7

typedef struct _test_tlv_element
{
    c_uint8_t type;
    c_uint32_t length;
    c_uint8_t instance;
    c_uint8_t *value;
    c_uint8_t val_char;
} test_tlv_element;

c_uint8_t test_tlv_value[TLV_VALUE_ARRAY_SIZE];

test_tlv_element tlv_element[] ={
    {1, TLV_0_LEN, TLV_0_INSTANCE, 0, 0x0a},
    {50, TLV_1_LEN, TLV_1_INSTANCE, 0, 0x0b},
    {255, TLV_2_LEN, TLV_2_INSTANCE, 0, 0x0c},
    {254, TLV_3_LEN, TLV_3_INSTANCE, 0, 0x0d},
    {5, TLV_4_LEN, TLV_4_INSTANCE, 0, 0x0e},
    {30, TLV_5_LEN, TLV_5_INSTANCE, 0, 0x0f}
};

void tlv_test_set_tlv_value(void)
{
    c_uint32_t inc = 0;

    /* set test tlv value */
    tlv_element[0].value = test_tlv_value;
    memset(tlv_element[0].value, tlv_element[0].val_char, tlv_element[0].length);
    inc += tlv_element[0].length;

    tlv_element[1].value = test_tlv_value + inc;
    memset(tlv_element[1].value,  tlv_element[1].val_char, tlv_element[1].length);
    inc += tlv_element[1].length;

    tlv_element[2].value = test_tlv_value + inc;
    memset(tlv_element[2].value,  tlv_element[2].val_char, tlv_element[2].length);
    inc += tlv_element[2].length;

    tlv_element[3].value = test_tlv_value + inc;
    memset(tlv_element[3].value,  tlv_element[3].val_char, tlv_element[3].length);
    inc += tlv_element[3].length;

    tlv_element[4].value = test_tlv_value + inc;
    memset(tlv_element[4].value,  tlv_element[4].val_char, tlv_element[4].length);
    inc += tlv_element[4].length;

    return;
}

void tlv_test_check_embed_tlv_test(abts_case *tc, tlv_t *root_tlv, int mode)
{
    c_uint32_t m;
    c_uint32_t parent_block_len;
    tlv_t *pTlv;
    tlv_t *embed_tlv = NULL, *parent_tlv = NULL, *parsed_tlv = NULL;
    c_uint8_t parent_block[4000];
    c_uint8_t *pos = NULL;
    int result;

    memset(parent_block, 0x00, sizeof(parent_block));

    parent_block_len = tlv_render(root_tlv, parent_block,
        sizeof(parent_block), mode);

    tlv_free_all(root_tlv);
    ABTS_INT_EQUAL(tc, tlv_pool_avail(), NUM_OF_TLV_NODE);

    pos = parent_block;

    switch(mode)
    {
        case TLV_MODE_T2_L2:
        {
            ABTS_INT_EQUAL(tc, 332, parent_block_len);
            ABTS_INT_EQUAL(tc, *(pos++), tlv_element[0].type >> 8);
            ABTS_INT_EQUAL(tc, *(pos++), tlv_element[0].type & 0xFF);
            ABTS_INT_EQUAL(tc, *(pos++), tlv_element[0].length >> 8);
            ABTS_INT_EQUAL(tc, *(pos++), tlv_element[0].length & 0xFF);
            for(m = 0; m < tlv_element[0].length; m++)
                ABTS_INT_EQUAL(tc, *(pos++), tlv_element[0].val_char);

            ABTS_INT_EQUAL(tc, *(pos++), EMBED_TLV_TYPE >> 8);
            ABTS_INT_EQUAL(tc, *(pos++), EMBED_TLV_TYPE & 0xFF);
            ABTS_INT_EQUAL(tc, *(pos++), (308 >> 8));
            ABTS_INT_EQUAL(tc, *(pos++), 308 & 0xFF);

            /* embedded tlv_t */
            ABTS_INT_EQUAL(tc, *(pos++), tlv_element[2].type >> 8);
            ABTS_INT_EQUAL(tc, *(pos++), tlv_element[2].type & 0xFF);    
            ABTS_INT_EQUAL(tc, *(pos++), tlv_element[2].length >> 8);
            ABTS_INT_EQUAL(tc, *(pos++), tlv_element[2].length & 0xFF);
            for(m = 0; m < tlv_element[2].length; m++)
                ABTS_INT_EQUAL(tc, *(pos++), tlv_element[2].val_char);


            ABTS_INT_EQUAL(tc, *(pos++), tlv_element[3].type >> 8);
            ABTS_INT_EQUAL(tc, *(pos++), tlv_element[3].type & 0xFF);    
            ABTS_INT_EQUAL(tc, *(pos++), tlv_element[3].length >> 8);
            ABTS_INT_EQUAL(tc, *(pos++), tlv_element[3].length & 0xFF);
            for(m = 0; m < tlv_element[3].length; m++)
                ABTS_INT_EQUAL(tc, *(pos++), tlv_element[3].val_char);


            ABTS_INT_EQUAL(tc, *(pos++), tlv_element[4].type >> 8);
            ABTS_INT_EQUAL(tc, *(pos++), tlv_element[4].type & 0xFF);
            ABTS_INT_EQUAL(tc, *(pos++), tlv_element[4].length >> 8);
            ABTS_INT_EQUAL(tc, *(pos++), tlv_element[4].length & 0xFF);

            for(m = 0; m < tlv_element[4].length; m++)
                ABTS_INT_EQUAL(tc, *(pos++), 0x0e);
            break;
        }
        case TLV_MODE_T1_L2:
        {
            ABTS_INT_EQUAL(tc, 327, parent_block_len);
            ABTS_INT_EQUAL(tc, *(pos++), tlv_element[0].type & 0xFF);
            ABTS_INT_EQUAL(tc, *(pos++), tlv_element[0].length >> 8);
            ABTS_INT_EQUAL(tc, *(pos++), tlv_element[0].length & 0xFF);
            for(m = 0; m < tlv_element[0].length; m++)
                ABTS_INT_EQUAL(tc, *(pos++), tlv_element[0].val_char);

            ABTS_INT_EQUAL(tc, *(pos++), EMBED_TLV_TYPE & 0xFF);
            ABTS_INT_EQUAL(tc, *(pos++), (306 >> 8));
            ABTS_INT_EQUAL(tc, *(pos++), 306 & 0xFF);

            /* embedded tlv_t */
            ABTS_INT_EQUAL(tc, *(pos++), tlv_element[2].type & 0xFF);    
            ABTS_INT_EQUAL(tc, *(pos++), tlv_element[2].length >> 8);
            ABTS_INT_EQUAL(tc, *(pos++), tlv_element[2].length & 0xFF);
            for(m = 0; m < tlv_element[2].length; m++)
                ABTS_INT_EQUAL(tc, *(pos++), tlv_element[2].val_char);


            ABTS_INT_EQUAL(tc, *(pos++), tlv_element[3].type & 0xFF);    
            ABTS_INT_EQUAL(tc, *(pos++), tlv_element[3].length >> 8);
            ABTS_INT_EQUAL(tc, *(pos++), tlv_element[3].length & 0xFF);
            for(m = 0; m < tlv_element[3].length; m++)
                ABTS_INT_EQUAL(tc, *(pos++), tlv_element[3].val_char);


            ABTS_INT_EQUAL(tc, *(pos++), tlv_element[4].type & 0xFF);
            ABTS_INT_EQUAL(tc, *(pos++), tlv_element[4].length >> 8);
            ABTS_INT_EQUAL(tc, *(pos++), tlv_element[4].length & 0xFF);

            for(m = 0; m < tlv_element[4].length; m++)
                ABTS_INT_EQUAL(tc, *(pos++), 0x0e);
            break;
        }
        case TLV_MODE_T1_L2_I1:
        {
            ABTS_INT_EQUAL(tc, 332, parent_block_len);
            ABTS_INT_EQUAL(tc, *(pos++), tlv_element[0].type & 0xFF);
            ABTS_INT_EQUAL(tc, *(pos++), tlv_element[0].length >> 8);
            ABTS_INT_EQUAL(tc, *(pos++), tlv_element[0].length & 0xFF);
            ABTS_INT_EQUAL(tc, *(pos++), tlv_element[0].instance & 0xFF);
            for(m = 0; m < tlv_element[0].length; m++)
                ABTS_INT_EQUAL(tc, *(pos++), tlv_element[0].val_char);

            ABTS_INT_EQUAL(tc, *(pos++), EMBED_TLV_TYPE & 0xFF);
            ABTS_INT_EQUAL(tc, *(pos++), (308 >> 8));
            ABTS_INT_EQUAL(tc, *(pos++), 308 & 0xFF);
            ABTS_INT_EQUAL(tc, *(pos++), EMBED_TLV_INSTANCE);

            /* embedded tlv_t */
            ABTS_INT_EQUAL(tc, *(pos++), tlv_element[2].type & 0xFF);    
            ABTS_INT_EQUAL(tc, *(pos++), tlv_element[2].length >> 8);
            ABTS_INT_EQUAL(tc, *(pos++), tlv_element[2].length & 0xFF);
            ABTS_INT_EQUAL(tc, *(pos++), tlv_element[2].instance & 0xFF);    
            for(m = 0; m < tlv_element[2].length; m++)
                ABTS_INT_EQUAL(tc, *(pos++), tlv_element[2].val_char);


            ABTS_INT_EQUAL(tc, *(pos++), tlv_element[3].type & 0xFF);    
            ABTS_INT_EQUAL(tc, *(pos++), tlv_element[3].length >> 8);
            ABTS_INT_EQUAL(tc, *(pos++), tlv_element[3].length & 0xFF);
            ABTS_INT_EQUAL(tc, *(pos++), tlv_element[3].instance & 0xFF);    
            for(m = 0; m < tlv_element[3].length; m++)
                ABTS_INT_EQUAL(tc, *(pos++), tlv_element[3].val_char);


            ABTS_INT_EQUAL(tc, *(pos++), tlv_element[4].type & 0xFF);
            ABTS_INT_EQUAL(tc, *(pos++), tlv_element[4].length >> 8);
            ABTS_INT_EQUAL(tc, *(pos++), tlv_element[4].length & 0xFF);
            ABTS_INT_EQUAL(tc, *(pos++), tlv_element[4].instance & 0xFF);

            for(m = 0; m < tlv_element[4].length; m++)
                ABTS_INT_EQUAL(tc, *(pos++), 0x0e);
            break;
        }
        default:
        {
            ABTS_TRUE(tc, 0);
            break;
        }
    }

    parsed_tlv = tlv_parse_block(parent_block_len,
        parent_block, mode);
    ABTS_PTR_NOTNULL(tc, parsed_tlv);

    pTlv = parsed_tlv;

    ABTS_INT_EQUAL(tc, pTlv->type, tlv_element[0].type);
    ABTS_INT_EQUAL(tc, pTlv->length, tlv_element[0].length);
    ABTS_INT_EQUAL(tc, pTlv->instance, tlv_element[0].instance);
    result = memcmp(pTlv->value, tlv_element[0].value, tlv_element[0].length);
    ABTS_INT_EQUAL(tc, result, 0);
    pTlv = pTlv->next;

    ABTS_INT_EQUAL(tc, pTlv->type, 20);
    switch(mode)
    {
        case TLV_MODE_T2_L2:
        case TLV_MODE_T1_L2_I1:
            ABTS_INT_EQUAL(tc, pTlv->length, 308);
            break;
        case TLV_MODE_T1_L2:
            ABTS_INT_EQUAL(tc, pTlv->length, 306);
            break;
        default:
            ABTS_TRUE(tc, 0);
            break;
    }
    pTlv = pTlv->next;

    ABTS_INT_EQUAL(tc, pTlv->type, tlv_element[4].type);
    ABTS_INT_EQUAL(tc, pTlv->length, tlv_element[4].length);
    if (mode == TLV_MODE_T1_L2_I1)
        ABTS_INT_EQUAL(tc, pTlv->instance, tlv_element[4].instance);
    else
        ABTS_INT_EQUAL(tc, pTlv->instance, 0);
    result = memcmp(pTlv->value, tlv_element[4].value, tlv_element[4].length);
    ABTS_INT_EQUAL(tc, result, 0);
    pTlv = pTlv->next;

    ABTS_PTR_NULL(tc, pTlv);

    parent_tlv = tlv_find(parsed_tlv,20);
    ABTS_PTR_NOTNULL(tc, parent_tlv);

    tlv_parse_embedded_block(parent_tlv, mode);
    embed_tlv = parent_tlv->embedded;
    ABTS_PTR_NOTNULL(tc, embed_tlv);

    ABTS_INT_EQUAL(tc, embed_tlv->type, tlv_element[2].type);
    ABTS_INT_EQUAL(tc, embed_tlv->length, tlv_element[2].length);
    if (mode == TLV_MODE_T1_L2_I1)
        ABTS_INT_EQUAL(tc, embed_tlv->instance, tlv_element[2].instance);
    else
        ABTS_INT_EQUAL(tc, embed_tlv->instance, 0);
    for(m = 0; m < tlv_element[2].length; m++)
    {
        ABTS_INT_EQUAL(tc, *((c_uint8_t*)(embed_tlv->value+m)),
            tlv_element[2].val_char);
    }
    embed_tlv = embed_tlv->next;

    ABTS_INT_EQUAL(tc, embed_tlv->type, tlv_element[3].type);
    ABTS_INT_EQUAL(tc, embed_tlv->length, tlv_element[3].length);
    if (mode == TLV_MODE_T1_L2_I1)
        ABTS_INT_EQUAL(tc, embed_tlv->instance, tlv_element[3].instance);
    else
        ABTS_INT_EQUAL(tc, embed_tlv->instance, 0);
    for(m = 0; m < tlv_element[3].length; m++)
    {
        ABTS_INT_EQUAL(tc, *((c_uint8_t*)(embed_tlv->value+m)),
            tlv_element[3].val_char);
    }
    embed_tlv = embed_tlv->next;

    ABTS_PTR_NULL(tc, embed_tlv);

    embed_tlv = tlv_find(parsed_tlv,254);
    ABTS_PTR_NOTNULL(tc, embed_tlv);

    embed_tlv = tlv_find(parsed_tlv,253);
    ABTS_PTR_NULL(tc, embed_tlv);

    tlv_free_all(parsed_tlv);
    ABTS_INT_EQUAL(tc, tlv_pool_avail(), NUM_OF_TLV_NODE);
	
	return;
}

/* basic encoding/decoding/finding Test */
static void tlv_test_1(abts_case *tc, void *data)
{
    c_uint32_t idx, m, parent_block_len;
    tlv_t *root_tlv = NULL, *parsed_tlv = NULL, *pTlv;
    c_uint8_t parent_block[4000];
    c_uint8_t *pos = NULL;
    c_uint8_t mode = (c_uintptr_t)data;

    tlv_test_set_tlv_value();

    /* tlv encoding for test */
    root_tlv = tlv_add(NULL,tlv_element[0].type,
        tlv_element[0].length, tlv_element[0].instance, tlv_element[0].value);

    for(idx = 1; idx < 4; idx++)
    {
        tlv_add(root_tlv, tlv_element[idx].type,
            tlv_element[idx].length, tlv_element[idx].instance,
            tlv_element[idx].value);
    }

    memset(parent_block, 0x00, sizeof(parent_block));
    parent_block_len = tlv_render(root_tlv, parent_block,
        sizeof(parent_block), mode);

    switch(mode)
    {
        case TLV_MODE_T2_L2:
        case TLV_MODE_T1_L2_I1:
            ABTS_INT_EQUAL(tc, 346, parent_block_len);
            break;
        case TLV_MODE_T1_L2:
            ABTS_INT_EQUAL(tc, 342, parent_block_len);
            break;
        case TLV_MODE_T1_L1:
            ABTS_INT_EQUAL(tc, 338, parent_block_len);
            break;
        default:
            ABTS_TRUE(tc, 0);
            break;
    }

    tlv_free_all(root_tlv);
    ABTS_INT_EQUAL(tc, tlv_pool_avail(), NUM_OF_TLV_NODE);

    pos = parent_block;

    for(idx = 0; idx < 4; idx++)
    {
        switch(mode)
        {
            case TLV_MODE_T2_L2:
                ABTS_INT_EQUAL(tc, (tlv_element[idx].type >> 8), *(pos++));
                ABTS_INT_EQUAL(tc, tlv_element[idx].type & 0xFF, *(pos++));
                ABTS_INT_EQUAL(tc, (tlv_element[idx].length >> 8), *(pos++));
                ABTS_INT_EQUAL(tc, tlv_element[idx].length & 0xFF, *(pos++));
                break;
            case TLV_MODE_T1_L2:
                ABTS_INT_EQUAL(tc, tlv_element[idx].type & 0xFF, *(pos++));
                ABTS_INT_EQUAL(tc, (tlv_element[idx].length >> 8), *(pos++));
                ABTS_INT_EQUAL(tc, tlv_element[idx].length & 0xFF, *(pos++));
                break;
            case TLV_MODE_T1_L1:
                ABTS_INT_EQUAL(tc, tlv_element[idx].type & 0xFF, *(pos++));
                ABTS_INT_EQUAL(tc, tlv_element[idx].length & 0xFF, *(pos++));
                break;
            case TLV_MODE_T1_L2_I1:
                ABTS_INT_EQUAL(tc, tlv_element[idx].type & 0xFF, *(pos++));
                ABTS_INT_EQUAL(tc, (tlv_element[idx].length >> 8), *(pos++));
                ABTS_INT_EQUAL(tc, tlv_element[idx].length & 0xFF, *(pos++));
                ABTS_INT_EQUAL(tc, tlv_element[idx].instance & 0xFF, *(pos++));
                break;
            default:
                ABTS_TRUE(tc, 0);
                break;
        }

        for(m = 0; m < tlv_element[idx].length; m++)
            ABTS_INT_EQUAL(tc, tlv_element[idx].val_char, *(pos++));
    }

    parsed_tlv = tlv_parse_block(parent_block_len,parent_block, mode);
    ABTS_PTR_NOTNULL(tc, parsed_tlv);

    pTlv = parsed_tlv;

    idx = 0;
    while(pTlv)
    {
        int result;
        ABTS_INT_EQUAL(tc, pTlv->type, tlv_element[idx].type);
        ABTS_INT_EQUAL(tc, pTlv->length, tlv_element[idx].length);
        if (mode == TLV_MODE_T1_L2_I1)
            ABTS_INT_EQUAL(tc, pTlv->instance, tlv_element[idx].instance);
        else
            ABTS_INT_EQUAL(tc, pTlv->instance, 0);
        result = memcmp(pTlv->value, tlv_element[idx].value, tlv_element[idx].length);
        ABTS_INT_EQUAL(tc, result, 0);

        pTlv = pTlv->next;
        idx++;
    }

    ABTS_INT_EQUAL(tc, idx, 4);

    pTlv = tlv_find(parsed_tlv, 255);
    ABTS_PTR_NOTNULL(tc, pTlv);

    pTlv = tlv_find(parsed_tlv, 253);
    ABTS_PTR_NULL(tc, pTlv);

    tlv_free_all(parsed_tlv);
    ABTS_INT_EQUAL(tc, tlv_pool_avail(), NUM_OF_TLV_NODE);
    
    return;
}

/* embedded tlv_t test :  first, make embedded tlv block  for embeded tlv
    and then make whole tlv block using embedded tlv block previously made*/
static void tlv_test_2(abts_case *tc, void *data)
{
    c_uint32_t embed_block_len;
    tlv_t *root_tlv = NULL;
    tlv_t *embed_tlv = NULL;
    c_uint8_t embed_block[1000];;
    c_uint8_t mode = (c_uintptr_t)data;

    tlv_test_set_tlv_value();

    /* Tlv Encoding for embeded tlv_t */
    embed_tlv = tlv_add(NULL, tlv_element[2].type,
        tlv_element[2].length, tlv_element[2].instance, tlv_element[2].value);
    tlv_add(embed_tlv,tlv_element[3].type,
        tlv_element[3].length, tlv_element[3].instance, tlv_element[3].value);

    embed_block_len = tlv_render(embed_tlv, embed_block,
        sizeof(embed_block), mode);
    switch(mode)
    {
        case TLV_MODE_T2_L2:
        case TLV_MODE_T1_L2_I1:
            ABTS_INT_EQUAL(tc, embed_block_len, 308);
            break;
        case TLV_MODE_T1_L2:
            ABTS_INT_EQUAL(tc, embed_block_len, 306);
            break;
        default:
            ABTS_TRUE(tc, 0);
            break;
    }

    tlv_free_all(embed_tlv);
    ABTS_INT_EQUAL(tc, tlv_pool_avail(), NUM_OF_TLV_NODE);

    root_tlv = tlv_add(NULL,tlv_element[0].type,
        tlv_element[0].length, tlv_element[0].instance, tlv_element[0].value);

    tlv_add(root_tlv, EMBED_TLV_TYPE, embed_block_len, 
            EMBED_TLV_INSTANCE, embed_block);
    tlv_add(root_tlv,tlv_element[4].type,
        tlv_element[4].length, tlv_element[4].instance, tlv_element[4].value);

    tlv_test_check_embed_tlv_test(tc, root_tlv, mode);

	return;
}


/* embedded tlv_t test :  make parent and embedded tlv and then tlv_render
    functions makes whole tlv block.  the value member of each tlv must
    point to vaild address at the time using tlv_rendering function */
static void tlv_test_3(abts_case *tc, void *data)
{
    tlv_t *root_tlv = NULL, *parent_tlv = NULL;
    c_uint8_t mode = (c_uintptr_t)data;

    tlv_test_set_tlv_value();

    /* Tlv Encoding for embeded tlv_t */
    root_tlv = tlv_add(NULL,tlv_element[0].type,
        tlv_element[0].length, tlv_element[0].instance, tlv_element[0].value);
    parent_tlv= tlv_add(root_tlv, EMBED_TLV_TYPE, 0, EMBED_TLV_INSTANCE, NULL);
    tlv_add(root_tlv,tlv_element[4].type,
        tlv_element[4].length, tlv_element[4].instance, tlv_element[4].value);

    tlv_embed(parent_tlv,tlv_element[2].type,
        tlv_element[2].length, tlv_element[2].instance, tlv_element[2].value);
    tlv_embed(parent_tlv,tlv_element[3].type,
        tlv_element[3].length, tlv_element[3].instance, tlv_element[3].value);

    tlv_test_check_embed_tlv_test(tc, root_tlv, mode);

    return;
}


/* embedded tlv_t test :  make parent and embedded tlv and then tlv_render
    functions makes whole tlv block.  The value member of each tlv is copied
    to the buff of the root tlv_t, so the allocated resource for the value
    member of each tlv can be deallocated after executing tlv_add or tlv embed
    function*/
static void tlv_test_4(abts_case *tc, void *data)
{
    tlv_t *root_tlv = NULL, *parent_tlv = NULL;
    c_uint8_t tlv_buff[2000];
    c_uint8_t mode = (c_uintptr_t)data;

    tlv_test_set_tlv_value();

    root_tlv = tlv_copy(tlv_buff, sizeof(tlv_buff),
        tlv_element[0].type, tlv_element[0].length, 
        tlv_element[0].instance, tlv_element[0].value);
    parent_tlv = tlv_add(root_tlv, EMBED_TLV_TYPE, 0, EMBED_TLV_INSTANCE, NULL);
    tlv_add(root_tlv, tlv_element[4].type, tlv_element[4].length, 
        tlv_element[4].instance, tlv_element[4].value);

    tlv_embed(parent_tlv,tlv_element[2].type,
        tlv_element[2].length, tlv_element[2].instance, tlv_element[2].value);
    tlv_embed(parent_tlv,tlv_element[3].type,
        tlv_element[3].length, tlv_element[3].instance, tlv_element[3].value);

    memset(tlv_element[2].value, 0x00, tlv_element[2].length);
    memset(tlv_element[3].value, 0xf0, tlv_element[3].length);

    tlv_test_check_embed_tlv_test(tc, root_tlv, mode);

    return;
}

/* endian check test */
static void tlv_test_5(abts_case *tc, void *data)
{
    c_uint32_t parent_block_len;
    tlv_t *root_tlv = NULL, *parsed_tlv = NULL, *p_tlv;
    c_uint8_t parent_block[4000];
    c_uint8_t *pos = NULL;
    c_uint16_t c_16 = 0x1122;
    c_uint32_t c_32 = 0x11223344;
    c_uint8_t mode = (c_uintptr_t)data;

    /* tlv encoding for test */
    c_16 = htons(c_16);
    root_tlv = tlv_add(NULL, 10, 2, 0, (c_uint8_t*)&c_16);
    c_32 = htonl(c_32);
    tlv_add(root_tlv, 20, 4, 0, (c_uint8_t*)&c_32);

    memset(parent_block, 0x00, sizeof(parent_block));
    parent_block_len = tlv_render(root_tlv, parent_block,
        sizeof(parent_block), mode);

    tlv_free_all(root_tlv);
    ABTS_INT_EQUAL(tc, tlv_pool_avail(), NUM_OF_TLV_NODE);

    pos = parent_block;

    switch(mode)
    {
        case TLV_MODE_T2_L2:
            ABTS_INT_EQUAL(tc, *(pos++), 0);
            ABTS_INT_EQUAL(tc, *(pos++), 10);
            ABTS_INT_EQUAL(tc, *(pos++), 0);
            ABTS_INT_EQUAL(tc, *(pos++), 2);
            ABTS_INT_EQUAL(tc, *(pos++), 0x11);
            ABTS_INT_EQUAL(tc, *(pos++), 0x22);
            ABTS_INT_EQUAL(tc, *(pos++), 0);
            ABTS_INT_EQUAL(tc, *(pos++), 20);
            ABTS_INT_EQUAL(tc, *(pos++), 0);
            ABTS_INT_EQUAL(tc, *(pos++), 4);
            ABTS_INT_EQUAL(tc, *(pos++), 0x11);
            ABTS_INT_EQUAL(tc, *(pos++), 0x22);
            ABTS_INT_EQUAL(tc, *(pos++), 0x33);
            ABTS_INT_EQUAL(tc, *(pos++), 0x44);
            break;
        case TLV_MODE_T1_L2:
            ABTS_INT_EQUAL(tc, *(pos++), 10);
            ABTS_INT_EQUAL(tc, *(pos++), 0);
            ABTS_INT_EQUAL(tc, *(pos++), 2);
            ABTS_INT_EQUAL(tc, *(pos++), 0x11);
            ABTS_INT_EQUAL(tc, *(pos++), 0x22);
            ABTS_INT_EQUAL(tc, *(pos++), 20);
            ABTS_INT_EQUAL(tc, *(pos++), 0);
            ABTS_INT_EQUAL(tc, *(pos++), 4);
            ABTS_INT_EQUAL(tc, *(pos++), 0x11);
            ABTS_INT_EQUAL(tc, *(pos++), 0x22);
            ABTS_INT_EQUAL(tc, *(pos++), 0x33);
            ABTS_INT_EQUAL(tc, *(pos++), 0x44);
            break;
        case TLV_MODE_T1_L2_I1:
            ABTS_INT_EQUAL(tc, *(pos++), 10);
            ABTS_INT_EQUAL(tc, *(pos++), 0);
            ABTS_INT_EQUAL(tc, *(pos++), 2);
            ABTS_INT_EQUAL(tc, *(pos++), 0);
            ABTS_INT_EQUAL(tc, *(pos++), 0x11);
            ABTS_INT_EQUAL(tc, *(pos++), 0x22);
            ABTS_INT_EQUAL(tc, *(pos++), 20);
            ABTS_INT_EQUAL(tc, *(pos++), 0);
            ABTS_INT_EQUAL(tc, *(pos++), 4);
            ABTS_INT_EQUAL(tc, *(pos++), 0);
            ABTS_INT_EQUAL(tc, *(pos++), 0x11);
            ABTS_INT_EQUAL(tc, *(pos++), 0x22);
            ABTS_INT_EQUAL(tc, *(pos++), 0x33);
            ABTS_INT_EQUAL(tc, *(pos++), 0x44);
            break;
        case TLV_MODE_T1_L1:
            ABTS_INT_EQUAL(tc, *(pos++), 10);
            ABTS_INT_EQUAL(tc, *(pos++), 2);
            ABTS_INT_EQUAL(tc, *(pos++), 0x11);
            ABTS_INT_EQUAL(tc, *(pos++), 0x22);
            ABTS_INT_EQUAL(tc, *(pos++), 20);
            ABTS_INT_EQUAL(tc, *(pos++), 4);
            ABTS_INT_EQUAL(tc, *(pos++), 0x11);
            ABTS_INT_EQUAL(tc, *(pos++), 0x22);
            ABTS_INT_EQUAL(tc, *(pos++), 0x33);
            ABTS_INT_EQUAL(tc, *(pos++), 0x44);
            break;
        default:
            ABTS_TRUE(tc, 0);
            break;
    }

    p_tlv = parsed_tlv = tlv_parse_block(parent_block_len,parent_block,
        mode);
    ABTS_INT_EQUAL(tc, tlv_value_16(p_tlv), 0x1122);
    p_tlv = parsed_tlv->next;
    ABTS_INT_EQUAL(tc, tlv_value_32(p_tlv), 0x11223344);

    tlv_free_all(parsed_tlv);
    ABTS_INT_EQUAL(tc, tlv_pool_avail(), NUM_OF_TLV_NODE);

    return;
}

/* Sample header for tlv_msg */
#define TLV_AUTHORIZATION_POLICY_SUPPORT_TYPE 21
#define TLV_AUTHORIZATION_POLICY_SUPPORT_LEN 1
typedef tlv_uint8_t tlv_authorization_policy_support_t;
extern tlv_desc_t tlv_desc_authorization_policy_support;

#define TLV_CLIENT_SECURITY_HISTORY_TYPE 108
#define TLV_CLIENT_SECURITY_HISTORY_LEN TLV_VARIABLE_LEN
typedef struct _tlv_client_security_history_t {
    tlv_presence_t presence;
    tlv_authorization_policy_support_t authorization_policy_support0;
    tlv_authorization_policy_support_t authorization_policy_support2;
} tlv_client_security_history_t;
extern tlv_desc_t tlv_desc_client_security_history;

#define TLV_CLIENT_INFO_TYPE 103
#define TLV_CLIENT_INFO_LEN TLV_VARIABLE_LEN
typedef struct _tlv_client_info_t {
    tlv_presence_t presence;
    tlv_client_security_history_t client_security_history;
} tlv_client_info_t;
extern tlv_desc_t tlv_desc_client_info;

#define TLV_SERVER_NAME_TYPE 25
#define TLV_SERVER_NAME_LEN TLV_VARIABLE_LEN
typedef tlv_octet_t tlv_server_name_t;
extern tlv_desc_t tlv_desc_server_name;

#define TLV_SERVER_INFO_TYPE 26
#define TLV_SERVER_INFO_LEN TLV_VARIABLE_LEN
typedef struct _tlv_server_info_t {
    tlv_presence_t presence;
    tlv_server_name_t TLV_1_OR_MORE(server_name);
} tlv_server_info_t;
extern tlv_desc_t tlv_desc_server_info;

typedef struct _tlv_attach_req {
    tlv_client_info_t client_info;
    tlv_server_info_t server_info;
} tlv_attach_req;

extern tlv_desc_t tlv_desc_attach_req;

/* Sample source for tlv_msg */
tlv_desc_t tlv_desc_authorization_policy_support0 = 
{
    TLV_UINT8,
    "Auth Policy0",
    TLV_AUTHORIZATION_POLICY_SUPPORT_TYPE,
    TLV_AUTHORIZATION_POLICY_SUPPORT_LEN,
    0,
    sizeof(tlv_authorization_policy_support_t),
    { NULL }
};
tlv_desc_t tlv_desc_authorization_policy_support2 = 
{
    TLV_UINT8,
    "Auth Policy2",
    TLV_AUTHORIZATION_POLICY_SUPPORT_TYPE,
    TLV_AUTHORIZATION_POLICY_SUPPORT_LEN,
    2,
    sizeof(tlv_authorization_policy_support_t),
    { NULL }
};

tlv_desc_t tlv_desc_client_security_history = 
{
    TLV_COMPOUND,
    "Sec History",
    TLV_CLIENT_SECURITY_HISTORY_TYPE,
    TLV_CLIENT_SECURITY_HISTORY_LEN,
    0, 
    sizeof(tlv_client_security_history_t),
    {
        &tlv_desc_authorization_policy_support0,
        &tlv_desc_authorization_policy_support2,
        NULL,
    }
};

tlv_desc_t tlv_desc_client_info = 
{
    TLV_COMPOUND,
    "Client Info",
    TLV_CLIENT_INFO_TYPE,
    TLV_CLIENT_INFO_LEN,
    0, 
    sizeof(tlv_client_info_t),
    {
        &tlv_desc_client_security_history,
        NULL,
    }
};

tlv_desc_t tlv_desc_server_name = 
{
    TLV_VAR_STR,
    "Server Name",
    TLV_SERVER_NAME_TYPE,
    TLV_SERVER_NAME_LEN,
    0, 
    sizeof(tlv_server_name_t),
    { NULL }
};

tlv_desc_t tlv_desc_server_info = 
{
    TLV_COMPOUND,
    "Server Info",
    TLV_SERVER_INFO_TYPE,
    TLV_SERVER_INFO_LEN,
    0, 
    sizeof(tlv_server_info_t),
    {
        &tlv_desc_server_name, &tlv_desc_more2,
        NULL,
    }
};

tlv_desc_t tlv_desc_attach_req = {
    TLV_MESSAGE, "Attach Req", 0, 0, 0, 0, {
    &tlv_desc_client_info,
    &tlv_desc_server_info,
    NULL,
}};

static void tlv_test_6(abts_case *tc, void *data)
{
    tlv_attach_req reqv;
    tlv_attach_req reqv2;

    pkbuf_t *req = NULL;
    char testbuf[1024];

    /* Initialize message value structure */
    memset(&reqv, 0, sizeof(tlv_attach_req));

    /* Set nessary members of message */
    reqv.client_info.presence = 1;
    reqv.client_info.client_security_history.presence = 1;
    reqv.client_info.client_security_history.
            authorization_policy_support0.presence = 1;
    reqv.client_info.client_security_history.
            authorization_policy_support0.u8 = 0x3;
    reqv.client_info.client_security_history.
            authorization_policy_support2.presence = 1;
    reqv.client_info.client_security_history.
            authorization_policy_support2.u8 = 0x9;
   
    reqv.server_info.presence = 1;
    reqv.server_info.server_name[0].presence = 1;
    reqv.server_info.server_name[0].data = 
        (c_uint8_t*)"\x11\x22\x33\x44\x55\x66";
    reqv.server_info.server_name[0].len = 6;

    reqv.server_info.presence = 1;
    reqv.server_info.server_name[1].presence = 1;
    reqv.server_info.server_name[1].data = 
        (c_uint8_t*)"\xaa\xbb\xcc\xdd\xee\xff";
    reqv.server_info.server_name[1].len = 6;

    /* Build message */
    tlv_build_msg(&req, &tlv_desc_attach_req, &reqv, TLV_MODE_T1_L2_I1);
#define TEST_TLV_BUILD_MSG \
    "67000e00 6c000a00 15000100 03150001" \
    "02091a00 14001900 06001122 33445566" \
    "19000600 aabbccdd eeff"

    ABTS_INT_EQUAL(tc, 42, req->len);
    ABTS_TRUE(tc, memcmp(req->payload, 
        CORE_HEX(TEST_TLV_BUILD_MSG, strlen(TEST_TLV_BUILD_MSG), testbuf), 
        req->len) == 0);

    /* Initialize message value structure */
    memset(&reqv2, 0, sizeof(tlv_attach_req));

    /* Parse message */
    tlv_parse_msg(&reqv2, &tlv_desc_attach_req, req,
            TLV_MODE_T1_L2_I1);

    ABTS_INT_EQUAL(tc, 1, reqv2.client_info.presence);
    ABTS_INT_EQUAL(tc, 1, 
            reqv2.client_info.client_security_history.presence);
    ABTS_INT_EQUAL(tc, 1, 
            reqv2.client_info.client_security_history.
                        authorization_policy_support0.presence);
    ABTS_INT_EQUAL(tc, 0x3, reqv2.client_info.
            client_security_history.authorization_policy_support0.u8);
    ABTS_INT_EQUAL(tc, 1, 
            reqv2.client_info.client_security_history.
                        authorization_policy_support2.presence);
    ABTS_INT_EQUAL(tc, 0x9, reqv2.client_info.
            client_security_history.authorization_policy_support2.u8);

    ABTS_INT_EQUAL(tc, 1, reqv2.server_info.presence);
    ABTS_INT_EQUAL(tc, 1, reqv2.server_info.server_name[0].presence);
    ABTS_INT_EQUAL(tc, 1, reqv2.server_info.server_name[1].presence);
    ABTS_INT_EQUAL(tc, 6, reqv2.server_info.server_name[0].len);
    ABTS_TRUE(tc, memcmp(reqv2.server_info.server_name[0].data, 
                (c_uint8_t*)"\x11\x22\x33\x44\x55\x66", 6) == 0);
    ABTS_INT_EQUAL(tc, 6, reqv2.server_info.server_name[1].len);
    ABTS_TRUE(tc, memcmp(reqv2.server_info.server_name[1].data, 
                (c_uint8_t*)"\xaa\xbb\xcc\xdd\xee\xff", 6) == 0);

    pkbuf_free(req);
}

abts_suite *testtlv(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, tlv_test_1, (void*)TLV_MODE_T2_L2);
    abts_run_test(suite, tlv_test_2, (void*)TLV_MODE_T2_L2);
	abts_run_test(suite, tlv_test_3, (void*)TLV_MODE_T2_L2);
	abts_run_test(suite, tlv_test_4, (void*)TLV_MODE_T2_L2);
	abts_run_test(suite, tlv_test_5, (void*)TLV_MODE_T2_L2);
    abts_run_test(suite, tlv_test_1, (void*)TLV_MODE_T1_L1);
    abts_run_test(suite, tlv_test_5, (void*)TLV_MODE_T1_L1);
    abts_run_test(suite, tlv_test_1, (void*)TLV_MODE_T1_L2);
    abts_run_test(suite, tlv_test_2, (void*)TLV_MODE_T1_L2);
	abts_run_test(suite, tlv_test_3, (void*)TLV_MODE_T1_L2);
	abts_run_test(suite, tlv_test_4, (void*)TLV_MODE_T1_L2);
	abts_run_test(suite, tlv_test_5, (void*)TLV_MODE_T1_L2);
    abts_run_test(suite, tlv_test_1, (void*)TLV_MODE_T1_L2_I1);
    abts_run_test(suite, tlv_test_2, (void*)TLV_MODE_T1_L2_I1);
	abts_run_test(suite, tlv_test_3, (void*)TLV_MODE_T1_L2_I1);
	abts_run_test(suite, tlv_test_4, (void*)TLV_MODE_T1_L2_I1);
	abts_run_test(suite, tlv_test_5, (void*)TLV_MODE_T1_L2_I1);

	abts_run_test(suite, tlv_test_6, NULL);

    return suite;
}

