FROM ubuntu:20.04

ENV TZ=America/Sao_Paulo

RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

RUN apt-get update && apt-get install -y build-essential git imagemagick

RUN git clone https://github.com/stefanhaustein/TerminalImageViewer.git

WORKDIR TerminalImageViewer/src/main/cpp

RUN make

RUN make install

WORKDIR /app
