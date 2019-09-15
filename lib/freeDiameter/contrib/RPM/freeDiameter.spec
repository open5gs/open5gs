
Name: freeDiameter		
Version: 1.1.6 	
Release: 1%{?dist}
Packager: krum.boy4ev@gmail.com
Summary: freeDiameter is an implementation of the Diameter protocol. 	
Group: Development/Libraries		
License: BSD License	
URL: http://www.freediameter.net		
Source0: http://www.freediameter.net/hg/freeDiameter/archive/1.1.6.tar.gz	

BuildRequires: cmake make gcc gcc-c++ flex bison lksctp-tools-devel 
BuildRequires: gnutls-devel libgcrypt-devel libidn-devel
BuildRequires: mercurial	
Requires: lksctp-tools	

%description
freeDiameter is an implementation of the Diameter protocol.
	
Diameter is a protocol designed to carry Authentication, Authorization and
Accounting (AAA) payload. It is an evolution of the RADIUS protocol (as the
name suggests).

See http://www.freediameter.net/ for more information on the project.
	
freeDiameter was previously known as the "waaad" project (WIDE AAA Daemon)
This project is not related to the "freediameter" project from Sun on sourceforge.
	
Author: Sebastien Decugis.

%package daemon
Summary:  Simple daemon parses the command line and initializes the freeDiameter framework.
Group: Development/Libraries
Requires: freeDiameter

%description daemon
freeDiameterd : this simple daemon parses the command line and initializes the
freeDiameter framework. Use it for your Diameter server & agent components.
In case of Diameter clients, you probably will prefer linking the libfdcore
directly with your client application that must be made Diameter-aware.


%prep
%setup -qn %{name}-%{version}


%build
mkdir -p build
cd build
cmake ../

make %{?_smp_mflags}


%install
rm -rf $RPM_BUILD_ROOT
cd build
make install DESTDIR=$RPM_BUILD_ROOT
make test

%post daemon
echo "/usr/local/lib/" > /etc/ld.so.conf.d/%{name}.conf 
/sbin/ldconfig

%files
%defattr(-,root,root,-)
/usr/local/include/
/usr/local/lib/

%files daemon
%defattr(-,root,root,-)
/usr/local/bin/



%changelog
 * Sat Jul 5 2013 Krum Boychev <krum.boy4ev@gmail.com> - 1.1.6-1
 - initial version
