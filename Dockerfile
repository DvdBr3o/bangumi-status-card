FROM ubuntu:24.04 AS builder

ENV DEBIAN_FRONTEND noninteractive

# Install xmake

RUN apt-get -y update && \
    apt-get -y install xmake unzip curl build-essential cmake git

ENV XMAKE_ROOT=y

# Deploy bangumi-status-card

WORKDIR /app

COPY . /app

RUN xmake f -c -m release -p linux -a x64 -y && \
    xmake b bangumi-status-card

FROM debian:trixie-slim

ENV DEBIAN_FRONTEND noninteractive

RUN apt-get -y update && \
    apt-get install -y ca-certificates curl

COPY --from=builder /app/build/linux/x64/release/bangumi-status-card .

EXPOSE 8080

CMD ["./bangumi-status-card"]
