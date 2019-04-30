#ifndef __APP_INIT_H__
#define __APP_INIT_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int app_initialize(app_param_t *param);
#define app_terminate epc_terminate

int epc_initialize(app_param_t *param);
void epc_terminate(void);

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
