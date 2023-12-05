FROM ubuntu:22.04

RUN \
    set -eux && \
    export DEBIAN_FRONTEND=noninteractive && \
    apt -y update && \
    apt -y upgrade && \
    apt -y install \
        build-essential cmake freeglut3-dev gdb git iputils-ping libgl1-mesa-dev \
        libglu1-mesa-dev libjpeg-dev libmysqlclient-dev libnss3-dev libopus-dev \
        libpng-dev libsqlite3-dev libssl-dev libx11-xcb-dev libxcb-xinerama0-dev \
        libxcb-xkb-dev libxcb1-dev libxcursor-dev libxi-dev libxml2-dev libxrender-dev \
        libxslt-dev lzip mesa-common-dev nano perl python3 valgrind wget zlib1g-dev \
        '^libxcb.*-dev' libxkbcommon-dev libxkbcommon-x11-dev libgl-dev sudo \
        xdotool \
        ffmpeg && \
    apt -y autoremove && \
    apt -y autoclean && \
    apt -y clean && \
    rm -rf /var/lib/apt/lists/* && \
    exit 0

RUN set -eux
RUN cd /opt
RUN wget -q https://qt-mirror.dannhauer.de/official_releases/qt/5.15/5.15.11/single/qt-everywhere-opensource-src-5.15.11.tar.xz
RUN mkdir qt
RUN tar xf qt-everywhere-opensource-src-5.15.11.tar.xz -C ./qt
RUN rm qt-everywhere-opensource-src-5.15.11.tar.xz
RUN cd /opt/qt
RUN ./configure -opensource -confirm-license -release -static -nomake tests -nomake examples -skip qtwebengine -qt-zlib -qt-libjpeg -qt-libpng -xcb -qt-freetype -qt-pcre -qt-harfbuzz
RUN make -j $(nproc)
RUN make install
RUN cd /opt
RUN exit 0

ENV PATH="${PATH}:/usr/local/Qt-5.15.9/bin"
ENTRYPOINT ["/bin/bash"]
