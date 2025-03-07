# Docker file to create an image that will run on a Mac
FROM debian:latest

# Install dependencies
RUN apt update && apt install -y \
    g++ \
    cmake \
    make \
    gdb \
    valgrind \
    wget \
    unzip \
    cppcheck \
    cpplint \
    && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /app

# Default command: Start a bash shell
CMD ["/bin/bash"]