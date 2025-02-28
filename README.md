# Branch-Bound-Flowshop

This project implements a simplified Branch-and-Bound (B&B) algorithm for solving the Permutation Flow-Shop Scheduling Problem (PFSP) with the objective of minimizing makespan. It uses static forward branching and the one-machine lower bound (LB1), based on the paper "A computationally efficient Branch-and-Bound algorithm for the permutation flow-shop scheduling problem" by Gmys et al. (2020).

## Prerequisites
- **CMake**: Version 3.10 or higher.
- **C++ Compiler**: Supporting C++17 (e.g., GCC, Clang, MSVC).

## Input file structure:
- first line: n m
- next n lines : line i represents the time needed for task i on all machines
- Example:
3 3
1 2 3
4 5 6
7 8 9

3 tasks 3 machines


## Building the Project
1. **Clone the Repository**:
   ```bash
   git clone https://github.com/SEGHAIRII/Branch-Bound-Flowshop.git
   cd Branch-Bound-Flowshop```

2. **Create a Build Directory**:
    ```bash
    mkdir build
    cd build```

3. **Run CMake**:
    ```bash
    cmake ..```

4. **Compile**:
    ```bash
    make```