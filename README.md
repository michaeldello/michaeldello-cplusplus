# michaeldello-cplusplus

Working examples demonstrating design principles and pattern based solutions using modern C++


# Docker

Docker build and VSCode Dev Container specs are under the `containerization` folder.

Symbolic linkage at the top level selects which container to use locally.

Build the Docker image with:

    docker build -t cpp_dev .

Run the Docker container with:

    docker run --rm -it --name cpp_dev_container -v $(pwd):/app cpp_dev

Develop and/or execute within Docker container in VSCode by selecting **Reopen in Container** if prompted (VSCode should detect devcontainer file included in this project).


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