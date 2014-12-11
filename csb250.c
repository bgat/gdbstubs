/* 
  Copyright (c) 2003 by      William A. Gatliff
  All rights reserved.      bgat@billgatliff.com

  See the file COPYING for details.

  This file is provided "as-is", and without any express or implied
  warranties, including, without limitation, the implied warranties of
  merchantability and fitness for a particular purpose.

  The author welcomes feedback regarding this file.

  **

  $Id: csb250.c,v 1.1 2003/09/13 04:39:33 bgat Exp $
*/


void mips_csb250_startup (void)
{
}

int gdb_putc (int c)
{
  return 0;
}

int gdb_getc (void)
{
  return 0;
}

void gdb_kill (void)
{
}

void gdb_detach (void)
{
}

#if defined(CRT0)

int main (void)
{
  return 0;
}

#endif
