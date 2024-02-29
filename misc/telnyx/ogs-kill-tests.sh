#!/bin/sh
ps aux | grep -E '([o]pen5gs-\w+|gtp-proxy)d .* -c .*/([a-z-]*sample|gtp-proxy)\.yaml' | awk '{ print $2 }' | xargs -r kill

