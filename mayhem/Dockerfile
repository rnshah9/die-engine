ARG image=ubuntu:bionic

FROM ${image} as source-internet

FROM source-internet as builder

RUN apt-get update --quiet
RUN apt-get install --quiet --assume-yes \
      git  \
      build-essential \
      qt5-default \
      qtbase5-dev \
      qtscript5-dev \
      qttools5-dev-tools
RUN DEBIAN_FRONTEND=noninteractive apt-get install -y clang

COPY . /repo
WORKDIR /repo
RUN chmod +x build_linux_portable.sh
RUN ./build_linux_portable.sh

ENV QMAKE_CXX=clang++
ENV QMAKE_LINK=clang++
RUN cp /repo/mayhem/lib.pro /repo/console_source/
WORKDIR /repo/console_source/
RUN qmake -o Makefile lib.pro
RUN make

WORKDIR /repo/mayhem
RUN qmake -o Makefile fuzz.pro
RUN make
RUN cp /repo/mayhem/fuzz /fuzz_infoMessage

FROM ubuntu:bionic as package

RUN apt-get update --quiet
RUN apt-get install --quiet --assume-yes \
      qt5-default qtscript5-dev 
COPY --from=builder /repo/build/release/diec /repo/build/release/diec
COPY --from=builder /fuzz_infoMessage /fuzz_infoMessage
