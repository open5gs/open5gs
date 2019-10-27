---
title: FreeBSD
head_inline: "<style> .blue { color: blue; } </style>"
---

This guide is based on **FreeBSD Relase 11.1**.
{: .blue}

### Getting MongoDB
---

Install MongoDB with package manager.
```bash
$ sudo pkg install mongodb
```

Run MongoDB server.
```bash
$ mkdir -p ./data/db
$ mongod --dbpath ./data/db
```

### Setting up TUN device (No persistent after rebooting)
---

Configure the TUN device.
```bash
$ sudo ifconfig lo0 alias 127.0.0.2 netmask 255.255.255.255
$ sudo ifconfig lo0 alias 127.0.0.3 netmask 255.255.255.255
$ sudo ifconfig lo0 alias 127.0.0.4 netmask 255.255.255.255
$ sudo ifconfig lo0 alias 127.0.0.5 netmask 255.255.255.255
```

Enable IP forwarding
```bash
$ sudo sysctl -w net.inet.ip.forwarding=1
```

**Tip:** The script provided in [$GIT_REPO/support/network/restart.sh](https://github.com/{{ site.github_username }}/open5gs/blob/master/support/network/restart.sh) makes it easy to configure the TUN device as follows:  
`$ sudo ./support/network/restart.sh`
{: .notice--info}

### Building Open5GS
---

Install the depedencies for building the source code.
```bash
$ sudo pkg install py36-pip ninja gcc bison gsed pkgconf git mongo-c-driver gnutls libgcrypt libidn libyaml
```

Install Meson using Python.
```bash
$ sudo pip install --upgrade pip
$ sudo pip install meson
```

Git clone.

```bash
$ git clone https://github.com/{{ site.github_username }}/open5gs
```

To compile with autotools:

```bash
$ cd open5gs
$ meson build --prefix=`pwd`/install
$ ninja -C build
```

Check whether the compilation is correct.

**Note:** This should require *sudo* due to access `/dev/tun0`.
{: .notice--danger}
```bash
$ sudo ninja -C build test
```

**Tip:** You can also check the result of `sudo ninja -C build test` with a tool that captures packets. If you are running `wireshark`, select the `loopback` interface and set FILTER to `s1ap || gtpv2 || diameter || gtp`.  You can see the virtually created packets. [[testsimple.pcapng]]({{ site.url }}{{ site.baseurl }}/assets/pcapng/testsimple.pcapng)
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
$ sudo pkg install node
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

