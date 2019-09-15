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
libssl-dev libcurl4-openssl-dev libxml2-dev libpcre3-dev bash-completion g++ autoconf rtpproxy libmnl-dev libsctp-dev ipsec-tools
```

#### 4. Install all required Kamailio packages (v5.2)

```
$ wget -O- http://deb.kamailio.org/kamailiodebkey.gpg | sudo apt-key add -
$ add-apt-repository 'deb http://deb.kamailio.org/kamailio52 bionic main'
$ apt install -y kamailio kamailio-mysql-modules kamailio-ims-modules kamailio-presence-modules kamailio-tls-modules kamailio-xml-modules kamailio-xmlrpc-modules
```

#### 5. Clone Kamailio repository and checkout 5.2 version of repository

```
$ mkdir -p /usr/local/src/kamailio-5.2
$ cd /usr/local/src/kamailio-5.2/
$ git clone git://git.kamailio.org/kamailio kamailio
$ cd kamailio
$ git checkout -b 5.2 origin/5.2
```

#### 6. Setup the DNS for resolving IMS components names

```
$ apt install -y bind9
```

Use the below example DNS Zone file to create a DNS Zone file in the /etc/bind folder and 
edit /etc/bind/named.conf.local, /etc/bind/named.conf.options accordingly:

```
$ cd /etc/bind
$ vim mnc096.mcc262.3gppnetwork.org
```

In the below example: Kamailio IMS & DNS server running at 10.4.128.7/172.24.15.21 (Floating IP) and PCRF at 10.4.128.11/172.24.15.3 (Floating IP)

```
$ORIGIN mnc096.mcc262.3gppnetwork.org.
$TTL 1W
@                       1D IN SOA       localhost. root.localhost. (
                                        1               ; serial
                                        3H              ; refresh
                                        15M             ; retry
                                        1W              ; expiry
                                        1D )            ; minimum

                        1D IN NS        ns
ns                     1D IN A         10.4.128.7

pcscf                   1D IN A         10.4.128.7
_sip._udp.pcscf         1D SRV 0 0 5060 pcscf
_sip._tcp.pcscf         1D SRV 0 0 5060 pcscf

icscf                   1D IN A         10.4.128.7
_sip._udp               1D SRV 0 0 4060 icscf
_sip._tcp               1D SRV 0 0 4060 icscf
_sip._udp.ims           1D SRV 0 0 4060 icscf
_sip._tcp.ims           1D SRV 0 0 4060 icscf

scscf                   1D IN A         10.4.128.7
_sip._udp.scscf         1D SRV 0 0 6060 scscf
_sip._tcp.scscf         1D SRV 0 0 6060 scscf

hss                     1D IN A         10.4.128.7
pcrf                     1D IN A         10.4.128.11
```

Edit /etc/bind/named.conf.local file as follows:

```
//
// Do any local configuration here
//

// Consider adding the 1918 zones here, if they are not used in your
// organization
//include "/etc/bind/zones.rfc1918";

zone "mnc096.mcc262.3gppnetwork.org" {
        type master;
        file "/etc/bind/mnc096.mcc262.3gppnetwork.org";
};
```

Edit /etc/bind/named.conf.options file as follows:

```
options {
        directory "/var/cache/bind";

        // If there is a firewall between you and nameservers you want
        // to talk to, you may need to fix the firewall to allow multiple
        // ports to talk.  See http://www.kb.cert.org/vuls/id/800113

        // If your ISP provided one or more IP addresses for stable
        // nameservers, you probably want to use them as forwarders.
        // Uncomment the following block, and insert the addresses replacing
        // the all-0's placeholder.

        forwarders {
          // Put here the IP address of other DNS server which could be used if name cannot be resolved with DNS server running in this machine
          10.4.128.2;
        };

        //========================================================================
        // If BIND logs error messages about the root key being expired,
        // you will need to update your keys.  See https://www.isc.org/bind-keys
        //========================================================================
        dnssec-validation no;
        allow-query { localhost; };

        auth-nxdomain no;    # conform to RFC1035
        //listen-on-v6 { any; };
};
```

```
$ systemctl restart bind9
```

Then, test DNS resolution by adding following entries on top of all other entries in /etc/resolv.conf.

search mnc096.mcc262.3gppnetwork.org
nameserver 10.4.128.7

Finally, ping to ensure

$ ping pcscf
PING pcscf.mnc096.mcc262.3gppnetwork.org (10.4.128.7) 56(84) bytes of data.
64 bytes from localhost (10.4.128.7): icmp_seq=1 ttl=64 time=0.017 ms
64 bytes from localhost (10.4.128.7): icmp_seq=2 ttl=64 time=0.041 ms

To make changes in /etc/resolv.conf be persistent across reboot edit the /etc/netplan/50-cloud-init.yaml file as follows:

```
# This file is generated from information provided by
# the datasource.  Changes to it will not persist across an instance.
# To disable cloud-init's network configuration capabilities, write a file
# /etc/cloud/cloud.cfg.d/99-disable-network-config.cfg with the following:
# network: {config: disabled}
network:
    version: 2
    ethernets:
        ens3:
            dhcp4: true
            match:
                macaddress: fa:16:3e:99:f5:67
            set-name: ens3
            nameservers:
                search: [mnc096.mcc262.3gppnetwork.org]
                addresses:
                      - 10.4.128.7
```

```
$ netplan apply
$ ln -sf /run/systemd/resolve/resolv.conf /etc/resolv.conf
$ systemctl restart systemd-resolved.service
```

#### 7. Populate MySQL database using kamctlrc command

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

#### 8. Edit /etc/default/rtpproxy file as follows:

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

#### 9. Edit configuration file to fit your requirements for the VoIP platform, you have to edit the /etc/kamailio/kamailio.cfg configuration file

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

#### 10. Run Kamailio SIP server

```
$ systemctl start kamailio
```

#### 11. A quick check for the basic working of SIP server can be done as follows:

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

#### 12. Create new mysql database for pcscf, scscf and icscf, populate databases and grant permissions to respective users identified by a password

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
<mysql> GRANT ALL PRIVILEGES ON pcscf.* TO 'pcscf'@'%' identified by 'heslo';
<mysql> GRANT ALL PRIVILEGES ON scscf.* TO 'scscf'@'%' identified by 'heslo';
<mysql> GRANT ALL PRIVILEGES ON icscf.* TO 'icscf'@'%' identified by 'heslo';
<mysql> GRANT ALL PRIVILEGES ON icscf.* TO 'provisioning'@'%' identified by 'provi';
<mysql> FLUSH PRIVILEGES;
```

Then,

```
$ mysql
<mysql> use icscf;
<mysql> INSERT INTO `nds_trusted_domains` VALUES (1,'mnc096.mcc262.3gppnetwork.org');
<mysql> INSERT INTO `s_cscf` VALUES (1,'First and only S-CSCF','sip:scscf.mnc096.mcc262.3gppnetwork.org:6060');
<mysql> INSERT INTO `s_cscf_capabilities` VALUES (1,1,0),(2,1,1);
```

#### 13. Copy pcscf, icscf and scscf configuration files to /etc/ folder and edit accordingly

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

Edit the DNS domain names, DB URL and IP addresses at all places in the icscf.cfg, icscf.xml files accordingly

Changes required in icscf.cfg:

```
# SIP / UDP
listen=udp:10.4.128.7:4060
# SIP / TCP
listen=tcp:10.4.128.7:4060

alias=icscf.mnc096.mcc262.3gppnetwork.org

#!define NETWORKNAME "mnc096.mcc262.3gppnetwork.org" 
#!define HOSTNAME "icscf.mnc096.mcc262.3gppnetwork.org"

#!define ENUM_SUFFIX "mnc096.mcc262.3gppnetwork.org."

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

#loadmodule "debugger.so"

# if !($rU =~ "\+.*") {
#   prefix("+");
# }
```

To perform a quick test edit the /etc/default/kamailio file, by changing the configuration file parameter as follows:

```
# Config file
CFGFILE=/etc/kamailio_icscf/kamailio_icscf.cfg
```
```
$ $ mkdir -p /var/run/kamailio_icscf
$ adduser --quiet --system --group --disabled-password \
        --shell /bin/false --gecos "Kamailio" \
        --home /var/run/kamailio_icscf kamailio
$ chown kamailio:kamailio /var/run/kamailio_icscf
```

Then, execute below command

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
#!define WITH_TCP
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

#loadmodule "debugger.so"

/* set the path to RPC fifo control file */
modparam("jsonrpcs", "fifo_name", "/var/run/kamailio_pcscf/kamailio_rpc.fifo")
/* set the path to RPC unix socket control file */
modparam("jsonrpcs", "dgram_socket", "/var/run/kamailio_pcscf/kamailio_rpc.sock")

modparam("tls", "config", "/etc/kamailio_pcscf/tls.cfg")

# ----- ctl params -----
modparam("ctl", "binrpc", "unix:/var/run/kamailio_pcscf/kamailio_ctl")

# AVP's required for Fail-Over-Support:
#modparam("dispatcher", "dst_avp", "$avp(DISPATCHER_DST_AVP)")
#modparam("dispatcher", "grp_avp", "$avp(DISPATCHER_GRP_AVP)")
#modparam("dispatcher", "cnt_avp", "$avp(DISPATCHER_CNT_AVP)")
#modparam("dispatcher", "sock_avp", "$avp(DISPATCHER_SOCK_AVP)")

#modparam("ims_usrloc_pcscf", "hashing_type", 2)

#!ifdef WITH_RX
# -- CDP params --
modparam("cdp","config_file","/etc/kamailio_pcscf/pcscf.xml")
# -- diameter_rx params --
modparam("ims_qos", "rx_dest_realm", "localdomain") # Enter realm to which PCRF belongs to
#modparam("ims_qos", "rx_dest_realm", "NETWORKNAME")

#!ifdef WITH_IPSEC
  if (!is_method("REGISTER")) {
        ipsec_forward("location");
  }
#!endif

#!ifdef WITH_IPSEC
      ipsec_destroy("location");
#!endif
```

Changes required in dispatcher.list:

```
# SBC's
```

To perform a quick test edit the /etc/default/kamailio file, by changing the configuration file parameter as follows:

```
# Config file
CFGFILE=/etc/kamailio_pcscf/kamailio_pcscf.cfg
```

```
$ mkdir -p /var/run/kamailio_pcscf
$ adduser --quiet --system --group --disabled-password \
        --shell /bin/false --gecos "Kamailio" \
        --home /var/run/kamailio_pcscf kamailio
$ chown kamailio:kamailio /var/run/kamailio_pcscf
```

Then, execute below command

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
# SIP / TCP
listen=tcp:10.4.128.7:6060

#!define NETWORKNAME "mnc096.mcc262.3gppnetwork.org"
#!define HOSTNAME "scscf.mnc096.mcc262.3gppnetwork.org"
#!define URI "sip:scscf.mnc096.mcc262.3gppnetwork.org:6060"

alias=scscf.mnc096.mcc262.3gppnetwork.org

# ENUM-Server to query:
#!define ENUM_SUFFIX "mnc096.mcc262.3gppnetwork.org."

#!define DB_URL "mysql://scscf:heslo@localhost/scscf"

#!define RO_MNC "96"

#!define WITH_TCP
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

#loadmodule "debugger.so"

/* set the path to RPC fifo control file */
modparam("jsonrpcs", "fifo_name", "/var/run/kamailio_scscf/kamailio_rpc.fifo")
/* set the path to RPC unix socket control file */
modparam("jsonrpcs", "dgram_socket", "/var/run/kamailio_scscf/kamailio_rpc.sock")

# ----- ctl params -----
modparam("ctl", "binrpc", "unix:/var/run/kamailio_scscf/kamailio_ctl")

# -- CDP params --
modparam("cdp","config_file","/etc/kamailio_scscf/scscf.xml")

# AVP's required for Fail-Over-Support:
#modparam("dispatcher", "dst_avp", "$avp(DISPATCHER_DST_AVP)")
#modparam("dispatcher", "grp_avp", "$avp(DISPATCHER_GRP_AVP)")
#modparam("dispatcher", "cnt_avp", "$avp(DISPATCHER_CNT_AVP)")
#modparam("dispatcher", "sock_avp", "$avp(DISPATCHER_SOCK_AVP)")

    # Terminating
    if (uri == myself || uri =~ "tel:.*") {
      if (!term_impu_registered("location")) {
        xlog("L_DBG", "We need to do an UNREG server SAR assignemnt");
                          assign_server_unreg("UNREG_SAR_REPLY", "location", "term");
        sl_send_reply("403","Forbidden - Domain not served");
                          exit;
            }
    }
#                } else {
#     sl_send_reply("403","Forbidden - Domain not served");
#     exit();
#   }
```

Changes required in dispatcher.list: Not sure what to do hence remove as follows

```
# ng-voice Interconnect
```

To perform a quick test edit the /etc/default/kamailio file, by changing the configuration file parameter as follows:

```
# Config file
CFGFILE=/etc/kamailio_scscf/kamailio_scscf.cfg
```

```
$ mkdir -p /var/run/kamailio_scscf
$ adduser --quiet --system --group --disabled-password \
        --shell /bin/false --gecos "Kamailio" \
        --home /var/run/kamailio_scscf kamailio
$ chown kamailio:kamailio /var/run/kamailio_scscf
```

Then, execute below command

```
$ systemctl restart kamailio.service
```

And, check that there are no error by viewing logs using the below command

```
$ journalctl -f --unit kamailio
```

Can run S-CSCF as follows: `kamailio -f kamailio_scscf.cfg`


#### 14. Install RTPEngine

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

Edit /etc/default/ngcp-rtpengine-daemon and /etc/default/ngcp-rtpengine-recording-daemon as follows in respective files:
```
RUN_RTPENGINE=yes
```
```
RUN_RTPENGINE_RECORDING=yes
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

Second instance of RTPENGINE can be run as follows:

```
$ iptables -I rtpengine -p udp -j RTPENGINE --id 1
$ ip6tables -I INPUT -p udp -j RTPENGINE --id 1
$ echo 'del 1' > /proc/rtpengine/control
$ /usr/sbin/rtpengine --table=1 --interface=10.4.128.7!172.24.15.21 --listen-ng=127.0.0.1:2224 --tos=184 --pidfile=ngcp-rtpengine-daemon2.pid --no-fallback
```

#### 15. Running I-CSCF, P-CSCF and S-CSCF as separate systemctl process

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

#### 16. Setup FoHSS in order to talk with I-CSCF and S-CSCF

Requirements for FoHSS: Install Java JDK and ant

Download Oracle Java 7 JDK from following link using a browser:
https://www.oracle.com/technetwork/java/javase/downloads/java-archive-downloads-javase7-521261.html

```
$ mkdir -p  /usr/lib/jvm/
$ tar -zxf jdk-7u79-linux-x64.tar.gz -C /usr/lib/jvm/
$ update-alternatives --install /usr/bin/java java /usr/lib/jvm/jdk1.7.0_79/bin/java 100
$ update-alternatives --install /usr/bin/javac javac /usr/lib/jvm/jdk1.7.0_79/bin/javac 100
```

Verify that java has been successfully configured by running:

```
$ update-alternatives --display java
java - auto mode
  link best version is /usr/lib/jvm/jdk1.7.0_79/bin/java
  link currently points to /usr/lib/jvm/jdk1.7.0_79/bin/java
  link java is /usr/bin/java
/usr/lib/jvm/jdk1.7.0_79/bin/java - priority 100

$ update-alternatives --display javac
javac - auto mode
  link best version is /usr/lib/jvm/jdk1.7.0_79/bin/javac
  link currently points to /usr/lib/jvm/jdk1.7.0_79/bin/javac
  link javac is /usr/bin/javac
/usr/lib/jvm/jdk1.7.0_79/bin/javac - priority 100

$ update-alternatives --config java
(select java jdk1.7.0_79)
$ update-alternatives --config javac
```

Check java version

```
$ java -version
```

Install Ant

```
$ cd ~
$ wget http://archive.apache.org/dist/ant/binaries/apache-ant-1.9.14-bin.tar.gz
$ tar xvfvz apache-ant-1.9.14-bin.tar.gz
$ mv apache-ant-1.9.14 /usr/local/
$ sh -c 'echo ANT_HOME=/usr/local/  >> /etc/environment'
$ ln -s /usr/local/apache-ant-1.9.14/bin/ant /usr/bin/ant
```

Verfiy ant version as follows:

```
$ ant -version
Apache Ant(TM) version 1.9.14 compiled on March 12 2019
```

Create working directories for OpenIMSCore:

```
$ mkdir /opt/OpenIMSCore
$ cd /opt/OpenIMSCore
```
  
Download:

```
$ svn checkout svn://svn.code.sf.net/p/openimscore/code/FHoSS/trunk
$ mv trunk FHoSS
```

Compile:

```
$ cd FHoSS
$ export JAVA_HOME="/usr/lib/jvm/jdk1.7.0_79"
$ export CLASSPATH="/usr/lib/jvm/jdk1.7.0_79/jre/lib/"
$ ant compile deploy | tee ant_compile_deploy.txt
```

Create configurator.sh using below script to change domain names and IP address in all configuration files

```
$ cd deploy
$ vim configurator.sh
```

```
#!/bin/bash

# Initialization & global vars
# if you execute this script for the second time
# you should change these variables to the latest
# domain name and ip address
DDOMAIN="open-ims\.test"
DSDOMAIN="open-ims\\\.test"
DEFAULTIP="127\.0\.0\.1"
CONFFILES=`ls *.cfg *.xml *.sql *.properties 2>/dev/null`

# Interaction
printf "Domain Name:"
read domainname 
printf "IP Adress:"
read ip_address

# input domain is to be slashed for cfg regexes 
slasheddomain=`echo $domainname | sed 's/\./\\\\\\\\\./g'`

  if [ $# != 0 ] 
  then 
  printf "changing: "
      for j in $* 
      do
    sed -i -e "s/$DDOMAIN/$domainname/g" $j
    sed -i -e "s/$DSDOMAIN/$slasheddomain/g" $j
    sed -i -e "s/$DEFAULTIP/$ip_address/g" $j
    printf "$j " 
      done
  echo 
  else 
  printf "File to change [\"all\" for everything, \"exit\" to quit]:"
  # loop
      while read filename ;
      do
        if [ "$filename" = "exit" ] 
        then 
        printf "exitting...\n"
        break ;

      elif [ "$filename" = "all" ]
      then    
          printf "changing: "
         for i in $CONFFILES 
         do
        sed -i -e "s/$DDOMAIN/$domainname/g" $i
        sed -i -e "s/$DSDOMAIN/$slasheddomain/g" $i
        sed -i -e "s/$DEFAULTIP/$ip_address/g" $i
        
        printf "$i " 
         done 
         echo 
         break;

        elif [ -w $filename ] 
        then
            printf "changing $filename \n"
            sed -i -e "s/$DDOMAIN/$domainname/g" $filename
            sed -i -e "s/$DSDOMAIN/$slasheddomain/g" $filename
            sed -i -e "s/$DEFAULTIP/$ip_address/g" $filename

          else 
          printf "cannot access file $filename. skipping... \n" 
        fi
        printf "File to Change:"
      done 
  fi
```

```
$ chmod +x configurator.sh
$ ./configurator.sh 
Domain Name:mnc096.mcc262.3gppnetwork.org
IP Adress:10.4.128.7

$ grep -r "open-ims"
$ vim webapps/hss.web.console/WEB-INF/web.xml
$ vim hibernate.properties
```
And, change the following line

hibernate.connection.url=jdbc:mysql://127.0.0.1:3306/hss_db

```
$ cp configurator.sh ../scripts/
$ cd ../scripts
$ grep -r "open-ims"
$ ./configurator.sh 
Domain Name:mnc096.mcc262.3gppnetwork.org
IP Adress:10.4.128.7

$ cp configurator.sh ../config/
$ cd ../config
$ ./configurator.sh 
Domain Name:mnc096.mcc262.3gppnetwork.org
IP Adress:10.4.128.7

$ cd ../src-web
$ vim WEB-INF/web.xml
```
And, change open-ims.test to mnc096.mcc262.3gppnetwork.org

Prepare mysql database:

```
$ mysql
<mysql> drop database hss_db;
<mysql> create database hss_db;
<mysql> quit
```

Import database located at /opt/OpenIMSCore into hss_db 
```
$ cd /opt/OpenIMSCore
$ mysql -u root -p hss_db < FHoSS/scripts/hss_db.sql
$ mysql -u root -p hss_db < FHoSS/scripts/userdata.sql
```

Check grants for mysql access rights at first time installation:

```
$ mysql
# See last line in hss_db.sql:
<mysql> grant delete,insert,select,update on hss_db.* to hss@localhost identified by 'hss';
<mysql> grant delete,insert,select,update on hss_db.* to hss@'%' identified by 'hss';
```

Check database if domain names are  o.k. in various entries and privileges
```
$ mysql -u hss -p
<mysql> show databases;
<mysql> use hss_db;
<mysql> select * from impu;
```

Prepare script-file, start HSS

Copy startup.sh to hss.sh in root directory

```
$ cp /opt/OpenIMSCore/FHoSS/deploy/startup.sh /root/hss.sh
```

And, add the following to hss.sh before echo "Building Classpath"

```
cd /opt/OpenIMSCore/FHoSS/deploy
JAVA_HOME="/usr/lib/jvm/jdk1.7.0_79"
CLASSPATH="/usr/lib/jvm/jdk1.7.0_79/jre/lib/"
```

Start HSS using hss.sh

```
$ ./hss.sh
```

Access the web-interface of HSS: http://<IMS_VM_FLOATING_IP>:8080/hss.web.console/

http://172.24.15.21:8080/hss.web.console/

```
user:      hssAdmin
password:  hss
```

#### 17. A quick check for the basic working of SIP IMS server can be done as follows

The steps are exactly the same as in Step 11, but now the usernames and passwords are alice and bob, which are the default users present in FoHSS.

#### 18. Ensure NextEPC PCRF of the Core Network is configured to use IMS

In nextepc.conf, add the floating IP of the VM running P-CSCF as shown below

```
parameter:
    no_ipv6: true
    prefer_ipv4: true

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
