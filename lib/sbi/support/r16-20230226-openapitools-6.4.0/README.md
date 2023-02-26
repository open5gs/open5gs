
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
