#!/bin/sh
set -e
. ./iso.sh
export HOST=${HOST:-$(./default-host.sh)}

qemu-system-$(./target-triplet-to-arch.sh $HOST) -serial stdio -m 2G -cdrom hippos.iso