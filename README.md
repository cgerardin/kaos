# README

## Quick start (GNU/Linux)

### 1 - Dependencies

First, ensure that all dependencies are installed :

#### Debian

```
# apt install git make clang-4.0 gnu-efi mtools qemu-system-x86 ovmf
# update-alternatives --install /usr/bin/clang clang /usr/bin/clang-4.0 100
```
#### Fedora
```
# dnf install git make clang gnu-efi mtools qemu-system-x86 edk2-ovmf
```

### 2 - Get the source code

Clone the GIT repository :

#### All distributions

```
$ git clone https://cgerardin@bitbucket.org/cgerardin/kaos.git
```

### 3 - Build and run

Build the kernel, copy it into a disk image and run it in QEMU :

#### All distributions
```
$ cd kaos
$ make run
```
