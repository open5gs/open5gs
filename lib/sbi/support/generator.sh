#!/bin/sh

###  Ubuntu 20.04(focal) is not working
#export C_POST_PROCESS_FILE="/usr/bin/uncrustify --no-backup"

export C_POST_PROCESS_FILE="${HOME}/Documents/git/my/uncrustify/build/uncrustify --no-backup"
export UNCRUSTIFY_CONFIG=../openapi/.openapi-generator/uncrustify-0.66.1.cfg
openapi_generator_cli="java -jar ${HOME}/Documents/git/my/openapi-generator/modules/openapi-generator-cli/target/openapi-generator-cli.jar"

$openapi_generator_cli generate -i ./modified/TS29507_Npcf_AMPolicyControl.yaml -c ../openapi/.openapi-generator/config.yaml -g c -o ../openapi
$openapi_generator_cli generate -i ./modified/TS29512_Npcf_SMPolicyControl.yaml -c ../openapi/.openapi-generator/config.yaml -g c -o ../openapi
$openapi_generator_cli generate -i ./modified/TS29503_Nudm_SDM.yaml -c ../openapi/.openapi-generator/config.yaml -g c -o ../openapi
$openapi_generator_cli generate -i ./modified/TS29503_Nudm_UECM.yaml -c ../openapi/.openapi-generator/config.yaml -g c -o ../openapi
$openapi_generator_cli generate -i ./modified/TS29503_Nudm_UEAU.yaml -c ../openapi/.openapi-generator/config.yaml -g c -o ../openapi
$openapi_generator_cli generate -i ./modified/TS29509_Nausf_UEAuthentication.yaml -c ../openapi/.openapi-generator/config.yaml -g c -o ../openapi
$openapi_generator_cli generate -i ./modified/TS29510_Nnrf_NFDiscovery.yaml -c ../openapi/.openapi-generator/config.yaml -g c -o ../openapi
$openapi_generator_cli generate -i ./modified/TS29510_Nnrf_NFManagement.yaml -c ../openapi/.openapi-generator/config.yaml -g c -o ../openapi
$openapi_generator_cli generate -i ./modified/TS29504_Nudr_DR.yaml -c ../openapi/.openapi-generator/config.yaml -g c -o ../openapi
$openapi_generator_cli generate -i ./modified/TS29502_Nsmf_PDUSession.yaml -c ../openapi/.openapi-generator/config.yaml -g c -o ../openapi
$openapi_generator_cli generate -i ./modified/TS29518_Namf_Communication.yaml -c ../openapi/.openapi-generator/config.yaml -g c -o ../openapi
