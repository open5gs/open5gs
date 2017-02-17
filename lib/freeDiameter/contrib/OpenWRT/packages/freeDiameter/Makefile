#
# Software License Agreement (BSD License)
#
# Copyright (c) 2013, WIDE Project and NICT
# All rights reserved.
#
# See LICENSE file from freeDiameter source package for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=freeDiameter
PKG_REV:=696
PKG_VERSION:=r$(PKG_REV)
PKG_RELEASE:=1

PKG_SOURCE_PROTO:=hg
PKG_SOURCE_VERSION:=$(PKG_REV)
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=http://www.freediameter.net/hg/freeDiameter
# PKG_MD5SUM:=

PKG_FIXUP:=libtool
PKG_INSTALL:=1

include $(INCLUDE_DIR)/package.mk

define Package/freeDiameter
  SECTION:=freeDiameter
  CATEGORY:=Network
  TITLE:=freeDiameter
  URL:=http://www.freediameter.net
  DEPENDS:=+sctp +libgnutls +libpthread +kmod-ipv6
endef

define Package/freeDiameter-test
  SECTION:=freeDiameter
  CATEGORY:=Network
  TITLE:=freeDiameter-test
  URL:=http://www.freediameter.net
  DEPENDS:=+freeDiameter
endef

define Package/freeDiameter/description
 freeDiameter + RADIUS/Diameter gateway extension package.
endef

define Package/freeDiameter-test/description
 The app_test.fdx extension for freeDiameter, useful only to perform some
tests between freeDiameter peers (ping-like for Diameter).
endef

define Package/freeDiameter/conffiles
/etc/freeDiameter/freeDiameter.conf
/etc/freeDiameter/rgw.conf
endef

define Build/Configure
    IN_OPENWRT=1 \
    AR="$(TARGET_CROSS)ar" \
    AS="$(TARGET_CC) -c $(TARGET_CFLAGS)" \
    LD="$(TARGET_CROSS)ld" \
    NM="$(TARGET_CROSS)nm" \
    CC="$(TARGET_CC)" \
    GCC="$(TARGET_CC)" \
    CXX="$(TARGET_CROSS)g++" \
    RANLIB="$(TARGET_CROSS)ranlib" \
    STRIP="$(TARGET_CROSS)strip" \
    OBJCOPY="$(TARGET_CROSS)objcopy" \
    OBJDUMP="$(TARGET_CROSS)objdump" \
    TARGET_CPPFLAGS="$(TARGET_CPPFLAGS)" \
    TARGET_CFLAGS="$(TARGET_CFLAGS)" \
    TARGET_LDFLAGS="$(TARGET_LDFLAGS)" \
    cmake \
		-DCMAKE_PREFIX_PATH:PATH=$(STAGING_DIR)/usr \
    		-DCMAKE_INSTALL_PREFIX:PATH=/usr \
                -DDIAMID_IDNA_REJECT:BOOL=ON \
                -DBUILD_TESTING:BOOL=OFF \
		-DCMAKE_BUILD_TYPE:STRING=DebianPackage \
		-DDEFAULT_CONF_PATH:PATH=/etc/freeDiameter \
		-DBUILD_APP_RADGW:BOOL=ON \
		-DBUILD_DBG_MONITOR:BOOL=ON \
		-DBUILD_TEST_APP:BOOL=ON \
		VERBOSE=1 \
	$(PKG_BUILD_DIR)/CMakeLists.txt 
endef

TARGET_LDFLAGS := -L$(STAGING_DIR)/usr/lib $(TARGET_LDFLAGS)
define Package/freeDiameter/install
	# binaries
	$(INSTALL_DIR) $(1)/usr/bin
	$(CP) \
		$(PKG_INSTALL_DIR)/usr/bin/freeDiameterd* \
		$(1)/usr/bin/
	# libraries & extensions
	$(INSTALL_DIR) $(1)/usr/lib/
	$(CP) \
		$(PKG_INSTALL_DIR)/usr/lib/* \
		$(1)/usr/lib/
	# Remove the test_app from the main package (see freeDiameter-test)
	$(RM) $(1)/usr/lib/freeDiameter/test_app*
	
	# configuration files
	$(INSTALL_DIR) $(1)/etc/freeDiameter
	$(INSTALL_CONF) \
		$(PKG_BUILD_DIR)/doc/freediameter.conf.sample \
		$(1)/etc/freeDiameter/freeDiameter.conf
	$(SED) 's,TLS_Cred,#TLS_Cred,g' 		   $(1)/etc/freeDiameter/freeDiameter.conf
	echo "" 					>> $(1)/etc/freeDiameter/freeDiameter.conf
	echo "### OPENWRT specific"		 	>> $(1)/etc/freeDiameter/freeDiameter.conf
	echo "TLS_Cred = \"/etc/freeDiameter/freeDiameter.pem\", \"/etc/freeDiameter/freeDiameter.key\";" \
							>> $(1)/etc/freeDiameter/freeDiameter.conf
	echo "TLS_CA = \"/etc/freeDiameter/freeDiameter.ca.pem\";" \
							>> $(1)/etc/freeDiameter/freeDiameter.conf
	echo "TLS_DH_File = \"/etc/freeDiameter/dh.pem\";" \
				 			>> $(1)/etc/freeDiameter/freeDiameter.conf
	echo "SCTP_streams = 3;"			>> $(1)/etc/freeDiameter/freeDiameter.conf
	echo "LoadExtension = \"dict_nasreq.fdx\";" 	>> $(1)/etc/freeDiameter/freeDiameter.conf
	echo "LoadExtension = \"dict_eap.fdx\";" 	>> $(1)/etc/freeDiameter/freeDiameter.conf
	echo "LoadExtension = \"app_radgw.fdx\":\"rgw.conf\";" \
							>> $(1)/etc/freeDiameter/freeDiameter.conf
	echo "# test_app.fdx provided in freeDiameter-test package:" \
							>> $(1)/etc/freeDiameter/freeDiameter.conf
	echo "# LoadExtension = \"test_app.fdx\";"	>> $(1)/etc/freeDiameter/freeDiameter.conf
	echo "## Add overrides below this point" 	>> $(1)/etc/freeDiameter/freeDiameter.conf

	
	$(INSTALL_CONF) \
		$(PKG_BUILD_DIR)/doc/app_radgw.conf.sample \
		$(1)/etc/freeDiameter/rgw.conf
	$(SED) 's,RGWX,#RGWX,g' 		  	   $(1)/etc/freeDiameter/rgw.conf
	echo "" 					>> $(1)/etc/freeDiameter/rgw.conf
	echo "### OPENWRT specific"		 	>> $(1)/etc/freeDiameter/rgw.conf
	echo "  RGWX = \"auth.rgwx\" : auth;"		>> $(1)/etc/freeDiameter/rgw.conf
	echo "  RGWX = \"acct.rgwx\" : acct;"		>> $(1)/etc/freeDiameter/rgw.conf
	echo "" 					>> $(1)/etc/freeDiameter/rgw.conf
	echo "  cli = 127.0.0.1 / \"secret key\" ;" 	>> $(1)/etc/freeDiameter/rgw.conf
	echo "  auth_server_ip4 = 127.0.0.1;" 		>> $(1)/etc/freeDiameter/rgw.conf
	echo "  auth_server_ip6 = ::1 ;" 		>> $(1)/etc/freeDiameter/rgw.conf
	echo "  acct_server_ip4 = 127.0.0.1;" 		>> $(1)/etc/freeDiameter/rgw.conf
	echo "  acct_server_ip6 = ::1 ;" 		>> $(1)/etc/freeDiameter/rgw.conf
endef

define Package/freeDiameter-test/install
	# Only the test_app extension
	$(INSTALL_DIR) $(1)/usr/lib/freeDiameter/
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/freeDiameter/test_app* \
		$(1)/usr/lib/freeDiameter/
endef

define Package/freeDiameter/postinst
#!/bin/sh

# Test if the configuration file contains the local identity already
localid=`sed -n -r -e "s/^[[:space:]]*Identity[[:space:]]*=[[:space:]]*\"([^\"]*)\"[[:space:]]*;/\1/p" /etc/freeDiameter/freeDiameter.conf`
if [ -z "$$localid" ]; then
   # Ask for the local name
   echo -n "Full name of your access point? (openwrt.localdomain) : "
   read localid
   if [ -z "$$localid" ]; then
      localid="openwrt.localdomain"
   fi
   echo "Identity = \"$$localid\";" >> /etc/freeDiameter/freeDiameter.conf
fi

# Is there already a ConnectPeer directive?
grep -q -E -e "^[[:space:]]*ConnectPeer[[:space:]]*=" /etc/freeDiameter/freeDiameter.conf
if [ "$$?" -eq "1" ]; then
   echo -n "Diameter Identity of your Diameter server: "
   read serverid
   if [ -z "$$serverid" ]; then
      echo "Skipped. Please add ConnectPeer directive to your /etc/freeDiameter/freeDiameter.conf file later."
   else
      echo -n "IP or IPv6 address of your Diameter server? (leave blank for dynamic resolution) "
      read serverip
      connstr=""
      if [ -n "$$serverip" ]; then
        connstr=" { ConnectTo = \"$$serverip\"; }"
      fi
      echo "ConnectPeer = \"$$serverid\"$$connstr;" >> /etc/freeDiameter/freeDiameter.conf
   fi
fi

# Certificate configuration    
if [ ! -f "/usr/bin/certtool" ]; then
   echo "certtool is not installed, skipping creation of default certificate and DH parameters."
   echo "The following files are expected by freeDiameter:"
   echo "  /etc/freeDiameter/freeDiameter.key"
   echo "  /etc/freeDiameter/freeDiameter.pem"
   echo "  /etc/freeDiameter/freeDiameter.ca.pem"
   echo "  /etc/freeDiameter/dh.pem"
   exit 0
fi
if [ ! -f "/etc/freeDiameter/freeDiameter.key" ]; then 
   echo "Creating a new private key for freeDiameter, please wait"
   certtool -p --outfile /etc/freeDiameter/freeDiameter.key
fi
if [ ! -f "/etc/freeDiameter/freeDiameter.pem" ]; then
   echo "organization = freeDiameter"		> /tmp/template.cnf
   echo "unit = OpenWRT"			>>/tmp/template.cnf
   echo "state = internet"			>>/tmp/template.cnf
   echo "country = net"				>>/tmp/template.cnf
   echo "cn = $$localid"			>>/tmp/template.cnf
   echo "expiration_days = 3650"		>>/tmp/template.cnf
   echo "signing_key"				>>/tmp/template.cnf
   echo "encryption_key"			>>/tmp/template.cnf
   if [ ! -f "/etc/freeDiameter/freeDiameter.csr" ]; then 
      echo "Creating a new CSR (use if you have a separate CA)"
      certtool -q --load-privkey /etc/freeDiameter/freeDiameter.key \
               --outfile /etc/freeDiameter/freeDiameter.csr \
	       --template /tmp/template.cnf
   fi
   echo "Creating a new certificate for freeDiameter"
   certtool -s --load-privkey /etc/freeDiameter/freeDiameter.key \
               --outfile /etc/freeDiameter/freeDiameter.pem \
	       --template /tmp/template.cnf
   rm -f /tmp/template.cnf
   cat /etc/freeDiameter/freeDiameter.pem >> /etc/freeDiameter/freeDiameter.ca.pem
   echo "Done."
   echo "========================================================================"
   echo "To enable TLS communication, you should either:"
   echo "  - use a real certificate signed by your server's CA:"
   echo "      Use the CSR provided in /etc/freeDiameter/freeDiameter.csr"
   echo "      Save the new certificate as /etc/freeDiameter/freeDiameter.pem"
   echo "      Replace the contents of /etc/freeDiameter/freeDiameter.ca.pem with your CA's certificate"
   echo "  - or, declare the certificates as trusted as follow: "
   echo "      Add your server's CA certificate into /etc/freeDiameter/freeDiameter.ca.pem"
   echo "      Add the content of /etc/freeDiameter/freeDiameter.pem into your server's trusted CA file"
   echo "========================================================================"
fi
if [ ! -f "/etc/freeDiameter/dh.pem" ]; then
   echo "Creating new Diffie-Hellman parameters file. This operation takes a while..."
   certtool --generate-dh-params --outfile /etc/freeDiameter/dh.pem
   echo "Done."
fi
echo "freeDiameter configuration completed and stored in /etc/freeDiameter/."
endef

$(eval $(call BuildPackage,freeDiameter))
$(eval $(call BuildPackage,freeDiameter-test))
