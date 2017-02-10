#ifndef _S1AP_MESSAGE_H__
#define _S1AP_MESSAGE_H__

#include "s1ap_codecs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define S1AP_SDU_SIZE 2048

#define BUFFER_TO_INT8(buf, x) (x = ((buf)[0]))

#define INT8_TO_BUFFER(x, buf) ((buf)[0] = (x))

/* Convert an integer on 16 bits to the given bUFFER */
#define INT16_TO_BUFFER(x, buf) \
do {                            \
    (buf)[0] = (x) >> 8;        \
    (buf)[1] = (x);             \
} while(0)

/* Convert an array of char containing vALUE to x */
#define BUFFER_TO_INT16(buf, x) \
do {                            \
    x = ((buf)[0] << 8)  |      \
        ((buf)[1]);             \
} while(0)

/* Convert an integer on 32 bits to the given bUFFER */
#define INT32_TO_BUFFER(x, buf) \
do {                            \
    (buf)[0] = (x) >> 24;       \
    (buf)[1] = (x) >> 16;       \
    (buf)[2] = (x) >> 8;        \
    (buf)[3] = (x);             \
} while(0)

/* Convert an array of char containing vALUE to x */
#define BUFFER_TO_INT32(buf, x) \
do {                            \
    x = ((buf)[0] << 24) |      \
        ((buf)[1] << 16) |      \
        ((buf)[2] << 8)  |      \
        ((buf)[3]);             \
} while(0)

/* Convert an integer on 32 bits to an octet string from aSN1c tool */
#define INT32_TO_OCTET_STRING(x, aSN)           \
do {                                            \
    (aSN)->buf = CALLOC(4, sizeof(uint8_t));    \
    INT32_TO_BUFFER(x, ((aSN)->buf));           \
    (aSN)->size = 4;                            \
} while(0)

#define INT32_TO_BIT_STRING(x, aSN) \
do {                                \
    INT32_TO_OCTET_STRING(x, aSN);  \
    (aSN)->bits_unused = 0;         \
} while(0)

#define INT16_TO_OCTET_STRING(x, aSN)           \
do {                                            \
    (aSN)->buf = CALLOC(2, sizeof(uint8_t));    \
    (aSN)->size = 2;              \
    INT16_TO_BUFFER(x, (aSN)->buf);             \
} while(0)

#define INT8_TO_OCTET_STRING(x, aSN)            \
do {                                            \
    (aSN)->buf = CALLOC(1, sizeof(uint8_t));    \
    (aSN)->size = 1;                            \
    INT8_TO_BUFFER(x, (aSN)->buf);              \
} while(0)

#define MME_CODE_TO_OCTET_STRING INT8_TO_OCTET_STRING
#define M_TMSI_TO_OCTET_STRING   INT32_TO_OCTET_STRING
#define MME_GID_TO_OCTET_STRING  INT16_TO_OCTET_STRING

#define OCTET_STRING_TO_INT8(aSN, x)    \
do {                                    \
    d_assert((aSN)->size == 1, , "invalid size(%d)", (aSN)->size); \
    BUFFER_TO_INT8((aSN)->buf, x);    \
} while(0)

#define OCTET_STRING_TO_INT16(aSN, x)   \
do {                                    \
    d_assert((aSN)->size == 2, , "invalid size(%d)", (aSN)->size); \
    BUFFER_TO_INT16((aSN)->buf, x);    \
} while(0)

#define OCTET_STRING_TO_INT32(aSN, x)   \
do {                                    \
    d_assert((aSN)->size == 4, ,  "invalid size(%d)", (aSN)->size); \
    BUFFER_TO_INT32((aSN)->buf, x);    \
} while(0)

#define BIT_STRING_TO_INT32(aSN, x)     \
do {                                    \
    d_assert((aSN)->bits_unused == 0, , "unused bits(%d)", (aSN)->bits_unused); \
    OCTET_STRING_TO_INT32(aSN, x);      \
} while(0)

#define BIT_STRING_TO_CELL_IDENTITY(aSN, vALUE)                     \
do {                                                                \
    d_assert((aSN)->bits_unused == 4, , "unused bits(%d)", (aSN)->bits_unused); \
    vALUE.enb_id = ((aSN)->buf[0] << 12) | ((aSN)->buf[1] << 4) |   \
        ((aSN)->buf[2] >> 4);                                       \
    vALUE.cell_id = ((aSN)->buf[2] << 4) | ((aSN)->buf[3] >> 4);    \
} while(0)

#define MCC_HUNDREDS(vALUE) \
    ((vALUE) / 100)
/* When MNC is only composed of 2 digits, set the hundreds unit to 0xf */
#define MNC_HUNDREDS(vALUE, mNCdIGITlENGTH) \
    ( mNCdIGITlENGTH == 2 ? 15 : (vALUE) / 100)
#define MCC_MNC_DECIMAL(vALUE) \
    (((vALUE) / 10) % 10)
#define MCC_MNC_DIGIT(vALUE) \
    ((vALUE) % 10)

#define MCC_TO_BUFFER(mCC, bUFFER)      \
do {                                    \
    d_assert(bUFFER, , "Null param"); \
    (bUFFER)[0] = MCC_HUNDREDS(mCC);    \
    (bUFFER)[1] = MCC_MNC_DECIMAL(mCC); \
    (bUFFER)[2] = MCC_MNC_DIGIT(mCC);   \
} while(0)

#define MCC_MNC_TO_PLMNID(mCC, mNC, mNCdIGITlENGTH, oCTETsTRING)               \
do {                                                                           \
    (oCTETsTRING)->buf = CALLOC(3, sizeof(uint8_t));                           \
    (oCTETsTRING)->buf[0] = (MCC_MNC_DECIMAL(mCC) << 4) | MCC_HUNDREDS(mCC);   \
    (oCTETsTRING)->buf[1] = (MNC_HUNDREDS(mNC,mNCdIGITlENGTH) << 4) | MCC_MNC_DIGIT(mCC);     \
    (oCTETsTRING)->buf[2] = (MCC_MNC_DIGIT(mNC) << 4) | MCC_MNC_DECIMAL(mNC);  \
    (oCTETsTRING)->size = 3;                                                   \
} while(0)

#define MCC_MNC_TO_TBCD(mCC, mNC, mNCdIGITlENGTH, tBCDsTRING)        \
do {                                                                 \
    char _buf[3];                                                    \
    d_assert((mNCdIGITlENGTH == 3) || (mNCdIGITlENGTH == 2),, "Invalid Length(%d)", mNCdIGITlENGTH);      \
    _buf[0] = (MCC_MNC_DECIMAL(mCC) << 4) | MCC_HUNDREDS(mCC);       \
    _buf[1] = (MNC_HUNDREDS(mNC,mNCdIGITlENGTH) << 4) | MCC_MNC_DIGIT(mCC);\
    _buf[2] = (MCC_MNC_DIGIT(mNC) << 4) | MCC_MNC_DECIMAL(mNC);      \
    OCTET_STRING_fromBuf(tBCDsTRING, _buf, 3);                       \
} while(0)

#define TBCD_TO_MCC_MNC(tBCDsTRING, mCC, mNC, mNCdIGITlENGTH)    \
do {                                                             \
    int mNC_hundred;                                             \
    d_assert((tBCDsTRING)->size == 3, , "invalid size(%d)", (tBCDsTRING)->size); \
    mNC_hundred = (((tBCDsTRING)->buf[1] & 0xf0) >> 4);          \
    if (mNC_hundred == 0xf) {                                    \
        mNC_hundred = 0;                                         \
        mNCdIGITlENGTH = 2;                                      \
    } else {                                                     \
            mNCdIGITlENGTH = 3;                                  \
    }                                                            \
    mCC = (((((tBCDsTRING)->buf[0]) & 0xf0) >> 4) * 10) +        \
        ((((tBCDsTRING)->buf[0]) & 0x0f) * 100) +                \
        (((tBCDsTRING)->buf[1]) & 0x0f);                         \
    mNC = (mNC_hundred * 100) +                                  \
        ((((tBCDsTRING)->buf[2]) & 0xf0) >> 4) +                 \
        ((((tBCDsTRING)->buf[2]) & 0x0f) * 10);                  \
} while(0)

#define TBCD_TO_PLMN_T(tBCDsTRING, pLMN)                            \
do {                                                                \
    d_assert((tBCDsTRING)->size == 3, , "invalid size(%d)", (tBCDsTRING)->size); \
    (pLMN)->mcc_digit2 = (((tBCDsTRING)->buf[0] & 0xf0) >> 4);      \
    (pLMN)->mcc_digit1 = ((tBCDsTRING)->buf[0]  & 0x0f);            \
    (pLMN)->mnc_digit3 = (((tBCDsTRING)->buf[1]  & 0xf0) >> 4);     \
    (pLMN)->mcc_digit3 = ((tBCDsTRING)->buf[1]  & 0x0f);            \
    (pLMN)->mnc_digit2 = (((tBCDsTRING)->buf[2] & 0xf0) >> 4);      \
    (pLMN)->mnc_digit1 = ((tBCDsTRING)->buf[2]  & 0x0f);            \
} while(0)

#define PLMN_T_TO_TBCD(pLMN, tBCDsTRING, mNClENGTH)                 \
do {                                                                \
    tBCDsTRING[0] = (pLMN.mcc_digit2 << 4) | pLMN.mcc_digit1;         \
    /* ambiguous (think about len 2) */                             \
    if (mNClENGTH == 2) {                                      \
        tBCDsTRING[1] = (0x0F << 4) | pLMN.mcc_digit3;               \
        tBCDsTRING[2] = (pLMN.mnc_digit2 << 4) | pLMN.mnc_digit1;     \
    } else {                                                        \
        tBCDsTRING[1] = (pLMN.mnc_digit3 << 4) | pLMN.mcc_digit3;     \
        tBCDsTRING[2] = (pLMN.mnc_digit2 << 4) | pLMN.mnc_digit1;     \
    }                                                               \
} while(0)

#define PLMN_T_TO_MCC_MNC(pLMN, mCC, mNC, mNCdIGITlENGTH)               \
do {                                                                    \
    mCC = pLMN.mcc_digit3 * 100 + pLMN.mcc_digit2 * 10 + pLMN.mcc_digit1;  \
    mNCdIGITlENGTH = (pLMN.mnc_digit3 == 0xF ? 2 : 3);                   \
    mNC = (mNCdIGITlENGTH == 2 ? 0 : pLMN.mnc_digit3 * 100)              \
          + pLMN.mnc_digit2 * 10 + pLMN.mnc_digit1;                       \
} while(0)

/*
 * TS 36.413 v10.9.0 section 9.2.1.37:
 * Macro eNB ID:
 * Equal to the 20 leftmost bits of the Cell
 * Identity IE contained in the E-UTRAN CGI
 * IE (see subclause 9.2.1.38) of each cell
 * served by the eNB.
 */
#define MACRO_ENB_ID_TO_BIT_STRING(mACRO, bITsTRING)    \
do {                                                    \
    (bITsTRING)->buf = CALLOC(3, sizeof(uint8_t));      \
    (bITsTRING)->buf[0] = ((mACRO) >> 12);              \
    (bITsTRING)->buf[1] = (mACRO) >> 4;                 \
    (bITsTRING)->buf[2] = ((mACRO) & 0x0f) << 4;        \
    (bITsTRING)->size = 3;                              \
    (bITsTRING)->bits_unused = 4;                       \
} while(0)
/*
 * TS 36.413 v10.9.0 section 9.2.1.38:
 * E-UTRAN CGI/Cell Identity
 * The leftmost bits of the Cell
 * Identity correspond to the eNB
 * ID (defined in subclause 9.2.1.37).
 */
#define MACRO_ENB_ID_TO_CELL_IDENTITY(mACRO, cELL_iD, bITsTRING) \
do {                                                    \
    (bITsTRING)->buf = CALLOC(4, sizeof(uint8_t));      \
    (bITsTRING)->buf[0] = ((mACRO) >> 12);              \
    (bITsTRING)->buf[1] = (mACRO) >> 4;                 \
    (bITsTRING)->buf[2] = (((mACRO) & 0x0f) << 4) | ((cELL_iD) >> 4);        \
    (bITsTRING)->buf[3] = ((cELL_iD) & 0x0f) << 4;        \
    (bITsTRING)->size = 4;                              \
    (bITsTRING)->bits_unused = 4;                       \
} while(0)

/* Used to format an uint32_t containing an ipv4 address */
#define IPV4_ADDR    "%u.%u.%u.%u"
#define IPV4_ADDR_FORMAT(aDDRESS)               \
    (uint8_t)((aDDRESS)  & 0x000000ff),         \
    (uint8_t)(((aDDRESS) & 0x0000ff00) >> 8 ),  \
    (uint8_t)(((aDDRESS) & 0x00ff0000) >> 16),  \
    (uint8_t)(((aDDRESS) & 0xff000000) >> 24)

#define IPV4_ADDR_DISPLAY_8(aDDRESS)            \
    (aDDRESS)[0], (aDDRESS)[1], (aDDRESS)[2], (aDDRESS)[3]

#define TAC_TO_ASN1 INT16_TO_OCTET_STRING
#define GTP_TEID_TO_ASN1 INT32_TO_OCTET_STRING
#define OCTET_STRING_TO_TAC OCTET_STRING_TO_INT16
#define OCTET_STRING_TO_MME_CODE OCTET_STRING_TO_INT8
#define OCTET_STRING_TO_M_TMSI   OCTET_STRING_TO_INT32
#define OCTET_STRING_TO_MME_GID  OCTET_STRING_TO_INT16
#define OCTET_STRING_TO_CSG_ID   OCTET_STRING_TO_INT27

/* Convert the IMSI contained by a char string NULL terminated to uint64_t */
#define IMSI_STRING_TO_IMSI64(sTRING, iMSI64_pTr) sscanf(sTRING, IMSI_64_FMT, iMSI64_pTr)
#define IMSI64_TO_STRING(iMSI64, sTRING) snprintf(sTRING, IMSI_BCD_DIGITS_MAX+1, IMSI_64_FMT, iMSI64)
#define IMSI_TO_IMSI64(iMsI_t_PtR,iMsI_u64) \
        {\
          uint64_t mUlT = 1; \
          iMsI_u64 = (iMsI_t_PtR)->u.num.digit1; \
          if ((iMsI_t_PtR)->u.num.digit15 != 0xf) { \
            iMsI_u64 = (iMsI_t_PtR)->u.num.digit15 + \
                       (iMsI_t_PtR)->u.num.digit14 *10 + \
                       (iMsI_t_PtR)->u.num.digit13 *100 + \
                       (iMsI_t_PtR)->u.num.digit12 *1000 + \
                       (iMsI_t_PtR)->u.num.digit11 *10000 + \
                       (iMsI_t_PtR)->u.num.digit10 *100000 + \
                       (iMsI_t_PtR)->u.num.digit9  *1000000 + \
                       (iMsI_t_PtR)->u.num.digit8  *10000000 + \
                       (iMsI_t_PtR)->u.num.digit7  *100000000;  \
            mUlT = 1000000000; \
          } else { \
            iMsI_u64 = (iMsI_t_PtR)->u.num.digit14  + \
                       (iMsI_t_PtR)->u.num.digit13 *10 + \
                       (iMsI_t_PtR)->u.num.digit12 *100 + \
                       (iMsI_t_PtR)->u.num.digit11 *1000 + \
                       (iMsI_t_PtR)->u.num.digit10 *10000 + \
                       (iMsI_t_PtR)->u.num.digit9  *100000 + \
                       (iMsI_t_PtR)->u.num.digit8  *1000000 + \
                       (iMsI_t_PtR)->u.num.digit7  *10000000;  \
            mUlT = 100000000; \
          } \
          if ((iMsI_t_PtR)->u.num.digit6 != 0xf) {\
              iMsI_u64 += (iMsI_t_PtR)->u.num.digit6 *mUlT;   \
              iMsI_u64 += (iMsI_t_PtR)->u.num.digit5 *mUlT*10; \
              iMsI_u64 += (iMsI_t_PtR)->u.num.digit4 *mUlT*100; \
              iMsI_u64 += (iMsI_t_PtR)->u.num.digit3 *mUlT*1000; \
              iMsI_u64 += (iMsI_t_PtR)->u.num.digit2 *mUlT*10000; \
              iMsI_u64 += (iMsI_t_PtR)->u.num.digit1 *mUlT*100000; \
          } else { \
              iMsI_u64 += (iMsI_t_PtR)->u.num.digit5 *mUlT;    \
              iMsI_u64 += (iMsI_t_PtR)->u.num.digit4 *mUlT*10;  \
              iMsI_u64 += (iMsI_t_PtR)->u.num.digit3 *mUlT*100;  \
              iMsI_u64 += (iMsI_t_PtR)->u.num.digit2 *mUlT*1000;  \
              iMsI_u64 += (iMsI_t_PtR)->u.num.digit1 *mUlT*10000;  \
          } \
        }
/*#define IMSI_TO_STRING(iMsI_t_PtR,iMsI_sTr, MaXlEn) \
        {\
          int l_offset = 0;\
          int l_ret    = 0;\
          l_ret = snprintf(iMsI_sTr + l_offset, MaXlEn - l_offset, "%u%u%u%u%u",\
              (iMsI_t_PtR)->u.num.digit1, (iMsI_t_PtR)->u.num.digit2,\
                  (iMsI_t_PtR)->u.num.digit3, (iMsI_t_PtR)->u.num.digit4,\
                  (iMsI_t_PtR)->u.num.digit5);\
          if (((iMsI_t_PtR)->u.num.digit6 != 0xf)  && (l_ret > 0)) {\
            l_offset += l_ret;\
            l_ret = snprintf(iMsI_sTr + l_offset, MaXlEn - l_offset,  "%u", (iMsI_t_PtR)->u.num.digit6);\
          }\
          if (l_ret > 0) {\
            l_offset += l_ret;\
            l_ret = snprintf(iMsI_sTr + l_offset, MaXlEn - l_offset, "%u%u%u%u%u%u%u%u",\
                (iMsI_t_PtR)->u.num.digit7, (iMsI_t_PtR)->u.num.digit8,\
                (iMsI_t_PtR)->u.num.digit9, (iMsI_t_PtR)->u.num.digit10,\
                (iMsI_t_PtR)->u.num.digit11, (iMsI_t_PtR)->u.num.digit12,\
                (iMsI_t_PtR)->u.num.digit13, (iMsI_t_PtR)->u.num.digit14);\
          }\
          if (((iMsI_t_PtR)->u.num.digit15 != 0x0)   && (l_ret > 0)) {\
            l_offset += l_ret;\
            l_ret = snprintf(iMsI_sTr + l_offset, MaXlEn - l_offset, "%u", (iMsI_t_PtR)->u.num.digit15);\
          }\
        }*/

#define IMSI_TO_STRING(iMsI_t_PtR,iMsI_sTr, MaXlEn) \
        do { \
          int l_i = 0; \
          int l_j = 0; \
          while((l_i < IMSI_BCD8_SIZE) && (l_j < MaXlEn - 1)){ \
            if((((iMsI_t_PtR)->u.value[l_i] & 0xf0) >> 4) > 9) \
              break; \
            sprintf(((iMsI_sTr) + l_j), "%u",(((iMsI_t_PtR)->u.value[l_i] & 0xf0) >> 4)); \
            l_j++; \
            if(((iMsI_t_PtR)->u.value[l_i] & 0xf) > 9 || (l_j >= MaXlEn - 1)) \
              break; \
            sprintf(((iMsI_sTr) + l_j), "%u", ((iMsI_t_PtR)->u.value[l_i] & 0xf)); \
            l_j++; \
            l_i++; \
          } \
          for(; l_j < MaXlEn; l_j++) \
              iMsI_sTr[l_j] = '\0'; \
        } while (0);\

#define IMEI_TO_STRING(iMeI_t_PtR,iMeI_sTr, MaXlEn) \
        {\
          int l_offset = 0;\
          int l_ret    = 0;\
          l_ret = snprintf(iMeI_sTr + l_offset, MaXlEn - l_offset, "%u%u%u%u%u%u%u%u",\
                  (iMeI_t_PtR)->u.num.tac1, (iMeI_t_PtR)->u.num.tac2,\
                  (iMeI_t_PtR)->u.num.tac3, (iMeI_t_PtR)->u.num.tac4,\
                  (iMeI_t_PtR)->u.num.tac5, (iMeI_t_PtR)->u.num.tac6,\
                  (iMeI_t_PtR)->u.num.tac7, (iMeI_t_PtR)->u.num.tac8);\
          if (l_ret > 0) {\
            l_offset += l_ret;\
            l_ret = snprintf(iMeI_sTr + l_offset, MaXlEn - l_offset, "%u%u%u%u%u%u",\
                (iMeI_t_PtR)->u.num.snr1, (iMeI_t_PtR)->u.num.snr2,\
                (iMeI_t_PtR)->u.num.snr3, (iMeI_t_PtR)->u.num.snr4,\
                (iMeI_t_PtR)->u.num.snr5, (iMeI_t_PtR)->u.num.snr6);\
          }\
          if (((iMeI_t_PtR)->u.num.parity != 0x0)   && (l_ret > 0)) {\
            l_offset += l_ret;\
            l_ret = snprintf(iMeI_sTr + l_offset, MaXlEn - l_offset, "%u", (iMeI_t_PtR)->u.num.cdsd);\
          }\
        }

CORE_DECLARE(status_t) s1ap_build_setup_rsp(pkbuf_t **pkbuf);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

