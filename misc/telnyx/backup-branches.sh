#!/bin/sh

TIMESTAMP=$(date +'%Y%m%d%H%M')

git tag backup/main-${TIMESTAMP} telnyx/main
git push origin 'refs/tags/backup/*'
