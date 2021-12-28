---
title: FreeBSD
head_inline: "<style> .blue { color: blue; } </style>"
---

This guide is based on **FreeBSD-11.4-STABLE**.
{: .blue}

## Install **FreeBSD-11.4-STABLE** from Vagrant box (optional)
---
Vagrant provides a simple way to create and deploy Virtual Machines from
pre-built images using VirtualBox, libvirt, or VMWare as a hypervisor engine.
This allows the user to quickly create a virtual machine without the hassle
of installing the operating system by hand.

### Install Vagrant
---

The instructions to install Vagrant are provided at
[vagrantup.com](https://www.vagrantup.com/).


### Create a FreeBSD-11.4-STABLE Virtual Machine using Vagrant
---

Use the supplied `Vagrantfile` in the `vagrant` directory to create the
virtual machine.

Note that this Vagrantfile is identical to the base FreeBSD 11 box, with
the exception that the amount of virtual memory has been increased to 1GB:

```bash
cd vagrant/freebsd
vagrant up --provider virtualbox
```

### Log into the newly created FreeBSD VM
---

Use SSH to log into the FreeBSD 11 VM:

```bash
vagrant ssh
```

Note that the Open5GS source is *not* copied into the VM.  The instructions
below provide the step by step instructions for setting up Open5GS for
either a bare metal or virtual FreeBSD 11 system.

The rest of the commands below are performed inside the FreeBSD VM as the
user 'vagrant', or on your bare metal FreeBSD 11 system as any normal user.

### Getting MongoDB
---

Install MongoDB with package manager.
```bash
$ sudo pkg install mongodb44
```

Run MongoDB server.
```bash
$ mkdir -p ./data/db
$ mongod --dbpath ./data/db
```

### Setting up network (No persistent after rebooting)
---

Configure the loopback interface.
```bash
$ sudo ifconfig lo0 alias 127.0.0.2 netmask 255.255.255.255
$ sudo ifconfig lo0 alias 127.0.0.3 netmask 255.255.255.255
$ sudo ifconfig lo0 alias 127.0.0.4 netmask 255.255.255.255
$ sudo ifconfig lo0 alias 127.0.0.5 netmask 255.255.255.255
$ sudo ifconfig lo0 alias 127.0.0.5 netmask 255.255.255.255
$ sudo ifconfig lo0 alias 127.0.0.6 netmask 255.255.255.255
$ sudo ifconfig lo0 alias 127.0.0.7 netmask 255.255.255.255
$ sudo ifconfig lo0 alias 127.0.0.8 netmask 255.255.255.255
$ sudo ifconfig lo0 alias 127.0.0.9 netmask 255.255.255.255
$ sudo ifconfig lo0 alias 127.0.0.10 netmask 255.255.255.255
$ sudo ifconfig lo0 alias 127.0.0.11 netmask 255.255.255.255
$ sudo ifconfig lo0 alias 127.0.0.12 netmask 255.255.255.255
$ sudo ifconfig lo0 alias 127.0.0.13 netmask 255.255.255.255
$ sudo ifconfig lo0 alias 127.0.0.14 netmask 255.255.255.255
$ sudo ifconfig lo0 alias 127.0.0.15 netmask 255.255.255.255
$ sudo ifconfig lo0 alias 127.0.0.16 netmask 255.255.255.255
$ sudo ifconfig lo0 alias 127.0.0.17 netmask 255.255.255.255
$ sudo ifconfig lo0 alias 127.0.0.18 netmask 255.255.255.255
$ sudo ifconfig lo0 alias 127.0.0.19 netmask 255.255.255.255
$ sudo ifconfig lo0 alias 127.0.0.20 netmask 255.255.255.255
```

Enable IP forwarding
```bash
$ sudo sysctl -w net.inet.ip.forwarding=1
$ sudo sysctl -w net.inet6.ip6.forwarding=1
```

**Tip:** The script provided in [$GIT_REPO/misc/netconf.sh](https://github.com/{{ site.github_username }}/open5gs/blob/main/misc/netconf.sh) makes it easy to configure the TUN device as follows:
`$ sudo ./misc/netconf.sh`
{: .notice--info}

### Building Open5GS
---

Install the depedencies for building the source code.
```bash
$ sudo pkg install meson ninja gcc bison gsed pkgconf git mongo-c-driver gnutls libgcrypt libidn libyaml libmicrohttpd nghttp2 talloc
```

Configure gcc PATH
```bash
$ setenv LIBRARY_PATH /usr/local/lib
$ setenv C_INCLUDE_PATH /usr/local/include
```

If you are using BASH instead of default CSH,
```bash
$ export LIBRARY_PATH=/usr/local/lib
$ export C_INCLUDE_PATH=/usr/local/include
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

**Note:** No source code changes are required for FreeBSD 11.x version. However, in FreeBSD 12.x version, we'll getting a crash with segmentation fault when calling basename(3). To avoid this, you need to change the freeDiameter source code as below.
{: .blue}

```diff
$ cd open5gs/subprojects/freeDiameter

$ diff --git a/include/freeDiameter/libfdproto.h b/include/freeDiameter/libfdproto.h
index 52c11ef..cd7f383 100644
--- a/include/freeDiameter/libfdproto.h
+++ b/include/freeDiameter/libfdproto.h
@@ -293,7 +293,7 @@ extern int fd_g_debug_lvl;

 /* A version of __FILE__ without the full path. This is specific to each C file being compiled */
 static char * file_bname = NULL;
-static char * file_bname_init(char * full) { file_bname = basename(full); return file_bname; }
+static char * file_bname_init(char * full) { file_bname = __old_basename(full); return file_bname; }
 #define __STRIPPED_FILE__      (file_bname ?: file_bname_init((char *)__FILE__))

```

Now, compile again:
{: .blue}

```bash
$ cd open5gs
$ ninja -C build
```

Check whether the compilation is correct.

**Note:** This should require *sudo* due to access `/dev/tun0`.
{: .notice--danger}

```bash
$ sudo ./build/tests/attach/attach ## EPC Only
$ sudo ./build/tests/registration/registration ## 5G Core Only
```

Run all test programs as below.

**Note:** This should require *sudo* due to access `/dev/tun0`.
{: .notice--danger}

```bash
$ cd build
$ sudo meson test -v
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
$ sudo pkg install node
```

Install the dependencies to run WebUI

```bash
$ cd webui
$ npm ci --no-optional
```

The WebUI runs as an [npm](https://www.npmjs.com/) script.

```bash
$ npm run dev
```
