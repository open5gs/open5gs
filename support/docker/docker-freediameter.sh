#!/bin/sh

DOCKER_IMAGE="ubuntu:16.04-build-freediameter"
HOST_OUT_PATH="${PWD}/ubuntu-16.04-build-freediameter.out"

docker build -f "Dockerfile.${DOCKER_IMAGE}" -t ${DOCKER_IMAGE} .
docker run -i -t --rm -v ${HOST_OUT_PATH}:/root/build-freediameter ${DOCKER_IMAGE}
