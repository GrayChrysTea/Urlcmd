# Urlcmd v0.3.0

Made by GrayChrysTea

## Warning

I have only tested this on Ubuntu 20.04. Depending on your OS, the build
process may be different, or may not work at all.

If you have multiple queries in one command, make sure to encase your command
in double quotes, otherwise your shell may treat it as multiple commands
instead. For example:

```bash
urlcmd "gcc?@0=helloworld.c&o=app"
```

## What it is

Urlcmd is a command runner, but the commands it accepts must be in the form
of URLs. It takes the *pseudo-URLS* and converts them into commands a
terminal would accept and runs it. Here's an example:

```bash
urlcmd echo?@0=hello+world
# hello world
```

Since the pseudo-URLs in Urlcmd (hopefully) work like the URLs you see in
your browser, you can use escape codes to display symbols like '+' (%2b).
Do note that you can only pass in escape codes between %00 and %7f
(case insensitive).

```bash
urlcmd echo?@0=%2b
# +
```

If you want to get a glimpse of what is going on behind the scenes, you can
pass the verbosity option `v` with the level of verbosity like this:

```bash
urlcmd -v1 git?^help=1#init # Resulting commmand: git init --help
```

You can even run more than 1 command!

```bash
urlcmd sudo#su apt#update apt?^y=1#full-upgrade
# sudo su
# apt update
# apt full-upgrade -y
```

### From v0.3 onwards

If you do not want to run the output commands (i.e. preview them), you can
pass the `-p`review flag.

```bash
urlcmd -p ls?^a=1
# ls -a (no output by ls)
```

Occasionally, you may type in an erroneous pseudo-URL by the slip of your
fingers. Upon encountering with an invalid command, Urlcmd can react in
one of 2 ways, immediately stop running this command and any other commands
that come after this, or simply skip the invalid command. By default, Urlcmd
chooses the first option and panics. However, you may decide to allow Urlcmd
to skip the bad command by using the `-S`kip flag.

```bash
# Good command sequence
urlcmd "gcc?@0=helloworld.c&^c=1" "gcc?@=helloworld.o&o=app"
# gcc helloworld.c -c
# gcc helloworld.o -o app

# Bad command without skip
urlcmd "gcc?@0=helloworld.c&^c" "gcc?@=helloworld.o&o=app"
# Invalid command: gcc?@0=helloworld.c&^c

# Bad command with skip
urlcmd -S "gcc?@0=helloworld.c&^c" "gcc?@=helloworld.o&o=app"
# Invalid command: gcc?@0=helloworld.c&^c
# gcc helloworld.o -o app
# ^ This command will produce an error message because helloworld.c hasn't
#   actually been compiled but it will still be run anyway since the
#   preview (-p) flag hasn't been issued.
```

If you are working with DOS commands (if Urlcmd somehow manages to compile),
you can set the `-d`os flag to get DOS versions of your commands.

```bash
# cl is MSVC cl
urlcmd -d cl?@0=main.c&out=app.exe
# cl "main.c" /out:app.exe
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
