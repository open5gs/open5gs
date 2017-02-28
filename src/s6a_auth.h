#ifndef _S6A_AUTH_H__
#define _S6A_AUTH_H__

#include "core_errno.h"

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define MAX_IMSI_BCD_DIGITS_LEN 15

#define MAX_EPS_AUTH_VECTORS    1
#define AUTS_LENGTH             14

#define RAND_LENGTH_BITS       (128)
#define RAND_LENGTH_OCTETS     (RAND_LENGTH_BITS/8)
#define XRES_LENGTH_MIN        (4)
#define XRES_LENGTH_MAX        (16)
#define KASME_LENGTH_BITS      (256)
#define KASME_LENGTH_OCTETS    (KASME_LENGTH_BITS/8)
#define AUTN_LENGTH_BITS       (128)
#define AUTN_LENGTH_OCTETS     (AUTN_LENGTH_BITS/8)

typedef struct {
    c_uint8_t size;
    c_uint8_t data[XRES_LENGTH_MAX];
} res_t;

typedef struct eutran_vector_s {
    c_uint8_t rand[RAND_LENGTH_OCTETS];
    res_t xres;         
    c_uint8_t autn[AUTN_LENGTH_OCTETS];
    c_uint8_t kasme[KASME_LENGTH_OCTETS];
} eutran_vector_t;    

typedef struct _authentication_info_s {
    c_uint8_t nb_of_vectors;
    eutran_vector_t eutran_vector[MAX_EPS_AUTH_VECTORS];
} authentication_info_t;

typedef struct _s6a_auth_info_req_s {
    char imsi[MAX_IMSI_BCD_DIGITS_LEN + 1];
    c_uint8_t imsi_len;
    plmn_id_t visited_plmn_id;
    c_uint8_t num_of_vectors;

    unsigned re_synchronization:1;
    c_uint8_t auts[AUTS_LENGTH];
} s6a_auth_info_req_t;

typedef struct _s6a_auth_info_ans_s {
    char imsi[MAX_IMSI_BCD_DIGITS_LEN + 1];
    c_uint8_t imsi_len;

    authentication_info_t auth_info;
} s6a_auth_info_ans_t;

CORE_DECLARE(int) s6_send_auth_info_req();

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

