Debian Pakcage 
===========================================

* Ubuntu Docker Setup

  $ docker run -p 4000:3000 --hostname build -ti --name build --privileged --cap-add=SYS_ADMIN -e "container=docker" -v /sys/fs/cgroup:/sys/fs/cgroup -v $PWD:/mnt ubuntu /sbin/init
  $ docker exec -it ubuntu /bin/login

* Access Source Repository
  $ sudo apt-get install sudo vim dpkg-dev git
  $ git clone https://github.com/acetcom/nextepc
  $ git checkout new_branch

* Check Pakcage
  $ dpkg-buildpackage
  $ sudo apt-get install ....
  $ dpkg-buildpackage

* Setup Debian Environment
export DEBFULLNAME='Sukchan Lee'
export DEBEMAIL='acetcom@gmail.com'

* Transfer GPG key
  $ gpg --export-secret-keys --armor --output private.asc'
  $ gpg --import private.asc
  $ gpg --export > public.asc'
  $ gpg --import public.asc

* Update debian/changelog
  $ sudo apt-get install devscripts
  $ dch -i

* Test OBS
  $ debuild -S
  - Upload *.dsc and *.tar.gz to https://build.opensuse.org/package/show/home:acetcom/nextepc

* Upload LaunchPad
  $ dput ppa:acetcom/nextepc *.source.changes


Docker Tun Configuration
===========================================
* sudo sysctl -w net.ipv6.conf.all.disable_ipv6=0
* sudo chmod 666 /dev/net/tun

