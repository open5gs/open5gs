#!/usr/bin/env sh
pkill -f '([o]pen5gs-\w+|gtp-proxy)d .* -c .*/build/configs/.*\.yaml'
