#ifndef __APP_INIT_H__
#define __APP_INIT_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* For testing */
void epc_child_terminate(void);
ogs_thread_t *epc_child_create(char *name, char **argv);

char *app_version(void);

int app_initialize(char **argv);
void app_terminate(void);

int mme_initialize();
void mme_terminate(void);

int hss_initialize();
void hss_terminate(void);

int sgw_initialize();
void sgw_terminate(void);

int pgw_initialize();
void pgw_terminate(void);

int pcrf_initialize();
void pcrf_terminate(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __APP_INIT_H__ */
