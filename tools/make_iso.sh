#!/bin/bash
set -e

# Create a minimal directory structure for GRUB
mkdir -p isodir/boot/grub

# Copy the kernel binary
cp build/andromache.bin isodir/boot/andromache.bin

# Copy the GRUB configuration
cp boot/grub/grub.cfg isodir/boot/grub/grub.cfg

# Generate the ISO
if command -v grub-mkrescue &> /dev/null; then
    grub-mkrescue -o andromache.iso isodir
    echo "ISO created: andromache.iso"
else
    echo "grub-mkrescue not found. Skipping ISO creation."
    # Fallback or error handling
fi
