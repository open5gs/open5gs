
* Ubuntu Docker Setup

  $ docker run -p 4000:3000 --hostname build -ti --name build --privileged --cap-add=SYS_ADMIN -e "container=docker" -v /sys/fs/cgroup:/sys/fs/cgroup -v $PWD:/mnt ubuntu /sbin/init

  $ docker exec -it ubuntu /bin/login

* Package Install
  $ sudo apt-get install sudo vim dpkg-dev git
  

* Getting Source Repository
  $ git clone https://github.com/acetcom/nextepc
  $ git checkout new_branch

* Check Pakcage
  $ dpkg-buildpackage
  $ sudo apt-get install ....

* Build Pakcage
  $ dpkg-buildpackage
  $ sudo apt-get install ....
