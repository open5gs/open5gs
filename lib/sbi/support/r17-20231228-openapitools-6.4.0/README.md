
### Ubuntu

* To install JDK(Java)
$ sudo apt install default-jdk

* To install Node.js
$ sudo apt update
$ sudo apt install curl
$ curl -fsSL https://deb.nodesource.com/setup_18.x | sudo -E bash -
$ sudo apt install nodejs

* To install OpenAPI Geneator
$ sudo npm install @openapitools/openapi-generator-cli -g

$ Run generator
$ sudo openapi-generator-cli version-manager set 6.4.0
$ ./generator.sh


### MacOSX

* To install JDK(Java)
$ brew install openjdk

* To install Node.js
$ brew install node

* To install OpenAPI Geneator
$ npm install @openapitools/openapi-generator-cli -g

$ Run generator
$ openapi-generator-cli version-manager set 6.4.0
$ ./generator.sh


### How to upgrade SBI

1. Download the *.yaml from etsi.org
2. Copy *.yaml to standard/modified directory
3. Run ./generator.sh and check for openapi-generator-cli errors
4. If you copy the entire OLD version without ERROR to modified,
   you can identify the problem.
5. Check the *.yaml below in the OLD version to apply the changes.

./modified/TS29508_Nsmf_EventExposure.yaml
./modified/TS32291_Nchf_ConvergedCharging.yaml
./modified/TS29519_Exposure_Data.yaml

./modified/TS29517_Naf_EventExposure.yaml
./modified/TS29521_Nbsf_Management.yaml
./modified/TS29531_Nnssf_NSSelection.yaml
./modified/TS29507_Npcf_AMPolicyControl.yaml
./modified/TS29512_Npcf_SMPolicyControl.yaml
./modified/TS29514_Npcf_PolicyAuthorization.yaml
./modified/TS29503_Nudm_SDM.yaml
./modified/TS29503_Nudm_UEAU.yaml
./modified/TS29503_Nudm_UECM.yaml
./modified/TS29509_Nausf_UEAuthentication.yaml
./modified/TS29502_Nsmf_PDUSession.yaml
./modified/TS29518_Namf_Communication.yaml
./modified/TS29510_Nnrf_NFManagement.yaml
./modified/TS29505_Subscription_Data.yaml
./modified/TS29571_CommonData.yaml

./modified/TS29520_Nnwdaf_EventsSubscription.yaml
./modified/TS29507_Npcf_AMPolicyControl.yaml
./modified/TS29512_Npcf_SMPolicyControl.yaml
./modified/TS29503_Nudm_SDM.yaml
./modified/TS29518_Namf_Communication.yaml
./modified/TS29510_Nnrf_NFManagement.yaml
./modified/TS29505_Subscription_Data.yaml
./modified/TS29571_CommonData.yaml

