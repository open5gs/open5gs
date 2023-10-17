Docker running example
===========================================
* Development
  
  `$ docker compose run dev`

* Run WebUI
  
  `$ docker-compose up webui`

* Test

  `$ docker compose run test`

* Test(ubuntu:focal)

  `$ TAG=focal docker compose run test`

* Development(fedora:latest)

  `$ DIST=fedora docker compose run dev`

* All Test with All Environment
  
  `$ ./check.sh`

* Runtime

  `$ docker compose run run`

For OpenSUSE Build Service Release
===========================================

* Build Package
  
  ```
  $ ./build-aux/git-version-gen . > .tarball-version
  $ dpkg-source -b .
  $ rm -f .tarball-version
  ```

* Get Release Key
  
  ```
  $ wget http://download.opensuse.org/repositories/home:/acetcom:/open5gs:/latest/xUbuntu_18.04/Release.key
  $ sudo apt-key add Release.key
  ```

* Setup Repository
  ```
  $ sudo sh -c "echo 'deb https://download.opensuse.org/repositories/home:/acetcom:/open5gs:/latest/xUbuntu_18.04/ ./' > /etc/apt/sources.list.d/open5gs-latest.list"
  $ sudo apt-get update
  ```
  
For Launchpad Release
===========================================

* Run Docker
  
  ```
  $ docker-compose run dev
  ```

* Setup Debian Environment

```
export DEBFULLNAME='Sukchan Lee'
export DEBEMAIL='acetcom@gmail.com'
```

* Transfer GPG key

```
  $ gpg --export-secret-keys --armor --output private.asc
  $ gpg --import private.asc
  $ gpg --export > public.asc
  $ gpg --import public.asc
  ```

* Version Change
  
  ```
  $ cat ./meson.build
  $ cat ./webui/package.json
  $ cat ./webui/package-lock.json
  $ cat ./docs/assets/webui/install
  ```

* New NF
  ```
  $ cat debian/control
  $ cat configs/systemd/open5gs-scpd.service.in
  $ cat configs/logrotate/open5gs.in
  $ cat configs/newsyslog/open5gs.conf.in
  ```

* Upload OpenBuildService
  
  ```
  $ dch -i
  $ meson subprojects download freeDiameter prometheus-client-c
  $ debuild -S -uc -us -d
  $ osc co home:acetcom:open5gs latest
  $ cd home\:acetcom\:open5gs/latest/
  $ cp ~/git/open5gs_1.0.0.* .
  $ osc ci -m "Update it"
  ```

* Upload LaunchPad
  
  ```
  $ dch -i
  $ meson subprojects download freeDiameter prometheus-client-c
  $ debuild -S -d
  $ dput ppa:open5gs/latest *.source.changes
  ```

* Tagging
  
  ```
  $ git tag v1.x.x -a
  ```

* Build package
  
  ```
  $ dpkg-buildpackage -d
  ```
