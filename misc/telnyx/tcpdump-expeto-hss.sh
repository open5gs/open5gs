#!/bin/sh
TANKER=infra-wireless-dc2-aws-02-prod CONTAINER=expeto-epc FILTER='sctp or udp' exec $(dirname $0)/tcpdump-proxy.sh
