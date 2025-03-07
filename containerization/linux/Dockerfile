# Docker file to create a Linux image
FROM ubuntu:22.04

# Set the timezone to avoid interactive prompts
ENV DEBIAN_FRONTEND=noninteractive

# Install essential build tools
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    ninja-build \
    gdb \
    git \
    wget \
    curl \
    unzip \
    valgrind \
    cppcheck \
    cpplint \
    && rm -rf /var/lib/apt/lists/*

# Install Catch2 for unit testing (header-only)
# Rather than do it here, fetch Catch2 in CMakeLists.txt
#RUN wget -O /usr/local/include/catch2.hpp https://github.com/catchorg/Catch2/releases/download/v3.4.0/catch.hpp

# Set up a non-root user for Dev Containers
#ARG USERNAME=dev
#RUN useradd -m ${USERNAME} && echo "${USERNAME} ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers
#USER ${USERNAME}

# Set the working directory
WORKDIR /app

# Default command (use bash shell)
# Uncomment for interactive use
#CMD ["/bin/bash"]