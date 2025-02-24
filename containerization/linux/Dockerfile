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
    && rm -rf /var/lib/apt/lists/*

# Install Catch2 for unit testing (header-only)
RUN wget -O /usr/local/include/catch2.hpp https://github.com/catchorg/Catch2/releases/download/v3.4.0/catch.hpp

# Set up a non-root user for Dev Containers
ARG USERNAME=dev
RUN useradd -m ${USERNAME} && echo "${USERNAME} ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers
USER ${USERNAME}

# Set the working directory
WORKDIR /workspace

# Default command (use bash shell)
CMD ["/bin/bash"]