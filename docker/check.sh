#!/bin/sh

docker compose run --rm test
TAG=focal docker compose run --rm test
DIST=fedora docker compose run --rm test
DIST=debian docker compose run --rm test
#DIST=alpine docker compose run --rm test

docker rm $(docker ps -qa --no-trunc --filter "status=exited")
