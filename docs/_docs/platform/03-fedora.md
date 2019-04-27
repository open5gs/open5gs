---
title: Fedora
head_inline: "<style> .blue { color: blue; } </style>"
---

This guide is based on **Fedora 27** Distribution.
{: .blue}

### Getting MongoDB
---

Install MongoDB with package manager.
```bash
$ sudo dnf -y install mongodb-server
```

Run MongoDB server.
```bash
$ mkdir -p ./data/db
$ mongod --dbpath ./data/db
```

### Setting up TUN device (No persistent after rebooting)
---

Create the TUN device. Interface name will be `pgwtun`.
```bash
$ sudo dnf -y install iproute
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
$ sudo dnf -y install git gcc flex bison autoconf libtool mongo-c-driver-devel lksctp-tools-devel libidn-devel gnutls-devel libgcrypt-devel openssl-devel cyrus-sasl-devel snappy-devel libyaml-devel
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

You need to perform **the installation process**.
```bash
➜  nextepc git:(master) ✗ make install
```

Check whether the installation is correct.
```bash
➜  nextepc git:(master) ✗ ./test/testcomplex
s1setup_test        : SUCCESS
attach_test         : SUCCESS
volte_test          : SUCCESS
handover_test       : SUCCESS
All tests passed.
```

**Tip:** You can also check the result of `./test/testcomplex` with a tool that captures packets. If you are running `wireshark`, select the `loopback` interface and set FILTER to `s1ap || gtpv2 || diameter || gtp`.  You can see the virtually created packets. [[testcomplex.pcapng]]({{ site.url }}{{ site.baseurl }}/assets/pcapng/testcomplex.pcapng)
{: .notice--info}

### Building WebUI of NextEPC
---

[Node.js](https://nodejs.org/) is required to build WebUI of NextEPC

```bash
$ sudo dnf -y install nodejs
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
