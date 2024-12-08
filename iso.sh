#!/bin/sh
set -e

if [! -d "limine" ]; then
      echo "limine does not exist."
        # Download the latest Limine binary release for the 8.x branch.
      git clone https://github.com/limine-bootloader/limine.git --branch=v8.x-binary --depth=1

      # Build "limine" utility.
      make -C limine
fi

# Create a directory which will be our ISO root.
mkdir -p isodir

# Copy the relevant files over.
mkdir -p isodir/boot
cp -v kernel/hippos.kernel isodir/boot/
mkdir -p isodir/boot/limine
cp -v limine.conf limine/limine-bios.sys limine/limine-bios-cd.bin \
      limine/limine-uefi-cd.bin isodir/boot/limine/

# Create the EFI boot tree and copy Limine's EFI executables over.
mkdir -p isodir/EFI/BOOT
cp -v limine/BOOTX64.EFI isodir/EFI/BOOT/
cp -v limine/BOOTIA32.EFI isodir/EFI/BOOT/

# Create the bootable ISO.
xorriso -as mkisofs -R -r -J -b boot/limine/limine-bios-cd.bin \
        -no-emul-boot -boot-load-size 4 -boot-info-table -hfsplus \
        -apm-block-size 2048 --efi-boot boot/limine/limine-uefi-cd.bin \
        -efi-boot-part --efi-boot-image --protective-msdos-label \
        isodir -o hippos.iso

# Install Limine stage 1 and 2 for legacy BIOS boot.
./limine/limine bios-install hippos.iso