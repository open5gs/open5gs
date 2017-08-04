
* Network Configuration
user@host ~/Documents/git/nextepc/support$ \
    sudo ./linux_netconfig.sh

* Generate Key & Cert for Diameter
user@host ~/Documents/git/nextepc/support$ \
    ./make_certs.sh ./freeDiameter

* Memory Leak Check
user@host ~/Documents/git/nextepc$ \
    sudo valgrind --leak-check=full --show-leak-kinds=all \
    --suppressions=support/valgrind/mongoc.suppressions \
    --suppressions=support/valgrind/freeDiameter.suppressions \
    ./epcd

* Generate Suppression File
user@host ~/Documents/git/nextepc$ \
    sudo valgrind --leak-check=full --show-reachable=yes --error-limit=no \
    --gen-suppressions=all --log-file=support/valgrind/test.log \
    ./epcd

user@host ~/Documents/git/nextepc/support/valgrind$ \
    sudo chown user:user test.log

user@host ~/Documents/git/nextepc/support/valgrind$ \
    cat ./test.log | ./parse_suppressions.sh > test.supp
