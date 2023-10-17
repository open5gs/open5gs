
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
