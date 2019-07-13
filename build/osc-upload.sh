#!/bin/sh

for arg in "$@"; do
  if [ "$arg" = "-h" ]; then
    echo "Usage: ./build/osc-upload.sh [PROJECT]"
    echo ""
    echo "  PROJECT: snapshot(=Default), release"
    echo ""
    echo "  This script is used to upload a source package to OBS(OpenSUSE Build Service)."
    echo ""
    echo "  This script must be called from the base directory of the repository, and"
    echo "  requires utilites from these packages: git-buildpackage osc git"
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

OGS_REPO=$TOP/build/obs-repo
PROJECT=home:acetcom:open5gs:${1-snapshot}
OBS_DIR=$OGS_REPO/$PROJECT/$PACKAGE

prepare() {
  if [ -d $OBS_DIR ]; then
    (cd $OBS_DIR && osc up)
  else
    mkdir -p $OGS_REPO
    (cd $OGS_REPO && osc co $PROJECT $PACKAGE)
  fi
}

DEB_SRC=$TOP/build/deb-src
DEB_DIR=$DEB_SRC/$PACKAGE

build_package() {
  if [ ! -d $DEB_DIR ]; then
    mkdir -p $DEB_DIR
  fi

  ./build/git-version-gen . > .tarball-version 2>/dev/null
  gbp buildpackage -S --no-sign -d \
    --git-ignore-branch --git-ignore-new "--git-export-dir=$DEB_DIR" \
    --git-postexport='cp $GBP_GIT_DIR/../.tarball-version $GBP_TMP_DIR/'
  rm -f .tarball-version

  cd $OBS_DIR

  file=$(cd "$DEB_DIR/" ; ls ./*.dsc)
  echo $file
  if [ -e "$file" ] ; then
    osc rm ./* || true
  fi

  mv "$DEB_DIR/"*.dsc .
  mv "$DEB_DIR/"*.tar* .
  osc add ./*

  rm "$DEB_DIR/"*
  rmdir "$DEB_DIR/"
  rmdir "$DEB_SRC/"

  cd $TOP
}

prepare
build_package

(cd $OBS_DIR && osc ci -m "Snapshot versions of $DT")
