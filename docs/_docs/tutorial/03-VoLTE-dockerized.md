---
title: Dockerized VoLTE Setup
head_inline: "<style> .blue { color: blue; } </style>"
---

Setup description:
- MCC: 001, MNC: 01
- Docker-compose
- srsENB + USRP B210 or a commercial eNB
- Sysmocom USIM - sysmoUSIM-SJS1
- UE: Mi 9 Pro 5G.  Other UE are being tested.
{: .blue}

#### 0. Introduction

This tutorial introduces an install-and-run lab for Open5GS + Kamailio IMS
VoLTE study, a follow-up project of [Open5GS Tutorial: VoLTE Setup with Kamailio IMS and Open5GS](https://open5gs.org/open5gs/docs/tutorial/02-VoLTE-setup/).

The main purpose is to save researchers' and students' time to debug for a
minimum-viable environment before actual study can be proceeded.

**Important notice before you start**

1. Java 7 is downloaded from an alternative location.  You have to agree with
   Oracle's term of service and have an Oracle account, to legally use Java SDK
   7u80.  By using this repo, I assume you have the legal right to use it and
   hold no liability.

You have to prepare IMSI, Ki, OP/OPc, SQN of your SIM cards.

#### 1. Prepare SIM cards for VoLTE

1. Wrong KIC / KID / KIK bricks your SIM card.
2. Use MCC = 001, MNC = 01 for a test network, unless you know your MCC/MNC is supported by Android Carrier Privileges.

Refer to: https://github.com/herlesupreeth/CoIMS_Wiki/blob/master/README.md
* gp --key-enc <KIC1> --key-mac <KID1> --key-dek <KIK1> -lvi
* gp --key-enc <KIC1> --key-mac <KID1> --key-dek <KIK1> --unlock
* gp --install applet.cap
* gp -a 00A4040009A00000015141434C0000 -a 80E2900033F031E22FE11E4F06FFFFFFFFFFFFC114E46872F28B350B7E1F140DE535C2A8D5804F0BE3E30DD00101DB080000000000000001
* gp --acr-list-aram

#### 2. Build Open5GS, Kamailio with docker-compose

Mandatory requirements:
  * [docker-ce](https://docs.docker.com/install/linux/docker-ce/ubuntu)
  * [docker-compose](https://docs.docker.com/compose)

Install docker-compose and make sure it works before going forward.

Clone the repository and build base docker images of open5gs and Kamailio:

```
git clone https://github.com/herlesupreeth/docker_open5gs
cd docker_open5gs/base
docker build --no-cache --force-rm -t docker_open5gs .

cd ../kamailio_base
docker build --no-cache --force-rm -t open5gs_kamailio .
```

#### 3. Configuring your setup

`.env` is the only file most of them need to edit as per their deployment needs
{: .notice--warning}

Edit only the following parameters in `.env` as per your setup

```
MCC
MNC
TEST_NETWORK --> Change this only if it clashes with the internal network at your home/office
DOCKER_HOST_IP --> This is the IP address of the host running your docker setup
SGWU_ADVERTISE_IP --> Change this to value of DOCKER_HOST_IP set above only if eNB is not running the same docker network/host
```

If eNB is NOT running in the same docker network/host as the host running the dockerized Core + IMS then follow the below additional steps

Under `mme` section in docker compose file (`docker-compose.yaml`, `nsa-deploy.yaml`), uncomment the following part

```
...
    # ports:
    #   - "36412:36412/sctp"
...
```

Under `sgwu` section in docker compose file (`docker-compose.yaml`, `nsa-deploy.yaml`), uncomment the following part

```
...
    # ports:
    #   - "2152:2152/udp"
...
```

#### 4. Building 4G/5G Core + IMS related components images

```
cd docker_open5gs
source .env
docker-compose build --no-cache
```

#### 5. (Optional) Run srsENB in a separate container

I use srsENB and USRP B210 in the lab.  Sometimes you may want to restart
srsENB while keeping the core network running.  It is thus recommended to run
srsENB separately.

```
cd docker_open5gs
source .env
docker-compose -f srsenb.yaml build --no-cache
docker-compose -f srsenb.yaml up
```

#### 6. Configuration and register two UE

If there is a need to change the Core Network component configuration files
found under their respective folder, make sure to re-compile images using
`docker-compose build` again.
{: .notice--warning}

Open (http://<DOCKER_HOST_IP>:3000) in a web browser, where <DOCKER_HOST_IP> is
the IP of the machine/VM running the open5gs containers. Login with following
credentials

```
Username : admin
Password : 1423
```

Follow the instructions in [VoLTE Setup](https://open5gs.org/open5gs/docs/tutorial/02-VoLTE-setup/):
- Step 18, set IMSI, Ki, OP, SQN and APN of your SIM cards.
  **Important!** Set the type of both APN to IPv4.  Kamailio does not support VoLTE over
  IPv6 at the moment. (See the screenshot below.)
- Step 20, add IMS subscriptions to FHoSS.

![Set both type to IPv4 only](https://raw.githubusercontent.com/miaoski/docker_open5gs/gh-pages/screenshots/subscriber-type-ipv4.png)

For already running systems, copy SQN from Open5GS and type it in FHoSS.  You
can type SQN in decimal. FHoSS will automagically convert it to hex.

Pay special attention to copy/paste. You might have leading or trailing spaces
in FHoSS, resulting in failed connections!

#### 7. Debugging with Wireshark

Thanks to Open5GS, the topology is super similar to [SAE on Wikipedia](https://en.wikipedia.org/wiki/System_Architecture_Evolution#/media/File:Evolved_Packet_Core.svg).

![Network topology of Open5GS + IMS](https://raw.githubusercontent.com/miaoski/docker_open5gs/master/network-topology.png)


**APN**

On your cellphone, there should be *internet* and *ims*.

<img src="https://raw.githubusercontent.com/miaoski/docker_open5gs/gh-pages/screenshots/apn-on-cellphone.jpg" width="320" />

CoIMS should look like the one below.  If you don't know what CoIMS is, please
refer to step 23 of VoLTE Setup.

<img src="https://raw.githubusercontent.com/miaoski/docker_open5gs/gh-pages/screenshots/coims.jpg" width="320" />


**Networking issues**

PCAP files of successful calls can be found on [VoLTE Setup](https://open5gs.org/open5gs/docs/tutorial/02-VoLTE-setup/).

When DNS is not properly set, you may end up with 478 Unresolvable destination (478/SL):

![478 unresolvable destination](https://raw.githubusercontent.com/miaoski/docker_open5gs/gh-pages/screenshots/478-unresolvable-destination.png)

If the port if not open, or DNS is not properly configured, the phone cannot
reach P-CSCF and fails.

![RST at port 5060](https://raw.githubusercontent.com/miaoski/docker_open5gs/gh-pages/screenshots/RST-5060.png)

If there is an NAT between PGW and P-CSCF, IPsec-NAT would not work, and the
PCAP looks like the one below.  N.B. that you need to run P-CSCF as root, in
order to add xfrm state and policy.

![401 Unauthorized](https://raw.githubusercontent.com/miaoski/docker_open5gs/gh-pages/screenshots/401-unauthorized.png)

#### 8. Successful calls

Herle Supreeth has shared PCAP files of successful calls, including
- [IPSec UE registration for VoLTE]({{ site.url }}{{ site.baseurl }}/assets/pcapng/ipsec_reg.pcapng)
- [Non-IPSec UE registration for VoLTE]({{ site.url }}{{ site.baseurl }}/assets/pcapng/nonipsec_reg.pcapng)
- [IPSec UE to IPSec UE calling]({{ site.url }}{{ site.baseurl }}/assets/pcapng/ipsec_to_ipsec_call.pcapng)
- [Non-IPSec UE to IPSec UE calling]({{ site.url }}{{ site.baseurl }}/assets/pcapng/nonipsec_to_ipsec_call.pcapng)
- [IPSec UE to Non-IPSec UE calling]({{ site.url }}{{ site.baseurl }}/assets/pcapng/ipsec_to_nonipsec_call.pcapng)

The successful calls were made with a commercial eNB (in his case a Casa
smallcell), while srsENB the ACK takes a very long time to reach the UE,
resulting in disconnected calls.

**UE registration**

![UE registration with IPSec](https://raw.githubusercontent.com/miaoski/docker_open5gs/gh-pages/screenshots/ue-ipsec.png)

From the screenshot, we see a UE that supports IPSec got a response from
S-CSCF, indicating that ipsec-3gpp is supported, protocol is ESP (ethernet
proto 50, IPSec).  Client port (port-c) is 5100 and server port (port-s) 6100.
Refer to [IMS/SIP - Basic Procedures](https://www.sharetechnote.com/html/IMS_SIP_Procedure_Reg_Auth_IPSec.html) if you want to know more.
Also, notice that packets after 401 Unauthorized are transmitted over ESP.

If a UE does not support IPSec, you don't see the "security-server", as shown below:

![UE registration without IPSec](https://raw.githubusercontent.com/miaoski/docker_open5gs/gh-pages/screenshots/ue-noipsec.png)


**VoLTE calls**

![ipsec to ipsec call](https://raw.githubusercontent.com/miaoski/docker_open5gs/gh-pages/screenshots/ipsec-to-ipsec%20calls.png)

The Wireshark above shows that after several IPSec (ESP) packets, S-CSCF is
sending a SIP INVITE for UE 03 to UE 04.  To be more precise,

```
Request-Line: INVITE sip:0398765432100;phone-context=0498765432100@0498765432100;user=phone SIP/2.0
...
Record-Route URI: sip:mo@10.4.128.21:6101;lr=on;ftag=7b3fae13;rm=8;did=078.654
```

The SIP port of the caller (`contact`) will also be passed to the callee,
```
Contact URI: sip:0498765432100@192.168.101.3:6400;alias=192.168.101.3~6401~1
```

After S-CSCF forwarded the INVITE to P-CSCF, it returns a 100 Trying, and contacts with the callee via IPSec:

![ipsec callee](https://raw.githubusercontent.com/miaoski/docker_open5gs/gh-pages/screenshots/ipsec-to-ipsec%20callee.png)

This can be contrasted when the callee does not support IPSec.  After 100
Trying, a UE that does not support IPSec is sent a SIP INVITE in clear text:

![non-ipsec callee](https://raw.githubusercontent.com/miaoski/docker_open5gs/gh-pages/screenshots/ipsec-to-noipsec.png)

#### 9. Known issues

- IPv6 is not supported.

#### 10. References

- [https://github.com/onmyway133/blog/issues/284](https://github.com/onmyway133/blog/issues/284)
- [https://realtimecommunication.wordpress.com/2015/05/26/at-your-service/](https://realtimecommunication.wordpress.com/2015/05/26/at-your-service/)
- [https://www.sharetechnote.com/html/Handbook_LTE_VoLTE.html](https://www.sharetechnote.com/html/Handbook_LTE_VoLTE.html)
