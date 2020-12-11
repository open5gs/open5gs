---
title: CentOS
head_inline: "<style> .blue { color: blue; } </style>"
---

This guide is based on **CentOS 8** Distribution.
{: .blue}

## Install **CentOS 8** from Vagrant box (optional)
---
Vagrant provides a simple way to create and deploy Virtual Machines from
pre-built images using VirtualBox, libvirt, or VMWare as a hypervisor engine.
This allows the user to quickly create a virtual machine without the hassle
of installing the operating system by hand.

### Install Vagrant
---

The instructions to install Vagrant are provided at
[vagrantup.com](https://www.vagrantup.com/).


### Create a CentOS 8 Virtual Machine using Vagrant
---

Use the supplied `Vagrantfile` in the `vagrant` directory to create the
virtual machine.

Note that this Vagrantfile is identical to the base CentOS 8 box, with
the exception that the amount of virtual memory has been increased to 1GB:

```bash
cd vagrant/centos
vagrant up --provider virtualbox
```

### Log into the newly created CentOS VM
---

Use SSH to log into the CentOS 8 VM:

```bash
vagrant ssh
```

Note that the Open5GS source is *not* copied into the VM.  The instructions
below provide the step by step instructions for setting up Open5GS for
either a bare metal or virtual CentOS 8 system.

The rest of the commands below are performed inside the CentOS VM as the
user 'vagrant', or on your bare metal CentOS 8 system as any normal user.

## Install prerequisite packages to build and run Open5GS
---

### Enable CentOS 8 PowerTools repository
---

```bash
$ sudo dnf install 'dnf-command(config-manager)'
$ sudo dnf config-manager --set-enabled PowerTools
```

### Enable the Extra Packages for Enterprise Linux
---

Enable the Extra Packages for Enterprise Linux repo by installing the
``epel-release`` package:
```bash
$ sudo dnf install epel-release
```

### Enable [ELRepo](https://elrepo.org)
---

Enable the ELRepo repo (with testing enabled):

```bash
$ sudo dnf install https://www.elrepo.org/elrepo-release-8.el8.elrepo.noarch.rpm
$ sudo dnf config-manager --set-enabled elrepo-testing
```

### Install MongoDB using the package manager:
---

Create a repository file to install the MongoDB packages:

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

Install MongoDB using the package manager:

```bash
$ sudo dnf -y install mongodb-org
```

### Install the dependencies for building the source code.
---
Open5GS requires several packages which are not installed by default in
a base CentOS 8 installation.


```bash
$ sudo dnf install python3 meson ninja-build gcc flex bison git lksctp-tools-devel libidn-devel gnutls-devel libgcrypt-devel openssl-devel cyrus-sasl-devel libyaml-devel mongo-c-driver-devel libmicrohttpd-devel libcurl-devel libnghttp2-devel
```

### Install the SCTP kernel module in kernel-modules-extra.
---

```bash
$ sudo dnf install kernel-modules-extra
```

### Install iproute IP interface tools.
---

```bash
$ sudo dnf install iproute
```

### Update all installed packages to the latest versions.
---

This will update all of the installed packages to the latest versions from
all of the repos that we enabled above.

```bash
$ sudo dnf update
```

Note that this may update the kernel version so you may need to reboot
after this step to ensure that you are running this new kernel version.
This is important when you try to load the SCTP kernel module later.

```bash
[vm] $ sudo reboot
[host] $ # ssh back into the VM after it reboots...
[host] $ vagrant ssh
```

## Build Open5GS from Source
---

### Git clone the Open5GS source code.
---

Clone a copy of the open5GS source code from github:

```bash
$ cd ~
$ git clone https://github.com/{{ site.github_username }}/open5gs
```

### Compile the Open5GS source.
---

Compile the source using `meson`.  Note that this sets the installation prefix
to `~/open5gs/install`:

```bash
$ cd ~/open5gs
$ meson build --prefix=`pwd`/install
$ ninja -C build
```

## Run and Test Open5GS
---

### Start MongoDB
---

After installing the MongoDB server, it will be started automatically
when the machine is rebooted.  You can check the status using `systemctl`:

```bash
$ sudo systemctl status mongod.service
● mongod.service - MongoDB Database Server
   Loaded: loaded (/usr/lib/systemd/system/mongod.service; enabled; vendor preset: disabled)
   Active: active (running) since Fri 2020-11-20 09:46:40 UTC; 10h ago
     Docs: https://docs.mongodb.org/manual
  Process: 779 ExecStart=/usr/bin/mongod $OPTIONS (code=exited, status=0/SUCCESS)
  Process: 775 ExecStartPre=/usr/bin/chmod 0755 /var/run/mongodb (code=exited, status=0/SUCCESS)
  Process: 770 ExecStartPre=/usr/bin/chown mongod:mongod /var/run/mongodb (code=exited, status=0/SUCCESS)
  Process: 732 ExecStartPre=/usr/bin/mkdir -p /var/run/mongodb (code=exited, status=0/SUCCESS)
 Main PID: 781 (mongod)
   Memory: 99.7M
   CGroup: /system.slice/mongod.service
           └─781 /usr/bin/mongod -f /etc/mongod.conf
```

If it has not started, then it can be started using `systemctl`:

```bash
$ sudo systemctl enable mongod.service
$ sudo systemctl start mongod.service
```

The default database location is `/var/log/mongodb/mongod.log`.  This can
be adjusted in `/etc/mongod.conf`.

### Set up a TUN interface
---

Create a TUN/TAP interface. The interface name will be `ogstun`.
```bash
$ sudo ip tuntap add name ogstun mode tun
$ ip link show
```

To support IPv6-enabled UEs, you must configure the `ogstun` interface
to support IPv6.  This is done by setting the `diable_ipv6` option for
`ogstun` to 0 (false):

```bash
$ sysctl -n net.ipv6.conf.ogstun.disable_ipv6
1

$ sudo -w net.ipv6.conf.ogstun.disable_ipv6=0

$ sysctl -n net.ipv6.conf.ogstun.disable_ipv6
0
```

**Note:** If your TUN interface already supports IPv6, you can skip this steps above.
{: .notice--info}

Set the IP address on the `ogstun` TUN interface.

```bash
$ sudo ip addr add 10.45.0.1/16 dev ogstun
$ sudo ip addr add cafe::1/64 dev ogstun
```

Make sure it is set up properly.
```bash
$ sudo ip link set ogstun up
$ ip link show
```

**Notice:** This configuration is not persistent after rebooting. The
script provided in [$GIT_REPO/misc/netconf.sh](https://github.com/{{
site.github_username }}/open5gs/blob/master/misc/netconf.sh) makes it easy
to configure the TUN device as follows:
`$ sudo ./misc/netconf.sh`
{: .notice--info}

### Install the SCTP kernel module
---
Open5GS requires the Linux SCTP kernel module to be loaded in the kernel.
In the CentOS 8 Vagrant box SCTP is not loaded into the kernel automatically
so must be installed as follows:

```bash
$ sudo modprobe sctp
$ # Check that SCTP was loaded successfully:
$ sudo dmesg | grep sctp
[  639.971360] sctp: Hash tables configured (bind 256/256)
```

## Testing Open5GS
---

### Run Individual Open5GS tests.
----
Open5GS test applications are created in the `~/open5gs/build/tests` directory.

```bash
$ cd ~/open5gs

$ ./build/tests/attach/attach ## EPC Only
s1setup-test        : SUCCESS
guti-test           : SUCCESS
auth-test           : SUCCESS
idle-test           : SUCCESS
emm-status-test     : SUCCESS
ue-context-test     : SUCCESS
reset-test          : SUCCESS
All tests passed.

$ ./build/tests/registration/registration ## 5G Core Only
guti-test           : SUCCESS
auth-test           : SUCCESS
idle-test           : SUCCESS
dereg-test          : SUCCESS
identity-test       : SUCCESS
gmm-status-test     : SUCCESS
ue-context-test     : SUCCESS
All tests passed.

```

### Run all Open5GS tests.
----

Run all Open5GS test programs:
```bash
$ cd ~/open5gs
$ cd build
$ meson test -v
```

**Tip:** You can also check the result of `meson test -v` with a tool that captures packets. If you are running `wireshark`, select the `loopback` interface and set FILTER to `s1ap || gtpv2 || pfcp || diameter || gtp || ngap || http`.  You can see the virtually created packets. [testattach.pcapng]({{ site.url }}{{ site.baseurl }}/assets/pcapng/testattach.pcapng)/[testregistration.pcapng]({{ site.url }}{{ site.baseurl }}/assets/pcapng/testregistration.pcapng)
{: .notice--info}


### Installing Open5GS.
----

Open5GS can be installed using meson as well.  This will install the
product to `$(HOME)/open5gs/install` if the `prefix` was set as shown above.

```bash
$ cd ~/open5gs
$ cd build
$ ninja install
$ cd ../
$ ls install/bin
open5gs-amfd   open5gs-hssd  open5gs-nrfd   open5gs-sgwcd  open5gs-smfd  open5gs-udrd
open5gs-ausfd  open5gs-mmed  open5gs-pcrfd  open5gs-sgwud  open5gs-udmd  open5gs-pcfd
open5gs-upfd
```

## Building WebUI of Open5GS
---

[Node.js](https://nodejs.org/) is required to build WebUI of Open5GS

Install Node.js:
```bash
$ sudo dnf install nodejs
```

Install the dependencies to run WebUI
```bash
$ cd ~/open5gs
$ cd webui
$ npm install
```

The WebUI runs as an [npm](https://www.npmjs.com/) script.

```bash
$ npm run dev
```
