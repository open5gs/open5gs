
* To install JDK(Java)
$ sudo apt install default-jdk

* To install Node.js
$ sudo apt update
$ sudo apt install curl
$ curl -fsSL https://deb.nodesource.com/setup_14.x | sudo -E bash -
$ sudo apt install nodejs

* To install OpenAPI Geneator
$ sudo npm install @openapitools/openapi-generator-cli -g

$ Run generator
$ openapi-generator-cli version-manager set 5.2.0
$ ./generator.sh
