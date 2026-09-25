


#if !defined(OGS_DIAMETER_INSIDE) && !defined(OGS_DIAMETER_COMPILATION)
#error "This header cannot be included directly."
#endif

#ifndef OGS_DIAM_S13_MESSAGE_H
#define OGS_DIAM_S13_MESSAGE_H

#ifdef __cplusplus
extern "C" {
#endif

/* RFC 5516 / TS 29.272 §6) */
#define OGS_DIAM_S13_APPLICATION_ID 16777252

#define OGS_DIAM_S13_EQUIPMENT_WHITELIST                (0)
#define OGS_DIAM_S13_EQUIPMENT_BLACKLIST                (1)
#define OGS_DIAM_S13_EQUIPMENT_GREYLIST                 (2)

extern struct dict_object *ogs_diam_s13_application;

extern struct dict_object *ogs_diam_s13_cmd_ecr;
extern struct dict_object *ogs_diam_s13_cmd_eca;

extern struct dict_object *ogs_diam_s13_terminal_information;
extern struct dict_object *ogs_diam_s13_imei;
extern struct dict_object *ogs_diam_s13_software_version;
extern struct dict_object *ogs_diam_s13_equipment_status;

typedef struct ogs_diam_s13_eca_message_s {
    uint32_t equipment_status_code;
} ogs_diam_s13_eca_message_t;

/* RFC 5516 / TS 29.272 §6) */
typedef struct ogs_diam_s13_message_s {
#define OGS_DIAM_S13_CMD_CODE_ME_IDENTITY_CHECK             324
#define OGS_DIAM_S13_ERROR_EQUIPMENT_UNKNOWN                5422
    uint16_t                   cmd_code;
    uint32_t                   result_code;
    uint32_t                   *err;
    uint32_t                   *exp_err;
    ogs_diam_s13_eca_message_t eca_message;
} ogs_diam_s13_message_t;

#define OGS_DIAM_S13_IMEI_LEN           14  /* TAC + SNR, no check digit */
#define OGS_DIAM_S13_SVN_LEN            2
/* "imeisv-" + 16 digits */
#define OGS_DIAM_S13_MAX_PEI_LEN \
    (sizeof(OGS_ID_SUPI_TYPE_IMEISV "-") - 1 + OGS_MAX_IMEISV_BCD_LEN)

/*
 * PEI string, as stored in the EIR database, out of the Terminal-Information
 * contents (TS 29.272 7.3.53 and 7.3.54): IMEI (14 digits) + Software-Version
 * (2 digits) gives "imeisv-<16 digits>", an IMEI alone gives
 * "imei-<15 digits>" with the Luhn check digit appended.
 * Returns OGS_ERROR when the input is not usable.
 */
int ogs_diam_s13_pei_from_terminal_info(
        const char *imei, size_t imei_len, const char *svn, size_t svn_len,
        char *pei, size_t pei_len);

int ogs_diam_s13_init(void);

#ifdef __cplusplus
}
#endif

#endif /* OGS_DIAM_S13_MESSAGE_H */