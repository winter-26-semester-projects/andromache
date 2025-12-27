![andro](docs/images/logo.png)

# AndromacheOS

Andromache is cross-hyrbid between an exokernel and a monolithic kernel design, implementimg a systematic fault tolerance policy known as the _Do Not Panic_ rule.

## Build

You may run the bash script in `/scripts/build.sh`, but they require certain dependencies, and the `i386-gcc-elf` compiler, or the clang equivalent.

The dependencies are:

```
build-essential
cmake
gcc-multilib
xorriso
mtools
grub-common
grub-pc-bin
```

If they are present, you may do the following:

```sh
bash -x scripts/build.sh
```

`andromache.iso` can then be found in the root directory; you may boot the ISO by running:

```sh
qemu-systems-i386 -cdrom andromache.iso
```
## Architecture

The kernel may encapsulate all services and modules after it is loaded onto memory; as processes are forked, the kernel may eliminate all services, and push them into user space.

This is a complete remodel of the System ABI, thus breaking the initial contract, and exposing many new contracts to user space.

The modules are:

```
child_handler.c
ipc.c
sched.c
signal.c
```

