# README

## Quick start

### Debian

First, ensure all build's dependencies are installed :

```
# apt install git make clang-3.8 gnu-efi mtools qemu-system-x86 ovmf
$ sudo update-alternatives --install /usr/bin/clang clang /usr/bin/clang-3.8 100
```

Clone the GIT repository  :
```
$ git clone git@bitbucket.org:cgerardin/kaos.git KaOS
```
Build kernel into a disk image and run it in QEMU :
```
$ cd KaOS
$ make run
```