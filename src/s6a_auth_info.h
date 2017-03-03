#ifndef _S6A_AUTH_H__
#define _S6A_AUTH_H__

#include "core_errno.h"

#include "plmn_id.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define MAX_IMSI_BCD_DIGITS_LEN 15

#define MAX_EUTRAN_VECTORS      1

#define MAX_RAND_LEN            (128/8)
#define MAX_XRES_LEN            16
#define MAX_AUTN_LEN            (128/8)
#define MAX_KASME_LEN           (256/8)

typedef struct _s6a_eutran_vector_t {
    c_uint8_t rand[MAX_RAND_LEN];
    c_uint8_t xres[MAX_XRES_LEN];
    c_uint8_t xres_len;
    c_uint8_t autn[MAX_AUTN_LEN];
    c_uint8_t kasme[MAX_KASME_LEN];
} s6a_eutran_vector_t;    

typedef struct _s6a_auth_info_t {
    c_uint8_t num_of_eutran_vector;
    s6a_eutran_vector_t s6a_eutran_vector[MAX_EUTRAN_VECTORS];

    c_uint8_t immediate_response_preferred;
} s6a_auth_info_t;

typedef struct _s6a_auth_info_req_t {
    c_uint8_t imsi[MAX_IMSI_BCD_DIGITS_LEN+1];
    c_uint8_t imsi_len;

    plmn_id_t visited_plmn_id;

    s6a_auth_info_t auth_info;
} s6a_auth_info_req_t;

typedef struct _s6a_auth_info_ans_t {
    s6a_auth_info_t auth_info;
} s6a_auth_info_ans_t;

CORE_DECLARE(int) s6a_send_auth_info_req(s6a_auth_info_req_t *air);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

