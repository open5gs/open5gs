Installation
============

This post will guide you on how to get installed with **NextEPC**. To date, **NextEPC** has been compiled and tested on GNU/Linux distributions(Debian, Ubuntu, Fedora, OpenSUSE) and FreeBSD.

We will describe this guide using **Ubuntu 16.04 LTS (Xenial)**. You'll need to install **Ubuntu** if you don't have it installed already. To get the latest Ubuntu version please visit the official Ubuntu website: [https://www.ubuntu.com/download/](https://www.ubuntu.com/download/). 

## Install the dependencies for building the source

The first step is to use **apt-get** to install all depedencies.

```bash
sudo apt-get -y install git gcc flex bison make
sudo apt-get -y install autoconf libtool pkg-config
sudo apt-get -y install libsctp-dev libssl-dev
sudo apt-get -y install libgnutls-dev libgcrypt-dev
sudo apt-get -y install libmongoc-dev libbson-dev
sudo apt-get -y install mongodb
```

## Retrieve the latest version of the source package

```bash
git clone https://github.com/acetcom/nextepc
```

## Configure the build

```bash
cd nextepc
test -f configure || autoreconf -iv
CFLAGS='-O2' ./configure --prefix=`pwd`/install
```

## Compiling

Hopefully, once you have completed the autotools configuration, you only need to run:

```bash
make -j `nproc`
```

## Installing

Once the compilation is complete, you can install in the configured paths with:

```bash
make install
```
(this might require sudo depending on the configured target directories)

Web Setup
=========

NextEPC is configured with **Mongo DB**, which manages _Subscriber Information_ mainly, and **Configuration File** made in _JSON_ format.

The configuration file can be modified using a general editor such as vi or emacs, but _Subscriber Information_ can be managed properly using _Mongo DB Client_.

NextEPC provides **Web User Interface** solely designed to manage _Subscriber Information_ in an easy way without using _Mongo DB Client_. Let's how to install it from now on.

## Install Node.js and NPM

To get the latest **Node.js** and **NPM** version, you can visit the official **Node.js** website:
[https://nodesjs.org/en/download/](https://nodesjs.org/en/download/).

Or, you can install _Node.js_ and _NPM_ on **Ubuntu** as follows:

```bash
sudo apt-get -y install curl
curl -sL https://deb.nodesource.com/setup_8.x | \
    sudo -E bash -
sudo apt-get -y install nodejs
```

## Install the dependencies for building the source

The first step is to use **npm** to install all depedencies.

```bash
cd nextepc/webui
npm install
```

## Build Web User Interface
```bash
npm run build
```

## Running Web Server

```bash
npm run start
```

## Login with default account

Use **Web Browser** to connect it. _http://localhost:3000_

  * Username : admin
  * Password : 1423

Then, you can change your password in _Account_ Menu.

Network Configuration
=====================

NextEPC consists of five nodes such as _MME, SGW, PGW, HSS and PCRF_. Basically, each node can be installed in a physically separate host or in the same host.

We will run these five nodes that make up NextEPC in a **Single Host**. The reason is why it is the easiest way to understand how to configure NextEPC network.

## IP Configuration

In order to run _MME, HSS, SGW, PGW, and PCRF_ on a **Single Host**, IP address is set by using **IP aliasing**.

```bash
sudo ifconfig eth1:hss 10.1.35.214/24 up
sudo ifconfig eth1:mme 10.1.35.215/24 up
sudo ifconfig eth1:sgw_s5 10.1.35.216/24 up
sudo ifconfig eth1:sgw_s11 10.1.35.217/24 up
sudo ifconfig eth1:pcrf 10.1.35.218/24 up
sudo ifconfig eth1:pgw 10.1.35.219/24 up
```

## Setup for Data Path

Use the **TUN Driver** to make _Data Path_ to be used by the **PGW**.

```bash
sudo ip tuntap add name pgwtun mode tun
sudo ifconfig pgwtun 45.45.0.1/16 up
```

## Check Configuration File

A configuration file is located `etc/nextepc.conf` from the installed paths. If you need to change the IP address for a particular problem, you should modify `XXXX_IPV4` field in the configuration file.

For example, if you want the IP aliasing address of *PGW* to be _10.1.35.254_, `PGW.NETWORK.S5C_IPV4` and `PGW_NETWORK.S5U_IPV4` field should be updated like the followings.

```json
  PGW :
  {
    FD_CONF_PATH : "/etc/freeDiameter

    NETWORK :
    {
      S5C_IPV4: "10.1.35.254",
      S5U_IPV4: "10.1.35.254"
    }

    TUNNEL:
    {
      DEV_NAME: "pgwtun"
    }

    IP_POOL :
    {
      CIDR: 45.45.45.0/24
    }

    DNS :
    {
      PRIMARY_IPV4: "8.8.8.8",
      SECONDARY_IPV4: "4.4.4.4"
    }
  }
```

## Testing Network Configuration

Once you are done, run the testing script.
```bash
./test/testepc
```
You can see the simulated packet through **Wireshark**.  _(FILTER : s1ap || gtpv2 || diameter)_

Running NextEPC
===============

## Update GUMMEI and TAI

The followings are the **GUMMEI** and **TAI** of the *MME* currently set to Default. Your *eNodeB* will also have a **PLMN ID** and **TAC** set. Refer to these parameters to change the setting of MME or eNodeB.

```json
GUMMEI:
{
  PLMN_ID : 
  {
    MCC : "001",
    MNC : "01"
  }
  MME_GID : 2,
  MME_CODE : 1
},
TAI:
{
  PLMN_ID :
  {
    MCC: "001",
    MNC: "01",
  }
  TAC: 12345
}
```

For reference, MME can set several GUMMEI and TAI as **JSON array notation** as follows.

```json
GUMMEI:
[
  {
    PLMN_ID : 
    {
      MCC : "001",
      MNC : "01"
    }
    MME_GID : 2,
    MME_CODE : 1
  },
  {
    PLMN_ID : 
    {
      MCC : "005",
      MNC : "05"
    }
    MME_GID : 5,
    MME_CODE : 6
  },
]
TAI:
[
  {
    PLMN_ID :
    {
      MCC: "001",
      MNC: "01",
    }
    TAC: 12345
  },
  {
    PLMN_ID :
    {
      MCC: "005",
      MNC: "05",
    }
    TAC: 6789
  }
]
```

## Register Subscriber Information

There is only one setting for this guide. The _Subscriber Information_ required for **HSS** should be registered in _Mongo DB_. Let's run the **Web User Interface** with `npm run start` as mentioned eariler.

Connect http://localhost:3000 using Web Browser.

  * Go to Subscriber Menu
  * Click + Button to add Subscriber Information
  * Fill IMSI, Security(K, OP, AMF), APN in the Form
  * Click the `SAVE` Button

## Running NextEPC

Generally, to use NextEPC in several hosts, you must use an independent daemon called _mmed, sgwd, pgwd, hssd, and pcrfd_.

However, we are preparing a more convenient daemon named **epcd**. It enables to service all five nodes of NextEPC.

```bash
./epcd
```

Turn on your **eNodeB** and **Mobile**. Check Wireshark!

Hopefully, you can see the real packet for TAU/Service Request, Dedicated Bearer, Multiple APN, and S1/X2-Handover.
