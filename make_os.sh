# 1. Собираем стартовый код
nasm -f elf32 boot/bootloader.asm -o start.o

# 2. Компилируем ядро на C
gcc -m32 -ffreestanding -c kernel/main.c      -o kernel.o
gcc -m32 -ffreestanding -c kernel/keyboard.c  -o keyboard.o
gcc -m32 -ffreestanding -c kernel/functions.c -o functions.o
gcc -m32 -ffreestanding -c kernel/terminal.c  -o terminal.o

# 3. Компонуем всё в ELF-файл. Обрати внимание на порядок: start.o должен быть первым.
gcc -m32 -ffreestanding -T linker.ld -o myos.elf -nostdlib start.o kernel.o keyboard.o functions.o terminal.o -lgcc
mkdir -p iso/boot/grub
cp myos.elf iso/boot/
grub-mkrescue -o os.iso iso/
