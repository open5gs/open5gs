#!/bin/sh
exec ssh -t infra-wireless-dc2-aws-03-prod sudo docker exec -it --workdir /poc expeto-pgw-710d3ef28c bash
