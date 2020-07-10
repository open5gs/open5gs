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
  $ TAG=eoan docker-compose build
  $ TAG=eoan docker-compose \
    -f docker-compose.yml -f docker-compose.test.yml run test

* Development(fedora:latest)
  $ DIST=fedora docker-compose build
  $ DIST=fedora docker-compose run dev

* Runtime(debian:jessie)
  $ DIST=debian TAG=stretch docker-compose build
  $ DIST=debian TAG=stretch docker-compose 
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

* Version Change
  $ vi meson.build
  $ ./misc/git-version-gen . > .tarball-version 2>/dev/null

* Upload OpenBuildService
  $ dch -i
  $ meson subprojects download freeDiameter
  $ debuild -S -uc -us -d
  $ osc co home:acetcom:open5gs latest
  $ cd home\:acetcom\:open5gs/latest/
  $ cp ~/git/open5gs_1.0.0.* .
  $ osc ci -m "Update it"

* Upload LaunchPad
  $ dch -i
  $ meson subprojects download freeDiameter
  $ debuild -S -d
  $ dput ppa:open5gs/latest *.source.changes

* Tagging
  $ git tag v1.x.x -a

* Build package
  $ dpkg-buildpackage -d

