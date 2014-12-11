/* $Id: h8s-2633evb.cmd,v 1.1.1.1 2002/03/19 02:53:13 bgat Exp $ */
/* 
  linker script for evb by Petr Ledvina, 
  ledvinap@kae.zcu.cz, 2001/10	
*/
OUTPUT_FORMAT("coff-h8300")

OUTPUT_ARCH(h8300s)
INPUT(libc.a libg.a libgcc.a libc.a libgcc.a)
ENTRY(_start)

/* use memory at the and of available space */
/* 6K is used by evb monitor */
MEMORY {
	VECTORS : ORIGIN = 0x200000	 , LENGTH = 512
	RAM     : ORIGIN = 0x200000+230K , LENGTH = 20K
}


SECTIONS
{
  .vectors			: {
			*(.vects)
			  } > VECTORS

  .text				: { *(.text) } >RAM
  .rodata 			: { *(.rodata) *(.strings)} >RAM
  .data	 			: { __s_data = .; *(.data) __se_data = .;} >RAM				
  .bss				: { __s_bss = .; 
				    *(.regfile) _gdb_register_file_end = .;
			            *(.bss)
			            *(COMMON) 
				    __se_bss = .; __s_end = .;
				  } >RAM
  .stack(NOLOAD) 		: { __s_stack = .; *(.stack) __se_stack = .; } >RAM
}


