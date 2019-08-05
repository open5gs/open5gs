---
title: Setting up Kamailio IMS 
head_inline: "<style> .blue { color: blue; } </style>"
---

Setting up Kamailio IMS in OpenStack VM and connecting P-CSCF of Kamailio IMS with PCRF of NextEPC (Running on another OpenStak VM) - Install from source
{: .blue}

#### 1. Start from Bionic Ubuntu cloud image
#### 2. Use the following Cloud init data while spawning an instance

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
$ apt install -y tcpdump screen ntp ntpdate bind9 mysql-server git-core gcc flex bison libmysqlclient-dev make libssl-dev libcurl4-openssl-dev libxml2-dev libpcre3-dev bash-completion g++ autoconf rtpproxy libmnl-dev libsctp-dev ipsec-tools
```

#### 4. Execute,

```
$ ntpdate pool.ntp.org
```

#### 5. Clone Kamailio repository and checkout 5.2 version of repository,

```
$ mkdir -p /usr/local/src/kamailio-5.2
$ cd /usr/local/src/kamailio-5.2/
$ git clone git://git.kamailio.org/kamailio kamailio
$ cd kamailio
$ git checkout -b 5.2 origin/5.2
```

#### 6. Copy the example DNS Zone file from /usr/local/src/kamailio-5.2/kamailio/misc/examples/ims/ims_dnszone into the bind folder and 

Edit /etc/bind/named.conf.local, /etc/bind/named.conf.options accordingly:
{: .notice--info}

```
$ cp /usr/local/src/kamailio-5.2/kamailio/misc/examples/ims/ims_dnszone/mnc001.mcc001.3gppnetwork.org /etc/bind/
$ cd /etc/bind
```

Rename as per your need

```
$ mv mnc001.mcc001.3gppnetwork.org mnc096.mcc262.3gppnetwork.org
```

In this example: PCRF is running in 10.4.128.11/172.24.15.3 (Floating IP) machine and IMS & DNS server running at 10.4.128.3/172.24.15.30 (Floating IP)

Edit /etc/bind/mnc096.mcc262.3gppnetwork.org file as follows:

Notice the change in PCRF address resolution (It should be the machine IP where PCRF is running)
{: .notice--warning}

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
ns                      1D IN A         127.0.0.1

pcscf                   1D IN A         127.0.0.1
_sip._udp.pcscf         1D SRV 0 0 5060 pcscf
_sip._tcp.pcscf         1D SRV 0 0 5060 pcscf

icscf                   1D IN A         127.0.0.1
_sip._udp               1D SRV 0 0 4060 icscf
_sip._tcp               1D SRV 0 0 4060 icscf
_sip._udp.ims           1D SRV 0 0 4060 icscf
_sip._tcp.ims           1D SRV 0 0 4060 icscf

scscf                   1D IN A         127.0.0.1
_sip._udp.scscf         1D SRV 0 0 6060 scscf
_sip._tcp.scscf         1D SRV 0 0 6060 scscf

hss                     1D IN A         127.0.0.1
pcrf                     1D IN A         10.4.128.11
```

Edit /etc/bind/named.conf.local file as follows:
{: .notice--info}

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

Edit /etc/bind/named.conf.options file as follows:

options {
        directory "/var/cache/bind";

        // If there is a firewall between you and nameservers you want
        // to talk to, you may need to fix the firewall to allow multiple
        // ports to talk.  See http://www.kb.cert.org/vuls/id/800113

        // If your ISP provided one or more IP addresses for stable
        // nameservers, you probably want to use them as forwarders.
        // Uncomment the following block, and insert the addresses replacing
        // the all-0's placeholder.

        // forwarders {
	// Put here the IP address of other DNS server which could be used if name cannot be resolved with DNS server running in this machine
        //      0.0.0.0; 
        // };

        //========================================================================
        // If BIND logs error messages about the root key being expired,
        // you will need to update your keys.  See https://www.isc.org/bind-keys
        //========================================================================
        //dnssec-validation auto;
        allow-query { any; };

        auth-nxdomain no;    # conform to RFC1035
        //listen-on-v6 { any; };
};
```

```
$ systemctl restart bind9
```

Then, test DNS resolution by adding following entries on top of all other entries in /etc/resolv.conf

```
search mnc096.mcc262.3gppnetwork.org
nameserver 10.4.128.3
```

Finally, ping domain names

```
$ ping pcrf
PING pcrf.mnc096.mcc262.3gppnetwork.org (10.4.128.11) 56(84) bytes of data.
64 bytes from 10.4.128.11 (10.4.128.11): icmp_seq=1 ttl=64 time=0.425 ms
64 bytes from 10.4.128.11 (10.4.128.11): icmp_seq=2 ttl=64 time=0.291 ms
64 bytes from 10.4.128.11 (10.4.128.11): icmp_seq=3 ttl=64 time=0.260 ms

$ ping pcscf
PING pcscf.mnc096.mcc262.3gppnetwork.org (127.0.0.1) 56(84) bytes of data.
64 bytes from localhost (127.0.0.1): icmp_seq=1 ttl=64 time=0.013 ms
64 bytes from localhost (127.0.0.1): icmp_seq=2 ttl=64 time=0.045 ms
64 bytes from localhost (127.0.0.1): icmp_seq=3 ttl=64 time=0.030 ms
```

Notice that domain names are properly resolved to IP address i.e 10.4.128.11 for pcrf and localhost for pcscf
{: .notice--warning}

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
                addresses: [10.4.128.3]
```

#### 7. Next step in installing SIP server is to generate build config files.

```
$ cd /usr/local/src/kamailio-5.2/kamailio
$ make cfg
```

#### 8. Then enable the MySQL module and all required IMS modules. Edit modules.lst file present at /usr/local/src/kamailio-5.2/kamailio/src

The contents of modules.lst should be as follows:

```
# this file is autogenerated by make modules-cfg

# the list of sub-directories with modules
modules_dirs:=modules

# the list of module groups to compile
cfg_group_include=

# the list of extra modules to compile
include_modules= cdp cdp_avp db_mysql ims_auth ims_charging ims_dialog ims_diameter_server ims_icscf ims_ipsec_pcscf ims_isc ims_ocs ims_qos ims_registrar_pcscf ims_registrar_scscf ims_usrloc_pcscf ims_usrloc_scscf presence presence_conference presence_dialoginfo presence_mwi presence_profile presence_reginfo presence_xml pua pua_bla pua_dialoginfo pua_reginfo pua_rpc pua_usrloc pua_xmpp regex sctp tls xmlops xmlrpc

# the list of static modules
static_modules=

# the list of modules to skip from compile list
skip_modules=

# the list of modules to exclude from compile list
exclude_modules= acc_json acc_radius app_java app_lua app_mono app_perl app_python app_python3 app_ruby auth_ephemeral auth_identity auth_radius cnxcc cplc crypto db2_ldap db_berkeley db_cassandra db_mongodb db_oracle db_perlvdb db_postgres db_redis db_sqlite db_unixodbc dialplan dnssec erlang evapi geoip geoip2 gzcompress h350 http_async_client http_client jansson janssonrpcc json jsonrpcc kazoo lcr ldap log_systemd memcached misc_radius ndb_cassandra ndb_mongodb ndb_redis nsq osp outbound peering phonenum pua_json rabbitmq rls snmpstats topos_redis utils uuid websocket xcap_client xcap_server xhttp_pi xmpp $(skip_modules)

modules_all= $(filter-out modules/CVS,$(wildcard modules/*))
modules_noinc= $(filter-out $(addprefix modules/, $(exclude_modules) $(static_modules)), $(modules_all)) 
modules= $(filter-out $(modules_noinc), $(addprefix modules/, $(include_modules) )) $(modules_noinc) 
modules_configured:=1
```

#### 9. When the compilation is ready, install Kamailio with the following command:

```
$ cd /usr/local/src/kamailio-5.2/kamailio
$ make Q=0 all | tee make_all.txt
$ make install | tee make_install.txt
$ ldconfig
```

The binaries and executable scripts were installed in: /usr/local/sbin

```
kamailio - Kamailio SIP server
kamdbctl - script to create and manage the Databases
kamctl - script to manage and control Kamailio SIP server
kamcmd - CLI - command line tool to interface with Kamailio SIP server
```

To be able to use the binaries from command line, make sure that '/usr/local/sbin' is set in PATH environment variable. 
You can check that with 'echo $PATH'. If not and you are using 'bash', open '/root/.bash_profile' and at the end add:

```
  PATH=$PATH:/usr/local/sbin
  export PATH
```

Kamailio modules are installed in:
```
/usr/local/lib64/kamailio/modules
```

The documentation and readme files are installed in:
```
/usr/local/share/doc/kamailio/
```

The configuration file was installed in:
```
/usr/local/etc/kamailio
```

In case you set the PREFIX variable in 'make cfg' command, then replace /usr/local in all paths above with the value of PREFIX 
in order to locate the files installed.

#### 10. Populate MySQL database using kamctlrc command

Before executing the command edit kamctlrc file to set the database server and sip domain type at /usr/local/etc/kamailio/

Locate DBENGINE and SIP_DOMAIN variable and set it to MYSQL and IP address of SIP server and uncomment the line:

```
SIP_DOMAIN=mnc096.mcc262.3gppnetwork.org (Created DNS Domain Name or IP to which IMS components are bound to, visible interface IP address)
#SIP_DOMAIN=10.4.128.3
DBENGINE=MYSQL
```

You can change other values in kamctlrc file. Once you are done updating kamctlrc file, run the script to create the database used by Kamailio:

```
$ kamdbctl create
```

(When prompted for mysql root user password, leave it blank i.e. Press Enter)

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

#### 11. Edit /etc/default/rtpproxy file as follows:

```
# Defaults for rtpproxy

# The control socket.
#CONTROL_SOCK="unix:/var/run/rtpproxy/rtpproxy.sock"
# To listen on an UDP socket, uncomment this line:
#CONTROL_SOCK=udp:127.0.0.1:22222
CONTROL_SOCK=udp:127.0.0.1:7722

# Additional options that are passed to the daemon.
EXTRA_OPTS="-l 172.24.15.30 -d DBUG:LOG_LOCAL0"
```

here, `-l <PUBLIC_IP>`

Then run,

```
$ systemctl restart rtpproxy
```

#### 12. Edit configuration file to fit your requirements for the VoIP platform, you have to edit the /usr/local/etc/kamailio/kamailio.cfg configuration file. 

Follow the instruction in the comments to enable usage of MySQL. Basically you have to add several lines at the top of config file, like:

```
#!define WITH_MYSQL
#!define WITH_AUTH
#!define WITH_USRLOCDB
#!define WITH_NAT (Include this if client and/or SIP server is behind a NAT)

(uncomment this line and enter the DNS domain created above)
alias="mnc096.mcc262.3gppnetwork.org"

(uncomment this line, 10.4.128.3 is the internal IP and 172.24.15.30 is the Public/Floating IP)
listen=udp:10.4.128.3:5060 advertise 172.24.15.30:5060
```

Further down, we will need to modify the rtpproxy_sock value to match the CONTROL_SOCK option we set for RTPProxy in /etc/default/rtpproxy
```
modparam("rtpproxy", "rtpproxy_sock", "udp:127.0.0.1:7722")
```

If you changed the password for the 'kamailio' user of MySQL, you have to update the value for 'DBURL' parameters.

#### 13. The init.d script

The init.d script can be used to start/stop the Kamailio server in a nicer way. A sample of init.d script for Kamailio is provided at:
/usr/local/src/kamailio-5.2/kamailio/pkg/kamailio/deb/debian/kamailio.init

Just copy the init file into the /etc/init.d/kamailio. Then change the permissions:

```
$ cp /usr/local/src/kamailio-5.2/kamailio/pkg/kamailio/deb/bionic/kamailio.init /etc/init.d/kamailio
$ chmod 755 /etc/init.d/kamailio
```

then edit the file updating the $DAEMON and $CFGFILE values:

```
PATH=/sbin:/bin:/usr/sbin:/usr/bin:/usr/local/sbin
DAEMON=/usr/local/sbin/kamailio
CFGFILE=/usr/local/etc/kamailio/kamailio.cfg
```

You need to setup a configuration file in the /etc/default/ directory. This file can be found at:
```
/usr/local/src/kamailio-5.2/kamailio/pkg/kamailio/deb/bionic/kamailio.default
```

You need to rename the file to 'kamailio' after you've copied it. Then edit this file and set RUN_KAMAILIO=yes. Edit the other options at your convenience.

```
$ cp /usr/local/src/kamailio-5.2/kamailio/pkg/kamailio/deb/bionic/kamailio.default /etc/default/kamailio
```

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

Set ownership

```
$ chown kamailio:kamailio /var/run/kamailio
$ chown kamailio:kamailio /var/run/kamailio_icscf
$ chown kamailio:kamailio /var/run/kamailio_pcscf
$ chown kamailio:kamailio /var/run/kamailio_scscf
```

Then you can start/stop Kamailio using the following commands:

```
$ systemctl start kamailio.service
$ systemctl stop kamailio.service
```

OR

```
$ /etc/init.d/kamailio start
$ /etc/init.d/kamailio stop
```

check running processes with: ps axw | egrep kamailio

#### 14. A quick check for the basic working of SIP server can be done as follows:

Start the kamailio sip server

```
$ systemctl start kamailio.service
```

Create new subscriber accounts. A new account can be added using `kamctl` tool via `kamctl add <username> <password>`

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
	Outbound proxy address: 172.24.15.30 (Floating IP of VM in case of OpenStack or else no need to fill in case of physical machine)
	Transport type: UDP
```

In Phone 2:

```
Username: test2
Password: testpasswd
Server: mnc096.mcc262.3gppnetwork.org (Created DNS Domain Name or IP to which IMS components are bound to, visible interface IP address)
Optional Settings:
	Authentication username: test2
	Outbound proxy address: 172.24.15.30 (Floating IP of VM in case of OpenStack or else no need to fill in case of physical machine)
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

#### 15. Create new mysql database for pcscf, scscf and icscf, populate databases and grant permissions to respective users identified by a password

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

<mysql> grant delete,insert,select,update on pcscf.* to pcscf@localhost identified by 'heslo';
<mysql> grant delete,insert,select,update on scscf.* to scscf@localhost identified by 'heslo';
<mysl> grant delete,insert,select,update on icscf.* to icscf@localhost identified by 'heslo';
<mysl> grant delete,insert,select,update on icscf.* to provisioning@localhost identified by 'provi';
```

#### 16. Copy pcscf, icscf and scscf configuration files to /etc/ folder and edit accordingly

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

Change the DB_URL in icscf.cfg as follows:
```
#!define DB_URL "mysql://icscf:heslo@localhost/icscf"
```

Edit kamailio_icscf.cfg modules path i.e. mpath variable. Add the following location to mpath: `/usr/local/lib64/kamailio/modules/`
```
# ------------------ module loading ----------------------------------
mpath="/usr/lib64/kamailio/modules_k/:/usr/lib64/kamailio/modules/:/usr/lib/kamailio/modules_k/:/usr/lib/kamailio/modules/:/usr/local/lib64/kamailio/modules/"
```

Can run I-CSCF as follows: kamailio -f kamailio_icscf.cfg

### P-CSCF 

Edit the DNS domain names, DB URL and IP addresses at all places in the pcscf.cfg, pcscf.xml files accordingly

Change the DB_URL in pcscf.cfg as follows:

```
#!define DB_URL "mysql://pcscf:heslo@localhost/pcscf"
Uncomment IPsec flag in pcscf.cfg as follows:
#!define WITH_IPSEC
Change the following line as follows
#!define SQLOPS_DBURL "pcscf=>mysql://pcscf:heslo@localhost/pcscf"
Uncomment RX interface flag in pcscf.cfg as follows:
#!define WITH_RX
```

Edit kamailio_pcscf.cfg modules path i.e. mpath variable. Add the following location to mpath: `/usr/local/lib64/kamailio/modules/`
```
# ------------------ module loading ----------------------------------
mpath="/usr/lib64/kamailio/modules_k/:/usr/lib64/kamailio/modules/:/usr/lib/kamailio/modules_k/:/usr/lib/kamailio/modules/:/usr/local/lib64/kamailio/modules/"
Comment out certain dispatcher modparam parts in kamailio_pcscf.cfg as it does not apply for v5.2 of Kamailio:
# ----------------- Settings for Dispatcher ---------------
modparam("dispatcher", "list_file", "/etc/kamailio_pcscf/dispatcher.list")

# Dispatcher: Enable Failover-Support
modparam("dispatcher", "flags", 2)
# Dispatcher: Overwrite Destination address, if required.
modparam("dispatcher", "force_dst", 1)
# AVP's required for Fail-Over-Support:
#modparam("dispatcher", "dst_avp", "$avp(DISPATCHER_DST_AVP)")
#modparam("dispatcher", "grp_avp", "$avp(DISPATCHER_GRP_AVP)")
#modparam("dispatcher", "cnt_avp", "$avp(DISPATCHER_CNT_AVP)")
#modparam("dispatcher", "sock_avp", "$avp(DISPATCHER_SOCK_AVP)")

# Try to recover disabled destinations every 15 seconds.
modparam("dispatcher", "ds_ping_interval", 15)
# Actively query the gateways:
modparam("dispatcher", "ds_probing_mode", 1)

Comment out hashing feature for usrloc params in kamailio_pcscf.cfg as it does not apply for v5.2 of Kamailio:
#modparam("ims_usrloc_pcscf", "hashing_type", 2)
```

Dispatcher List file - Not sure what to do

Can run P-CSCF as follows: `kamailio -f kamailio_pcscf.cfg`

### S-CSCF

Edit the DNS domain names, DB URL and IP addresses at all places in the scscf.cfg, scscf.xml files accordingly

Change the DB_URL in scscf.cfg as follows:

```
#!define DB_URL "mysql://scscf:heslo@localhost/scscf"
```

Edit kamailio_scscf.cfg modules path i.e. mpath variable. Add the following location to mpath: `/usr/local/lib64/kamailio/modules/`

```
# ------------------ module loading ----------------------------------
mpath="/usr/lib64/kamailio/modules_k/:/usr/lib64/kamailio/modules/:/usr/lib/kamailio/modules_k/:/usr/lib/kamailio/modules/:/usr/local/lib64/kamailio/modules/"
Comment out certain dispatcher modparam parts in kamailio_scscf.cfg as it does not apply for v5.2 of Kamailio:
# ----------------- Settings for Dispatcher ---------------
modparam("dispatcher", "list_file", "/etc/kamailio_scscf/dispatcher.list")
# Dispatcher: Enable Failover-Support
modparam("dispatcher", "flags", 2)
# Dispatcher: Overwrite Destination address, if required.
modparam("dispatcher", "force_dst", 1)
# AVP's required for Fail-Over-Support:
#modparam("dispatcher", "dst_avp", "$avp(DISPATCHER_DST_AVP)")
#modparam("dispatcher", "grp_avp", "$avp(DISPATCHER_GRP_AVP)")
#modparam("dispatcher", "cnt_avp", "$avp(DISPATCHER_CNT_AVP)")
#modparam("dispatcher", "sock_avp", "$avp(DISPATCHER_SOCK_AVP)")

# Try to recover disabled destinations every 15 seconds.
modparam("dispatcher", "ds_ping_interval", 15)
# Actively query the gateways:
modparam("dispatcher", "ds_probing_mode", 1)
modparam("dispatcher", "ds_ping_reply_codes", "class=2;code=404;code=480")
```

Dispatcher List file - Not sure what to do

Can run S-CSCF as follows: `kamailio -f kamailio_scscf.cfg`

OR

edit the /etc/default/kamailio file, by changing the configuration file parameter as follows:

```
# Config file
#CFGFILE=/etc/kamailio_icscf/kamailio_icscf.cfg
#CFGFILE=/etc/kamailio_scscf/kamailio_scscf.cfg
CFGFILE=/etc/kamailio_pcscf/kamailio_pcscf.cfg
```

(This example runs PCSCF)

After altering the above file, execute below command

```
$ systemctl restart kamailio.service
```

#### 17. Install RTPEngine

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
interface = 10.4.128.3!172.24.15.30
listen-ng = 127.0.0.1:12222
```

Port on which rtpengine binds i.e. listen_ng parameter is udp port 12222. This should be updated in kamailio_pcscf.cfg file at modparam(rtpengine ...

```
# ----- rtpproxy params -----
modparam("rtpengine", "rtpengine_sock", "1 == udp:localhost:12222")
#modparam("rtpengine", "rtpengine_sock", "2 == udp:localhost:9911")
#modparam("rtpengine", "setid_avp", "$avp(setid)")
modparam("rtpengine", "setid_default", 1)
modparam("rtpengine", "extra_id_pv", "$avp(extra_id)")

$ cp /etc/rtpengine/rtpengine-recording.sample.conf /etc/rtpengine/rtpengine-recording.conf
$ mkdir /var/spool/rtpengine
$ systemctl restart ngcp-rtpengine-daemon.service ngcp-rtpengine-recording-daemon.service ngcp-rtpengine-recording-nfs-mount.service
$ systemctl enable ngcp-rtpengine-daemon.service ngcp-rtpengine-recording-daemon.service ngcp-rtpengine-recording-nfs-mount.service

$ systemctl stop rtpproxy
$ systemctl disable rtpproxy
```

#### 18. Ensure NextEPC HSS and PCRF of the Core Network is configured to use IMS

In nextepc.conf, add the floating IP of the VM running P-CSCF as shown below

```
pcscf:
  - 172.24.15.30
```

And, make sure to run NextEPC with Realm as created above "mnc096.mcc262.3gppnetwork.org" so that when a connection request from I-SCSF or S-SCSF
arrives to HSS with a FQDN hss.mnc096.mcc262.3gppnetwork.org it should be resolved as localhost of the NextEPC machine
{: .notice--warning}

- To change Realm in All-in-One NextEPC configuration

```
$ cd ~/nextepc/install/etc/nextepc/freediameter
```

- And, change all the configuration files in freediameter folder (hss.conf, mme.conf, pcrf.conf, pgw.conf) to have desired Domain Name
- Copy make_certs.sh from ~/nextepc/support/freeDiameter/ to ~/nextepc/install/etc/nextepc/freediameter
- Modify Domain Names in make_certs.sh and Generate the certificates using make_certs.sh
- Insert additional "ConnectPeer" entry in ~/nextepc/install/etc/nextepc/freediameter/pcrf.conf to connect to P-CSCF as show below

```
ConnectPeer = "pcscf.mnc096.mcc262.3gppnetwork.org" { ConnectTo = "172.24.15.30"; Port=3871; No_TLS; NO_SCTP; };
```
