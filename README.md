# Compile instruction

## Before compiling

please check the [Releases](https://github.com/Franklalalala/usenauty/releases). There are pre-compiled linux (gcc 8.4.0) and windows(gnu8.3.0 & MSVC 19.29.30037.0) versions, respectively.

## Compile a linux version

The [CXX standard](https://en.wikipedia.org/wiki/C%2B%2B17) is set to be **17**, which means the gcc version need to be 8 or higher. Additionally, the cmake version needs to be **3.1** or higher.

With these conditions, one can follow these steps:

1. Load gcc and cmake modules. For example:

```
module load compiler/gcc/8.4.0
module load compiler/cmake/3.15.6
```

2. Make sure the compiler works.

```
which gcc
# /public/software/compiler/gnu/gcc-8.4.0/bin/gcc # The gcc path
export  CC=/public/software/compiler/gnu/gcc-8.4.0/bin/gcc
export  CXX=/public/software/compiler/gnu/gcc-8.4.0/bin/g++
```

3. Compile with commands as following:

```
git clone https://github.com/Franklalalala/usenauty
cd usenauty
mkdir build
cd build
cmake .. -G "Unix Makefiles"
make
```

## Compile a Window version

### The IDE way

The easiest way to compile is to setup a [Visual Studio IDE](https://en.wikipedia.org/wiki/Microsoft_Visual_Studio#2017), according to [CXX 17 standard](https://en.wikipedia.org/wiki/C%2B%2B17), the version of VS needs to be 17 or higher. 

After that, drag the whole project into the IDE. VS will automatically cmake this project. 

Finally, open the `.path/to/project/src/cagesearch.cpp` file in IDE, and click build. 

The target binary file will present in `.path/to/project/bin/` folder.

### Otherwise

One needs to prepare a compiler and cmake that meets requirements. See above.

The compile lines are the same as above.

## Note for compiler version

Generally speaking, a binary file that compiled from an advanced compiler may achieve a better performance, especially for computation intensive programs. Therefore, we recommend the compilers that satisfies the [CXX 17 standard](https://en.wikipedia.org/wiki/C%2B%2B17). 

On the other hand, for users who do not have the latest version of compiler, some changes of the `CMakeLists.txt` may help:

1. For a lower version of compiler, change the line in [CMakeLists.txt](https://github.com/Franklalalala/usenauty/blob/6cbecb9241538b83ffb0daaafa7ab78206fe2cff/CMakeLists.txt#L4) to:

```
set(CMAKE_CXX_STANDARD 14)
```

This will lower the compiler version requirements to [CXX 14 standard](https://en.wikipedia.org/wiki/C%2B%2B14).

2. For a lower version of cmake, change the line in [CMakeLists.txt](https://github.com/Franklalalala/usenauty/blob/6cbecb9241538b83ffb0daaafa7ab78206fe2cff/CMakeLists.txt#L1) and [src/CMakeLists.txt](https://github.com/Franklalalala/usenauty/blob/6cbecb9241538b83ffb0daaafa7ab78206fe2cff/src/CMakeLists.txt#L1):

```
cmake_minimum_required(VERSION 3.0)
```

This will lower the cmake version requirements to 3.0.

# File Comments

## File differences with source library codes

Changed some files from [source](https://pallini.di.uniroma1.it/index.html) Version 2.7R1 (MAY 19 2019):

- naututil.h:
  For msvc use, change `<sys/time.h>` to `"times.h"` and `times.c` with Macro defined for arch choice.
- gtools.h:
  For msvc use, change `<sys/wait.h>` to `"wait.h"`
- showg.c:
  Add a compiler choice `ASLIB`

# Code structure and function enhancement information

- New `CMakeLists.txt` instead of `makefile`.
- All `boolean` type code are changed to `_Boolean`
- Change a Macro in `nauty.h`:
  `FIRSTBITNZ(x)` to `msc_bsr_xx(x)` where call `unsigned long p` instead of `unsigned long *p`

File and `wait.h` is from `https://github.com/win32ports`, By MIT License. Some necessary changed is with them.
File `times.h` is used for necessary function and types.

Additionally, this project conducted several changes comparing to the original modification in [saltball/usenauty (github.com)](https://github.com/saltball/usenauty). 

* The step and random mode are created on top of the base mode.
* Code structures are refactored to enable some suitable API.
* Several test files in `./cmd_test/` folder.
  * Mode test and speed test for fullerene systems in `cmd_test/fullerene`
  * One dimension PBC system in `cmd_test/cabon_nano_tube`
  * Two dimension PBC system in `cmd_test/graphene_nano_ribbon`

* Bugs fixed for compilation in the Linux platform. (credit to @saltball)

# Other info
This repo is try to use `nauty` for some chemical research and create some new API. It's still on going.
Original License is [COPYRIGHT](libnauty/COPYRIGHT)
