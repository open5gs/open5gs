---
title: Roaming
---

## 0. Introduction

This document explains how to install and test 5G Core Roaming using Open5GS. Since roaming cannot be tested with UERANSIM, we use Open5GS's built-in test environment.

Open5GS supports both LBO (Local Breakout) and HR (Home Routed) roaming methods. The 5G roaming implementation follows the specifications outlined in the document below:
```
TS29.500
6 General Functionalities in Service Based Architecture
6.1 Routing Mechanisms
6.1.4.3 Routing across PLMN
```

This explanation assumes that you have already completed the UE connection test on a single host.

## 1. Your First Roaming with Test Program

#### Build & Install

Git clone, compile and install:
```
$ git clone https://github.com/open5gs/open5gs
$ cd open5gs
$ meson build --prefix=`pwd`/install
$ ninja -C build install
```

#### Configure DNS for FQDN Resolution

For routing to the Home PLMN, the NRF, AUSF, and UDM addresses in H-PLMN must use FQDN format. Additionally, for HR roaming, NSSF, SMF, and BSF must also use FQDN format. Please edit the /etc/hosts file as follows:

```diff
--- hosts	2025-07-15 08:33:36.594334895 +0900
+++ /etc/hosts	2025-07-15 08:46:53.727356793 +0900
@@ -1,6 +1,27 @@
 127.0.0.1	localhost
 127.0.1.1	open5gs

+127.0.1.10	nrf.5gc.mnc070.mcc999.3gppnetwork.org
+127.0.1.11	ausf.5gc.mnc070.mcc999.3gppnetwork.org
+127.0.1.12	udm.5gc.mnc070.mcc999.3gppnetwork.org
+127.0.1.14	nssf.5gc.mnc070.mcc999.3gppnetwork.org
+127.0.1.4	smf.5gc.mnc070.mcc999.3gppnetwork.org
+127.0.1.15	bsf.5gc.mnc070.mcc999.3gppnetwork.org
+
+127.0.2.10	nrf.5gc.mnc001.mcc001.3gppnetwork.org
+127.0.2.11	ausf.5gc.mnc001.mcc001.3gppnetwork.org
+127.0.2.12	udm.5gc.mnc001.mcc001.3gppnetwork.org
+127.0.2.14	nssf.5gc.mnc001.mcc001.3gppnetwork.org
+127.0.2.4	smf.5gc.mnc001.mcc001.3gppnetwork.org
+127.0.2.15	baf.5gc.mnc001.mcc001.3gppnetwork.org
+
+127.0.3.10	nrf.5gc.mnc010.mcc315.3gppnetwork.org
+127.0.3.11	ausf.5gc.mnc010.mcc315.3gppnetwork.org
+127.0.3.12	udm.5gc.mnc010.mcc315.3gppnetwork.org
+127.0.3.14	nssf.5gc.mnc010.mcc315.3gppnetwork.org
+127.0.3.4	smf.5gc.mnc010.mcc315.3gppnetwork.org
+127.0.3.15	bsf.5gc.mnc010.mcc315.3gppnetwork.org
+
 # The following lines are desirable for IPv6 capable hosts
 ::1     ip6-localhost ip6-loopback
 fe00::0 ip6-localnet
```

#### Run the V-PLMN 5G Core and H-PLMN 5G Core on a single host

5G Core requires root privileges as it uses reserved ports such as http(80) or https(443).

```
$ sudo ./build/tests/app/5gc -c ./build/configs/examples/5gc-sepp1-999-70.yaml
$ sudo ./build/tests/app/5gc -c ./build/configs/examples/5gc-sepp2-001-01.yaml
$ sudo ./build/tests/app/5gc -c ./build/configs/examples/5gc-sepp3-315-010.yaml
```

Test UE access while roaming with UEs subscribed to different H-PLMNs. The same test commands work for both LBO and HR roaming scenarios - the roaming type is determined by the network configuration and policies:
```
$ ./build/tests/registration/registration -c ./build/configs/examples/gnb-999-70-ue-001-01.yaml simple-test
$ ./build/tests/registration/registration -c ./build/configs/examples/gnb-999-70-ue-315-010.yaml simple-test
$ ./build/tests/registration/registration -c ./build/configs/examples/gnb-001-01-ue-999-70.yaml simple-test
$ ./build/tests/registration/registration -c ./build/configs/examples/gnb-001-01-ue-315-010.yaml simple-test
$ ./build/tests/registration/registration -c ./build/configs/examples/gnb-315-010-ue-999-70.yaml simple-test
$ ./build/tests/registration/registration -c ./build/configs/examples/gnb-315-010-ue-001-01.yaml simple-test
```

You can see the sample traffic for both roaming types:
- LBO Roaming: [[5g-roaming-lbo.pcapng]]({{ site.url }}{{ site.baseurl }}/assets/pcapng/5g-roaming-lbo.pcapng)
- HR Roaming: [[5g-roaming-hr.pcapng]]({{ site.url }}{{ site.baseurl }}/assets/pcapng/5g-roaming-hr.pcapng)


## 2. Roaming Test on a Single Host

Open5GS now supports both roaming architectures:

1. **LBO (Local Breakout)**: User plane traffic is routed locally through the visited network's UPF
2. **HR (Home Routed)**: User plane traffic is routed back to the home network's UPF through the visited network

The choice between LBO and HR is determined by subscriber configuration and roaming agreements. In Open5GS WebUI, you can configure the roaming type for each subscriber session using the `lbo_roaming_allowed` parameter:

- **LBO Roaming**: Set `Subscriber.slice.session.lbo_roaming_allowed` to `true`
- **HR Roaming**: Set `Subscriber.slice.session.lbo_roaming_allowed` to `false` or leave it unset (default behavior)

This allows flexible per-session roaming policy configuration based on subscriber profiles and service requirements.

### Home PLMN

The NRF must include PLMN information when serving in a 5G roaming environment. This enables proper network function discovery and routing by providing the necessary PLMN context for roaming subscribers.

**Create h-nrf.yaml:**

```bash
$ sh -c 'cat << EOF > ./install/etc/open5gs/h-nrf.yaml
logger:
  file:
    path: /home/acetcom/Documents/git/open5gs/install/var/log/open5gs/h-nrf.log
#  level: info   # fatal|error|warn|info(default)|debug|trace

global:
  max:
    ue: 1024  # The number of UE can be increased depending on memory size.
#    peer: 64

nrf:
  serving:  # 5G roaming requires PLMN in NRF
    - plmn_id:
        mcc: 999
        mnc: 70
  sbi:
    server:
      - address: nrf.5gc.mnc070.mcc999.3gppnetwork.org
EOF'
```

The SCP acts as a communication proxy between different network functions, enabling indirect communication and service discovery in the 5G Service Based Architecture.

**Create h-scp.yaml:**

```bash
$ sh -c 'cat << EOF > ./install/etc/open5gs/h-scp.yaml
logger:
  file:
    path: /home/acetcom/Documents/git/open5gs/install/var/log/open5gs/h-scp.log
#  level: info   # fatal|error|warn|info(default)|debug|trace

global:
  max:
    ue: 1024  # The number of UE can be increased depending on memory size.
#    peer: 64

scp:
  sbi:
    server:
      - address: 127.0.1.200
        port: 7777
    client:
      nrf:
        - uri: http://nrf.5gc.mnc070.mcc999.3gppnetwork.org
EOF'
```

AUSF handles authentication procedures for UE access. It must use FQDN in the Home PLMN for proper authentication in roaming scenarios, ensuring that roaming subscribers can be properly authenticated by the home network.

**Update ausf.yaml:**

```diff
diff -u ./install/etc/open5gs/ausf.yaml.old ./install/etc/open5gs/ausf.yaml
--- ./install/etc/open5gs/ausf.yaml.old	2023-11-19 17:50:12.469116283 +0900
+++ ./install/etc/open5gs/ausf.yaml	2023-11-19 17:52:35.201116202 +0900
@@ -10,13 +10,12 @@
 ausf:
   sbi:
     server:
-      - address: 127.0.0.11
-        port: 7777
+      - address: ausf.5gc.mnc070.mcc999.3gppnetwork.org
     client:
 #      nrf:
 #        - uri: http://127.0.0.10:7777
       scp:
-        - uri: http://127.0.0.200:7777
+        - uri: http://127.0.1.200:7777

 ################################################################################
 # SBI Server
```

UDM handles user data management and subscription information. It must also use Home PLMN FQDN to ensure proper user data retrieval and management for roaming subscribers.

**Update udm.yaml:**

```diff
$ diff -u ./install/etc/open5gs/udm.yaml.old ./install/etc/open5gs/udm.yaml
--- ./install/etc/open5gs/udm.yaml.old	2023-11-19 17:50:17.713116280 +0900
+++ ./install/etc/open5gs/udm.yaml	2023-11-19 17:52:40.701116199 +0900
@@ -29,13 +29,12 @@
       key: /home/acetcom/Documents/git/open5gs/install/etc/open5gs/hnet/secp256r1-6.key
   sbi:
     server:
-      - address: 127.0.0.12
-        port: 7777
+      - address: udm.5gc.mnc070.mcc999.3gppnetwork.org
     client:
 #      nrf:
 #        - uri: http://127.0.0.10:7777
       scp:
-        - uri: http://127.0.0.200:7777
+        - uri: http://127.0.1.200:7777

 #
 ################################################################################
```

UDR stores subscription data and user profiles. It needs updated SCP client configuration to properly communicate with other network functions through the service communication proxy.

**Update udr.yaml:**

```diff
$ diff -u ./install/etc/open5gs/udr.yaml.old ./install/etc/open5gs/udr.yaml
--- ./install/etc/open5gs/udr.yaml.old	2023-11-19 18:00:27.049115935 +0900
+++ ./install/etc/open5gs/udr.yaml	2023-11-19 18:00:31.713115932 +0900
@@ -17,7 +17,7 @@
 #      nrf:
 #        - uri: http://127.0.0.10:7777
       scp:
-        - uri: http://127.0.0.200:7777
+        - uri: http://127.0.1.200:7777

 ################################################################################
 # SBI Server
```

SMF manages PDU sessions and requires comprehensive configuration for data plane handling. It coordinates with UPF for user plane traffic and handles session establishment, modification, and termination.

**Create h-smf.yaml:**

```bash
$ sh -c 'cat << EOF > ./install/etc/open5gs/h-smf.yaml
logger:
  file:
    path: /home/acetcom/Documents/git/open5gs/install/var/log/open5gs/smf.log
#  level: info   # fatal|error|warn|info(default)|debug|trace

global:
  max:
    ue: 1024  # The number of UE can be increased depending on memory size.
#    peer: 64

smf:
  sbi:
    server:
      - address: smf.5gc.mnc070.mcc999.3gppnetwork.org
    client:
#      nrf:
#        - uri: http://127.0.1.10:7777
      scp:
        - uri: http://127.0.1.200:7777
  pfcp:
    server:
      - address: 127.0.1.4
    client:
      upf:
        - address: 127.0.1.7
  gtpc:
    server:
      - address: 127.0.1.4
  gtpu:
    server:
      - address: 127.0.1.4
  metrics:
    server:
      - address: 127.0.1.4
        port: 9090
  session:
    - subnet: 10.46.0.0/16
      gateway: 10.46.0.1
    - subnet: 2001:db8:babe::/48
      gateway: 2001:db8:babe::1
  dns:
    - 8.8.8.8
    - 8.8.4.4
    - 2001:4860:4860::8888
    - 2001:4860:4860::8844
  mtu: 1400
EOF'
```

UPF handles user data forwarding and requires coordination with SMF. It manages the user plane traffic routing and forwarding, including packet inspection and QoS enforcement.

**Create h-upf.yaml:**

```bash
$ sh -c 'cat << EOF > ./install/etc/open5gs/h-upf.yaml
logger:
  file:
    path: /home/acetcom/Documents/git/open5gs/install/var/log/open5gs/upf.log
#  level: info   # fatal|error|warn|info(default)|debug|trace

global:
  max:
    ue: 1024  # The number of UE can be increased depending on memory size.
#    peer: 64

upf:
  pfcp:
    server:
      - address: 127.0.1.7
    client:
#      smf:     #  UPF PFCP Client try to associate SMF PFCP Server
#        - address: 127.0.1.4
  gtpu:
    server:
      - address: 127.0.1.7
  session:
    - subnet: 10.46.0.0/16
      gateway: 10.46.0.1
      dev: ogstun2
    - subnet: 2001:db8:babe::/48
      gateway: 2001:db8:babe::1
      dev: ogstun2
  metrics:
    server:
      - address: 127.0.1.7
        port: 9090
EOF'
```

PCF provides policy control and charging rules for subscriber sessions. It determines QoS policies, charging rules, and session management policies based on subscriber profiles and network conditions.

**Create h-pcf.yaml:**

```bash
$ sh -c 'cat << EOF > ./install/etc/open5gs/h-pcf.yaml
db_uri: mongodb://localhost/open5gs
logger:
  file:
    path: /home/acetcom/Documents/git/open5gs/install/var/log/open5gs/pcf.log
#  level: info   # fatal|error|warn|info(default)|debug|trace

global:
  max:
    ue: 1024  # The number of UE can be increased depending on memory size.
#    peer: 64

pcf:
  sbi:
    server:
      - address: 127.0.1.13
        port: 7777
    client:
#      nrf:
#        - uri: http://127.0.0.10:7777
      scp:
        - uri: http://127.0.1.200:7777
  metrics:
    server:
      - address: 127.0.1.13
        port: 9090
EOF'
```

BSF manages binding information for network functions. It maintains bindings between different network function instances and provides binding discovery services for dynamic network function selection.

**Create h-bsf.yaml:**

```bash
$ sh -c 'cat << EOF > ./install/etc/open5gs/h-bsf.yaml
logger:
  file:
    path: /home/acetcom/Documents/git/open5gs/install/var/log/open5gs/bsf.log
#  level: info   # fatal|error|warn|info(default)|debug|trace

global:
  max:
    ue: 1024  # The number of UE can be increased depending on memory size.
#    peer: 64

bsf:
  sbi:
    server:
      - address: 127.0.1.15
        port: 7777
    client:
#      nrf:
#        - uri: http://127.0.0.10:7777
      scp:
        - uri: http://127.0.1.200:7777
EOF'
```

NSSF handles network slice selection and must use FQDN for proper slice identification. It determines which network slice instance should serve a particular UE based on subscription information and network slice availability.

**Create h-nssf.yaml:**

```bash
$ sh -c 'cat << EOF > ./install/etc/open5gs/h-nssf.yaml
logger:
  file:
    path: /home/acetcom/Documents/git/open5gs/install/var/log/open5gs/nssf.log
#  level: info   # fatal|error|warn|info(default)|debug|trace

global:
  max:
    ue: 1024  # The number of UE can be increased depending on memory size.
#    peer: 64

nssf:
  sbi:
    server:
      - address: nssf.5gc.mnc070.mcc999.3gppnetwork.org
    client:
#      nrf:
#        - uri: http://127.0.0.10:7777
      scp:
        - uri: http://127.0.1.200:7777
      nsi:
        - uri: http://nrf.5gc.mnc070.mcc999.3gppnetwork.org
          s_nssai:
            sst: 1
EOF'
```


SEPP handles security for inter-PLMN communication and requires SCP client update. It provides security functions for N32 interface communication between different PLMN networks, including message filtering and topology hiding.

**Update sepp1.yaml:**

```diff
$ diff -u ./install/etc/open5gs/sepp1.yaml.old ./install/etc/open5gs/sepp1.yaml
--- ./install/etc/open5gs/sepp1.yaml.old	2023-11-19 19:11:02.293113538 +0900
+++ ./install/etc/open5gs/sepp1.yaml	2023-11-19 19:11:22.429113526 +0900
@@ -23,7 +23,7 @@
 #      nrf:
 #        - uri: http://127.0.0.10:7777
       scp:
-        - uri: http://127.0.0.200:7777
+        - uri: http://127.0.1.200:7777
   n32:
     server:
       - sender: sepp1.localdomain
```

### Visited PLMN

The visited network NRF must be configured with the visited PLMN ID and use appropriate FQDN for network function registration and discovery.

**Update nrf.yaml:**

```diff
$ diff -u ./install/etc/open5gs/nrf.yaml ./install.new/etc/open5gs/nrf.yaml
--- ./install/etc/open5gs/nrf.yaml	2025-07-18 21:57:51.244553083 +0900
+++ ./install.new/etc/open5gs/nrf.yaml	2025-07-18 21:32:57.596880880 +0900
@@ -11,12 +11,11 @@
 nrf:
   serving:  # 5G roaming requires PLMN in NRF
     - plmn_id:
-        mcc: 999
-        mnc: 70
+        mcc: 001
+        mnc: 01
   sbi:
     server:
-      - address: 127.0.0.10
-        port: 7777
+      - address: nrf.5gc.mnc001.mcc001.3gppnetwork.org

 ################################################################################
 # SBI Server
```

The visited network SCP must be updated to reference the visited network NRF for proper service discovery and communication routing.

**Update scp.yaml:**

```diff
$ diff -u ./install/etc/open5gs/scp.yaml ./install.new/etc/open5gs/scp.yaml
--- ./install/etc/open5gs/scp.yaml	2025-07-18 21:57:51.262552520 +0900
+++ ./install.new/etc/open5gs/scp.yaml	2025-07-18 21:33:54.670492255 +0900
@@ -15,7 +15,7 @@
         port: 7777
     client:
       nrf:
-        - uri: http://127.0.0.10:7777
+        - uri: http://nrf.5gc.mnc001.mcc001.3gppnetwork.org

 ################################################################################
 # SCP Info
 ```

AMF handles access and mobility management for UEs. The visited network AMF must be configured to support both home and visited PLMN access control, and handle roaming subscribers appropriately.

**Update amf.yaml:**

```diff
$ diff -u ./install/etc/open5gs/amf.yaml ./install.new/etc/open5gs/amf.yaml
--- ./install/etc/open5gs/amf.yaml	2025-07-18 21:57:51.173555303 +0900
+++ ./install.new/etc/open5gs/amf.yaml	2025-07-18 21:05:05.396288769 +0900
@@ -20,27 +20,34 @@
         - uri: http://127.0.0.200:7777
   ngap:
     server:
-      - address: 127.0.0.5
+      - address: 127.0.2.5
   metrics:
     server:
       - address: 127.0.0.5
         port: 9090
-  guami:
+  access_control:
+    - plmn_id:
+        mcc: 001
+        mnc: 01
     - plmn_id:
         mcc: 999
         mnc: 70
+  guami:
+    - plmn_id:
+        mcc: 001
+        mnc: 01
       amf_id:
         region: 2
         set: 1
   tai:
     - plmn_id:
-        mcc: 999
-        mnc: 70
+        mcc: 001
+        mnc: 01
       tac: 1
   plmn_support:
     - plmn_id:
-        mcc: 999
-        mnc: 70
+        mcc: 001
+        mnc: 01
       s_nssai:
         - sst: 1
   security:
```

The visited network SMF must use the visited PLMN FQDN for proper session management and coordination with other network functions.

**Update smf.yaml:**

```diff
$ diff -u ./install/etc/open5gs/smf.yaml ./install.new/etc/open5gs/smf.yaml
--- ./install/etc/open5gs/smf.yaml	2025-07-18 21:57:51.154555897 +0900
+++ ./install.new/etc/open5gs/smf.yaml	2025-07-18 21:28:29.423632460 +0900
@@ -11,8 +11,7 @@
 smf:
   sbi:
     server:
-      - address: 127.0.0.4
-        port: 7777
+      - address: smf.5gc.mnc001.mcc001.3gppnetwork.org
     client:
 #      nrf:
 #        - uri: http://127.0.0.10:7777
 ```


Due to the absence of UDR in the visiting network, V-PCF uses locally configured policies. When the UE is located in the home PLMN (999/70), MongoDB is used. When the UE is located in the visiting PLMN (001/01), locally configured policies are used based on roaming agreements.

**Update pcf.yaml:**

```diff
$ diff --git a/configs/open5gs/pcf.yaml.in b/configs/open5gs/pcf.yaml.in
index 4c5d103f9..d16e85728 100644
--- a/configs/open5gs/pcf.yaml.in
+++ b/configs/open5gs/pcf.yaml.in
@@ -23,6 +23,84 @@ pcf:
     server:
       - address: 127.0.0.13
         port: 9090
+  policy:
+    - supi_range:
+        - 999700000000001-999709999999999
+      slice:
+        - sst: 1  # 1,2,3,4
+          default_indicator: true
+          session:
+            - name: internet
+              type: 3  # 1:IPv4, 2:IPv6, 3:IPv4v6
+              ambr:
+                downlink:
+                  value: 1
+                  unit: 3  # 0:bps, 1:Kbps, 2:Mbps, 3:Gbps, 4:Tbps
+                uplink:
+                  value: 1
+                  unit: 3
+              qos:
+                index: 9  # 1, 2, 3, 4, 65, 66, 67, 75, 71, 72, 73, 74, 76, 5, 6, 7, 8, 9, 69, 70, 79, 80, 82, 83, 84, 85, 86
+                arp:
+                  priority_level: 8  # 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
+                  pre_emption_vulnerability: 1  # 1: Disabled, 2:Enabled
+                  pre_emption_capability: 1  # 1: Disabled, 2:Enabled
+            - name: ims
+              type: 3  # 1:IPv4, 2:IPv6, 3:IPv4v6
+              ambr:
+                downlink:
+                  value: 1
+                  unit: 3  # 0:bps, 1:Kbps, 2:Mbps, 3:Gbps, 4:Tbps
+                uplink:
+                  value: 1
+                  unit: 3  # 0:bps, 1:Kbps, 2:Mbps, 3:Gbps, 4:Tbps
+              qos:
+                index: 5  # 1, 2, 3, 4, 65, 66, 67, 75, 71, 72, 73, 74, 76, 5, 6, 7, 8, 9, 69, 70, 79, 80, 82, 83, 84, 85, 86
+                arp:
+                  priority_level: 1  # 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
+                  pre_emption_vulnerability: 1  # 1: Disabled, 2:Enabled
+                  pre_emption_capability: 1  # 1: Disabled, 2:Enabled
+              pcc_rule:
+                - qos:
+                    index: 1  # 1, 2, 3, 4, 65, 66, 67, 75, 71, 72, 73, 74, 76, 5, 6, 7, 8, 9, 69, 70, 79, 80, 82, 83, 84, 85, 86
+                    arp:
+                      priority_level: 1  # 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
+                      pre_emption_vulnerability: 1   # 1: Disabled, 2:Enabled
+                      pre_emption_capability: 1   # 1: Disabled, 2:Enabled
+                    mbr:
+                      downlink:
+                        value: 82
+                        unit: 1  # 0:bps, 1:Kbps, 2:Mbps, 3:Gbps, 4:Tbps
+                      uplink:
+                        value: 82
+                        unit: 1  # 0:bps, 1:Kbps, 2:Mbps, 3:Gbps, 4:Tbps
+                    gbr:
+                      downlink:
+                        value: 82
+                        unit: 1  # 0:bps, 1:Kbps, 2:Mbps, 3:Gbps, 4:Tbps
+                      uplink:
+                        value: 82
+                        unit: 1  # 0:bps, 1:Kbps, 2:Mbps, 3:Gbps, 4:Tbps
+                - qos:
+                    index: 2  # 1, 2, 3, 4, 65, 66, 67, 75, 71, 72, 73, 74, 76, 5, 6, 7, 8, 9, 69, 70, 79, 80, 82, 83, 84, 85, 86
+                    arp:
+                      priority_level: 4  # 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
+                      pre_emption_vulnerability: 2   # 1: Disabled, 2:Enabled
+                      pre_emption_capability: 2   # 1: Disabled, 2:Enabled
+                    mbr:
+                      downlink:
+                        value: 802
+                        unit: 1  # 0:bps, 1:Kbps, 2:Mbps, 3:Gbps, 4:Tbps
+                      uplink:
+                        value: 802
+                        unit: 1  # 0:bps, 1:Kbps, 2:Mbps, 3:Gbps, 4:Tbps
+                    gbr:
+                      downlink:
+                        value: 802
+                        unit: 1  # 0:bps, 1:Kbps, 2:Mbps, 3:Gbps, 4:Tbps
+                      uplink:
+                        value: 802
+                        unit: 1  # 0:bps, 1:Kbps, 2:Mbps, 3:Gbps, 4:Tbps

 ################################################################################
 # PCF Policy Configuration: UE Home PLMN and SUPI Range Based Policies
```

The visited network BSF requires updated addresses to align with the visited network addressing scheme.

**Update bsf.yaml:**

```diff
$ diff -u ./install/etc/open5gs/bsf.yaml ./install.new/etc/open5gs/bsf.yaml
--- ./install/etc/open5gs/bsf.yaml	2025-07-18 21:57:51.405548049 +0900
+++ ./install.new/etc/open5gs/bsf.yaml	2025-07-18 21:30:49.312390532 +0900
@@ -11,13 +11,13 @@
 bsf:
   sbi:
     server:
-      - address: 127.0.0.15
+      - address: 127.0.1.15
         port: 7777
     client:
 #      nrf:
 #        - uri: http://127.0.0.10:7777
       scp:
-        - uri: http://127.0.0.200:7777
+        - uri: http://127.0.1.200:7777

 ################################################################################
 # SBI Server
```
The visited network NSSF must use the visited PLMN FQDN and reference the appropriate NRF for network slice selection services.

**Update nssf.yaml:**

```diff
diff -u ./install/etc/open5gs/nssf.yaml ./install.new/etc/open5gs/nssf.yaml
--- ./install/etc/open5gs/nssf.yaml	2025-07-18 21:57:51.387548612 +0900
+++ ./install.new/etc/open5gs/nssf.yaml	2025-07-18 21:21:33.415221920 +0900
@@ -11,15 +11,14 @@
 nssf:
   sbi:
     server:
-      - address: 127.0.0.14
-        port: 7777
+      - address: nssf.5gc.mnc001.mcc001.3gppnetwork.org
     client:
 #      nrf:
 #        - uri: http://127.0.0.10:7777
       scp:
         - uri: http://127.0.0.200:7777
       nsi:
-        - uri: http://127.0.0.10:7777
+        - uri: http://nrf.5gc.mnc001.mcc001.3gppnetwork.org
           s_nssai:
             sst: 1
 ################################################################################
```

### Run the V-PLMN 5G Core and H-PLMN 5G Core on a single host

#### Home Network

5G Core requires root privileges as it uses reserved ports such as http(80) or https(443).

```
$ sudo ./install/bin/open5gs-nrfd -c ./install/etc/open5gs/h-nrf.yaml
$ ./install/bin/open5gs-scpd -c ./install/etc/open5gs/h-scp.yaml
$ sudo ./install/bin/open5gs-ausfd
$ sudo ./install/bin/open5gs-udmd
$ ./install/bin/open5gs-udrd
$ sudo ./install/bin/open5gs-smfd -c ./install/etc/open5gs/h-smf.yaml
$ ./install/bin/open5gs-upfd -c ./install/etc/open5gs/h-upf.yaml
$ ./install/bin/open5gs-pcfd -c ./install/etc/open5gs/h-pcf.yaml
$ ./install/bin/open5gs-bsfd -c ./install/etc/open5gs/h-bsf.yaml
$ sudo ./install/bin/open5gs-nssfd -c ./install/etc/open5gs/h-nssf.yaml
$ ./install/bin/open5gs-seppd -c ./install/etc/open5gs/sepp1.yaml
```

#### Visited Network
```
$ sudo ./install/bin/open5gs-nrfd
$ ./install/bin/open5gs-scpd
$ ./install/bin/open5gs-amfd
$ sudo ./install/bin/open5gs-smfd
$ ./install/bin/open5gs-upfd
$ ./install/bin/open5gs-pcfd
$ ./install/bin/open5gs-bsfd
$ sudo ./install/bin/open5gs-nssfd
$ ./install/bin/open5gs-seppd -c ./install/etc/open5gs/sepp2.yaml
```

### Performs a test of UE access while roaming subscribed to H-PLMN.
```
$ ./build/tests/registration/registration -c ./build/configs/examples/gnb-001-01-ue-999-70.yaml simple-test
```

## 3. Roaming Deployment

### VM and Subscriber Information

Each VMs are as follows.

- VM1
```
Hostname: sepp1.localdomain
PLMN-ID: MNC(999), MNC(70)
IP Address: 10.10.1.5
N32-c: 10.10.1.251
N32-c: 10.10.1.252
```

- VM2
```
Hostname: sepp2.localdomain
PLMN-ID: MNC(001), MNC(01)
IP Address: 10.10.2.5
N32-c: 10.10.2.251
N32-c: 10.10.2.252
```

- VM3
```
Hostname: sepp3.localdomain
PLMN-ID: MNC(003), MNC(03)
IP Address: 10.10.3.5
N32-c: 10.10.3.251
N32-c: 10.10.3.252
```

- Subscriber
```
imsi-999700000000001
imsi-001010000000001
imsi-315010000000001
```

### Setting for VM1

- Edit /etc/hosts

```diff
$ diff -u hosts.old hosts.new
--- hosts.old	2025-07-19 07:02:44.813666212 +0900
+++ hosts.new	2025-07-19 07:02:51.019489167 +0900
@@ -1,6 +1,13 @@
 127.0.0.1	localhost
 127.0.1.1	open5gs

+127.0.1.10	nrf.5gc.mnc070.mcc999.3gppnetwork.org
+127.0.1.11	ausf.5gc.mnc070.mcc999.3gppnetwork.org
+127.0.1.12	udm.5gc.mnc070.mcc999.3gppnetwork.org
+127.0.1.14	nssf.5gc.mnc070.mcc999.3gppnetwork.org
+127.0.1.4	smf.5gc.mnc070.mcc999.3gppnetwork.org
+127.0.1.15	bsf.5gc.mnc070.mcc999.3gppnetwork.org
+
 # The following lines are desirable for IPv6 capable hosts
 ::1     ip6-localhost ip6-loopback
 fe00::0 ip6-localnet
```

NRF shall follow TS23.003(28.3.2.3.2 Format of NRF FQDN) for routing.

- Update nrf.yaml

```diff
$ diff --git a/configs/open5gs/nrf.yaml.in b/configs/open5gs/nrf.yaml.in
index 3996b2bd9..e57f286b7 100644
--- a/configs/open5gs/nrf.yaml.in
+++ b/configs/open5gs/nrf.yaml.in
@@ -13,8 +13,7 @@ nrf:
           mnc: 70
     sbi:
       server:
-        - address: 127.0.0.10
-          port: 7777
+        - address: nrf.5gc.mnc070.mcc999.3gppnetwork.org

 ################################################################################
 # SBI Server
```

- Update scp.yaml

```diff
$ diff --git a/configs/open5gs/scp.yaml.in b/configs/open5gs/scp.yaml.in
index 9be6cdc93..eee7d3e3f 100644
--- a/configs/open5gs/scp.yaml.in
+++ b/configs/open5gs/scp.yaml.in
@@ -13,7 +13,7 @@ scp:
           port: 7777
       client:
         nrf:
-          - uri: http://127.0.0.10:7777
+          - uri: http://nrf.5gc.mnc070.mcc999.3gppnetwork.org

 ################################################################################
 # SCP Info
```

- Update nssf.yaml

```diff
$ diff --git a/configs/open5gs/nssf.yaml.in b/configs/open5gs/nssf.yaml.in
index 015085f56..92ceb63b5 100644
--- a/configs/open5gs/nssf.yaml.in
+++ b/configs/open5gs/nssf.yaml.in
@@ -11,15 +11,14 @@ global:
 nssf:
   sbi:
     server:
-      - address: 127.0.0.14
-        port: 7777
+      - address: nssf.5gc.mnc070.mcc999.3gppnetwork.org
     client:
 #      nrf:
 #        - uri: http://127.0.0.10:7777
       scp:
         - uri: http://127.0.0.200:7777
       nsi:
-        - uri: http://127.0.0.10:7777
+        - uri: http://nrf.5gc.mnc070.mcc999.3gppnetwork.org
           s_nssai:
             sst: 1
 ################################################################################
```

AUSF, UDM and SMF shall use FQDN in the Home PLMN.

- Update ausf.yaml

```diff
$ diff --git a/configs/open5gs/ausf.yaml.in b/configs/open5gs/ausf.yaml.in
index cd272cd3f..3e1cb67eb 100644
--- a/configs/open5gs/ausf.yaml.in
+++ b/configs/open5gs/ausf.yaml.in
@@ -9,8 +9,7 @@ max:
 ausf:
     sbi:
       server:
-        - address: 127.0.0.11
-          port: 7777
+        - address: ausf.5gc.mnc070.mcc999.3gppnetwork.org
       client:
 #        nrf:
 #          - uri: http://127.0.0.10:7777
```

- Update udm.yaml

```diff
$ diff --git a/configs/open5gs/udm.yaml.in b/configs/open5gs/udm.yaml.in
index ce650d5c2..ed756fc28 100644
--- a/configs/open5gs/udm.yaml.in
+++ b/configs/open5gs/udm.yaml.in
@@ -28,8 +28,7 @@ udm:
         key: @sysconfdir@/open5gs/hnet/secp256r1-6.key
     sbi:
       server:
-        - address: 127.0.0.12
-          port: 7777
+        - address: udm.5gc.mnc070.mcc999.3gppnetwork.org
       client:
 #        nrf:
 #          - uri: http://127.0.0.10:7777
```

- Update smf.yaml
```diff
$ diff --git a/configs/open5gs/smf.yaml.in b/configs/open5gs/smf.yaml.in
index e47752393..b6cee9ff8 100644
--- a/configs/open5gs/smf.yaml.in
+++ b/configs/open5gs/smf.yaml.in
@@ -11,8 +11,7 @@ global:
 smf:
   sbi:
     server:
-      - address: 127.0.0.4
-        port: 7777
+      - address: smf.5gc.mnc070.mcc999.3gppnetwork.org
     client:
 #      nrf:
 #        - uri: http://127.0.0.10:7777
```


AMF and UPF must use external IP addresses such as 10.10.1.x for communication between VM2 and VM3.

- Update amf.yaml

```diff
$ diff --git a/configs/open5gs/amf.yaml.in b/configs/open5gs/amf.yaml.in
index 938917e32..11a19c808 100644
--- a/configs/open5gs/amf.yaml.in
+++ b/configs/open5gs/amf.yaml.in
@@ -18,11 +18,21 @@ amf:
           - uri: http://127.0.0.200:7777
     ngap:
       server:
-        - address: 127.0.0.5
+        - address: 10.10.1.5
     metrics:
       server:
         - address: 127.0.0.5
           port: 9090
+    access_control:
+      - plmn_id:
+          mcc: 999
+          mnc: 70
+      - plmn_id:
+          mcc: 001
+          mnc: 01
+      - plmn_id:
+          mcc: 315
+          mnc: 010
     guami:
       - plmn_id:
           mcc: 999
```

- Update upf.yaml

```diff
$ diff --git a/configs/open5gs/upf.yaml.in b/configs/open5gs/upf.yaml.in
index e78b018f1..3032a06c6 100644
--- a/configs/open5gs/upf.yaml.in
+++ b/configs/open5gs/upf.yaml.in
@@ -15,7 +15,7 @@ upf:
 #          - address: 127.0.0.4
     gtpu:
       server:
-        - address: 127.0.0.7
+        - address: 10.10.1.5
     session:
       - subnet: 10.45.0.1/16
       - subnet: 2001:db8:cafe::1/48
```

Due to the absence of UDR in the visiting network, V-PCF uses locally configured policies. When the UE is located in the home PLMN (999/70), MongoDB is used. On the other hand, when the UE is located in the visiting PLMN (001/01, 315/010), locally configured policies are used. This is because there is no session management policy data for the UE in the visiting network, so locally configured information based on the roaming agreement is used.

- Update pcf.yaml

```diff
$ diff --git a/configs/open5gs/pcf.yaml.in b/configs/open5gs/pcf.yaml.in
index 4c5d103f9..c1526ff3a 100644
--- a/configs/open5gs/pcf.yaml.in
+++ b/configs/open5gs/pcf.yaml.in
@@ -23,6 +23,85 @@ pcf:
     server:
       - address: 127.0.0.13
         port: 9090
+  policy:
+    - supi_range:
+        - 001010000000001-001019999999999
+        - 315010000000001-315010999999999
+      slice:
+        - sst: 1  # 1,2,3,4
+          default_indicator: true
+          session:
+            - name: internet
+              type: 3  # 1:IPv4, 2:IPv6, 3:IPv4v6
+              ambr:
+                downlink:
+                  value: 1
+                  unit: 3  # 0:bps, 1:Kbps, 2:Mbps, 3:Gbps, 4:Tbps
+                uplink:
+                  value: 1
+                  unit: 3
+              qos:
+                index: 9  # 1, 2, 3, 4, 65, 66, 67, 75, 71, 72, 73, 74, 76, 5, 6, 7, 8, 9, 69, 70, 79, 80, 82, 83, 84, 85, 86
+                arp:
+                  priority_level: 8  # 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
+                  pre_emption_vulnerability: 1  # 1: Disabled, 2:Enabled
+                  pre_emption_capability: 1  # 1: Disabled, 2:Enabled
+            - name: ims
+              type: 3  # 1:IPv4, 2:IPv6, 3:IPv4v6
+              ambr:
+                downlink:
+                  value: 1
+                  unit: 3  # 0:bps, 1:Kbps, 2:Mbps, 3:Gbps, 4:Tbps
+                uplink:
+                  value: 1
+                  unit: 3  # 0:bps, 1:Kbps, 2:Mbps, 3:Gbps, 4:Tbps
+              qos:
+                index: 5  # 1, 2, 3, 4, 65, 66, 67, 75, 71, 72, 73, 74, 76, 5, 6, 7, 8, 9, 69, 70, 79, 80, 82, 83, 84, 85, 86
+                arp:
+                  priority_level: 1  # 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
+                  pre_emption_vulnerability: 1  # 1: Disabled, 2:Enabled
+                  pre_emption_capability: 1  # 1: Disabled, 2:Enabled
+              pcc_rule:
+                - qos:
+                    index: 1  # 1, 2, 3, 4, 65, 66, 67, 75, 71, 72, 73, 74, 76, 5, 6, 7, 8, 9, 69, 70, 79, 80, 82, 83, 84, 85, 86
+                    arp:
+                      priority_level: 1  # 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
+                      pre_emption_vulnerability: 1   # 1: Disabled, 2:Enabled
+                      pre_emption_capability: 1   # 1: Disabled, 2:Enabled
+                    mbr:
+                      downlink:
+                        value: 82
+                        unit: 1  # 0:bps, 1:Kbps, 2:Mbps, 3:Gbps, 4:Tbps
+                      uplink:
+                        value: 82
+                        unit: 1  # 0:bps, 1:Kbps, 2:Mbps, 3:Gbps, 4:Tbps
+                    gbr:
+                      downlink:
+                        value: 82
+                        unit: 1  # 0:bps, 1:Kbps, 2:Mbps, 3:Gbps, 4:Tbps
+                      uplink:
+                        value: 82
+                        unit: 1  # 0:bps, 1:Kbps, 2:Mbps, 3:Gbps, 4:Tbps
+                - qos:
+                    index: 2  # 1, 2, 3, 4, 65, 66, 67, 75, 71, 72, 73, 74, 76, 5, 6, 7, 8, 9, 69, 70, 79, 80, 82, 83, 84, 85, 86
+                    arp:
+                      priority_level: 4  # 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
+                      pre_emption_vulnerability: 2   # 1: Disabled, 2:Enabled
+                      pre_emption_capability: 2   # 1: Disabled, 2:Enabled
+                    mbr:
+                      downlink:
+                        value: 802
+                        unit: 1  # 0:bps, 1:Kbps, 2:Mbps, 3:Gbps, 4:Tbps
+                      uplink:
+                        value: 802
+                        unit: 1  # 0:bps, 1:Kbps, 2:Mbps, 3:Gbps, 4:Tbps
+                    gbr:
+                      downlink:
+                        value: 802
+                        unit: 1  # 0:bps, 1:Kbps, 2:Mbps, 3:Gbps, 4:Tbps
+                      uplink:
+                        value: 802
+                        unit: 1  # 0:bps, 1:Kbps, 2:Mbps, 3:Gbps, 4:Tbps

 ################################################################################
 # PCF Policy Configuration: UE Home PLMN and SUPI Range Based Policies
```

For now we will set up SEPP without using TLS.

- Create sepp.yaml

```bash
$ sh -c 'cat << EOF > ./install/etc/open5gs/sepp.yaml
logger:
    file:
      path: /home/acetcom/Documents/git/open5gs/install/var/log/open5gs/sepp.log
#    level: info   # fatal|error|warn|info(default)|debug|trace

max:
    ue: 1024  # The number of UE can be increased depending on memory size.
#    peer: 64

sepp:
    sbi:
      server:
        - address: 127.0.0.250
          port: 7777
      client:
        scp:
          - uri: http://127.0.0.200:7777
    n32:
      server:
        - sender: sepp1.localdomain
          address: 10.10.1.251
          port: 7777
          n32f:
            address: 10.10.1.252
            port: 7777
      client:
        sepp:
          - receiver: sepp2.localdomain
            uri: http://10.10.2.251:7777
            n32f:
              uri: http://10.10.2.252:7777
          - receiver: sepp3.localdomain
            uri: http://10.10.3.251:7777
            n32f:
              uri: http://10.10.3.252:7777
EOF'
```

NFs requires root privileges as it uses reserved ports such as http (80) or https (443).

- Run NFs in VM1

```bash
$ sudo ./install/bin/open5gs-nrfd
$ sudo ./install/bin/open5gs-scpd
$ sudo ./install/bin/open5gs-seppd
$ sudo ./install/bin/open5gs-amfd
$ sudo ./install/bin/open5gs-smfd
$ sudo ./install/bin/open5gs-upfd
$ sudo ./install/bin/open5gs-ausfd
$ sudo ./install/bin/open5gs-udmd
$ sudo ./install/bin/open5gs-pcfd
$ sudo ./install/bin/open5gs-nssfd
$ sudo ./install/bin/open5gs-bsfd
$ sudo ./install/bin/open5gs-udrd
```


### Setting for VM2

- Edit /etc/hosts

```diff
$ diff -u hosts.old hosts.new
--- hosts.old	2025-07-19 07:02:44.813666212 +0900
+++ hosts.new	2025-07-19 07:03:40.466147375 +0900
@@ -1,6 +1,13 @@
 127.0.0.1	localhost
 127.0.1.1	open5gs

+127.0.2.10	nrf.5gc.mnc001.mcc001.3gppnetwork.org
+127.0.2.11	ausf.5gc.mnc001.mcc001.3gppnetwork.org
+127.0.2.12	udm.5gc.mnc001.mcc001.3gppnetwork.org
+127.0.2.14	nssf.5gc.mnc001.mcc001.3gppnetwork.org
+127.0.2.4	smf.5gc.mnc001.mcc001.3gppnetwork.org
+127.0.2.15	baf.5gc.mnc001.mcc001.3gppnetwork.org
+
 # The following lines are desirable for IPv6 capable hosts
 ::1     ip6-localhost ip6-loopback
 fe00::0 ip6-localnet
```

NRF shall follow TS23.003(28.3.2.3.2 Format of NRF FQDN) for routing.

- Update nrf.yaml

```diff
$ diff --git a/configs/open5gs/nrf.yaml.in b/configs/open5gs/nrf.yaml.in
index 3996b2bd9..ab708abd8 100644
--- a/configs/open5gs/nrf.yaml.in
+++ b/configs/open5gs/nrf.yaml.in
@@ -9,12 +9,11 @@ max:
 nrf:
     serving:  # 5G roaming requires PLMN in NRF
       - plmn_id:
-          mcc: 999
-          mnc: 70
+          mcc: 001
+          mnc: 01
     sbi:
       server:
-        - address: 127.0.0.10
-          port: 7777
+        - address: nrf.5gc.mnc001.mcc001.3gppnetwork.org

 ################################################################################
 # SBI Server
```

- Update scp.yaml

```diff
$ diff --git a/configs/open5gs/scp.yaml.in b/configs/open5gs/scp.yaml.in
index 9be6cdc93..eee7d3e3f 100644
--- a/configs/open5gs/scp.yaml.in
+++ b/configs/open5gs/scp.yaml.in
@@ -13,7 +13,7 @@ scp:
           port: 7777
       client:
         nrf:
-          - uri: http://127.0.0.10:7777
+          - uri: http://nrf.5gc.mnc001.mcc001.3gppnetwork.org

 ################################################################################
 # SCP Info
```

- Update nssf.yaml

```diff
$ diff --git a/configs/open5gs/nssf.yaml.in b/configs/open5gs/nssf.yaml.in
index 015085f56..92ceb63b5 100644
--- a/configs/open5gs/nssf.yaml.in
+++ b/configs/open5gs/nssf.yaml.in
@@ -11,15 +11,14 @@ global:
 nssf:
   sbi:
     server:
-      - address: 127.0.0.14
-        port: 7777
+      - address: nssf.5gc.mnc001.mcc001.3gppnetwork.org
     client:
 #      nrf:
 #        - uri: http://127.0.0.10:7777
       scp:
         - uri: http://127.0.0.200:7777
       nsi:
-        - uri: http://127.0.0.10:7777
+        - uri: http://nrf.5gc.mnc001.mcc001.3gppnetwork.org
           s_nssai:
             sst: 1
 ################################################################################
```

AUSF, UDM and SMF shall use FQDN in the Home PLMN.

- Update ausf.yaml

```diff
$ diff --git a/configs/open5gs/ausf.yaml.in b/configs/open5gs/ausf.yaml.in
index cd272cd3f..3e1cb67eb 100644
--- a/configs/open5gs/ausf.yaml.in
+++ b/configs/open5gs/ausf.yaml.in
@@ -9,8 +9,7 @@ max:
 ausf:
     sbi:
       server:
-        - address: 127.0.0.11
-          port: 7777
+        - address: ausf.5gc.mnc001.mcc001.3gppnetwork.org
       client:
 #        nrf:
 #          - uri: http://127.0.0.10:7777
```

- Update udm.yaml

```diff
$ diff --git a/configs/open5gs/udm.yaml.in b/configs/open5gs/udm.yaml.in
index ce650d5c2..ed756fc28 100644
--- a/configs/open5gs/udm.yaml.in
+++ b/configs/open5gs/udm.yaml.in
@@ -28,8 +28,7 @@ udm:
         key: @sysconfdir@/open5gs/hnet/secp256r1-6.key
     sbi:
       server:
-        - address: 127.0.0.12
-          port: 7777
+        - address: udm.5gc.mnc001.mcc001.3gppnetwork.org
       client:
 #        nrf:
 #          - uri: http://127.0.0.10:7777
```

- Update smf.yaml
```diff
$ diff --git a/configs/open5gs/smf.yaml.in b/configs/open5gs/smf.yaml.in
index e47752393..b6cee9ff8 100644
--- a/configs/open5gs/smf.yaml.in
+++ b/configs/open5gs/smf.yaml.in
@@ -11,8 +11,7 @@ global:
 smf:
   sbi:
     server:
-      - address: 127.0.0.4
-        port: 7777
+      - address: smf.5gc.mnc001.mcc001.3gppnetwork.org
     client:
 #      nrf:
 #        - uri: http://127.0.0.10:7777
```


AMF and UPF must use external IP addresses such as 10.10.2.x for communication between VM1 and VM3.

- Update amf.yaml

```diff
$ diff --git a/configs/open5gs/amf.yaml.in b/configs/open5gs/amf.yaml.in
index 938917e32..2f7822b46 100644
--- a/configs/open5gs/amf.yaml.in
+++ b/configs/open5gs/amf.yaml.in
@@ -18,27 +18,37 @@ amf:
           - uri: http://127.0.0.200:7777
     ngap:
       server:
-        - address: 127.0.0.5
+        - address: 10.10.2.5
     metrics:
       server:
         - address: 127.0.0.5
           port: 9090
-    guami:
+    access_control:
       - plmn_id:
           mcc: 999
           mnc: 70
+      - plmn_id:
+          mcc: 001
+          mnc: 01
+      - plmn_id:
+          mcc: 315
+          mnc: 010
+    guami:
+      - plmn_id:
+          mcc: 001
+          mnc: 01
         amf_id:
           region: 2
           set: 1
     tai:
       - plmn_id:
-          mcc: 999
-          mnc: 70
+          mcc: 001
+          mnc: 01
         tac: 1
     plmn_support:
       - plmn_id:
-          mcc: 999
-          mnc: 70
+          mcc: 001
+          mnc: 01
         s_nssai:
           - sst: 1
     security:
```

- Update upf.yaml

```diff
$ diff --git a/configs/open5gs/upf.yaml.in b/configs/open5gs/upf.yaml.in
index e78b018f1..3032a06c6 100644
--- a/configs/open5gs/upf.yaml.in
+++ b/configs/open5gs/upf.yaml.in
@@ -15,7 +15,7 @@ upf:
 #          - address: 127.0.0.4
     gtpu:
       server:
-        - address: 127.0.0.7
+        - address: 10.10.2.5
     session:
       - subnet: 10.45.0.1/16
       - subnet: 2001:db8:cafe::1/48
```

Due to the absence of UDR in the visiting network, V-PCF uses locally configured policies. When the UE is located in the home PLMN (001/01), MongoDB is used. On the other hand, when the UE is located in the visiting PLMN (999/70, 315/010), locally configured policies are used. This is because there is no session management policy data for the UE in the visiting network, so locally configured information based on the roaming agreement is used.

- Update pcf.yaml

```diff
$ diff --git a/configs/open5gs/pcf.yaml.in b/configs/open5gs/pcf.yaml.in
index 4c5d103f9..c1526ff3a 100644
--- a/configs/open5gs/pcf.yaml.in
+++ b/configs/open5gs/pcf.yaml.in
@@ -23,6 +23,85 @@ pcf:
     server:
       - address: 127.0.0.13
         port: 9090
+  policy:
+    - supi_range:
+        - 999700000000001-999709999999999
+        - 315010000000001-315010999999999
+      slice:
+        - sst: 1  # 1,2,3,4
+          default_indicator: true
+          session:
+            - name: internet
+              type: 3  # 1:IPv4, 2:IPv6, 3:IPv4v6
+              ambr:
+                downlink:
+                  value: 1
+                  unit: 3  # 0:bps, 1:Kbps, 2:Mbps, 3:Gbps, 4:Tbps
+                uplink:
+                  value: 1
+                  unit: 3
+              qos:
+                index: 9  # 1, 2, 3, 4, 65, 66, 67, 75, 71, 72, 73, 74, 76, 5, 6, 7, 8, 9, 69, 70, 79, 80, 82, 83, 84, 85, 86
+                arp:
+                  priority_level: 8  # 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
+                  pre_emption_vulnerability: 1  # 1: Disabled, 2:Enabled
+                  pre_emption_capability: 1  # 1: Disabled, 2:Enabled
+            - name: ims
+              type: 3  # 1:IPv4, 2:IPv6, 3:IPv4v6
+              ambr:
+                downlink:
+                  value: 1
+                  unit: 3  # 0:bps, 1:Kbps, 2:Mbps, 3:Gbps, 4:Tbps
+                uplink:
+                  value: 1
+                  unit: 3  # 0:bps, 1:Kbps, 2:Mbps, 3:Gbps, 4:Tbps
+              qos:
+                index: 5  # 1, 2, 3, 4, 65, 66, 67, 75, 71, 72, 73, 74, 76, 5, 6, 7, 8, 9, 69, 70, 79, 80, 82, 83, 84, 85, 86
+                arp:
+                  priority_level: 1  # 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
+                  pre_emption_vulnerability: 1  # 1: Disabled, 2:Enabled
+                  pre_emption_capability: 1  # 1: Disabled, 2:Enabled
+              pcc_rule:
+                - qos:
+                    index: 1  # 1, 2, 3, 4, 65, 66, 67, 75, 71, 72, 73, 74, 76, 5, 6, 7, 8, 9, 69, 70, 79, 80, 82, 83, 84, 85, 86
+                    arp:
+                      priority_level: 1  # 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
+                      pre_emption_vulnerability: 1   # 1: Disabled, 2:Enabled
+                      pre_emption_capability: 1   # 1: Disabled, 2:Enabled
+                    mbr:
+                      downlink:
+                        value: 82
+                        unit: 1  # 0:bps, 1:Kbps, 2:Mbps, 3:Gbps, 4:Tbps
+                      uplink:
+                        value: 82
+                        unit: 1  # 0:bps, 1:Kbps, 2:Mbps, 3:Gbps, 4:Tbps
+                    gbr:
+                      downlink:
+                        value: 82
+                        unit: 1  # 0:bps, 1:Kbps, 2:Mbps, 3:Gbps, 4:Tbps
+                      uplink:
+                        value: 82
+                        unit: 1  # 0:bps, 1:Kbps, 2:Mbps, 3:Gbps, 4:Tbps
+                - qos:
+                    index: 2  # 1, 2, 3, 4, 65, 66, 67, 75, 71, 72, 73, 74, 76, 5, 6, 7, 8, 9, 69, 70, 79, 80, 82, 83, 84, 85, 86
+                    arp:
+                      priority_level: 4  # 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
+                      pre_emption_vulnerability: 2   # 1: Disabled, 2:Enabled
+                      pre_emption_capability: 2   # 1: Disabled, 2:Enabled
+                    mbr:
+                      downlink:
+                        value: 802
+                        unit: 1  # 0:bps, 1:Kbps, 2:Mbps, 3:Gbps, 4:Tbps
+                      uplink:
+                        value: 802
+                        unit: 1  # 0:bps, 1:Kbps, 2:Mbps, 3:Gbps, 4:Tbps
+                    gbr:
+                      downlink:
+                        value: 802
+                        unit: 1  # 0:bps, 1:Kbps, 2:Mbps, 3:Gbps, 4:Tbps
+                      uplink:
+                        value: 802
+                        unit: 1  # 0:bps, 1:Kbps, 2:Mbps, 3:Gbps, 4:Tbps

 ################################################################################
 # PCF Policy Configuration: UE Home PLMN and SUPI Range Based Policies
```

For now we will set up SEPP without using TLS.

- Create sepp.yaml

```bash
$ sh -c 'cat << EOF > ./install/etc/open5gs/sepp.yaml
logger:
    file:
      path: /home/acetcom/Documents/git/open5gs/install/var/log/open5gs/sepp.log
#    level: info   # fatal|error|warn|info(default)|debug|trace

max:
    ue: 1024  # The number of UE can be increased depending on memory size.
#    peer: 64

sepp:
    sbi:
      server:
        - address: 127.0.0.250
          port: 7777
      client:
        scp:
          - uri: http://127.0.0.200:7777
    n32:
      server:
        - sender: sepp2.localdomain
          address: 10.10.2.251
          port: 7777
          n32f:
            address: 10.10.2.252
            port: 7777
      client:
        sepp:
          - receiver: sepp1.localdomain
            uri: http://10.10.1.251:7777
            n32f:
              uri: http://10.10.1.252:7777
          - receiver: sepp3.localdomain
            uri: http://10.10.3.251:7777
            n32f:
              uri: http://10.10.3.252:7777
EOF'
```

NFs requires root privileges as it uses reserved ports such as http (80) or https (443).

- Run NFs in VM2

```bash
$ sudo ./install/bin/open5gs-nrfd
$ sudo ./install/bin/open5gs-scpd
$ sudo ./install/bin/open5gs-seppd
$ sudo ./install/bin/open5gs-amfd
$ sudo ./install/bin/open5gs-smfd
$ sudo ./install/bin/open5gs-upfd
$ sudo ./install/bin/open5gs-ausfd
$ sudo ./install/bin/open5gs-udmd
$ sudo ./install/bin/open5gs-pcfd
$ sudo ./install/bin/open5gs-nssfd
$ sudo ./install/bin/open5gs-bsfd
$ sudo ./install/bin/open5gs-udrd
```


### Setting for VM3

- Edit /etc/hosts

```diff
 $ diff -u hosts.old hosts.new
--- hosts.old	2025-07-19 07:02:44.813666212 +0900
+++ hosts.new	2025-07-19 07:04:14.598284689 +0900
@@ -1,6 +1,13 @@
 127.0.0.1	localhost
 127.0.1.1	open5gs

+127.0.3.10	nrf.5gc.mnc010.mcc315.3gppnetwork.org
+127.0.3.11	ausf.5gc.mnc010.mcc315.3gppnetwork.org
+127.0.3.12	udm.5gc.mnc010.mcc315.3gppnetwork.org
+127.0.3.14	nssf.5gc.mnc010.mcc315.3gppnetwork.org
+127.0.3.4	smf.5gc.mnc010.mcc315.3gppnetwork.org
+127.0.3.15	bsf.5gc.mnc010.mcc315.3gppnetwork.org
+
 # The following lines are desirable for IPv6 capable hosts
 ::1     ip6-localhost ip6-loopback
 fe00::0 ip6-localnet
```

NRF shall follow TS23.003(28.3.2.3.2 Format of NRF FQDN) for routing.

- Update nrf.yaml

```diff
$ diff --git a/configs/open5gs/nrf.yaml.in b/configs/open5gs/nrf.yaml.in
index 3996b2bd9..ab708abd8 100644
--- a/configs/open5gs/nrf.yaml.in
+++ b/configs/open5gs/nrf.yaml.in
@@ -9,12 +9,11 @@ max:
 nrf:
     serving:  # 5G roaming requires PLMN in NRF
       - plmn_id:
-          mcc: 999
-          mnc: 70
+          mcc: 315
+          mnc: 010
     sbi:
       server:
-        - address: 127.0.0.10
-          port: 7777
+        - address: nrf.5gc.mnc010.mcc315.3gppnetwork.org

 ################################################################################
 # SBI Server
```

- Update scp.yaml

```diff
$ diff --git a/configs/open5gs/scp.yaml.in b/configs/open5gs/scp.yaml.in
index 9be6cdc93..eee7d3e3f 100644
--- a/configs/open5gs/scp.yaml.in
+++ b/configs/open5gs/scp.yaml.in
@@ -13,7 +13,7 @@ scp:
           port: 7777
       client:
         nrf:
-          - uri: http://127.0.0.10:7777
+          - uri: http://nrf.5gc.mnc010.mcc315.3gppnetwork.org

 ################################################################################
 # SCP Info
```

- Update nssf.yaml

```diff
$ diff --git a/configs/open5gs/nssf.yaml.in b/configs/open5gs/nssf.yaml.in
index 015085f56..92ceb63b5 100644
--- a/configs/open5gs/nssf.yaml.in
+++ b/configs/open5gs/nssf.yaml.in
@@ -11,15 +11,14 @@ global:
 nssf:
   sbi:
     server:
-      - address: 127.0.0.14
-        port: 7777
+      - address: nssf.5gc.mnc010.mcc315.3gppnetwork.org
     client:
 #      nrf:
 #        - uri: http://127.0.0.10:7777
       scp:
         - uri: http://127.0.0.200:7777
       nsi:
-        - uri: http://127.0.0.10:7777
+        - uri: http://nrf.5gc.mnc010.mcc315.3gppnetwork.org
           s_nssai:
             sst: 1
 ################################################################################
```

AUSF, UDM and SMF shall use FQDN in the Home PLMN.

- Update ausf.yaml

```diff
$ diff --git a/configs/open5gs/ausf.yaml.in b/configs/open5gs/ausf.yaml.in
index cd272cd3f..3e1cb67eb 100644
--- a/configs/open5gs/ausf.yaml.in
+++ b/configs/open5gs/ausf.yaml.in
@@ -9,8 +9,7 @@ max:
 ausf:
     sbi:
       server:
-        - address: 127.0.0.11
-          port: 7777
+        - address: ausf.5gc.mnc010.mcc315.3gppnetwork.org
       client:
 #        nrf:
 #          - uri: http://127.0.0.10:7777
```

- Update udm.yaml

```diff
$ diff --git a/configs/open5gs/udm.yaml.in b/configs/open5gs/udm.yaml.in
index ce650d5c2..ed756fc28 100644
--- a/configs/open5gs/udm.yaml.in
+++ b/configs/open5gs/udm.yaml.in
@@ -28,8 +28,7 @@ udm:
         key: @sysconfdir@/open5gs/hnet/secp256r1-6.key
     sbi:
       server:
-        - address: 127.0.0.12
-          port: 7777
+        - address: udm.5gc.mnc010.mcc315.3gppnetwork.org
       client:
 #        nrf:
 #          - uri: http://127.0.0.10:7777
```

- Update smf.yaml
```diff
$ diff --git a/configs/open5gs/smf.yaml.in b/configs/open5gs/smf.yaml.in
index e47752393..b6cee9ff8 100644
--- a/configs/open5gs/smf.yaml.in
+++ b/configs/open5gs/smf.yaml.in
@@ -11,8 +11,7 @@ global:
 smf:
   sbi:
     server:
-      - address: 127.0.0.4
-        port: 7777
+      - address: smf.5gc.mnc010.mcc315.3gppnetwork.org
     client:
 #      nrf:
 #        - uri: http://127.0.0.10:7777
```

AMF and UPF must use external IP addresses such as 10.10.3.x for communication between VM1 and VM2.

- Update amf.yaml

```diff
$ diff --git a/configs/open5gs/amf.yaml.in b/configs/open5gs/amf.yaml.in
index 938917e32..383489d13 100644
--- a/configs/open5gs/amf.yaml.in
+++ b/configs/open5gs/amf.yaml.in
@@ -18,27 +18,37 @@ amf:
           - uri: http://127.0.0.200:7777
     ngap:
       server:
-        - address: 127.0.0.5
+        - address: 10.10.3.5
     metrics:
       server:
         - address: 127.0.0.5
           port: 9090
-    guami:
+    access_control:
       - plmn_id:
           mcc: 999
           mnc: 70
+      - plmn_id:
+          mcc: 001
+          mnc: 01
+      - plmn_id:
+          mcc: 315
+          mnc: 010
+    guami:
+      - plmn_id:
+          mcc: 315
+          mnc: 010
         amf_id:
           region: 2
           set: 1
     tai:
       - plmn_id:
-          mcc: 999
-          mnc: 70
+          mcc: 315
+          mnc: 010
         tac: 1
     plmn_support:
       - plmn_id:
-          mcc: 999
-          mnc: 70
+          mcc: 315
+          mnc: 010
         s_nssai:
           - sst: 1
     security:
```

- Update upf.yaml

```diff
$ diff --git a/configs/open5gs/upf.yaml.in b/configs/open5gs/upf.yaml.in
index e78b018f1..3032a06c6 100644
--- a/configs/open5gs/upf.yaml.in
+++ b/configs/open5gs/upf.yaml.in
@@ -15,7 +15,7 @@ upf:
 #          - address: 127.0.0.4
     gtpu:
       server:
-        - address: 127.0.0.7
+        - address: 10.10.3.5
     session:
       - subnet: 10.45.0.1/16
       - subnet: 2001:db8:cafe::1/48
```

Due to the absence of UDR in the visiting network, V-PCF uses locally configured policies. When the UE is located in the home PLMN (315/010), MongoDB is used. On the other hand, when the UE is located in the visiting PLMN (999/70, 001/01), locally configured policies are used. This is because there is no session management policy data for the UE in the visiting network, so locally configured information based on the roaming agreement is used.

- Update pcf.yaml

```diff
$ diff --git a/configs/open5gs/pcf.yaml.in b/configs/open5gs/pcf.yaml.in
index 4c5d103f9..c1526ff3a 100644
--- a/configs/open5gs/pcf.yaml.in
+++ b/configs/open5gs/pcf.yaml.in
@@ -23,6 +23,85 @@ pcf:
     server:
       - address: 127.0.0.13
         port: 9090
+  policy:
+    - supi_range:
+        - 999700000000001-999709999999999
+        - 001010000000001-001019999999999
+      slice:
+        - sst: 1  # 1,2,3,4
+          default_indicator: true
+          session:
+            - name: internet
+              type: 3  # 1:IPv4, 2:IPv6, 3:IPv4v6
+              ambr:
+                downlink:
+                  value: 1
+                  unit: 3  # 0:bps, 1:Kbps, 2:Mbps, 3:Gbps, 4:Tbps
+                uplink:
+                  value: 1
+                  unit: 3
+              qos:
+                index: 9  # 1, 2, 3, 4, 65, 66, 67, 75, 71, 72, 73, 74, 76, 5, 6, 7, 8, 9, 69, 70, 79, 80, 82, 83, 84, 85, 86
+                arp:
+                  priority_level: 8  # 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
+                  pre_emption_vulnerability: 1  # 1: Disabled, 2:Enabled
+                  pre_emption_capability: 1  # 1: Disabled, 2:Enabled
+            - name: ims
+              type: 3  # 1:IPv4, 2:IPv6, 3:IPv4v6
+              ambr:
+                downlink:
+                  value: 1
+                  unit: 3  # 0:bps, 1:Kbps, 2:Mbps, 3:Gbps, 4:Tbps
+                uplink:
+                  value: 1
+                  unit: 3  # 0:bps, 1:Kbps, 2:Mbps, 3:Gbps, 4:Tbps
+              qos:
+                index: 5  # 1, 2, 3, 4, 65, 66, 67, 75, 71, 72, 73, 74, 76, 5, 6, 7, 8, 9, 69, 70, 79, 80, 82, 83, 84, 85, 86
+                arp:
+                  priority_level: 1  # 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
+                  pre_emption_vulnerability: 1  # 1: Disabled, 2:Enabled
+                  pre_emption_capability: 1  # 1: Disabled, 2:Enabled
+              pcc_rule:
+                - qos:
+                    index: 1  # 1, 2, 3, 4, 65, 66, 67, 75, 71, 72, 73, 74, 76, 5, 6, 7, 8, 9, 69, 70, 79, 80, 82, 83, 84, 85, 86
+                    arp:
+                      priority_level: 1  # 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
+                      pre_emption_vulnerability: 1   # 1: Disabled, 2:Enabled
+                      pre_emption_capability: 1   # 1: Disabled, 2:Enabled
+                    mbr:
+                      downlink:
+                        value: 82
+                        unit: 1  # 0:bps, 1:Kbps, 2:Mbps, 3:Gbps, 4:Tbps
+                      uplink:
+                        value: 82
+                        unit: 1  # 0:bps, 1:Kbps, 2:Mbps, 3:Gbps, 4:Tbps
+                    gbr:
+                      downlink:
+                        value: 82
+                        unit: 1  # 0:bps, 1:Kbps, 2:Mbps, 3:Gbps, 4:Tbps
+                      uplink:
+                        value: 82
+                        unit: 1  # 0:bps, 1:Kbps, 2:Mbps, 3:Gbps, 4:Tbps
+                - qos:
+                    index: 2  # 1, 2, 3, 4, 65, 66, 67, 75, 71, 72, 73, 74, 76, 5, 6, 7, 8, 9, 69, 70, 79, 80, 82, 83, 84, 85, 86
+                    arp:
+                      priority_level: 4  # 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
+                      pre_emption_vulnerability: 2   # 1: Disabled, 2:Enabled
+                      pre_emption_capability: 2   # 1: Disabled, 2:Enabled
+                    mbr:
+                      downlink:
+                        value: 802
+                        unit: 1  # 0:bps, 1:Kbps, 2:Mbps, 3:Gbps, 4:Tbps
+                      uplink:
+                        value: 802
+                        unit: 1  # 0:bps, 1:Kbps, 2:Mbps, 3:Gbps, 4:Tbps
+                    gbr:
+                      downlink:
+                        value: 802
+                        unit: 1  # 0:bps, 1:Kbps, 2:Mbps, 3:Gbps, 4:Tbps
+                      uplink:
+                        value: 802
+                        unit: 1  # 0:bps, 1:Kbps, 2:Mbps, 3:Gbps, 4:Tbps

 ################################################################################
 # PCF Policy Configuration: UE Home PLMN and SUPI Range Based Policies
```

NFs requires root privileges as it uses reserved ports such as http (80) or https (443).

- Run NFs in VM3

```bash
$ sudo ./install/bin/open5gs-nrfd
$ sudo ./install/bin/open5gs-scpd
$ sudo ./install/bin/open5gs-seppd
$ sudo ./install/bin/open5gs-amfd
$ sudo ./install/bin/open5gs-smfd
$ sudo ./install/bin/open5gs-upfd
$ sudo ./install/bin/open5gs-ausfd
$ sudo ./install/bin/open5gs-udmd
$ sudo ./install/bin/open5gs-pcfd
$ sudo ./install/bin/open5gs-nssfd
$ sudo ./install/bin/open5gs-bsfd
$ sudo ./install/bin/open5gs-udrd
```

## 3. Setting SEPP on VM1 to HTTPS scheme with TLS

Only N32 uses HTTPS with TLS, while other NFs use HTTP without TLS.

```diff
$ diff -u sepp.yaml.old sepp.yaml
--- sepp.yaml.old	2023-10-02 18:35:23.585643661 +0900
+++ sepp.yaml	2023-10-02 18:36:17.971104248 +0900
@@ -7,6 +7,13 @@
 #    peer: 64

 sepp:
+    default:
+      tls:
+        server:
+          private_key: /home/acetcom/Documents/git/my/open5gs/build/configs/open5gs/tls/sepp1.key
+          cert: /home/acetcom/Documents/git/my/open5gs/build/configs/open5gs/tls/sepp1.crt
+        client:
+          cacert: /home/acetcom/Documents/git/my/open5gs/build/configs/open5gs/tls/ca.crt
     sbi:
       server:
         - address: 127.0.0.250
@@ -17,18 +24,22 @@
     n32:
       server:
         - sender: sepp1.localdomain
+          scheme: https
           address: 10.10.1.251
-          port: 7777
           n32f:
+            scheme: https
             address: 10.10.1.252
-            port: 7777
       client:
         sepp:
           - receiver: sepp2.localdomain
-            uri: http://10.10.2.251:7777
+            uri: https://sepp2.localdomain
+            resolve: 10.10.2.251
             n32f:
-              uri: http://10.10.2.252:7777
+              uri: https://sepp2.localdomain
+              resolve: 10.10.2.252
           - receiver: sepp3.localdomain
-            uri: http://10.10.3.251:7777
+            uri: https://sepp3.localdomain
+            resolve: 10.10.3.251
             n32f:
-              uri: http://10.10.3.252:7777
+              uri: https://sepp3.localdomain
+              resolve: 10.10.3.252
```

Add client TLS verification to N32 interface

```diff
$ diff -u sepp.yaml.old sepp.yaml
--- sepp.yaml.old	2023-10-02 18:44:56.011099652 +0900
+++ sepp.yaml	2023-10-02 18:45:48.884662145 +0900
@@ -12,8 +12,12 @@
         server:
           private_key: /home/acetcom/Documents/git/my/open5gs/build/configs/open5gs/tls/sepp1.key
           cert: /home/acetcom/Documents/git/my/open5gs/build/configs/open5gs/tls/sepp1.crt
+          verify_client: true
+          verify_client_cacert: /home/acetcom/Documents/git/my/open5gs/build/configs/open5gs/tls/ca.crt
         client:
           cacert: /home/acetcom/Documents/git/my/open5gs/build/configs/open5gs/tls/ca.crt
+          client_private_key: /home/acetcom/Documents/git/my/open5gs/build/configs/open5gs/tls/sepp1.key
+          client_cert: /home/acetcom/Documents/git/my/open5gs/build/configs/open5gs/tls/sepp1.crt
     sbi:
       server:
         - address: 127.0.0.250
```

For more information, please refer to the [LINK](https://github.com/open5gs/open5gs/blob/main/configs/open5gs/sepp1.yaml.in).
