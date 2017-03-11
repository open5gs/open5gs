#ifndef __CORE_TLV_H__
#define __CORE_TLV_H__

#include "core.h"
#include "core_pool.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#define TLV_MODE_T1_L1              1
#define TLV_MODE_T1_L2              2
#define TLV_MODE_T1_L2_I1           3
#define TLV_MODE_T2_L2              4

#define NUM_OF_TLV_NODE             100

/* tlv_t struncture */

typedef struct _tlv_t
{
    /* for tlv management */
    struct _tlv_t* head;
    struct _tlv_t* tail;  /* this is used only for head tlv_t */
    struct _tlv_t* next;

    struct _tlv_t* parent;
    struct _tlv_t* embedded;


    /* tlv basic element */
    c_uint32_t  type;
    c_uint32_t  length;
    c_uint8_t   instance;
    void*       value;

    /* can be needed in encoding tlv_t*/
    c_uint8_t   buff_allocated;
    c_uint32_t  buff_len;
    c_uint8_t*  buff_ptr;
    c_uint8_t*  buff;
} tlv_t;

#define tlv_type(pTlv) pTlv->type
#define tlv_length(pTlv) pTlv->length
#define tlv_instance(pTlv) pTlv->instance
#define tlv_value(pTlv) pTlv->value

/* tlv_t pool related functions */
CORE_DECLARE(tlv_t*) tlv_get(void);
CORE_DECLARE(void) tlv_free(tlv_t *pTlv);
CORE_DECLARE(void) tlv_free_all(tlv_t *rootTlv);

CORE_DECLARE(status_t) tlv_init(void);
CORE_DECLARE(status_t) tlv_final(void);

/* tlv_t encoding functions */
CORE_DECLARE(tlv_t*) tlv_add(tlv_t *headTlv, 
    c_uint32_t type, c_uint32_t length, c_uint8_t instance, c_uint8_t *value);
CORE_DECLARE(tlv_t*) tlv_copy(c_uint8_t *buff, c_uint32_t buff_len,
    c_uint32_t type, c_uint32_t length, c_uint8_t instance, c_uint8_t *value);
CORE_DECLARE(tlv_t*) tlv_embed(tlv_t *parent_tlv, 
    c_uint32_t type, c_uint32_t length, c_uint8_t instance, c_uint8_t *value);

CORE_DECLARE(c_uint32_t) tlv_render(
        tlv_t *rootTlv, c_uint8_t *blk, c_uint32_t length, c_uint8_t mode);

/* tlv_t parsing functions */
CORE_DECLARE(tlv_t*) tlv_parse_block(
        c_uint32_t length, c_uint8_t *blk, c_uint8_t mode);
CORE_DECLARE(tlv_t*) tlv_parse_embedded_block(tlv_t* pTlv, c_uint8_t mode);

/* tlv operation-related function */
CORE_DECLARE(tlv_t*) tlv_find(tlv_t* pTlv, c_uint32_t type);
CORE_DECLARE(tlv_t*) tlv_find_root(tlv_t* pTlv);
CORE_DECLARE(c_uint32_t) tlv_pool_avail(void);
CORE_DECLARE(c_uint32_t) tlv_calc_length(tlv_t *p_tlv, c_uint8_t mode);
CORE_DECLARE(c_uint32_t) tlv_calc_count(tlv_t *p_tlv);
CORE_DECLARE(c_uint8_t) tlv_value_8(tlv_t *tlv);
CORE_DECLARE(c_uint16_t) tlv_value_16(tlv_t *tlv);
CORE_DECLARE(c_uint32_t) tlv_value_32(tlv_t *tlv);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !__CORE_TLV_H__ */
