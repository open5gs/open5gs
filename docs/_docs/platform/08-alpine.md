---
title: Alpine
head_inline: "<style> .blue { color: blue; } </style>"
---

This guide is based on **Alpine 3.13** Distribution.
{: .blue}

### Getting MongoDB
---

Install MongoDB with package manager.
```bash
$ sudo apk update
$ sudo apk add mongodb
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
$ sudo apk add iproute2
$ sudo ip tuntap add name ogstun mode tun
$ ip link show
```

You are now ready to set the IP address on TUN device. 

```bash
$ sudo ip addr add 10.45.0.1/16 dev ogstun
$ sudo ip addr add 2001:db8:cafe::1/48 dev ogstun
```

Make sure it is set up properly.
```bash
$ sudo ip link set ogstun up
$ ip link show
```

**Tip:** The script provided in [$GIT_REPO/misc/netconf.sh](https://github.com/{{ site.github_username }}/open5gs/blob/main/misc/netconf.sh) makes it easy to configure the TUN device as follows:
`$ sudo ./misc/netconf.sh`
{: .notice--info}

### Building Open5GS
---

Install the depedencies for building the source code.
```bash
$ sudo apk add alpine-sdk bison flex git cmake meson bash sudo linux-headers bsd-compat-headers yaml-dev lksctp-tools-dev gnutls-dev libgcrypt-dev libidn-dev mongo-c-driver-dev libmicrohttpd-dev curl-dev nghttp2-dev talloc-dev
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
$ ./build/tests/attach/attach ## EPC Only
$ ./build/tests/registration/registration ## 5G Core Only
```

Run all test programs as below.
```bash
$ cd build
$ meson test -v
```

**Tip:** You can also check the result of `ninja -C build test` with a tool that captures packets. If you are running `wireshark`, select the `loopback` interface and set FILTER to `s1ap || gtpv2 || pfcp || diameter || gtp || ngap || http2.data.data || http2.headers`.  You can see the virtually created packets. [testattach.pcapng]({{ site.url }}{{ site.baseurl }}/assets/pcapng/testattach.pcapng)/[testregistration.pcapng]({{ site.url }}{{ site.baseurl }}/assets/pcapng/testregistration.pcapng)
{: .notice--info}

You need to perform the **installation process**.
```bash
$ cd build
$ ninja install
$ cd ../
```

### Building WebUI of Open5GS
---

[Node.js](https://nodejs.org/) is required to build WebUI of Open5GS

```bash
$ sudo apk add nodejs
```

Install the dependencies to run WebUI

```bash
$ cd webui
$ npm ci
```

The WebUI runs as an [npm](https://www.npmjs.com/) script.

```bash
$ npm run dev
```
