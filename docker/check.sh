#!/bin/sh

docker-compose build
docker-compose -f docker-compose.yml -f docker-compose.test.yml run --rm test

TAG=bionic docker-compose build
TAG=bionic docker-compose -f docker-compose.yml -f docker-compose.test.yml run --rm test

DIST=fedora TAG=32 docker-compose build
DIST=fedora TAG=32 docker-compose -f docker-compose.yml -f docker-compose.test.yml run --rm test

DIST=debian docker-compose build
DIST=debian docker-compose -f docker-compose.yml -f docker-compose.test.yml run --rm test

docker rm $(docker ps -qa --no-trunc --filter "status=exited")
