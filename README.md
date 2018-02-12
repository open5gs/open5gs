What's NextEPC
================

NextEPC is a C-language Open Source implementation of the 3GPP Evolved Packet Core, i.e. the core network of an LTE network.

NextEPC provides the MME (Mobility Management Engine), which terminates the S1 interfaces from the eNodeBs cells in the cellular network, and interfaces via S11 to the SGW as well as via S6a to the HSS.

NextEPC provides the SGW (Serving Gateway) which is situated between the MME and PGW.  It implements the S11 interface to the MME, and the S5 interface to the PGW.

NextEPC provides the PGW or PDN-GW (Packet Data Network Gateway) element of the EPC, i.e. the gateway between the EPC and the external packet data network, such as the public Internet.  It implements the S5 interface towards the S-GW, the SGi interface towards the Internet, and the S7 interface towards the PCRF.

NextEPC provides the HSS (Home Subscriber Server) element of the EPC, i.e. the central database of mobile network subscribers, with their IMSI, MSISDN, cryptographic key materials, service subscription information, etc.  It implements the S6a interface towards the MME using the DIAMETER protocol.

NextEPC contains the PCRF (Policy and Charging Rules Function), which controls the service quality (QoS) of individual connections and how to account/charge related traffic.  It implements the Gx interface towards the PGW using the DIAMETER protocol.

Installation 
============

This post will guide you on how to get installed **NextEPC** with your environment. To date, NextEPC has been tested on GNU/Linux distributions(Debian, Ubuntu, CentOS, Fedora), FreeBSD, and Mac OS X.



## Ubuntu

To get the latest Ubuntu version, please visit the official Ubuntu website: [https://www.ubuntu.com/download/](https://www.ubuntu.com/download/). 

* ### MME, SGW, PGW, HSS, and PCRF

The NextEPC package is available on the recent versions of Ubuntu.

```bash
sudo apt-get -y install software-properties-common
sudo add-apt-repository ppa:acetcom/nextepc
sudo apt-get update
sudo apt-get -y install nextepc
```
This will create a virtual network interface named as *pgwtun*. It is automatically removed by uninstalling NextEPC.

```markdown
ifconfig pgwtun
pgwtun    Link encap:UNSPEC  HWaddr 00-00-00-00-00-00-00-00-00-00-00-00-00-00-00-00  
          inet addr:45.45.0.1  P-t-P:45.45.0.1  Mask:255.255.0.0
          inet6 addr: fe80::50f6:182c:5aa3:16bb/64 Scope:Link
          inet6 addr: cafe::1/64 Scope:Global
          ...
```

The NextEPC service is registered in `systemd` environment, and is started automatically during the installation phase. The service names are *nextepc-mmed*, *nextepc-sgwd*, *nextepc-pgwd*, *nextepc-hssd*, and *nextepc-pcrfd*. You can use the `systemctl` command to control specific services.

```bash
sudo systemctl status nextepc-mmed (Check the service status)
sudo systemctl stop nextepc-mmed (Stop the service)
sudo systemctl disable nextepc-mmed (Will not be started after rebooting)
sudo systemctl enable nextepc-mmed (Will be started after rebooting)
sudo systemctl start nextepc-mmed (Start the service)
sudo systemctl restart nextepc-mmed (Stop and start)
```


* ### Web User Interface

The LTE user subcription information of NextEPC is stored and maintained by [Mongo DB](https://www.mongodb.com/). To manage the subscriber information, [Mongo DB client](https://docs.mongodb.com/ecosystem/tools/) is required, and this client can connect to the DB URI [_mongodb://localhost/nextepc_]. 

NextEPC provides an alternative management interface for customers to manage their subscriber information in an easy way, that is **Web User Interface**. The following shows how to install the Web UI of NextEPC.

```bash
sudo apt-get -y install curl
curl -sL https://deb.nodesource.com/setup_8.x | sudo -E bash -
curl -sL http://nextepc.org/static/webui/install | sudo -E bash -
```

The service name is *nextepc-webui*. You must run it manually after installation. And then, the web server will be running on _http://localhost:3000_.

```bash
sudo systemctl start nextepc-webui
```



* ### Uninstall NextEPC

```bash
curl -sL http://nextepc.org/static/webui/uninstall | sudo -E bash -
sudo apt-get purge nextepc*
```

You may need to remove manually /var/log/nextepc unless it is empty.
```bash
sudo rm -Rf /var/log/nextepc
```


## CentOS, Fedora, FreeBSD, and Mac OS X

For these OS, you should build NextEPC from the code. First clone this [repository](https://github.com/acetcom/nextepc.git) and then follow instructions described in the [documentation](http://nextepc.org/docs/). 

* ### [CentOS](http://nextepc.org/docs/build/1-centos)
* ### [Fedora](http://nextepc.org/docs/build/2-fedora)
* ### [FreeBSD](http://nextepc.org/docs/build/3-freebsd)
* ### [Mac OS X](http://nextepc.org/docs/build/4-macosx)

Build 
=====

Since NextEPC is an open source project, you can build and use this program directly from source code. If you have already installed it with a package manager, or are not interested in learning the source code, you can skip this guide and proceed to the next [configuration guide](http://nextepc.org/guides/3-configuration). 

Note that this guide is based on Ubuntu 16.04.3(Zenial) Distribution.

## Prerequisites

NextEPC requires MongoDB and TUN device. If you have previously installed NextEPC according to the [Installation Guide](http://nextepc.org/guides/1-installation), they was configured at that time. So, you can skip this step.

Install Mongo DB with Package Manager.
```bash
sudo apt-get -y install mongodb
sudo systemctl start mongodb (if '/usr/bin/mongod' is not running)
```

To run NextEPC with least privilege, TUN device permission should be a `crw-rw-rw-`(666). Otherwise, you need to run nextepc daemon with root privilege. If the permission is not `crw-rw-rw-`(666), you may need to install `udev` package.  Nevertheless, if the permssions do not change , you can run nextepc with root privileges or change the permission using `chmod 666 /dev/net/tun`.

```bash
ls -al /dev/net/tun
crw-rw---- 1 root 28 10, 200 Feb 11 05:13 /dev/net/tun
sudo apt-get install udev
sudo systemctl start systemd-udevd (if '/lib/systemd/systemd-udevd' is not running)
```

Write the configuration file for the TUN deivce.
```bash
sudo sh -c "cat << EOF > /etc/systemd/network/99-nextepc.netdev
[NetDev]
Name=pgwtun
Kind=tun
EOF"
```

Craete the TUN device. Interface name will be `pgwtun`.
```
sudo systemctl enable systemd-networkd
sudo systemctl restart systemd-networkd

sudo apt-get -y install net-tools
ifconfig pgwtun
```

Then, you need to check *IPv6 Kernel Configuration*. Although you can skip this process, we recommend that you set this up to support IPv6-enabled UE.

```bash
sysctl -n net.ipv6.conf.pgwtun.disable_ipv6

(if the output is 0 and IPv6 is enabled, skip the followings)
sudo sh -c "echo 'net.ipv6.conf.pgwtun.disable_ipv6=0' > /etc/sysctl.d/30-nextepc.conf"
sudo sysctl -p /etc/sysctl.d/30-nextepc.conf
```

You are now ready to set the IP address on TUN device. If IPv6 is disabled for TUN device, please remove `Address=cafe::1/64` from below.

```bash
sudo sh -c "cat << EOF > /etc/systemd/network/99-nextepc.network
[Match]
Name=pgwtun
[Network]
Address=45.45.0.1/16
Address=cafe::1/64
EOF"
```

Check the TUN(pgwtun) device again.
```
sudo systemctl restart systemd-networkd
ifconfig pgwtun
```


## MME, SGW, PGW, HSS, and PCRF

Install the depedencies for building the source
```bash
sudo apt-get -y install autoconf libtool gcc pkg-config git flex bison libsctp-dev libgnutls28-dev libgcrypt-dev libssl-dev libmongoc-dev libbson-dev libyaml-dev
```

Git clone and compile
```bash
acetcom@nextepc:~$ git clone https://github.com/acetcom/nextepc
ccetcom@nextepc:~$ cd nextepc
acetcom@nextepc:~/nextepc$ autoreconf -iv
acetcom@nextepc:~/nextepc$ ./configure --prefix=`pwd`/install
acetcom@nextepc:~/nextepc$ make -j `nproc`
acetcom@nextepc:~/nextepc$ make install
```

We provide a program that checks whether the installation is correct. After running the wireshark, select `loopback` interface, filter `s1ap || diameter || gtpv2 || gtp` and run `./test/testepc`. You can see the virtually created packets. [[testepc.pcapng]](http://nextepc.org/static/pcapng/testepc.pcapng)

Note that you should stop all nextepc daemons before running test program if you have already installed it with a package manage.
```bash
(if nextepc-daemons are running)
sudo systemctl stop nextepc-mmed
sudo systemctl stop nextepc-sgwd
sudo systemctl stop nextepc-pgwd
sudo systemctl stop nextepc-hssd
sudo systemctl stop nextepc-pcrfd

acetcom@nextepc:~/nextepc$ ./test/testepc
```

It is a convenient tool called `nextepc-epcd` for developers. This daemon includes both *MME*, *SGW*, *PGW*, *HSS*, and *PCRF*. So, instead of running all 5 daemons, you can just run `nextepc-epcd` in your development environment.

```bash
acetcom@nextepc:~/nextepc$ ./nextepc-epcd
NextEPC daemon v0.3.3 - Feb 11 2018 07:19:59

  PID[3720] : '/home/acetcom/nextepc/install/var/run/nextepc-epcd/pid'
  File Logging : '/home/acetcom/nextepc/install/var/log/nextepc/nextepc.log'
  MongoDB URI : 'mongodb://localhost/nextepc'
  Configuration : '/home/acetcom/nextepc/install/etc/nextepc/nextepc.conf'
[02/11 07:26:42.001] PCRF try to initialize
...
```

When you run `nextepc-epcd`, all logs for MME, SGW, PGW, PCRF, and HSS are written to `nextepc.log`, and all settings are managed in one place for `nextepc.conf`. You can find the log/conf path at the beginning of running screen.

Sometimes, you may want to use newly updated source code.
```bash
(Control-C kill nextepc-epcd)
acetcom@nextepc:~/nextepc$ make maintainer-clean
acetcom@nextepc:~/nextepc$ rm -rf ./install
acetcom@nextepc:~/nextepc$ git pull
acetcom@nextepc:~/nextepc$ autoreconf -iv
acetcom@nextepc:~/nextepc$ ./configure --prefix=`pwd`/install
acetcom@nextepc:~/nextepc$ make -j `nproc`
acetcom@nextepc:~/nextepc$ make install
acetcom@nextepc:~/nextepc$ ./nextepc-epcd
```

## Web User Interface

To get the latest [Node.js](https://nodejs.org/) and [NPM](https://www.npmjs.com/), please visit the official Node.js website:
[https://nodesjs.org/en/download/](https://nodesjs.org/en/download/).

Or, you can install [Node.js](https://nodejs.org/) and [NPM](https://www.npmjs.com/) with a package manager.

```bash
sudo apt-get -y install curl
curl -sL https://deb.nodesource.com/setup_8.x | sudo -E bash -
sudo apt-get -y install nodejs
```

Install the dependencies to run WebUI

```bash
acetcom@nextepc:~/nextepc$ cd webui
acetcom@nextepc:~/nextepc/webui$ npm install
```

Running WebUI

```bash
acetcom@nextepc:~/nextepc/webui$ npm run dev
```

Now the web server is running on _http://localhost:3000_.


Configuraiton 
=============

In LTE, there are tons of configurable parameters. This page will guide you to set essential parameters up. The configuration consists of three parts: IP network connectivity, LTE network settings and Subscribers registering.

## 1. IP Connectivity between Network Entities

The minimum requirement of having IP connectvity is to modify the configuration files of MME and SGW. Once NextEPC has been installed, you can find [YAML](http://yaml.org/)-format configuration files in `/etc/nextepc/*.conf`.

Note that [/etc/nextepc/nextepc.conf](https://github.com/acetcom/nextepc/blob/master/support/config/nextepc.conf.in) is just a manual. If you use `nextepc-epcd` in a build environment, this configuration file could be used, but if you installed it with the package manager, modifying this configuration file has no effect.

Anyway, before setting up, please decide a network interface to run NextEPC, and then the IP address of the interface needs to be recorded in the configuration files.

### Modification of MME config

Open `/etc/nextepc/mme.conf` file, and find an item in mme &rarr; s1ap. Please set your IP address with putting `addr:` keyword.

```yaml
mme:
    freeDiameter: mme.conf
    s1ap:
      addr: <ip address>
...
```

Save and exit.

### Modification of SGW config

Open `/etc/nextepc/sgw.conf` file, and find an item in sgw &rarr; gtpu. Please set your IP address with putting `addr:` keyword.

```yaml
sgw:
    gtpc:
      addr: 127.0.0.2
    gtpu:
      addr: <ip address>
...
```

Save and exit.


### Adding a route for UE to have Internet connectivity

By default, a LTE UE will receive a IP address with the network address of 45.45.0.0/16. If you have a [NAT](https://en.wikipedia.org/wiki/Network_address_translation) router (e.g., wireless router, cable modem, etc), the LTE UE can reach Internet in uplink, but it cannot in downlink. It's because the NAT router has no idea on 45.45.0.0/16, so adding a route is required. Please refer to the user manual to know how to add a static route in your router.

Add a route of both 45.45.0.0/16 and cafe::0/64 to go the PGW IP address. For example, a command for Linux will be:

```bash
sudo ip route add 45.45.0.0/16 via <PGW IP address>
sudo ip route add cafe::0/64 via <PGW IP address>
```

If you have no NAT router, there is another option for you. `iptables` can solve the problem. You execute the following command in NextEPC installed host.

```bash
sudo sh -c "echo 1 > /proc/sys/net/ipv4/ip_forward"
sudo iptables -t nat -A POSTROUTING -o eth0 -j MASQUERADE
sudo iptables -I INPUT -i pgwtun -j ACCEPT
```

## 2. LTE Network Settings

### PLMN and TAC

By default, LTE PLMN and TAC are set as shown in the following:

```yaml
mme:
    gummei: 
      plmn_id:
        mcc: 001
        mnc: 01
      mme_gid: 2
      mme_code: 1
    tai:
      plmn_id:
        mcc: 001
        mnc: 01
      tac: 12345
```

The LTE EnodeBs need to be set to use the same values of PLMN and TAC in NextEPC. If you want to change them, please modifiy in `/etc/nextepc/mme.conf`.


### Restarting MME and SGW.

After changing conf files, please restart NextEPC daemons.

```bash
sudo systemctl restart nextepc-mmed
sudo systemctl restart nextepc-sgwd
```

## 3. Register a subscriber

Open _http://localhost:3000_. Login with **admin**. Later, you can change the password in _Account_ Menu.

```markdown
  - Username : admin
  - Password : 1423
```

Using Web UI, you can add a subscriber without a Mongo DB client. 

```markdown
  - Go to Subscriber Menu.
  - Click `+` Button to add a new subscriber.
  - Fill the IMSI, security context(K, OPc, AMF), and APN of the subscriber.
  - Click `SAVE` Button
```

This addition affects immediately NextEPC without restaring any daemon.

## Appendix 

When you install NextEPC for the first time, the default configuration looks like this:

### Network

```
* MME
  S1AP: listen on all address avaiable in system
  GTP-C: listen on the first IP address in system
  DIAMETER: 127.0.0.2 (No TLS)

* SGW
  GTP-C: 127.0.0.2
  GTP-U: listen on the first IP address in system

* PGW
  GTP-C: Both 127.0.0.3 and [::1]
  GTP-U: Both 127.0.0.3 and [::1]
  DIAMETER: 127.0.0.3 (No TLS)

* HSS
  DIAMETER: 127.0.0.4 (No TLS)

* PCRF
  DIAMETER: 127.0.0.5 (No TLS)
```

### GUMMEI, PLMN and TAC

```
* GUMMEI
  PLMN ID - MNC: 001, MCC: 01
  MME Group : 2
  MME Code : 1

* TAI
  PLMN ID - MNC: 001, MCC: 01
  TAC : 12345
```

### Security

```
* Integrity : EIA1 - Snow 3G
* Ciphering : EEA0 - Nothing
```

### UE Network

```
* IPv4 : 45.45.0.1/16
* IPv6 : cafe::1/64
```

### DNS

```
* IPv4
  Primary : 8.8.8.8 
  Secondary : 8.8.4.4

* IPv6
  Primary : 2001:4860:4860::8888
  Secondary : 2001:4860:4860::8844
```
