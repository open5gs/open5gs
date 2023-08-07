---
title: Roaming
---

## 0. Introduction

Now, we will explain how to install and test 5G Core Roaming using Open5GS. Since we cannot test roaming with UERANSIM, we will use the test environment of Open5GS. 

We can only use the LBO method. HR roaming will be implemented at a later date. And also, 5G roaming has been implemented according to the document below.

```
TS29.500
6 General Functionalities in Service Based Architecture
6.1 Routing Mechanisms
6.1.4.3 Routing across PLMN
```

Let's start the explanation assuming that the connection test of the UE to this single host has been completed.

## 1. Your First Roaming with Test Program

#### Build & Install

We'll start with the `roaming-tier2` branch from now on.
```
$ git clone https://github.com/open5gs/open5gs
$ cd open5gs
$ git checkout roaming-tier2
$ meson build --prefix=`pwd`/install
$ ninja -C build install
```

#### Configure FQDN to DNS

For routing to Home PLMN, NRF, AUSF and UDM address in H-PLMN shall use FQDN format, so please edit the /etc/hosts file as follows.

```diff
$ diff -u hosts /etc/hosts
--- hosts	2023-07-09 17:26:50.775626909 +0900
+++ /etc/hosts	2023-07-09 17:27:14.941277632 +0900
@@ -1,6 +1,13 @@
127.0.0.1	localhost
127.0.1.1	open5gs

+127.0.3.10	nrf.5gc.mnc001.mcc001.3gppnetwork.org
+127.0.3.11	ausf.5gc.mnc001.mcc001.3gppnetwork.org
+127.0.3.12	udm.5gc.mnc001.mcc001.3gppnetwork.org
+
+127.0.1.15	sepp1.localdomain
+127.0.2.15	sepp2.localdomain
+
# The following lines are desirable for IPv6 capable hosts
::1     ip6-localhost ip6-loopback
fe00::0 ip6-localnet
```

Note that we are using 5gc.mnc010.mcc001.3gppnetwork.org for our home network domain for NRF, AUSF and UDM. However, SEPP can be routed without following this home network domain naming, so sepp.localdomain is used for convenience.

#### Running V-PLMN 5G Core and H-PLMN 5G Core in the Single Host

H-PLMN 5G Core requires root privileges as it uses reserved ports such as http(80) or https(443).

```
$ sudo ./build/tests/app/5gc -c ./build/configs/examples/roaming-lbo-hplmn.yaml
Open5GS daemon v2.6.4-15-gab42065
```

Since V-PLMN Core uses port 7777, it can be run without root privileges.
```
$ ./build/tests/app/5gc -c ./build/configs/examples/roaming-lbo-vplmn.yaml 
Open5GS daemon v2.6.4-15-gab42065
```

Performs a test of UE access while roaming subscribed to H-PLMN.
```
./build/tests/registration/registration -c ./build/configs/examples/001-01.yaml simple-test
```

A UE subscribed to V-PLMN connects without roaming.
```
./build/tests/registration/registration -c ./build/configs/examples/999-01.yaml simple-test
```

## 2. Roaming Deployment

#### vNRF and hNRF
We plan to use 001-01 for the Home PLMN and 999-70 for the Visited PLMN. To do so, serving PLMN ID shall be set in the NRF. NF PLMN ID is configured with the same PLMN ID as the NRF during the NFProfile registration process.

Note that visited NRF can use IP address, but home NRF should follow TS23.003(28.3.2.3.2 Format of NRF FQDN) for routing.

- Create h-nrf.yaml for Home PLMN

```
$ cp install/etc/open5gs/nrf.yaml install/etc/open5gs/h-nrf.yaml
```

- Update h-nrf.yaml

```diff
$ diff -u install/etc/open5gs/nrf.yaml install/etc/open5gs/h-nrf.yaml
--- install/etc/open5gs/nrf.yaml	2023-08-06 22:46:02.263941577 +0900
+++ install/etc/open5gs/h-nrf.yaml	2023-08-06 23:08:22.562644531 +0900
@@ -9,11 +9,10 @@
nrf:
    serving:  # 5G roaming requires PLMN in NRF
      - plmn_id:
-          mcc: 999
-          mnc: 70
+          mcc: 001
+          mnc: 01
    sbi:
-      - address: 127.0.0.10
-        port: 7777
+      - address:  nrf.5gc.mnc001.mcc001.3gppnetwork.org

################################################################################
# SBI Server
```

- Run vNRF and hNRF

```
$ ./install/bin/open5gs-nrfd
$ sudo ./install/bin/open5gs-nrfd -c ./install/etc/open5gs/h-nrf.yaml
```

Note that Home NRF requires root privileges as it uses reserved ports such as http (80) or https (443).


#### vSCP and hSCP

SCPs do not have to use FQDNs.

- Create h-scp.yaml for Home PLMN

```
$ cp install/etc/open5gs/scp.yaml install/etc/open5gs/h-scp.yaml
```

- Update h-scp.yaml

```diff
$ diff -u install/etc/open5gs/scp.yaml install/etc/open5gs/h-scp.yaml
--- install/etc/open5gs/scp.yaml	2023-08-06 22:46:02.279940823 +0900
+++ install/etc/open5gs/h-scp.yaml	2023-08-06 23:11:29.727177201 +0900
@@ -8,10 +8,10 @@

scp:
    sbi:
-      - address: 127.0.1.10
+      - address: 127.0.2.10
        port: 7777
    nrf:
-      - uri: http://127.0.0.10:7777
+      - uri: http://nrf.5gc.mnc001.mcc001.3gppnetwork.org

################################################################################
# SCP Info
```

- Update scp.yaml

The V-PLMN SCP needs TLS information to connect SEPP.

To connect SCP and NRF, the user directly specifies the client connection information in the configuration file. On the other hand, SEPP connection information can identify NF profile through NRF discovery function.

TLS information for connecting SCP and NRF can be set directly by the user in the configuration file, but not in SEPP. Therefore, in order for SCP to connect to SEPP by TLS authentication, the default configuration method as shown below should be used.

```diff
diff -u install/etc/open5gs/scp.yaml.old install/etc/open5gs/scp.yaml
--- install/etc/open5gs/scp.yaml.old	2023-08-06 23:45:34.184505318 +0900
+++ install/etc/open5gs/scp.yaml	2023-08-06 23:45:38.216322924 +0900
@@ -7,6 +7,11 @@
#    peer: 64

scp:
+    defconfig:
+      tls:
+        client:
+          scheme: https
+          cacert: /home/acetcom/Documents/git/open5gs/build/configs/open5gs/tls/ca.crt
    sbi:
      - address: 127.0.1.10
        port: 7777
```

- Run vSCP and hSCP

```
$ ./install/bin/open5gs-scpd
$ ./install/bin/open5gs-scpd -c ./install/etc/open5gs/h-scp.yaml
```

#### vSEPP and hSEPP

SEPP can be run without changing the configuration file. And SEPP uses HTTPS scheme with TLS in default setting. Please refer to the configuration file(sepp1.yaml/sepp2.yaml) for more details.

- Run vSEPP and hSEPP

```
$ ./install/bin/open5gs-seppd -c ./install/etc/open5gs/sepp1.yaml
$ ./install/bin/open5gs-seppd -c ./install/etc/open5gs/sepp2.yaml
```

As shown below, you can confirm that the SEPP n32c interface is connected.
```
INFO: [sepp1.localdomain] SEPP established (../src/sepp/handshake-sm.c:297)
```

#### hAUSF, hUDM and hUDR

Home UDR can use IP address, but Home AUSF and Home UDM shall use FQDN.

- Create h-udr.yaml, h-ausf.yaml and h-udm.yaml for Home PLMN

```
$ cp install/etc/open5gs/udr.yaml install/etc/open5gs/h-udr.yaml
$ cp install/etc/open5gs/ausf.yaml install/etc/open5gs/h-ausf.yaml
$ cp install/etc/open5gs/udm.yaml install/etc/open5gs/h-udm.yaml
```

- Update h-udr.yaml

```diff
$ diff -u install/etc/open5gs/udr.yaml install/etc/open5gs/h-udr.yaml
--- install/etc/open5gs/udr.yaml	2023-08-06 22:46:02.363936865 +0900
+++ install/etc/open5gs/h-udr.yaml	2023-08-06 23:26:45.195462712 +0900
@@ -9,12 +9,12 @@

udr:
    sbi:
-      - address: 127.0.0.20
+      - address: 127.0.3.20
        port: 7777
#    nrf:
#      - uri: http://127.0.0.10:7777
    scp:
-      - uri: http://127.0.1.10:7777
+      - uri: http://127.0.2.10:7777

################################################################################
# SBI Server
```

- Update h-ausf.yaml

```diff
$ diff -u install/etc/open5gs/ausf.yaml install/etc/open5gs/h-ausf.yaml
--- install/etc/open5gs/ausf.yaml	2023-08-06 22:46:02.327938562 +0900
+++ install/etc/open5gs/h-ausf.yaml	2023-08-06 23:27:08.614413521 +0900
@@ -8,12 +8,11 @@

ausf:
    sbi:
-      - address: 127.0.0.11
-        port: 7777
+      - address: ausf.5gc.mnc001.mcc001.3gppnetwork.org
#    nrf:
#      - uri: http://127.0.0.10:7777
    scp:
-      - uri: http://127.0.1.10:7777
+      - uri: http://127.0.2.10:7777

################################################################################
# SBI Server
```

- Update h-udm.yaml

```diff
diff -u install/etc/open5gs/udm.yaml install/etc/open5gs/h-udm.yaml
--- install/etc/open5gs/udm.yaml	2023-08-06 22:46:02.347937619 +0900
+++ install/etc/open5gs/h-udm.yaml	2023-08-06 23:27:25.177671001 +0900
@@ -27,12 +27,11 @@
        scheme: 2
        key: /home/acetcom/Documents/git/open5gs/install/etc/open5gs/hnet/secp256r1-6.key
    sbi:
-      - address: 127.0.0.12
-        port: 7777
+      - address: udm.5gc.mnc001.mcc001.3gppnetwork.org
#    nrf:
#      - uri: http://127.0.0.10:7777
    scp:
-      - uri: http://127.0.1.10:7777
+      - uri: http://127.0.2.10:7777

#
################################################################################
```

```
$ ./install/bin/open5gs-udrd -c ./install/etc/open5gs/h-udr.yaml
$ sudo ./install/bin/open5gs-ausfd -c ./install/etc/open5gs/h-ausf.yaml
$ sudo ./install/bin/open5gs-udmd -c ./install/etc/open5gs/h-udm.yaml
```

Like hNRF, hAUSF and hUDM also require root privileges.

#### Run all NFs in Visited PLMN

Since NFs in the V-PLMN can use IP addresses, we will use the default configuration.

```
$ ./install/bin/open5gs-amfd
$ ./install/bin/open5gs-smfd
$ ./install/bin/open5gs-upfd
$ ./install/bin/open5gs-ausfd -l ausf.log
$ ./install/bin/open5gs-udmd -l udm.log
$ ./install/bin/open5gs-pcfd
$ ./install/bin/open5gs-nssfd
$ ./install/bin/open5gs-bsfd
$ ./install/bin/open5gs-udrd -l udr.log
```

Since AUSF/UDM/UDR runs on Home PLMN, the log location is re-configured with the -l option to avoid log conflicts.

#### Test Roaming
Performs a test of UE access while roaming subscribed to H-PLMN.
```
./build/tests/registration/registration -c ./build/configs/examples/001-01.yaml simple-test
```

A UE subscribed to V-PLMN connects without roaming.
```
./build/tests/registration/registration -c ./build/configs/examples/999-01.yaml simple-test
```

You can see the sample traffic.  -- [[5g-roaming-lbo.pcapng]]({{ site.url }}{{ site.baseurl }}/assets/pcapng/5g_roaming_lbo.pcapng).
