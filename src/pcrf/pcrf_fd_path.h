#ifndef __PCRF_FD_PATH_H__
#define __PCRF_FD_PATH_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

struct sess_state;
typedef struct _rx_message_t rx_message_t;

int pcrf_fd_init(void);
void pcrf_fd_final(void);

int pcrf_gx_init(void);
void pcrf_gx_final(void);
int pcrf_rx_init(void);
void pcrf_rx_final(void);

int pcrf_gx_send_rar(
        uint8_t *gx_sid, uint8_t *rx_sid, rx_message_t *rx_message);
int pcrf_rx_send_asr(
        uint8_t *rx_sid, uint32_t abort_cause);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __PCRF_FD_PATH_H__ */

