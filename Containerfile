FROM ghcr.io/tcarland/tcanetpp:v1.7.1

ENV TCAMAKE_PROJECT=/opt
ENV TCAMAKE_HOME=/opt/tcamake
ENV TCAMAKE_PREFIX=/usr

USER root
WORKDIR /opt

COPY . libhexes

RUN chown -R tdh:tdh /opt/libhexes && \
    cd libhexes && \
    make

USER tdh

