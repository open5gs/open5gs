---
title: Dockerized VoLTE Setup
head_inline: "<style> .blue { color: blue; } </style>"
---

Setup description:
- MCC: 001, MNC: 01
- Docker + Docker Compose
- VM/Physical machine running Ubuntu 20.04 or 22.04
- srsENB + USRP B210 or a commercial eNB
- Sysmocom USIM - sysmoUSIM-SJS1
- UE: Mi 9 Pro 5G, Oneplus 5, iPhone X and above, Nokia 5.3.  Other UE are being tested.
{: .blue}

#### 0. Introduction

This tutorial introduces an install-and-run setup for Open5GS + Kamailio IMS VoLTE study.

The main purpose is to save researchers' and students' time to debug for a minimum-viable environment before actual study can be proceeded.

#### 1. Prepare SIM cards for VoLTE

Program IMSI, Ki, OP/OPc, SQN for the SIM cards you plan to use.

If VoLTE is not enabled on your Android phone then please refer to the instructions mentioned here: [VoLTE settings overriding](https://github.com/herlesupreeth/CoIMS_Wiki/blob/master/README.md).
{: .notice--info}

1. Using wrong KIC / KID / KIK bricks your SIM card and is irrecoverable
2. Use MCC = 001, MNC = 01 for a test network, unless you know your MCC/MNC is supported by Android Carrier Privileges.
{: .notice--warning}

#### 2. Build Open5GS, Kamailio docker images

* Mandatory requirements:
	* [docker-ce](https://docs.docker.com/install/linux/docker-ce/ubuntu) - Version 22.0.5 or above
	* [docker compose](https://docs.docker.com/compose) - Version 2.14 or above

Clone repository and build base docker image of open5gs and kamailio:

```
# Build docker images for open5gs EPC/5GC components
git clone https://github.com/herlesupreeth/docker_open5gs
cd docker_open5gs/base
docker build --no-cache --force-rm -t docker_open5gs .

# Build docker images for kamailio IMS components
cd ../ims_base
docker build --no-cache --force-rm -t docker_kamailio .

# Build docker images for additional components
cd ..
docker compose -f 4g-volte-deploy.yaml build
```

#### 3. Configuring your setup

`.env` is the only file most of them need to edit as per your deployment needs
{: .notice--warning}

Edit only the following parameters in `.env` as per your setup

```
MCC
MNC
DOCKER_HOST_IP --> This is the IP address of the host running (EPC+IMS)
SGWU_ADVERTISE_IP --> Change this to value of DOCKER_HOST_IP
UE_IPV4_INTERNET --> Change this to your desired (Not conflicted) UE network ip range for internet APN
UE_IPV4_IMS --> Change this to your desired (Not conflicted) UE network ip range for ims APN
```

If eNB is NOT running in the same docker network/host as the host running the dockerized Core + IMS then follow the below additional steps

Under **mme** section in docker compose file (**4g-volte-deploy.yaml**), uncomment the following part
```
...
    # ports:
    #   - "36412:36412/sctp"
...
```

Then, uncomment the following part under **sgwu** section
```
...
    # ports:
    #   - "2152:2152/udp"
...
```

#### 4. Deploying 4G/5G Core + IMS related components images

```
cd docker_open5gs
source .env
sudo ufw disable
sudo sysctl -w net.ipv4.ip_forward=1
docker compose -f 4g-volte-deploy.yaml up
```

#### 5. (Optional) Run srsENB in a separate container

Sometimes you may want to restart srsENB while keeping the core network running. It is thus recommended to run srsENB separately.

In order to run srsENB in a separate host, clone the docker_open5gs repository as mentioned above and build srsENB docker images.

```
# Build docker images for srsRAN_4G eNB
cd ../srslte
docker build --no-cache --force-rm -t docker_srslte .
```

And, edit only the following parameters in **.env** as per your setup

```
MCC
MNC
DOCKER_HOST_IP --> This is the IP address of the host running eNB
MME_IP --> Change this to IP address of host running (EPC+IMS)
SRS_ENB_IP --> Change this to the IP address of the host running eNB
```

Replace the following part in the docker compose file (**srsenb.yaml**)
```
    networks:
      default:
        ipv4_address: ${SRS_ENB_IP}
networks:
  default:
    external:
      name: docker_open5gs_default
```
with 
```
	network_mode: host
```

```
cd docker_open5gs
source .env
sudo cpupower frequency-set -g performance
docker compose -f srsenb.yaml up -d && docker container attach srsenb
```

#### 6. Configuration and provisioning of SIM information in HSS and HLR

If there is a need to change the Core Network component configuration, then corresponding configuration files can be found under their respective folders.
{: .notice--warning}

- Provision SIM details in open5gs HSS

Open (http://<DOCKER_HOST_IP>:9999) in a web browser, where <DOCKER_HOST_IP> is the IP of the machine/VM running the open5gs containers. Login with following credentials

```
Username : admin
Password : 1423
```

Add users with their corresponding IMSI, Ki, OP/OPc value and APN settings. The APN settings should look like below:

<pre>
APN Configuration:
---------------------------------------------------------------------------------------------------------------------
| APN      | Type | QCI | ARP | Capability | Vulnerablility | MBR DL/UL(Kbps)     | GBR DL/UL(Kbps) | PGW IP        |
---------------------------------------------------------------------------------------------------------------------
| internet | IPv4 | 9   | 8   | Disabled   | Disabled       | unlimited/unlimited |                 |               |
---------------------------------------------------------------------------------------------------------------------
| ims      | IPv4 | 5   | 1   | Disabled   | Disabled       | 3850/1530           |                 |               |
|          |      | 1   | 2   | Enabled    | Enabled        | 128/128             | 128/128         |               |
|          |      | 2   | 4   | Enabled    | Enabled        | 128/128             | 128/128         |               |
---------------------------------------------------------------------------------------------------------------------
</pre>

**Important!** Set the type of both APN to IPv4. Kamailio does not support VoLTE over IPv6 at the moment. (See the screenshot below)

![Set both type to IPv4 only](https://raw.githubusercontent.com/miaoski/docker_open5gs/gh-pages/screenshots/subscriber-type-ipv4.png)

- Provision IMSI and MSISDN in osmohlr to allow [SMS over SGs](https://github.com/herlesupreeth/docker_open5gs#provisioning-of-imsi-and-msisdn-with-osmohlr-as-follows)

- Provision SIM information in pyHSS [IMS](https://github.com/herlesupreeth/docker_open5gs#provisioning-of-sim-information-in-pyhss-is-as-follows)

#### 7. Debugging with Wireshark

Thanks to Open5GS, the topology is super similar to [SAE on Wikipedia](https://en.wikipedia.org/wiki/System_Architecture_Evolution#/media/File:Evolved_Packet_Core.svg).

![Network topology of Open5GS + IMS](https://raw.githubusercontent.com/miaoski/docker_open5gs/master/network-topology.png)

**APN**

On your cellphone, there should be *internet* and *ims*.

<img src="https://raw.githubusercontent.com/miaoski/docker_open5gs/gh-pages/screenshots/apn-on-cellphone.jpg" width="320" />

If CoIMS is used to force enable VoLTE on the Android device, it should look like in the screenshot below:

<img src="https://raw.githubusercontent.com/miaoski/docker_open5gs/gh-pages/screenshots/coims.jpg" width="320" />


**Networking issues**

PCAP files of successful calls can be found on [VoLTE Setup](https://open5gs.org/open5gs/docs/tutorial/02-VoLTE-setup/).

When DNS is not properly set, you may end up with 478 Unresolvable destination (478/SL):

![478 unresolvable destination](https://raw.githubusercontent.com/miaoski/docker_open5gs/gh-pages/screenshots/478-unresolvable-destination.png)

If the port if not open, or DNS is not properly configured, the phone cannot reach P-CSCF and fails.

![RST at port 5060](https://raw.githubusercontent.com/miaoski/docker_open5gs/gh-pages/screenshots/RST-5060.png)

If there is an NAT between PGW and P-CSCF, IPsec-NAT would not work, and the PCAP looks like the one below. Note that you need to run P-CSCF as root, in
order to add xfrm state and policy.

![401 Unauthorized](https://raw.githubusercontent.com/miaoski/docker_open5gs/gh-pages/screenshots/401-unauthorized.png)

#### 8. Successful calls

Herle Supreeth has shared PCAP files of successful calls, including
- [IPSec UE registration for VoLTE]({{ site.url }}{{ site.baseurl }}/assets/pcapng/ipsec_reg.pcapng)
- [Non-IPSec UE registration for VoLTE]({{ site.url }}{{ site.baseurl }}/assets/pcapng/nonipsec_reg.pcapng)
- [IPSec UE to IPSec UE calling]({{ site.url }}{{ site.baseurl }}/assets/pcapng/ipsec_to_ipsec_call.pcapng)
- [Non-IPSec UE to IPSec UE calling]({{ site.url }}{{ site.baseurl }}/assets/pcapng/nonipsec_to_ipsec_call.pcapng)
- [IPSec UE to Non-IPSec UE calling]({{ site.url }}{{ site.baseurl }}/assets/pcapng/ipsec_to_nonipsec_call.pcapng)


**UE registration**

![UE registration with IPSec](https://raw.githubusercontent.com/miaoski/docker_open5gs/gh-pages/screenshots/ue-ipsec.png)

From the screenshot, we see a UE that supports IPSec got a response from S-CSCF, indicating that ipsec-3gpp is supported, protocol is ESP (ethernet proto 50, IPSec). Client port (port-c) is 5100 and server port (port-s) 6100.
Refer to [IMS/SIP - Basic Procedures](https://www.sharetechnote.com/html/IMS_SIP_Procedure_Reg_Auth_IPSec.html) if you want to know more.
Also, notice that packets after 401 Unauthorized are transmitted over ESP.

If a UE does not support IPSec, you don't see the "security-server", as shown below:

![UE registration without IPSec](https://raw.githubusercontent.com/miaoski/docker_open5gs/gh-pages/screenshots/ue-noipsec.png)


**VoLTE calls**

![ipsec to ipsec call](https://raw.githubusercontent.com/miaoski/docker_open5gs/gh-pages/screenshots/ipsec-to-ipsec%20calls.png)

The Wireshark above shows that after several IPSec (ESP) packets, S-CSCF is sending a SIP INVITE for UE 03 to UE 04. To be more precise,

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
