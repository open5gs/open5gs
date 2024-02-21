#!/bin/sh
exec ssh -t infra-wireless-sv1-aws-01-dev sudo docker exec -it epc-smf tail -F /etc/open5gs/install/var/log/open5gs/smf.log
