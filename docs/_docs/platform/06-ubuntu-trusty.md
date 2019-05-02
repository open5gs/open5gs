---
title: Ubuntu (Trusty)
head_inline: "<style> .blue { color: blue; } </style>"
---

This guide is based on **Ubuntu 14.04(Trusty)** Distribution.
{: .blue}

## Getting MongoDB

Install MongoDB with package manager.
```bash
$ sudo apt update
$ sudo apt install mongodb
$ sudo systemctl start mongodb (if '/usr/bin/mongod' is not running)
```

### TUN device permissions 
---

To run nextepc with least privilege, the TUN device privilege should be a `crw-rw-rw-`(666). Otherwise, you need to run nextepc daemon as root. If the permission is not `crw-rw-rw-`(666), you may need to install [udev](https://mirrors.edge.kernel.org/pub/linux/utils/kernel/hotplug/udev/udev.html) package.

```bash
$ ls -al /dev/net/tun
crw-rw---- 1 root 28 10, 200 Feb 11 05:13 /dev/net/tun

$ sudo apt install udev
$ sudo systemctl start systemd-udevd (if '/lib/systemd/systemd-udevd' is not running)

$ ls -al /dev/net/tun
crw-rw-rw- 1 root 28 10, 200 Feb 11 05:13 /dev/net/tun
```

Nevertheless, if the permission do not change, you can run nextepc with root privileges or change the permission using [chmod](https://www.gnu.org/software/coreutils/manual/html_node/chmod-invocation.html) as follows:

```bash
$ sudo chmod 666 /dev/net/tun
```

### Making TUN Permanent
---

Write the configuration file for the TUN deivce.
```bash
$ sudo sh -c "cat << EOF > /etc/network/interfaces.d/nextepc
auto pgwtun
iface pgwtun inet static
    address 45.45.0.1
    netmask 255.255.0.0
    pre-up ip tuntap add name pgwtun mode tun
    post-down ip tuntap del name pgwtun mode tun
iface pgwtun inet6 static
    address cafe::1
    netmask 64
EOF"
```

For loading TUN configuration,
```bash
$ sudo sh -c 'if ! grep "source-directory" /etc/network/interfaces | grep "/etc/network/interfaces.d" > /dev/null; then
    echo "source-directory /etc/network/interfaces.d" >> /etc/network/interfaces
fi'
```

Create the TUN device. Interface name will be `pgwtun`.
```bash
$ ifup pgwtun
```

Then, to support IPv6-enabled UEs, you must configure your TUN device to support IPv6.

```bash
$ sysctl -n net.ipv6.conf.pgwtun.disable_ipv6
1

$ sudo sh -c "echo 'net.ipv6.conf.pgwtun.disable_ipv6=0' > /etc/sysctl.d/30-nextepc.conf"
$ sudo sysctl -p /etc/sysctl.d/30-nextepc.conf

$ sysctl -n net.ipv6.conf.pgwtun.disable_ipv6
0
```

**Note:** If your TUN device already supports IPv6, you can skip this steps above.
{: .notice--warning}


### Building NextEPC
---

Install the depedencies for building the source code.

```bash
sudo apt install git gcc flex bison make autoconf libtool pkg-config libsctp-dev libssl-dev libgnutls-dev libidn11-dev libyaml-dev
```

Then, compile and install Mongo C Driver like the followings.
```bash
sudo apt-get -y install g++ libsasl2-dev
tar xzf mongo-c-driver-1.8.0.tar.gz
cd mongo-c-driver-1.8.0
./configure --disable-automatic-init-and-cleanup
make
sudo make install
sudo ldconfig
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

For developers, it provides `nextepc-epcd` daemon that includes both *MME*, *SGW*, *PGW*, *HSS*, and *PCRF*.

```bash
➜  nextepc git:(master) ✗ ./nextepc-epcd
04/06 23:13:03.367: [core] INFO: NextEPC daemon start (main.c:169)

PID[6404]: '/home/acetcom/Documents/git/open5gs/nextepc/install/var/run/nextepc-epcd/pid'
File Logging: '/home/acetcom/Documents/git/open5gs/nextepc/install/var/log/nextepc/nextepc.log'
MongoDB URI: 'mongodb://localhost/nextepc'
Configuration: '/home/acetcom/Documents/git/open5gs/nextepc/install/etc/nextepc/nextepc.conf'
04/06 23:13:03.369: [core] INFO: PCRF try to initialize (epc.c:37)
...
```
