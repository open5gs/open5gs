#!/bin/sh
exec ssh -t infra-wireless-sv1-aws-01-dev sudo docker exec -it --workdir /etc/open5gs/install epc-pcrf bash
