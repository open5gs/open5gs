---
title: Simple Issue
---

**Note:** This guide assumes you've already installed Open5GS and it's running with it's default values. If you're new to Open5GS check out the [Quickstart Guide]({{ site.url }}{{ site.baseurl }}/docs/guide/01-quickstart)
{: .notice--warning}

## Services Running

#### Confirming All Services are Running
Before we get too far in troubleshooting let's first check all our services are running.

```bash
$ ps aux | grep open5gs
open5gs    3434  0.2  2.8 1074020 27972 ?       Ssl  02:39   0:00 /usr/bin/open5gs-pgwd -c /etc/open5gs/pgw.yaml
open5gs    3672  0.0  1.0 922972 10408 ?        Ssl  02:39   0:00 /usr/bin/open5gs-sgwd -c /etc/open5gs/sgw.yaml
open5gs    3893  0.1  1.3 958412 13260 ?        Ssl  02:39   0:00 /usr/bin/open5gs-hssd -c /etc/open5gs/hss.yaml
open5gs    4011  0.1  1.3 956320 13140 ?        Ssl  02:39   0:00 /usr/bin/open5gs-pcrfd -c /etc/open5gs/pcrf.yaml
open5gs    4138  0.3  1.2 986524 12608 ?        Ssl  02:40   0:00 /usr/bin/open5gs-mmed -c /etc/open5gs/mme.yaml
root       4198  0.0  0.1  13136  1060 pts/0    S+   02:40   0:00 grep --color=auto open5gs
```

You should see each of the services above, PGW, SGW, HSS, PCRF & MME all running. 

If your instance doesn't show this make sure you're started each service:
```bash
$ systemctl start open5gs-*
```

#### Finding out why a Service isn't Starting

If a service isn't running check the log for that service - logs for each service live in */var/log/open5gs/* where each service logs to it's own file - MME logs in mme.log, PGW logs in pgw.log, and so on. 

```bash
$ cat /var/log/open5gs/mme.log
Open5GS daemon v1.0.0

[app] INFO: Configuration: '/etc/open5gs/mme.yaml' (../src/main.c:54)
[app] INFO: File Logging: '/var/log/open5gs/mme.log' (../src/main.c:57)
[mme] ERROR: No sgw.gtpc in '/etc/open5gs/mme.yaml' (../src/mme/mme-context.c:192)
[app] ERROR: Failed to intialize MME (../src/mme/app-init.c:30)
[app] FATAL: Open5GS initialization failed. Aborted (../src/main.c:222)
```

Or, you can use `journalctl` like below to view live log.

```bash
$ journalctl -u open5gs-mmed.service --since today -f
```

In the example above we can see the error - no SGW GTPC address is configured in the mme.yaml file, meaning Open5GS MME is failing to start.

The errors you experience may be different, but if a service is failing to start it's most often due to a misconfiguration issue in one or more of the Open5GS *.yaml* configuration files. The log should tell you which section of the yaml file is missing or invalid.


## eNB Connection Issues
---
When a UE connects to Open5GS MME the log shows the presence of a new S1AP connection the log at */var/log/open5gs/mme.log*:

##### No S1AP Connection
If you're not seeing any S1AP connection attempts check the eNB can contact the IP the MME is on (No firewall / ACLs etc blocking) and that SCTP Traffic is able to be carried across your transmission network. 

**Note:** 3GPP defines SCTP as the transport protocol for S1-AP/S1-CP traffic (not TCP/UDP). Not all devices / routers support S1AP, particularly over the Internet.

If you're confident the service is running and connectivity is able to be established across your transmission network, you should see the *SCTP INIT* packets in Wireshark. If you're not seeing these packets go back and check your network.

If you are seeing the SCTP INIT messages and seeing an ABORT after each one, that suggests the SCTP connection is trying to be established. Check that the MME service is started and listening on the interface / IP you're sending traffic to.

##### S1AP Connection Rejected
If you're seeing S1AP Connection attempts but seeing them rejected by Open5GS, the S1AP message show in a packet capture will indicate the rejection reason, as well as in the mme log.
```
$ tail -f /var/log/open5gs/mme.log
[mme] INFO: eNB-S1[10.0.1.14] connection refused!!! (mme-sm.c:176)
```

Typically S1AP connections are rejected due to one of these reasons:
* MNC / MCC in eNB does not match *gummei* and *tai* MCC/MNC pair in (mme.yaml*.
* Tracking Area Code does not match configured TACs in *mme.yaml*.

Each of these can be addressed by editing the relevant section in the MME config in */etc/open5gs/mme.yaml*

__Example of sucesful eNB connection to MME:__
```
$ tail -f /var/log/open5gs/mme.log
[mme] INFO: eNB-S1 accepted[10.0.1.14]:3223 in s1_path module (s1ap-lkpath.c:70)
[mme] INFO: eNB-S1 accepted[10.0.1.14] in master_sm module (mme-sm.c:126)
```

## UE Connection Issues
---
If your network is setup there are a variety of reasons your network may not perform correctly / at all when connecting a UE.

#### UE cannot See Network
If while scanning for the networks on your UE / Phone you're not seeing your network, there's a few things to check:
* Check your eNB is connected to the MME
* Check the eNB status
* Check the UE you are scanning with is capable of working on the frequencies / bands and duplex mode (TDD or FDD) used by the UE
* Check UE is within range of eNB
* Check PLMN is not forbidden on USIM (F-PLMN List)

#### UE Fails to Attach to the Network
Assuming while scanning for networks the UE can see the network, but not connect, the most common issues stem from Authentication.

LTE/E-UTRAN employs *Mutual Authentication* of both the network and the subscriber. This means the credentials in the HSS must match the credentials on the USIM and the credentials in the USIM must match those in the HSS. This means unlike GSM, you cannot use just any SIM and disable crypto, you have to know the details on the USIM or be able to program this yourself in order to authenticate.

If the issue is authentication, the mme and hss log will give you an indication as to which side is rejecting the authentication, the UE or the Network (Open5GS);

__IMSI/Subscriber not present in HSS:__
If the USIM's IMSI is not present in the HSS the HSS will reject the Authentication.

```
$ tail -f /var/log/open5gs/hss.log
[hss] WARNING: Cannot find IMSI in DB : 001000000000001 (hss-context.c:309)
```

__IMSI/Subscriber configured in HSS with wrong credentails:__
If the credentials on the HSS do not match what is configured on the USIM, the USIM will reject the connection (MAC Error).

```
$ tail -f /var/log/open5gs/mme.log
[emm] WARNING: Authentication failure(MAC failure) (emm-sm.c:573)
```

__APN Requested by UE not present in HSS:__
Ensure the APNs requested by the UE are present in the HSS.


#### UE shows "4G" or "LTE" Connection but has no IP Connectivity to the outside World
If your device shows as connected (Includes LTE/4G symbol) there are a few simple things to check to diagnose connectivity issues:
* The PGW can contact the outside world (Can resolve DNS, browse, etc)
* Check if the interface connected to the internet is correctly `NAT` with the `ogstun` interface.
   - Ensure that the packets in the `INPUT` chain to the `ogstun` interface are accepted 
   ```
   $ sudo iptables -I INPUT -i ogstun -j ACCEPT
   ```
* Check if the UE's IP can be pinged successfully by performing `ping <IP of UE>` -- [e.g. `ping 10.45.0.2`]
* Configure the firewall correctly. Some operating systems (Ubuntu) by default enable firewall rules to block traffic
   - Explicitly disable it to see if it resolves the problem of granting data access to the UE by doing
   ```
   $ sudo ufw disable
   ```

## Further Debugging
---

Problem with Open5GS can be filed as [GitHub Issues](https://github.com/open5gs/open5gs/issues). Please include the following to get help:

- Attach `*.pcapng` file created by wireskark.
- Attach configuration files at `/etc/open5gs/*.yaml`.
- Attach log files at `/var/log/open5gs/*.log`.

You can modify the configuration file to record more logs.

```diff
diff -u /etc/open5gs/mme.yaml.old /etc/open5gs/mme.yaml
--- mme.yaml.old	2018-04-15 18:28:31.000000000 +0900
+++ mme.yaml	2018-04-15 19:53:10.000000000 +0900
@@ -2,6 +2,7 @@

 logger:
     file: /var/log/open5gs/mme.log
+    level: debug

 parameter:
```

After changing conf files, please restart Open5GS daemons.

```bash
$ sudo systemctl restart open5gs-mmed
$ sudo systemctl restart open5gs-sgwd
```

