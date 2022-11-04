FROM node:19

MAINTAINER Sukchan Lee <acetcom@gmail.com>

ARG PACKAGE=open5gs
ARG VERSION=2.5.5

RUN set -e; \
    cd /usr/src; \
    rm -rf ./$PACKAGE; \
    curl -SLO "https://github.com/open5gs/$PACKAGE/archive/v$VERSION.tar.gz"; \
    tar -xvf v$VERSION.tar.gz; \
    mv ./$PACKAGE-$VERSION/ ./$PACKAGE;

WORKDIR /usr/src/open5gs/webui
RUN npm clean-install && \
    npm run build

ADD https://github.com/ufoscout/docker-compose-wait/releases/download/2.9.0/wait /wait
RUN chmod +x /wait

CMD /wait && npm run start

EXPOSE 3000
