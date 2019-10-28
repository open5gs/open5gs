#!/bin/sh

for arg in "$@"; do
  if [ "$arg" = "-h" ]; then
    echo "Usage: ./build/dput-upload.sh [PROJECT]"
    echo ""
    echo "  PROJECT: snapshot(=Default), release"
    echo ""
    echo "  This script is used to upload a source package to OBS(OpenSUSE Build Service)."
    echo ""
    echo "  This script must be called from the base directory of the repository, and"
    echo "  requires utilites from these packages: git-buildpackage dput git"
    echo ""
    exit
  fi
done

if [ ! -x /usr/bin/dpkg-parsechangelog ]; then
  echo "Missing the dpkg-parsechangelog utility from the dpkg-dev package"
  exit 1
fi

if [ ! -f debian/changelog ]; then
  echo "This script must be called from the base directory of the package"
  exit 1
fi

if [ ! -d .git ]; then
  echo "This script only works from within a repository"
  exit 1
fi

if [ ! -x /usr/bin/gbp ]; then
  echo "Missing git-buildpackage"
  exit 1
fi

PACKAGE=$(dpkg-parsechangelog | sed -n 's/^Source: //p')
DT="$(date +%Y%m%d)+git$(git rev-parse --short HEAD)"
TOP=$(pwd)

PROJECT=ppa:open5gs/${1-snapshot}

DEB_SRC=$TOP/build/deb-src
DEB_DIR=$DEB_SRC/$PACKAGE

build_package() {
  if [ ! -d $DEB_DIR ]; then
    mkdir -p $DEB_DIR
  fi

  ./build/git-version-gen . > .tarball-version 2>/dev/null
  gbp buildpackage -S -d \
    --git-ignore-branch --git-ignore-new "--git-export-dir=$DEB_DIR" \
    --git-postexport='cp $GBP_GIT_DIR/../.tarball-version $GBP_TMP_DIR/'
  rm -f .tarball-version

  cd $DEB_DIR
  dput $PROJECT *_source.changes

  rm "$DEB_DIR/"*
  rmdir "$DEB_DIR/"
  rmdir "$DEB_SRC/"

  cd $TOP
}

build_package
echo "${1-snapshot} versions of $DT"
