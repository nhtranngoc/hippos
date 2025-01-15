#!/bin/sh
set -e
. ./iso.sh
export HOST=${HOST:-$(./default-host.sh)}

if [ "$1" = "debug" ];
then
    qemu-system-$(./target-triplet-to-arch.sh $HOST) -serial stdio -m 2G -S -s -cdrom hippos.iso
else
    qemu-system-$(./target-triplet-to-arch.sh $HOST) -serial stdio -m 2G -cdrom hippos.iso
fi