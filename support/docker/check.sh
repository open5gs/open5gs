#!/bin/sh

docker-compose up -d
docker-compose -f docker-compose.yml -f docker-compose.test.yml run --rm test
docker rm $(docker ps -qa --no-trunc --filter "status=exited")

TAG=artful docker-compose up -d
TAG=artful docker-compose -f docker-compose.yml -f docker-compose.test.yml run --rm test
docker rm $(docker ps -qa --no-trunc --filter "status=exited")
