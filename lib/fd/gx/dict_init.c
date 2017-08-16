#include "gx_lib.h"

#define CHECK_dict_search( _type, _criteria, _what, _result )	\
	CHECK_FCT(  fd_dict_search( fd_g_config->cnf_dict, (_type), (_criteria), (_what), (_result), ENOENT) );

struct dict_object *gx_application = NULL;

struct dict_object *gx_cmd_ccr = NULL;
struct dict_object *gx_cmd_cca = NULL;

int gx_dict_init(void)
{
    application_id_t id = 16777238;

    CHECK_dict_search( DICT_APPLICATION, APPLICATION_BY_ID, (void *)&id, &gx_application);

    CHECK_dict_search( DICT_COMMAND, CMD_BY_NAME, "Credit-Control-Request", &gx_cmd_ccr);
    CHECK_dict_search( DICT_COMMAND, CMD_BY_NAME, "Credit-Control-Answer", &gx_cmd_cca);

    return 0;
}
