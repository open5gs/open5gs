FROM ubuntu:16.04

RUN apt update
# Base Dependencies
RUN apt --assume-yes install bison flex wget gcc g++ cmake git
# Dependencies for Debian package
RUN apt --assume-yes install fakeroot debhelper
# Dependencies for freeDiameter core
RUN apt --assume-yes install libsctp-dev libgnutls-dev libidn11-dev
# Dependencies for freeDiameter extensions
RUN apt --assume-yes install libxml2-dev libpostgresql-ocaml-dev libmysql-ocaml-dev libgcrypt-dev
RUN apt --assume-yes install swig python-dev

# Download freeDiameter and Unpack
WORKDIR /root
#RUN git clone https://github.com/Metaswitch/freeDiameter.git -b release-128
CMD (\
    mkdir freeDiameter;\
    wget -q -O - http://www.freediameter.net/hg/freeDiameter/archive/2cb8d71a405d.tar.bz2 | tar xj -C freeDiameter --strip-components=1; \
    cd /root/freeDiameter; \
    ln -s contrib/debian .; \
    fakeroot dh binary)

# Start to build freeDiameter
#RUN mkdir build
#WORKDIR /root/freeDiameter-2cb8d71a405d/build
#RUN cmake ../
#RUN make

