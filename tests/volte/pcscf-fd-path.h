#ifndef __PCSCF_FD_PATH_H__
#define __PCSCF_FD_PATH_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int pcscf_fd_init(void);
void pcscf_fd_final(void);

void pcscf_rx_send_aar(uint8_t **rx_sid, const char *ip,
        int qos_type, int flow_presence);
void pcscf_rx_send_str(uint8_t *rx_sid);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __PCSCF_FD_PATH_H__ */

