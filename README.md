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