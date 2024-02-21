#!/bin/sh
exec ssh -t infra-wireless-dc2-aws-03-prod sudo docker exec -it expeto-pgw-710d3ef28c tail -F /poc/logs/gtp-proxy-stdout.log /poc/logs/sgwu-stdout.log
