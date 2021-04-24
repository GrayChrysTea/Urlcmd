# Urlcmd v0.4.0

Made by GrayChrysTea

## Warning

I have only tested this on Ubuntu 20.04. Depending on your OS, the build
process may be different, or may not work at all.

## What it is

Urlcmd is a command runner, but the commands it accepts must be in the form
of URLs. It takes the *pseudo-URLS* and converts them into commands a
terminal would accept and runs it. Here's an example:

```bash
urlcmd echo?@0=hello&@1=world
# hello world
```

If you want to get a glimpse of what is going on behind the scenes, you can
pass the verbosity option `v` with the level of verbosity like this:

```bash
urlcmd -v1 git?^help=1#init # Resulting commmand: git init --help
```

## Why

I was just bored.

## Build Instructions

Urlcmd requires [CMake](https://cmake.org/) version 3.12 or higher to generate
the Makefiles required to build it, which you can install using the following
command:

```bash
sudo snap install cmake --classic
```

If you already have CMake, you need to check to see whether you already
have the [Boost](https://www.boost.org) C++ libraries (at least version 1.67).
If you do not have it, install it using:

```bash
sudo apt install libboost-all-dev
```

Then, `cd` into the root folder for this project and run:

```bash
sudo bash build.sh
sudo bash install.sh
```

These 2 commands will build Urlcmd and copy the executable into your
/usr/bin directory.

## TODO

- [x] Complete the core features of Urlcmd
- [x] Improve help message to include command examples and formats
- [x] Add flag to preview commands without running them
