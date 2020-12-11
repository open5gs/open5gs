---
title: Open5GS Splitting Network Functions
head_inline: "<style> .blue { color: blue; } </style>"
---

In a production network, NFs are usually not on the same machine as the default example that comes with Open5GS.

Open5GS is designed to comply with standards, so in theory you can connect core network functions from Open5GS or other vendors to form a functioning network. However, this is only possible if it complies with 3GPP.

To demonstrate this, we will cover how to isolate each network function and connect each network function to another. For some interfaces, you can specify multiple interfaces to connect to multiple interfaces.

In this example, we are connecting the Open5GS NF together, but instead of the Open5GS network function, you can easily connect them like other vendors' NFs.

## 1. Configuring Open5GS (Without other vendors' NFs)

_Cloud Authentication and Session Management Server_

```
MongoDB   = 127.0.0.1 (subscriber data) - http://localhost:3000
MME-s1ap  = 10.10.0.2 :36412 (authVPN) for S1-MME
MME-gtpc  = 127.0.0.2 :2123 for S11
MME-frDi  = 127.0.0.2 :3868 for S6a auth
SGWC-gtpc = 127.0.0.3 :2123 for S11
SGWC-pfcp = 10.10.0.3 :8805 (authVPN) for Sxa
SMF-gtpc  = 127.0.0.4 :2123 for S5/8c, N11
SMF-pfcp  = 10.10.0.4 :8805 (authVPN) for N4
SMF-frDi  = 127.0.0.4 :3868 for Gx auth
SMF-sbi   = 127.0.0.4 :7777 for 5G SBI (N7,N10,N11)
AMF-ngap  = 10.10.0.5 :38412 (authVPN) for N2
AMF-sbi   = 127.0.0.5 :7777 for 5G SBI (N8,N12,N11)
HSS-frDi  = 127.0.0.8 :3868 for S6a auth
PCRF-frDi = 127.0.0.9 :3868 for Gx auth
NRF-sbi   = 127.0.0.10:7777 for 5G SBI
AUSF-sbi  = 127.0.0.11:7777 for 5G SBI
UDM-sbi   = 127.0.0.12:7777 for 5G SBI
PCF-sbi   = 127.0.0.12:7777 for 5G SBI
UDR-sbi   = 127.0.0.20:7777 for 5G SBI
```

_Cloud Userplane Server (to host IMS etc)_

```
SGWU-pfcp = 10.10.0.6 :8805 (authVPN) for Sxa
SGWU-gtpu = 10.11.0.6 :2152 (userplaneVPN) for S1-U, S5/8u
UPF-pfcp  = 10.10.0.7 :8805 (authVPN) for N4
UPF-gtpu  = 10.11.0.7 :2152 (userplaneVPN) for S5/8u, N3
```

You can refer to the network settings at
[{{ site.url }}{{ site.baseurl }}/assets/Open5GS-Diagram.pdf]({{ site.url }}{{ site.baseurl }}/assets/Open5GS-Diagram.pdf) provided by [@kbarlee](https://github.com/kbarlee) in issue [#528](https://github.com/{{ site.github_username }}/open5gs/issues/528)
{: .notice--danger}

Modify [install/etc/open5gs/mme.yaml](https://github.com/{{ site.github_username }}/open5gs/blob/master/configs/open5gs/mme.yaml.in) to set the S1AP IP address, PLMN ID, and TAC.

```diff
$ diff -u /etc/open5gs/mme.yaml.old /etc/open5gs/mme.yaml
--- mme.yaml.old	2020-08-22 11:36:40.512418765 -0400
+++ mme.yaml	2020-08-22 11:36:27.081466682 -0400
@@ -204,20 +204,20 @@
 mme:
     freeDiameter: /home/acetcom/Documents/git/open5gs/install/etc/freeDiameter/mme.conf
     s1ap:
-      addr: 127.0.0.2
+      addr: 10.10.0.2
     gtpc:
       addr: 127.0.0.2
     gummei: 
       plmn_id:
-        mcc: 001
-        mnc: 01
+        mcc: 901
+        mnc: 70
       mme_gid: 2
       mme_code: 1
     tai:
       plmn_id:
-        mcc: 001
-        mnc: 01
-      tac: 7
+        mcc: 901
+        mnc: 70
+      tac: 1
     security:
         integrity_order : [ EIA1, EIA2, EIA0 ]
         ciphering_order : [ EEA0, EEA1, EEA2 ]
```

Modify [install/etc/open5gs/sgwc.yaml](https://github.com/{{ site.github_username }}/open5gs/blob/master/configs/open5gs/sgwc.yaml.in) to set the PFCP IP address.
```diff
$ diff -u /etc/open5gs/sgwc.yaml.old /etc/open5gs/sgwc.yaml
@@ -49,7 +49,7 @@
     gtpc:
       addr: 127.0.0.3
     pfcp:
-      addr: 127.0.0.3
+      addr: 10.10.0.3

 #
 # sgwu:
@@ -100,7 +100,7 @@
 #
 sgwu:
     pfcp:
-      addr: 127.0.0.6
+      addr: 10.10.0.6

 #
 # parameter:
```

Modify [install/etc/open5gs/smf.yaml](https://github.com/{{ site.github_username }}/open5gs/blob/master/configs/open5gs/smf.yaml.in) to set the PFCP IP address.
```diff
$ diff -u /etc/open5gs/smf.yaml.old /etc/open5gs/smf.yaml
--- smf.yaml.old	2020-08-22 11:37:39.990816411 -0400
+++ smf.yaml	2020-08-22 11:38:18.647999952 -0400
@@ -187,8 +187,7 @@
       - addr: 127.0.0.4
       - addr: ::1
     pfcp:
-      - addr: 127.0.0.4
-      - addr: ::1
+      - addr: 10.10.0.4
     pdn:
       - addr: 10.45.0.1/16
       - addr: cafe::1/64
@@ -282,7 +281,7 @@
 #
 upf:
     pfcp:
-      - addr: 127.0.0.7
+      - addr: 10.10.0.7

 #
 # parameter:

```

Modify [install/etc/open5gs/amf.yaml](https://github.com/{{ site.github_username }}/open5gs/blob/master/configs/open5gs/amf.yaml.in) to set the NGAP IP address, PLMN ID, TAC and NSSAI.

```diff
diff -u /etc/open5gs/amf.yaml.old /etc/open5gs/amf.yaml
--- amf.yaml.old	2020-06-21 23:34:14.643114779 -0400
+++ amf.yaml	2020-06-21 23:34:28.718482095 -0400
@@ -67,25 +67,25 @@
       - addr: 127.0.0.5
         port: 7777
     ngap:
-      - addr: 127.0.0.5
+      - addr: 10.10.0.5
     guami:
       - plmn_id:
-          mcc: 001
-          mnc: 01
+          mcc: 901
+          mnc: 70
         amf_id:
           region: 2
           set: 1
     tai:
       - plmn_id:
-          mcc: 001
-          mnc: 01
-        tac: 7
+          mcc: 901
+          mnc: 70
+        tac: 1
     plmn:
       - plmn_id:
-          mcc: 001
-          mnc: 01
+          mcc: 901
+          mnc: 70
         s_nssai:
           - sst: 1
-          - sd: 2
     security:
         integrity_order : [ NIA1, NIA2, NIA0 ]
         ciphering_order : [ NEA0, NEA1, NEA2 ]
```

Modify [install/etc/open5gs/sgwu.yaml](https://github.com/{{ site.github_username }}/open5gs/blob/master/configs/open5gs/sgwu.yaml.in) to set the GTP-U and PFCP IP address.
```diff
$ diff -u /etc/open5gs/sgwu.yaml.old /etc/open5gs/sgwu.yaml
--- sgwu.yaml.old	2020-08-22 11:41:09.214670723 -0400
+++ sgwu.yaml	2020-08-22 11:41:27.433937124 -0400
@@ -51,9 +51,9 @@
 #
 sgwu:
     gtpu:
-      addr: 127.0.0.6
+      addr: 10.11.0.6
     pfcp:
-      addr: 127.0.0.6
+      addr: 10.10.0.6

 #
 # sgwc:
```

Modify [install/etc/open5gs/upf.yaml](https://github.com/{{ site.github_username }}/open5gs/blob/master/configs/open5gs/upf.yaml.in) to set the GTP-U and PFCP IP address.
```diff
$ diff -u /etc/open5gs/upf.yaml.old /etc/open5gs/upf.yaml
--- upf.yaml.old	2020-08-22 11:42:57.781750067 -0400
+++ upf.yaml	2020-08-22 11:43:13.268901616 -0400
@@ -59,11 +59,9 @@
 #
 upf:
     pfcp:
-      - addr: 127.0.0.7
+      - addr: 10.10.0.7
     gtpu:
-      - addr:
-        - 127.0.0.7
-        - ::1
+      - addr: 10.11.0.7
     pdn:
       - addr: 10.45.0.1/16
       - addr: cafe::1/64
```

After changing conf files, please restart Open5GS daemons.

```bash
$ sudo systemctl restart open5gs-mmed.service
$ sudo systemctl restart open5gs-sgwcd.service
$ sudo systemctl restart open5gs-smfd.service
$ sudo systemctl restart open5gs-amfd.service
$ sudo systemctl restart open5gs-upfd.service
$ sudo systemctl restart open5gs-sgwud.service
```

## 2. External SMF
In it's simplest form the SMF has 4 interfaces:
 * S5 - Connection to home network SGW-C (GTP-C)
 * Gx - Connection to PCRF (Diameter)
 * SBI - Service-based interface(HTTP2)
 * Sgi - Connection to external network (Generally the Internet via standard TCP/IP)

#### S5 Interface Configuration
Edit ```/etc/open5gs/smf.yaml``` and change the address to IP of the server running the SMF for the listener on GTP-C interfaces.

```
smf:
    freeDiameter: /etc/freeDiameter/smf.conf
    gtpc: 
      addr:
        - 10.0.1.121
```

#### Gx Interface Configuration
Edit ```/etc/freeDiameter/smf.conf```

Update ```ListenOn``` address to IP of the server running the SMF:

``` ListenOn = "10.0.1.121"; ``` 

Update ConnectPeer to connect to the PCRF on it's IP.

```ConnectPeer = "pcrf.localdomain" { ConnectTo = "10.0.1.123"; No_TLS; };```

#### Restart Services
Restart Open5GS SMF Daemon:

``` $ sudo systemctl restart open5gs-smfd.service ```


## 3. External SGW-C
In it's simplest form the SGW-C has 2 interfaces:
 * S11 - Connection to MME (GTP-C)
 * S5 - Connection to the home network SMF (GTP-C)

#### S5 Interface Configuration
Edit ```/etc/open5gs/sgwc.yaml``` and change the address to IP of the server running the SGW-C for the listener on GTP-C interface.

```
sgwc:
    gtpc: 
      addr:
        - 10.0.1.122
```
 
 Restart Open5GS SGW-C Daemon:

``` $ sudo systemctl restart open5gs-sgwcd.service ```


## 4. External PCRF
In it's simplest from the PCRF has 1 network interface:
 * Gx - Connection to SMF (Diameter)

#### Gx Interface Configuration
Edit ```/etc/freeDiameter/pcrf.conf```

Update ```ListenOn``` address to IP of the server running the HSS on it's IP:

``` ListenOn = "10.0.1.123"; ``` 

Update ConnectPeer to connect to the MME.

```ConnectPeer = "smf.localdomain" { ConnectTo = "10.0.1.121"; No_TLS; };```

#### MongoDB Interface Configuration
Edit the ```db_uri:``` to point at the Open5GS: ```db_uri: mongodb://10.0.1.118/open5gs``` 

Restart Open5GS PCRF Daemon:

``` $ sudo systemctl restart open5gs-pcrfd.service ```

## 5. External HSS
In it's simplest form the HSS has 1 network interface:
 * S6a - Connection to MME (Diameter)

#### S6a Interface Configuration
Edit ```/etc/freeDiameter/hss.conf```

Update ```ListenOn``` address to IP of the server running the HSS on it's IP:

``` ListenOn = "10.0.1.118"; ``` 

Update ConnectPeer to connect to the MME.

```ConnectPeer = "mme.localdomain" { ConnectTo = "10.0.1.124"; No_TLS; };```

Restart Open5GS HSS Daemon:

``` $ sudo systemctl restart open5gs-hssd.service ```

#### MongoDB Interface Configuration (Open5GS specific)
If you are using Open5GS's HSS you may need to enable MongoDB access from the PCRF. This is done by editing ''/etc/mongodb.conf'' and changing the bind IP to:
``` bind_ip = 0.0.0.0 ```

Restart MongoDB for changes to take effect.

``` $ /etc/init.d/mongodb restart ```

## 6. External MME
In it's simplest form the MME has 3 interfaces:
 * S1AP - Connections from eNodeBs
 * S6a - Connection to HSS (Diameter)
 * S11 - Connection to SGW-C (GTP-C)
 
#### S11 Interface Configuration
Edit ```/etc/open5gs/mme.yaml``` and filling the IP address of the SGW-C and SMF servers.
```
sgwc:
    gtpc:
      addr: 10.0.1.122

smf:
    gtpc:
      addr:
        - 10.0.1.121
```

#### S6a Interface Configuration
Edit ```/etc/freeDiameter/mme.conf```

Update ```ListenOn``` address to IP of the server running the MME:

``` ListenOn = "10.0.1.124"; ``` 

Update ConnectPeer to connect to the PCRF on it's IP.

```ConnectPeer = "hss.localdomain" { ConnectTo = "10.0.1.118"; No_TLS; };```


#### Restart Services
Restart Open5GS MME Daemon:

``` $ sudo systemctl restart open5gs-mmed.service ```
