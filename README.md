# About
This a library to parse .ini files for other projects. I want to use this for all of my other projects instead of relying on a 3rd-party. I just thought why not and here are we.

# Disclaimer
**This library is originally meant to be used for other SMaster4 projects.
It is not a library meant to be used in combination with anything else since the stability and maintanance is tied to other SMaster4 Project needs.**
No one dissallows you from using it, but there is no guarantue for stability.

# Building
## Requirements
+ CMake(Version 3.12 and upwards)
+ A C11 compatible compiler(like GCC)
+ A Build System CMake can compile to(comes usally with the compiler)
+ Git™(optional, but recommended)

## Install requirements
### CMake
CMake is a tool to generate build files for your choosen build system.
You check if CMake is installed by running:
```bash
cmake --version
```
If CMake is installed, the version of CMake should be now printed out.
If this version isn't 3.12 or an error stating that cmake isn't installed or similar, please install CMake with the package manager of your OS or use the [download page](https://cmake.org/download/) of CMake.

### C11 compatible compiler(In this case GCC)
Please note that they are many other compilers, but for this explanation we are just gona use GCC.
The building steps can slightly differ if you choose a different compiler.
Consider reading the documentation of your compiler if you have issues.
You can check if GCC is allready installed by running:
```bash
gcc --version
```
If it now prints out the version of GCC, then you can skip the installation process.
If it doesn't please install GCC or your choosen compiler with the package manager of your OS or follow the [official install guide](https://gcc.gnu.org/install/) of GGC.
Do note that GCC isn't officially available on Windows.
There are forks(projects using other projects as a base to develope on) like MinGW64 supporting Windows.

### Git™
Git™ is a version control system whit which you can download the entire project with one command.
To check if it is allready installed(which it mostly is) run:
```bash
git --version
```
If it isn't installed, then install it with your package manager or download it from the [offical website](git-scm.com).

## Build the project
1. Clone the project by typing:
```bash
git clone https://github.com/SMASTER4/smaster4s-inis
```
2. Change your directory to the project(optional):
```bash
cd smaster4s-inis
```
3. Create a new directory to build your project in(optional):
```bash
mkdir out # The name doesn't matter at all
```
4. Now just go into the directory(optional):
```bash
cd out
```
5. If you use a different compiler you might need to set the CC enviorement variable so anything works(this might differ from OS to OS):
```bash
export CC=/usr/bin/clang # Example for Linux and clang
```
6. Now just compile your Makefile with cmake:
```bash
cmake .. # The directory where the so called CMakeLists.txt is inside
```
7. Now you can just build this **WILL** differ from compiler to compiler. This is an example for GCC:
```bash
make
```
