---
title: Quickstart
---

**Note:** Open5GS supports installation of packages in *Debian/Ubuntu and openSUSE* environments. *CentOS, Fedora, FreeBSD, and Mac OSX* require you to [build with source code]({{ site.url }}{{ site.baseurl }}/docs/guide/02-building-open5gs-from-sources)
{: .notice--warning}

### Install Open5GS with a Package Manager
---

The Open5GS package is available on the recent versions of *Ubuntu*.

```bash
$ sudo apt update
$ sudo apt install software-properties-common
$ sudo add-apt-repository ppa:open5gs/latest
$ sudo apt update
$ sudo apt install open5gs
```

The Open5GS package is also available on [OBS](https://build.opensuse.org/project/show/home:acetcom:open5gs). First, install the authentication key as shown below.

```bash
$ sudo apt update
$ sudo apt install wget gnupg
$ wget https://download.opensuse.org/repositories/home:/acetcom:/open5gs:/latest/Debian_10/Release.key
$ sudo apt-key add Release.key

```

In Debian 10(buster), you can install it as follows:

```bash
$ sudo sh -c "echo 'deb http://download.opensuse.org/repositories/home:/acetcom:/open5gs:/latest/Debian_10/ ./' > /etc/apt/sources.list.d/open5gs.list"
$ sudo apt update
$ sudo apt install open5gs
```

Other Linux distributions can be installed by changing the path.

```
http://download.opensuse.org/repositories/home:/acetcom:/open5gs:/latest/Debian_10/
http://download.opensuse.org/repositories/home:/acetcom:/open5gs:/latest/Debian_Testing/
http://download.opensuse.org/repositories/home:/acetcom:/open5gs:/latest/Debian_Unstable/
http://download.opensuse.org/repositories/home:/acetcom:/open5gs:/latest/Raspbian_10/
http://download.opensuse.org/repositories/home:/acetcom:/open5gs:/latest/xUbuntu_18.04/
http://download.opensuse.org/repositories/home:/acetcom:/open5gs:/latest/xUbuntu_19.04/
http://download.opensuse.org/repositories/home:/acetcom:/open5gs:/latest/xUbuntu_19.10/
```

[Martin Hauke](https://build.opensuse.org/user/show/mnhauke) packaged Open5GS for *openSUSE* on [OBS](https://build.opensuse.org/package/show/home:mnhauke:nextepc/nextepc).

```bash
$ sudo zypper addrepo -f obs://home:mnhauke:nextepc home:mnhauke:nextepc
$ sudo zypper install nextepc
$ sudo zypper install mongodb-server mongodb-shell
```

### Configure Open5GS
---

Modify [/etc/open5gs/mme.yaml](https://github.com/{{ site.github_username }}/open5gs/blob/master/configs/open5gs/mme.yaml.in) to set the S1AP/GTP-C IP address, PLMN ID, and TAC

```diff
diff -u /etc/open5gs/mme.yaml.old /etc/open5gs/mme.yaml
--- mme.yaml.old	2018-04-15 18:28:31.000000000 +0900
+++ mme.yaml	2018-04-15 19:53:10.000000000 +0900
@@ -8,18 +8,20 @@ parameter:
 mme:
     freeDiameter: /etc/freeDiameter/mme.conf
     s1ap:
+      addr: 192.168.0.100
     gtpc:
+      addr: 192.168.0.100
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
-      tac: 12345
+        mcc: 901
+        mnc: 70
+      tac: 7
     security:
         integrity_order : [ EIA1, EIA2, EIA0 ]
         ciphering_order : [ EEA0, EEA1, EEA2 ]
```

Modify [/etc/open5gs/sgw.yaml](https://github.com/{{ site.github_username }}/open5gs/blob/master/configs/open5gs/sgw.yaml.in) to set the GTP-U IP address.  
```diff
diff -u /etc/open5gs/sgw.yaml.old /etc/open5gs/sgw.yaml
--- sgw.yaml.old	2018-04-15 18:30:25.000000000 +0900
+++ sgw.yaml	2018-04-15 18:30:30.000000000 +0900
@@ -14,3 +14,4 @@
     gtpc:
       addr: 127.0.0.2
     gtpu:
+      addr: 192.168.0.100
```

After changing conf files, please restart Open5GS daemons.

```bash
$ sudo systemctl restart open5gs-mmed
$ sudo systemctl restart open5gs-sgwd
```

### Install WebUI of Open5GS
---

[Node.js](https://nodejs.org/) is required to install WebUI of Open5GS

1. *Debian and Ubuntu* based Linux distributions can install [Node.js](https://nodejs.org/) as follows:

    ```bash
    $ sudo apt update
    $ sudo apt install curl
    $ curl -sL https://deb.nodesource.com/setup_12.x | sudo -E bash -
    $ sudo apt install nodejs
    ```

2. To install [Node.js](https://nodejs.org/) on *openSUSE*, run the following:

    ```bash
    $ sudo zypper install nodejs8
    ```

You can now install WebUI of Open5GS.

```bash
$ curl -sL {{ site.url }}{{ site.baseurl }}/assets/webui/install | sudo -E bash -
```

### Register Subscriber Information
---

Connect to `http://localhost:3000` and login with **admin** account.

> Username : admin  
> Password : 1423

**Tip:** You can change the password in _Account_ Menu.
{: .notice--info}

To add subscriber information, you can do WebUI operations in the following order:

  1. Go to `Subscriber` Menu.
  2. Click `+` Button to add a new subscriber.
  3. Fill the IMSI, security context(K, OPc, AMF), and APN of the subscriber.
  4. Click `SAVE` Button

**Tip:** This addition immediately affects Open5GS without restaring any daemon.
{: .notice--info}


### Adding a route for UE to have Internet connectivity {#UEInternet}
---

If your phone can connect to internet, you must run the following command in Open5GS-PGW installed host. 

```bash
### Check IP Tables
$ sudo iptables -L
Chain INPUT (policy ACCEPT)
target     prot opt source               destination

Chain FORWARD (policy ACCEPT)
target     prot opt source               destination

Chain OUTPUT (policy ACCEPT)
target     prot opt source               destination

### Check NAT Tables
$ sudo iptables -L -t nat
Chain PREROUTING (policy ACCEPT)
target     prot opt source               destination

Chain INPUT (policy ACCEPT)
target     prot opt source               destination

Chain OUTPUT (policy ACCEPT)
target     prot opt source               destination

Chain POSTROUTING (policy ACCEPT)
target     prot opt source               destination

### Enable IPv4 Forwarding
$ sudo sh -c "echo 1 > /proc/sys/net/ipv4/ip_forward"

### Add NAT Rule
$ sudo iptables -t nat -A POSTROUTING -s 45.45.0.0/16 ! -o ogstun -j MASQUERADE
```

**Note:** For the first time, it is a good condition if you do not have any rules in the IP/NAT tables. If a program such as docker has already set up a rule, you will need to add a rule differently.
{: .notice--danger}

### Turn on your eNodeB and Phone
---
- Connect your eNodeB to the IP of your server via the standard S1AP port of SCTP 36412 (for MME)
- You can see actual traffic through wireshark -- [[srsenb.pcapng]]({{ site.url }}{{ site.baseurl }}/assets/pcapng/srsenb.pcapng).
- You can view the log at `/var/log/open5gs/*.log`.

### Troubleshooting
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

#### Additional Troubleshooting

After successfully attaching the UE (e.g. `45.45.0.2`) to the EPC if the UE device doesn't have access to internet,
it is recommended to do the following checks:
- Check if the interface connected to the internet is correctly `NAT` with the `ogstun` interface. Follow the documentation [Here](#UEInternet)
   - Ensure that the packets in the `INPUT` chain to the `ogstun` interface are accepted 
   ```
   $ sudo iptables -I INPUT -i ogstun -j ACCEPT
   ```
- Check if the UE's IP can be pinged successfully by performing `ping <IP of UE>` e.g.`ping 45.45.0.2`
- Configure the firewall correctly. Some operating systems (Ubuntu) by default enable firewall rules to block traffic
   - Explicitly disable it to see if it resolves the problem of granting data access to the UE by doing
   ```
   $ sudo ufw disable
   ```


### Uninstall Open5GS and WebUI

How to remove Open5GS package:

1. On *Ubuntu*:

    ```bash
    $ sudo apt purge open5gs
    $ sudo apt autoremove
    ```

2. On *openSUSE*:

    ```bash
    $ sudo zypper rm open5gs
    ```

You may need to remove manually `/var/log/open5gs` unless it is empty.

```bash
$ sudo rm -Rf /var/log/open5gs
```

The WebUI of Open5GS can be removed as follows:

```bash
curl -sL {{ site.url }}{{ site.baseurl }}/assets/webui/uninstall | sudo -E bash -
```

