# Hipp'OS

Hipp'OS (pronounced **hippos**) is an attempt at trying to build an operating system. I have no expectation for this, but should be a good jumping off point. I'm using this README as build log, for my future self and any other lost souls out there. 

I am following Little OS Book as of right now

## Development Environment
Currently developing on Ubuntu 24.04 LTS, running as a VM on my UnRaid server

Connected to VSCode via Remote-SSH session, and exposed to SSH via Cloudflare Tunnels.

To install dev toolchain, run:

```
    sudo apt-get install build-essential nasm xorriso grub-pc-bin bochs bochs-sdl mtools
```

## A departure from LittleOSBook

I am using this section of the README as a form of journal, admittedly not the best idea, but it's convenient and available. After following the LittleOSBook to Chapter 4, I learned a lot of things - and some of those might not be best practice, looking back. It still remains an invaluable resource to get started, but for future me's sake, let's try and do things a bit more up to code (pun not intended). 

I will move this littleosbook to its own branch, and refactor the codebase to more closely resemble the OSDev wiki, starting with Barebones tutorial. This means rewriting the Makefile to use the Cross Compiler, rewriting the Assembly code, and actually understanding how the Linker works. 

Currently I am merging what I already wrote with the better structured code from Meaty Bones on OSDev.

# Cross Compiling
For HippOS, I started out with a i386 architecture, following the OSDev tutorial series. However, as the project grows, as all things must, I have decided to port everything over to x86_64, while (hopefully) keep compatibility with the existing 32 bit code.

Most of the changes lay in kernel/Makefile and build scripts, however these are reversible.

One notable thing to check is to follow this tutorial - I have no idea why this isn't linked any where else, and is only available upon a Google search (truncated error)

https://osdev.wiki/wiki/Building_libgcc_for_mcmodel%3Dkernel