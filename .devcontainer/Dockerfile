# Inspired by https://github.com/devcontainers/images/tree/main/src/cpp
ARG VARIANT=ubuntu-24.04
FROM mcr.microsoft.com/devcontainers/base:${VARIANT}
USER root

# Install needed packages. Use a separate RUN statement to add your own dependencies.
RUN wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | gpg --dearmor - | sudo tee /usr/share/keyrings/kitware-archive-keyring.gpg >/dev/null

RUN echo 'deb [signed-by=/usr/share/keyrings/kitware-archive-keyring.gpg] https://apt.kitware.com/ubuntu/ noble main' | sudo tee /etc/apt/sources.list.d/kitware.list >/dev/null

RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
    && apt-get -y install \
    git \
    build-essential \
    cmake \
    ninja-build \
    gdb \
    doxygen \
    graphviz \
    mscgen \
    dia \
    clang \
    clang-tools \
    clang-format \
    valgrind \
    gawk \
    cppcheck \
    lcov \
    python3 \
    python3-pip \
    pkg-config