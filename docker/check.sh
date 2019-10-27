#!/bin/sh

docker-compose build
docker-compose -f docker-compose.yml -f docker-compose.test.yml run --rm test

TAG=eoan docker-compose build
TAG=eoan docker-compose -f docker-compose.yml -f docker-compose.test.yml run --rm test

DIST=fedora docker-compose build
DIST=fedora docker-compose -f docker-compose.yml -f docker-compose.test.yml run --rm test

DIST=fedora TAG=29 docker-compose build
DIST=fedora TAG=29 docker-compose -f docker-compose.yml -f docker-compose.test.yml run --rm test

DIST=centos docker-compose build
DIST=centos docker-compose -f docker-compose.yml -f docker-compose.test.yml run --rm test

DIST=centos TAG=7 docker-compose build
DIST=centos TAG=7 docker-compose -f docker-compose.yml -f docker-compose.test.yml run --rm test

DIST=debian docker-compose build
DIST=debian docker-compose -f docker-compose.yml -f docker-compose.test.yml run --rm test

DIST=debian TAG=stretch docker-compose build
DIST=debian TAG=stretch docker-compose -f docker-compose.yml -f docker-compose.test.yml run --rm test

docker rm $(docker ps -qa --no-trunc --filter "status=exited")
