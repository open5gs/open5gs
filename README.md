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

This post will guide you on how to get installed **NextEPC** with your environment. To date, NextEPC has been tested on GNU/Linux distributions(Debian, Ubuntu, CentOS, Fedora, OpenSUSE), FreeBSD, and Mac OS X.



## Ubuntu

To get the latest Ubuntu version, please visit the official Ubuntu website: [https://www.ubuntu.com/download/](https://www.ubuntu.com/download/). 

* ### Install with a Package Manager

The Nextepc package is available on the recent versions of Ubuntu.

```bash
sudo add-apt-repository ppa:acetcom/nextepc
sudo apt-get update
sudo apt-get install nextepc
```
This will create a virtual network interface named as *pgwtun*. It is automatically removed by uninstalling NextEPC.

```markdown
ifconfig pgwtun
pgwtun    Link encap:UNSPEC  HWaddr 00-00-00-00-00-00-00-00-00-00-00-00-00-00-00-00  
          inet addr:45.45.0.1  P-t-P:45.45.0.1  Mask:255.255.0.0
          ...
```



* ### Uninstall NextEPC

```bash
sudo apt-get purge nextepc-core
```

You may need to remove manually /var/log/nextepc unless it is empty.
```bash
sudo rm -Rf /var/log/nextepc
```


## Debian, CentOS, Fedora, OpenSUSE, FreeBSD, and Mac OS X

For these OS, you should build NextEPC from the code. First clone this [repository](https://github.com/acetcom/nextepc.git) and then follow instructions described in the [documentation](http://nextepc.org/docs/). 

* ### [FreeBSD](http://nextepc.org/docs/build/1-freebsd)
* ### [Mac OS X](http://nextepc.org/docs/build/2-macosx)
* ### [CentOS](http://nextepc.org/docs/build/3-centos)
* ### [Fedora](http://nextepc.org/docs/build/4-fedora)
* ### [Ubuntu](http://nextepc.org/docs/build/5-ubuntu)

Configuraiton 
=============

In LTE, there are tons of configurable parameters. This page will guide you to set essential parameters up. The configuration consists of two parts: IP network connectivity and LTE network settings.

## 1. IP Connectivity between Network Entities

The minimum requirement of having IP connectvity is to modify the configuration files of MME and SGW. Once NextEPC has been installed, you can find [JSON](https://www.json.org/)-format configuration files in `/etc/nextepc/*.conf`.

Before setting up, please decide a network interface to run NextEPC, and then the IP address of the interface needs to be recorded in the configuration files (Note that the IPv6 support requires v0.3.0 or higher).

### Modification of MME config

Open `/etc/nextepc/mme.conf` file, and find an item in MME &rarr; NETWORK &rarr; S1AP_IPV4. Please set your IP address for S1AP_IPV4 putting double quotes around it.  

```json
  MME :
  {
    NETWORK :
    {
      S1AP_IPV4: "<ip address>",
      GTPC_IPV4: "127.76.0.1"
    }
  },
```

Similarily, find the next item in SGW &rarr; NETWORK &rarr; GTPU_IPV4 in the same file. Please set your IP address for GTPU_IPV4 putting double quotes around it, again.  

```json
  SGW :
  {
    NETWORK :
    {
      GTPC_IPV4: "127.76.0.2",
      GTPU_IPV4: "<ip address>"
    }
  }
```

Save and exit.


### Modification of SGW config

Open `/etc/nextepc/sgw.conf` file, and find an item in SGW &rarr; NETWORK &rarr; GTPU_IPV4. Please set your IP address for GTPU_IPV4 putting double quotes around it.

```json
  SGW :
  {
    NETWORK :
    {
      GTPC_IPV4: "127.76.0.2",
      GTPU_IPV4: "<ip address>"
    }
  }
```

Save and exit.


### Adding a route for UE to have Internet connectivity

By default, a LTE UE will receive a IP address with the network address of 45.45.0.0/16. If you have a [NAT](https://en.wikipedia.org/wiki/Network_address_translation) router (e.g., wireless router, cable modem, etc), the LTE UE can reach Internet in uplink, but it cannot in downlink. It's because the NAT router has no idea on 45.45.0.0/16, so adding a route is required. Please refer to the user manual to know how to add a static route in your router.

Add a route of 45.45.0.0/16 to go the ip address mentioned above. For example, a command for Linux will be:

```bash
sudo ip route add 45.45.0.0/16 via <ip address>
```

## 2. LTE Network Settings

### PLMN and TAC

By default, LTE PLMN and TAC are set as shown in the following:

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

The LTE EnodeBs need to be set to use the same values of PLMN and TAC in NextEPC. If you want to change them, please modifiy in `/etc/nextepc/mme.conf`.


### Restarting MME and SGW.

After changing conf files, please restart NextEPC daemons.

```bash
systemctl restart nextepc-mmed
systemctl restart nextepc-sgwd
```


Web UI
======

NextEPC has a number of configuration files corresponding to LTE network entities, which are in [JSON](https://www.json.org/) format. The LTE user subcription information of NextEPC is stored and maintained by [Mongo DB](https://www.mongodb.com/). Configuration files, located in `etc/nextepc/*.conf` can be easily modified using a general text editor such as [vi](http://www.vim.org/) or [emacs](https://www.gnu.org/s/emacs/), while managing the subscriber information requires a [Mongo DB client](https://docs.mongodb.com/ecosystem/tools/).

NextEPC provides an alternative management interface for customers to manage their subscriber information in an easy way, that is **Web User Interface**. The following shows how to install the Web UI of NextEPC.

## 1. Install Node.js and NPM

To get the latest [Node.js](https://nodejs.org/) and [NPM](https://www.npmjs.com/), please visit the official Node.js website:
[https://nodesjs.org/en/download/](https://nodesjs.org/en/download/).

Or, you can install [Node.js](https://nodejs.org/) and [NPM](https://www.npmjs.com/) if you're using [Ubuntu](https://www.ubuntu.com):

```bash
sudo apt-get -y install curl gnupg
curl -sL https://deb.nodesource.com/setup_8.x | sudo -E bash -
sudo apt-get -y install nodejs
```

## 2. Obtain the source code

```bash
git clone https://github.com/acetcom/nextepc
```

## 3. Install the dependencies to build the code

```bash
cd nextepc/webui
npm install
```

## 4. Build
```bash
npm run build
```

## 5. Running

```bash
npm run start
```

Now the web server is running on _http://localhost:3000_.

## 6. Login with the default account

Open _http://localhost:3000_. Login with **admin**.

  * Username : admin
  * Password : 1423

Please change the password in _Account_ Menu.

## 7. Register a subscriber

Using Web UI, you can add a subscriber without a Mongo DB client. 

  * Go to Subscriber Menu.
  * Click `+` Button to add a new subscriber.
  * Fill the IMSI, security context(K, OPc, AMF), and APN of the subscriber.
  * Click `SAVE` Button

This addition affects immediately NextEPC without restaring any daemon.

