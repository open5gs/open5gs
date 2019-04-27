#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct app_param_s {
    const char *name;

    const char *config_path;
    const char *log_path;
    const char *pid_path;

    bool logfile_disabled;
    bool db_disabled;

    ogs_log_level_e log_level;
    const char *log_domain;
} app_param_t;

int app_will_initialize(app_param_t *param);
int app_did_initialize(void);
void app_will_terminate(void);
void app_did_terminate(void);

int config_initialize(const char *config_path);
void config_terminate(void);

int app_logger_restart(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __APPLICATION_H__ */
