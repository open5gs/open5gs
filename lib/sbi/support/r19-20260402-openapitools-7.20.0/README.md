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
$ python3 transform.py ./patched ./modified
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
$ python3 transform.py ./patched ./modified
$ ./generator.sh


### How to upgrade SBI

1. Download the *.yaml from etsi.org
2. Copy *.yaml to standard directory
3. The patched directory contains diffs of hand-edited changes
   from the original ETSI YAML files in the standard directory.
   Apply similar modifications to the new version.
4. Run python3 transform.py ./patched ./modified
   to generate the modified directory.
5. Run ./generator.sh and check for openapi-generator-cli errors
6. If you copy the entire OLD version without ERROR to standard,
   you can identify the problem.

./standard/TS29573_N32_Handshake.yaml
./standard/TS29504_Nudr_DR.yaml
./standard/TS29521_Nbsf_Management.yaml
./standard/TS29531_Nnssf_NSSelection.yaml
./standard/TS29507_Npcf_AMPolicyControl.yaml
./standard/TS29512_Npcf_SMPolicyControl.yaml
./standard/TS29514_Npcf_PolicyAuthorization.yaml
./standard/TS29503_Nudm_SDM.yaml
./standard/TS29503_Nudm_UECM.yaml
./standard/TS29503_Nudm_UEAU.yaml
./standard/TS29509_Nausf_UEAuthentication.yaml
./standard/TS29502_Nsmf_PDUSession.yaml
./standard/TS29518_Namf_Communication.yaml
./standard/TS29510_Nnrf_NFDiscovery.yaml
./standard/TS29510_Nnrf_NFManagement.yaml
