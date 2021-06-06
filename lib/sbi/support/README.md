
* You can use the built package.

$ sudo apt install default-jdk maven cmake uncrustify
$ npm install @openapitools/openapi-generator-cli -g



* Open5GS uses the source code instead of packages.

- VERSION: 4.3.1-SNAPSHOT
$ git clone https://github.com/OpenAPITools/openapi-generator.git
$ cd openapi-generator
$ git checkout 15d58dc89b11d17f52c2daadfae9736e442f3b1a
$ mvn clean package

- VERSION: 0.66.1
$ git clone https://github.com/uncrustify/uncrustify.git
$ cd uncrustify
$ git checkout uncrustify-0.66.1
$ mkdir build
$ cd build
$ cmake ..
$ make -j 8

