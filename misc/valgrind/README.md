
* Memory Leak Check
user@host ~/Documents/git/open5gs$ \
    sudo valgrind --leak-check=full --show-leak-kinds=all \
    --suppressions=support/valgrind/mongoc.suppressions \
    --suppressions=support/valgrind/freeDiameter.suppressions \
    ./test/testepc

* Generate Suppression File
user@host ~/Documents/git/open5gs$ \
    sudo valgrind --leak-check=full --show-reachable=yes --error-limit=no \
    --gen-suppressions=all --log-file=support/valgrind/test.log \
    --suppressions=support/valgrind/mongoc.suppressions \
    ./test/testepc

user@host ~/Documents/git/open5gs/support/valgrind$ \
    sudo chown user:user test.log

user@host ~/Documents/git/open5gs/support/valgrind$ \
    cat ./test.log | ./parse_suppressions.sh > test.supp
