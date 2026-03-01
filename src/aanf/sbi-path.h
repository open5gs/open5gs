 #ifndef AANF_SBI_PATH_H
 #define AANF_SBI_PATH_H
 
 #ifdef __cplusplus
 extern "C" {
 #endif
 
 int annf_sbi_open(void);
 void annf_sbi_close(void);
 
 bool annf_sbi_send_request(
         ogs_sbi_nf_instance_t *nf_instance, ogs_sbi_xact_t *xact);

//  int ausf_sbi_discover_and_send(
//          ogs_sbi_service_type_e service_type,
//          ogs_sbi_discovery_option_t *discovery_option,
//          ogs_sbi_request_t *(*build)(ausf_ue_t *ausf_ue, void *data),
//          ausf_ue_t *ausf_ue, ogs_sbi_stream_t *stream, void *data);
 
 #ifdef __cplusplus
 }
 #endif
 
 #endif /* AUSF_SBI_PATH_H */
 