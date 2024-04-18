---
title: Simple Issue
---

**Note:** This guide assumes you've already installed Open5GS and it's running with it's default values. If you're new to Open5GS check out the [Quickstart Guide]({{ site.url }}{{ site.baseurl }}/docs/guide/01-quickstart)
{: .notice--warning}

## Services Running

#### Confirming All Services are Running
Before we get too far in troubleshooting let's first check all our services are running.

```bash
ps aux | grep open5gs
open5gs  26633  0.1  0.0 2890828 12308 ?       Ssl  12:13   0:00 /usr/bin/open5gs-mmed -c /etc/open5gs/mme.yaml
open5gs  26784  0.0  0.1 1534456 18344 ?       Ssl  12:13   0:00 /usr/bin/open5gs-sgwcd -c /etc/open5gs/sgwc.yaml
open5gs  27076  0.1  0.2 3811148 42192 ?       Ssl  12:13   0:00 /usr/bin/open5gs-smfd -c /etc/open5gs/smf.yaml
open5gs  27174  0.0  0.0 258096 12720 ?        Ssl  12:13   0:00 /usr/bin/open5gs-amfd -c /etc/open5gs/amf.yaml
open5gs  26844  0.0  0.0 1237524 14084 ?       Ssl  12:13   0:00 /usr/bin/open5gs-sgwud -c /etc/open5gs/sgwu.yaml
open5gs  26934  0.0  0.0 707472 12732 ?        Ssl  12:13   0:00 /usr/bin/open5gs-upfd -c /etc/open5gs/upf.yaml
open5gs  27244  0.2  0.0 2861424 13584 ?       Ssl  12:13   0:00 /usr/bin/open5gs-hssd -c /etc/open5gs/hss.yaml
open5gs  27366  0.0  0.0 2890772 14380 ?       Ssl  12:13   0:00 /usr/bin/open5gs-pcrfd -c /etc/open5gs/pcrf.yaml
open5gs  27485  0.0  0.0 243816 15064 ?        Ssl  12:13   0:00 /usr/bin/open5gs-nrfd -c /etc/open5gs/nrf.yaml
open5gs  27485  0.0  0.0 243916 13064 ?        Ssl  12:13   0:00 /usr/bin/open5gs-scpd -c /etc/open5gs/scp.yaml
open5gs  27543  0.0  0.0 222416  9672 ?        Ssl  12:13   0:00 /usr/bin/open5gs-ausfd -c /etc/open5gs/ausf.yaml
open5gs  27600  0.0  0.0 222328  9668 ?        Ssl  12:13   0:00 /usr/bin/open5gs-udmd -c /etc/open5gs/udm.yaml
open5gs  27600  0.0  0.0 222329  9669 ?        Ssl  12:13   0:00 /usr/bin/open5gs-pcfd -c /etc/open5gs/pcf.yaml
open5gs  27600  0.0  0.0 222329  9669 ?        Ssl  12:13   0:00 /usr/bin/open5gs-nssfd -c /etc/open5gs/nssf.yaml
open5gs  27600  0.0  0.0 222329  9669 ?        Ssl  12:13   0:00 /usr/bin/open5gs-bsfd -c /etc/open5gs/bsf.yaml
open5gs  27697  0.0  0.0 243976 13716 ?        Ssl  12:13   0:00 /usr/bin/open5gs-udrd -c /etc/open5gs/udr.yaml
```

You should see each of the above services, MME, SGW-C, SMF, AMF, SGW-U, UPF, HSS, PCRF, NRF, SCP, AUSF, UDM, PCF, NSSF, BSF & UDR are all running.

If your instance doesn't show this make sure you're started each service:
```bash
$ systemctl start open5gs-mmed.service
$ systemctl start open5gs-sgwcd.service
$ systemctl start open5gs-smfd.service
$ systemctl start open5gs-amfd.service
$ systemctl start open5gs-sgwud.service
$ systemctl start open5gs-upfd.service
$ systemctl start open5gs-hssd.service
$ systemctl start open5gs-pcrfd.service
$ systemctl start open5gs-nrfd.service
$ systemctl start open5gs-scpd.service
$ systemctl start open5gs-ausfd.service
$ systemctl start open5gs-udmd.service
$ systemctl start open5gs-pcfd.service
$ systemctl start open5gs-nssfd.service
$ systemctl start open5gs-bsfd.service
$ systemctl start open5gs-udrd.service
```

#### Finding out why a Service isn't Starting

If a service isn't running check the log for that service - logs for each service live in */var/log/open5gs/* where each service logs to it's own file - MME logs in mme.log, AMF logs in amf.log, and so on. 

```bash
$ cat a.log
Open5GS daemon v2.4.11-100-gbea24d7

10/22 11:05:40.032: [app] INFO: Configuration: '/home/acetcom/Documents/git/open5gs/install/etc/open5gs/mme.yaml' (../lib/app/ogs-init.c:126)
10/22 11:05:40.032: [app] INFO: File Logging: 'a.log' (../lib/app/ogs-init.c:129)
10/22 11:05:40.094: [gtp] INFO: gtp_server() [127.0.0.2]:2123 (../lib/gtp/path.c:30)
10/22 11:05:40.094: [gtp] INFO: gtp_connect() [127.0.0.3]:2123 (../lib/gtp/path.c:60)
10/22 11:05:40.094: [mme] INFO: s1ap_server() [127.0.0.2]:36412 (../src/mme/s1ap-sctp.c:62)
10/22 11:05:40.094: [sctp] INFO: MME initialize...done (../src/mme/app-init.c:33)
```

Or, you can use `journalctl` like below to view live log.

```bash
$ journalctl -u open5gs-mmed.service --since today -f
```

In the example above we can see the error - no SGW-C GTP-C address is configured in the mme.yaml file, meaning Open5GS MME is failing to start.

The errors you experience may be different, but if a service is failing to start it's most often due to a misconfiguration issue in one or more of the Open5GS *.yaml* configuration files. The log should tell you which section of the yaml file is missing or invalid.


## gNB/eNB Connection Issues
---
When a UE connects to Open5GS AMF/MME the log shows the presence of a new NGAP/S1AP connection the log at */var/log/open5gs/mme.log*:

##### No NGAP/S1AP Connection
If you're not seeing any NGAP/S1AP connection attempts check the gNB/eNB can contact the IP the AMF/MME is on (No firewall / ACLs etc blocking) and that SCTP Traffic is able to be carried across your transmission network. 

**Note:** 3GPP defines SCTP as the transport protocol for NGAP/S1AP traffic (not TCP/UDP). Not all devices / routers support NGAP/S1AP, particularly over the Internet.

If you're confident the service is running and connectivity is able to be established across your transmission network, you should see the *SCTP INIT* packets in Wireshark. If you're not seeing these packets go back and check your network.

If you are seeing the SCTP INIT messages and seeing an ABORT after each one, that suggests the SCTP connection is trying to be established. Check that the AMF/MME service is started and listening on the interface / IP you're sending traffic to.

##### NGAP/S1AP Connection Rejected
If you're seeing NGAP/S1AP Connection attempts but seeing them rejected by Open5GS, the NGAP/S1AP message show in a packet capture will indicate the rejection reason, as well as in the mme log.
```
$ tail -f /var/log/open5gs/mme.log
[mme] INFO: eNB-S1[10.0.1.14] connection refused!!! (mme-sm.c:176)
```

Typically NGAP/S1AP connections are rejected due to one of these reasons:
* MNC / MCC in gNB/eNB does not match *guami/gummei* and *tai* MCC/MNC pair in amf.yaml/mme.yaml*.
* Tracking Area Code does not match configured TACs in *amf.yaml/mme.yaml*.

Each of these can be addressed by editing the relevant section in the AMF/MME config in */etc/open5gs/amf.yaml* OR */etc/open5gs/mme.yaml*

__Example of successful eNB connection to MME:__
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
* Check your gNB/eNB is connected to the AMF/MME
* Check the gNB/eNB status
* Check the UE you are scanning with is capable of working on the frequencies / bands and duplex mode (TDD or FDD) used by the UE
* Check UE is within range of gNB/eNB
* Check PLMN is not forbidden on USIM (F-PLMN List)

#### UE Fails to Register/Attach to the Network
Assuming while scanning for networks the UE can see the network, but not connect, the most common issues stem from Authentication.

NR/LTE employs *Mutual Authentication* of both the network and the subscriber. This means the credentials in the UDM/HSS must match the credentials on the USIM and the credentials in the USIM must match those in the UDM/HSS. This means unlike GSM, you cannot use just any SIM and disable crypto, you have to know the details on the USIM or be able to program this yourself in order to authenticate.

If the issue is authentication, the amf/mme and udm/hss log will give you an indication as to which side is rejecting the authentication, the UE or the Network (Open5GS);

__IMSI/Subscriber not present in UDM/HSS:__
If the USIM's IMSI is not present in the UDM/HSS the UDM/HSS will reject the Authentication.

```
$ tail -f /var/log/open5gs/hss.log
[hss] WARNING: Cannot find IMSI in DB : 001000000000001 (hss-context.c:309)
```

__IMSI/Subscriber configured in UDM/HSS with wrong credentails:__
If the credentials on the UDM/HSS do not match what is configured on the USIM, the USIM will reject the connection (MAC Error).

```
$ tail -f /var/log/open5gs/mme.log
[emm] WARNING: Authentication failure(MAC failure) (emm-sm.c:573)
```

__DNN/APN Requested by UE not present in UDM/HSS:__
Ensure the DNNs/APNs requested by the UE are present in the UDM/HSS.


## Further Debugging
---

Problem with Open5GS can be filed as [GitHub Issues](https://github.com/{{ site.github_username }}/open5gs/issues). Please include the following to get help:

- Attach `*.pcapng` file created by wireskark.
- Attach configuration files at `/etc/open5gs/*.yaml`.
- Attach log files at `/var/log/open5gs/*.log`.

You can modify the configuration file to record more logs.

```diff
$ diff --git a/configs/open5gs/amf.yaml.in b/configs/open5gs/amf.yaml.in
index a70143f08..e0dba560c 100644
--- a/configs/open5gs/amf.yaml.in
+++ b/configs/open5gs/amf.yaml.in
@@ -1,6 +1,6 @@
 logger:
     file:
       path: @localstatedir@/log/open5gs/amf.log
-#    level: info   # fatal|error|warn|info(default)|debug|trace
+    level: debug

 max:
     ue: 1024  # The number of UE can be increased depending on memory size.
```

After changing conf files, please restart Open5GS daemons.

```bash
$ sudo systemctl restart open5gs-amfd.service
```

