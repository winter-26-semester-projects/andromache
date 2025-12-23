#!usr/bin/env

rm -rf build/
mkdir -p build

cd ./build/
cmake ..
make

cd ..

mkdir -p isodir/boot/grub/
cp build/kernel.elf isodir/boot/
cp include/boot/grub.cfg isodir/boot/grub/grub.cfg

grub-mkrescue -o andromache.iso isodir