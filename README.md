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

## Install with a Package Manager

The nextepc package is available on recent versions of Ubuntu.

```bash
sudo add-apt-repository ppa:acetcom/nextepc
sudo apt-get update
sudo apt-get install nextepc
```

That's it!

Configuraiton 
=============
## Configuration IP connectivity

Configuration files are located `/etc/nextepc` directory. 

Let's modify first the `/etc/nextepc/mme.conf` to set your IP address which is connected to eNodeB. For example, if your IP address is 192.168.0.6, both MME.NETWORK.S1AP_IPV4 and SGW.NETWORK.GTPU_IPV4 are changed as follows.
```
  MME :
  {
    NETWORK :
    {
      S1AP_IPV4: "192.168.0.6",
      GTPC_IPV4: "127.76.0.1"
    }
  },
  SGW :
  {
    NETWORK :
    {
      GTPC_IPV4: "127.76.0.2",
      GTPU_IPV4: "192.168.0.6"
    }
  }
```

And then, modify `/etc/nextepc/sgw.conf` to set your IP address. SGW.NETWORK.GTPU_IPV4 is updated with 192.168.0.6.
```
  SGW :
  {
    NETWORK :
    {
      GTPC_IPV4: "127.76.0.2",
      GTPU_IPV4: "192.168.0.6"
    }
  }
```

Finally, you should modify the routing table of the router, which is connected to the nextepc installed host. The following command is just a sample. The configuration method for each router will be different.
```bash
sudo route add -net 45.45.0.0 192.168.0.6
```

## Update GUMMEI and TAI

The followings are the **GUMMEI** and **TAI** of the *MME* currently set to Default. Your *eNodeB* will also have a **PLMN ID** and **TAC** set. Refer to these parameters to change the setting of MME or eNodeB.

```
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

```
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

## Restart MME and SGW.

```bash
systemctl restart nextepc-mmed
systemctl restart nextepc-sgwd
```

Now, S1-Setup is ready! 

Web User Interface 
==================

## Install Node.js and NPM

To get the latest **Node.js** and **NPM** version, you can visit the official **Node.js** website:
[https://nodesjs.org/en/download/](https://nodesjs.org/en/download/).

Or, you can install _Node.js_ and _NPM_ on **Ubuntu** as follows:

```bash
sudo apt-get -y install curl gnupg
curl -sL https://deb.nodesource.com/setup_8.x | sudo -E bash -
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

This will start a server available on [http://localhost:3000](http://localhost:3000).

## Login with default account

Use **Web Browser** to connect it. _http://localhost:3000_

  * Username : admin
  * Password : 1423

Then, you can change your password in _Account_ Menu.

## Register Subscriber Information

There is only one setting for this guide. The _Subscriber Information_ required for **HSS** should be registered in _Mongo DB_. 

  * Go to Subscriber Menu
  * Click + Button to add Subscriber Information
  * Fill IMSI, Security(K, OPc, AMF), APN in the Form
  * Click the `SAVE` Button

Turn on your **eNodeB** and **Mobile**. Check Wireshark!
