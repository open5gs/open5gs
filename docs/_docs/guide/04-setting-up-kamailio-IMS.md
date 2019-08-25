---
title: Setting up Kamailio IMS 
head_inline: "<style> .blue { color: blue; } </style>"
---

Setting up Kamailio IMS in OpenStack VM and connecting P-CSCF of Kamailio IMS with PCRF of NextEPC (Running on another OpenStak VM) - Install from deb packages
{: .blue}


#### 1. Start from Bionic Ubuntu cloud image
#### 2. Use the following Cloud init config while spawning an instance

```
#cloud-config
disable_root: 0
ssh_pwauth: True
users:
  - name: root
chpasswd:
  list: |
    root:admin
  expire: False
runcmd:
  - sed -i -e '/^#PermitRootLogin/s/^.*$/PermitRootLogin yes/' /etc/ssh/sshd_config
  - systemctl restart sshd
```

This removes all existing cloud users and allows only root user and sets a password
{: .notice--warning}

#### 3. Install following packages

```
$ apt update && apt upgrade -y && apt install -y mysql-server tcpdump screen ntp ntpdate git-core dkms gcc flex bison libmysqlclient-dev make \
                                                 libssl-dev libcurl4-openssl-dev libxml2-dev libpcre3-dev bash-completion g++ autoconf rtpproxy libmnl-dev \
                                                 libsctp-dev ipsec-tools
```

#### 4. Install all required Kamailio packages (v5.2)

```
$ wget -O- http://deb.kamailio.org/kamailiodebkey.gpg | sudo apt-key add -
$ add-apt-repository 'deb http://deb.kamailio.org/kamailio52 bionic main'
$ apt install -y kamailio kamailio-mysql-modules kamailio-ims-modules kamailio-presence-modules kamailio-tls-modules kamailio-xml-modules kamailio-xmlrpc-modules
```

#### 5. Create necessary folders for kamailio pid files and set ownership

Create the directory for pid file:

```
$ mkdir -p /var/run/kamailio
$ mkdir -p /var/run/kamailio_icscf
$ mkdir -p /var/run/kamailio_pcscf
$ mkdir -p /var/run/kamailio_scscf
```

Default setting is to run Kamailio as user kamailio and group kamailio. For that you need to create the user:

```
$ adduser --quiet --system --group --disabled-password \
        --shell /bin/false --gecos "Kamailio" \
        --home /var/run/kamailio kamailio

$ adduser --quiet --system --group --disabled-password \
        --shell /bin/false --gecos "Kamailio" \
        --home /var/run/kamailio_icscf kamailio

$ adduser --quiet --system --group --disabled-password \
        --shell /bin/false --gecos "Kamailio" \
        --home /var/run/kamailio_pcscf kamailio

$ adduser --quiet --system --group --disabled-password \
        --shell /bin/false --gecos "Kamailio" \
        --home /var/run/kamailio_scscf kamailio
```

Set ownership:

```
$ chown kamailio:kamailio /var/run/kamailio
$ chown kamailio:kamailio /var/run/kamailio_icscf
$ chown kamailio:kamailio /var/run/kamailio_pcscf
$ chown kamailio:kamailio /var/run/kamailio_scscf
```

#### 6. Clone Kamailio repository and checkout 5.2 version of repository

```
$ mkdir -p /usr/local/src/kamailio-5.2
$ cd /usr/local/src/kamailio-5.2/
$ git clone git://git.kamailio.org/kamailio kamailio
$ cd kamailio
$ git checkout -b 5.2 origin/5.2
```

#### 7. Edit /etc/hosts file for hostname resolution

Edit the /etc/hosts file as follows:

In the below example ims-deb is the hostname of the machine, PCRF is running in 10.4.128.11/172.24.15.3 (Floating IP) machine and IMS running at 10.4.128.7/172.24.15.21 (Floating IP)

Notice the change in PCRF address resolution (It should be the machine IP where PCRF is running)
{: .notice--warning}

```
root@ims-deb:~# cat /etc/hosts
127.0.0.1   localhost
127.0.1.1   ims-deb
10.4.128.7  hss.mnc096.mcc262.3gppnetwork.org     mnc096.mcc262.3gppnetwork.org
10.4.128.7  icscf.mnc096.mcc262.3gppnetwork.org   mnc096.mcc262.3gppnetwork.org
10.4.128.7  pcscf.mnc096.mcc262.3gppnetwork.org   mnc096.mcc262.3gppnetwork.org
10.4.128.7  scscf.mnc096.mcc262.3gppnetwork.org   mnc096.mcc262.3gppnetwork.org
172.24.15.3 pcrf.mnc096.mcc262.3gppnetwork.org    mnc096.mcc262.3gppnetwork.org

# The following lines are desirable for IPv6 capable hosts
::1 ip6-localhost ip6-loopback
fe00::0 ip6-localnet
ff00::0 ip6-mcastprefix
ff02::1 ip6-allnodes
ff02::2 ip6-allrouters
ff02::3 ip6-allhosts
```

After editing the file, save it and reboot the machine
{: .notice--info}


#### 8. Populate MySQL database using kamctlrc command

Edit SIP_DOMAIN and DBENGINE in the /etc/kamailio/kamctlrc configuration file (Used by kamctl and kamdbctl tools).

Set the SIP_DOMAIN to your SIP service domain (or IP address if you don't have a DNS hostname associated with your SIP service).
Set the DBENGINE to be MYSQL and adjust other setting as you want. Finally, uncomment both SIP_DOMAIN and DBENGINE.

In example above, the following values are set for SIP_DOMAIN and DBENGINE

```
SIP_DOMAIN=mnc096.mcc262.3gppnetwork.org
DBENGINE=MYSQL
```

You can change other values in kamctlrc file. Once you are done updating kamctlrc file, run the script to create the database used by Kamailio:

```
$ kamdbctl create
```

When prompted for mysql root user password enter the root password if its is set or else leave it blank i.e. Press Enter
{: .notice--info}

check database manually;
```
$ mysql
<mysql> show databases;
<mysql> use kamailio;
<mysql> show tables;
<mysql> select * from subscriber;
```

No Subscribers are added yet

The kamdbctl will add two users in MySQL user tables:
```
- kamailio   - (with default password 'kamailiorw') - user which has full access rights to 'kamailio' database
- kamailioro - (with default password 'kamailioro') - user which has read-only access rights to 'kamailio' database
```

#### 9. Edit /etc/default/rtpproxy file as follows:

```
# Defaults for rtpproxy

# The control socket.
#CONTROL_SOCK="unix:/var/run/rtpproxy/rtpproxy.sock"
# To listen on an UDP socket, uncomment this line:
CONTROL_SOCK=udp:127.0.0.1:22222

# Additional options that are passed to the daemon.
EXTRA_OPTS="-l 172.24.15.21 -d DBUG:LOG_LOCAL0"
```

here, `-l <PUBLIC_IP>`

Then run,

```
$ systemctl restart rtpproxy
```

#### 10. Edit configuration file to fit your requirements for the VoIP platform, you have to edit the /etc/kamailio/kamailio.cfg configuration file

Follow the instruction in the comments to enable usage of MySQL. Basically you have to add several lines at the top of config file, like:

```
#!define WITH_MYSQL
#!define WITH_AUTH
#!define WITH_USRLOCDB
#!define WITH_NAT (Include this if client and/or SIP server is behind a NAT)

(uncomment this line and enter the DNS domain created above)
alias="mnc096.mcc262.3gppnetwork.org"

(uncomment this line, 10.4.128.7 is the internal IP and 172.24.15.21 is the Public/Floating IP)
listen=udp:10.4.128.7:5060 advertise 172.24.15.21:5060
```

Further down, we will need to modify the rtpproxy_sock value to match the CONTROL_SOCK option we set for RTPProxy in /etc/default/rtpproxy
```
modparam("rtpproxy", "rtpproxy_sock", "udp:127.0.0.1:22222")
```

If you changed the password for the 'kamailio' user of MySQL, you have to update the value for 'DBURL' parameters.

#### 11. Run Kamailio SIP server

```
$ systemctl start kamailio
```

#### 12. A quick check for the basic working of SIP server can be done as follows:

Create new subscriber accounts. A new account can be added using `kamctl` tool via `kamctl add <username> <password>`
(When asked for entering MySQL password for user 'kamailio@localhost': type 'kamailiorw', as provided in kamailio.cfg)

```
$ kamctl add test testpasswd
$ kamctl add test2 testpasswd
```

Setting on OnePlus phones

- Connect to a network through which SIP server is reachable (either Wi-Fi or LTE)
- Goto phone dialer and select the "Settings" in the menu on top right corner
- Then select "Call settings"
- Configure SIP accounts in phones as added above using kamctl:

In Phone 1:

```
Username: test
Password: testpasswd
Server: mnc096.mcc262.3gppnetwork.org (Created DNS Domain Name or IP to which IMS components are bound to, visible interface IP address)
Optional Settings:
	Authentication username: test
	Outbound proxy address: 172.24.15.21 (Floating IP of VM in case of OpenStack or else no need to fill in case of physical machine)
	Transport type: UDP
```

In Phone 2:

```
Username: test2
Password: testpasswd
Server: mnc096.mcc262.3gppnetwork.org (Created DNS Domain Name or IP to which IMS components are bound to, visible interface IP address)
Optional Settings:
	Authentication username: test2
	Outbound proxy address: 172.24.15.21 (Floating IP of VM in case of OpenStack or else no need to fill in case of physical machine)
	Transport type: UDP
```

- Set "Receive incoming calls" option to enabled state in both phones
- Set "Use SIP calling" to "For all calls"
- Add a new contact as follows:

In Phone 1:

Select "more" option

```
Name: SIP Contact test2 (Any arbitary name)
SIP: test2@mnc096.mcc262.3gppnetwork.org (Created DNS Domain Name or IP to which IMS components are bound to, visible interface IP address)
```

Save and exit

In Phone 2:

Select "more" option

```
Name: SIP Contact test (Any arbitary name)
SIP: test@mnc096.mcc262.3gppnetwork.org (Created DNS Domain Name or IP to which IMS components are bound to, visible interface IP address)
```

Save and exit

- Now try calling from either phone

#### 13. Create new mysql database for pcscf, scscf and icscf, populate databases and grant permissions to respective users identified by a password

```
$ mysql
<mysql> CREATE DATABASE  `pcscf`;
<mysl> CREATE DATABASE  `scscf`;
<mysl> CREATE DATABASE  `icscf`;
```

In all of the below steps, when prompted for mysql root user password, leave it blank i.e. Press Enter

```
$ cd /usr/local/src/kamailio-5.2/kamailio/utils/kamctl/mysql
$ mysql -u root -p pcscf < standard-create.sql
$ mysql -u root -p pcscf < presence-create.sql
$ mysql -u root -p pcscf < ims_usrloc_pcscf-create.sql
$ mysql -u root -p pcscf < ims_dialog-create.sql

$ mysql -u root -p scscf < standard-create.sql
$ mysql -u root -p scscf < presence-create.sql
$ mysql -u root -p scscf < ims_usrloc_scscf-create.sql
$ mysql -u root -p scscf < ims_dialog-create.sql
$ mysql -u root -p scscf < ims_charging_create.sql

$ cd /usr/local/src/kamailio-5.2/kamailio/misc/examples/ims/icscf
$ mysql -u root -p icscf < icscf.sql
```

Verify that following tables are present in respective databases by logging into mysql

```
+-----------------+
| Tables_in_pcscf |
+-----------------+
| active_watchers |
| dialog_in       |
| dialog_out      |
| dialog_vars     |
| location        |
| presentity      |
| pua             |
| version         |
| watchers        |
| xcap            |
+-----------------+

+-----------------+
| Tables_in_scscf |
+-----------------+
| active_watchers |
| contact         |
| dialog_in       |
| dialog_out      |
| dialog_vars     |
| impu            |
| impu_contact    |
| impu_subscriber |
| presentity      |
| pua             |
| ro_session      |
| subscriber      |
| version         |
| watchers        |
| xcap            |
+-----------------+

+---------------------+
| Tables_in_icscf     |
+---------------------+
| nds_trusted_domains |
| s_cscf              |
| s_cscf_capabilities |
+---------------------+
```

```
<mysql> grant delete,insert,select,update on pcscf.* to pcscf@localhost identified by 'heslo';
<mysql> grant delete,insert,select,update on scscf.* to scscf@localhost identified by 'heslo';
<mysl> grant delete,insert,select,update on icscf.* to icscf@localhost identified by 'heslo';
<mysl> grant delete,insert,select,update on icscf.* to provisioning@localhost identified by 'provi';
```

#### 14. Copy pcscf, icscf and scscf configuration files to /etc/ folder and edit accordingly

```
$ cd /usr/local/src/kamailio-5.2/kamailio/misc/examples/ims/
$ cp -r icscf/ /etc/kamailio_icscf
$ cp -r pcscf/ /etc/kamailio_pcscf
$ cp -r scscf/ /etc/kamailio_scscf
```

Rename files in these folder kamailio_pcscf, kamailio_icscf and kamailio_scscf by removing .sample part from the configuration files.

And, rename kamailio.cfg in respective folder as follows: kamailio_pcscf.cfg, kamailio_icscf.cfg and kamailio_scscf.cfg

Edit the configuration files as per your deployment needs

### I-CSCF

Changes required in icscf.cfg:

```
# SIP / UDP
listen=udp:10.4.128.7:4060

alias=icscf.mnc096.mcc262.3gppnetwork.org

#!define NETWORKNAME "mnc096.mcc262.3gppnetwork.org" 
#!define HOSTNAME "icscf.mnc096.mcc262.3gppnetwork.org"

#!define DB_URL "mysql://icscf:heslo@localhost/icscf"
```

Changes required in icscf.xml:

```
FQDN="icscf.mnc096.mcc262.3gppnetwork.org"
Realm="mnc096.mcc262.3gppnetwork.org"

<Peer FQDN="hss.mnc096.mcc262.3gppnetwork.org" Realm="mnc096.mcc262.3gppnetwork.org" port="3868"/>

<Acceptor port="3869" bind="10.4.128.7"/>

<DefaultRoute FQDN="hss.mnc096.mcc262.3gppnetwork.org" metric="10"/>
```

Changes required in kamailio_icscf.cfg:

```
# ------------------ module loading ----------------------------------
mpath="/usr/lib64/kamailio/modules_k/:/usr/lib64/kamailio/modules/:/usr/lib/kamailio/modules_k/:/usr/lib/kamailio/modules/:/usr/lib/x86_64-linux-gnu/kamailio/modules/"
```

To perform a quick test edit the /etc/default/kamailio file, by changing the configuration file parameter as follows:

```
# Config file
CFGFILE=/etc/kamailio_icscf/kamailio_icscf.cfg
```

After altering the above file, execute below command

```
$ systemctl restart kamailio.service
```

And, check that there are no error by viewing logs using the below command

```
$ journalctl -f --unit kamailio
```

### P-CSCF 

Edit the DNS domain names, DB URL and IP addresses at all places in the pcscf.cfg, pcscf.xml files accordingly

Changes required in pcscf.cfg:

```
# SIP / UDP
listen=udp:10.4.128.7:5060

#!define IPSEC_LISTEN_ADDR "10.4.128.7"

alias=pcscf.mnc096.mcc262.3gppnetwork.org

#!define PCSCF_URL "sip:pcscf.mnc096.mcc262.3gppnetwork.org"

#!subst "/NETWORKNAME/mnc096.mcc262.3gppnetwork.org/"
#!subst "/HOSTNAME/pcscf.mnc096.mcc262.3gppnetwork.org/"

#!define DB_URL "mysql://pcscf:heslo@localhost/pcscf"

#!define SQLOPS_DBURL "pcscf=>mysql://pcscf:heslo@localhost/pcscf"

##!define TRF_FUNCTION "trf.mnc096.mcc262.3gppnetwork.org"

#!define WITH_RX
#!define WITH_RX_REG
#!define WITH_RX_CALL

#!define WITH_IPSEC
```

Changes required in pcscf.xml:

```
FQDN="pcscf.mnc096.mcc262.3gppnetwork.org"

Realm="mnc096.mcc262.3gppnetwork.org"

<Peer FQDN="pcrf.mnc096.mcc262.3gppnetwork.org" Realm="mnc096.mcc262.3gppnetwork.org" port="3868"/>

<Acceptor port="3871" bind="10.4.128.7"/>

<DefaultRoute FQDN="pcrf.mnc096.mcc262.3gppnetwork.org" metric="10"/>
```

Changes required in kamailio_pcscf.cfg:

```
# ------------------ module loading ----------------------------------
mpath="/usr/lib64/kamailio/modules_k/:/usr/lib64/kamailio/modules/:/usr/lib/kamailio/modules_k/:/usr/lib/kamailio/modules/:/usr/lib/x86_64-linux-gnu/kamailio/modules/"

# AVP's required for Fail-Over-Support:
#modparam("dispatcher", "dst_avp", "$avp(DISPATCHER_DST_AVP)")
#modparam("dispatcher", "grp_avp", "$avp(DISPATCHER_GRP_AVP)")
#modparam("dispatcher", "cnt_avp", "$avp(DISPATCHER_CNT_AVP)")
#modparam("dispatcher", "sock_avp", "$avp(DISPATCHER_SOCK_AVP)")

#modparam("ims_usrloc_pcscf", "hashing_type", 2)
```

Changes required in dispatcher.list:

```
# SBC's
2 sip:10.4.128.7:5070
```

To perform a quick test edit the /etc/default/kamailio file, by changing the configuration file parameter as follows:

```
# Config file
CFGFILE=/etc/kamailio_pcscf/kamailio_pcscf.cfg
```

After altering the above file, execute below command

```
$ systemctl restart kamailio.service
```

And, check that there are no error by viewing logs using the below command (Ignore the rtpengine related for now, as we will install in next step)

```
$ journalctl -f --unit kamailio
```

### S-CSCF

Edit the DNS domain names, DB URL and IP addresses at all places in the scscf.cfg, scscf.xml files accordingly

Changes required in scscf.cfg:

```
# SIP / UDP
listen=udp:10.4.128.7:6060

#!define NETWORKNAME "mnc096.mcc262.3gppnetwork.org"
#!define HOSTNAME "scscf.mnc096.mcc262.3gppnetwork.org"
#!define URI "sip:scscf.mnc096.mcc262.3gppnetwork.org:6060"

alias=scscf.mnc096.mcc262.3gppnetwork.org

# ENUM-Server to query:
#!define ENUM_SUFFIX "mnc096.mcc262.3gppnetwork.org."

#!define DB_URL "mysql://scscf:heslo@localhost/scscf"

#!define RO_MNC "96"
```

Changes required in scscf.xml:

```
FQDN="scscf.mnc096.mcc262.3gppnetwork.org"

Realm="mnc096.mcc262.3gppnetwork.org"

<Peer FQDN="hss.mnc096.mcc262.3gppnetwork.org" Realm="mnc096.mcc262.3gppnetwork.org" port="3868"/>

<Acceptor port="3870" bind="10.4.128.7"/>

<DefaultRoute FQDN="hss.mnc096.mcc262.3gppnetwork.org" metric="10"/>
```

Changes required in kamailio_scscf.cfg:

```
# ------------------ module loading ----------------------------------
mpath="/usr/lib64/kamailio/modules_k/:/usr/lib64/kamailio/modules/:/usr/lib/kamailio/modules_k/:/usr/lib/kamailio/modules/:/usr/lib/x86_64-linux-gnu/kamailio/modules/"

# AVP's required for Fail-Over-Support:
#modparam("dispatcher", "dst_avp", "$avp(DISPATCHER_DST_AVP)")
#modparam("dispatcher", "grp_avp", "$avp(DISPATCHER_GRP_AVP)")
#modparam("dispatcher", "cnt_avp", "$avp(DISPATCHER_CNT_AVP)")
#modparam("dispatcher", "sock_avp", "$avp(DISPATCHER_SOCK_AVP)")
```

Changes required in dispatcher.list: Not sure what to do hence comment out as follows

```
# ng-voice Interconnect
#1 sip:sbc-1.ng-voice.com
#1 sip:sbc-2.ng-voice.com
```

To perform a quick test edit the /etc/default/kamailio file, by changing the configuration file parameter as follows:

```
# Config file
CFGFILE=/etc/kamailio_scscf/kamailio_scscf.cfg
```

After altering the above file, execute below command

```
$ systemctl restart kamailio.service
```

And, check that there are no error by viewing logs using the below command

```
$ journalctl -f --unit kamailio
```

Can run S-CSCF as follows: `kamailio -f kamailio_scscf.cfg`


#### 15. Install RTPEngine

Check for dependencies, install dependencies and build .deb packages

```
$ export DEB_BUILD_PROFILES="pkg.ngcp-rtpengine.nobcg729"
$ apt install dpkg-dev
$ git clone https://github.com/sipwise/rtpengine
$ cd rtpengine && git checkout mr7.4.1
$ dpkg-checkbuilddeps
```
The above command checks for dependencies and give you a list of dependencies which are missing in the system. The below list is the result of this command

```
$ apt install debhelper default-libmysqlclient-dev gperf iptables-dev libavcodec-dev libavfilter-dev libavformat-dev libavutil-dev libbencode-perl libcrypt-openssl-rsa-perl libcrypt-rijndael-perl libdigest-crc-perl libdigest-hmac-perl libevent-dev libhiredis-dev libio-multiplex-perl libio-socket-inet6-perl libiptc-dev libjson-glib-dev libnet-interface-perl libpcap0.8-dev libsocket6-perl libspandsp-dev libswresample-dev libsystemd-dev libxmlrpc-core-c3-dev markdown dkms module-assistant keyutils libnfsidmap2 libtirpc1 nfs-common rpcbind
```

After installing dependencies run the below command again and verify that no dependencies are left out

```
$ dpkg-checkbuilddeps
```

This should just return back to shell with no output if all depedencies are met

```
$ dpkg-buildpackage -uc -us
$ cd ..
$ dpkg -i *.deb
$ cp /etc/rtpengine/rtpengine.sample.conf /etc/rtpengine/rtpengine.conf
```

Edit this file as follows under "[rtpengine]":

```
interface = 10.4.128.7!172.24.15.21
```

Port on which rtpengine binds i.e. listen_ng parameter is udp port 2223. This should be updated in kamailio_pcscf.cfg file at modparam(rtpengine ...

```
# ----- rtpproxy params -----
modparam("rtpengine", "rtpengine_sock", "1 == udp:localhost:2223")
```

```
$ cp /etc/rtpengine/rtpengine-recording.sample.conf /etc/rtpengine/rtpengine-recording.conf
$ mkdir /var/spool/rtpengine
$ systemctl restart ngcp-rtpengine-daemon.service ngcp-rtpengine-recording-daemon.service ngcp-rtpengine-recording-nfs-mount.service
$ systemctl enable ngcp-rtpengine-daemon.service ngcp-rtpengine-recording-daemon.service ngcp-rtpengine-recording-nfs-mount.service

$ systemctl stop rtpproxy
$ systemctl disable rtpproxy
$ systemctl mask rtpproxy
```

#### 16. Running I-CSCF, P-CSCF and S-CSCF as separate systemctl process

First, stop the default kamailio SIP server

```
$ systemctl stop kamailio
$ systemctl disable kamailio
$ systemctl mask kamailio
```

Copy the init file each of the process you need

```
$ cp /etc/init.d/kamailio /etc/init.d/kamailio_icscf
$ cp /etc/init.d/kamailio /etc/init.d/kamailio_pcscf
$ cp /etc/init.d/kamailio /etc/init.d/kamailio_scscf
```

Changes required in /etc/init.d/kamailio_icscf

```
NAME="kamailio_icscf"
CFGFILE=/etc/$NAME/kamailio_icscf.cfg
```

Changes required in /etc/init.d/kamailio_pcscf

```
NAME="kamailio_pcscf"
CFGFILE=/etc/$NAME/kamailio_pcscf.cfg
```

Changes required in /etc/init.d/kamailio_scscf

```
NAME="kamailio_scscf"
CFGFILE=/etc/$NAME/kamailio_scscf.cfg
```

```
$ cd /etc/default/
$ cp kamailio kamailio_icscf
$ cp kamailio kamailio_pcscf
$ cp kamailio kamailio_scscf
```

Changes required in /etc/default/kamailio_icscf

```
CFGFILE=/etc/kamailio_icscf/kamailio_icscf.cfg

RUN_KAMAILIO=yes
```

Changes required in /etc/default/kamailio_pcscf

```
CFGFILE=/etc/kamailio_pcscf/kamailio_pcscf.cfg

RUN_KAMAILIO=yes
```

Changes required in /etc/default/kamailio_scscf

```
CFGFILE=/etc/kamailio_scscf/kamailio_scscf.cfg

RUN_KAMAILIO=yes
```

Finally,

```
$ systemctl start kamailio_icscf kamailio_pcscf kamailio_scscf
```

#### 17. Ensure NextEPC PCRF of the Core Network is configured to use IMS

In nextepc.conf, add the floating IP of the VM running P-CSCF as shown below

```
pcscf:
  - 172.24.15.21
```

And, make sure to run NextEPC with Realm as created above "mnc096.mcc262.3gppnetwork.org" so that when a connection request from P-SCSF to PCRF with a FQDN pcrf.mnc096.mcc262.3gppnetwork.org it should be resolved as localhost of the NextEPC machine
{: .notice--warning}

- To change Realm in All-in-One NextEPC configuration

```
$ cd ~/nextepc/install/etc/nextepc/freediameter
```

- And, change all the configuration files in freediameter folder (hss.conf, mme.conf, pcrf.conf, pgw.conf) to have desired Domain Name
- Copy `make_certs.sh` from `~/nextepc/support/freeDiameter/` to `~/nextepc/install/etc/nextepc/freediameter`.
- Modify Domain Names in make_certs.sh and Generate the certificates using make_certs.sh
- Insert additional "ConnectPeer" entry in `~/nextepc/install/etc/nextepc/freediameter/pcrf.conf` to connect to P-CSCF as show below

```
ConnectPeer = "pcscf.mnc096.mcc262.3gppnetwork.org" { ConnectTo = "172.24.15.21"; Port=3871; No_TLS; NO_SCTP; };
```
