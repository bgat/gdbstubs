/*
  Copyright (c) 1999 by      William A. Gatliff
  All rights reserved.     bgat@open-widgets.com

  See the file COPYING for details.

  This file is provided "as-is", and without any express
  or implied warranties, including, without limitation,
  the implied warranties of merchantability and fitness
  for a particular purpose.

  The author welcomes feedback regarding this file.
*/

/* $Id: sh2-7045edk.cmd,v 1.1.1.1 2002/03/19 02:53:13 bgat Exp $ */

OUTPUT_FORMAT( /* "coff-sh" */ "srec" )


OUTPUT_ARCH(sh)
INPUT(libc.a libg.a libgcc.a libc.a libgcc.a)
ENTRY(_start)

MEMORY
{
  vect  : o = 0, l = 1k
  rom   : o = 0x400, l = 127k
  ram   : o = 0x400000, l = 128k
  cache : o = 0xfffff000, l = 4k
}


SECTIONS
{
  /* the interrupt vector table */
  .vect :
  {
    __vect_start = .;
    *(.vect);
    __vect_end = .;
  } > vect
  
  /* code and constants */
  .text :
  {
    __text_start = .;
    *(.text)
    *(.strings)
     __text_end = .;
  }  > rom 
  
  /* uninitialized data */
  .bss :
  {
     __bss_start = . ; 
    *(.bss)
    *(COMMON)
     __bss_end = . ;  
  }  > ram
  
  /* initialized data section
     ld will load this at __text_end in the output file,
     but will actually locate it in ram.  By doing a
     memcpy( &_data_start, &_text_end, (&_data_end - &_data_start);
     during startup, we initialize our nonzero globals . */
  .init : AT (__text_end)
  {
    __data_start = .;
    *(.data)
    *(.regfile)
    __data_end = .;
  } > ram

  /* application stack */
  .stack :
  {
     __stack_start = .; 
    *(.stack)
     __stack_end = .;
  }  > cache
}


