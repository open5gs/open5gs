#!/bin/sh
TANKER=infra-wireless-sv1-aws-01-dev CONTAINER=epc-smf exec $(dirname $0)/tcpdump-proxy.sh
