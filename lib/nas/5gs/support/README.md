
* Install python3-docx
user@host ~/Documents/git/open5gs/lib/nas/5gs/support$ \
    sudo apt install python3-docx

 Adjust table cell in 24301-j62.docx

* Generate Message support files
user@host ~/Documents/git/open5gs/lib/nas/5gs/support$ \
    python3 nas-message.py -f r19.6.2/24501-j62.docx -o ..

* Check lib/nas/5gs/decoder.c
$ diff --git a/lib/nas/5gs/decoder.c b/lib/nas/5gs/decoder.c
index c03e529ec..f471f294b 100644
--- a/lib/nas/5gs/decoder.c
+++ b/lib/nas/5gs/decoder.c
@@ -968,7 +968,6 @@ int ogs_nas_5gs_decode_registration_accept(ogs_nas_5gs_message_t *message, ogs_p
             registration_accept->presencemask |= OGS_NAS_5GS_REGISTRATION_ACCEPT_NEGOTIATED_PEIPS_ASSISTANCE_INFORMATION_PRESENT;
             decoded += size;
             break;
-#if 0 /* Modified by acetcom */
         case OGS_NAS_5GS_REGISTRATION_ACCEPT_5GS_ADDITIONAL_REQUEST_RESULT_TYPE:
             size = ogs_nas_5gs_decode_5gs_additional_request_result(&registration_accept->additional_request_result, pkbuf);
             if (size < 0) {
@@ -979,7 +978,6 @@ int ogs_nas_5gs_decode_registration_accept(ogs_nas_5gs_message_t *message, ogs_p
             registration_accept->presencemask |= OGS_NAS_5GS_REGISTRATION_ACCEPT_5GS_ADDITIONAL_REQUEST_RESULT_PRESENT;
             decoded += size;
             break;
-#endif
         case OGS_NAS_5GS_REGISTRATION_ACCEPT_NSSRG_INFORMATION_TYPE:
             size = ogs_nas_5gs_decode_nssrg_information(&registration_accept->nssrg_information, pkbuf);
             if (size < 0) {

* Add new structure to the types.h
