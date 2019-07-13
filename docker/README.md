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

For OpenSUSE Build Service Release
===========================================

* Build Package
  $ ./build-aux/git-version-gen . > .tarball-version
  $ dpkg-source -b .
  $ rm -f .tarball-version

* Get Release Key
  $ wget http://download.opensuse.org/repositories/home:/acetcom:/open5gs:/latest/xUbuntu_18.04/Release.key
  $ sudo apt-key add Release.key

* Setup Repository
  $ sudo sh -c "echo 'deb https://download.opensuse.org/repositories/home:/acetcom:/open5gs:/latest/xUbuntu_18.04/ ./' > /etc/apt/sources.list.d/open5gs-latest.list"
  $ sudo apt-get update
  
For Launchpad Release
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

