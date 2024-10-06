# OpenFHE Basics README

## Installation

### Step-by-Step Guide

1. **Clone the Repository**:
    ```bash
    git clone https://github.com/smarti39/openfhe-basics.git
    cd openfhe-basics
    ```

2. **Install Dependencies**:
    Ensure you have all necessary dependencies installed:
    - OpenFHE
    - CMake
    - Standard C++ libraries
    
    Note: `tjws-03` has all dependencies installed.

3. **Build the Project**:
    ```bash
    mkdir build
    cd build
    cmake ..
    make
    ```

## Usage

### Execution

To run the executable, navigate to the `build` folder and use the following command in your terminal:

```bash
./BasicsExecutable
```

Note that any input files the program needs to access must be located in the `build` folder.

### New Source Files

If any new source files are created, be sure to add them to the bottom of `CMakeLists.txt` as follows:

```cmake
### CMakeLists.txt
add_executable( BasicsExecutable
    src/main.cpp
    src/openFHE_wrapper.cpp
    [ NEW SOURCE FILEPATH ]
)
```