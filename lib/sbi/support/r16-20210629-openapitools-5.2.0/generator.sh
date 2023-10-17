#!/bin/sh

###  Ubuntu 20.04(focal) is not working
#export C_POST_PROCESS_FILE="/usr/bin/uncrustify --no-backup"
#export UNCRUSTIFY_CONFIG="./openapi-generator/uncrustify-rules.cfg"

openapi_generator_cli="openapi-generator-cli"

$openapi_generator_cli generate -i ./modified/TS29521_Nbsf_Management.yaml -c ./openapi-generator/config.yaml -g c -o ../../openapi
$openapi_generator_cli generate -i ./modified/TS29531_Nnssf_NSSelection.yaml -c ./openapi-generator/config.yaml -g c -o ../../openapi
$openapi_generator_cli generate -i ./modified/TS29507_Npcf_AMPolicyControl.yaml -c ./openapi-generator/config.yaml -g c -o ../../openapi
$openapi_generator_cli generate -i ./modified/TS29512_Npcf_SMPolicyControl.yaml -c ./openapi-generator/config.yaml -g c -o ../../openapi
$openapi_generator_cli generate -i ./modified/TS29503_Nudm_SDM.yaml -c ./openapi-generator/config.yaml -g c -o ../../openapi
$openapi_generator_cli generate -i ./modified/TS29503_Nudm_UECM.yaml -c ./openapi-generator/config.yaml -g c -o ../../openapi
$openapi_generator_cli generate -i ./modified/TS29503_Nudm_UEAU.yaml -c ./openapi-generator/config.yaml -g c -o ../../openapi
$openapi_generator_cli generate -i ./modified/TS29509_Nausf_UEAuthentication.yaml -c ./openapi-generator/config.yaml -g c -o ../../openapi
$openapi_generator_cli generate -i ./modified/TS29510_Nnrf_NFDiscovery.yaml -c ./openapi-generator/config.yaml -g c -o ../../openapi
$openapi_generator_cli generate -i ./modified/TS29510_Nnrf_NFManagement.yaml -c ./openapi-generator/config.yaml -g c -o ../../openapi
$openapi_generator_cli generate -i ./modified/TS29504_Nudr_DR.yaml -c ./openapi-generator/config.yaml -g c -o ../../openapi
$openapi_generator_cli generate -i ./modified/TS29502_Nsmf_PDUSession.yaml -c ./openapi-generator/config.yaml -g c -o ../../openapi
$openapi_generator_cli generate -i ./modified/TS29518_Namf_Communication.yaml -c ./openapi-generator/config.yaml -g c -o ../../openapi

# 'AfEvent' Model was duplicated in TS29514_Npcf_PolicyAuthorization and
# TS29517_Naf_EventExposure.yaml.
# You should put TS29514_Npcf_PolicyAuthorization bottom to use 'AfEvent'
# in TS29514_Npcf_PolicyAuthorization,
$openapi_generator_cli generate -i ./modified/TS29514_Npcf_PolicyAuthorization.yaml -c ./openapi-generator/config.yaml -g c -o ../../openapi
