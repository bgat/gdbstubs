OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm",
	      "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_start)
SECTIONS
{
  . = 0x1820000;

 .text :
   {
   PROVIDE (__text_start__ = .);
   *(.text)
   PROVIDE (__rodata_start__ = .);
   *(.rodata)
   PROVIDE (__rodata_end__ = .);
   PROVIDE (__text_end__ = .);
   }

 . = 0x1000;

 .data :
   {
   PROVIDE (__data_start__ = .);
   PROVIDE (__data_start = .);
   *(.data)
   *(.sdata)
   PROVIDE (__data_end__ = .);
   PROVIDE (__data_end = .);
   }

 .bss :
   {
   PROVIDE (__bss_start__ = .);
   *(.bss)
   PROVIDE (__bss_end__ = .);
  }

 PROVIDE (_stack = 0x80000);
}
