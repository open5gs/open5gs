#!/bin/sh
docker build -t ubuntu:16.04-freediameter .
docker run -v $PWD:/root ubuntu:16.04-freediameter
