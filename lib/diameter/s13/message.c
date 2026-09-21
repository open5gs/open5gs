#include "ogs-diameter-s13.h"

#define CHECK_dict_search( _type, _criteria, _what, _result )    \
    CHECK_FCT(  fd_dict_search( fd_g_config->cnf_dict, (_type), (_criteria), (_what), (_result), ENOENT) );

struct dict_object *ogs_diam_s13_application = NULL;

struct dict_object *ogs_diam_s13_cmd_ecr = NULL;
struct dict_object *ogs_diam_s13_cmd_eca = NULL;

struct dict_object *ogs_diam_s13_terminal_information = NULL;
struct dict_object *ogs_diam_s13_imei = NULL;
struct dict_object *ogs_diam_s13_software_version = NULL;
struct dict_object *ogs_diam_s13_equipment_status = NULL;

extern int ogs_dict_s13_entry(char *conffile);

int ogs_diam_s13_init(void)
{
    application_id_t id = OGS_DIAM_S13_APPLICATION_ID;

    ogs_assert(ogs_dict_s13_entry(NULL) == 0);

    CHECK_dict_search(DICT_APPLICATION, APPLICATION_BY_ID, (void *)&id, &ogs_diam_s13_application);

    CHECK_dict_search(DICT_COMMAND, CMD_BY_NAME, "ME-Identity-Check-Request", &ogs_diam_s13_cmd_ecr);
    CHECK_dict_search(DICT_COMMAND, CMD_BY_NAME, "ME-Identity-Check-Answer", &ogs_diam_s13_cmd_eca);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Terminal-Information", &ogs_diam_s13_terminal_information);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "IMEI", &ogs_diam_s13_imei);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Software-Version", &ogs_diam_s13_software_version);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Equipment-Status", &ogs_diam_s13_equipment_status);

    return 0;
}

static bool s13_all_digits(const char *s, size_t len)
{
    size_t i;

    if (!s)
        return false;
    for (i = 0; i < len; i++)
        if (s[i] < '0' || s[i] > '9')
            return false;

    return true;
}

/* Luhn check digit over the 14 IMEI digits (TS 23.003 Annex B) */
static int s13_imei_check_digit(const char *imei)
{
    int i, sum = 0;

    for (i = 0; i < OGS_DIAM_S13_IMEI_LEN; i++) {
        int digit = imei[i] - '0';

        if (i % 2 == 1) {
            digit *= 2;
            if (digit > 9)
                digit -= 9;
        }
        sum += digit;
    }

    return (10 - (sum % 10)) % 10;
}

int ogs_diam_s13_pei_from_terminal_info(
        const char *imei, size_t imei_len, const char *svn, size_t svn_len,
        char *pei, size_t pei_len)
{
    int n;

    ogs_assert(pei);
    ogs_assert(pei_len > 0);
    pei[0] = 0;

    if (imei_len != OGS_DIAM_S13_IMEI_LEN || !s13_all_digits(imei, imei_len)) {
        ogs_error("Invalid IMEI in Terminal-Information [len:%u]",
                (unsigned)imei_len);
        return OGS_ERROR;
    }

    if (svn) {
        if (svn_len != OGS_DIAM_S13_SVN_LEN || !s13_all_digits(svn, svn_len)) {
            ogs_error("Invalid Software-Version in Terminal-Information "
                    "[len:%u]", (unsigned)svn_len);
            return OGS_ERROR;
        }
        n = ogs_snprintf(pei, pei_len, "%s-%.*s%.*s",
                OGS_ID_SUPI_TYPE_IMEISV,
                (int)imei_len, imei, (int)svn_len, svn);
    } else {
        n = ogs_snprintf(pei, pei_len, "%s-%.*s%d",
                OGS_ID_PEI_TYPE_IMEI,
                (int)imei_len, imei, s13_imei_check_digit(imei));
    }

    if (n < 0 || (size_t)n >= pei_len) {
        ogs_error("PEI buffer too small [%u]", (unsigned)pei_len);
        pei[0] = 0;
        return OGS_ERROR;
    }

    return OGS_OK;
}
