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

We'll start with the `roaming-tier3` branch from now on.
```
$ git clone https://github.com/open5gs/open5gs
$ cd open5gs
$ git checkout roaming-tier3
$ meson build --prefix=`pwd`/install
$ ninja -C build install
```

#### Configure FQDN to DNS

For routing to Home PLMN, NRF, AUSF and UDM address in H-PLMN shall use FQDN format, so please edit the /etc/hosts file as follows.

```diff
$ diff -u hosts.old hosts.new
--- hosts.old	2023-10-02 11:58:54.261154726 +0900
+++ hosts.new	2023-10-02 11:59:02.868771248 +0900
@@ -1,6 +1,18 @@
 127.0.0.1	localhost
 127.0.1.1	open5gs

+127.0.1.10	nrf.5gc.mnc070.mcc999.3gppnetwork.org
+127.0.1.11	ausf.5gc.mnc070.mcc999.3gppnetwork.org
+127.0.1.12	udm.5gc.mnc070.mcc999.3gppnetwork.org
+
+127.0.2.10	nrf.5gc.mnc001.mcc001.3gppnetwork.org
+127.0.2.11	ausf.5gc.mnc001.mcc001.3gppnetwork.org
+127.0.2.12	udm.5gc.mnc001.mcc001.3gppnetwork.org
+
+127.0.3.10	nrf.5gc.mnc010.mcc315.3gppnetwork.org
+127.0.3.11	ausf.5gc.mnc010.mcc315.3gppnetwork.org
+127.0.3.12	udm.5gc.mnc010.mcc315.3gppnetwork.org
+
 # The following lines are desirable for IPv6 capable hosts
 ::1     ip6-localhost ip6-loopback
 fe00::0 ip6-localnet
```


#### Running V-PLMN 5G Core and H-PLMN 5G Core in the Single Host

5G Core requires root privileges as it uses reserved ports such as http(80) or https(443).

```
$ sudo ./build/tests/app/5gc -c ./build/configs/examples/5gc-sepp1-999-70.yaml
$ sudo ./build/tests/app/5gc -c ./build/configs/examples/5gc-sepp2-001-01.yaml
$ sudo ./build/tests/app/5gc -c ./build/configs/examples/5gc-sepp3-315-010.yaml
```

Performs a test of UE access while roaming subscribed to H-PLMN.
```
$ ./build/tests/registration/registration -c ./build/configs/examples/gnb-999-70-ue-001-01.yaml simple-test
$ ./build/tests/registration/registration -c ./build/configs/examples/gnb-999-70-ue-315-010.yaml simple-test
$ ./build/tests/registration/registration -c ./build/configs/examples/gnb-001-01-ue-999-70.yaml simple-test
$ ./build/tests/registration/registration -c ./build/configs/examples/gnb-001-01-ue-315-010.yaml simple-test
$ ./build/tests/registration/registration -c ./build/configs/examples/gnb-315-010-ue-999-70.yaml simple-test
$ ./build/tests/registration/registration -c ./build/configs/examples/gnb-315-010-ue-001-01.yaml simple-test
```

You can see the sample traffic.  -- [[5g-roaming-lbo.pcapng]]({{ site.url }}{{ site.baseurl }}/assets/pcapng/5g_roaming_lbo.pcapng).


## 2. Roaming Deployment

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
--- hosts.old	2023-10-02 12:22:41.320546720 +0900
+++ hosts.new	2023-10-02 12:22:33.576884651 +0900
@@ -1,6 +1,10 @@
 127.0.0.1	localhost
 127.0.1.1	open5gs

+127.0.0.10	nrf.5gc.mnc070.mcc999.3gppnetwork.org
+127.0.0.11	ausf.5gc.mnc070.mcc999.3gppnetwork.org
+127.0.0.12	udm.5gc.mnc070.mcc999.3gppnetwork.org
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
index d01645b2c..7d89cffef 100644
--- a/configs/open5gs/nssf.yaml.in
+++ b/configs/open5gs/nssf.yaml.in
@@ -17,7 +17,7 @@ nssf:
         scp:
           - uri: http://127.0.0.200:7777
         nsi:
-          - uri: http://127.0.0.10:7777
+          - uri: http://nrf.5gc.mnc070.mcc999.3gppnetwork.org
             s_nssai:
               sst: 1
 ################################################################################
```

AUSF and UDM shall use FQDN in the Home PLMN.

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


For now we will set up SEPP without using TLS.

- Create sepp.yaml

```bash
$ sh -c 'cat << EOF > ./install/etc/open5gs/sepp.yaml
logger:
    file: /home/acetcom/Documents/git/open5gs/install/var/log/open5gs/sepp.log
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
--- hosts.old	2023-10-02 12:22:41.320546720 +0900
+++ hosts.new	2023-10-02 12:22:33.576884651 +0900
@@ -1,6 +1,10 @@
 127.0.0.1	localhost
 127.0.1.1	open5gs

+127.0.0.10	nrf.5gc.mnc001.mcc001.3gppnetwork.org
+127.0.0.11	ausf.5gc.mnc001.mcc001.3gppnetwork.org
+127.0.0.12	udm.5gc.mnc001.mcc001.3gppnetwork.org
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
index d01645b2c..7d89cffef 100644
--- a/configs/open5gs/nssf.yaml.in
+++ b/configs/open5gs/nssf.yaml.in
@@ -17,7 +17,7 @@ nssf:
         scp:
           - uri: http://127.0.0.200:7777
         nsi:
-          - uri: http://127.0.0.10:7777
+          - uri: http://nrf.5gc.mnc001.mcc001.3gppnetwork.org
             s_nssai:
               sst: 1
 ################################################################################
```

AUSF and UDM shall use FQDN in the Home PLMN.

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


For now we will set up SEPP without using TLS.

- Create sepp.yaml

```bash
$ sh -c 'cat << EOF > ./install/etc/open5gs/sepp.yaml
logger:
    file: /home/acetcom/Documents/git/open5gs/install/var/log/open5gs/sepp.log
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
--- hosts.old	2023-10-02 12:22:41.320546720 +0900
+++ hosts.new	2023-10-02 12:22:33.576884651 +0900
@@ -1,6 +1,10 @@
 127.0.0.1	localhost
 127.0.1.1	open5gs

+127.0.0.10	nrf.5gc.mnc010.mcc315.3gppnetwork.org
+127.0.0.11	ausf.5gc.mnc010.mcc315.3gppnetwork.org
+127.0.0.12	udm.5gc.mnc010.mcc315.3gppnetwork.org
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
index d01645b2c..7d89cffef 100644
--- a/configs/open5gs/nssf.yaml.in
+++ b/configs/open5gs/nssf.yaml.in
@@ -17,7 +17,7 @@ nssf:
         scp:
           - uri: http://127.0.0.200:7777
         nsi:
-          - uri: http://127.0.0.10:7777
+          - uri: http://nrf.5gc.mnc010.mcc315.3gppnetwork.org
             s_nssai:
               sst: 1
 ################################################################################
```

AUSF and UDM shall use FQDN in the Home PLMN.

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


For now we will set up SEPP without using TLS.

- Create sepp.yaml

```bash
$ sh -c 'cat << EOF > ./install/etc/open5gs/sepp.yaml
logger:
    file: /home/acetcom/Documents/git/open5gs/install/var/log/open5gs/sepp.log
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
        - sender: sepp3.localdomain
          address: 10.10.3.251
          port: 7777
          n32f:
            address: 10.10.3.252
            port: 7777
      client:
        sepp:
          - receiver: sepp1.localdomain
            uri: http://10.10.1.251:7777
            n32f:
              uri: http://10.10.1.252:7777
          - receiver: sepp2.localdomain
            uri: http://10.10.2.251:7777
            n32f:
              uri: http://10.10.2.252:7777
EOF'
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
+    defconfig:
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

For more information, please refer to the [LINK](https://github.com/open5gs/open5gs/blob/roaming-tier3/configs/open5gs/sepp1.yaml.in).
