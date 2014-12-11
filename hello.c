/* mips-elf-gcc -g -o hello hello.c -mips1 -mno-crt0 -T idt.ld */ 

#include <stdio.h>

int main (void)
{
  printf("Hello, world!\n");
  __asm__("break");
  return 0;
}
