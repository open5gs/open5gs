Docker running example
===========================================
* Build Image
  $ docker-compose build

* Development 
  $ docker-compose run dev

* Runtime
  $ docker-compose \
    -f docker-compose.yml -f docker-compose.run.yml run run

* Test
  $ docker-compose \
    -f docker-compose.yml -f docker-compose.test.yml run test

* Test(ubuntu:artful)
  $ TAG=artful docker-compose build
  $ TAG=artful docker-compose \
    -f docker-compose.yml -f docker-compose.test.yml run test

* Development(fedora:latest)
  $ DIST=fedora docker-compose build
  $ DIST=fedora docker-compose run dev

* Runtime(debian:jessie)
  $ DIST=debian TAG=jessie docker-compose build
  $ DIST=debian TAG=jessie docker-compose 
    -f docker-compose.yml -f docker-compose.run.yml run run

* All Test with All Environment
  $ ./check.sh

* Run WebUI 
  $ docker-compose up -d

For Debian Package Release
===========================================

* Run Docker
  $ docker-compose run dev

* Setup Debian Environment
export DEBFULLNAME='Sukchan Lee'
export DEBEMAIL='acetcom@gmail.com'

* Transfer GPG key
  $ gpg --export-secret-keys --armor --output private.asc
  $ gpg --import private.asc
  $ gpg --export > public.asc
  $ gpg --import public.asc

* Upload LaunchPad
  $ dch -i
  $ debuild -S
  $ dput ppa:acetcom/nextepc *.source.changes

* Build pckage
  $ dpkg-buildpackage

For Mac OS X (SCTP-patch)
===========================================
* /Applications/Docker.app/Contents/Resources/moby
Update linuxkit-kernel, vmlinuz64
