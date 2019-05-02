# README

## Quick start

### 1 - Dependencies

First, ensure that all dependencies are installed :

#### Debian / Ubuntu / Windows 10 Linux's subsystem

```
# apt install git make clang-4.0 gnu-efi mtools qemu-system-x86 ovmf parted
# update-alternatives --install /usr/bin/clang clang /usr/bin/clang-4.0 100
```
#### Fedora
```
# dnf install git make clang gnu-efi gnu-efi-devel mtools qemu-system-x86 edk2-ovmf parted
```

### 2 - Get the source code

Clone the GIT repository :

#### All distributions

```
$ git clone https://cgerardin@bitbucket.org/cgerardin/kaos.git
```

### 3 - Build and run

Build the kernel, copy it into a disk image and run it in QEMU :

#### Fedora

- First, adapte the Makefile according to the comments marked 'Fedora :'.

#### Windows 10 Linux's subsystem

- First, install and run an X server like [VcXsrv](https://sourceforge.net/projects/vcxsrv/), then run :
```
$ export DISPLAY=0:0
```

#### All systems
```
$ cd kaos
$ make buildrun
```
