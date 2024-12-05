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