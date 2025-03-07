# michaeldello-cplusplus

Working examples demonstrating design principles and pattern based solutions using modern C++


# Docker

## Local Development

Docker build and VSCode Dev Container specs are under the `containerization` folder.

Symbolic linkage at the top level selects which container to use locally.

Build the Docker image with:

    docker build -t cpp_dev .

Run the Docker container with:

    docker run --rm -it --name cpp_dev_container -v $(pwd):/app cpp_dev

Develop and/or execute within Docker container in VSCode by selecting **Reopen in Container** if prompted (VSCode should detect devcontainer file included in this project).

## Use in GitHub Actions

To use the Docker container in GitHub actions:

1. Authenticate Docker with GitHub Container Registry

    echo *YOUR_GITHUB_TOKEN* | docker login ghcr.io -u *YOUR_GITHUB_USERNAME* --password-stdin

1. Build and tag the Docker image from the Dockerfile

    docker build -t ghcr.io/*YOUR_GITHUB_USERNAME*/cpp-dev:latest .

1. Push the image to GHCR

    docker push ghcr.io/*YOUR_GITHUB_USERNAME*/cpp-dev:latest

1. Use/Pull the container image in GitHub Actions

    docker pull ghcr.io/*YOUR_GITHUB_USERNAME*/cpp-dev:latest

Notes:

1. The container will need to either be public, or if private, be made accessible by the repository


# Build Code

From the root directory in Docker container:

    cmake -B build
    cmake --build build
    ./build/run_cpp_examples

To clean the build, remove the build folder

    rm -rf ./build

Alternatively, when working in VSCode and attached to the running container, the .vscode/tasks.json config allows building using:

    Cmd + Shift + B

... and Clean and/or Rebuild options are available using:

    Cmd + Shift + P (then search for Clean or Rebuild)


# Debug Using GDB

From the root directory in Docker container:

    cd build
    cmake .. -DCMAKE_BUILD_TYPE=Debug
    make clean
    make    gdb build/run_cpp_examples
    (gdb) break main
    (gdb) run
    (gdb) step
    (gdb) print <some variable>
    (gdb) continue
    etc...
    (gdb) quit


# Debug Using VSCode

Install Extensions:

1. C/C++ Extension
1. Remote - Containers
1. CMake Tools

Build the code.

The .vscode/launch.json file will be detected by VSCode when you select **Run and Debug** (or press F5).

Select **Docker C++ Debug**.


# Unit Testing

Catch2 is used because:

1. It integrates well with CMake
1. It is a header-only dependency model
1. Nothing extra to install
1. Nothing extra to compile
1. Portable

Inside the container, after building the code, run the unit tests:

    cd build && ctest --verbose

Directing this to a file to make it available for archiving is advisable

    cd build && ctest --verbose > unit_test_results.txt

Debugging Unit Tests requires building with debug symbols in CMakeLists.txt

    set(CMAKE_BUILD_TYPE Debug)
    set(CMAKE_CXX_FLAGS_DEBUG "-g")

... then building for Debug

    cmake -B build -DCMAKE_BUILD_TYPE=Debug
    cmake --build build

# Static Code Analysis

Install cppcheck and cpplint (Debian Container)

    apt update && apt install -y cppcheck cpplint
