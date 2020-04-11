---
title: Mac OS X
head_inline: "<style> .blue { color: blue; } </style>"
---

This guide is based on **macOS Catalina 10.15**.
{: .blue}

### Installing Homebrew
---

```bash
$ /usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
```

### Getting MongoDB
---

Install MongoDB with Package Manager.
```bash
$ brew tap mongodb/brew
$ brew install mongodb-community
```

Run MongoDB server.
```bash
$ mkdir -p ./data/db
$ mongod --dbpath ./data/db
```

**Tip:** MongoDB is persistent after rebooting with the following commands:
`$ brew services start mongodb-community`
{: .notice--info}


### Setting up TUN device (No persistent after rebooting)
---

Install TUN/TAP driver
- You can download it from [http://tuntaposx.sourceforge.net/](http://tuntaposx.sourceforge.net/)

Configure the TUN device.
```bash
$ sudo ifconfig lo0 alias 127.0.0.2 netmask 255.255.255.255
$ sudo ifconfig lo0 alias 127.0.0.3 netmask 255.255.255.255
$ sudo ifconfig lo0 alias 127.0.0.4 netmask 255.255.255.255
$ sudo ifconfig lo0 alias 127.0.0.5 netmask 255.255.255.255
```

Enable IP forwarding & Masquerading
```bash
$ sudo sysctl -w net.inet.ip.forwarding=1
$ sudo sh -c "echo 'nat on {en0} from 10.45.0.0/16 to any -> {en0}' > /etc/pf.anchors/org.open5gs"
$ sudo pfctl -e -f /etc/pf.anchors/org.open5gs
```

**Tip:** The script provided in [$GIT_REPO/misc/netconf.sh](https://github.com/{{ site.github_username }}/open5gs/blob/master/misc/netconf.sh) makes it easy to configure the TUN device as follows:  
`$ sudo ./misc/netconf.sh`
{: .notice--info}

### Building Open5GS
---

Install the depedencies for building the source code.
```bash
$ brew install mongo-c-driver gnutls libgcrypt libidn libyaml pkg-config
```

Install Meson using Homebrew.
```bash
$ brew install meson
```

Git clone.

```bash
$ git clone https://github.com/{{ site.github_username }}/open5gs
```

To compile with meson:

```bash
$ cd open5gs
$ meson build --prefix=`pwd`/install -D c_std=c99
$ ninja -C build
```

Check whether the compilation is correct.

**Note:** This should require `sudo` due to access `/dev/tun0`.
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
$ brew install node
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

