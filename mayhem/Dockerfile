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

COPY . /repo
WORKDIR /repo
RUN chmod +x build_linux_portable.sh
RUN ./build_linux_portable.sh

RUN mkdir -p /deps
RUN ldd /repo/build/release/diec | tr -s '[:blank:]' '\n' | grep '^/' | xargs -I % sh -c 'cp % /deps;'

FROM ubuntu:bionic as package

COPY --from=builder /deps /deps
COPY --from=builder /repo/build/release/diec /repo/build/release/diec
ENV LD_LIBRARY_PATH=/deps
