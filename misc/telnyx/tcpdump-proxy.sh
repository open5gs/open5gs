#!/bin/sh
# WARNING: THIS LEAVES DANGLING tcpdump processes in the container
# ALWAYS MAKE SURE TO CLEAN UP AFTERWARDS
#   https://superuser.com/a/1557724
#   https://gist.github.com/SkyperTHC/cb4ebb633890ac36ad86e80c6c7a9bb2

TANKER=${TANKER:-infra-wireless-dc2-aws-03-prod}
CONTAINER=${CONTAINER:-expeto-pgw-710d3ef28c}
# Note: Quoting is extremely tricky in TCPDUMP_FLAGS
TCPDUMP_FLAGS='-s0 -qnUw - -i any'
TCPDUMP_FILTER=${FILTER:-udp}

cleanup() {
    >&2 echo "\n*** Please wait. Cleaning up the runaway tcpdump processes in the Docker container"
    ssh -t "$TANKER" sudo docker exec -it "$CONTAINER" bash -c "\"ps aux | grep -E '[t]cpdump(.bin)? $TCPDUMP_FLAGS' | awk '{ print \\\$2 }' | xargs -rn1 kill\""
}

trap cleanup EXIT INT HUP TERM PIPE

ssh "$TANKER" sudo docker exec "$CONTAINER" tcpdump $TCPDUMP_FLAGS "$TCPDUMP_FILTER"
