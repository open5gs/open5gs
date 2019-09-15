---
title: CentOS
head_inline: "<style> .blue { color: blue; } </style>"
---

This guide is based on **CentOS 7** Distribution.
{: .blue}

### Getting MongoDB
---

Create the MongoDB repository file.
```bash
$ sudo sh -c 'cat << EOF > /etc/yum.repos.d/mongodb-org-3.4.repo
[mongodb-org-3.4]
name=MongoDB Repository
baseurl=https://repo.mongodb.org/yum/redhat/\$releasever/mongodb-org/3.4/x86_64/
gpgcheck=1
enabled=1
gpgkey=https://www.mongodb.org/static/pgp/server-3.4.asc
EOF'
```

Install MongoDB with Package Manager.
```bash
sudo yum -y install mongodb-org
sudo systemctl start mongod (if '/usr/bin/mongod' is not running)
```

### Setting up TUN device (No persistent after rebooting)
---

Create the TUN device. Interface name will be `pgwtun`.
```bash
$ sudo yum -y install iproute
$ sudo ip tuntap add name pgwtun mode tun
$ ip link show
```

Then, to support IPv6-enabled UEs, you must configure your TUN device to support IPv6.

```bash
$ sysctl -n net.ipv6.conf.pgwtun.disable_ipv6
1

$ sudo -w net.ipv6.conf.pgwtun.disable_ipv6=0

$ sysctl -n net.ipv6.conf.pgwtun.disable_ipv6
0
```

**Note:** If your TUN device already supports IPv6, you can skip this steps above.
{: .notice--warning}

You are now ready to set the IP address on TUN device. 

```bash
$ sudo ip addr add 45.45.0.1/16 dev pgwtun
$ sudo ip addr add cafe::1/64 dev pgwtun
```

Make sure it is set up properly.
```bash
$ sudo ip link set pgwtun up
$ ip link show
```

**Tip:** The script provided in [$GIT_REPO/support/network/restart.sh](https://github.com/{{ site.github_username }}/nextepc/blob/master/support/network/restart.sh) makes it easy to configure the TUN device as follows:  
`$ sudo ./support/network/restart.sh`
{: .notice--info}

### Building NextEPC
---

Install the depedencies for building the source code.
```bash
$ sudo yum -y install git flex bison autoconf libtool lksctp-tools-devel libidn-devel gnutls-devel libgcrypt-devel openssl-devel cyrus-sasl-devel libyaml-devel
```

Configure EPEL package and install mongo-c-driver. 
```bash
$ sudo yum -y install epel-release
$ sudo yum -y install mongo-c-driver-devel
```

Git clone with `--recursive` option.

```bash
➜  open5gs git clone --recursive https://github.com/{{ site.github_username }}/nextepc
```

To compile with autotools:

```bash
➜  open5gs cd nextepc
➜  nextepc git:(master) ✗ autoreconf -iv
➜  nextepc git:(master) ✗ ./configure --prefix=`pwd`/install
➜  nextepc git:(master) ✗ make -j `nproc`
```

Check whether the compilation is correct.
```bash
➜  nextepc git:(master) ✗ make check
```

**Tip:** You can also check the result of `make check` with a tool that captures packets. If you are running `wireshark`, select the `loopback` interface and set FILTER to `s1ap || gtpv2 || diameter || gtp`.  You can see the virtually created packets. [[testcomplex.pcapng]]({{ site.url }}{{ site.baseurl }}/assets/pcapng/testcomplex.pcapng)
{: .notice--info}

You need to perform the **installation process**.
```bash
➜  nextepc git:(master) ✗ make install
```

### Building WebUI of NextEPC
---

[Node.js](https://nodejs.org/) is required to build WebUI of NextEPC

```bash
$ curl --silent --location https://rpm.nodesource.com/setup_8.x | sudo bash -
$ sudo yum -y install nodejs
```

Install the dependencies to run WebUI

```bash
➜  nextepc git:(master) ✗ cd webui
➜  webui git:(master) ✗ npm install
```

The WebUI runs as an [npm](https://www.npmjs.com/) script.

```bash
➜  webui git:(master) ✗ npm run dev
```
