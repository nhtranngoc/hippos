#!/bin/sh
set -e
. ./build.sh

mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub

cp sysroot/boot/hippos.kernel isodir/boot/hippos.kernel
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "hippOS" {
	multiboot /boot/hippos.kernel
}
EOF
grub-mkrescue -o hippos.iso isodir