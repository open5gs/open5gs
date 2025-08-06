# open5gs modifications

- `Attach complete`
- `Removed Session: UE` or `MME_SESS_CLEAR`


- `src/mme/emm-sm.c` -> `ogs_info("[%s] Attach complete", mme_ue->imsi_bcd);` should also send to some udp ip/port in json we also will want imei from mme_ue->imeisv
- `/src/smf/gn-handler.c` line 306 `ogs_info("UE IMSI[%s] APN[%s] IPv4[%s] IPv6[%s]",` should also emit
- 

