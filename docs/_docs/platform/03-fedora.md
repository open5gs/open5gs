---
title: Fedora
head_inline: "<style> .blue { color: blue; } </style>"
---

This guide is based on **Fedora 30** Distribution.
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

Create the TUN device. Interface name will be `ogstun`.
```bash
$ sudo dnf -y install iproute
$ sudo ip tuntap add name ogstun mode tun
$ ip link show
```

Then, to support IPv6-enabled UEs, you must configure your TUN device to support IPv6.

```bash
$ sysctl -n net.ipv6.conf.ogstun.disable_ipv6
1

$ sudo -w net.ipv6.conf.ogstun.disable_ipv6=0

$ sysctl -n net.ipv6.conf.ogstun.disable_ipv6
0
```

**Note:** If your TUN device already supports IPv6, you can skip this steps above.
{: .notice--warning}

You are now ready to set the IP address on TUN device. 

```bash
$ sudo ip addr add 10.45.0.1/16 dev ogstun
$ sudo ip addr add cafe::1/64 dev ogstun
```

Make sure it is set up properly.
```bash
$ sudo ip link set ogstun up
$ ip link show
```

**Tip:** The script provided in [$GIT_REPO/misc/netconf.sh](https://github.com/{{ site.github_username }}/open5gs/blob/master/misc/netconf.sh) makes it easy to configure the TUN device as follows:  
`$ sudo ./misc/netconf.sh`
{: .notice--info}

### Building Open5GS
---

Install the depedencies for building the source code.
```bash
$ sudo dnf install python3 ninja-build gcc flex bison git lksctp-tools-devel libidn-devel gnutls-devel libgcrypt-devel openssl-devel cyrus-sasl-devel libyaml-devel mongo-c-driver-devel libmicrohttpd-devel libcurl-devel iproute
```

Install Meson
```bash
$ sudo dnf install meson
```

Git clone.

```bash
$ git clone https://github.com/{{ site.github_username }}/open5gs
```

To compile with meson:

```bash
$ cd open5gs
$ meson build --prefix=`pwd`/install
$ ninja -C build
```

Check whether the compilation is correct.
```bash
$ ninja -C build test
```

**Tip:** You can also check the result of `ninja -C build test` with a tool that captures packets. If you are running `wireshark`, select the `loopback` interface and set FILTER to `s1ap || gtpv2 || diameter || gtp`.  You can see the virtually created packets. [[testsimple.pcapng]]({{ site.url }}{{ site.baseurl }}/assets/pcapng/testsimple.pcapng)
{: .notice--info}

You need to perform **the installation process**.
```bash
$ cd build
$ ninja install
```

### Building WebUI of Open5GS
---

[Node.js](https://nodejs.org/) is required to build WebUI of Open5GS

```bash
$ sudo dnf -y install nodejs
```

Install the dependencies to run WebUI

```bash
$ cd webui
$ npm install
```

The WebUI runs as an [npm](https://www.npmjs.com/) script.

```bash
$ npm run dev
```
