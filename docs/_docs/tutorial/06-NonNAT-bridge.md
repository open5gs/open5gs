---
title: Non-NAT-bridge
---

## 0. Introduction

In mobile networks, Network Address Translation (NAT) is often used to manage IP address allocation and ensure network security. However, NAT can introduce complexity and potential performance issues in certain scenarios. The NonNAT feature in Open5GS offers an alternative approach by allowing user equipment (UE) to receive IP addresses directly from the network, typically managed through a Carrier-Grade NAT (CGNAT) system.

By avoiding the traditional NAT process, NonNAT simplifies IP address management and can reduce latency in data transmission. This feature is particularly useful in environments where low-latency communication is critical, or in networks where IP addresses are allocated from a CGNAT pool.

To do that, we just need to set open5gs to use ogstap instead of ogstun, put this new ogstap device in a bridge with the interface that will communicate with the CGNAT and set the CGNAT MAC address in UPF configuration.

## 1. UE Static IP

For UPF to simulate the UE MAC address, we're using UE IP, so for one to know which UE is talking, it's necessary to have UE IP statically set in mongodb:

```
open5gs-dbctl static_ip 999990000000001 10.45.0.50
```

The resulting MAC address is calculated as 0E:00:x:x:x:x, where the lasting four octets are the 4 IP address bytes.

## 2. Configuration changes

#### UPF.yaml

```diff
$ diff -u upf.yaml.old upf.yaml
--- upf.yaml.old	2024-08-01 16:53:42.409779148 +0000
+++ upf.yaml.new	2024-08-01 16:54:21.950191331 +0000
@@ -19,10 +20,8 @@
     server:
       - address: 127.0.0.7
   session:
-    - subnet: 10.45.0.0/16
-      gateway: 10.45.0.1
-    - subnet: 2001:db8:cafe::/48
-      gateway: 2001:db8:cafe::1
+    - dev: ogstap
+      mac_address: 48:A9:8A:8C:E1:15
   metrics:
     server:
       - address: 127.0.0.7
```

#### 99-open5gs.netdev

```diff
$ diff -u 99-open5gs.netdev.old 99-open5gs.netdev
--- 99-open5gs.netdev.old	2024-08-01 17:02:40.967406398 +0000
+++ 99-open5gs.netdev.new	2024-08-01 17:00:52.846274905 +0000
@@ -1,3 +1,3 @@
 [NetDev]
-Name=ogstun
-Kind=tun
+Name=ogstap
+Kind=tap
```

#### 99-open5gs.network

```diff
$ diff -u 99-open5gs.network.old 99-open5gs.network
--- 99-open5gs.network.old	2024-08-01 17:02:47.355473269 +0000
+++ 99-open5gs.network.new	2024-08-01 17:29:44.853435909 +0000
@@ -1,17 +1,5 @@
 [Match]
-Name=ogstun
-
-[Network]
-Address=10.45.0.1/16
-Address=2001:db8:cafe::1/48
-
-[Route]
-Gateway=0.0.0.0
-Destination=10.45.0.0/16
-
-[Route]
-Gateway=::
-Destination=2001:db8:cafe::0/48
+Name=ogstap
 
 [Link]
 MTUBytes=1400
```

## 3. Putting ogstap in a bridge

Create the bridge, put the out interface (usually a VLAN) in it and finally ogstap too:

```
ip link add br0 type bridge
ip link set vlan10 master br0
ip link set ogstap master br0
```
